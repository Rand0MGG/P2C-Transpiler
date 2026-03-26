/**
 * @file symbol_table.h
 * @brief 定义符号表和类型表及其管理机制。
 *
 * 该文件包含了一个通用的符号表模板 `SymbolTableTemplate`，以及由此派生的
 * `TypeTable` (类型表) 和 `SymbolTable` (符号表)。
 * `TableSet` 类则将这两种表组合起来，用于管理特定作用域内的符号和类型，
 * 并支持嵌套作用域的查找。
 */
#ifndef P2C-Transpiler_SYMBOL_TABLE_H_
#define P2C-Transpiler_SYMBOL_TABLE_H_

#include <string>
#include <unordered_map>

#include "log.h"    // 用于日志记录
#include "symbol.h" // 符号定义
#include "type.h"   // 类型定义

/**
 * @brief 通用符号表模板。
 *
 * @tparam T 表中存储的条目类型 (例如 `TypeTemplate*` 或 `SymbolTemplate*`)。
 * 提供基本的插入和按名称查找的功能。
 */
template <typename T>
class SymbolTableTemplate {
 public:
  /**
   * @brief 构造一个新的符号表模板实例。
   * @param tag 表的标签，主要用于调试和日志记录，默认为空字符串。
   */
  SymbolTableTemplate(std::string tag = "") : tag_(tag) {}

  /**
   * @brief 析构函数。
   * @note 此基类析构函数不负责释放存储的指针所指向的对象，
   *       具体释放逻辑由派生类根据 `T` 的实际类型决定。
   */
  ~SymbolTableTemplate() {}

  /**
   * @brief向表中插入一个符号。
   *
   * @param name 符号的名称 (作为键)。
   * @param symbol 指向符号对象的指针 (作为值)。
   * @return 如果名称不存在且插入成功，返回 `true`；如果名称已存在，返回 `false`。
   */
  bool Insert(std::string name, T* symbol) {
    if (table_.find(name) != table_.end()) {
      // 名称已存在，插入失败
      return false;
    }
    table_.insert(std::make_pair(name, symbol));
    return true;
  }

  /**
   * @brief 根据名称查找符号。
   *
   * @param name 要查找的符号名称。
   * @return 如果找到，返回指向符号对象的指针；否则返回 `nullptr`。
   */
  T* FindName(std::string name) {
    auto it = table_.find(name);
    if (it != table_.end()) return it->second;
    return nullptr;
  }

  /**
   * @brief 重载下标运算符 `[]`，提供便捷的按名称查找功能。
   * @param name 要查找的符号名称。
   * @return 如果找到，返回指向符号对象的指针；否则返回 `nullptr`。
   * @see FindName
   */
  T* operator[](std::string name) { return FindName(name); }

 protected:
  std::string tag_; ///< 表的标签，用于调试和区分不同的表实例。
  std::unordered_map<std::string, T*> table_; ///< 存储符号名称到符号指针的映射表。
};

/**
 * @brief 类型表，专门用于存储类型定义。
 *
 * 继承自 `SymbolTableTemplate<TypeTemplate>`。
 * 负责管理程序中定义和遇到的各种类型，例如基本类型、数组类型、记录类型等。
 */
class TypeTable : public SymbolTableTemplate<TypeTemplate> {
 public:
  /**
   * @brief 构造一个新的类型表。
   * @param tag 表的标签，会自动附加 "_type" 后缀以作区分，默认为空字符串。
   */
  TypeTable(std::string tag = "") : SymbolTableTemplate(tag + "_type") {
    log_debug("TypeTable(): create '%s'", tag_.c_str());
  }

  /**
   * @brief 类型表析构函数。
   * @details 负责释放类型表中存储的非基本类型对象。
   *          基本类型 (如 integer, real, char, boolean) 通常是全局单例，不应在此处删除。
   */
  ~TypeTable() {
    log_debug("~TypeTable(): delete %lu symbols from '%s'", table_.size(),
              tag_.c_str());
    for (auto it = table_.begin(); it != table_.end(); ++it) {
      // 只删除非基本类型。基本类型由 TypeSystem 全局管理。
      if (!(it->second->isSpecificBasicType())) { // 使用 isSpecificBasicType 进行判断
        delete it->second;
      }
    }
  }

