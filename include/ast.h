/**
 * @file ast.h
 * @brief 定义了抽象语法树 (Abstract Syntax Tree, AST) 的所有节点类型。
 *
 * AST 是源代码在语法分析阶段后生成的树状结构表示。每个节点代表源代码中的一个构造，
 * 例如程序、声明、语句、表达式等。这个文件定义了AST节点的基类 `Node`，
 * 以及代表各种Pascal语言构造的特定节点类型。
 */
#ifndef P2C-Transpiler_AST_H
#define P2C-Transpiler_AST_H

#include <iostream>
#include <stack>
#include <vector>

#include "type.h"           // 类型系统定义
#include "code_generation.h" // 代码生成相关定义
#include "symbol_table.h"   // 符号表定义

/* **************** basic class **************** */
/**
 * @brief AST节点的抽象基类。
 *
 * 所有具体的AST节点都从此类派生。它提供了管理子节点和父节点的基本功能。
 * 每个节点可以有零个或多个子节点，形成树状结构。
 */
class Node {
 public:
  /**
   * @brief 默认构造函数。
   * @details 初始化父指针为 `nullptr`。
   */
  Node() : parent_(nullptr){};

  /**
   * @brief 虚析构函数。
   * @details 递归删除所有子节点，防止内存泄漏。
   */
  virtual ~Node() {
    for (auto child : child_list_) {
      delete child;
    }
  };

  /**
   * @brief 动态类型转换的辅助函数。
   * @tparam T 目标节点的具体类型。
   * @return 如果当前节点可以安全地转换为类型 `T`，则返回转换后的指针；否则返回 `nullptr`。
   * @example `IfNode* if_node = current_node->DynamicCast<IfNode>();`
   */
  template <typename T>
  T *DynamicCast() {
    return dynamic_cast<T *>(this);
  }

  // getter and setter
  /**
   * @brief向当前节点添加一个子节点。
   * @param child 指向要添加的子节点的指针。
   * @note 子节点的所有权将转移给当前节点，并在当前节点析构时被删除。
   */
  void append_child(Node *child) { child_list_.emplace_back(child); }

  /**
   * @brief 获取指定位置的子节点。
   * @param pos 子节点在列表中的索引。如果为负数，则从列表末尾开始计数 (例如 -1 是最后一个子节点)。
   * @return 指向子节点的指针。如果索引无效，行为未定义 (可能导致越界访问)。
   * @deprecated 建议使用 `get_child(int index)` 以获得更安全的边界检查。
   */
  Node *get(int pos) {
    return child_list_[pos < 0 ? pos += child_list_.size() : pos];
  }

  /**
   * @brief 获取子节点的数量。
   * @return `size_t` 子节点的数量。
   */
  size_t child_count() const { return child_list_.size(); }

  /**
   * @brief 获取指定索引的子节点 (带边界检查)。
   * @param index 子节点的索引 (0-based)。如果为负数，则从列表末尾开始计数。
   * @return 指向子节点的指针。如果索引超出范围，返回 `nullptr`。
   */
  Node* get_child(int index) const {
    if (index < 0) index += child_list_.size();
    if (index < 0 || static_cast<size_t>(index) >= child_list_.size()) return nullptr; // 修正类型比较
    return child_list_[index];
  }

  /**
   * @brief 获取所有子节点的只读引用。
   * @return `const std::vector<Node*>&` 对子节点列表的常量引用。
   */
  const std::vector<Node*>& children() const {
    return child_list_;
  }

 protected:
  Node *parent_; ///< 指向父节点的指针。根节点的父指针为 `nullptr`。
  std::vector<Node *> child_list_; ///< 存储所有子节点的列表。
};

/**
 * @brief 代表整个抽象语法树 (AST)。
 * @details AST是源代码的树形表示。此类持有AST的根节点 (`root_`)，
 *          并管理一些全局信息，如代码生成器辅助对象 (`libs_`) 和树的有效性状态 (`valid_`)。
 * @example
 *   对于 Pascal 代码:
 *   ```pascal
 *   program Test;
 *   var
 *     a: integer;
 *   begin
 *     a := 1;
 *   end.
 *   ```
 *   一个 `AST` 对象会被创建。它的 `root_` 成员会指向代表整个程序的 `ProgramNode`。
 *   这棵树的大致结构如下 (简化版):
 *
 *   AST instance
 *     |
 *     +-> root_ (points to ProgramNode for "program Test; ... end.")
 *           |
 *           +-- ProgramHeadNode (for "program Test;")
 *           |     |
 *           |     +-- LeafNode (name: "Test")
 *           |
 *           +-- ProgramBodyNode (for "var a: integer; begin a := 1; end.")
 *                 |
 *                 +-- VariableDeclarationsNode (for "var a: integer;")
 *                 |     |
 *                 |     +-- VariableDeclarationNode (for "a: integer")
 *                 |           |
 *                 |           +-- IdListNode (name: "a") ---> LeafNode ("a")
 *                 |           |
 *                 |           +-- TypeNode (type: integer) ---> BasicTypeNode
 *                 |
 *                 +-- CompoundStatementNode (for "begin a := 1; end.")
 *                       |
 *                       +-- StatementListNode (for "a := 1;")
 *                             |
 *                             +-- StatementNode (Assignment: a := 1)
 *                                   |
 *                                   +-- VariableNode (variable "a") ---> LeafNode ("a")
 *                                   |
 *                                   +-- ExpressionNode (value 1) ---> ... ---> LeafNode (1)
 *
 *   `valid_` 标记AST是否有效。
 *   `libs_` 成员 (CodeGenerator类型) 可能用于管理编译过程中需要的库函数或代码生成相关的辅助信息。
 */
class AST {
 public:
  /**
   * @brief AST 析构函数。
   * @details 删除根节点 `root_`，这将触发整个AST中所有节点的递归删除。
   */
  ~AST() {
    log_debug("~AST(): delete ast");
    if (root_ != nullptr) delete root_;
  }

  /**
   * @brief 检查AST是否有效。
   * @return 如果AST被标记为有效且根节点不为空，则为 `true`；否则为 `false`。
   */
  bool Valid() const { return valid_ && (root_ != nullptr); }

  /**
   * @brief 设置AST的有效性状态。
   * @param v `true` 表示有效，`false` 表示无效。
   */
  void set_valid(bool v) { valid_ = v;}

  // getter and setter
  /**
   * @brief 获取AST的根节点。
   * @return 指向根节点 `Node` 的指针。
   */
  Node *root() const { return root_; }

  /**
   * @brief 获取代码生成器辅助对象。
   * @return 指向 `CodeGenerator` 对象的指针。
   * @note 返回的是 `const_cast` 转换后的指针，允许修改，需谨慎使用。
   */
  CodeGenerator *libs() const { return const_cast<CodeGenerator*>(&libs_); }

  /**
   * @brief 设置AST的根节点。
   * @param root 指向新的根节点的指针。
   * @note 如果之前已存在根节点，旧的根节点不会被自动删除，需要调用者负责管理其生命周期。
   */
  void set_root(Node *root) { root_ = root; }

