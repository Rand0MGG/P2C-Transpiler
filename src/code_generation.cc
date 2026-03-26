#include "code_generation.h" // Changed from pstdlib.h
#include "ast.h"      // For LeafNode definition and its members
#include "type.h"     // For BasicType, TYPE_INT, etc.
#include "log.h"      // For log_debug, log_info, etc. if needed (already used by PStdLibs)
#include <cstdio>    // For FILE, fprintf
#include <ctime>     // For time_t, tm, localtime, time (used in ProgramHeadNode)

CodeGenerator::CodeGenerator() { // Changed from PStdLibs
  lib_map_["abs"] = {"#define abs(x) (x < 0 ? -x : x)", false};
  lib_map_["sqr"] = {"#define sqr(x) (x * x)", false};
  lib_map_["odd"] = {"#define odd(x) ((x < 0 ? -x : x) % 2 == 1)", false};
  lib_map_["chr"] = {"#define chr(x) ((char)x)", false};
  lib_map_["ord"] = {"#define ord(x) ((int)x)", false};
  lib_map_["succ"] = {"#define succ(x) (x + 1)", false};
  lib_map_["pred"] = {"#define pred(x) (x - 1)", false};
  lib_map_["round"] = {"#define round(x) ((int)(x + 0.5))", false};
  lib_map_["trunc"] = {"#define trunc(x) ((int)x)", false};
  lib_map_["sin"] = {"#define sin(x) (sin((float)x))", false};
  lib_map_["cos"] = {"#define cos(x) (cos((float)x))", false};
  lib_map_["exp"] = {"#define exp(x) (exp((float)x))", false};
  lib_map_["ln"] = {"#define ln(x) (log((float)x))", false};
  lib_map_["sqrt"] = {"#define sqrt(x) (sqrt((float)x))", false};
  lib_map_["arctan"] = {"#define arctan(x) (atan((float)x))", false};
  lib_map_["eof"] = {"#define eof() (feof(stdin)==0)", false};
  lib_map_["eoln"] = {
      "int eoln() {if(feof(stdin)==0) return 1;\nint c = "
      "getc(stdin);\nungetc(c,stdin);\nreturn c == '\\n' || c == EOF;\n}",
      false};
}

void CodeGenerator::Preset(SymbolTable* st) { // Changed from PStdLibs
  using ParamMode = FunctionSymbol::PARAM_MODE;

  struct param_struct {
    std::string name;
    BasicType* return_type;
    BasicType* param_type;
  };

  std::vector<param_struct> params = {
      {"abs", TYPE_INT, TYPE_INT},      {"sqr", TYPE_INT, TYPE_INT},
      {"odd", TYPE_BOOL, TYPE_INT},     {"chr", TYPE_CHAR, TYPE_INT},
      {"ord", TYPE_INT, TYPE_CHAR},     {"succ", TYPE_CHAR, TYPE_CHAR},
      {"pred", TYPE_CHAR, TYPE_CHAR},   {"round", TYPE_INT, TYPE_REAL},
      {"trunc", TYPE_INT, TYPE_REAL},   {"sin", TYPE_REAL, TYPE_REAL},
      {"cos", TYPE_REAL, TYPE_REAL},    {"exp", TYPE_REAL, TYPE_REAL},
      {"ln", TYPE_REAL, TYPE_REAL},     {"sqrt", TYPE_REAL, TYPE_REAL},
      {"arctan", TYPE_REAL, TYPE_REAL}, {"eof", TYPE_BOOL, nullptr},
      {"eoln", TYPE_BOOL, nullptr},     {"read", TYPE_NONE, nullptr},
      {"readln", TYPE_NONE, nullptr},   {"write", TYPE_NONE, nullptr},
      {"writeln", TYPE_NONE, nullptr}};

  for (auto& p : params) {
    std::vector<FunctionSymbol::ParameterInfo> param_list;
    if (p.param_type != nullptr)
      param_list.emplace_back(FunctionSymbol::ParameterInfo{"x", p.param_type, ParamMode::VALUE});
    st->Insert(p.name,
               new FunctionSymbol(p.name, p.return_type, 0, param_list));
  }
}

bool CodeGenerator::Call(std::string lib_name) { // Changed from PStdLibs
  if (lib_map_.find(lib_name) != lib_map_.end()) {
    lib_map_[lib_name].called_ = true;
    return true;
  }
  return false;
}

bool CodeGenerator::UnCall(std::string lib_name) { // Changed from PStdLibs
  if (lib_map_.find(lib_name) != lib_map_.end()) {
    lib_map_[lib_name].called_ = false;
    return true;
  }
  return false;
}