  /**
   * @brief 根据名称查找类型，并进行类型检查和转换。
   *
   * @tparam TargetType 期望获取的类型 (例如 `ArrayType`, `BasicType`, `RecordType` 或 `TypeTemplate`)。
   * @param name 要查找的类型名称。
   * @return 如果找到指定名称的类型，并且该类型与 `TargetType` 兼容，则返回转换后的指针；
   *         否则返回 `nullptr`。
   */
  template <typename TargetType>
  TargetType* Find(std::string name) {
    auto entry = (*this)[name]; // 使用基类的 operator[] 查找
    if (entry == nullptr) return nullptr;

    // 进行类型兼容性检查
    // 如果期望的是基类 TypeTemplate，则直接返回
    if (std::is_same<TargetType, TypeTemplate>::value) {
      return dynamic_cast<TargetType*>(entry);
    } 
    // 如果期望的是 BasicType，且实际存储的是 BASIC 类型
    else if (std::is_same<TargetType, BasicType>::value &&
            entry->template_type() == TypeTemplate::TYPE::BASIC) {
      return dynamic_cast<TargetType*>(entry);
    } 
    // 如果期望的是 ArrayType，且实际存储的是 ARRAY 类型
    else if (std::is_same<TargetType, ArrayType>::value &&
            entry->template_type() == TypeTemplate::TYPE::ARRAY) {
      return dynamic_cast<TargetType*>(entry);
    } 
    // 如果期望的是 RecordType，且实际存储的是 RECORD 类型
    else if (std::is_same<TargetType, RecordType>::value &&
            entry->template_type() == TypeTemplate::TYPE::RECORD) {
      return dynamic_cast<TargetType*>(entry);
    }
    return nullptr; // 类型不匹配或不兼容
  }
};

/**
 * @brief 符号表，专门用于存储程序中的符号对象 (变量、常量、函数等)。
 *
 * 继承自 `SymbolTableTemplate<SymbolTemplate>`。
 */
class SymbolTable : public SymbolTableTemplate<SymbolTemplate> {
 public:
  /**
   * @brief 构造一个新的符号表。
   * @param tag 表的标签，会自动附加 "_symbols" 后缀以作区分，默认为空字符串。
   */
  SymbolTable(std::string tag = "") : SymbolTableTemplate(tag + "_symbols") {
    log_debug("SymbolTable(): create '%s'", tag_.c_str());
  }

  /**
   * @brief 符号表析构函数。
   * @details 负责释放符号表中存储的所有 `SymbolTemplate` 对象。
   *          假定符号表拥有其存储的符号对象的所有权。
   */
  ~SymbolTable() {
    log_debug("~SymbolTable(): delete %lu symbols from '%s'", table_.size(),
              tag_.c_str());
    for (auto it = table_.begin(); it != table_.end(); ++it) {
      delete it->second; // 删除符号对象
    }
  }

  /**
   * @brief 根据名称查找符号，并进行类型检查和转换。
   *
   * @tparam TargetSymbolType 期望获取的符号类型 (例如 `FunctionSymbol`, `ConstSymbol` 或 `SymbolTemplate`)。
   * @param name 要查找的符号名称。
   * @return 如果找到指定名称的符号，并且该符号与 `TargetSymbolType` 兼容，则返回转换后的指针；
   *         否则返回 `nullptr`。
   */
  template <typename TargetSymbolType>
  TargetSymbolType* Find(std::string name) {
    auto entry = (*this)[name]; // 使用基类的 operator[] 查找
    if (entry == nullptr) return nullptr;

    // 进行符号类型兼容性检查
    // 如果期望的是基类 SymbolTemplate
    if (std::is_same<TargetSymbolType, SymbolTemplate>::value) {
      return dynamic_cast<TargetSymbolType*>(entry);
    } 
    // 如果期望的是 FunctionSymbol，且实际存储的是 FUNCTION 符号
    else if (std::is_same<TargetSymbolType, FunctionSymbol>::value &&
            entry->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION) {
      return dynamic_cast<TargetSymbolType*>(entry);
    } 
    // 如果期望的是 ConstSymbol，且实际存储的是 CONST 符号
    else if (std::is_same<TargetSymbolType, ConstSymbol>::value &&
            entry->symbol_type() == SymbolTemplate::SYMBOL_TYPE::CONST) {
      return dynamic_cast<TargetSymbolType*>(entry);
    } 
    // 注意：这里没有处理 VAR 类型的 SymbolTemplate 的特定派生类 (如果存在的话)
    // 如果 TargetSymbolType 是普通的 SymbolTemplate 且 entry 是 VAR，第一个 if 会处理
    return nullptr; // 类型不匹配或不兼容
  }
};

