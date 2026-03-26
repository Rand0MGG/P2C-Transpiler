#ifndef P2C-Transpiler_TYPE_H_
#define P2C-Transpiler_TYPE_H_

#include <string.h>

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <variant>

/**
 * @brief 所有类型的基类模板。
 * @details 定义了类型的基本结构和通用操作，如动态类型转换、获取类型名称等。
 */
class TypeTemplate {
 public:
  /**
   * @brief 枚举类，定义了类型模板的基本种类。
   */
  enum class TYPE {
    BASIC,   ///< 基础类型，例如 integer, real, char, boolean
    RECORD,  ///< 记录类型 (Pascal中的 RECORD)
    ARRAY    ///< 数组类型 (Pascal中的 ARRAY)
  };
  TypeTemplate(); ///< 默认构造函数。
  TypeTemplate(TYPE template_type); ///< 构造函数，根据指定的类型种类初始化。
  virtual ~TypeTemplate(); ///< 虚析构函数，确保派生类对象能被正确销毁。

  /**
   * @brief 模板方法，用于将当前对象动态转换为指定的类型 `T`。
   * @tparam T 目标类型。
   * @return 如果转换成功，返回指向 `T` 类型的指针；否则返回 `nullptr`。
   * @example BasicType* bt = type_ptr->DynamicCast<BasicType>();
   */
  template <typename T>
  T *DynamicCast() {
    return dynamic_cast<T *>(this);
  }

  /**
   * @brief 模板方法，用于将当前对象动态转换为指定的常量类型 `T`。
   * @tparam T 目标类型。
   * @return 如果转换成功，返回指向常量 `T` 类型的指针；否则返回 `nullptr`。
   */
  template <typename T>
  const T *DynamicCast() const {
    return dynamic_cast<const T *>(this);
  }

  /**
   * @brief 获取当前类型模板的种类。
   * @return 类型模板的种类 (TYPE)。
   * @example if (type_ptr->template_type() == TypeTemplate::TYPE::BASIC) { ... }
   */
  TYPE template_type() const { return template_type_; }

  /**
   * @brief 纯虚函数，获取类型的字符串表示。
   * @details 派生类必须实现此方法，以返回其特定类型的名称。
   * @return 类型的字符串名称。例如，`BasicType` 可能返回 "integer", "real" 等。
   */
  virtual std::string name() = 0;

  /**
   * @brief 检查类型是否类似字符串（即字符数组）。
   * @details 例如，`array [1..10] of char` 就是类字符串类型。
   * @return 如果是类字符串类型则返回 `true`，否则返回 `false`。
   */
  bool StringLike();

  /**
   * @brief 检查当前类型是否为预定义的全局基础类型之一。
   * @details 例如 `TYPE_INT`, `TYPE_REAL`, `TYPE_BOOL`, `TYPE_CHAR`。
   * @return 如果是特定的全局基础类型则返回 `true`，否则返回 `false`。
   */
  bool isSpecificBasicType() const;

  /**
   * @brief 比较当前类型是否与另一个类型 `other` 相同。
   * @param other 指向待比较的另一个类型的指针。
   * @return 如果类型相同则返回 `true`，否则返回 `false`。
   * @details 对于数组类型，会比较其结构和元素类型；对于基础类型，通常比较指针是否指向同一个全局单例对象。
   */
  virtual bool isSameAs(const TypeTemplate* other) const;

 protected:
  TYPE template_type_;  ///< 当前类型模板的种类。
};

/**
 * @brief 基础类型，包括 Pascal 中的 INTEGER, REAL, CHAR, BOOLEAN。
 */
class BasicType : public TypeTemplate {
 public:
  /**
   * @brief 枚举类，定义了基础类型的具体种类。
   */
  enum class BASIC_TYPE {
    INT,   ///< 整型 (integer)
    REAL,  ///< 实型 (real)
    BOOL,  ///< 布尔型 (boolean)
    CHAR,  ///< 字符型 (char)
    NONE   ///< 空类型或未定义类型
  };

  BasicType(); ///< 默认构造函数，默认为 `NONE` 类型。
  BasicType(BASIC_TYPE basic_type); ///< 构造函数，根据指定的基础类型种类初始化。

