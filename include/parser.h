#ifndef P2C-Transpiler_PARSER_H
#define P2C-Transpiler_PARSER_H

#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include "type.h"
#include "ast.h"
extern int yydebug;

struct Token {
  ConstValue value;
  int length;
  int line_num;  //行号 
  int column_num;  //列号
};

struct IdListAttr {
  std::vector<std::pair<std::string, int>>* list_ref;
  IdListNode* id_list_node;
};

struct ValueAttr {
  BasicType* type_ptr;
  ConstValue value;
  LeafNode* const_variable_node;
  bool is_right = true;
};

struct StandardTypeAttr {
  BasicType* type_ptr;
  BasicTypeNode* standard_type_node;
};

struct TypeAttr {
  enum MainType { BASIC, ARRAY, RECORD } main_type;
  TypeTemplate* type_ptr;
  TypeNode* base_type_node;
  TypeNode* type_node;
  std::vector<ArrayType::ArrayBound>* bounds;
  std::unordered_map<std::string, TypeTemplate*>* record_info;
};

struct PeriodsAttr {
  std::vector<ArrayType::ArrayBound>* bounds;
  // std::vector<std::pair<int, int>>* bounds;
  PeriodsNode* periods_node;
};

struct PeriodAttr {
  ArrayType::ArrayBound* bound;
  // std::pair<int, int>* bound;
  PeriodNode* period_node;
};

struct RecordAttr {
  std::unordered_map<std::string, TypeTemplate*>* record_info;
  RecordBodyNode* record_body_node;
};

struct VariableDeclarationAttr {
  std::unordered_map<std::string, TypeTemplate*>* record_info;
  VariableDeclarationNode* variable_declaration_node;
  std::unordered_map<std::string, std::pair<int, int>>* pos_info;
};

struct FormalParameterAttr {
  std::vector<FunctionSymbol::ParameterInfo>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  FormalParamNode* formal_parameter_node;
};

struct ParameterListsAttr {
  std::vector<FunctionSymbol::ParameterInfo>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  ParamListsNode* param_lists_node;
};
struct ParameterListAttr {
  std::vector<FunctionSymbol::ParameterInfo>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  ParamListNode* param_list_node;
};
struct VarParameterAttr {
  std::vector<FunctionSymbol::ParameterInfo>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  VarParamNode* var_parameter_node;
};

struct ValueParameterAttr {
  std::vector<FunctionSymbol::ParameterInfo>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  ValueParamNode* value_parameter_node;
};

struct VariableAttr {
  TypeTemplate* type_ptr;
  VariableNode* variable_node;
  std::string* name;
  bool is_lvalue;
};

struct VariableListAttr {
  std::vector<TypeTemplate*>* type_ptr_list;
  VariableListNode* variable_list_node;
};

struct ExpressionAttr {
  TypeTemplate* type_ptr;
  ExpressionNode* expression_node;
  int length;
  bool is_lvalue;
};



struct SimpleExpressionAttr {
  TypeTemplate* type_ptr;
  SimpleExpressionNode* simple_expression_node;
  bool is_lvalue;
};

struct TermAttr {
  TypeTemplate* type_ptr;
  TermNode* term_node;
  bool is_lvalue;
};

struct FactorAttr {
  TypeTemplate* type_ptr;
  FactorNode* factor_node;
  bool is_lvalue;
};

struct UnsignedConstantVarAttr {
  TypeTemplate* type_ptr;
  UnsignConstVarNode* unsigned_constant_var_node;
};

struct VarParts {
  bool flag;
  std::vector<TypeTemplate*>* subscript;
  std::string name;
};