void CodeGenerator::FormatStdLib(FILE* dst) { // Changed from PStdLibs::Format
  // format cstdlib
  fprintf(dst, "#include <stdio.h>\n");
  fprintf(dst, "#include <string.h>\n");
  if (CheckMathLib()) {
    fprintf(dst, "#include <math.h>\n\n");
  }
  // format defines
  for (auto& lib : lib_map_) {
    if (lib.second.called_) {
      fprintf(dst, "%s\n", lib.second.def_.c_str());
    }
  }
}

bool CodeGenerator::CheckMathLib() { // Changed from PStdLibs
  return lib_map_["sin"].called_ || lib_map_["cos"].called_ ||
         lib_map_["exp"].called_ || lib_map_["ln"].called_ ||
         lib_map_["sqrt"].called_ || lib_map_["arctan"].called_;
}

// Implementation of FormatLeafNode
void CodeGenerator::FormatLeafNode(const LeafNode* node, FILE* dst) {
  // We need to access node->value_ which is private in LeafNode.
  // This indicates a design issue. For now, we might need to make value_ accessible
  // or add a public getter in LeafNode, e.g., node->getValue().
  // Assuming a getter `node->getValue()` which returns `ConstValue` and `node->getType()` which returns `BasicType*`
  // Actually, LeafNode has public methods: value() (template) and type().

  const LeafNode* leaf = static_cast<const LeafNode*>(node);
  BasicType *tp = leaf->type();
  if (tp == TYPE_INT) {
    fprintf(dst, "%d", leaf->value<int>());
  } else if (tp == TYPE_REAL) {
    fprintf(dst, "%.6f", leaf->value<float>());
  } else if (tp == TYPE_BOOL) {
    fprintf(dst, "%d", leaf->value<bool>());
  } else if (tp == TYPE_CHAR) {
    fprintf(dst, "'%c'", leaf->value<char>());
  } else if (tp == TYPE_STRINGLIKE) {
    // Ensure value<std::string>() is safe if type is STRINGLIKE
    fprintf(dst, "%s", leaf->value<std::string>().c_str());
  }
  // Add other type handlers if necessary, or a default case
}

// Implementation of FormatBasicTypeNode
void CodeGenerator::FormatBasicTypeNode(const BasicTypeNode* node, FILE* dst) {
  if (node && node->type()) { 
    fprintf(dst, "%s", node->type()->c_name().c_str());
  } else {
    fprintf(dst, "<invalid_type_node>"); 
  }
}

void CodeGenerator::FormatIdListNode(const IdListNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_id_list_node>"); return; }
  if (node->get_child(0)) {
    CodeGenerator::DispatchFormat(node->get_child(0), dst);
  }
  if (node->grammar_type() == IdListNode::GrammarType::MULTIPLE_ID) {
    fprintf(dst, ", ");
    if (node->get_child(1)) {
      CodeGenerator::DispatchFormat(node->get_child(1), dst);
    }
  }
}

void CodeGenerator::FormatPeriodNode(const PeriodNode* node, FILE* dst) {
  if (!node) {
    fprintf(dst, "<null_period_node>");
    return;
  }
  fprintf(dst, "[%d]", node->len());
}

void CodeGenerator::FormatProgramHeadNode(const ProgramHeadNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_program_head_node>"); return; }
  time_t now = time(nullptr);
  tm *local_time_info = localtime(&now);
  fprintf(dst, "/// [");
  if (node->get_child(0)) {
    CodeGenerator::DispatchFormat(node->get_child(0), dst);
  }
  if (local_time_info) {
      fprintf(dst, "] created on %d/%d/%d BY GROUP 10\n", 1900 + local_time_info->tm_year, 1 + local_time_info->tm_mon, local_time_info->tm_mday);
  } else {
      fprintf(dst, "] created on <unknown_date> BY GROUP 10\n");
  }
}

void CodeGenerator::FormatTypeNode(const TypeNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_type_node>"); return; }
  switch (node->grammar_type()) {
    case TypeNode::GrammarType::BASIC_TYPE:
      if (node->get_child(0)) {
        CodeGenerator::DispatchFormat(node->get_child(0), dst);
      }
      break;
    case TypeNode::GrammarType::ARRAY:
      break;
    case TypeNode::GrammarType::RECORD_TYPE:
      fprintf(dst, "struct {\n");
      if (node->get_child(0)) {
        CodeGenerator::DispatchFormat(node->get_child(0), dst);
      }
      fprintf(dst, "}");
      break;
    default:
      fprintf(dst, "<unknown_type_node_subtype>");
      break;
  }
}