  ~BasicType(); ///< 析构函数。

  /**
   * @brief 获取基础类型的具体种类。
   * @return 基础类型的种类 (BASIC_TYPE)。
   * @example if (basic_type_ptr->type() == BasicType::BASIC_TYPE::INT) { ... }
   */
  BASIC_TYPE type() const { return basic_type_; }

  /**
   * @brief 获取该基础类型在目标C语言中对应的名称。
   * @return C语言中的类型名称字符串。例如，Pascal的 `integer` 对应C的 `int`。
   */
  std::string c_name() const {
    switch (basic_type_) {
      case BASIC_TYPE::INT:
        return "int";
      case BASIC_TYPE::REAL:
        return "float"; // Pascal real通常映射为C的float或double
      case BASIC_TYPE::BOOL:
        return "int"; // Pascal boolean通常映射为C的int (0或1)
      case BASIC_TYPE::CHAR:
        return "char";
      default:
        return "void"; // 通常表示类型不适用或错误
    }
  }
  /**
   * @brief 获取基础类型的Pascal名称。
   * @return Pascal中的类型名称字符串。例如："integer", "real"。
   */
  std::string name() override;

  /**
   * @brief 计算当前基础类型与另一个基础类型 `other` 在给定操作符 `op` 下的运算结果类型。
   * @param other 指向另一个基础类型的指针。
   * @param op 操作符字符串 (例如 "+", "-", "and", "or")。
   * @return 运算结果的类型（`BasicType*`）。如果操作无效或类型不兼容，可能返回 `TYPE_ERROR`。
   * @example TYPE_INT->computeWith(TYPE_REAL, "+") // 结果应为 TYPE_REAL
   */
  BasicType* computeWith(const BasicType* other, const std::string& op) const;

  /**
   * @brief 计算当前基础类型在给定一元操作符 `op` 下的运算结果类型。
   * @param op 一元操作符字符串 (例如 "-", "not")。
   * @return 运算结果的类型（`BasicType*`）。如果操作无效，可能返回 `TYPE_ERROR`。
   * @example TYPE_INT->computeUnary("-") // 结果应为 TYPE_INT
   */
  BasicType* computeUnary(const std::string& op) const;

 private:
  BASIC_TYPE basic_type_; ///< 基础类型的具体种类。
};

/* **************** 全局基础类型单例 **************** */
// 这些是全局唯一的对象，代表Pascal中的标准基础类型。
// 程序中所有对这些类型的引用都应指向这些单例，以便通过指针比较判断类型是否相等。
extern BasicType *TYPE_INT;        ///< 全局整型单例 (对应 Pascal INTEGER)
extern BasicType *TYPE_REAL;       ///< 全局实型单例 (对应 Pascal REAL)
extern BasicType *TYPE_BOOL;       ///< 全局布尔型单例 (对应 Pascal BOOLEAN)
extern BasicType *TYPE_CHAR;       ///< 全局字符型单例 (对应 Pascal CHAR)
extern BasicType *TYPE_NONE;       ///< 全局空类型/未定义类型单例
extern BasicType *TYPE_ERROR;      ///< 全局错误类型单例 (通常为 nullptr)，表示类型检查失败或无效操作
extern BasicType *TYPE_STRINGLIKE; ///< 全局类字符串类型单例 (通常指代 `array of char` 的基类型 CHAR)

/**
 * @brief 数组类型 (Pascal ARRAY)。
 */
class ArrayType : public TypeTemplate {
 public:
  /**
   * @brief 结构体，表示数组某一维度的边界。
   * @details 包含该维度索引的类型（通常是整数或字符）、下界和上界。
   */
  struct ArrayBound {
    BasicType *type_; ///< 维度索引的类型，例如 `TYPE_INT` (for `1..10`) 或 `TYPE_CHAR` (for `'a'..'z'`)。
    int lb_;          ///< 下界 (lower bound)。
    int ub_;          ///< 上界 (upper bound)。

    ArrayBound(); ///< 默认构造函数。
    ArrayBound(const ArrayBound &b2); ///< 拷贝构造函数。
    ArrayBound &operator=(const ArrayBound &b2); ///< 赋值运算符。