 private:
  bool valid_ = false;         ///< 标记AST是否有效 (例如，语法分析是否成功)。
  Node *root_ = nullptr;      ///< 指向AST的根节点。
  CodeGenerator libs_;        ///< 代码生成器辅助对象，可能用于管理库函数或代码片段。
};

/* **************** inherit class **************** */
/**
 * @brief 叶节点 (Terminal Node)，代表源代码中的字面量、标识符等终结符。
 * @details 叶节点是AST中不再有子节点的节点。它们通常存储具体的值，
 *          如数字、字符串、字符或标识符名称。
 * @example
 *   在 `x := 10;` 中, `x` 和 `10` 都可以是 LeafNode。
 *   `value_` 存储具体的值，如 "x" (作为 `ConstValue` 类型存储的字符串) 或 10 (作为 `ConstValue` 类型存储的整数)。
 *   `is_ref_` 指示是否为引用（例如，在函数参数传递中，如果一个变量作为VAR参数被引用）。
 */
class LeafNode : public Node {
 public:
  /** @brief 默认构造函数。*/
  LeafNode() {}
  /**
   * @brief 使用 `ConstValue` 构造叶节点。
   * @param val 存储在叶节点中的常量值。
   */
  LeafNode(ConstValue val) : value_(val) {}

  // getter and setter
  /**
   * @brief 获取标识符的引用形式 (如果 `is_ref_` 为true，则添加解引用符号)。
   * @return `std::string` 表示标识符的字符串，例如 `"myVar"` 或 `"(*myVar)"`。
   * @note 仅当 `value_` 中存储的是字符串类型 (标识符名) 时有意义。
   */
  const std::string id_ref() const {
    return is_ref_ ? "(*" + value_.get<std::string>() + ")"
                   : value_.get<std::string>();
  }

  /**
   * @brief 设置叶节点存储的值。
   * @param value 新的 `ConstValue`。
   */
  void set_value(ConstValue value) { value_ = value; }

  /**
   * @brief 设置叶节点是否为引用。
   * @param ref `true` 表示是引用，`false` 表示不是。
   */
  void set_ref(bool ref) { is_ref_ = ref; }

  /**
   * @brief 获取叶节点存储的值 (模板方法)。
   * @tparam T期望获取的值的类型 (例如 `int`, `float`, `std::string`)。
   * @return `T` 类型的值。
   * @throw 如果 `ConstValue` 中实际存储的类型与 `T` 不匹配，可能会抛出异常。
   */
  template <typename T>
  T value() const {
    return value_.get<T>();
  }

  /**
   * @brief 获取叶节点值的基本类型。
   * @return `BasicType*` 指向该值的基础类型的指针。
   */
  BasicType *type() const { return value_.type(); }

  /**
   * @brief 分析当前叶节点 (通常是标识符) 在特定函数上下文中是否作为引用参数使用。
   * @param ts 当前的符号表集合，用于查找符号。
   * @param fn 当前所在的函数符号，用于检查参数传递模式。
   * @return 如果此叶节点代表一个引用参数，则为 `true`，并设置 `is_ref_` 标志；否则为 `false`。
   */
  bool AnalyzeReference(TableSet *ts, FunctionSymbol *fn);

 private:
  ConstValue value_;     ///< 存储叶节点的常量值 (如数字、字符、字符串字面量或标识符名)。
  bool is_ref_ = false;  ///< 标记此叶节点 (通常是变量标识符) 是否作为引用使用。
};

/**
 * @brief 程序节点，AST的根节点，代表整个Pascal程序。
 * @details 语法规则: `program → program_head program_body`
 * @example
 *   `program MyProgram; begin ... end.`
 *   子节点通常是: `ProgramHeadNode` (代表 `program MyProgram;`), `ProgramBodyNode` (代表其余部分)。
 */
class ProgramNode : public Node {
  // program → program_head  program_body
};

/**
 * @brief 程序头部节点，代表 `program` 声明部分。
 * @details 语法规则: `programhead → program_id (idlists)` (idlists部分可选)
 * @example
 *   `program MyProgram (input, output);`
 *   子节点通常是: `LeafNode` (存储程序名 `MyProgram`), `IdListNode` (存储参数列表 `input, output`, 如果存在)。
 */
class ProgramHeadNode : public Node {
  // programhead → program_id (idlists)
};

/**
 * @brief 程序体节点，包含常量、类型 (Pascal标准不在此处定义，但扩展可能支持)、变量、子程序声明和主执行块。
 * @details 语法规则: `program_body → const_declarations var_declarations subprogram_declarations compound_statement`
 *          (各声明部分可选，但顺序固定，最后是复合语句)
 * @example
 *   `const C=1; var i:integer; procedure P; begin end; begin ... end.`
 *   子节点通常是: `ConstDeclarationsNode`, `VariableDeclarationsNode`, `SubprogramDeclarationsNode`, `CompoundStatementNode`.
 */
class ProgramBodyNode : public Node {
  // program_body → const_declarations
  //                var_declarations
  //                subprogram_declarations
  //                compound_statement
};

/**
 * @brief 标识符列表节点，用于声明多个标识符或在程序头中列出程序参数。
 * @details
 *   语法规则:
 *   - `idlists → id` (单个标识符)
 *   - `idlists → idlist, id` (多个标识符，递归定义)
 * @example
 *   在 `var a, b, c: integer;` 中, `a, b, c` 会形成一个 IdListNode。
 *   在 `program MyProgram (input, output);` 中, `input, output` 也是一个 IdListNode。
 *   子节点是 `LeafNode` (代表各个标识符)。`grammar_type_` 区分是单个ID还是递归列表。
 */
class IdListNode : public Node {
 public:
  /** @brief 定义标识符列表的语法产生式类型。*/
  enum class GrammarType {
    SINGLE_ID,   ///< `idlists → id`
    MULTIPLE_ID  ///< `idlists → idlist, id` (递归)
  };
  /**
   * @brief 构造函数。
   * @param gt 标识符列表的语法类型。
   */
  IdListNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /**
   * @brief 将标识符列表中的所有标识符提取为 `LeafNode` 指针的向量。
   * @return `std::vector<LeafNode *>` 包含所有标识符叶节点的向量。
   */
  std::vector<LeafNode *> Lists() const;

 private:
  GrammarType grammar_type_; ///< 标识符列表的语法类型。
};

/**
 * @brief 常量声明块节点。
 * @details 语法规则:
 *   - `const_declarations → EPSILON` (无常量声明)
 *   - `const_declarations → const const_declaration ;` (Pascal通常是 `CONST const_decl; {const_decl;}`)
 *          (此处的 `const_declaration` 可能指代一连串 `id = const_value`)
 * @example
 *   `const PI = 3.14; MAX = 100;`
 *   可以为空，或包含一个或多个 `ConstDeclarationNode`。
 *   子节点是 `ConstDeclarationNode`。
 */
class ConstDeclarationsNode : public Node {
  //  enum class GrammarType {
  //    EPSILON,             //const_declarations → EPSILON
  //    CONST_DECLARATION    //const_declarations → const_declaration
  //  };
};