struct IDVarpartsAttr {
  std::vector<VarParts>* var_parts;
  IDVarPartsNode* id_varparts_node;
  TypeTemplate* AccessCheck(TypeTemplate* base_type) {
    int in_array = 0;
    std::vector<TypeTemplate *> vistor;
    TypeTemplate *cur_type = base_type;

    for (int i = 0; i < var_parts->size(); i++) {
      if ((*var_parts)[i].flag == 0) {  //数组
        if (!in_array) in_array = 1;
        std::copy((*var_parts)[i].subscript->begin(),
                  (*var_parts)[i].subscript->end(), std::back_inserter(vistor));
      } else {  //结构体
        if (in_array) {
          if (cur_type->template_type() != TypeTemplate::TYPE::ARRAY) {
            return TYPE_ERROR;
          }
          auto cur_array_type =
              new ArrayType(cur_type->DynamicCast<ArrayType>()->Visit(vistor));
          PtrCollect(cur_array_type);
          if (!cur_array_type->Valid()) {
            return TYPE_ERROR;
          }
          if (cur_array_type->dims() != 0) {
            return TYPE_ERROR;
          }
          cur_type = cur_array_type->base_type();
          in_array = 0;
        }
        vistor.clear();
        if (cur_type->template_type() != TypeTemplate::TYPE::RECORD) {
          return TYPE_ERROR;
        }
        cur_type =
            cur_type->DynamicCast<RecordType>()->Find((*var_parts)[i].name);
        if (cur_type == TYPE_ERROR) {
          return TYPE_ERROR;
        }
      }
    }
    if (in_array) {
      if (cur_type->template_type() != TypeTemplate::TYPE::ARRAY) {
        return TYPE_ERROR;
      }
      auto cur_array_type =
          new ArrayType(cur_type->DynamicCast<ArrayType>()->Visit(vistor));
      PtrCollect(cur_array_type);
      if (!cur_array_type->Valid()) {
        return TYPE_ERROR;
      }
      if (cur_array_type->dims() == 0) {
        cur_type = cur_array_type->base_type();
      } else
        cur_type = cur_array_type;
    }
    return cur_type;
  }
};

struct IDVarpartAttr {
  VarParts* var_part;
  IDVarPartNode* id_varpart_node;
};

struct ExpressionListAttr {
  std::vector<TypeTemplate*>* type_ptr_list;
  ExpressionListNode* expression_list_node;
  std::vector<bool>* is_lvalue_list;
};


extern std::string buf;

// #define YYSTYPE Token
// #define YYSTYPE

//#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED

struct YYSTYPE {
  Token token_info;
  ValueAttr value_node_info;
  union {
    IdListAttr id_list_node_info;
    TypeAttr type_node_info;
    StandardTypeAttr standared_type_node_info;
    PeriodsAttr periods_node_info;
    PeriodAttr period_node_info;
    RecordAttr record_node_info;
    FormalParameterAttr formal_parameter_node_info;
    ParameterListsAttr parameter_lists_node_info;
    ParameterListAttr parameter_list_node_info;
    VarParameterAttr var_parameter_node_info;
    ValueParameterAttr value_parameter_node_info;

    VariableDeclarationAttr variable_declaration_node_info;
    VariableAttr variable_node_info;
    VariableListAttr variable_list_node_info;
    ExpressionAttr expression_node_info;
    SimpleExpressionAttr simple_expression_node_info;
    TermAttr term_node_info;
    FactorAttr factor_node_info;
    UnsignedConstantVarAttr unsigned_constant_var_node_info;
    IDVarpartsAttr id_varparts_node_info;
    IDVarpartAttr id_varpart_node_info;
    ExpressionListAttr expression_list_node_info;
    ProgramNode* program_node;
    ProgramHeadNode* program_head_node;
    ProgramBodyNode* program_body_node;
    ConstDeclarationsNode* const_declarations_node;
    ConstDeclarationNode* const_declaration_node;
    BasicTypeNode* basic_type_node;
    VariableDeclarationsNode* variable_declarations_node;
    SubprogramDeclarationsNode* subprogram_declarations_node;
    SubprogramDeclarationNode* subprogram_declaration_node;
    SubprogramHeadNode* subprogram_head_node;
    SubprogramBodyNode* subprogram_body_node;
    CompoundStatementNode* compound_statement_node;
    StatementListNode* statement_list_node;
    StatementNode* statement_node;
    ElseNode* else_node;
    UpdownNode* updown_node;
    ProcedureCallNode* procedure_call_node;
  };
};
typedef struct YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif
//#endif
