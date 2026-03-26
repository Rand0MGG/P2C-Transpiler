#include "type.h"
#include "log.h"
#include <vector>
#include "parser.h"

using std::string;
using std::vector;

// TypeTemplate Implementations
/**
 * @brief TypeTemplate 默认构造函数。
 */
TypeTemplate::TypeTemplate() : template_type_(TYPE::BASIC) {}

/**
 * @brief TypeTemplate 构造函数，根据指定的类型种类初始化。
 * @param template_type 类型的种类 (BASIC, RECORD, ARRAY)。
 */
TypeTemplate::TypeTemplate(TYPE template_type) : template_type_(template_type) {}

/**
 * @brief TypeTemplate 虚析构函数。
 * @details 确保通过基类指针删除派生类对象时，能够正确调用派生类的析构函数。
 */
TypeTemplate::~TypeTemplate() {}

/**
 * @brief 判断一个通用类型是否为类字符串类型。
 * @details 如果类型是数组类型，则调用数组类型的 `StringLike` 方法进行判断。
 *          其他类型（如基础类型、记录类型）本身不是类字符串类型。
 * @return 如果是类字符串类型，返回 `true`；否则返回 `false`。
 */
bool TypeTemplate::StringLike() {
  if (template_type_ == TYPE::ARRAY) {
    auto array_ptr = DynamicCast<ArrayType>(); 
    return array_ptr ? array_ptr->StringLike() : false;
  }
  return false;
}

/**
 * @brief 检查当前类型是否为几个预定义的全局基础类型之一。
 * @details 这几个全局基础类型包括 `TYPE_INT`, `TYPE_REAL`, `TYPE_BOOL`, `TYPE_CHAR`。
 *          通过直接比较指针实现，因为这些全局类型是单例。
 * @return 如果当前类型对象是这些全局基础类型之一，则返回 `true`；否则返回 `false`。
 */
bool TypeTemplate::isSpecificBasicType() const {
  if (template_type_ != TYPE::BASIC) {
    return false;
  }
  return this == TYPE_INT || this == TYPE_REAL || this == TYPE_BOOL || this == TYPE_CHAR;
}

/**
 * @brief 比较当前类型对象是否与另一个类型对象 `other` 相同。
 * @param other 指向待比较的另一个 `TypeTemplate` 对象的指针。
 * @return 如果类型相同，则返回 `true`；否则返回 `false`。
 */
bool TypeTemplate::isSameAs(const TypeTemplate* other) const {
  if (this == other) return true;
  if (other == nullptr) return false;
                                     
  if (this->template_type_ != other->template_type_) return false;
  
  if (this->template_type_ == TypeTemplate::TYPE::ARRAY) {
    auto a1 = dynamic_cast<const ArrayType*>(this);
    auto a2 = dynamic_cast<const ArrayType*>(other);
    if (a1 && a2) { 
        return (*a1) == (*a2); 
    } else {
        return false; 
    }
  }
  return false;
}

// BasicType Implementations
/**
 * @brief BasicType 默认构造函数。
 */
BasicType::BasicType() : TypeTemplate(TYPE::BASIC), basic_type_(BASIC_TYPE::NONE) {}

/**
 * @brief BasicType 构造函数，根据指定的基础类型种类初始化。
 * @param basic_type 基础类型的具体种类 (INT, REAL, BOOL, CHAR, NONE)。
 */
BasicType::BasicType(BASIC_TYPE basic_type)
    : TypeTemplate(TYPE::BASIC), basic_type_(basic_type) {}

/**
 * @brief BasicType 析构函数。
 */
BasicType::~BasicType() {}

/**
 * @brief 获取基础类型的 Pascal 风格名称。
 */
std::string BasicType::name() {
  switch (basic_type_) {
    case BASIC_TYPE::INT:
      return "integer";
    case BASIC_TYPE::REAL:
      return "real";
    case BASIC_TYPE::BOOL:
      return "boolean";
    case BASIC_TYPE::CHAR:
      return "char";
    default:
      return "none";
  }
}

