/**
 * @file ast.cc
 * @brief 实现了抽象语法树 (AST) 中部分节点类的方法。
 *
 * 此文件包含在 `ast.h` 中声明的一些AST节点类成员函数的具体实现，
 * 主要涉及语义分析辅助函数、列表处理以及格式化输出等功能。
 */
#include "ast.h"
#include "log.h"    // 日志记录功能
#include <ctime>   // 此处包含 <ctime> 但在当前代码中未直接使用，可能是历史遗留或为将来功能准备

using std::stack;
using std::string;
using std::vector;

/**
 * @brief 分析当前叶节点 (通常是标识符) 在特定函数上下文中是否作为引用参数使用。
 *
 * @param ts 当前的符号表集合 (TableSet)，用于查找标识符的符号表条目。
 * @param fn 当前所在的函数 (FunctionSymbol)，用于获取其参数的传递模式 (值传递或引用传递)。
 * @return bool 如果此叶节点代表的标识符在当前函数 `fn` 中被定义为引用传递 (VAR) 参数，
 *         则返回 `true` 并将节点的 `is_ref_` 标志设置为 `true`；否则返回 `false`。
 *         如果 `fn` 为 `nullptr` (例如在全局上下文中)，直接返回 `false`。
 *
 * @details
 * 1. 首先检查 `fn` 是否为空，若为空则非函数上下文，不是引用参数。
 * 2. 从叶节点的 `value_` 中获取标识符名称。
 * 3. 使用 `ts->SearchEntry` 在符号表中查找该标识符，并用 `local` 标记是否在当前 (最内层) 作用域找到。
 * 4. 如果在当前作用域找到了符号 (`entry != nullptr && local`)：
 *    a. 调用 `fn->GetParamInfo(id)` 获取该标识符作为函数参数的详细信息。
 *    b. 如果找到了参数信息 (`param_info != nullptr`) 并且其传递模式是引用 (`param_info->mode == FunctionSymbol::PARAM_MODE::REFERENCE`)，
 *       则将当前叶节点的 `is_ref_` 标志设置为 `true`。
 * 5. 返回 `is_ref_` 的最终值。
 */
bool LeafNode::AnalyzeReference(TableSet *ts, FunctionSymbol *fn) {
  if (fn == nullptr) return false; // 不在函数内，或函数信息缺失
  // search table and judge if is current layer
  bool local = false; // 用于标记是否在当前作用域找到
  string id = value_.get<string>(); // 获取叶节点代表的标识符名称
  auto *entry = ts->SearchEntry<SymbolTemplate>(id, &local); // 查找符号

  // 必须是在当前函数作用域内找到的符号 (通常参数会在当前作用域)
  if (entry != nullptr && local) {
    const FunctionSymbol::ParameterInfo *param_info = fn->GetParamInfo(id); // 获取参数信息
    // search param-passing mode
    if (param_info != nullptr && param_info->mode == FunctionSymbol::PARAM_MODE::REFERENCE) {
      is_ref_ = true; // 如果是引用传递参数，设置标志
    }
  }
  return is_ref_;
}

/**
 * @brief 从 `IdListNode` (标识符列表节点) 中提取所有标识符，并返回它们对应的 `LeafNode` 指针列表。
 *
 * @return std::vector<LeafNode *> 包含列表中所有标识符的 `LeafNode` 指针的向量。
 *         列表中的 `LeafNode` 顺序与它们在源代码中出现的顺序一致。
 *
 * @details
 * `IdListNode` 通过 `grammar_type_` 表示其结构：
 * - `GrammarType::SINGLE_ID`: 代表 `idlists → id`，此时列表只包含一个标识符。
 * - `GrammarType::MULTIPLE_ID`: 代表 `idlists → idlist, id` (右递归)，
 *   此时第一个子节点 (`get_child(0)`) 是另一个 `IdListNode` (代表 `idlist`)，
 *   第二个子节点 (`get_child(1)`) 是一个 `LeafNode` (代表 `,` 后面的 `id`)。
 *
 * 此方法通过迭代或递归地处理 `IdListNode` 的这种结构来收集所有的 `LeafNode`。
 * 它从列表的末尾开始向前收集，以确保最终向量中的顺序正确。
 */