void CodeGenerator::FormatTypeNodePeriods(const TypeNode* node, FILE* dst) {
  if (!node) {
    fprintf(dst, "<null_type_node_for_periods>");
    return;
  }
  if (node->grammar_type() == TypeNode::GrammarType::ARRAY) {
    if (node->child_count() >= 2) { 
      if (node->get_child(0)) {
        CodeGenerator::DispatchFormat(node->get_child(0), dst);
      }
      const Node* child1_node = node->get_child(1);
      if (child1_node) {
        const TypeNode* next_type_node = dynamic_cast<const TypeNode*>(child1_node);
        if (next_type_node) {
          CodeGenerator::FormatTypeNodePeriods(next_type_node, dst); 
        } else {
          fprintf(dst, "<error_unexpected_child_in_array_periods>");
        }
      }
    } else {
        fprintf(dst, "<error_malformed_array_for_periods>");
    }
  } 
}

static void FormatBasicTypeForConstDecl(BasicType* type, FILE* dst) {
  if (!type) return;
  if (type == TYPE_INT) { fprintf(dst, "int "); }
  else if (type == TYPE_REAL) { fprintf(dst, "float "); }
  else if (type == TYPE_CHAR) { fprintf(dst, "char "); }
  // Add other types if necessary, e.g., bool if it has a distinct C representation.
}

void CodeGenerator::FormatConstDeclarationNode(const ConstDeclarationNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_const_decl_node>"); return; }
  if (node->grammar_type() == ConstDeclarationNode::GrammarType::DECLARATION) {
    if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
    fprintf(dst, "const ");
    FormatBasicTypeForConstDecl(node->type(), dst);
    if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
    fprintf(dst, " = ");
    if (node->get_child(2)) CodeGenerator::DispatchFormat(node->get_child(2), dst);
    fprintf(dst, ";\n");
  } else if (node->grammar_type() == ConstDeclarationNode::GrammarType::VALUE) {
    fprintf(dst, "const ");
    FormatBasicTypeForConstDecl(node->type(), dst);
    if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
    fprintf(dst, " = ");
    if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
    fprintf(dst, ";\n");
  }
}

void CodeGenerator::FormatVariableDeclarationNode(const VariableDeclarationNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_var_decl_node>"); return; }
  if (node->grammar_type() == VariableDeclarationNode::GrammarType::MULTIPLE_DECL) {
    if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
  }
  int child_count = node->child_count();
  if ( (node->grammar_type() == VariableDeclarationNode::GrammarType::MULTIPLE_DECL && child_count < 3) || 
       (node->grammar_type() == VariableDeclarationNode::GrammarType::SINGLE_DECL && child_count < 2) ) {
      fprintf(dst, "<malformed_var_decl_node>"); return; 
  }
  Node* idlist_node_orig = node->get_child(child_count - 2);
  Node* tnode_orig = node->get_child(child_count - 1);
  const IdListNode* idlist_node = dynamic_cast<const IdListNode*>(idlist_node_orig);
  if (!idlist_node) { fprintf(dst, "<error_expected_idlist_node>"); return; }
  std::vector<LeafNode*> idlist = idlist_node->Lists(); 
  if (node->list_type() == VariableDeclarationNode::ListType::ID) {
    if (tnode_orig) CodeGenerator::DispatchFormat(tnode_orig, dst);
    fprintf(dst, " ");
    for (size_t i = 0; i < idlist.size(); ++i) {
      if (idlist[i]) CodeGenerator::DispatchFormat(idlist[i], dst);
      if (i < idlist.size() - 1) { fprintf(dst, ", "); }
    }
  } else { 
    const TypeNode* actual_type_node = dynamic_cast<const TypeNode*>(tnode_orig);
    if (!actual_type_node || !actual_type_node->base_type()) { fprintf(dst, "<error_expected_type_node_with_base_type>"); return; }
    CodeGenerator::DispatchFormat(actual_type_node->base_type(), dst);
    fprintf(dst, " ");
    bool array_flag = (actual_type_node->grammar_type() == TypeNode::GrammarType::ARRAY);
    for (size_t i = 0; i < idlist.size(); ++i) {
      if (idlist[i]) CodeGenerator::DispatchFormat(idlist[i], dst);
      if (array_flag) {
        CodeGenerator::FormatTypeNodePeriods(actual_type_node, dst); // This is a specific call, not general dispatch
      }
      if (i < idlist.size() - 1) { fprintf(dst, ", "); }
    }
  }
  fprintf(dst, ";\n");
}

void CodeGenerator::FormatSubprogramDeclarationsNode(const SubprogramDeclarationsNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_subprogram_declarations_node>"); return; }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst);
  }
}