/**
 * @brief 计算当前基础类型与另一个基础类型 `other` 在二元操作符 `op` 下的运算结果类型。
 */
BasicType* BasicType::computeWith(const BasicType* other, const std::string& op) const {
  if (this == TYPE_ERROR || other == TYPE_ERROR) return TYPE_ERROR;
  if (this == nullptr) return TYPE_ERROR; 

  auto res = operation_map.find(Operation(const_cast<BasicType*>(this), const_cast<BasicType*>(other), op));
  if (res != operation_map.end()) {
    return res->second;
  } else {
    return TYPE_ERROR;
  }
}

/**
 * @brief 计算当前基础类型在一元操作符 `op` 下的运算结果类型。
 */
BasicType* BasicType::computeUnary(const std::string& op) const {
  if (this == TYPE_ERROR) return TYPE_ERROR;
  if (this == nullptr) return TYPE_ERROR;

  auto res = operation_map.find(Operation(const_cast<BasicType*>(this), nullptr, op));
  if (res != operation_map.end()) {
    return res->second;
  } else {
    return TYPE_ERROR;
  }
}

// ArrayType::ArrayBound Implementations
/**
 * @brief ArrayBound 默认构造函数。
 */
ArrayType::ArrayBound::ArrayBound() : type_(TYPE_NONE), lb_(0), ub_(0) {}

/**
 * @brief ArrayBound 拷贝构造函数。
 * @param b2 要拷贝的 ArrayBound 对象。
 */
ArrayType::ArrayBound::ArrayBound(const ArrayBound &b2)
    : type_(b2.type_), lb_(b2.lb_), ub_(b2.ub_) {}

/**
 * @brief 获取数组维度边界的字符串表示。
 */
std::string ArrayType::ArrayBound::name() {
  if(type_ != TYPE_CHAR && type_ != TYPE_INT) {
    return "[error]";
  }
  if(type_ == TYPE_CHAR){
    return "[" + string(1, static_cast<char>(lb_)) + ".." + string(1, static_cast<char>(ub_)) + "]";
  }
  return "[" + std::to_string(lb_) + ".." + std::to_string(ub_) + "]";
}

/**
 * @brief ArrayBound 的赋值运算符。
 */
ArrayType::ArrayBound& ArrayType::ArrayBound::operator=(const ArrayBound& b2) {
  if (this != &b2) { 
    type_ = b2.type_;
    lb_ = b2.lb_;
    ub_ = b2.ub_;
  }
  return *this;
}

// ArrayType Implementations
/**
 * @brief ArrayType 默认构造函数。
 */
ArrayType::ArrayType() : TypeTemplate(TYPE::ARRAY), base_type_(TYPE_NONE) {}

/**
 * @brief ArrayType 构造函数，指定数组的基类型（元素类型）。
 */
ArrayType::ArrayType(TypeTemplate *type) : TypeTemplate(TYPE::ARRAY), base_type_(type) {}

/**
 * @brief ArrayType 构造函数，指定数组的基类型和所有维度的边界。
 */
ArrayType::ArrayType(TypeTemplate *type, std::vector<ArrayBound> bounds)
    : TypeTemplate(TYPE::ARRAY),
      base_type_(type),
      bounds_(std::move(bounds)) {}

/**
 * @brief ArrayType 的拷贝构造函数。
 */
ArrayType::ArrayType(const ArrayType& other)
  : TypeTemplate(TYPE::ARRAY), base_type_(other.base_type_) {
  for (const auto& b : other.bounds_) {
    bounds_.emplace_back(b);
  }
}

/**
 * @brief ArrayType 析构函数。
 */
ArrayType::~ArrayType() {}

/**
 * @brief 获取数组类型的完整字符串表示。
 */
std::string ArrayType::name() {
  std::string ret = "array";
  for (auto& b : bounds_) {
    ret += b.name();
  }
  string base_name = (base_type_ == TYPE_ERROR || base_type_ == nullptr) ? "error" : base_type_->name();
  return ret + " of " + base_name;
}