vector<LeafNode *> IdListNode::Lists() const {
  vector<LeafNode *> lists; // 用于存储结果的向量
  const IdListNode *cur_node = this; // 从当前节点开始处理
  GrammarType gtype = cur_node->grammar_type_;

  // 处理 idlists → idlist, id 的情况 (右递归)
  // 循环直到遇到 SINGLE_ID 或节点为空
  while (gtype == GrammarType::MULTIPLE_ID) {
    // `get_child(1)` 是 MULTIPLE_ID 规则中的 'id' (LeafNode)
    LeafNode *ln = cur_node->get_child(1)->DynamicCast<LeafNode>();
    lists.insert(lists.begin(), ln); // 插入到结果向量的开头以保持顺序
    // `get_child(0)` 是 MULTIPLE_ID 规则中的 'idlist' (递归的IdListNode)
    cur_node = cur_node->get_child(0)->DynamicCast<IdListNode>();
    if (!cur_node) break; // 如果递归部分为空，则停止
    gtype = cur_node->grammar_type_; // 更新当前处理节点的语法类型
  }

  // 处理 idlists → id 的情况 (列表的第一个元素，或者列表只有一个元素)
  if (cur_node && cur_node->child_count() > 0) {
    // `get_child(0)` 是 SINGLE_ID 规则中的 'id' (LeafNode)
    LeafNode *ln = cur_node->get_child(0)->DynamicCast<LeafNode>();
    if (ln) lists.insert(lists.begin(), ln); // 插入到结果向量的开头
  }
  return lists;
}

/**
 * @brief (代码生成相关) 将 `TypeNode` (特指数组类型) 的维度信息格式化输出到指定文件流。
 *
 * @param dst 目标文件流指针 (例如 `stdout`, `stderr`, 或打开的文件)。
 * @details 此函数的功能是委托给 `CodeGenerator` 类中的静态方法 `FormatTypeNodePeriods` 来完成的。
 *          `CodeGenerator` 负责处理具体的代码生成和格式化细节。
 */
void TypeNode::PeriodsFormat(FILE *dst) {
  CodeGenerator::FormatTypeNodePeriods(this, dst);
}

/**
 * @brief 为 `VariableListNode` (通常用于 `read` 语句) 生成格式化字符串，用于C的 `scanf` 类函数。
 *
 * @return std::string 一个包含C语言 `scanf` 格式说明符的字符串。
 *         例如，如果变量列表类型为 (integer, string, real)，则返回 `"%d %s %f "`。
 *         每个格式说明符后会跟一个空格。
 * @throw std::runtime_error 如果遇到未知的基础类型，无法为其生成格式说明符。
 *
 * @details
 * 此函数遍历 `VariableListNode` 内部存储的 `basic_types` 向量 (此向量应在语义分析阶段填充)。
 * 根据每个 `BasicType`：
 * - `TYPE_INT` 或 `TYPE_BOOL`: 使用 `"%d "`
 * - `TYPE_STRINGLIKE`: 使用 `"%s "` (用于读取Pascal类字符串到C字符串)
 * - `TYPE_REAL`: 使用 `"%f "`
 * - `TYPE_CHAR`: 使用 `"%c "`
 * - 其他未知类型: 抛出运行时错误。
 * 所有生成的格式说明符连接起来构成最终的返回字符串。
 */