    /**
     * @brief 获取维度边界的字符串表示。
     * @return 形如 "[下界..上界]" 的字符串。例如 `[1..10]` 或 `['a'..'z']`。
     */
    std::string name();

    /**
     * @brief 比较两个数组维度边界是否完全相同。
     * @param b2 另一个要比较的 ArrayBound 对象。
     * @return 如果索引类型、下界和上界都相同，则返回 `true`，否则返回 `false`。
     */
    bool operator==(const ArrayBound &b2) const {
      return type_ == b2.type_ && lb_ == b2.lb_ && ub_ == b2.ub_;
    }
  };

  ArrayType(); ///< 默认构造函数，基类型为 `TYPE_NONE`。
  ArrayType(TypeTemplate *type); ///< 构造函数，指定数组的基类型（元素类型）。
  ArrayType(TypeTemplate *type, std::vector<ArrayBound> bounds); ///< 构造函数，指定数组的基类型和所有维度的边界。
  ArrayType(const ArrayType &a2); ///< 拷贝构造函数。
  ~ArrayType(); ///< 析构函数。

  /**
   * @brief 获取数组的基类型（元素类型）。
   * @return 指向基类型的指针。
   * @example 对于 `array [1..10] of integer`，基类型是 `TYPE_INT`。
   */
  TypeTemplate *base_type() { return base_type_; }

  /**
   * @brief 检查数组类型是否有效。
   * @return 如果基类型不是 `TYPE_NONE` 或 `TYPE_ERROR`，则返回 `true`，否则返回 `false`。
   */
  bool Valid() { return base_type_ != TYPE_NONE && base_type_ != TYPE_ERROR; }

  /**
   * @brief 获取数组的维度数量。
   * @return 维度数量。
   */
  size_t dims() const { return bounds_.size(); }

  /**
   * @brief 获取所有维度边界的引用。
   * @return 一个包含所有维度边界的 `std::vector` 的引用。
   */
  std::vector<ArrayBound> &bounds() { return bounds_; }

  /**
   * @brief 获取第 `i` 个维度的边界（从0开始计数）。
   * @param i 维度索引。
   * @return 第 `i` 个维度的 `ArrayBound` 对象。
   */
  ArrayBound bound(size_t i) { return bounds_[i]; }

  /**
   * @brief 检查数组是否为类字符串类型。
   * @param access_layer 访问层数，默认为0。用于处理多维数组访问的情况，
   *                     判断在访问了 `access_layer` 层后，剩余的是否为一维字符数组。
   * @return 如果是类字符串类型，则返回 `true`，否则 `false`。
   * @example `array [1..10] of char` 调用 `StringLike(0)` 返回 `true`。
   * @example `array [1..5] of array [1..10] of char` 调用 `StringLike(0)` 返回 `false`，
   *          但如果通过一层索引访问后，例如 `arr[i].StringLike(1)`，则返回 `true`。
   */
  bool StringLike(
      int access_layer = 0);
  /**
   * @brief 获取数组类型的字符串表示。
   * @return 数组类型的名称字符串。例如：`array[1..10] of integer`。
   */
  std::string name() override;

  ArrayType &operator=(const ArrayType &a2); ///< 赋值运算符。

  /**
   * @brief 比较两个数组类型是否完全相同。
   * @param a2 另一个要比较的 `ArrayType` 对象。
   * @return 如果基类型和所有维度边界都相同，则返回 `true`，否则返回 `false`。
   */
  bool operator==(const ArrayType &a2) const;

  /**
   * @brief 根据提供的索引类型列表 `v_types` 访问（或"剥离"）多维数组，获取子数组或元素类型。
   * @param v_types 一个 `TypeTemplate*` 的向量，代表每个维度的索引实际使用的类型。
   * @return 访问指定层数后的数组子类型。如果提供的索引类型与数组对应维度的索引类型不匹配，
   *         或索引数量超过维度，则返回元素为 `TYPE_ERROR` 的 `ArrayType`。
   * @example 设 `a` 类型为 `array[1..5] of array['a'..'c'] of integer`。
   *          若要模拟访问 `a[3]`，则 `v_types` 可能包含一个 `TYPE_INT`。
   *          结果类型为 `array['a'..'c'] of integer`。
   */
  ArrayType Visit(std::vector<TypeTemplate *> v_types) const;