/**
 * @brief ArrayType 的赋值运算符。
 */
ArrayType& ArrayType::operator=(const ArrayType& other) {
  if (this != &other) { 
    template_type_ = TYPE::ARRAY;
    base_type_ = other.base_type_;
    bounds_.clear();
    for (const auto& b : other.bounds_) {
      bounds_.emplace_back(b);
    }
  }
  return *this;
}

/**
 * @brief 通过一系列索引类型访问多维数组的子类型。
 */
ArrayType ArrayType::Visit(std::vector<TypeTemplate*> v_types) const {
  if (v_types.empty()) return *this;
  if (v_types.size() > dims()) return ArrayType(TYPE_ERROR, {});

  for (size_t i = 0; i < v_types.size(); i++) {
    if (bounds_[i].type_ != v_types[i]) {
      return ArrayType(TYPE_ERROR, {});
    }
  }
  return Visit(static_cast<unsigned int>(v_types.size()));
}

/**
 * @brief 剥离数组的前 `v_layer` 个维度，返回剩余维度的数组类型。
 */
ArrayType ArrayType::Visit(unsigned int v_layer) const {
  if (v_layer == 0) return *this;
  if (v_layer > dims()) return ArrayType(TYPE_ERROR, {});

  vector<ArrayBound> remaining_bounds;
  for (size_t i = v_layer; i < bounds_.size(); i++) {
    remaining_bounds.emplace_back(bounds_[i]);
  }
  return ArrayType(base_type_, remaining_bounds);
}

/**
 * @brief 比较两个数组类型是否完全相同。
 */
bool ArrayType::operator==(const ArrayType& a2) const {
  if (template_type_ != a2.template_type_) return false;
  if (base_type_ != a2.base_type_) return false;
  if (dims() != a2.dims()) return false;
  for (size_t i = 0; i < dims(); i++) {
    if (!(bounds_[i] == a2.bounds_[i])) return false;
  }
  return true;
}

/**
 * @brief 判断数组类型是否为类字符串。
 */
bool ArrayType::StringLike(int access_layer) {
  if (base_type_ != TYPE_CHAR) return false;
  if (bounds_.size() != static_cast<size_t>(access_layer + 1)) return false;
  return true;
}

/**
 * @brief 比较当前数组从 `vdim1` 层访问后的子数组，是否与另一个数组 `other` 从 `vdim2` 层访问后的子数组类型相同。
 */
bool ArrayType::isSameAs(int vdim1, const ArrayType* other, int vdim2) const {
  if (this == nullptr || other == nullptr) return false;
  if (this->base_type_ != other->base_type_) return false; // Added this check based on original logic for global is_same
  
  ArrayType sub_array_this = this->Visit(static_cast<unsigned int>(vdim1));
  ArrayType sub_array_other = other->Visit(static_cast<unsigned int>(vdim2));

  return sub_array_this == sub_array_other; 
}

/**
 * @brief 比较当前数组在访问 `vdim` 层后得到的子数组，是否与给定的基础类型 `other_basic_type` 相同。
 */
bool ArrayType::isSameAsVisited(int vdim, const BasicType* other_basic_type) const {
  if (this == nullptr || other_basic_type == nullptr) return false;
  
  ArrayType visited_array = this->Visit(static_cast<unsigned int>(vdim));
  ArrayType temp_basic_wrapper = ArrayType(const_cast<BasicType*>(other_basic_type)); 
  
  return visited_array == temp_basic_wrapper;
}

// RecordType Implementations
/**
 * @brief RecordType 默认构造函数。
 */
RecordType::RecordType() : TypeTemplate(TYPE::RECORD) {}

/**
 * @brief RecordType 构造函数，使用给定的字段名到类型的映射初始化记录。
 */
RecordType::RecordType(std::unordered_map<std::string, TypeTemplate *> types_map)
    : TypeTemplate(TYPE::RECORD), types_map_(std::move(types_map)) {}

/**
 * @brief RecordType 析构函数。
 */
RecordType::~RecordType() {}