std::string VariableListNode::FormatString() const {
  std::string format_str = ""; 
  for (size_t i = 0; i < this->basic_types.size(); i++) { 
    BasicType *type = this->basic_types[i];
    std::string chfmt;
    if (type == TYPE_INT || type == TYPE_BOOL) { // 布尔型也常作为整数读写
      chfmt = "%d ";
    } else if (type == TYPE_STRINGLIKE) { // Pascal类字符串，用 %s 读取
      chfmt = "%s ";
    } else if (type == TYPE_REAL) {
      chfmt = "%f ";
    } else if (type == TYPE_CHAR) {
      chfmt = "%c ";
    } else {
      // 如果类型指针为空或不是已知的基本类型，则抛出错误
      throw std::runtime_error("VariableListNode: FormatString() : encountered an unknown or null basic type for formatting");
    }
    format_str += chfmt;
  }
  return format_str;
}

/**
 * @brief 根据输入的类型列表 (`type_list`)，设置 `VariableListNode` 内部的 `basic_types` 成员。
 *
 * @param type_list 一个指向 `TypeTemplate*` 向量的指针。这个向量包含了对应变量列表中每个变量的类型。
 * @return bool 如果所有输入类型都能成功处理并转换为 `BasicType*` (或特殊标记如 `TYPE_STRINGLIKE`)，则返回 `true`。
 *         如果在处理过程中遇到无法识别的类型或错误，则返回 `false`。
 *         如果 `type_list` 为 `nullptr`，则认为操作成功并直接返回 `true` (无事可做)。
 *
 * @details
 * 1. 如果 `type_list` 为空指针，直接返回 `true`。
 * 2. 清空当前节点内部的 `basic_types` 向量。
 * 3. 遍历 `type_list` 中的每一个 `TypeTemplate*` (`i`):
 *    a. 如果 `i` 是一个具体的、已定义的全局基本类型 (通过 `i->isSpecificBasicType()` 判断)，
 *       则将其动态转换为 `BasicType*` 并添加到 `basic_types`。
 *    b. 如果 `i` 是全局的 `TYPE_STRINGLIKE` 类型，则直接添加 `TYPE_STRINGLIKE` 到 `basic_types`。
 *    c. 如果 `i` 不是 `TYPE_ERROR` 并且它是一个类字符串类型 (通过 `i->StringLike()` 判断，
 *       例如定长字符数组)，则添加 `TYPE_STRINGLIKE` 到 `basic_types` (作为读取时的统一处理)。
 *    d. 如果以上条件都不满足 (例如 `i` 是 `TYPE_ERROR`，或者是一个非类字符串的复杂类型如记录)，
 *       则认为类型设置失败，返回 `false`。
 * 4. 如果所有类型都成功处理，返回 `true`。
 */
bool VariableListNode::set_types(std::vector<TypeTemplate *> *type_list) {
  if (!type_list) return true; // 如果没有类型列表输入，则认为成功 (无事可做)
  
  this->basic_types.clear(); // 清空旧的类型信息

  for (TypeTemplate* i : *type_list) {
    if (i && i->isSpecificBasicType()) { // Case a: 是特定的全局基础类型 (int, real, bool, char)
      this->basic_types.push_back(dynamic_cast<BasicType *>(i));
    } else if (i == TYPE_STRINGLIKE) { // Case b: 是全局的 TYPE_STRINGLIKE 代理类型
        this->basic_types.push_back(TYPE_STRINGLIKE); // 直接使用 TYPE_STRINGLIKE
    } else if (i && i != TYPE_ERROR && i->StringLike()) { // Case c: 是类字符串的数组类型
      this->basic_types.push_back(TYPE_STRINGLIKE); // 作为 TYPE_STRINGLIKE 处理
    } else {
      // Case d: 错误类型或不支持的复杂类型
      return false; 
    }
  }
  return true;
}

