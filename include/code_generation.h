/**
 * @file code_generation.h
 * @brief 定义 `CodeGenerator` 类，负责最终C代码的生成和格式化。
 *
 * `CodeGenerator` 类管理着一个标准库函数的集合，能够在代码生成时按需引入这些函数的定义 (通常是宏定义)。
 * 它还提供了一系列静态方法，用于将抽象语法树 (AST) 的各种节点格式化为C代码字符串并输出到指定文件流。
 * `DispatchFormat` 作为核心的格式化分发器，能够处理不同类型的AST节点。
 * `FormatProgram` 方法则用于格式化整个程序，包括库函数的引入和AST根节点的格式化。
 */
#include <iostream> // C++标准输入输出流，此头文件中可能未直接使用，但作为通用包含可能存在。
#include <unordered_map> // 用于存储库函数定义的映射表。
#include <vector>      // 可能在某些格式化函数内部或辅助数据结构中使用。

#include "symbol_table.h" // 需要符号表定义，例如在 Preset 方法中。

#ifndef P2C-Transpiler_CODE_GENERATION_H
#define P2C-Transpiler_CODE_GENERATION_H

// 前向声明各种AST节点类型
// 这些声明允许 CodeGenerator 类的方法参数使用这些类型，而无需包含完整的 ast.h，
// 从而减少编译依赖，避免循环包含问题。
class Node;                  ///< AST 节点基类
class LeafNode;              ///< 叶节点 (标识符、字面量等)
class BasicTypeNode;         ///< 基本类型节点 (integer, real等)
class IdListNode;            ///< 标识符列表节点
class PeriodNode;            ///< 数组维度区间节点
class ProgramHeadNode;       ///< 程序头部节点
class TypeNode;              ///< 类型定义节点
class ConstDeclarationNode;  ///< 常量声明节点
class VariableDeclarationNode; ///< 变量声明节点
class SubprogramDeclarationsNode; ///< 子程序声明块节点
class SubprogramDeclarationNode;  ///< 单个子程序声明节点
class SubprogramHeadNode;    ///< 子程序头部节点
class FormalParamNode;       ///< 形式参数节点 (括号部分)
class ParamListsNode;        ///< 参数列表节点
class VarParamNode;          ///< VAR参数节点
class ValueParamNode;        ///< 值参数节点
class StatementNode;         ///< 语句节点
class ElseNode;              ///< Else分支节点
class ProcedureCallNode;     ///< 过程调用节点
class VariableListNode;      ///< 变量列表节点 (用于read/write)
class ExpressionListNode;    ///< 表达式列表节点 (用于过程调用参数、write等)
class UpdownNode;            ///< For循环方向节点 (to/downto)
class ExpressionNode;        ///< 表达式节点
class VariableNode;          ///< 变量引用节点
class TermNode;              ///< 表达式中的项节点
class FactorNode;            ///< 表达式中的因子节点
class IDVarPartNode;         ///< 变量部分访问节点 (数组索引、记录字段)
class ProgramBodyNode;       ///< 程序体节点
class AST;                   ///< 抽象语法树总控类

/**
 * @brief 管理代码生成过程，包括标准库函数的定义和AST节点的格式化输出。
 *
 * 该类维护了一个Pascal标准库函数的映射表 (`lib_map_`)，这些函数在C中通过宏定义实现。
 * 它能够跟踪哪些库函数在编译过程中被实际调用，并在最终生成代码时只输出被调用函数的定义。
 * `Preset` 方法用于预先向符号表中添加这些标准库函数的符号。
 * 大量的静态 `FormatXxxNode` 方法负责将AST的不同节点转换为C代码的文本表示。
 * `DispatchFormat` 是一个统一的调度入口，根据节点类型调用相应的格式化方法。
 * `FormatProgram` 用于输出整个程序的C代码。
 */
class CodeGenerator {
 public:
  /**
   * @brief 构造函数，初始化标准库函数映射表 `lib_map_`。
   * @details `lib_map_` 中存储了每个库函数的C语言宏定义及其调用状态 (初始为false)。
   */
  CodeGenerator();
  /**
   * @brief 析构函数。
   * @details 目前为空，因为 `lib_map_` 中的 `Lib` 结构体不持有需要手动释放的资源。
   */
  ~CodeGenerator(){};