/**
 * @brief 获取记录类型的字符串表示。
 */
std::string RecordType::name() {
  std::string ret = "record";
  bool first_field = true;
  for (const auto& pair : types_map_) { 
    if (!first_field) {
      ret += ";"; 
    }
    ret += " "; 
    string type_name_str = (pair.second == TYPE_ERROR || pair.second == nullptr) ? "error" : pair.second->name();
    ret += pair.first + ":" + type_name_str;
    first_field = false; 
  }
  if (!types_map_.empty()) {
      ret += ";"; 
  }
  return ret + " end";
}

/**
 * @brief 通过字段名列表递归访问记录类型（可能嵌套）的成员类型。
 */
TypeTemplate* RecordType::Visit(std::vector<std::string> names) const {
  const TypeTemplate* current_type_node = this;
  for (const std::string& field_name : names) {
    if (current_type_node == nullptr) return nullptr;
    if (current_type_node->template_type() != TypeTemplate::TYPE::RECORD) {
      return nullptr;
    }
    const RecordType* current_record = current_type_node->DynamicCast<const RecordType>();
    if (!current_record) {
      return nullptr; 
    }
    TypeTemplate* next_type_node = current_record->Find(field_name);
    if (next_type_node == nullptr) {
      return nullptr;
    }
    current_type_node = next_type_node;
  }
  return const_cast<TypeTemplate*>(current_type_node);
}

/**
 * @brief 向记录类型中添加一个新字段。
 */
bool RecordType::add(std::string name, TypeTemplate* type) {
  if (types_map_.find(name) != types_map_.end()) return false;
  types_map_[name] = type;
  return true;
}

/**
 * @brief 在记录类型中查找指定名称的字段，并返回其类型。
 */
TypeTemplate* RecordType::Find(std::string name) const {
  auto iter = types_map_.find(name);
  if (iter != types_map_.end()) {
    return iter->second; 
  } else {
    return nullptr;
  }
}

/**
 * @brief 比较当前记录类型通过字段名列表 `names1` 访问到的成员类型，是否与另一个记录类型 `other_record` 通过字段名列表 `names2` 访问到的成员类型相同。
 */
bool RecordType::isSameAs(const std::vector<std::string>& names1, const RecordType* other_record, const std::vector<std::string>& names2) const {
  if (this == nullptr || other_record == nullptr) return false;
  
  TypeTemplate* member_type1 = this->Visit(names1);
  TypeTemplate* member_type2 = other_record->Visit(names2);
  
  if (member_type1) {
    return member_type1->isSameAs(member_type2);
  } else {
    return member_type2 == nullptr;
  }
}

/**
 * @brief 比较当前记录类型通过字段名列表 `names1` 访问到的成员类型，是否与给定的任意类型 `other_type` 相同。
 */
bool RecordType::isSameAsVisited(const std::vector<std::string>& names1, const TypeTemplate* other_type) const {
  if (this == nullptr) return false; 
  
  TypeTemplate* member_type = this->Visit(names1);
  
  if (member_type) {
    return member_type->isSameAs(other_type);
  } else {
    return other_type == nullptr;
  }
}

// ConstValue Implementations
/**
 * @brief ConstValue 默认构造函数。
 */
ConstValue::ConstValue() : m_Type(TYPE_INT), m_value(0) {}

/**
 * @brief ConstValue 析构函数。
 */
ConstValue::~ConstValue() {}

/**
 * @brief ConstValue 拷贝构造函数。
 */
ConstValue::ConstValue(const ConstValue& other) : m_Type(other.m_Type), m_value(other.m_value) {}

/**
 * @brief ConstValue 赋值运算符。
 */
ConstValue& ConstValue::operator=(const ConstValue& other) {
  if (this != &other) { 
    m_Type = other.m_Type;
    m_value = other.m_value;
  }
  return *this;
}

/**
 * @brief 通过整数构造 ConstValue。
 */
ConstValue::ConstValue(int v, bool to_float) {
  if (to_float) {
    set(static_cast<float>(v));
  } else {
    set(v);
  }
}