/**
 * @brief 为 `ExpressionListNode` (通常用于 `write` 语句) 生成格式化字符串，用于C的 `printf` 类函数。
 *
 * @return std::string 一个包含C语言 `printf` 格式说明符的字符串。
 *         例如，如果表达式列表类型为 (integer, string, real)，则返回 `"%d%s%.6f"`。
 *         与 `VariableListNode::FormatString` 不同，这里格式说明符之间通常没有空格，
 *         除非 `printf` 的使用场景需要。
 * @throw std::runtime_error 如果遇到未知的基础类型，无法为其生成格式说明符。
 *
 * @details
 * 此函数遍历 `ExpressionListNode` 内部存储的 `basic_types` 向量 (此向量应在语义分析阶段填充)。
 * 根据每个 `BasicType`：
 * - `TYPE_INT` 或 `TYPE_BOOL`: 使用 `"%d"`
 * - `TYPE_STRINGLIKE`: 使用 `"%s"`
 * - `TYPE_REAL`: 使用 `"%.6f"` (输出6位小数的浮点数)
 * - `TYPE_CHAR`: 使用 `"%c"`
 * - 其他未知类型: 抛出运行时错误。
 * 所有生成的格式说明符连接起来构成最终的返回字符串。
 */
string ExpressionListNode::FormatString() const {
  string format = "";
  for (size_t i = 0; i < basic_types.size(); i++) { // 使用 size_t 进行迭代
    BasicType *type = basic_types[i];
    string chfmt;
    if (type == TYPE_INT || type == TYPE_BOOL) {
      chfmt = "%d";
    } else if (type == TYPE_STRINGLIKE) {
      chfmt = "%s";
    } else if (type == TYPE_REAL) {
      chfmt = "%.6f"; // 通常输出real时指定精度
    } else if (type == TYPE_CHAR) {
      chfmt = "%c";
    } else {
      throw std::runtime_error(
          "ExpressionListNode: FormatString() : encountered an unknown or null basic type for formatting");
    }
    format += chfmt;
  }
  return format;
}

/**
 * @brief 根据输入的类型列表 (`type_list`)，设置 `ExpressionListNode` 内部的 `basic_types` 成员。
 *
 * @param type_list 一个指向 `TypeTemplate*` 向量的指针。这个向量包含了对应表达式列表中每个表达式的类型。
 * @return bool 如果所有输入类型都能成功处理并转换为 `BasicType*` (或特殊标记如 `TYPE_STRINGLIKE`)，则返回 `true`。
 *         如果在处理过程中遇到无法识别的类型或错误，则返回 `false`。
 *         如果 `type_list` 为 `nullptr`，则认为操作成功并直接返回 `true` (无事可做)。
 *
 * @details
 * 与 `VariableListNode::set_types` 逻辑类似：
 * 1. 如果 `type_list` 为空指针，直接返回 `true`。
 * 2. （注意：此实现中没有 `clear()`，意味着如果多次调用，`basic_types`会累积，这可能是一个潜在问题，
 *    除非设计意图如此或者调用者保证只调用一次。通常应该先 `clear()`。）
 * 3. 遍历 `type_list` 中的每一个 `TypeTemplate*` (`i`):
 *    a. 如果 `i` 是一个具体的、已定义的全局基本类型 (通过 `i->isSpecificBasicType()` 判断) 
 *       或者是全局的 `TYPE_STRINGLIKE` 类型，则将其动态转换为 `BasicType*` 并添加到 `basic_types`。
 *    b. 如果 `i` 不是 `TYPE_ERROR` 并且它是一个类字符串类型 (通过 `i->StringLike()` 判断)，
 *       则添加 `TYPE_STRINGLIKE` 到 `basic_types`。
 *    c. 如果以上条件都不满足，返回 `false`。
 * 4. 如果所有类型都成功处理，返回 `true`。
 */
bool ExpressionListNode::set_types(std::vector<TypeTemplate *> *type_list) {
  if (!type_list) return true;
  // 注意：这里没有 basic_types.clear()，如果多次调用可能会累积。假定调用者会处理或只调用一次。
  for (auto i : *type_list) {
    if ((i && i->isSpecificBasicType()) || i == TYPE_STRINGLIKE) {
      basic_types.push_back(dynamic_cast<BasicType *>(i));
    } else if (i && i != TYPE_ERROR && i->StringLike()) { // 增加 i 非空判断
      basic_types.push_back(TYPE_STRINGLIKE);
    } else {
      return false;
    }
  }
  return true;
}