/**
 * @brief 表集合，包含一个符号表和一个类型表，代表一个作用域。
 *
 * `TableSet` 用于管理特定作用域 (例如全局作用域、函数作用域、过程作用域) 内
 * 定义的符号和类型。它通过 `prev_table_set_` 指针链接到其父作用域的 `TableSet`，
 * 从而形成作用域链，支持嵌套作用域的符号查找。
 */
class TableSet {
 public:
  /**
   * @brief 构造一个新的表集合 (作用域)。
   * @param tag 当前作用域的标签 (例如 "global", "function_foo")。
   * @param pre_set 指向父作用域的 `TableSet` 的指针；对于全局作用域，此参数为 `nullptr`。
   */
  TableSet(std::string tag, TableSet* pre_set)
      : tag_(tag),
        prev_table_set_(pre_set),
        symbols_(SymbolTable(tag)),      // 初始化当前作用域的符号表
        def_types_(TypeTable(tag)) {     // 初始化当前作用域的类型表
    log_debug("TableSet(): created '%s'", tag.c_str());
  }

  /**
   * @brief 表集合析构函数。
   * @details 将指向父作用域的指针设为 `nullptr`。符号表和类型表的析构函数会被自动调用，
   *          它们负责释放各自存储的符号和类型对象。
   */
  ~TableSet() {
    prev_table_set_ = nullptr; // 断开与父作用域的链接
    log_debug("~TableSet(): delete '%s'", tag_.c_str());
  }

  // Getter 方法
  /** @brief 获取当前作用域的符号表。*/
  SymbolTable* symbols() { return &symbols_; }
  /** @brief 获取当前作用域的类型表。*/
  TypeTable* def_types() { return &def_types_; }
  /** @brief 获取父作用域的表集合。*/
  TableSet* previous() { return prev_table_set_; }
  /** @brief 获取当前作用域的标签。*/
  std::string tag() { return tag_; }

  /*
   * @brief (已注释掉的) 通用插入接口。
   * @details 此通用模板函数曾尝试统一处理符号和类型的插入。
   *          但为了更清晰的职责分离、类型安全以及避免类型名和符号名之间的潜在冲突，
   *          已被替换为特定的 `InsertSymbol` 和 `InsertType` 方法。
   *          例如，Pascal允许类型名和变量名相同，如果作用域不同。
   *          这个通用插入方法在查重时可能不够精确，因为它只检查了当前作用域。
   *          分离的插入方法可以进行更全面的查重，例如 `InsertSymbol` 会检查
   *          是否与当前及外层作用域的类型名冲突。
   */
  /* template <typename T>
  bool Insert(std::string name, T* symbol) {
    SymbolTemplate* object_flag = symbols_.FindName(name);
    TypeTemplate* type_flag = SearchEntry<TypeTemplate>(name);
    if (object_flag != nullptr || type_flag != nullptr) {
      return false;
    }
    if (std::is_same<T, SymbolTemplate>::value ||
        std::is_same<T, ConstSymbol>::value ||
        std::is_same<T, FunctionSymbol>::value) {
      symbols_.Insert(name, (SymbolTemplate*)symbol);
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, BasicType>::value ||
               std::is_same<T, RecordType>::value) {
      def_types_.Insert(name, (TypeTemplate*)symbol);
    }
    return true;
  }*/

  /**
   * @brief 向当前作用域的符号表中插入一个对象符号 (变量、常量、函数)。
   * @param name 符号的名称。
   * @param symbol 指向 `SymbolTemplate` 对象的指针。
   * @return 如果符号名在当前作用域的符号表和整个作用域链的类型表中均不冲突，
   *         并且成功插入到当前作用域的符号表，则返回 `true`；否则返回 `false`。
   * @note 查重逻辑：不仅检查当前符号表，还检查所有可见作用域中的类型名，避免符号名与类型名冲突。
   */
  bool InsertSymbol(std::string name, SymbolTemplate* symbol) {
    SymbolTemplate* object_flag = symbols_.FindName(name); // 检查当前符号表是否有同名符号
    TypeTemplate* type_flag = SearchEntry<TypeTemplate>(name); // 检查所有可见作用域是否有同名类型
    if (object_flag != nullptr || type_flag != nullptr) {
      return false; // 名称冲突
    }
    return symbols_.Insert(name, symbol);
  }