void CodeGenerator::FormatSubprogramDeclarationNode(const SubprogramDeclarationNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_subprogram_declaration_node>"); return; }
  if (node->child_count() < 2) { fprintf(dst, "<malformed_subprogram_declaration_node>"); return; }
  Node* head_node_generic = node->get_child(0);
  const SubprogramHeadNode* headnode = dynamic_cast<const SubprogramHeadNode*>(head_node_generic);
  Node* body_node_generic = node->get_child(1);
  if (!headnode) { fprintf(dst, "<error_expected_subprogram_head>"); return; }
  bool func_flag = (headnode->grammar_type() == SubprogramHeadNode::GrammarType::FUNCTION);
  std::string func_internal_id; BasicType *return_type = nullptr;
  if (func_flag) {
    func_internal_id = "internal_" + headnode->id();
    if (headnode->child_count() >= 3) { 
        const BasicTypeNode* return_type_node = dynamic_cast<const BasicTypeNode*>(headnode->get_child(2));
        if (return_type_node) return_type = return_type_node->type();
    }
    if (!return_type) { fprintf(dst, "<error_missing_return_type_for_function>"); return; }
  }
  if (head_node_generic) CodeGenerator::DispatchFormat(head_node_generic, dst);
  fprintf(dst, " {\n");
  if (func_flag && return_type) {
    fprintf(dst, "  %s %s;\n", return_type->c_name().c_str(), func_internal_id.c_str());
  }
  if (body_node_generic) CodeGenerator::DispatchFormat(body_node_generic, dst);
  if (func_flag && return_type) {
    fprintf(dst, "  return %s;\n", func_internal_id.c_str());
  }
  fprintf(dst, "}\n");
}

void CodeGenerator::FormatSubprogramHeadNode(const SubprogramHeadNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_subprogram_head_node>"); return; }
  if (node->grammar_type() == SubprogramHeadNode::GrammarType::PROCEDURE) {
    fprintf(dst, "void ");
  } else { 
    if (node->get_child(2)) CodeGenerator::DispatchFormat(node->get_child(2), dst);
    fprintf(dst, " ");
  }
  if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
  if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
}

void CodeGenerator::FormatFormalParamNode(const FormalParamNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_formal_param_node>"); return; }
  fprintf(dst, "(");
  if (node->child_count() > 0 && node->get_child(0)) {
    CodeGenerator::DispatchFormat(node->get_child(0), dst);
  }
  fprintf(dst, ")");
}

void CodeGenerator::FormatParamListsNode(const ParamListsNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_param_lists_node>"); return; }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst);
    if (i < node->child_count() - 1) {
      fprintf(dst, ","); 
    }
  }
}

void CodeGenerator::FormatVarParamNode(const VarParamNode* node, FILE* dst) {
  if (!node || node->child_count() == 0) { fprintf(dst, "<malformed_var_param_node>"); return; }
  const ValueParamNode* vp_node = dynamic_cast<const ValueParamNode*>(node->get_child(0));
  if (vp_node) {
    CodeGenerator::FormatValueParamNode(vp_node, true, dst);
  } else {
    fprintf(dst, "<error_expected_value_param_in_var_param>");
  }
}

void CodeGenerator::FormatValueParamNode(const ValueParamNode* node, FILE* dst) {
  CodeGenerator::FormatValueParamNode(node, false, dst);
}

void CodeGenerator::FormatValueParamNode(const ValueParamNode* node, bool ref, FILE* dst) {
  if (!node || node->child_count() < 2) { fprintf(dst, "<malformed_value_param_node>"); return; }

  const IdListNode* idnode = dynamic_cast<const IdListNode*>(node->get_child(0));
  const BasicTypeNode* tnode = dynamic_cast<const BasicTypeNode*>(node->get_child(1));

  if (!idnode || !tnode) { fprintf(dst, "<error_value_param_children_type_mismatch>"); return; }

  std::vector<LeafNode*> idlist = idnode->Lists();
  // BasicTypeNode::TypeName() is const and takes bool ref
  std::string tname = tnode->TypeName(ref); 

  for (size_t i = 0; i < idlist.size(); ++i) {
    if (idlist[i]) { // LeafNode::id_ref() is const
        fprintf(dst, "%s %s", tname.c_str(), idlist[i]->id_ref().c_str());
    }
    if (i < idlist.size() - 1) {
      fprintf(dst, ", ");
    }
  }
}

void CodeGenerator::FormatElseNode(const ElseNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_else_node>"); return; }
  switch (node->grammar_type()) {
    case ElseNode::GrammarType::EPSILON:
      return;
    case ElseNode::GrammarType::ELSE_STATEMENT: {
      fprintf(dst, "else {\n");
      Node* child_statement = node->get_child(0);
      if (child_statement) CodeGenerator::DispatchFormat(child_statement, dst);
      fprintf(dst, "}\n");
      break;
    }
    default:
      fprintf(dst, "<unknown_else_node_type>");
      break;
  }
}