/**
 * @brief 单个常量声明节点 (实际上可能代表一系列通过分号分隔的常量定义)。
 * @details 语法规则 (简化，实际可能是列表):
 *  `const_declaration → id = const_value` (单个)
 *  `const_declaration → const_declaration ; id = const_value` (多个)
 * @example
 *   `PI = 3.14` (作为 `ConstDeclarationsNode` 的子节点出现)
 *   子节点通常是: `LeafNode` (常量名), `LeafNode` (常量值)。
 *   `type_` 存储该常量的类型 (在语义分析时确定)。
 */
class ConstDeclarationNode : public Node {
 public:
  /** @brief 定义常量声明的语法产生式类型。*/
  enum class GrammarType {
    VALUE,       ///< `ConstDeclaration → id = const_var` (单个常量定义)
    DECLARATION  ///< `ConstDeclaration → ConstDeclaration; id = const_var` (常量定义列表的递归部分)
  };
  /**
   * @brief 构造函数。
   * @param gt 常量声明的语法类型。
   * @param bt 常量的基础类型 (通常在解析时确定)。
   */
  ConstDeclarationNode(GrammarType gt, BasicType *bt)
      : grammar_type_(gt), type_(bt) {}
  /** @brief 获取语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /** @brief 获取常量类型。*/
  BasicType* type() const { return type_; }
  /**
   * @brief (可能用于代码生成) 打印类型信息到文件。
   * @param dst 目标文件指针。
   */
  void print_type(FILE *dst);

 private:
  GrammarType grammar_type_; ///< 常量声明的语法类型。
  BasicType *type_;          ///< 常量的基础类型。
};


/**
 * @brief 变量声明块节点。
 * @details 语法规则:
 *   - `variable_declarations → EPSILON` (无变量声明)
 *   - `variable_declarations → VAR variable_declaration ;` (Pascal通常是 `VAR var_decl; {var_decl;}`)
 * @example
 *   `var i: integer; x: real;`
 *   可以为空，或包含一个或多个 `VariableDeclarationNode`。
 *   子节点是 `VariableDeclarationNode`。
 */
class VariableDeclarationsNode : public Node {
  //  variable_declarations → EPSILON
  //                        | var variable_declaration
};

/**
 * @brief 单个变量声明节点 (可能包含同一类型的多个变量，也可能是一系列声明)。
 * @details 语法规则:
 *   - `variable_declaration → idlist : type_specifier` (一组同类型变量)
 *   - `variable_declaration → variable_declaration ; idlist : type_specifier` (多个声明组)
 * @example
 *   `i, j : integer`
 *   子节点通常是: `IdListNode` (变量名列表), `TypeNode` (类型)。
 *   `grammar_type_` 区分是单个声明组还是递归列表。
 *   `list_type_` 可能用于区分是处理标识符列表还是类型 (用途待进一步确认)。
 */
class VariableDeclarationNode : public Node {
 public:
  /** @brief 定义变量声明的语法产生式类型。 */
  enum class GrammarType {
    SINGLE_DECL,   ///< `variable_declaration → idlist : type_specifier`
    MULTIPLE_DECL  ///< `variable_declaration → variable_declaration ; idlist : type_specifier` (递归)
  };
  /** @brief (用途待确认) 可能用于区分声明中的子部分是ID列表还是类型。*/
  enum class ListType { TYPE, ID };
  /**
   * @brief 构造函数。
   * @param gt 变量声明的语法类型。
   * @param lt (用途待确认) 列表类型。
   */
  VariableDeclarationNode(GrammarType gt, ListType lt)
      : grammar_type_(gt), list_type_(lt) {}
  /** @brief 获取语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /** @brief 获取列表类型。*/
  ListType list_type() const { return list_type_; }


 private:
  GrammarType grammar_type_; ///< 变量声明的语法类型。
  ListType list_type_;       ///< (用途待确认) 列表类型。
};

/**
 * @brief 类型节点，用于描述变量、参数、函数返回值等的类型。
 * @details
 *   语法规则 (示例):
 *   - `type_specifier → basic_type_name` (如 `integer`)
 *   - `type_specifier → ARRAY [ periods ] OF type_specifier` (如 `array[1..10] of integer`)
 *   - `type_specifier → RECORD record_body END` (如 `record a:integer; b:char end`)
 * @example
 *   `integer`
 *   `array[1..10] of integer`
 *   `record a:integer; b:char end`
 *   子节点根据 `grammar_type_` 不同而不同:
 *   - `BASIC_TYPE`: 子节点是 `BasicTypeNode`。
 *   - `ARRAY`: 子节点是 `TypeNode` (数组的元素类型) 和 `PeriodsNode` (数组的维度范围)。
 *              `base_type_node_` 指向元素类型节点。
 *   - `RECORD_TYPE`: 子节点是 `RecordBodyNode` (记录体的字段声明)。
 */
class TypeNode : public Node {
 public:
  /** @brief 定义类型节点的语法类别。*/
  enum class GrammarType {
    BASIC_TYPE,  ///< 基本类型 (integer, real, char, boolean)
    ARRAY,       ///< 数组类型
    RECORD_TYPE  ///< 记录类型
  };

  /**
   * @brief 构造函数。
   * @param gt 类型的语法类别。
   */
  TypeNode(GrammarType gt) : grammar_type_(gt), base_type_node_(nullptr) {} // 初始化 base_type_node_

  /** @brief 获取类型的语法类别。*/
  GrammarType grammar_type() const { return grammar_type_; }

  /**
   * @brief (主要用于数组类型) 设置数组的基类型 (元素类型) 节点。
   * @param node 指向代表基类型的 `TypeNode`。
   */
  void set_base_type_node(TypeNode *node) { base_type_node_ = node; }

  /**
   * @brief (主要用于数组类型) 获取数组的基类型 (元素类型) 节点。
   * @return 指向代表基类型的 `TypeNode` 的指针。
   */
  TypeNode *base_type() const { return base_type_node_; }

  /**
   * @brief (可能用于代码生成) 将数组维度的格式化表示输出到文件。
   * @param dst 目标文件指针。
   */
  void PeriodsFormat(FILE *dst);

 private:
  GrammarType grammar_type_;  ///< 类型的语法类别。
  TypeNode *base_type_node_;  ///< 对于数组类型，指向其元素类型的 `TypeNode`。
};

/**
 * @brief 基本类型节点，表示一个具体的 Pascal 基本类型。
 * @details 语法规则: `BasicType → integer | real | boolean | char`
 * @example
 *   `integer` 在 `var i : integer;` 中。
 *   `type_` 存储具体的 `BasicType` 对象 (例如 `TYPE_INT`, `TYPE_REAL` 等全局类型对象)。
 */
class BasicTypeNode : public Node {
  // BasicType →integer|real|boolean|char
 public:
  /** @brief 默认构造函数。*/
  BasicTypeNode() : type_(nullptr) {} // 初始化 type_
  /**
   * @brief 使用 `BasicType` 指针构造节点。
   * @param type 指向 `BasicType` 对象的指针。
   */
  BasicTypeNode(BasicType *type) : type_(type) {}

