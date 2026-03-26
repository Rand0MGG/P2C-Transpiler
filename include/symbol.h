/**
 * @file symbol.h
 * @brief 定义编译器符号表中使用的各种符号对象。
 *
 * 该文件包含了符号的基类 `SymbolTemplate`，以及派生出的具体符号类型，
 * 如常量符号 `ConstSymbol` 和函数符号 `FunctionSymbol`。
 * 这些类用于在编译过程中存储和管理关于变量、常量、函数等程序实体的信息。
 */
#ifndef P2C-Transpiler_SYMBOL_H
#define P2C-Transpiler_SYMBOL_H

#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "type.h"

/**
 * @brief 符号对象的基类。
 *
 * 代表程序中的一个符号实体，例如变量、常量或函数。
 * 存储了符号的名称、类型、声明行号等基本信息。
 */
class SymbolTemplate {
 public:
  /**
   * @brief 定义符号的具体种类。
   */
  enum class SYMBOL_TYPE {
    VAR,     ///< 变量符号
    CONST,   ///< 常量符号
    FUNCTION ///< 函数符号
  };

  /**
   * @brief 默认构造函数。
   */
  SymbolTemplate() {}

  /**
   * @brief 构造一个新的 SymbolTemplate 对象。
   * @param name 符号的名称。
   * @param type 指向符号类型的 `TypeTemplate` 指针。
   * @param decl_line 符号声明所在的行号。
   * @param is_ref 标记是否为引用传递（主要用于变量），默认为 `false`。
   */
  SymbolTemplate(std::string name, TypeTemplate* type, int decl_line,
               bool is_ref = false)
      : name_(name), type_(type), decl_line_(decl_line), is_ref_(is_ref) {
    symbol_type_ = SYMBOL_TYPE::VAR; // 默认为变量类型，子类会覆盖
  }

  /**
   * @brief 虚析构函数。
   * @details 确保通过基类指针删除派生类对象时，能够正确调用派生类的析构函数。
   */
  virtual ~SymbolTemplate() {}

  // getter and setter
  /**
   * @brief 获取符号的名称。
   * @return `std::string` 符号名称。
   */
  std::string name() { return name_; }

  /**
   * @brief 获取符号的类型。
   * @return `TypeTemplate*` 指向符号类型的指针。
   */
  TypeTemplate* type() { return type_; }

  /**
   * @brief 获取符号声明所在的行号。
   * @return `int` 声明行号。
   */
  int decl_line() { return decl_line_; }

  /**
   * @brief 设置符号是否为引用。
   * @param r 如果是引用则为 `true`，否则为 `false`。
   * @note 对于常量和函数符号，此属性通常无意义或固定为 `false`。
   *       此方法主要用于变量符号。
   */
  void set_ref(bool r) {
    if (symbol_type_ == SYMBOL_TYPE::VAR) {
      is_ref_ = r;
    } else {
      // 对于常量和函数，is_ref_ 应该固有地为 false。
      // SymbolTemplate 构造函数中 is_ref 的默认值为 false,
      // ConstSymbol/FunctionSymbol 构造函数也以这种方式调用它。
      // 因此，如果 symbol_type_ 在构造后可能改变（当前设计并非如此），
      // 在此处显式设置为 false 更多的是一种保障或澄清。
      // 根据当前的构造函数，它们（常量和函数）的 is_ref_ 已经为 false。
      is_ref_ = false;
    }
  }

  /**
   * @brief 检查符号是否为引用。
   * @return 如果是变量符号且为引用，则为 `true`；否则为 `false`。
   */
  bool is_ref() {
    if (symbol_type_ == SYMBOL_TYPE::VAR) {
      return is_ref_;
    }
    return false; // 常量和函数不视为传统意义上的引用参数 (is_ref)
  }

  /**
   * @brief 获取符号的种类 (变量, 常量, 函数)。
   * @return `SYMBOL_TYPE` 符号的种类。
   */
  SYMBOL_TYPE symbol_type() { return symbol_type_; }

 protected:
  std::string name_;         ///< 符号的名称
  TypeTemplate* type_;     ///< 指向符号类型的指针
  bool is_ref_;            ///< 标记是否为引用 (主要用于变量)
  int decl_line_;          ///< 符号声明所在的行号
  SYMBOL_TYPE symbol_type_; ///< 符号的种类
};

/**
 * @brief 常量符号。
 *
 * 继承自 `SymbolTemplate`，专门用于表示程序中的常量。
 * 额外存储了常量的值。
 */
class ConstSymbol : public SymbolTemplate {
 public:
  /**
   * @brief 默认构造函数。
   */
  ConstSymbol() {}

  /**
   * @brief 构造一个新的 ConstSymbol 对象。
   * @param name 常量的名称。
   * @param value 常量的值，类型为 `ConstValue`。
   * @param decl_line 常量声明所在的行号。
   */
  ConstSymbol(std::string name, ConstValue value, int decl_line)
      : SymbolTemplate(name, value.type(), decl_line), value_(value) {
    symbol_type_ = SYMBOL_TYPE::CONST;
  }

  /**
   * @brief 析构函数。
   */
  ~ConstSymbol() {}