/**
 * @brief 通过浮点数构造 ConstValue。
 */
ConstValue::ConstValue(float v) { set(v); }

/**
 * @brief 通过布尔值构造 ConstValue。
 */
ConstValue::ConstValue(bool v) { set(v); }

/**
 * @brief 通过字符构造 ConstValue。
 */
ConstValue::ConstValue(char v) { set(v); }

/**
 * @brief 通过C风格字符串构造 ConstValue。
 */
ConstValue::ConstValue(const char *v) { set(std::string(v)); }

/**
 * @brief 通过 std::string 构造 ConstValue。
 */
ConstValue::ConstValue(std::string v) { set(v); }

/**
 * @brief 设置 ConstValue 的值为整数。
 */
void ConstValue::set(int v) {
  m_Type = TYPE_INT;
  m_value = v;
}

/**
 * @brief 设置 ConstValue 的值为浮点数。
 */
void ConstValue::set(float v) {
  m_Type = TYPE_REAL;
  m_value = v;
}

/**
 * @brief 设置 ConstValue 的值为布尔值。
 */
void ConstValue::set(bool v) {
  m_Type = TYPE_BOOL;
  m_value = v;
}

/**
 * @brief 设置 ConstValue 的值为字符。
 */
void ConstValue::set(char v) {
  m_Type = TYPE_CHAR;
  m_value = v;
}

/**
 * @brief 设置 ConstValue 的值为字符串。
 */
void ConstValue::set(std::string v) {
  m_Type = TYPE_STRINGLIKE;
  m_value = v;
}

/**
 * @brief 模板方法，获取 ConstValue 中存储的实际值。
 */
template <typename T>
T ConstValue::get() const {
    if constexpr (std::is_same_v<T, int>) {
        if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value)) return std::get<int>(m_value);
    } else if constexpr (std::is_same_v<T, float>) {
        if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value)) return std::get<float>(m_value);
    } else if constexpr (std::is_same_v<T, char>) {
        if (m_Type == TYPE_CHAR && std::holds_alternative<char>(m_value)) return std::get<char>(m_value);
    } else if constexpr (std::is_same_v<T, bool>) {
        if (m_Type == TYPE_BOOL && std::holds_alternative<bool>(m_value)) return std::get<bool>(m_value);
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (m_Type == TYPE_STRINGLIKE && std::holds_alternative<std::string>(m_value)) return std::get<std::string>(m_value);
    }
    throw std::runtime_error("ConstValue::get(): 类型不匹配或内部不一致。请求类型: " + 
                             std::string(typeid(T).name()) + 
                             ", 实际存储类型标记 m_Type: " + (m_Type ? m_Type->name() : "null") + 
                             ", Variant持有类型索引: " + std::to_string(m_value.index()));
}
// Explicit instantiations
template int ConstValue::get<int>() const;
template float ConstValue::get<float>() const;
template char ConstValue::get<char>() const;
template bool ConstValue::get<bool>() const;
template std::string ConstValue::get<std::string>() const;

/**
 * @brief 对 ConstValue 的值执行一元负操作。
 */
void ConstValue::set_unimus() {
  if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value)) {
    m_value = -std::get<int>(m_value);
  } else if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value)) {
    m_value = -std::get<float>(m_value);
  } else if (m_Type == TYPE_CHAR && std::holds_alternative<char>(m_value)) {
    m_value = static_cast<char>(-static_cast<int>(std::get<char>(m_value)));
  } else {
    throw std::runtime_error("ConstValue::set_unimus: 类型不支持一元负运算或内部类型不一致。");
  }
}

/**
 * @brief ConstValue 加法运算符。
 */