  /**
   * @brief 标记一个标准库函数已被调用。
   * @param lib_name 要标记的库函数名称 (例如 "abs", "sin")。
   * @return 如果库函数存在于 `lib_map_` 中且成功标记，则为 `true`；否则为 `false`。
   */
  bool Call(std::string lib_name);
  /**
   * @brief 取消标记一个标准库函数的调用状态 (主要用于测试或重置)。
   * @param lib_name 要取消标记的库函数名称。
   * @return 如果库函数存在于 `lib_map_` 中且成功取消标记，则为 `true`；否则为 `false`。
   */
  bool UnCall(std::string lib_name);

  /**
   * @brief 向符号表中预置标准库函数的符号定义。
   * @param st 指向目标符号表 (`SymbolTable`) 的指针。
   * @details 为 `lib_map_` 中定义的每个函数创建一个 `FunctionSymbol` 对象，
   *          并将其插入到给定的符号表中，使得这些库函数在语义分析阶段可以被正确识别。
   */
  void Preset(SymbolTable* st);

  /**
   * @brief 将C标准库的包含指令和已调用的Pascal标准库函数的宏定义输出到文件流。
   * @param dst 目标文件流指针。
   * @details 首先输出 `#include <stdio.h>` 和 `#include <string.h>`。
   *          如果检测到有数学函数被调用 (通过 `CheckMathLib()`)，则输出 `#include <math.h>`。
   *          然后遍历 `lib_map_`，将所有 `called_` 标记为 `true` 的库函数的宏定义输出。
   */
  void FormatStdLib(FILE* dst);

  // AST节点格式化方法
  // 以下所有 FormatXxxNode 方法都是静态的，接收一个特定类型的AST节点指针和目标文件流指针，
  // 并将该节点格式化为C代码输出到文件流。

  /** @brief 格式化叶节点 (LeafNode) 为C代码。*/
  static void FormatLeafNode(const LeafNode* node, FILE* dst);
  /** @brief 格式化基本类型节点 (BasicTypeNode) 为C代码 (通常是类型名)。*/
  static void FormatBasicTypeNode(const BasicTypeNode* node, FILE* dst);
  /** @brief 格式化标识符列表节点 (IdListNode) 为C代码 (通常是逗号分隔的标识符)。*/
  static void FormatIdListNode(const IdListNode* node, FILE* dst);
  /** @brief 格式化数组维度区间节点 (PeriodNode) 为C代码 (例如 `[length]`)。*/
  static void FormatPeriodNode(const PeriodNode* node, FILE* dst);
  /** @brief 格式化程序头节点 (ProgramHeadNode) 为C注释 (包含程序名和创建日期)。*/
  static void FormatProgramHeadNode(const ProgramHeadNode* node, FILE* dst);
  /** @brief 格式化类型节点 (TypeNode) 为C代码 (处理基本类型、数组、记录等)。*/
  static void FormatTypeNode(const TypeNode* node, FILE* dst);
  /** @brief (特定于数组类型) 格式化TypeNode的维度部分为C代码。*/
  static void FormatTypeNodePeriods(const TypeNode* node, FILE* dst);
  /** @brief 格式化常量声明节点 (ConstDeclarationNode) 为C的 `const` 声明。*/
  static void FormatConstDeclarationNode(const ConstDeclarationNode* node, FILE* dst);
  /** @brief 格式化变量声明节点 (VariableDeclarationNode) 为C的变量声明。*/
  static void FormatVariableDeclarationNode(const VariableDeclarationNode* node, FILE* dst);
  /** @brief 格式化子程序声明块节点 (SubprogramDeclarationsNode) (通常是迭代调用子节点的格式化)。*/
  static void FormatSubprogramDeclarationsNode(const SubprogramDeclarationsNode* node, FILE* dst);
  /** @brief 格式化单个子程序声明节点 (SubprogramDeclarationNode) 为C函数定义。*/
  static void FormatSubprogramDeclarationNode(const SubprogramDeclarationNode* node, FILE* dst);
  /** @brief 格式化子程序头节点 (SubprogramHeadNode) 为C函数头部 (返回类型、函数名、参数列表)。*/
  static void FormatSubprogramHeadNode(const SubprogramHeadNode* node, FILE* dst);
  /** @brief 格式化形式参数节点 (FormalParamNode) (即参数列表的括号)。*/
  static void FormatFormalParamNode(const FormalParamNode* node, FILE* dst);
  /** @brief 格式化参数列表节点 (ParamListsNode) (逗号分隔的参数声明组)。*/
  static void FormatParamListsNode(const ParamListsNode* node, FILE* dst);
  /** @brief 格式化VAR参数节点 (VarParamNode) 为C的指针参数。*/
  static void FormatVarParamNode(const VarParamNode* node, FILE* dst);
  /** @brief 格式化值参数节点 (ValueParamNode) 为C的值参数 (可带指针用于VAR参数)。*/
  static void FormatValueParamNode(const ValueParamNode* node, bool ref, FILE* dst);
  /** @brief 格式化值参数节点 (ValueParamNode) 为C的值参数 (重载版本，ref默认为false)。*/
  static void FormatValueParamNode(const ValueParamNode* node, FILE* dst); // Overload
  /** @brief 格式化语句节点 (StatementNode) 为C的相应语句。*/
  static void FormatStatementNode(const StatementNode* node, FILE* dst);
  /** @brief 格式化Else分支节点 (ElseNode) 为C的 `else` 块。*/
  static void FormatElseNode(const ElseNode* node, FILE* dst);
  /** @brief 格式化过程调用节点 (ProcedureCallNode) 为C的函数调用语句。*/
  static void FormatProcedureCallNode(const ProcedureCallNode* node, FILE* dst);
  /** @brief 格式化变量引用节点 (VariableNode) (可带 `&` 用于传递地址)。*/
  static void FormatVariableNode(const VariableNode* node, bool ref, FILE* dst);
  /** @brief 格式化变量列表节点 (VariableListNode) (用于 `scanf` 等的参数列表)。*/
  static void FormatVariableListNode(const VariableListNode* node, FILE* dst);
  /** @brief 格式化变量列表节点 (VariableListNode) (可带 `&` 用于传递地址)。*/
  static void FormatVariableListNode(const VariableListNode* node, bool ref, FILE* dst);
  /** @brief 格式化表达式列表节点 (ExpressionListNode) (用于函数调用参数、`printf`等)。*/
  static void FormatExpressionListNode(const ExpressionListNode* node, FILE* dst);
  /** @brief 格式化表达式节点 (ExpressionNode)。*/
  static void FormatExpressionNode(const ExpressionNode* node, FILE* dst);
  /** @brief 格式化项节点 (TermNode) (表达式的一部分)。*/
  static void FormatTermNode(const TermNode* node, FILE* dst);
  /** @brief 格式化因子节点 (FactorNode) (表达式的最基本单元)。*/
  static void FormatFactorNode(const FactorNode* node, FILE* dst);
  /** @brief 格式化变量部分访问节点 (IDVarPartNode) (数组下标 `[]` 或记录字段 `.`)。*/
  static void FormatIDVarPartNode(const IDVarPartNode* node, FILE* dst);
  /** @brief 辅助函数：将可能是嵌套的 ExpressionListNode 扁平化输出 (例如用于多维数组)。*/
  static void FormatFlattenedExpressionList(Node* node, FILE* dst);
  /** @brief 格式化程序体节点 (ProgramBodyNode) (包含声明和 `main` 函数的复合语句)。*/
  static void FormatProgramBodyNode(const ProgramBodyNode* node, FILE* dst);