  /**
   * @brief 获取该基本类型对应的C语言类型名称。
   * @param ref 如果为 `true`，表示是指针类型，会在类型名后加 `*`。
   * @return `std::string` C语言类型名 (例如 `"int"`, `"float*"`)。
   */
  std::string TypeName(bool ref = false) const {
    if (!type_) return "void"; // 处理 type_ 为空的情况
    std::string type_name = type_->c_name() + (ref ? "*" : "");
    return type_name;
  }
  /**
   * @brief 设置节点的具体基本类型。
   * @param type 指向 `BasicType` 对象的指针。
   */
  void set_type(BasicType *type) { type_ = type; }
  /**
   * @brief 获取节点的具体基本类型。
   * @return 指向 `BasicType` 对象的指针。
   */
  BasicType *type() const { return type_; }


 private:
  BasicType *type_; ///< 指向实际的 `BasicType` 对象 (例如 `TYPE_INT`)。
};

/**
 * @brief 记录体定义节点，包含记录类型中的字段声明。
 * @details 语法规则: `record_body → EPSILON | variable_declaration ;`
 *          (可以为空记录，或包含一系列变量声明作为字段)
 * @example
 *   在 `record num: integer; ch: char; end` 中, `num: integer; ch: char;` 部分的AST表示。
 *   子节点通常是 `VariableDeclarationNode` (用于字段声明)。
 */
class RecordBodyNode : public Node {
  // record_body → EPSILON | var_declaration
};

/**
 * @brief 数组维度区间列表节点。
 * @details 语法规则:
 *   - `periods → period` (单个维度)
 *   - `periods → periods , period` (多个维度，递归定义)
 * @example
 *   对于 `array[1..10, 0..5] of integer;`, `[1..10, 0..5]` 这部分。
 *   子节点是 `PeriodNode`。
 */
class PeriodsNode : public Node {
  // periods → period
  //         | periods, period
};

/**
 * @brief 数组单个维度区间节点。
 * @details 语法规则: `Period → const_value .. const_value`
 * @example
 *   `1..10` 在 `array[1..10] of integer` 中。
 *   子节点通常是两个 `LeafNode` (代表下界和上界常量)。
 *   `len_` 存储此维度的长度 (上界 - 下界 + 1)，在语义分析时计算。
 */
class PeriodNode : public Node {
  // Period → const_var ... const var
 public:
  /** @brief 获取此维度的长度。*/
  int len() const { return len_; }
  /** @brief 设置此维度的长度。*/
  void set_len(int len) { len_ = len; }

 private:
  int len_ = 0; ///< 此维度的长度。
};

/**
 * @brief 子程序 (过程/函数) 声明块节点。
 * @details 语法规则:
 *   - `subprogram_declarations → EPSILON` (无子程序声明)
 *   - `subprogram_declarations → subprogram_declarations subprogram_declaration ;` (递归定义)
 * @example
 *   `procedure P1; begin ... end; function F1: integer; begin ... end;`
 *   可以为空，或包含一个或多个 `SubprogramDeclarationNode`。
 */
class SubprogramDeclarationsNode : public Node {
  // subprogram_declarations → EPSILON
  //                         | subprogram_declarations subprogram_declaration
};

/**
 * @brief 单个子程序 (过程/函数) 声明节点。
 * @details 语法规则: `subprogram_declaration → subprogram_head subprogram_body`
 * @example
 *   `procedure MyProc(a: integer); begin ... end;`
 *   子节点通常是: `SubprogramHeadNode` (头部定义), `SubprogramBodyNode` (具体实现)。
 */
class SubprogramDeclarationNode : public Node {
  // subprogram_declaration -> subprogram_head subprogram_body
};

/**
 * @brief 子程序体节点，包含子程序的局部声明 (常量、变量) 和执行语句。
 * @details 语法规则: `subprogram_body → const_declarations var_declarations compound_statement`
 *           (常量和变量声明可选且有序，后跟复合语句)
 * @example
 *   在 `procedure P; const C=1; var x:real; begin ... end;` 中 `const C=1; var x:real; begin ... end;` 这部分。
 *   子节点通常是: `ConstDeclarationsNode`, `VariableDeclarationsNode`, `CompoundStatementNode`.
 */
class SubprogramBodyNode : public Node {
  // subprogram_body → const_declarations
  //                var_declarations
  //                compound_statement
};

/**
 * @brief 子程序头部节点，定义子程序的名称、参数和 (对于函数) 返回类型。
 * @details 语法规则:
 *   - `subprogram_head → PROCEDURE id formal_parameter`
 *   - `subprogram_head → FUNCTION id formal_parameter : basic_type`
 * @example
 *   `procedure MyProc (a: integer, var b: char)`
 *   `function Calculate (x: real) : real`
 *   子节点通常是: `LeafNode` (存储子程序名 `id_`), `FormalParamNode` (形式参数部分), 
 *   以及 (对于函数) `BasicTypeNode` (返回类型)。
 *   `id_` 成员变量也存储子程序名。
 */
class SubprogramHeadNode : public Node {
 public:
  /** @brief 定义子程序头部的类型 (过程或函数)。*/
  enum class GrammarType {
    PROCEDURE,  ///< `subprogram_head → procedure id formal_parameter`
    FUNCTION    ///< `subprogram_head → function id formal_parameter : basic_type`
  };

  /**
   * @brief 构造函数。
   * @param gt 子程序头部的类型 (过程或函数)。
   */
  SubprogramHeadNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取子程序头部的类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /**
   * @brief 设置子程序的名称。
   * @param id 子程序名称。
   */
  void set_id(std::string id) { id_ = id; }
  /**
   * @brief 获取子程序的名称。
   * @return `std::string` 子程序名称。
   */
  std::string id() const { return id_; }

 private:
  std::string id_; ///< 子程序的名称。
  GrammarType grammar_type_; ///< 标记是过程还是函数。
};

/**
 * @brief 形式参数节点，代表子程序定义中参数列表的括号 `()` 部分。
 * @details 语法规则:
 *   - `formal_parameter → EPSILON` (无参数)
 *   - `formal_parameter → ( parameter_lists )` (有参数)
 * @example
 *   `(a: integer; var b: char)` 在 `procedure MyProc(...)` 中，此节点代表整个括号及其内容。
 *   可以为空 (无子节点，对应EPSILON)，或包含一个 `ParamListsNode`作为子节点。
 */
class FormalParamNode : public Node {
 public:
  // formal_parameter → EPSILON
  //                  | ( parameter_lists )
};

/**
 * @brief 参数列表节点，包含一个或多个参数声明组，这些组之间用分号分隔。
 * @details 语法规则:
 *   - `parameter_lists → parameter_list` (单个参数组)
 *   - `parameter_lists → parameter_lists ; parameter_list` (多个参数组，递归定义)
 * @example
 *   `a: integer; var b: char` (这是 `parameter_lists` 的内容)
 *   子节点是 `ParamListNode` (`a: integer` 是一个, `var b: char` 是另一个)。
 *   `grammar_type_` 区分是单个参数组还是递归列表。
 */