  // getter and stter
  /**
   * @brief 获取常量的值。
   * @return `ConstValue` 常量的值。
   */
  ConstValue value() { return value_; }

  /**
   * @brief 获取常量的基础类型。
   * @return `BasicType*` 指向常量基础类型的指针。
   *         注意：常量通常具有基础类型。
   */
  BasicType* type() { return value_.type(); }

 private:
  ConstValue value_; ///< 常量的值
};

/**
 * @brief 函数符号。
 *
 * 继承自 `SymbolTemplate`，专门用于表示程序中的函数或过程。
 * 额外存储了函数的参数信息列表。
 */
class FunctionSymbol : public SymbolTemplate {
 public:
  /**
   * @brief 定义函数参数的传递模式。
   */
  enum class PARAM_MODE {
    VALUE,     ///< 值传递
    REFERENCE, ///< 引用传递 (VAR 参数)
  };

  /**
   * @brief 存储函数单个参数的信息。
   */
  struct ParameterInfo {
    std::string name; ///< 参数的名称
    BasicType* type;  ///< 指向参数类型的指针 (通常为基础类型)
    PARAM_MODE mode;  ///< 参数的传递模式 (值传递或引用传递)
  };

  /**
   * @brief 默认构造函数。
   */
  FunctionSymbol() {}

  /**
   * @brief 拷贝构造函数。
   * @param f 要拷贝的 `FunctionSymbol` 对象。
   */
  FunctionSymbol(const FunctionSymbol& f);

  /**
   * @brief 构造一个新的 FunctionSymbol 对象 (带参数列表)。
   * @param name 函数的名称。
   * @param return_type 指向函数返回类型的指针 (通常为 `BasicType*`)。
   *                    对于过程 (procedure)，可以传入 `TYPE_NONE` 或特定表示无返回值的类型。
   * @param decl_line 函数声明所在的行号。
   * @param params 函数的参数信息列表。
   */
  FunctionSymbol(std::string name, BasicType* return_type, int decl_line,
                 const std::vector<ParameterInfo>& params);

  /**
   * @brief 构造一个新的 FunctionSymbol 对象 (无参数)。
   * @param name 函数的名称。
   * @param return_type 指向函数返回类型的指针。
   * @param decl_line 函数声明所在的行号。
   */
  FunctionSymbol(std::string name, BasicType* return_type, int decl_line);

  /**
   * @brief 获取函数参数的数量。
   * @return `int` 参数数量。
   */
  int param_size() { return params_.size(); }

  /**
   * @brief 获取指定位置的参数信息。
   * @param pos 参数在列表中的索引 (从0开始)。
   * @return `const ParameterInfo*` 指向参数信息的指针，如果位置无效可能返回行为未定义或错误。
   *         调用者应确保 `pos` 在有效范围内。
   */
  const ParameterInfo* ParamAt(int pos) { return &params_[pos]; }

  /**
   * @brief 生成并返回一个表示函数参数类型和模式的字符串。
   * @return `std::string` 例如 "integer, char&" (引用传递会追加 '&')。
   */
  std::string ParamName();

  /**
   * @brief 向函数符号中插入一个新的参数信息。
   * @param p 要插入的参数的 `ParameterInfo` 对象。
   * @return 如果参数名不存在且插入成功，返回 `true`；如果参数名已存在，返回 `false`。
   */
  bool InsertParam(const ParameterInfo& p);

  /**
   * @brief 断言传入的参数类型和值类型是否与函数定义的参数匹配。
   * @param params_in 调用时传入的参数类型列表。
   * @param value_type_in 标记对应 `params_in` 中的每个参数是否为左值/可传递引用。
   *                      `true` 表示是值或可以安全地按值传递，
   *                      `false` 表示可能需要引用传递（例如变量本身）。
   * @return 如果所有参数都匹配，返回 `true`；否则返回 `false`。
   */
  bool AssertParamsMatch(const std::vector<TypeTemplate*>& params_in,
                         const std::vector<bool> value_type_in);

  /**
   * @brief 断言函数是否没有参数。
   * @return 如果函数没有参数，返回 `true`；否则返回 `false`。
   */
  bool AssertHasNoParams();

  /**
   * @brief 根据参数名称获取参数信息。
   * @param name 要查找的参数的名称。
   * @return `const ParameterInfo*` 指向参数信息的指针；如果未找到，返回 `nullptr`。
   */
  const ParameterInfo* GetParamInfo(std::string name);

  /**
   * @brief 检查指定名称的参数是否为引用传递。
   * @param name 参数的名称。
   * @return 如果参数存在且为引用传递，返回 `true`；否则返回 `false`。
   */
  bool IsReference(std::string name);

  /**
   * @brief 获取一个布尔向量，表示每个参数是否为引用传递。
   * @return `std::vector<bool>` 向量中每个元素对应一个参数，`true` 表示引用传递。
   */
  std::vector<bool> ParamRefVec();

 private:
  std::vector<ParameterInfo> params_; ///< 存储函数所有参数信息的列表
  std::unordered_map<std::string, int> param_map_; ///< 参数名到其在 `params_` 列表中索引的映射，用于快速查找
};

#endif  // P2C-Transpiler_SYMBOL_H