  /**
   * @brief 访问（或"剥离"）数组的前 `v_layer` 个维度，返回剩余维度的数组类型。
   * @param v_layer 要剥离的维度层数。
   * @return 剥离 `v_layer` 层维度后的新 `ArrayType`。
   *         如果 `v_layer` 为0，返回自身的拷贝。
   *         如果 `v_layer` 大于数组维度，返回元素为 `TYPE_ERROR` 的 `ArrayType`。
   * @example 对于 `array [1..10][1..5] of integer`，`Visit(1)` 返回 `array [1..5] of integer`。
   */
  ArrayType Visit(unsigned int v_layer) const;

  // New methods for type comparison (这些是在后续合并进来的，注释也应相应更新)
  /**
   * @brief 比较当前数组从第 `vdim1` 维开始的子数组是否与另一个数组 `other` 从第 `vdim2` 维开始的子数组类型相同。
   * @param vdim1 当前数组开始比较的维度层数（访问几层后）。
   * @param other 指向另一个 `ArrayType` 的指针。
   * @param vdim2 另一个数组开始比较的维度层数。
   * @return 如果子数组类型相同，则返回 `true`，否则 `false`。
   */
  bool isSameAs(int vdim1, const ArrayType* other, int vdim2) const;

  /**
   * @brief 比较当前数组在访问 `vdim` 层后得到的子数组（如果最终为元素类型），是否与给定的基础类型 `other` 相同。
   * @param vdim 当前数组开始比较的维度层数。
   * @param other 指向一个 `BasicType` 的指针。
   * @return 如果访问后的类型与 `other` 相同（会将 `other` 包装成临时单元素数组进行比较），则返回 `true`。
   */
  bool isSameAsVisited(int vdim, const BasicType* other) const;

 private:
  TypeTemplate *base_type_;         ///< 数组的基类型（元素类型）。可以是 `BasicType` 或 `RecordType`。
  std::vector<ArrayBound> bounds_;  ///< 存储数组所有维度边界信息的向量。
};

/**
 * @brief 记录类型 (Pascal RECORD)。
 */
class RecordType : public TypeTemplate {
 public:
  RecordType(); ///< 默认构造函数。
  RecordType(std::unordered_map<std::string, TypeTemplate *> types_map); ///< 构造函数，使用给定的字段名到类型的映射初始化记录。
  ~RecordType(); ///< 析构函数。

  /**
   * @brief 获取记录类型的字符串表示。
   * @return 记录类型的名称字符串。例如：`record x:integer; y:real; end`。
   */
  std::string name() override;

  /**
   * @brief 向记录类型中添加一个字段。
   * @param name 字段的名称。
   * @param type 指向字段类型的指针。
   * @return 如果成功添加（即不存在同名字段），返回 `true`；否则返回 `false`。
   */
  bool add(std::string name, TypeTemplate *type);

  /**
   * @brief 根据字段名查找字段类型。
   * @param name 要查找的字段名称。
   * @return 如果找到，返回指向字段类型的指针；否则返回 `nullptr`。
   */
  TypeTemplate *Find(std::string name) const;

  /**
   * @brief 根据字段名列表访问嵌套记录类型中的成员类型。
   * @param names 字段名列表，按访问顺序排列。
   * @return 如果成功访问到最内层成员，返回其类型指针；如果任何一步查找失败或类型不匹配，返回 `nullptr`。
   * @example 对于 `record r1: record r2: integer end end;`，调用 `Visit({"r1", "r2"})` 将返回 `integer` 类型。
   */
  TypeTemplate *Visit(std::vector<std::string> names) const;

  // New methods for type comparison (这些是在后续合并进来的，注释也应相应更新)
  /**
   * @brief 比较当前记录类型通过 `names1` 访问到的成员类型，是否与另一个记录类型 `other` 通过 `names2` 访问到的成员类型相同。
   * @param names1 用于访问当前记录类型成员的字段名列表。
   * @param other 指向另一个 `RecordType` 的指针。
   * @param names2 用于访问 `other` 记录类型成员的字段名列表。
   * @return 如果对应的成员类型相同，则返回 `true`，否则 `false`。
   */
  bool isSameAs(const std::vector<std::string>& names1, const RecordType* other, const std::vector<std::string>& names2) const;