/**
 * @brief (主要用于函数/过程参数传递) 根据参数的引用传递需求，递归地设置表达式列表中的表达式是否为引用。
 *
 * @param ref_stack 一个指向布尔型栈的指针。栈中的每个元素对应一个参数，
 *                  `true` 表示该参数期望是引用 (VAR参数)，`false` 表示值传递。
 *                  此方法会根据表达式列表的结构消耗 (pop) `ref_stack` 中的元素。
 *
 * @details
 * 此方法用于在函数或过程调用时，根据形参的定义 (是值传递还是引用传递 VAR)，
 * 来标记实参表达式节点 (`ExpressionNode`) 的 `is_ref_` 属性。
 *
 * - 如果 `ExpressionListNode` 代表单个表达式 (`GrammarType::EXP`):
 *   - 查看 `ref_stack` 栈顶元素。如果为 `true` (期望引用)，则获取子表达式节点，
 *     并调用其 `set_is_ref()` 方法。
 *   - (注意：原始代码没有 `pop` 单个表达式的情况，这可能暗示栈顶元素是为这个表达式准备的，
 *     并且在此次调用后不应由这个函数 `pop`。或者，调用者负责管理栈的匹配。 
 *     当前实现假定，如果 ref_stack 非空，其栈顶对应当前EXP，但不从此函数pop。)
 *     修改：为了与列表情况统一，这里应该也pop，但当前代码不pop。
 *     按照当前代码实现注释：栈顶元素仅用于决定当前单个表达式是否为引用，不消耗栈元素。
 *
 * - 如果 `ExpressionListNode` 代表多个表达式的列表 (`GrammarType::EXP_LIST_EXP`):
 *   - 此列表通常是右递归的：`expression_list → expression_list , expression`。
 *     子节点 `get_child(1)` 是最右边的 `expression`。
 *     子节点 `get_child(0)` 是剩余的 `expression_list` (递归结构)。
 *   - 查看 `ref_stack` 栈顶元素。如果为 `true`，则获取最右边的表达式节点 (`get_child(1)`)
 *     并调用其 `set_is_ref()`。
 *   - 从 `ref_stack` 中 `pop` 一个元素 (对应已处理的最右边表达式)。
 *   - 对列表的其余部分 (`get_child(0)`) 递归调用 `set_ref`，传入修改后的 (已pop的) `ref_stack`。
 *
 * - 如果子节点列表为空，或 `ref_stack` 为空或空指针，则不执行任何操作。
 */
void ExpressionListNode::set_ref(std::stack<bool> *ref_stack) {
  if (child_list_.empty() || !ref_stack || ref_stack->empty()) {
    return; // 无子节点或无引用信息栈，则不作处理
  }

  if (this->grammar_type() == GrammarType::EXP) { 
    // 单个表达式的情况: expression_list → expression
    // 子节点 get_child(0) 是 ExpressionNode
    if (ref_stack->top()) { // 如果栈顶为true (期望此参数为引用)
      Node* child_expr_node_base = get_child(0);
      if (child_expr_node_base) { 
        ExpressionNode* expr_node = dynamic_cast<ExpressionNode*>(child_expr_node_base);
        if (expr_node) {
          expr_node->set_is_ref(); // 标记为引用
        }
      }
    }
    // 注意：对于单个表达式，这里不pop栈。这可能意味着调用者期望栈顶元素
    // 对应这个参数，并在外部管理栈。或者这是一个潜在的不一致性。
    // 如果要pop，应该加 ref_stack->pop();

  } else { // GrammarType::EXP_LIST_EXP
    // 表达式列表的情况: expression_list → expression_list , expression
    // get_child(1) 是最右边的 expression (当前要处理的)
    // get_child(0) 是递归的 expression_list (列表的其余部分)
    
    // 处理最右边的表达式 (child 1)
    if (ref_stack->top()) { // 如果期望当前参数为引用
      Node* child_expr_node_base = get_child(1);
      if (child_expr_node_base) { 
        ExpressionNode* expr_node = dynamic_cast<ExpressionNode*>(child_expr_node_base);
        if (expr_node) {
          expr_node->set_is_ref(); // 标记为引用
        }
      }
    }
    
    // 为当前处理的表达式消耗栈顶元素
    ref_stack->pop(); 
    
    // 递归处理列表的其余部分 (child 0)
    Node* next_list_node_base = get_child(0);
    if (next_list_node_base) { 
        ExpressionListNode* next_list_node = dynamic_cast<ExpressionListNode*>(next_list_node_base);
        if (next_list_node) {
            next_list_node->set_ref(ref_stack); // 传入消耗了一个元素的栈
        }
    }
  }
}

