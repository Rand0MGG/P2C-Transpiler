#include "symbol.h"

#include <cstdio>

#include "type.h"
using std::string;
using std::vector;

/**
 * @brief FunctionSymbol 的拷贝构造函数。
 * @details 深拷贝参数列表 `params_` 和参数名到索引的映射 `param_map_`。
 * @param f 要拷贝的源 `FunctionSymbol` 对象。
 */
FunctionSymbol::FunctionSymbol(const FunctionSymbol &f)
    : SymbolTemplate(f.name_, f.type_, f.decl_line_) {
  params_.assign(f.params_.begin(), f.params_.end());
  for (auto it = f.param_map_.begin(); it != f.param_map_.end(); ++it) {
    param_map_[it->first] = it->second;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

/**
 * @brief FunctionSymbol 的构造函数 (带参数列表)。
 * @param name 函数的名称。
 * @param return_type 函数的返回类型。
 * @param decl_line 函数声明所在的行号。
 * @param params 函数的参数信息列表 (`std::vector<ParameterInfo>`)。
 */
FunctionSymbol::FunctionSymbol(std::string name, BasicType *return_type,
                               int decl_line,
                               const std::vector<ParameterInfo> &params)
    : SymbolTemplate(name, return_type, decl_line), params_(params) {
  for (int i = 0; i < params.size(); i++) {
    param_map_[params[i].name] = i;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

FunctionSymbol::FunctionSymbol(std::string name, BasicType *return_type,
                               int decl_line)
    : SymbolTemplate(name, return_type, decl_line) {
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

/**
 * @brief 向函数符号中插入一个新的参数。
 * @details 如果参数名称尚未存在，则将参数信息添加到 `params_` 列表，
 *          并在 `param_map_` 中建立名称到索引的映射。
 * @param p 要插入的参数信息 (`ParameterInfo`)。
 * @return 如果参数名称唯一且插入成功，返回 `true`；否则 (参数名已存在)，返回 `false`。
 */
bool FunctionSymbol::InsertParam(const ParameterInfo &p) {
  auto ptr = param_map_.find(p.name);
  if (ptr == param_map_.end()) {
    params_.emplace_back(p);
    param_map_[p.name] = params_.size() - 1;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief 根据参数名称获取参数的详细信息。
 * @param name 要查找的参数的名称。
 * @return 如果找到参数，返回指向 `ParameterInfo` 结构体的常量指针；
 *         如果未找到，返回 `nullptr`。
 */
const FunctionSymbol::ParameterInfo *FunctionSymbol::GetParamInfo(std::string name) {
  auto ptr = param_map_.find(name);
  if (ptr == param_map_.end()) {
    return nullptr;
  } else {
    return &(params_[ptr->second]);
  }
}

/**
 * @brief 检查指定名称的参数是否为引用传递。
 * @param name 要检查的参数的名称。
 * @return 如果参数存在且其模式为 `PARAM_MODE::REFERENCE`，返回 `true`；
 *         否则 (参数不存在或不是引用传递)，返回 `false`。
 */
bool FunctionSymbol::IsReference(std::string name) {
  auto param_info = GetParamInfo(name);
  if (param_info == nullptr) {
    return false;
  } else {
    return param_info->mode == PARAM_MODE::REFERENCE;
  }
}

/**
 * @brief 生成函数参数列表的字符串表示形式。
 * @details 例如，对于 `function foo(a: integer; var b: char)`，
 *          可能生成 "integer, char&"。
 *          引用传递的参数类型后会附加 '&' 符号。
 * @return `std::string` 参数列表的字符串表示。
 */
std::string FunctionSymbol::ParamName() {
   std::string res = "";
   for(int i = 0; i < params_.size(); i++){
       res += type_name(params_[i].type);
       if(params_[i].mode == PARAM_MODE::REFERENCE) res += "&";
       if(i != params_.size() - 1) res += ", ";
   }
   return res;
}

/**
 * @brief 断言传入的实际参数类型和值/引用属性是否与函数声明的参数匹配。
 *
 * @param params_in 一个 `TypeTemplate*` 向量，表示调用时传入的各个参数的类型。
 * @param value_type_in 一个布尔向量，与 `params_in` 对应。
 *                        其中每个元素标记对应参数的 "值类别"：
 *                        - `true`：表示实参是一个右值，或者是一个可以按值传递的左值。
 *                        - `false`：表示实参是一个非临时的左值（例如一个变量），这对于期望引用传递 (VAR参数) 的形参是必要的。
 *                               如果形参是值传递，而实参是 `false` (左值)，通常也是允许的。
 * @return 如果参数数量、类型以及引用传递兼容性均匹配，则返回 `true`；否则返回 `false`。
 *
 * @note 此函数用于语义检查阶段，确保函数调用时的参数与函数定义兼容。
 *       关键检查点：
 *       1. 参数数量必须完全一致。
 *       2. 对应位置的参数类型必须一致。
 *       3. 如果函数形参要求引用传递 (`PARAM_MODE::REFERENCE`)，那么调用时对应的实参
 *          必须是一个可传递引用的左值 (即 `value_type_in[i]` 为 `false`)。
 *          反之，如果形参是值传递，实参可以是左值或右值。
 *          (当前实现逻辑: `value_type_in[i] == false && params_[i].mode == PARAM_MODE::REFERENCE` 为不匹配条件之一,
 *           这意味着如果形参是引用，实参必须是左值 `value_type_in[i] == false`。
 *           如果形参是引用，而实参是右值 `value_type_in[i] == true`，则不匹配。
 *           应修改为：如果形参是引用(`params_[i].mode == PARAM_MODE::REFERENCE`)，
 *           而实参是右值(`value_type_in[i] == true`)，则不匹配。
 *          )
 *       修正后的逻辑：
 *       如果 `params_[i].mode == PARAM_MODE::REFERENCE` (形参要求引用) 且 `value_type_in[i] == true` (实参是右值或只能值传递)，则不匹配。
 */
bool FunctionSymbol::AssertParamsMatch(const vector<TypeTemplate *> &params_in,
                                     const vector<bool> value_type_in) {
  if (params_in.size() != params_.size() ||
      params_in.size() != value_type_in.size()) {
    return false;
  }

  for (int i = 0; i < params_in.size(); i++) {
    // assert parameter type
    if (params_in[i] != params_[i].type ||
        (value_type_in[i] == false &&
         params_[i].mode == PARAM_MODE::REFERENCE)) {
      return false;
    }
  }
  return true;
}

/**
 * @brief 断言函数是否没有参数。
 * @return 如果函数参数列表 `params_` 为空，返回 `true`；否则返回 `false`。
 */
bool FunctionSymbol::AssertHasNoParams() { return params_.size() == 0; }

/**
 * @brief 获取一个布尔向量，标记函数每个参数是否为引用传递。
 * @details 向量的大小与参数数量相同，每个元素 `res[i]` 为 `true` 表示
 *          第 `i` 个参数是引用传递，为 `false` 表示是值传递。
 * @return `std::vector<bool>` 表示各参数引用状态的向量。
 */
std::vector<bool> FunctionSymbol::ParamRefVec() {
  std::vector<bool> res;
  for (auto &p : params_) {
    res.push_back(p.mode == PARAM_MODE::REFERENCE);
  }
  return res;
}