void CodeGenerator::FormatProcedureCallNode(const ProcedureCallNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_procedure_call_node>"); return; }
  if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
  
  if (node->grammar_type() == ProcedureCallNode::GrammarType::ID) {
    fprintf(dst, "();");
  } else if (node->grammar_type() == ProcedureCallNode::GrammarType::ID_EXP_LIST) {
    fprintf(dst, "(");
    if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
    fprintf(dst, ");");
  }
}

void CodeGenerator::FormatStatementNode(const StatementNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_statement_node>"); return; }

  switch (node->grammar_type()) {
    case StatementNode::GrammarType::EPSILON:
      return;
    case StatementNode::GrammarType::VAR_ASSIGN_OP_EXP:
    case StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP: {
      const ExpressionNode* exp =
          node->child_count() > 1 ? dynamic_cast<const ExpressionNode*>(node->get_child(1)) : nullptr;
      if (!exp || !node->get_child(0)) { fprintf(dst, "<malformed_assign_statement>"); break; }

      switch (exp->target_type()) {
        case ExpressionNode::TargetType::EXPRESSION: {
          CodeGenerator::DispatchFormat(node->get_child(0), dst);
          fprintf(dst, " = ");
          CodeGenerator::DispatchFormat(node->get_child(1), dst);
          break;
        }
        case ExpressionNode::TargetType::VAR_ARRAY: { 
          fprintf(dst, "memcpy(");
          CodeGenerator::DispatchFormat(node->get_child(0), dst);
          fprintf(dst, ", ");
          CodeGenerator::DispatchFormat(node->get_child(1), dst);
          fprintf(dst, ", sizeof(");
          CodeGenerator::DispatchFormat(node->get_child(0), dst);
          fprintf(dst, "))");
          break;
        }
        case ExpressionNode::TargetType::CONST_STRING: { 
          fprintf(dst, "strcpy(");
          CodeGenerator::DispatchFormat(node->get_child(0), dst);
          fprintf(dst, ", ");
          CodeGenerator::DispatchFormat(node->get_child(1), dst);
          fprintf(dst, ")");
          break;
        }
      }
      fprintf(dst, ";\n");
      break;
    }
    case StatementNode::GrammarType::PROCEDURE_CALL:
    case StatementNode::GrammarType::COMPOUND_STATEMENT: {
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      break;
    }
    case StatementNode::GrammarType::IF_STATEMENT: {
      fprintf(dst, "if (");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, ") {\n");
      if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
      fprintf(dst, "}\n");
      if (node->get_child(2)) CodeGenerator::DispatchFormat(node->get_child(2), dst);
      break;
    }
    case StatementNode::GrammarType::FOR_STATEMENT: {
      if (node->child_count() < 4) { fprintf(dst, "<malformed_for_statement>"); break; }
      const UpdownNode* updown_node = dynamic_cast<const UpdownNode*>(node->get_child(2));
      if (!updown_node) { fprintf(dst, "<error_expected_updown_node_in_for>"); break; }
      bool increase = updown_node->IsIncrease();
      fprintf(dst, "for (");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, "=");
      if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
      fprintf(dst, ";");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, increase ? "<=" : ">=");
      if (node->get_child(3)) CodeGenerator::DispatchFormat(node->get_child(3), dst);
      fprintf(dst, ";");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, increase ? "++" : "--");
      fprintf(dst, ") {\n");
      for (size_t i = 4; i < node->child_count(); ++i) {
          if(node->get_child(i)) CodeGenerator::DispatchFormat(node->get_child(i), dst);
      }
      fprintf(dst, "}\n");
      break;
    }
    case StatementNode::GrammarType::READ_STATEMENT:
    case StatementNode::GrammarType::READLN_STATEMENT: {
      if (node->child_count() > 0) {
        const VariableListNode* vlnodes = dynamic_cast<const VariableListNode*>(node->get_child(0));
        if (vlnodes) {
          fprintf(dst, "scanf(\"%s\", ", vlnodes->FormatString().c_str()); 
          CodeGenerator::FormatVariableListNode(vlnodes, true, dst);
          fprintf(dst, ");\n");
        } else { fprintf(dst, "<error_expected_varlist_for_read>");}
      }
      if (node->grammar_type() == StatementNode::GrammarType::READLN_STATEMENT) {
        fprintf(dst, "while(1){\nchar c = getchar(); if(c == '\\n' || c== EOF) break;\n};\n");
      }
      break;
    }
    case StatementNode::GrammarType::WRITE_STATEMENT:
    case StatementNode::GrammarType::WRITELN_STATEMENT: {
      if (node->child_count() == 0 && node->grammar_type() == StatementNode::GrammarType::WRITELN_STATEMENT) {
        fprintf(dst, "printf(\"\\n\");\n"); 
        break;
      } else if (node->child_count() == 0 && node->grammar_type() == StatementNode::GrammarType::WRITE_STATEMENT) {
        break; 
      }
      const ExpressionListNode* elnode = dynamic_cast<const ExpressionListNode*>(node->get_child(0));
      if (elnode) {
        if (node->grammar_type() == StatementNode::GrammarType::WRITELN_STATEMENT) {
          fprintf(dst, "printf(\"%s\\n\"", elnode->FormatString().c_str()); 
        } else {
          fprintf(dst, "printf(\"%s\"", elnode->FormatString().c_str());
        }
        fprintf(dst, ", ");
        CodeGenerator::FormatExpressionListNode(elnode, dst);
        fprintf(dst, ");\n");
      } else { fprintf(dst, "<error_expected_explist_for_write>");}
      break;
    }
    case StatementNode::GrammarType::CASE_STATEMET: {
      fprintf(dst, "switch (");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, ") {\n");
      if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
      fprintf(dst, "}\n");
      break;
    }
    case StatementNode::GrammarType::WHILE_STATEMENT: {
      fprintf(dst, "while (");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, ") {\n");
      if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
      fprintf(dst, "}\n");
      break;
    }
    case StatementNode::GrammarType::REPEAT_STATEMENT: {
      fprintf(dst, "do {\n");
      if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
      fprintf(dst, "} while (!(");
      if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
      fprintf(dst, "));\n");
      break;
    }
    default:
      fprintf(dst, "<unknown_statement_type>");
      break;
  }
}