class ParamListsNode : public Node {
 public:
  /** @brief 定义参数列表的语法产生式类型。*/
  enum class GrammarType {
    SINGLE_PARAM_LIST,    ///< `param_lists → param_list`
    MULTIPLE_PARAM_LIST,  ///< `param_lists → param_lists ; param_list` (递归)
  };

  /**
   * @brief 构造函数。
   * @param gt 参数列表的语法类型。
   */
  ParamListsNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取参数列表的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  GrammarType grammar_type_; ///< 参数列表的语法类型。
};

/**
 * @brief 单个参数声明组节点 (要么是一组值参，要么是一组变参)。
 * @details 语法规则: `parameter_list → var_parameter | value_parameter`
 * @example
 *   `a,b : integer` (这是一个 `value_parameter`)
 *   `var x : real` (这是一个 `var_parameter`)
 *   子节点是 `VarParamNode` (对于 `var` 参数) 或 `ValueParamNode` (对于值参数)。
 */
class ParamListNode : public Node {
  //  parameter_list → var_parameter | value_parameter
};

/**
 * @brief 可变参数 (var parameter) 节点，以 `var` 关键字开头。
 * @details 语法规则: `var_parameter → VAR value_parameter`
 * @example
 *   `var count: integer` 在参数列表中。
 *   子节点通常是 `ValueParamNode` (包含标识符列表和类型)。
 */
class VarParamNode : public Node {
  // var_param → var value_param
};

/**
 * @brief 值参数节点，声明一个或多个同类型的值传递参数。
 * @details 语法规则: `value_parameter → idlist : basic_type`
 * @example
 *   `id_list : basic_type` 如 `a, b : integer` 在参数列表中。
 *   子节点通常是 `IdListNode` (参数名列表), `BasicTypeNode` (参数类型)。
 */
class ValueParamNode : public Node {
  // ValueParam → idlist : basic_type
};

/**
 * @brief 复合语句节点，代表 `BEGIN ... END` 块。
 * @details 语法规则: `CompoundStatement → BEGIN StatementList END`
 * @example
 *   `begin a := 1; b := 2; end`
 *   子节点是 `StatementListNode`。
 */
class CompoundStatementNode : public Node {
  // CompoundStatement → begin StatementList end
};

/**
 * @brief 语句列表节点，包含一个或多个由分号分隔的语句。
 * @details 语法规则:
 *   - `statement_list → statement` (单个语句)
 *   - `statement_list → statement_list ; statement` (多个语句，递归定义)
 * @example
 *   `a := 1; b := 2` 在 `begin ... end` 块中。
 *   子节点是 `StatementNode`。
 */
class StatementListNode : public Node {
  // statement_list → statement | statement_list ; statement (应为 statement_list ; statement)
};

/**
 * @brief 语句节点，代表各类Pascal语句的通用节点。
 * @details `grammar_type_` 枚举指明了此节点代表的具体语句类型。
 *          其子节点结构根据 `grammar_type_` 决定。
 * @example
 *   - `x := y + 1` (VAR_ASSIGN_OP_EXP: 子节点 VariableNode, ExpressionNode)
 *   - `MyProc(a, b)` (PROCEDURE_CALL: 子节点 ProcedureCallNode)
 *   - `if c > 0 then s1 else s2` (IF_STATEMENT: 子节点 ExpressionNode(条件), StatementNode(then分支), ElseNode(else分支))
 *   - `for i := 1 to 10 do s` (FOR_STATEMENT: 子节点 LeafNode(循环变量), ExpressionNode(初值), UpdownNode, ExpressionNode(终值), StatementNode(循环体))
 *   - `read(v)` (READ_STATEMENT: 子节点 VariableListNode)
 *   - `write(e)` (WRITE_STATEMENT: 子节点 ExpressionListNode)
 *   - `while cond do s` (WHILE_STATEMENT: 子节点 ExpressionNode(条件), StatementNode(循环体))
 *   - `repeat s until cond` (REPEAT_STATEMENT: 子节点 StatementListNode(循环体), ExpressionNode(条件))
 */
class StatementNode : public Node {
 public:
  /** @brief 定义语句节点的具体语法类型。*/
  enum class GrammarType {
    EPSILON,             ///< 空语句 `statement → EPSILON`
    VAR_ASSIGN_OP_EXP,   ///< 变量赋值语句 `statement → variable assignop expression`
    FUNC_ASSIGN_OP_EXP,  ///< 函数返回值赋值语句 `statement → func_id assignop expression` (用于函数内部给函数名赋值)
    PROCEDURE_CALL,      ///< 过程调用语句 `statement → procedure_call`
    COMPOUND_STATEMENT,  ///< 复合语句 `statement → compound_statement` (BEGIN...END块)
    IF_STATEMENT,        ///< IF语句 `statement → if expression then statement else_part`
    FOR_STATEMENT,       ///< FOR语句 `statement → for id assignop expression (TO|DOWNTO) expression do statement`
    READ_STATEMENT,      ///< READ语句 `statement → read ( variable_list )`
    READLN_STATEMENT,    ///< READLN语句 `statement → readln ( variable_list )` (或无参数)
    WRITE_STATEMENT,     ///< WRITE语句 `statement → write ( expression_list )`
    WRITELN_STATEMENT,   ///< WRITELN语句 `statement → writeln ( expression_list )` (或无参数)
    CASE_STATEMET,       ///< CASE语句 `statement → case expression of case_body end`
    WHILE_STATEMENT,     ///< WHILE语句 `statement → while expression do statement`
    REPEAT_STATEMENT     ///< REPEAT语句 `statement → repeat statement_list until expression`
  };

  /**
   * @brief 构造函数。
   * @param gt 语句的具体语法类型。
   */
  StatementNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取语句的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  GrammarType grammar_type_; ///< 语句的具体语法类型。
};

/**
 * @brief 变量列表节点，通常用于 `read` 或 `readln` 语句的参数列表。
 * @details 语法规则:
 *   - `variable_list → variable` (单个变量)
 *   - `variable_list → variable_list , variable` (多个变量，递归定义)
 * @example
 *   `a, arr[i]` 在 `read(a, arr[i])` 中。
 *   子节点是 `VariableNode`。
 *   `basic_types` 存储解析时每个变量的 `BasicType` 信息，用于后续处理 (如 `printf` 格式化)。
 */
class VariableListNode : public Node {
 public:
  /** @brief 定义变量列表的语法产生式类型。*/
  enum class GrammarType {
    VARIABLE,                ///< `variable_list → variable`
    VARIABLE_LIST_VARIABLE,  ///< `variable_list → variable_list , variable` (递归)
  };