  /**
   * @brief 比较当前记录类型通过 `names1` 访问到的成员类型，是否与给定的类型 `other` 相同。
   * @param names1 用于访问当前记录类型成员的字段名列表。
   * @param other 指向待比较的任意类型的指针。
   * @return 如果访问到的成员类型与 `other` 类型相同，则返回 `true`，否则 `false`。
   */
  bool isSameAsVisited(const std::vector<std::string>& names1, const TypeTemplate* other) const;

 private:
  /// 存储记录中所有字段及其类型的映射。键是字段名，值是指向字段类型的指针。
  std::unordered_map<std::string, TypeTemplate *> types_map_;
};

/* **************** 全局指针收集器 **************** */
// 用于管理动态分配的 TypeTemplate 派生类对象 (ArrayType, RecordType) 的生命周期，
// 以便在程序结束时统一释放，避免内存泄漏。基础类型是全局单例，不需要收集。
extern std::vector<TypeTemplate *> *PTR_COLLECTOR; ///< 全局指针收集器实例。

/**
 * @brief 将动态创建的非基础类型指针 `t` 加入到 `PTR_COLLECTOR` 中进行管理。
 * @param t 指向动态分配的 `TypeTemplate` 派生类对象的指针。
 * @details 基础类型 (`BasicType`) 和错误类型 (`TYPE_ERROR`) 不需要也不应该被收集。
 */
static void PtrCollect(TypeTemplate *t) {
  if (t == TYPE_ERROR || t->template_type() == TypeTemplate::TYPE::BASIC)
    return;
  PTR_COLLECTOR->push_back(t);
}

/**
 * @brief 释放 `PTR_COLLECTOR` 中收集的所有类型对象的内存。
 * @details 通常在程序退出前调用。
 */
static void ReleaseTemp() {
  for (auto t : *PTR_COLLECTOR) {
    delete t;
  }
}

/**
 * @brief 辅助类，用于表示一个具体的运算操作。
 * @details 包含操作数类型和操作符，主要用作 `OperationMap` 的键。
 */
class Operation {
 public:
  Operation(); ///< 默认构造函数。
  /**
   * @brief 构造一个操作。
   * @param in_type1 第一个操作数的类型 (BasicType*)。
   * @param in_type2 第二个操作数的类型 (BasicType*)。对于一元运算，此参数可能为 `nullptr`。
   * @param op 操作符字符串 (例如："+", "-", "and", "or", "not")。
   */
  Operation(BasicType *in_type1, BasicType *in_type2, std::string op)
      : in_type1(in_type1), in_type2(in_type2), op(std::move(op)) {}
  ~Operation(); ///< 析构函数。 (原始实现中将指针置nullptr，但通常不需要这样做，因为它们不拥有对象)

  /**
   * @brief 比较两个操作是否相同（操作数类型和操作符都相同）。
   */
  bool operator==(const Operation &other) const {
    return in_type1 == other.in_type1 && in_type2 == other.in_type2 &&
           op == other.op;
  }
  BasicType *in_type1; ///< 第一个操作数的类型。
  BasicType *in_type2; ///< 第二个操作数的类型 (一元运算时可能为 `nullptr`)。
  std::string op;      ///< 操作符字符串。
};

/**
 * @brief 为 `Operation` 对象提供哈希函数。
 * @details 使 `Operation` 对象可以作为 `std::unordered_map` 的键。
 */
struct OperationHash {
  std::size_t operator()(const Operation &k) const {
    // 一个简单的哈希组合方式
    return ((std::hash<BasicType *>()(k.in_type1) ^
             (std::hash<BasicType *>()(k.in_type2) >> 1)) >>
            1) ^
           std::hash<std::string>()(k.op);
  }
};

/// 类型别名：定义了从 `Operation` 到结果 `BasicType*` 的哈希映射。
/// 用于存储各种操作（如算术运算、逻辑运算）的类型推导规则。
/// 例如，(integer + real) -> real。
typedef std::unordered_map<Operation, BasicType *, OperationHash> OperationMap;
extern OperationMap operation_map; ///< 全局的 `OperationMap` 实例，存储了预定义的类型运算规则。