ConstValue ConstValue::operator+(const ConstValue& other) {
  if (m_Type != other.m_Type) throw std::runtime_error("ConstValue::operator+: 类型不匹配");
  ConstValue ret;
  ret.m_Type = m_Type;

  if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value) && std::holds_alternative<int>(other.m_value)) {
    ret.m_value = std::get<int>(m_value) + std::get<int>(other.m_value);
  } else if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value) && std::holds_alternative<float>(other.m_value)) {
    ret.m_value = std::get<float>(m_value) + std::get<float>(other.m_value);
  } else if (m_Type == TYPE_STRINGLIKE && std::holds_alternative<std::string>(m_value) && std::holds_alternative<std::string>(other.m_value)) {
    ret.m_value = std::get<std::string>(m_value) + std::get<std::string>(other.m_value);
  } else if (m_Type == TYPE_CHAR && std::holds_alternative<char>(m_value) && std::holds_alternative<char>(other.m_value)) {
    ret.m_value = static_cast<char>(static_cast<int>(std::get<char>(m_value)) + static_cast<int>(std::get<char>(other.m_value)));
  } else {
    throw std::runtime_error("ConstValue::operator+: 类型不支持加法运算或内部值类型不一致。");
  }
  return ret;
}

/**
 * @brief ConstValue 减法运算符。
 */
ConstValue ConstValue::operator-(const ConstValue& other) {
  if (m_Type != other.m_Type) throw std::runtime_error("ConstValue::operator-: 类型不匹配");
  ConstValue ret;
  ret.m_Type = m_Type;

  if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value) && std::holds_alternative<int>(other.m_value)) {
    ret.m_value = std::get<int>(m_value) - std::get<int>(other.m_value);
  } else if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value) && std::holds_alternative<float>(other.m_value)) {
    ret.m_value = std::get<float>(m_value) - std::get<float>(other.m_value);
  } else if (m_Type == TYPE_CHAR && std::holds_alternative<char>(m_value) && std::holds_alternative<char>(other.m_value)) {
    ret.m_value = static_cast<char>(static_cast<int>(std::get<char>(m_value)) - static_cast<int>(std::get<char>(other.m_value)));
  } else {
    throw std::runtime_error("ConstValue::operator-: 类型不支持减法运算 (例如字符串) 或内部值类型不一致。");
  }
  return ret;
}

/**
 * @brief ConstValue 乘法运算符。
 */
ConstValue ConstValue::operator*(const ConstValue& other) {
  if (m_Type != other.m_Type) throw std::runtime_error("ConstValue::operator*: 类型不匹配");
  ConstValue ret;
  ret.m_Type = m_Type;

  if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value) && std::holds_alternative<int>(other.m_value)) {
    ret.m_value = std::get<int>(m_value) * std::get<int>(other.m_value);
  } else if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value) && std::holds_alternative<float>(other.m_value)) {
    ret.m_value = std::get<float>(m_value) * std::get<float>(other.m_value);
  } else {
    throw std::runtime_error("ConstValue::operator*: 类型不支持乘法运算或内部值类型不一致。");
  }
  return ret;
}

/**
 * @brief ConstValue 除法运算符。
 */
ConstValue ConstValue::operator/(const ConstValue& other) {
  if (m_Type != other.m_Type) throw std::runtime_error("ConstValue::operator/: 类型不匹配");
  ConstValue ret;

  if (m_Type == TYPE_INT && std::holds_alternative<int>(m_value) && std::holds_alternative<int>(other.m_value)) {
    ret.m_Type = TYPE_INT;
    if (std::get<int>(other.m_value) == 0) throw std::runtime_error("ConstValue::operator/: 整数除零错误");
    ret.m_value = std::get<int>(m_value) / std::get<int>(other.m_value);
  } else if (m_Type == TYPE_REAL && std::holds_alternative<float>(m_value) && std::holds_alternative<float>(other.m_value)) {
    ret.m_Type = TYPE_REAL;
    if (std::get<float>(other.m_value) == 0.0f) throw std::runtime_error("ConstValue::operator/: 浮点数除零错误");
    ret.m_value = std::get<float>(m_value) / std::get<float>(other.m_value);
  } else {
    throw std::runtime_error("ConstValue::operator/: 类型不支持除法运算或内部值类型不一致。");
  }
  return ret;
}

// Operation Implementations
/**
 * @brief Operation 默认构造函数。
 */