  /**
   * @brief 构造函数。
   * @param gt 变量列表的语法类型。
   */
  VariableListNode(GrammarType gt) : grammar_type_(gt) {}
  /**
   * @brief 生成用于 `scanf` 或类似函数的格式化字符串。
   * @return `std::string` 格式化字符串，例如 `"%d %s %f"`。
   */
  std::string FormatString() const;
  /**
   * @brief 根据提供的类型列表设置 `basic_types` 成员。
   * @param type_list 指向 `TypeTemplate*` 向量的指针，包含各变量的类型。
   * @return 如果所有类型都能成功转换为 `BasicType` 或 `TYPE_STRINGLIKE`，则为 `true`；否则为 `false`。
   */
  bool set_types(std::vector<TypeTemplate *> *type_list);
  /** @brief 获取变量列表的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  std::vector<BasicType *> basic_types; ///< 存储列表中每个变量的基础类型 (或 `TYPE_STRINGLIKE`)。
  GrammarType grammar_type_;            ///< 变量列表的语法类型。
};

/**
 * @brief 变量引用节点，代表源代码中对一个变量的访问。
 * @details 语法规则: `variable → id id_varparts`
 *          (`id_varparts` 用于数组索引或记录字段访问，可以为空)
 * @example
 *   `myVar` (子节点: LeafNode("myVar"), IdVarPartsNode(空))
 *   `myArray[index]` (子节点: LeafNode("myArray"), IdVarPartsNode(包含IDVarPartNode for `[index]`))
 *   `myRecord.field` (子节点: LeafNode("myRecord"), IdVarPartsNode(包含IDVarPartNode for `.field`))
 *   `sym_ptr_` 指向符号表中该变量的条目。
 *   `type_ptr_` 指向该变量的类型。
 *   `id_` 存储变量的基本名称 (不包含下标或字段部分)。
 */
class VariableNode : public Node {
  // variable → id id_varparts
 public:
  /**
   * @brief 对变量节点进行语义检查和类型分析。
   * @details 通常在语义分析阶段调用。会填充 `type_ptr_` 和 `id_`，
   *          并检查变量是否已声明、类型是否正确等。
   *          对于数组类型的char且符合特定条件的，可能会将 `type_ptr_` 设置为 `TYPE_STRINGLIKE`。
   */
  void Check();
  // Getter methods for analyzed info (optional, depends on usage)
  /** @brief 获取变量符号在符号表中的指针 (在Check后有效)。*/
  SymbolTemplate* symbol_pointer() const { return sym_ptr_; }
  /** @brief 获取变量的类型指针 (在Check后有效)。*/
  TypeTemplate* type_pointer() const { return type_ptr_; }
  /** @brief 获取变量的基本名称 (在Check后有效)。*/
  std::string identifier_name() const { return id_; }

 private:
  SymbolTemplate *sym_ptr_ = nullptr;    ///< 指向此变量在符号表中的 `SymbolTemplate` 条目。
  TypeTemplate *type_ptr_ = nullptr;   ///< 指向此变量的 `TypeTemplate` 类型。
  std::string id_ = "";                ///< 变量的标识符名称 (不含下标或字段部分)。
};

/**
 * @brief 变量部分访问节点，用于表示数组索引 `[expr]` 或记录字段访问 `.id`。
 * @details 语法规则:
 *   - `id_varpart → .id`
 *   - `id_varpart → [ expression_list ]`
 * @example
 *   - `.fieldName` (当 `grammar_type_` 是 `_ID`, 子节点是 `LeafNode("fieldName")`)
 *   - `[expr]` (当 `grammar_type_` 是 `EXP_LIST`, 子节点是 `ExpressionListNode`)
 *   `array_lb_` (仅用于数组索引) 存储当前访问维度的下界 (lower bound)，用于地址计算。
 */
class IDVarPartNode : public Node {
 public:
  /** @brief 定义变量部分访问的语法类型。*/
  enum class GrammarType {
    _ID,       ///< 记录字段访问 `.id`
    EXP_LIST,  ///< 数组索引 `[ expression_list ]`
  };

  /**
   * @brief 构造函数。
   * @param gt 变量部分访问的语法类型。
   */
  IDVarPartNode(GrammarType gt) : grammar_type_(gt), array_lb_(0) {} // 初始化 array_lb_
  /** @brief 获取变量部分访问的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /** @brief (数组索引专用) 获取当前维度的下界。*/
  int array_lb() const { return array_lb_; }
  /** @brief (数组索引专用) 设置当前维度的下界。*/
  void set_array_lb(int lb) { array_lb_ = lb; }

 private:
  int array_lb_ = 0;         ///< (数组索引专用) 当前访问维度的下界。
  GrammarType grammar_type_; ///< 变量部分访问的语法类型。
};

/**
 * @brief 变量访问链节点，用于处理如 `arr[i].field[j]` 这样的多级复杂访问。
 * @details 语法规则:
 *   - `id_varparts → EPSILON` (无后续访问，如简单变量)
 *   - `id_varparts → id_varparts id_varpart` (递归定义，链式访问)
 * @example
 *   对于 `arr[i].field[j]`, 此节点可能有多个 `IDVarPartNode` 作为子节点链:
 *   - 第一个 `IDVarPartNode` 可能是代表 `[i]` (EXP_LIST 类型)
 *   - 第二个 `IDVarPartNode` 可能是代表 `.field` (_ID 类型)
 *   - 第三个 `IDVarPartNode` 可能是代表 `[j]` (EXP_LIST 类型)
 *   子节点是 `IDVarPartNode`。
 *   `set_lb` 方法用于在语义分析时，根据数组类型信息，为链中每个数组访问部分的 `IDVarPartNode` 设置正确的维度下界。
 */
class IDVarPartsNode : public Node {
 public:
  //  enum class GrammarType {
  //    EPSILON,      //id_varparts → EPSILON
  //    MULTIPLE_IDv, //id_varparts → id_varparts id_varpart
  //  };
  /**
   * @brief 设置访问链中各数组维度的下界。
   * @param bound 一个包含数组所有维度边界信息的向量。此方法会消耗 `bound` 中的元素。
   * @note 此方法通常是递归调用的，或者其子节点是递归的 `IDVarPartsNode`。
   *       它会遍历访问链 (子节点)，如果遇到数组索引 (`EXP_LIST` 类型的 `IDVarPartNode`)，
   *       就从 `bound` 中取出一个下界信息并设置给该 `IDVarPartNode`。
   */
  void set_lb(std::vector<ArrayType::ArrayBound> &bound) {
    if (child_list_.empty() || bound.empty()) return; // 增加 bound.empty() 检查
    // 假设子节点结构是: [IDVarPartsNode (recursive), IDVarPartNode (current part)]
    // 或者直接就是一系列 IDVarPartNode
    // 此处实现依赖于parser如何构建这棵树，原始的 set_lb 实现可能基于特定的子节点结构。
    // 假设一个简化的平铺结构或左递归结构：child_list_[0]是下一个IDVarPartsNode, child_list_[1]是当前的IDVarPartNode
    // 如果是纯平铺的IDVarPartNode列表，则需要迭代

    // 以下逻辑基于原始代码的推断，可能需要根据实际的解析器构建方式调整。
    // 假设 IDVarPartsNode 的子节点是 [IDVarPartsNode_递归, IDVarPartNode_当前]
    // 或者只有一个 IDVarPartNode (链的末端)
    // 或者为空 (EPSILON)
    
    // 尝试处理更通用的子节点结构：迭代所有子节点
    // 但原始代码似乎是为特定的递归结构设计的。我们先尝试模仿原始逻辑。
    // 原始逻辑: child_list_[0]->DynamicCast<IDVarPartsNode>()->set_lb(bound);
    //          if (child_list_[1]->...)
    // 这暗示了子节点0是递归的IDVarPartsNode，子节点1是IDVarPartNode

    Node* first_child = get_child(0);
    Node* second_child = get_child(1);

    if (first_child && first_child->DynamicCast<IDVarPartsNode>()) {
        // 对应 id_varparts -> id_varparts id_varpart 产生式的右递归部分
        first_child->DynamicCast<IDVarPartsNode>()->set_lb(bound);
        if (bound.empty()) return; // 如果bound被递归调用消耗完了

        IDVarPartNode* current_part = second_child ? second_child->DynamicCast<IDVarPartNode>() : nullptr;
        if (current_part && current_part->grammar_type() == IDVarPartNode::GrammarType::EXP_LIST) {
            current_part->set_array_lb(bound[0].lb_);
            bound.erase(bound.begin());
        }
    } else if (first_child && first_child->DynamicCast<IDVarPartNode>() && !second_child) {
        // 对应 id_varparts -> id_varpart (链的末端，只有一个IDVarPartNode)
        IDVarPartNode* current_part = first_child->DynamicCast<IDVarPartNode>();
        if (current_part->grammar_type() == IDVarPartNode::GrammarType::EXP_LIST) {
            current_part->set_array_lb(bound[0].lb_);
            bound.erase(bound.begin());
        }
    }
    // 如果是EPSILON (child_list_.empty())，则直接返回，已在开头处理
  }
};


/**
 * @brief For循环方向节点，表示 `to` (递增) 或 `downto` (递减)。
 * @details 语法规则: `updown → TO | DOWNTO`
 * @example
 *   `to` 在 `for i := 1 to 10 do ...`
 *   `downto` 在 `for i := 10 downto 1 do ...`
 *   `is_increase_` 存储是递增 (`true`) 还是递减 (`false`)。
 */
class UpdownNode : public Node {
  // updown → to | downto
 public:
  /**
   * @brief 构造函数。
   * @param is_increase `true` 表示 `TO` (递增)，`false` 表示 `DOWNTO` (递减)。默认为 `true`。
   */
  UpdownNode(bool is_increase = true) : is_increase_(is_increase) {}
  /** @brief 判断是否为递增方向 (`TO`)。*/
  bool IsIncrease() const { return is_increase_; }
  /** @brief 设置循环方向。*/
  void set_increase(bool inc) { is_increase_ = inc; }