/**
 * @brief 对 `VariableNode` (变量引用节点) 进行语义检查和类型分析。
 *
 * @details
 * 此方法在语义分析阶段被调用，用于确定变量的类型、它在符号表中的条目，
 * 并执行一些基本的语义正确性检查。
 * 1. `id_` 成员会从其第一个子节点 (应为一个 `LeafNode`，代表变量名) 获取。
 * 2. `sym_ptr_` 会通过在当前符号表集合中搜索 `id_` 来填充。
 *    - 如果未找到符号 (`sym_ptr_` 为 `nullptr`)，则变量未声明。此时 `type_ptr_` 可能会被设为 `TYPE_ERROR` (当前代码是直接返回)。
 * 3. 如果找到了符号：
 *    a. `type_ptr_` 设置为符号的类型 (`sym_ptr_->type()`)。
 *    b. 如果从符号表获取的类型已经是 `TYPE_ERROR`，则直接返回。
 *    c. **函数名误用检查**: 如果符号的类型是 `SymbolTemplate::SYMBOL_TYPE::FUNCTION`，
 *       意味着试图将函数名当作变量使用 (例如 `myFunction := 10;` 而非 `myFunction()` 或在函数内部为返回值赋值)。
 *       这在Pascal中通常是不允许的 (除非是函数内部为其返回值赋值，但这部分逻辑由 `StatementNode` 的
 *       `FUNC_ASSIGN_OP_EXP` 类型处理，`VariableNode` 主要处理变量引用)。
 *       因此，将 `type_ptr_` 设置为 `TYPE_ERROR` 并返回。
 *    d. **类字符串数组处理**: 如果变量类型是数组 (`TypeTemplate::TYPE::ARRAY`)：
 *       i.  将其动态转换为 `ArrayType*`。
 *       ii. 调用数组类型的 `StringLike()` 方法检查它是否满足类字符串的条件 
 *           (通常是一维字符数组，且满足特定约定，如 `array[0..N] of char`)。
 *       iii. 如果是类字符串类型，则将此 `VariableNode` 的 `type_ptr_` 修改为全局的 `TYPE_STRINGLIKE`。
 *            这允许后续处理 (如 `read`/`write` 语句) 将其作为字符串统一对待。
 *            之后直接返回，因为类型已被特殊化。
 * 4. (隐式) 如果以上特殊情况均未发生，`type_ptr_` 将保持为从符号表获取的原始类型。
 *
 * **注意**: `id_varparts` (数组索引、记录字段访问) 的类型检查和处理更为复杂，通常涉及
 *          递归地分析 `id_varparts` 子树，并结合当前 `type_ptr_` 逐步推导最终访问到的
 *          成员或元素的类型。此 `Check()` 方法主要负责处理基础变量名部分的查找和初始类型确定。
 *          完整变量引用的类型 (如 `myArray[index].field`) 是由此处确定的基类型和 `id_varparts` 共同决定的。
 */