Operation::Operation() : in_type1(nullptr), in_type2(nullptr), op("") {}

/**
 * @brief Operation 析构函数。
 */
Operation::~Operation() {
  in_type1 = nullptr;
  in_type2 = nullptr;
}

// Global Helper Functions for Types
/**
 * @brief 获取单个类型的字符串名称。
 */
string type_name(TypeTemplate* t){
  if(t == nullptr) {
      if (t == TYPE_ERROR) return "error";
      return "null_type_pointer";
  }
  if(t == TYPE_STRINGLIKE) return "stringlike";
  return t->name();
}

/**
 * @brief 将一组类型转换为用逗号和空格分隔的字符串表示。
 */
string type_name(const vector<TypeTemplate*> &t){
  string res = "";
  for(size_t i = 0; i < t.size(); i++){ 
    res += type_name(t[i]);
    if(i != t.size() - 1) res += ", ";
  }
  return res;
}

/* **************** 全局变量初始化区 **************** */
BasicType* TYPE_INT = nullptr;
BasicType* TYPE_REAL = nullptr;
BasicType* TYPE_BOOL = nullptr;
BasicType* TYPE_CHAR = nullptr;
BasicType* TYPE_NONE = nullptr;
BasicType* TYPE_ERROR = nullptr;
BasicType* TYPE_STRINGLIKE = nullptr;

std::vector<TypeTemplate*>* PTR_COLLECTOR = nullptr;
OperationMap operation_map;

/**
 * @brief 初始化全局类型系统。
 */
void TypeInit() {
  TYPE_ERROR = nullptr;

  TYPE_BOOL = new BasicType(BasicType::BASIC_TYPE::BOOL);
  TYPE_CHAR = new BasicType(BasicType::BASIC_TYPE::CHAR);
  TYPE_INT = new BasicType(BasicType::BASIC_TYPE::INT);
  TYPE_REAL = new BasicType(BasicType::BASIC_TYPE::REAL);
  TYPE_NONE = new BasicType(BasicType::BASIC_TYPE::NONE);
  TYPE_STRINGLIKE = new BasicType(BasicType::BASIC_TYPE::CHAR); 

  PTR_COLLECTOR = new std::vector<TypeTemplate*>();

  // Boolean operations
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "and")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "or")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, NULL, "not")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, ">=")] = TYPE_BOOL;
  
  // Integer operations
  operation_map[Operation(TYPE_INT, NULL, "-")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, NULL, "+")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "+")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "-")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "*")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, NULL, "not")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "mod")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "div")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, ">=")] = TYPE_BOOL;
  
  // Real operations
  operation_map[Operation(TYPE_REAL, NULL, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, NULL, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<=")] = TYPE_BOOL;
  
  // Char operations
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, ">=")] = TYPE_BOOL;
  
  // Integer-Real mixed operations
  operation_map[Operation(TYPE_REAL, TYPE_INT, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<")] = TYPE_BOOL;
  
  operation_map[Operation(TYPE_INT, TYPE_REAL, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<")] = TYPE_BOOL;
}

/**
 * @brief Ensures TypeInit() is called at program startup.
 */
int type_init_dummy_ = (TypeInit(), 0);

/**
 * @brief 释放全局类型系统占用的资源。
 */
void TypeRelease() {
  ReleaseTemp();
  
delete TYPE_CHAR;
  TYPE_CHAR = nullptr;
  delete TYPE_INT;
  TYPE_INT = nullptr;
  delete TYPE_REAL;
  TYPE_REAL = nullptr;
  delete TYPE_NONE;
  TYPE_NONE = nullptr;
  delete TYPE_STRINGLIKE;
  TYPE_STRINGLIKE = nullptr;
  delete TYPE_BOOL;
  TYPE_BOOL = nullptr;

  delete PTR_COLLECTOR;
  PTR_COLLECTOR = nullptr;
}

/**
 * @brief Ensures TypeRelease() is called at program exit.
 */
int type_release_dummy_ = (atexit(TypeRelease), 0);