 private:
  bool is_increase_; ///< `true` 代表 `TO` (递增)，`false` 代表 `DOWNTO` (递减)。
};

/**
 * @brief 过程调用节点。
 * @details 语法规则:
 *   - `procedure_call → id` (无参数过程调用)
 *   - `procedure_call → id ( expression_list )` (带参数过程调用)
 * @example
 *   `MyProcedure` (当 `grammar_type_` 是 `ID`)
 *   `MyProcedure(arg1, arg2)` (当 `grammar_type_` 是 `ID_EXP_LIST`)
 *   子节点: `LeafNode` (存储过程名), `ExpressionListNode` (存储参数列表，如果存在)。
 */
class ProcedureCallNode : public Node {
 public:
  /** @brief 定义过程调用语句的语法类型。*/
  enum class GrammarType {
    ID,           ///< 无参数过程调用 `procedure_call → id`
    ID_EXP_LIST,  ///< 带参数过程调用 `procedure_call → id ( expression_list )`
  };

  /**
   * @brief 构造函数。
   * @param gt 过程调用的语法类型。
   */
  ProcedureCallNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取过程调用的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  GrammarType grammar_type_; ///< 过程调用的语法类型。
};

/**
 * @brief If语句的Else分支节点。
 * @details 语法规则:
 *   - `else_part → EPSILON` (无else分支)
 *   - `else_part → ELSE statement` (有else分支)
 * @example
 *   `else statement` 在 `if ... then ... else statement` 中。
 *   可以为空 (无子节点，对应 `EPSILON`)，或包含一个 `StatementNode` (else后的语句) 作为子节点。
 */
class ElseNode : public Node {
 public:
  /** @brief 定义Else部分的语法类型。*/
  enum class GrammarType {
    EPSILON,         ///< 无else分支 `else_part → EPSILON`
    ELSE_STATEMENT,  ///< 有else分支 `else_part → else statement`
  };

  /**
   * @brief 构造函数。
   * @param gt Else部分的语法类型。
   */
  ElseNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取Else部分的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }
  /**
   * @brief 获取Else分支中的语句节点。
   * @return 指向 `StatementNode` 的指针。如果无Else分支或结构不符，可能返回 `nullptr`。
   * @note 假定如果存在Else分支，其第一个子节点就是代表该分支语句的 `StatementNode`。
   */
  StatementNode *GetStatement() {
    if (grammar_type_ == GrammarType::ELSE_STATEMENT && !child_list_.empty()) {
        return child_list_[0]->DynamicCast<StatementNode>();
    }
    return nullptr;
  }

 private:
  GrammarType grammar_type_; ///< Else部分的语法类型。
};

/**
 * @brief 表达式列表节点，常见于函数/过程调用的参数列表或数组索引列表。
 * @details 语法规则:
 *   - `expression_list → expression` (单个表达式)
 *   - `expression_list → expression_list , expression` (多个表达式，递归定义)
 * @example
 *   `a+b, 10, c` 在 `MyFunc(a+b, 10, c)` 中。
 *   子节点是 `ExpressionNode`。
 *   `basic_types` 存储解析时每个表达式的 `BasicType` 信息，用于后续处理 (如 `printf` 格式化)。
 *   `set_ref` 方法用于处理参数传递时的引用问题。
 */
class ExpressionListNode : public Node {
 public:
  /** @brief 定义表达式列表的语法产生式类型。*/
  enum class GrammarType {
    EXP,           ///< `expression_list → expression`
    EXP_LIST_EXP,  ///< `expression_list → expression_list , expression` (递归)
  };