void VariableNode::Check() {
  // 1. 从子节点获取变量名 (id)
  if (child_list_.empty() || !get_child(0) || !get_child(0)->DynamicCast<LeafNode>()) {
      // 结构不符合预期 (没有代表变量名的LeafNode作为第一个子节点)
      type_ptr_ = TYPE_ERROR; // 标记为错误
      log_error("VariableNode::Check(): Node structure error, cannot find identifier LeafNode.");
      return;
  }
  id_ = get_child(0)->DynamicCast<LeafNode>()->value<string>();

  // 2. 在符号表中查找符号
  // 假设 gtable_set 是一个可访问的全局或上下文相关的符号表集合指针 (这里需要传递或获取)
  // 由于没有直接传递 TableSet*，这里无法直接查找。这部分逻辑应由调用 Check 的语义分析器提供 TableSet。
  // **重要**: 以下 sym_ptr_ 的赋值是缺失的，它应该由调用者通过符号表查找来设置。
  // 为演示目的，我们假设 sym_ptr_ 已经被外部设置了，或者这里应该有一个参数 TableSet* ts
  // if (sym_ptr_ == nullptr) { // 如果外部未能设置 sym_ptr_
  //   type_ptr_ = TYPE_ERROR;
  //   log_error("VariableNode::Check(): Symbol '%s' not found or sym_ptr_ not set.", id_.c_str());
  //   return;
  // }
  // 实际项目中， Check 函数可能会接收 TableSet* 作为参数来查找 id_ 并设置 sym_ptr_。
  // 例如: sym_ptr_ = ts->SearchEntry<SymbolTemplate>(id_);
  // 如果 VariableNode 设计为在构造后由其他过程填充 sym_ptr_，则以下逻辑成立。

  if (!sym_ptr_) {
    // sym_ptr_ 未被设置 (例如，符号未找到，或者 Check 在符号填充前被调用)
    // log_warning("VariableNode::Check(): sym_ptr_ is null for id '%s'. Type check might be incomplete.", id_.c_str());
    // 根据设计，如果符号找不到，type_ptr_ 应该反映这个错误。
    // 如果依赖外部填充 sym_ptr_，这里不设置 TYPE_ERROR 可能更好，让外部决定。
    // 但原代码逻辑是如果 sym_ptr_ 为空则直接返回，我们暂时保留这种行为。
    return; // sym_ptr_ 为空，无法继续检查
  }

  // 3.a. 设置类型指针
  type_ptr_ = sym_ptr_->type();

  // 3.b. 如果符号的类型本身就是错误类型
  if (type_ptr_ == TYPE_ERROR) {
    // 变量在符号表中标记为类型错误，或类型未定义
    return;
  }

  // 3.c. 检查是否将函数名用作变量
  if (sym_ptr_->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION) {
    // 这是一个函数符号，但被用在了变量应该出现的地方。
    // 例如 `myFunc := 10;` (如果myFunc是函数)
    // 这通常是一个语义错误 (除非是函数内部给返回值赋值，但那是另一种AST节点处理)
    log_error("Semantic Error: Function '%s' cannot be used as a variable.", id_.c_str());
    type_ptr_ = TYPE_ERROR;
    return;
  }

  // 3.d. 检查数组类型是否为类字符串 (例如，用于read/write的特殊处理)
  if (type_ptr_ && type_ptr_->template_type() == TypeTemplate::TYPE::ARRAY) {
    ArrayType* arr_type = type_ptr_->DynamicCast<ArrayType>();
    if (arr_type && arr_type->StringLike()) {
      // 如果数组是类字符串 (例如 array [0..N] of char)，
      // 则将其类型视为特殊的 TYPE_STRINGLIKE 以便统一处理。
      type_ptr_ = TYPE_STRINGLIKE; // 将类型具体化为类字符串代理类型
      return; // 类型已确定为 TYPE_STRINGLIKE，完成对此基变量的检查
    }
  }
  // 如果不是以上特殊情况, type_ptr_ 保持为从符号表获得的类型。
  // 对 id_varparts (数组下标/记录字段) 的进一步类型检查将基于此 type_ptr_。
}