/********************************************
 *             类型断言与名称辅助函数             *
 ********************************************/
/**
 * @brief 获取单个类型 `t` 的字符串名称。
 * @param t 指向 `TypeTemplate` 对象的指针。
 * @return 类型的字符串表示。对 `TYPE_ERROR` 返回 "error"，对 `TYPE_STRINGLIKE` 返回 "stringlike"。
 */
std::string type_name(TypeTemplate* t);

/**
 * @brief 获取类型向量（列表） `t` 中所有类型的字符串名称，并用逗号加空格分隔。
 * @param t 一个 `TypeTemplate*` 的向量。
 * @return 组合后的类型名称字符串。例如："integer, real, boolean"。
 */
std::string type_name(const std::vector<TypeTemplate*> &t);

/**
 * @brief 表示一个常量值，可以存储不同基础类型的值。
 * @details 内部使用 `std::variant` 来存储实际值，并记录其对应的 `BasicType`。
 *          支持不同类型常量之间的基本算术运算。
 */
class ConstValue {
 public:
  ConstValue(); ///< 默认构造函数，默认值为整型0。
  ~ConstValue(); ///< 析构函数。
  ConstValue(const ConstValue &other); ///< 拷贝构造函数。

  /**
   * @brief 通过整数构造 `ConstValue`。
   * @param v 整数值。
   * @param to_float 如果为 `true`，则将该整数值存储为浮点类型。默认为 `false`。
   */
  ConstValue(int v, bool to_float = false);
  ConstValue(float v);  ///< 通过浮点数构造 `ConstValue`。
  ConstValue(bool v);   ///< 通过布尔值构造 `ConstValue`。
  ConstValue(char v);   ///< 通过字符构造 `ConstValue`。
  ConstValue(const char *v); ///< 通过C风格字符串构造 `ConstValue` (内部转换为 `std::string`)。
  ConstValue(std::string v); ///< 通过 `std::string` 构造 `ConstValue`。

  // 设置值的系列方法
  void set(int v);         ///< 设置值为整数。
  void set(float v);       ///< 设置值为浮点数。
  void set(bool v);        ///< 设置值为布尔值。
  void set(char v);        ///< 设置值为字符。
  void set(std::string v); ///< 设置值为字符串 (类型为 `TYPE_STRINGLIKE`)。

  /**
   * @brief 对 `ConstValue` 的值执行一元负操作 (取反或取负)。
   * @details 仅支持整型、实型。字符型的取负行为可能依赖具体实现（原代码有特殊处理）。
   *          如果类型不支持或内部状态不一致，可能抛出异常。
   */
  void set_unimus();

  /**
   * @brief 获取常量值的 `BasicType` 类型。
   * @return 指向该常量值对应 `BasicType` 的指针。
   */
  BasicType *type() const { return m_Type; }

  /**
   * @brief 模板方法，获取常量值的具体数据。
   * @tparam T 期望获取的数据类型 (例如 `int`, `float`, `char`, `bool`, `std::string`)。
   * @return 存储的值。
   * @throw std::runtime_error 如果请求的类型 `T` 与实际存储的类型不匹配，或内部数据不一致。
   * @example `int val = const_val.get<int>();`
   */
  template <typename T>
  T get() const;

  ConstValue &operator=(const ConstValue &other); ///< 赋值运算符。
  // 算术运算符，用于 ConstValue 对象之间的运算。
  // 运算时会进行类型检查，不匹配或不支持的操作会抛出 std::runtime_error。
  ConstValue operator+(const ConstValue &other); ///< 加法运算。
  ConstValue operator-(const ConstValue &other); ///< 减法运算。
  ConstValue operator*(const ConstValue &other); ///< 乘法运算。
  ConstValue operator/(const ConstValue &other); ///< 除法运算。

 private:
  BasicType *m_Type = nullptr; ///< 指向该常量值对应的 `BasicType`。例如 `TYPE_INT`, `TYPE_REAL`。
  /// 使用 `std::variant` 存储实际的常量值，可以是整型、浮点型、字符、布尔型或字符串。
  std::variant<int, float, char, bool, std::string> m_value;
};

#endif  // P2C-Transpiler_TYPE_H_