// New Implementations:
void CodeGenerator::FormatVariableNode(const VariableNode* node, bool ref, FILE* dst) {
  if (!node) { fprintf(dst, "<null_variable_node>"); return; }
  if (ref) { fprintf(dst, "&"); }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst);
  }
}

void CodeGenerator::FormatVariableListNode(const VariableListNode* node, FILE* dst) {
  // This overload calls the one with ref=false
  CodeGenerator::FormatVariableListNode(node, false, dst);
}

void CodeGenerator::FormatVariableListNode(const VariableListNode* node, bool ref, FILE* dst) {
  if (!node) { fprintf(dst, "<null_variable_list_node>"); return; }

  if (node->grammar_type() == VariableListNode::GrammarType::VARIABLE) {
    Node* child_var_node_generic = node->get_child(0);
    if (child_var_node_generic) {
        const VariableNode* child_var_node = dynamic_cast<const VariableNode*>(child_var_node_generic);
        if (child_var_node) {
            CodeGenerator::FormatVariableNode(child_var_node, ref, dst);
        } else {
            fprintf(dst, "<error_casting_to_VariableNode_in_VarList>");
        }
    }
  } else if (node->grammar_type() == VariableListNode::GrammarType::VARIABLE_LIST_VARIABLE) {
    Node* child_list_node_generic = node->get_child(0);
    if (child_list_node_generic) {
        const VariableListNode* child_list_node = dynamic_cast<const VariableListNode*>(child_list_node_generic);
        if (child_list_node) {
            CodeGenerator::FormatVariableListNode(child_list_node, ref, dst);
        } else {
            fprintf(dst, "<error_casting_to_VariableListNode_in_VarList>");
        }
    }
    fprintf(dst, ",");
    Node* child_var_node_generic = node->get_child(1);
    if (child_var_node_generic) {
        const VariableNode* child_var_node = dynamic_cast<const VariableNode*>(child_var_node_generic);
        if (child_var_node) {
            CodeGenerator::FormatVariableNode(child_var_node, ref, dst);
        } else {
            fprintf(dst, "<error_casting_to_VariableNode_in_VarList2>");
        }
    }
  }
}

void CodeGenerator::FormatExpressionListNode(const ExpressionListNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_expression_list_node>"); return; }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst); // Changed child->Format(dst)
    if (i < node->child_count() - 1) {
      fprintf(dst, ", ");
    }
  }
}

// Implementations for the final batch
void CodeGenerator::FormatExpressionNode(const ExpressionNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_expression_node>"); return; }
  if (node->is_ref()) { fprintf(dst, "&"); }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst); // Changed child->Format(dst)
  }
}

void CodeGenerator::FormatTermNode(const TermNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_term_node>"); return; }
  if (node->get_op_div()) { fprintf(dst, "(float)"); }
  for (size_t i = 0; i < node->child_count(); ++i) {
    Node* child = node->get_child(i);
    if (child) CodeGenerator::DispatchFormat(child, dst); // Changed child->Format(dst)
  }
}