  /**
   * @brief 构造函数。
   * @param gt 表达式列表的语法类型。
   */
  ExpressionListNode(GrammarType gt) : grammar_type_(gt) {}
  /**
   * @brief 生成用于 `printf` 或类似函数的格式化字符串。
   * @return `std::string` 格式化字符串，例如 `"%d%s%.6f"`。
   */
  std::string FormatString() const;
  /**
   * @brief 根据提供的类型列表设置 `basic_types` 成员。
   * @param type_list 指向 `TypeTemplate*` 向量的指针，包含各表达式的类型。
   * @return 如果所有类型都能成功转换为 `BasicType` 或 `TYPE_STRINGLIKE`，则为 `true`；否则为 `false`。
   */
  bool set_types(std::vector<TypeTemplate *> *type_list);
  /**
   * @brief (用于函数/过程调用参数传递) 根据参数是否为引用传递，设置表达式是否为引用。
   * @param ref_stack 一个布尔类型的栈，栈顶指示当前表达式是否应为引用。
   *                  此方法会消耗栈中元素 (通过 `pop`)。
   */
  void set_ref(std::stack<bool> *ref_stack);
  /** @brief 获取表达式列表的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  std::vector<BasicType *> basic_types; ///< 存储列表中每个表达式的基础类型 (或 `TYPE_STRINGLIKE`)。
  GrammarType grammar_type_;            ///< 表达式列表的语法类型。
};

/**
 * @brief 表达式节点，代表一个可以求值的表达式。
 * @details 语法规则 (简化):
 *   - `expression → simple_expression`
 *   - `expression → simple_expression relop simple_expression`
 *   - `expression → string_literal` (特定情况，由 `target_type_` 区分)
 * @example
 *   - `a + b` (此节点可能只有一个 `SimpleExpressionNode` 子节点)
 *   - `x > 5` (子节点: `SimpleExpressionNode` for `x`, `LeafNode` for `>`, `SimpleExpressionNode` for `5`)
 *   - `'Hello'` (当 `target_type_` 为 `CONST_STRING`，子节点是 `LeafNode("Hello")`)
 *   - `myArray[i]` (当表达式是一个变量数组访问，`target_type_` 为 `VAR_ARRAY`，子节点是 `VariableNode`)
 *   子节点结构根据表达式的具体形式和 `target_type_` 而定。
 *   `is_ref_` 指示表达式是否作为引用被使用 (例如，作为VAR参数传递的变量，或赋值语句的左侧)。
 */
class ExpressionNode : public Node {
  // expression → str_expression (string literal)
  //            | simple_expression
  //            | simple_expression relop simple_expression
 public:
  /** @brief 定义表达式的预期类型或主要构成。*/
  enum class TargetType {
    EXPRESSION,    ///< 通用表达式 (通常是算术或逻辑表达式)
    VAR_ARRAY,     ///< 表达式本质上是一个数组变量的访问 (例如在 `write(myArray[i])` 中)
    CONST_STRING,  ///< 表达式是一个字符串字面量
  };
  /** @brief 默认构造函数，目标类型为 `EXPRESSION`，非引用。*/
  ExpressionNode() : target_type_(TargetType::EXPRESSION), is_ref_(false) {}
  /**
   * @brief 带目标类型的构造函数。
   * @param tg 表达式的目标类型。
   */
  ExpressionNode(TargetType tg) : target_type_(tg), is_ref_(false) {}
  /** @brief 将表达式标记为引用。*/
  void set_is_ref() { is_ref_ = true; }
  /** @brief 检查表达式是否为引用。*/
  bool is_ref() const { return is_ref_; }
  /** @brief 设置表达式的目标类型。*/
  void set_expression_type(TargetType tg) { target_type_ = tg; }
  /** @brief 获取表达式的目标类型。*/
  TargetType target_type() const { return target_type_; }

 private:
  TargetType target_type_; ///< 表达式的预期类型或主要构成。
  bool is_ref_ = false;    ///< 标记表达式是否作为引用使用 (例如，赋值语句左侧的变量)。
};

/**
 * @brief 简单表达式节点，由项 (`TermNode`) 和加法类运算符 (`+`, `-`, `or`) 组成。
 * @details 语法规则 (简化):
 *   - `simple_expression → term`
 *   - `simple_expression → [SIGN] term` (SIGN is `+` or `-`)
 *   - `simple_expression → simple_expression addop term` (`addop` is `+`, `-`, `or`)
 * @example
 *   `a + b - c`
 *   `-x`
 *   `flag1 or flag2`
 *   子节点是 `TermNode` 和代表运算符的 `LeafNode` (或通过节点类型间接表示运算符)。
 */
class SimpleExpressionNode : public Node {
  // simple_expression → term | +term | -term | simple_expression addop term
};

/**
 * @brief 项节点，由因子 (`FactorNode`) 和乘法类运算符 (`*`, `/`, `div`, `mod`, `and`) 组成。
 * @details 语法规则 (简化):
 *   - `term → factor`
 *   - `term → term mulop factor` (`mulop` is `*`, `/`, `div`, `mod`, `and`)
 * @example
 *   `x * y / 2`
 *   `a and b`
 *   子节点是 `FactorNode` 和代表运算符的 `LeafNode` (或通过节点类型间接表示运算符)。
 *   `op_div_` 标记此项是否涉及整数除法 `DIV`，可能影响代码生成。
 */
class TermNode : public Node {
 public:
  /** @brief 默认构造函数，`op_div_` 初始化为 `false`。*/
  TermNode() : op_div_(false) {}
  /** @brief 标记此项是否包含整数除法 `DIV`。*/
  void set_op_div(bool op) { op_div_ = op; }
  /** @brief 获取此项是否包含整数除法 `DIV` 的标记。*/
  bool get_op_div() const { return op_div_; }
 private:
  bool op_div_ = false; ///< 如果项包含整数除法 `DIV` 运算符，则为 `true`。
};

/**
 * @brief 因子节点，表达式的最基本组成单元。
 * @details `grammar_type_` 枚举指明了此因子节点的具体类型。
 * @example
 *   - `123` (UCONST_VAR, 子节点是 `UnsignConstVarNode`)
 *   - `myVariable` (VARIABLE, 子节点是 `VariableNode`)
 *   - `(a + b)` (EXP, 子节点是 `ExpressionNode` for `a+b`)
 *   - `MyFunction(x, y)` (ID_EXP_LIST, 子节点是 `LeafNode` (函数名 `MyFunction`), `ExpressionListNode` (参数 `x,y`))
 *   - `not flag` (NOT_BOOL 或 NOT_INT, 子节点是 `FactorNode` for `flag`)
 *   其子节点结构根据 `grammar_type_` 决定。
 */
class FactorNode : public Node {
 public:
  /** @brief 定义因子节点的具体语法类型。*/
  enum class GrammarType {
    UCONST_VAR,   ///< 无符号常量或变量 `factor → unsigned_constant | variable` (这里简化为UCONST_VAR)
                  ///< Pascal 中 variable 也是 factor, unsigned_constant 也是 factor
    VARIABLE,     ///< 变量引用 `factor → variable` (此枚举值可能与UCONST_VAR重叠或需细化)
    EXP,          ///< 带括号的表达式 `factor → ( expression )`
    ID_EXP_LIST,  ///< 函数调用 `factor → id ( expression_list )`
    NOT_INT,      ///< 整数取反 `factor → not factor` (Pascal `not` 也可用于整数)
    NOT_BOOL      ///<布尔取反 `factor → not factor`
  };

  /**
   * @brief 构造函数。
   * @param gt 因子的具体语法类型。
   */
  FactorNode(GrammarType gt) : grammar_type_(gt) {}
  /** @brief 获取因子的语法类型。*/
  GrammarType grammar_type() const { return grammar_type_; }

 private:
  GrammarType grammar_type_; ///< 因子的具体语法类型。
};

/**
 * @brief 无符号常量值节点 (数字或字符字面量)。
 * @details 语法规则 (简化): `unsigned_constant → NUM | 'char_literal'`
 * @example
 *   `123` (子节点: `LeafNode(123)`)
 *   `'A'` (子节点: `LeafNode('A')`)
 *   通常其子节点是一个 `LeafNode`，存储实际的常量值。
 */
class UnsignConstVarNode : public Node {
  // unsigned_const_variable → num | 'letter'
};

#endif //P2C-Transpiler_AST_H