  /**
   * @brief 中心调度函数，根据节点的实际类型调用相应的 `FormatXxxNode` 方法。
   * @param node 指向要格式化的通用 `Node` 指针。
   * @param dst 目标文件流指针。
   * @details 使用 `dynamic_cast`尝试将 `Node` 转换为具体的派生类型，
   *          然后调用该类型的专用格式化方法。如果无特定方法，则默认遍历并格式化其子节点。
   */
  static void DispatchFormat(const Node* node, FILE* dst);

  /**
   * @brief 格式化整个程序 (AST)。
   * @param ast_ptr 指向包含整个程序AST的 `AST` 对象。
   * @param dst 目标文件流指针。
   * @details 首先调用 `FormatStdLib` 输出库定义，然后调用 `DispatchFormat` 格式化AST的根节点。
   */
  static void FormatProgram(const AST* ast_ptr, FILE* dst);

 private:
  /**
   * @brief 检查是否有任何数学相关的库函数 (sin, cos, sqrt等) 被标记为已调用。
   * @return 如果至少有一个数学库函数被调用，则为 `true`；否则为 `false`。
   * @details 用于决定是否需要在生成的C代码中 `#include <math.h>`。
   */
  bool CheckMathLib();

  /**
   * @brief 内部结构体，用于存储单个库函数的信息。
   */
  struct Lib {
    std::string def_; ///< 库函数的C语言宏定义字符串。
    bool called_;     ///< 标记该库函数是否在当前编译中被调用。
  };
  /**
   * @brief 存储所有已知标准库函数的映射表。
   * @details键是库函数的名称 (如 "abs")，值是 `Lib` 结构体，包含其定义和调用状态。
   */
  std::unordered_map<std::string, Lib> lib_map_;
};

#endif  // P2C-Transpiler_CODE_GENERATION_H 