void CodeGenerator::FormatFactorNode(const FactorNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_factor_node>"); return; }
  Node* child0 = (node->child_count() > 0) ? node->get_child(0) : nullptr;
  Node* child1 = (node->child_count() > 1) ? node->get_child(1) : nullptr;

  switch (node->grammar_type()) { // grammar_type() is const
    case FactorNode::GrammarType::UCONST_VAR:   // Child 0: UnsignConstVarNode
    case FactorNode::GrammarType::VARIABLE:     // Child 0: VariableNode
      if (child0) CodeGenerator::DispatchFormat(child0, dst);
      break;
    case FactorNode::GrammarType::EXP:          // Child 0: ExpressionNode
      fprintf(dst, "(");
      if (child0) CodeGenerator::DispatchFormat(child0, dst);
      fprintf(dst, ")");
      break;
    case FactorNode::GrammarType::ID_EXP_LIST:  // Child 0: id (LeafNode), Child 1: ExpressionListNode
      if (child0) CodeGenerator::DispatchFormat(child0, dst);
      fprintf(dst, "(");
      if (child1) CodeGenerator::DispatchFormat(child1, dst);
      fprintf(dst, ")");
      break;
    case FactorNode::GrammarType::NOT_BOOL:
      fprintf(dst, "!");
      if (child0) CodeGenerator::DispatchFormat(child0, dst); // Child 0: FactorNode
      break;
    case FactorNode::GrammarType::NOT_INT:
      fprintf(dst, "~");
      if (child0) CodeGenerator::DispatchFormat(child0, dst); // Child 0: FactorNode
      break;
    default:
      fprintf(dst, "<unknown_factor_type>");
      break;
  }
}

void CodeGenerator::FormatIDVarPartNode(const IDVarPartNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_id_var_part_node>"); return; }

  if (node->grammar_type() == IDVarPartNode::GrammarType::_ID) { // Child 0: id (LeafNode)
    fprintf(dst, ".");
    if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst); // Changed node->get_child(0)->Format(dst)
  } else if (node->grammar_type() == IDVarPartNode::GrammarType::EXP_LIST) { // Child 0: ExpressionListNode
    fprintf(dst, "[");
    Node* expr_list_node = node->get_child(0);
    CodeGenerator::FormatFlattenedExpressionList(expr_list_node, dst);
    int current_array_lb = node->array_lb(); // array_lb() is const. This lb applies to THIS part.
    if (current_array_lb > 0) {
      fprintf(dst, " - %d", current_array_lb);
      fprintf(dst, " + %d", -current_array_lb);
    }
    fprintf(dst, "]");
  }
}
void CodeGenerator::FormatFlattenedExpressionList(Node* node, FILE* dst) {
    auto* list_node = node->DynamicCast<ExpressionListNode>();
    if (!list_node) {
        // 如果是单个 expression 节点，直接打印
        CodeGenerator::DispatchFormat(node,dst);
        return;
    }
    // 如果是 expression_list ',' expression 类型
    if (list_node->child_count() == 2) {
        CodeGenerator::FormatFlattenedExpressionList(list_node->get_child(0), dst);  // 递归左侧
        fprintf(dst, "]["); // 打印中括号分隔
        CodeGenerator::DispatchFormat(list_node->get_child(1),dst);  // 打印右侧 expression
    } else if (list_node->child_count() == 1) {
        // 表达式列表只有一个表达式，直接打印
        DispatchFormat(list_node->get_child(0), dst);
    }
}
void CodeGenerator::FormatProgramBodyNode(const ProgramBodyNode* node, FILE* dst) {
  if (!node) { fprintf(dst, "<null_program_body_node>"); return; }
  // Children: 0: const_declarations, 1: var_declarations, 2: subprogram_declarations, 3: compound_statement
  if (node->get_child(0)) CodeGenerator::DispatchFormat(node->get_child(0), dst);
  if (node->get_child(1)) CodeGenerator::DispatchFormat(node->get_child(1), dst);
  if (node->get_child(2)) CodeGenerator::DispatchFormat(node->get_child(2), dst);
  fprintf(dst, "int main() {\n");
  if (node->get_child(3)) CodeGenerator::DispatchFormat(node->get_child(3), dst);
  fprintf(dst, "  return 0;\n"); // Added indent for return
  fprintf(dst, "}\n");
}