  /**
   * @brief 向当前作用域的类型表中插入一个类型定义。
   * @param name 类型的名称。
   * @param type 指向 `TypeTemplate` 对象的指针。
   * @return 如果类型名在当前作用域的类型表和符号表中均不冲突，
   *         并且成功插入到当前作用域的类型表，则返回 `true`；否则返回 `false`。
   * @note 查重逻辑：检查当前符号表是否有同名符号，以及当前类型表是否有同名类型。
   *         这里的 `SearchEntry<SymbolTemplate>(name)` 可以用于更严格地检查所有可见作用域的符号名，
   *         但当前实现 `SymbolTemplate* object_flag = symbols_.FindName(name);` 只检查了当前符号表。
   *         这可能是一个设计选择，允许在内部作用域定义与外部作用域变量同名的类型，
   *         但通常不建议这样做。更严格的检查会调用 `SearchEntry<SymbolTemplate>(name)`。
   */
  bool InsertType(std::string name, TypeTemplate* type) {
    SymbolTemplate* object_flag = symbols_.FindName(name); // 检查当前作用域符号表是否有同名符号
    TypeTemplate* type_flag = def_types_.FindName(name); // 检查当前作用域类型表是否有同名类型
    if (object_flag != nullptr || type_flag != nullptr) {
      return false; // 名称冲突
    }
    return def_types_.Insert(name, type);
  }

  /**
   * @brief 在作用域链中统一查找符号或类型。
   * @details 首先在当前作用域的符号表和类型表中查找，如果未找到，则递归地在其父作用域中查找，
   *          直到找到匹配的条目或到达全局作用域的父级 (即 `nullptr`)。
   *
   * @tparam T期望查找的条目类型。可以是 `SymbolTemplate` 及其派生类 (如 `ConstSymbol`, `FunctionSymbol`),
   *         或者 `TypeTemplate` 及其派生类 (如 `ArrayType`, `RecordType`, `BasicType`)。
   * @param name 要查找的标识符名称。
   * @param local_zone (可选输出参数) 如果非 `nullptr`，用于指示查找到的符号/类型是否位于当前 (最内层) 作用域。
   *                   查找开始时设为 `true`，如果进入父作用域查找则设为 `false`。
   * @return 如果找到匹配的条目，则返回指向该条目的指针 (已转换为类型 `T`)；否则返回 `nullptr`。
   */
  template <typename T>
  T* SearchEntry(std::string name, bool* local_zone = nullptr) {
    if (local_zone != nullptr) *local_zone = true; // 初始假定在当前作用域找到

    // 根据 T 的类型，决定是搜索符号表还是类型表
    if (std::is_base_of<SymbolTemplate, T>::value || std::is_same<SymbolTemplate, T>::value) {
      // 目标是 SymbolTemplate 或其派生类
      auto symbol_entry = symbols_.Find<T>(name); // 使用 SymbolTable 的类型安全 Find
      if (symbol_entry != nullptr) return static_cast<T*>(symbol_entry);
    } else if (std::is_base_of<TypeTemplate, T>::value || std::is_same<TypeTemplate, T>::value) {
      // 目标是 TypeTemplate 或其派生类
      auto type_entry = def_types_.Find<T>(name); // 使用 TypeTable 的类型安全 Find
      if (type_entry != nullptr) return static_cast<T*>(type_entry);
    }

    // 当前作用域未找到，尝试在父作用域中查找
    if (local_zone != nullptr) *local_zone = false; // 标记为非本地查找
    if (prev_table_set_ != nullptr) {
      return prev_table_set_->SearchEntry<T>(name, local_zone);
    }
    return nullptr; // 到达最外层作用域仍未找到
  }

 private:
  std::string tag_; ///< 当前作用域的标签。
  SymbolTable symbols_;       ///< 当前作用域的符号表。
  TypeTable def_types_;       ///< 当前作用域的类型表。
  TableSet* prev_table_set_;  ///< 指向父作用域的表集合的指针。
};

#endif  // P2C-Transpiler_SYMBOL_TABLE_H_