// Central dispatcher implementation
void CodeGenerator::DispatchFormat(const Node* node, FILE* dst) {
  if (!node) {
    // Optionally print a placeholder or log if a null node is dispatched
    // fprintf(dst, "<null_node_in_dispatch>");
    return;
  }

  // Try to cast to specific known types that have dedicated Format methods in CodeGenerator
  if (auto specific_node = dynamic_cast<const LeafNode*>(node)) {
    FormatLeafNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const BasicTypeNode*>(node)) {
    FormatBasicTypeNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const IdListNode*>(node)) {
    FormatIdListNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const PeriodNode*>(node)) {
    FormatPeriodNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ProgramHeadNode*>(node)) {
    FormatProgramHeadNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const TypeNode*>(node)) {
    FormatTypeNode(specific_node, dst); // Note: PeriodsFormat is called contextually, not directly by general dispatch
  } else if (auto specific_node = dynamic_cast<const ConstDeclarationNode*>(node)) {
    FormatConstDeclarationNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const VariableDeclarationNode*>(node)) {
    FormatVariableDeclarationNode(specific_node, dst);
  // Note: SubprogramDeclarationsNode might not have a specific formatter in CodeGenerator if it relies on default child iteration.
  // If it does (e.g., FormatSubprogramDeclarationsNode just iterates children), it can be added here.
  // For now, assuming it and similar containers will fall through to the default child iteration logic below.
  } else if (auto specific_node = dynamic_cast<const SubprogramDeclarationNode*>(node)) {
    FormatSubprogramDeclarationNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const SubprogramHeadNode*>(node)) {
    FormatSubprogramHeadNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const FormalParamNode*>(node)) {
    FormatFormalParamNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ParamListsNode*>(node)) {
    FormatParamListsNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const VarParamNode*>(node)) {
    FormatVarParamNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ValueParamNode*>(node)) {
    FormatValueParamNode(specific_node, dst); // Calls the overload that defaults ref to false
  } else if (auto specific_node = dynamic_cast<const StatementNode*>(node)) {
    FormatStatementNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ElseNode*>(node)) {
    FormatElseNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ProcedureCallNode*>(node)) {
    FormatProcedureCallNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const VariableNode*>(node)) {
    // Variable(bool ref, ...) - DispatchFormat doesn't know 'ref'.
    // This implies FormatVariableNode might need a default ref or context.
    // For now, assume ref=false for a general dispatch call if not otherwise specified.
    // This could be an issue if VariableNode is dispatched directly and needs ref=true.
    // Let's assume direct calls to FormatVariableNode with specific ref are done by its parent formatter.
    // If DispatchFormat hits a VariableNode, it's likely for a simple non-ref usage or a part of a larger structure.
    FormatVariableNode(specific_node, false, dst); 
  } else if (auto specific_node = dynamic_cast<const VariableListNode*>(node)) {
    FormatVariableListNode(specific_node, dst); // Calls the overload that defaults ref to false
  } else if (auto specific_node = dynamic_cast<const ExpressionListNode*>(node)) {
    FormatExpressionListNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ExpressionNode*>(node)) {
    FormatExpressionNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const TermNode*>(node)) {
    FormatTermNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const FactorNode*>(node)) {
    FormatFactorNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const IDVarPartNode*>(node)) {
    FormatIDVarPartNode(specific_node, dst);
  } else if (auto specific_node = dynamic_cast<const ProgramBodyNode*>(node)) {
    FormatProgramBodyNode(specific_node, dst);
  // Add other specific types above this else block.
  // Default handling for nodes that don't have a specific CodeGenerator::Format NodeType method
  // (e.g., ProgramNode, StatementListNode, CompoundStatementNode, etc.)
  // This replicates the original Node::Format virtual function's behavior.
  } else {
    for (size_t i = 0; i < node->child_count(); ++i) {
        Node* child = node->get_child(i); // get_child is const and returns Node*
        if (child) {
            DispatchFormat(child, dst); // Recursive call to the dispatcher
        }
    }
  }
}

// Method to format the entire AST (program)
void CodeGenerator::FormatProgram(const AST* ast_ptr, FILE* dst) {
  if (!ast_ptr || !ast_ptr->Valid()) {
    // Handle invalid AST, perhaps print an error or do nothing
    fprintf(stderr, "Error: Attempting to format an invalid or null AST.\n");
    return;
  }

  Node* root_node = ast_ptr->root();
  CodeGenerator* libs_instance = ast_ptr->libs(); // libs() returns CodeGenerator*

  if (libs_instance) {
    libs_instance->FormatStdLib(dst); // Call non-static member on the instance
  }

  if (root_node) {
    CodeGenerator::DispatchFormat(root_node, dst);
  }

  // The log_info and fclose(dst) logic from original AST::Format
  // can either stay in AST::Format after this call, or be moved here.
  // For a cleaner separation, it might be better to move it here too.
  log_info("AST 代码生成成功"); // log_info from log.h
  if (dst != stdout && dst != stderr) { // Avoid closing stdout/stderr
      // It's generally safer for the caller管理文件流的打开和关闭
      // fclose(dst); // Consider if CodeGenerator should be responsible for closing.
                  // If dst is opened by main, main should close it.
  }
} 