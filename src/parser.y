%code requires {
  #include "parser.h"   /* 你的大 struct 定义 */
}

%define api.value.type { YYSTYPE }   /* 告诉 Bison 用你的类型 */
%define parse.error verbose          /* 例子：顺便开详细报错 */

%{
#include "log.h"    // 日志头文件
#include"parser.h"  // 解析器头文件
using std::string;
extern "C"			
{					
    void yyerror(const char *s);    // 语法错误处理函数
    extern int yylex(void);         // 词法分析函数
    extern int line_count;          // 当前行号
    extern bool new_line_flag;      // 新行标志
    extern int yyleng;              // 当前词的长度
    extern int last_line_count;     // 上一行号
}
extern std::string cur_line_info;       // 当前行信息
extern std::string last_line_info;      // 上一行信息
std::stack<TableSet*> table_set_queue;  // 符号表栈
int _ = (log_set_level(LOG_INFO), 0);   // 日志级别设置
TableSet* top_table_set = new TableSet("main",nullptr);  // 当前符号表

extern int lex_error_flag;      // 词法错误标志
int semantic_error_flag = 0;    // 语义错误标志
int error_flag = 0;             // 语法错误标志
char location_pointer[256];     // 位置指针
void location_pointer_refresh();// 位置指针刷新函数

void yyerror(AST* real_ast, const char *msg);    // 语法错误处理函数
void yynote(std::string msg, int line);          // 语法错误处理函数
void semantic_error(AST* real_ast, std::string msg, int line, int row);  // 语义错误处理函数
%}

%parse-param {AST *real_ast}  // 解析参数
%start program  // 开始符号

// 终结符
%token PROGRAM FUNCTION PROCEDURE TO DOWNTO 
%token ARRAY CONST RECORD
%token IF THEN ELSE OF WHILE DO FOR BEGIN_ END
%token ADDOP NOT PLUS UMINUS CONSTASSIGNOP  
%token<token_info> ID CHAR INT_NUM REAL_NUM BASIC_TYPE RELOP MULOP STRING_ VAR SUBCATALOG
%token<token_info> ASSIGNOP WRITE SEP READ TRUE FALSE ';'

// 非终结符
%type<id_list_node_info> id_list
%type<value_node_info> const_variable num
%type<periods_node_info> periods
%type<period_node_info> period
%type<type_node_info> type
%type<record_node_info> record_body
%type<variable_declaration_node_info> var_declaration
%type<standared_type_node_info> standard_type
%type<formal_parameter_node_info> formal_parameter
%type<parameter_lists_node_info> parameter_lists
%type<parameter_list_node_info> parameter_list
%type<var_parameter_node_info> var_parameter
%type<value_parameter_node_info> value_parameter
%type<variable_node_info> variable
%type<variable_list_node_info> variable_list
%type<expression_node_info> expression
%type<simple_expression_node_info> simple_expression
%type<term_node_info> term
%type<factor_node_info> factor
%type<unsigned_constant_var_node_info> unsigned_const_variable
%type<id_varparts_node_info> id_varparts
%type<id_varpart_node_info> id_varpart
%type<expression_list_node_info> expression_list
%type<program_head_node> program_head
%type<program_body_node> program_body
%type<const_declarations_node> const_declarations
%type<const_declaration_node> const_declaration
%type<variable_declarations_node> var_declarations
%type<subprogram_declarations_node> subprogram_declarations
%type<subprogram_declaration_node> subprogram_declaration
%type<subprogram_head_node> subprogram_head
%type<subprogram_body_node> subprogram_body
%type<compound_statement_node> compound_statement
%type<statement_list_node> statement_list
%type<statement_node> statement
%type<else_node> else_part
%type<updown_node> updown
%type<procedure_call_node> call_procedure_statement

%%

program : 
    program_head program_body '.'
    {   
        // prgram -> program_head program_body '.'
	    ProgramNode* node = new ProgramNode();  // 创建 ProgramNode 节点
        // 添加 program_head 和 program_body 节点到 real_ast 中
        node->append_child($1); // $1 是 Bison 特有语法，引用产生式右侧的第一个符号
        node->append_child($2); // 同理，$2 引用产生式右侧的第二个符号

        if((!error_flag) && (!semantic_error_flag) && (!lex_error_flag)){  // 如果语法、语义和词法没有错误
            real_ast->set_root(node);   // 设置 real_ast 的根节点
            real_ast->set_valid(true);  // 设置 real_ast 的有效性
        }
        delete top_table_set;  // 删除当前符号表
    };
program_head :
    PROGRAM ID '(' id_list ')' ';' {
        // program_head -> PROGRAM ID '(' id_list ')' ';'
    	if(error_flag)
	        break;
        $$ = new ProgramHeadNode();  // 创建 ProgramHeadNode 节点
        LeafNode* leaf_node = new LeafNode($2.value);  // 创建 LeafNode 节点
        $$->append_child(leaf_node);  // 将 LeafNode 节点添加到 ProgramHeadNode 节点中
        table_set_queue.push(top_table_set);  // 将当前符号表添加到符号表栈中
        real_ast->libs()->Preset(table_set_queue.top()->symbols());  // 将当前符号表的符号添加到 real_ast 的符号表中
    } | PROGRAM ID '('  ')' ';' {
        // program_head -> PROGRAM ID '('  ')' ';'
	    if(error_flag)
	        break;
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    } | PROGRAM ID ';' {
        // program_head -> PROGRAM ID ';'
        if(error_flag)
            break;
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    };
program_body :
    const_declarations var_declarations 
    subprogram_declarations compound_statement {
        // program_body -> const_declarations var_declarations subprogram_declarations compound_statement
        if(error_flag)
            break;
        $$ = new ProgramBodyNode();
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
    };
id_list :
    id_list ',' ID { 
        // id_list -> id_list ',' ID
        $1.list_ref->push_back(std::make_pair($3.value.get<string>(),$3.column_num));
        $$.list_ref = $1.list_ref;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        if(error_flag)
            break;
        LeafNode* leaf_node = new LeafNode($3.value);
        $$.id_list_node->append_child($1.id_list_node);
        $$.id_list_node->append_child(leaf_node);
    } | ID {
        // id_list -> ID
        $$.list_ref = new std::vector<std::pair<std::string,int>>();
        $$.list_ref->push_back(std::make_pair($1.value.get<string>(),$1.column_num));
        if(error_flag)
            break;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode($1.value);
        $$.id_list_node->append_child(leaf_node);
    };
const_declarations :{
        // const_declarations -> empty
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode();
    }
    | CONST const_declaration ';'
    {   
        // const_declarations -> CONST const_declaration ';'
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode(); 
        $$->append_child($2);
    };
const_declaration :
    const_declaration ';' ID '=' const_variable
    {
        // const_declaration -> const_declaration ';' ID '=' const_variable
        if(error_flag)
            break;
        if (!$5.is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol($3.value.get<string>(),$5.value,$3.line_num);

        if(!table_set_queue.top()->InsertSymbol($3.value.get<string>(),symbol)){
            string tn = $3.value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",$3.line_num,$3.column_num);
        } else{
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION,$5.type_ptr);
            $$->append_child($1);
            LeafNode* leaf_node = new LeafNode($3.value);
            $$->append_child(leaf_node);
            $$->append_child($5.const_variable_node);
        }
    }
    | ID '=' const_variable
    {   
        // const_declaration -> ID '=' const_variable
        if(error_flag)
            break;
        if (!$3.is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol($1.value.get<string>(),$3.value,$1.line_num);

        if(!table_set_queue.top()->InsertSymbol($1.value.get<string>(),symbol)){
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",$1.line_num,$1.column_num);
        } else {
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE,$3.type_ptr);
            LeafNode* leaf_node = new LeafNode($1.value);
            $$->append_child(leaf_node);
            $$->append_child($3.const_variable_node);
        }
    };
const_variable :
    UMINUS num
    {  
        // const_variable -> UMINUS num
        $$.type_ptr = $2.type_ptr;
        $2.value.set_unimus();
        $$.value = $2.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($2.value);
    }
    | num
    {   
        // const_variable -> num
        $$.type_ptr = $1.type_ptr;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);
    }
    |PLUS num
    {  
        // const_variable -> PLUS num
        $$.type_ptr = $2.type_ptr;
        $$.value = $2.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($2.value);
    }
    | CHAR
    {
        // const_variable -> CHAR
        $$.type_ptr = TYPE_CHAR;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);

    };
num :
    INT_NUM
    {
        // num -> INT_NUM
        $$.type_ptr = TYPE_INT;
        $$.value = $1.value;
    }
    | REAL_NUM
    {   
        // num -> REAL_NUM
        $$.type_ptr = TYPE_REAL;
        $$.value = $1.value;
    };
type :
    standard_type
    {
        // type -> standard_type
        $$.main_type = (TypeAttr::MainType)0;
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->set_base_type_node($$.type_node);
        $$.type_node->append_child($1.standard_type_node);
    }
    | ARRAY '[' periods ']' OF type
    {
        // type -> ARRAY '[' periods ']' OF type
        $$.main_type = (TypeAttr::MainType)1;
        $$.base_type_node = $6.base_type_node;
        $$.bounds = $3.bounds;
        if ($3.bounds){
            auto merged_bounds = new std::vector<ArrayType::ArrayBound>();
            for (auto i : *($3.bounds)){
                merged_bounds->push_back(i);
            }
            auto basic_type = $6.type_ptr;
            if($6.type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
                for (auto i : *($6.bounds)){
                    merged_bounds->push_back(i);
                    $$.bounds->push_back(i);
                }
                basic_type = $6.type_ptr->DynamicCast<ArrayType>()->base_type();
            }
            PtrCollect($6.type_ptr);
            $$.type_ptr = new ArrayType(basic_type, *merged_bounds);
            
            delete merged_bounds;
        }
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        $$.type_node->set_base_type_node($6.base_type_node);
        $$.type_node->append_child($3.periods_node);
        $$.type_node->append_child($6.type_node);
        delete $6.bounds;
        if ($6.record_info){
            delete $6.record_info;
        }
    }
    | RECORD record_body END
    {
        // type -> RECORD record_body END
        $$.main_type = (TypeAttr::MainType)2;
        $$.record_info = $2.record_info;
        if ($2.record_info){
            $$.type_ptr = new RecordType(*($2.record_info));
        } else{
             $$.type_ptr = new RecordType();
        }
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->append_child($2.record_body_node);
        $$.type_node->set_base_type_node($$.type_node);
    };
record_body :
    {
        // record_body -> empty
        $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
    } | var_declaration {
	$$.record_info = $1.record_info;
	if(error_flag)
	    break;
	$$.record_body_node = new RecordBodyNode();
	$$.record_body_node->append_child($1.variable_declaration_node);
	delete $1.pos_info;
    } | var_declaration ';'
    {
        // record_body -> var_declaration ';'
        $$.record_info = $1.record_info;
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
        $$.record_body_node->append_child($1.variable_declaration_node);
        delete $1.pos_info;
    };
standard_type :
    BASIC_TYPE
    {
        // standard_type -> BASIC_TYPE
        string typestr = $1.value.get<string>();
        if (typestr == "integer"){
            $$.type_ptr = TYPE_INT;
        } else if(typestr == "real"){
            $$.type_ptr = TYPE_REAL;
        } else if(typestr == "boolean"){
            $$.type_ptr = TYPE_BOOL;
        } else{
            $$.type_ptr = TYPE_CHAR;
        }
        if(error_flag)
            break;
        $$.standard_type_node = new BasicTypeNode();
        $$.standard_type_node->set_type(dynamic_cast<BasicType*>($$.type_ptr));
    };
periods :
    periods ',' period
    {
        // periods -> periods ',' period
        $$.bounds = $1.bounds;
        $$.bounds->push_back(*($3.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.periods_node);
        $$.periods_node->append_child($3.period_node);
        delete $3.bound;
    }
    | period
    {
        // periods -> period
        $$.bounds = new std::vector<ArrayType::ArrayBound>();
        $$.bounds->push_back(*($1.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.period_node);
        delete $1.bound;
    };
period :
    const_variable SUBCATALOG const_variable
    {     
        // period -> const_variable SUBCATALOG const_variable
        int arr_len=0;
        $$.bound = new ArrayType::ArrayBound();
        if ($1.type_ptr == TYPE_INT&&$3.type_ptr == TYPE_INT){
            arr_len = ($3.value - $1.value).get<int>();
            $$.bound-> type_ = TYPE_INT;
            $$.bound->lb_ = $1.value.get<int>();
            $$.bound->ub_ = $3.value.get<int>();
        } else if($1.type_ptr == TYPE_CHAR&&$3.type_ptr == TYPE_CHAR){
            arr_len = (int)($3.value - $1.value).get<char>();
            $$.bound-> type_ = TYPE_CHAR;
            $$.bound->lb_ = int($1.value.get<int>());
            $$.bound->ub_ = int($3.value.get<int>());
        } else {
            semantic_error(real_ast,"array bound should be integer or char",$2.line_num,$2.column_num);
        }
        if(arr_len < 0){
            arr_len = 0;
            semantic_error(real_ast,"array bound should be positive",$2.line_num,$2.column_num);
        }
        if(error_flag){
            break;
        }
        $$.period_node =new PeriodNode();
        $$.period_node->set_len(arr_len+1);
        $$.period_node->append_child($1.const_variable_node);
        $$.period_node->append_child($3.const_variable_node);
    };
var_declarations : 
    {
        // var_declarations -> empty
        if(error_flag)
            break;
        $$ = new VariableDeclarationsNode();
    }
    | VAR var_declaration ';'
    {
        // var_declarations -> VAR var_declaration ';'
        if(error_flag)
            break;
        for (auto i : *($2.record_info)){
            int line = $2.pos_info->find(i.first)->second.first;
            int row = $2.pos_info->find(i.first)->second.second;
            SymbolTemplate *obj = new SymbolTemplate(i.first, i.second,line);
            if(!table_set_queue.top()->InsertSymbol(i.first,obj)){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line,row);
                yynote(i.first,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.first)->decl_line());
            }
        }
        $$ = new VariableDeclarationsNode();
        $$->append_child($2.variable_declaration_node);
        delete $2.pos_info;
        if ($2.record_info){
            delete $2.record_info;
        }
    };
var_declaration :
    var_declaration ';' id_list ':' type 
    {
        // var_declaration -> var_declaration ';' id_list ':' type
         if(error_flag)
            break;   
        $$.record_info = $1.record_info;
        $$.pos_info = $1.pos_info;
        for (auto i : *($3.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $5.type_ptr));
            $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        $$.variable_declaration_node->append_child($5.type_node);
        delete $3.list_ref;
        if($5.bounds) {
            delete $5.bounds;
        }
        if($5.record_info) {
            delete $5.record_info;
        }
        PtrCollect($5.type_ptr);

    }
    | id_list ':' type 
    {
        // var_declaration -> id_list ':' type
        if(error_flag)
           break;
        $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
        $$.pos_info = new std::unordered_map<std::string, std::pair<int,int>>();
        for (auto i : *($1.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $3.type_ptr));
            $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.id_list_node);
        $$.variable_declaration_node->append_child($3.type_node);
        delete $1.list_ref;
        if($3.bounds) {
            delete $3.bounds;
        }
        if($3.record_info) {
            delete $3.record_info;
        }
        PtrCollect($3.type_ptr);
    };
subprogram_declarations : 
    {
        // subprogram_declarations -> empty
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationsNode();
    }
    | subprogram_declarations subprogram_declaration ';'
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration ';'
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationsNode();
        $$->append_child($1);
        $$->append_child($2);
        TableSet* top = table_set_queue.top();
        table_set_queue.pop();
        delete top;
    };
subprogram_declaration :
    subprogram_head subprogram_body
    {
        // subprogram_declaration -> subprogram_head subprogram_body
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationNode();
        $$->append_child($1);
        $$->append_child($2);
    };
subprogram_body :
    const_declarations var_declarations compound_statement
    {
        // subprogram_body -> const_declarations var_declarations compound_statement
        if(error_flag)
            break;
        $$ = new SubprogramBodyNode();
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
    };
subprogram_head :
    FUNCTION ID formal_parameter ':' standard_type ';'
    {
        // subprogram_head -> FUNCTION ID formal_parameter ':' standard_type ';'
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num);
        }
        if (!table_set_queue.top()->InsertSymbol($2.value.get<string>(), tmp)){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"redefinition of function '"+tn+"'",$2.line_num,$2.column_num);
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet($2.value.get<string>(), table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        string tag = $2.value.get<string>();
        table_set_queue.top()->InsertSymbol(tag, tmp2);
        SymbolTemplate* tmp3 = new SymbolTemplate("internal_"+tag, $5.type_ptr, $2.line_num);
        table_set_queue.top()->InsertSymbol("internal_"+tag, tmp3);
        if ($3.parameters){
            int cnt = 0;
            for (auto i : *($3.parameters)){
                SymbolTemplate *tmp = new SymbolTemplate(i.name,i.type, $2.line_num);
                if (i.mode == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->InsertSymbol(i.name, tmp)){
                    int line = $3.pos_info->at(cnt).first;
                    int row = $3.pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.name +"'",line,row);
                    yynote(i.name,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.name)->decl_line());
                }
                cnt++;
            }
        }
        if(error_flag)
            break;
        $$ = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::FUNCTION);
        $$->set_id($2.value.get<string>());
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        $$->append_child($5.standard_type_node);
        if($3.parameters){
            delete $3.parameters;
        }
        if($3.pos_info){
            delete $3.pos_info;
        }
    }
    | PROCEDURE ID formal_parameter ';'
    {
        // subprogram_head -> PROCEDURE ID formal_parameter ';'
        if(error_flag)
            break;
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num);
        }
        
        if (!table_set_queue.top()->InsertSymbol($2.value.get<string>(), tmp)){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"redefinition of procedure '"+tn+"'",$2.line_num,$2.column_num);
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet($2.value.get<string>(),table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        table_set_queue.top()->InsertSymbol($2.value.get<string>(), tmp2);
        if ($3.parameters){
            int cnt = 0;
            for (auto i : *($3.parameters)){
                SymbolTemplate *tmp = new SymbolTemplate(i.name,i.type, $2.line_num);
                if (i.mode == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->InsertSymbol(i.name, tmp)){
                    int line = $3.pos_info->at(cnt).first;
                    int row = $3.pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.name +"'",line,row);
                    yynote(i.name,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.name)->decl_line());
                }
                cnt++;
            }
        }
        
        $$ = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        delete $3.parameters;
        delete $3.pos_info;
    };
formal_parameter :
    {   
        // formal_parameter -> empty
        $$.parameters = new std::vector<FunctionSymbol::ParameterInfo>();
        $$.pos_info = new std::vector<std::pair<int,int>>();
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();
    }
    | '(' parameter_lists ')'
    {
        // formal_parameter -> '(' parameter_lists ')'
        $$.parameters = $2.parameters;
        $$.pos_info = $2.pos_info;
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();
        $$.formal_parameter_node->append_child($2.param_lists_node);
    };
parameter_lists :
    parameter_lists ';' parameter_list
    {   
        // parameter_lists -> parameter_lists ';' parameter_list
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        $$.parameters->insert($$.parameters->end(), $3.parameters->begin(), $3.parameters->end());
        $$.pos_info->insert($$.pos_info->end(),$3.pos_info->begin(), $3.pos_info->end());
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_lists_node);
        $$.param_lists_node->append_child($3.param_list_node);
    }
    | parameter_list
    {  
        // parameter_lists -> parameter_list
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_list_node);
    };
parameter_list :
    var_parameter
    {   
        // parameter_list -> var_parameter
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.var_parameter_node);
    }
    | value_parameter
    {   
        // parameter_list -> value_parameter
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.value_parameter_node);
    };
var_parameter :
    VAR value_parameter
    {   
        // var_parameter -> VAR value_parameter
        int para_len = $2.parameters->size();
        for (int i = 0; i < para_len; i++){
            $2.parameters->at(i).mode = FunctionSymbol::PARAM_MODE::REFERENCE;
        }
        $$.parameters = $2.parameters;
        $$.pos_info = $2.pos_info;
        if(error_flag)
            break;
        $$.var_parameter_node = new VarParamNode();
        $$.var_parameter_node->append_child($2.value_parameter_node);
    };
value_parameter :
    id_list ':' standard_type
    {   
        // value_parameter -> id_list ':' standard_type
        $$.parameters = new std::vector<FunctionSymbol::ParameterInfo>();
        $$.pos_info = new std::vector<std::pair<int,int>>();
        for(auto id_entry : *$1.list_ref){ // Assuming $1.list_ref is a vector of pairs like std::pair<std::string, int>
            FunctionSymbol::ParameterInfo param_info;
            param_info.name = id_entry.first; // or however the name is accessed from id_entry
            param_info.type = $3.type_ptr;    // Assuming $3.type_ptr is BasicType*
            param_info.mode = FunctionSymbol::PARAM_MODE::VALUE;
            $$.parameters->push_back(param_info);
        }
        
        if(error_flag)
            break;
        $$.value_parameter_node = new ValueParamNode();
        $$.value_parameter_node->append_child($1.id_list_node);
        $$.value_parameter_node->append_child($3.standard_type_node);
        delete $1.list_ref;
    };
compound_statement :
    BEGIN_ statement_list END 
    {
        // compound_statement -> BEGIN_ statement_list END
        if(error_flag)
            break;
        $$ = new CompoundStatementNode();
        $$->append_child($2);
    };
statement_list :
    statement_list ';' statement
    {
        // statement_list -> statement_list ';' statement
        if(error_flag)
            break;
        $$ = new StatementListNode();
        $$->append_child($1);
        $$->append_child($3);
    } | statement
    {
        // statement_list -> statement
        if(error_flag)
            break;
        $$ = new StatementListNode();
        $$->append_child($1);
    };
statement:
    variable ASSIGNOP expression
    {   
        //statement -> variable ASSIGNOP expression
        if(error_flag)
            break;
        bool var_flag = ($1.type_ptr==TYPE_REAL && $3.type_ptr==TYPE_INT) || ($1.type_ptr && $1.type_ptr->isSameAs($3.type_ptr));
        bool str_flag = ($1.type_ptr != TYPE_ERROR &&
        		 $1.type_ptr->StringLike() &&
        		 $3.type_ptr==TYPE_STRINGLIKE);
        if(!var_flag && !str_flag){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
            break;
        }
        std::string func_name = table_set_queue.top()->tag();

        if(func_name == *$1.name){
            $$ = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            $$ = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
            if (!$1.is_lvalue){
                semantic_error(real_ast,"lvalue required as left operand of assignment",$2.line_num,$2.column_num);
            }
        }
        if(error_flag)
            break;
        $$->append_child($1.variable_node);
        $$->append_child($3.expression_node);
        delete $1.name;
    }
    | call_procedure_statement
    {
        // statement -> call_procedure_statement
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        $$->append_child($1);
    }
    | compound_statement
    {
        // statement -> compound_statement
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        $$->append_child($1);
    }
    | IF expression THEN statement else_part
    {   
        // statement -> IF expression THEN statement else_part
        if(error_flag)
            break;
        //类型检查
        if(!($2.type_ptr && $2.type_ptr->isSameAs(TYPE_BOOL))){
            string tn = type_name($2.type_ptr);
            semantic_error(real_ast,"IF quantity cannot be '"+tn+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);
        $$->append_child($5);
    }
    | WHILE expression DO statement
    {
        // statement -> WHILE expression DO statement
        if(error_flag)
            break;
        if(!($2.type_ptr && $2.type_ptr->isSameAs(TYPE_BOOL))){
            string tn = type_name($2.type_ptr);
            semantic_error(real_ast,"WHILE quantity cannot be '"+tn+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);

    } 
    | FOR ID ASSIGNOP expression updown expression DO statement
    {
        // statement -> FOR ID ASSIGNOP expression updown expression DO statement
        if(error_flag)
            break;
        //类型检查
        SymbolTemplate *tmp = table_set_queue.top()->SearchEntry<SymbolTemplate>($2.value.get<string>());
        if(tmp==nullptr){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared",$2.line_num,$2.column_num);
            break;
        }
        if((!(tmp->type() && tmp->type()->isSpecificBasicType()))||(!(tmp->type() && tmp->type()->isSameAs($4.type_ptr)))){
            string tn1 = type_name(tmp->type());
            string tn2 = type_name($4.type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
        }

        if((!($4.type_ptr && $4.type_ptr->isSameAs($6.type_ptr)))||($4.type_ptr && $4.type_ptr->isSameAs(TYPE_REAL))){
            string tn1 = type_name($4.type_ptr);
            string tn2 = type_name($6.type_ptr);
            semantic_error(real_ast,"invalid updown type from '"+tn1+"' to '"+tn2+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::FOR_STATEMENT);
        LeafNode *id_node = new LeafNode($2.value);
        $$->append_child(id_node);
        $$->append_child($4.expression_node);
        $$->append_child($5);
        $$->append_child($6.expression_node);
        $$->append_child($8);
    }
    | 
    {
        // statement -> empty
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::EPSILON);
    }
    |READ '(' variable_list ')'
    {
        // statement -> READ '(' variable_list ')'
        if(error_flag)
            break;
        if(!$3.variable_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in READ",$1.line_num,$1.column_num);
        }  
        $$ = new StatementNode(StatementNode::GrammarType::READ_STATEMENT);
        $$->append_child($3.variable_list_node);
        delete $3.type_ptr_list;
    }
    |WRITE '(' expression_list ')'
    {
        // statement -> WRITE '(' expression_list ')'
        if(error_flag)
            break;
        if(!$3.expression_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in WRITE",$1.line_num,$1.column_num);
        }
        
        $$ = new StatementNode(StatementNode::GrammarType::WRITE_STATEMENT);
        $$->append_child($3.expression_list_node);
        delete $3.type_ptr_list;
        delete $3.is_lvalue_list;
    };

variable_list :
    variable
    { 
        // variable_list -> variable
        $$.type_ptr_list = new std::vector<TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE);
        $$.variable_list_node->append_child($1.variable_node);
        if($1.name) delete $1.name;
    } | variable_list ',' variable{
        // variable_list -> variable_list ',' variable
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE_LIST_VARIABLE);
        $$.variable_list_node->append_child($1.variable_list_node);
        $$.variable_list_node->append_child($3.variable_node);
        if($3.name) delete $3.name;
    };
variable:
    ID '('')'
    {
        // variable -> ID '('')'
        if (error_flag) break;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        $$.is_lvalue = false;
        if(tmp == nullptr){
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",$1.line_num,$1.column_num);
            break;
        }
        if(tmp->type() != nullptr && tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION){
            if(!tmp->AssertHasNoParams()){
                string tn = $1.value.get<string>();
                string param = tmp->ParamName();
	        semantic_error(real_ast,"too few arguments to function '"+tn+"' (expected '("+param+")')",$1.line_num,$1.column_num);
                break;
            }
            $$.type_ptr = tmp->type();
            $$.name = new std::string($1.value.get<string>());
            real_ast->libs()->Call(*($$.name));
            string name = $1.value.get<string>()+"()";
            $$.variable_node = new VariableNode();
            LeafNode *id_node = new LeafNode(name);
            $$.variable_node->append_child(id_node);
        } else {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",$1.line_num,$1.column_num);
        }
         
    }
    | ID id_varparts
    {
        // variable -> ID id_varparts
        if(error_flag)
            break;
        SymbolTemplate *tmp = table_set_queue.top()->SearchEntry<SymbolTemplate>($1.value.get<string>());
        string name = $1.value.get<string>();
        $$.name = new std::string($1.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        if(tmp == nullptr) {
            semantic_error(real_ast,"'"+name+ "' undeclared",$1.line_num,$1.column_num);
            break;
        } else {
            //类型检查
            $$.is_lvalue = true;
            if (SymbolTemplate::SYMBOL_TYPE::CONST == tmp->symbol_type()){
                tmp = dynamic_cast<ConstSymbol*>(tmp);
                $$.is_lvalue = false;
            } else if(SymbolTemplate::SYMBOL_TYPE::FUNCTION == tmp->symbol_type()){
                //函数调用 类型检查
                if (name!=table_set_queue.top()->tag()){
                    if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertHasNoParams()){
                        string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
                        semantic_error(real_ast,"too few arguments to function '"+name+"' (expected '("+param+")')",line_count,0);
                    }else{
                    	name += "()";
                    }
                }else{
                    name="internal_"+name;
                }
                $$.is_lvalue = false;
                real_ast->libs()->Call(tmp->name());
            } else {
                if (tmp->type()->template_type() == TypeTemplate::TYPE::ARRAY && !error_flag){
                    std::vector<ArrayType::ArrayBound> bounds = dynamic_cast<ArrayType*>(tmp->type())->bounds();
                    $2.id_varparts_node->set_lb(bounds);
                }
            }
            $$.type_ptr = $2.AccessCheck(tmp->type());
            if($$.type_ptr==nullptr){
                string tn1 = type_name(tmp->type());
                if(tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION ||
                   tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::CONST ||
                   tmp->type() == TYPE_ERROR ||
                   tmp->type()->template_type() == TypeTemplate::TYPE::BASIC){
                    if($2.var_parts != nullptr && $2.var_parts->size() != 0){
                    	semantic_error(real_ast,"requested value is nither array nor record (have '"+tn1+"')",line_count,0);
                    }
                }else{
                    string tn2;
                    std::vector<VarParts>* vp = $2.var_parts;
                    if(vp == nullptr) tn2 += "error";
                    else{
                        for(int i=0;i<vp->size();i++){
                            if((*vp)[i].flag == 0){
                                tn2 += "[" + type_name(*((*vp)[i].subscript)) + "]";
                            }else{
                            	tn2 += (*vp)[i].name;
                            }
			    if(i != vp->size() - 1) tn2 += ",";
		        }
                    }
                    semantic_error(real_ast,"invalid request from '"+tn1+"' with '"+tn2+"'",line_count,0);
                }
            }
            if(tmp->is_ref()){
                name = "*("+name+")";
            }
        }

        $$.variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode(name);
        $$.variable_node->append_child(id_node);
        $$.variable_node->append_child($2.id_varparts_node);
        for (auto i : *($2.var_parts)){
            delete i.subscript;
        }
        delete $2.var_parts;
    };

id_varparts:
    {
        // id_varparts -> empty.
        if(error_flag)
            break;
        $$.var_parts = new std::vector<VarParts>();
        if(error_flag)
            break;
        $$.id_varparts_node = new IDVarPartsNode();
    }
    | id_varparts id_varpart
    {
        // id_varparts -> id_varparts id_varpart.
        if(error_flag)
            break;
        if($1.var_parts){
            $$.var_parts = $1.var_parts;
        } else {
            $$.var_parts = new std::vector<VarParts>();
        }
        
        $$.var_parts->push_back(*($2.var_part));

        $$.id_varparts_node = new IDVarPartsNode();
        $$.id_varparts_node->append_child($1.id_varparts_node);
        $$.id_varparts_node->append_child($2.id_varpart_node);
        delete $2.var_part;
    };

id_varpart:
    '[' expression_list ']'
    {   
        // id_varpart -> [expression_list].
        $$.var_part= new VarParts();
        $$.var_part->flag = 0;//数组
        $$.var_part->subscript = $2.type_ptr_list;
        if(error_flag)
            break;
        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::EXP_LIST);
        $$.id_varpart_node->append_child($2.expression_list_node);
        delete $2.is_lvalue_list;
    }
    | '.' ID
    {
        // id_varpart -> .id.
        $$.var_part= new VarParts();
        $$.var_part->flag = 1;//结构体
        $$.var_part->name = $2.value.get<string>();
        if(error_flag)
            break;
        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::_ID);
        LeafNode *id_node = new LeafNode($2.value);
        $$.id_varpart_node->append_child(id_node);
    };
else_part:
    {
        // else_part -> empty.
        if(error_flag)
            break;
        $$ = new ElseNode(ElseNode::GrammarType::EPSILON);
    }
    | ELSE statement
    {
        // else_part -> ELSE statement.
        if(error_flag)
            break;
        $$ = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        $$->append_child($2);
    } ;
updown:
    TO
    {
        // updown -> TO.
        if(error_flag)
            break;
        $$ = new UpdownNode(true);
    }
    | DOWNTO
    {
        // updown -> DOWNTO.
        if(error_flag)
            break;
        $$ = new UpdownNode(false);
    };
call_procedure_statement:
    ID '(' expression_list ')'
    {
        // call_procedure_statement -> id (expression_list).
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        }
        if(!tmp || !tmp->AssertParamsMatch(*($3.type_ptr_list),*($3.is_lvalue_list))){
            string tn = $1.value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*($3.type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        $$->append_child($3.expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        $3.expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete $3.is_lvalue_list;
        delete $3.type_ptr_list;
    }
    | ID
    {   
        // call_procedure_statement -> id.
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        } else {
            //函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertHasNoParams()){
                string tn = $1.value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too few arguments to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
            }
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    }
    | ID '(' ')'
    {
        // call_procedure_statement -> id().
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        } else {
            // 函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertHasNoParams()){
                string tn = $1.value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too many arguments to procedure '"+tn+"' (expected '("+param+")')",$1.line_num,0);
            }
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    };
expression_list:
    expression_list ',' expression
    {
        // expression_list -> expression_list ',' expression
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        $$.is_lvalue_list = $1.is_lvalue_list;
        $$.is_lvalue_list->push_back($3.is_lvalue);
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        $$.expression_list_node->append_child($1.expression_list_node);
        $$.expression_list_node->append_child($3.expression_node);
    }
    | expression
    {
        // expression_list -> expression
        $$.type_ptr_list = new std::vector<TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        $$.is_lvalue_list = new std::vector<bool>();
        $$.is_lvalue_list->push_back($1.is_lvalue);
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        $$.expression_list_node->append_child($1.expression_node);
    };
expression:
    simple_expression RELOP simple_expression
    {
        // expression -> simple_expression RELOP simple_expression.
        if(error_flag)
            break;
        // 类型检查
        //从这里开始进行运算检查
        if((!($1.type_ptr && $1.type_ptr->isSpecificBasicType()))||(!($3.type_ptr && $3.type_ptr->isSpecificBasicType())))
        {
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
    auto result=($1.type_ptr && $1.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($1.type_ptr)->computeWith(static_cast<BasicType*>($3.type_ptr), $2.value.get<string>()) : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
	    string tn2 = type_name($3.type_ptr);
	    string tn3 = $2.value.get<string>();
	    semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        
        std::string relop = $2.value.get<string>();
        if($2.value.get<string>() == "<>") {
            relop = "!=";
        }
        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue(relop));
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
    }
    | simple_expression '=' simple_expression
    {
        // expression -> simple_expression '=' simple_expression.
        // 类型检查
        if((!($1.type_ptr && $1.type_ptr->isSpecificBasicType()))||(!($3.type_ptr && $3.type_ptr->isSpecificBasicType()))){
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=($1.type_ptr && $1.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($1.type_ptr)->computeWith(static_cast<BasicType*>($3.type_ptr), "=") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue("=="));
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
    }
    | simple_expression
    {
        // expression -> simple_expression.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        if($$.type_ptr && $$.type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
            $$.expression_node = new ExpressionNode(ExpressionNode::TargetType::VAR_ARRAY);
        } else {
            $$.expression_node = new ExpressionNode();
        }
        
        $$.expression_node->append_child($1.simple_expression_node);
    };
simple_expression:
    simple_expression ADDOP term
    {
        // simple_expression -> simple_expression or term.
        //类型检查
        if($1.type_ptr!=$3.type_ptr){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary or (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = $1.type_ptr;

        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *addop_node = new LeafNode(ConstValue("||"));
        $$.simple_expression_node->append_child(addop_node);
        $$.simple_expression_node->append_child($3.term_node);
    }
    | simple_expression PLUS term
    { 
        // 类型检查
        // simple_expression -> simple_expression + term.
        $$.is_lvalue = false;
        if(error_flag)
            break;
        if((!($1.type_ptr && $1.type_ptr->isSpecificBasicType()))||(!($3.type_ptr && $3.type_ptr->isSpecificBasicType())))
        {
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
           $$.type_ptr = $1.type_ptr;
        }
        else{
            auto result=($1.type_ptr && $1.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($1.type_ptr)->computeWith(static_cast<BasicType*>($3.type_ptr), "+") : TYPE_ERROR);
            if(result==TYPE_ERROR){
                 string tn1 = type_name($1.type_ptr);
                 string tn2 = type_name($3.type_ptr);
                 semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
            }
            $$.type_ptr = result;
        }
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($3.term_node);
    }
    | simple_expression UMINUS term
    {
        $$.is_lvalue = false;
        if(error_flag)
            break;
        // 类型检查
        // simple_expression -> simple_expression - term.
         if((!($1.type_ptr && $1.type_ptr->isSpecificBasicType()))||(!($3.type_ptr && $3.type_ptr->isSpecificBasicType())))
         {
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
auto result=($1.type_ptr && $1.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($1.type_ptr)->computeWith(static_cast<BasicType*>($3.type_ptr), "-") : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.type_ptr = result;

        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($3.term_node);
    }
    |term
    {   
        // simple_expression -> term.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.term_node);
    }
    |PLUS term
    {
        // simple_expression -> + term.
        //类型检查
        if(!($2.type_ptr && $2.type_ptr->isSpecificBasicType())){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }

       auto result=($2.type_ptr && $2.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($2.type_ptr)->computeUnary("-") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($2.term_node);
    }
    |UMINUS term
    {
        // simple_expression -> - term.
        //类型检查
        if(!($2.type_ptr && $2.type_ptr->isSpecificBasicType()))
        {
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        auto result=($2.type_ptr && $2.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($2.type_ptr)->computeUnary("-") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($2.term_node);
    };
term:
    factor
    {   
        // term -> factor.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.factor_node);
    }
    | term MULOP factor
    {  
        // term -> term mulop factor.
        // 类型检查
        if((!($1.type_ptr && $1.type_ptr->isSpecificBasicType()))||(!($3.type_ptr && $3.type_ptr->isSpecificBasicType())))
        {
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=($1.type_ptr && $1.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($1.type_ptr)->computeWith(static_cast<BasicType*>($3.type_ptr), $2.value.get<string>()) : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        
        std::string mulop = $2.value.get<string>();
        if(mulop == "/" && !error_flag){
            $1.term_node->set_op_div(true);
        } else if(mulop == "div"){
            mulop = "/";
        } else if (mulop == "mod"){
            mulop = "%";
        } else if (mulop == "and"){
            mulop = "&&";
        }
        if(error_flag)
            break;
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.term_node);
        LeafNode *mulop_node = new LeafNode(ConstValue(mulop));
        $$.term_node->append_child(mulop_node);
        $$.term_node->append_child($3.factor_node);
    };
factor:
    unsigned_const_variable
    {   
        // factor -> unsigned_const_variable.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::UCONST_VAR);
        $$.factor_node->append_child($1.unsigned_constant_var_node);
    }
    | variable
    {   
        // factor -> variable.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::VARIABLE);
        $$.factor_node->append_child($1.variable_node);
        if($1.name) delete $1.name;
    }
    |ID '(' expression_list ')'
    {
        if(error_flag)
            break;
        $$.is_lvalue = false;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            semantic_error(real_ast,"undefined function '" + $1.value.get<string>() + "'",$1.line_num,$1.column_num);
            break;
        }else if(!tmp->AssertParamsMatch(*($3.type_ptr_list),*($3.is_lvalue_list))){
            string tn = $1.value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*($3.type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to function '"+tn+"' (expected '("+param+")')",line_count,0);
            break;
        }
        //if(error_flag)
        //   break;
        $$.type_ptr = tmp->type();
        $$.factor_node = new FactorNode(FactorNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value);
        $$.factor_node->append_child(id_node);
        $$.factor_node->append_child($3.expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        $3.expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete $3.type_ptr_list;
        delete $3.is_lvalue_list;

    }
    | '(' expression ')'
    {
        // factor -> (expression).
        $$.type_ptr = $2.type_ptr;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::EXP);
        $$.factor_node->append_child($2.expression_node);
    }
    | NOT factor
    {   
        // factor -> not factor.
        // 类型检查
        if(!($2.type_ptr && $2.type_ptr->isSpecificBasicType()))
        {
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
auto result=($2.type_ptr && $2.type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>($2.type_ptr)->computeUnary("not") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        if(result==TYPE_BOOL){
            $$.factor_node = new FactorNode(FactorNode::GrammarType::NOT_BOOL);
        }
         if(result==TYPE_INT){
            $$.factor_node = new FactorNode(FactorNode::GrammarType::NOT_INT);
        }
        $$.factor_node->append_child($2.factor_node);
    };
unsigned_const_variable :
    num
    {
        // unsigned_const_variable -> num
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        LeafNode *num_node = new LeafNode($1.value);
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        $$.unsigned_constant_var_node->append_child(num_node);
    }
    | CHAR
    {
        // unsigned_const_variable -> 'LETTER'
        $$.type_ptr = TYPE_CHAR;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode($1.value);
        $$.unsigned_constant_var_node->append_child(char_node);
    }
    |TRUE
    {
        // unsigned_const_variable -> true
        $$.type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(ConstValue(true));
        $$.unsigned_constant_var_node->append_child(true_node);
    }
    | FALSE
    {   
        // unsigned_const_variable -> false
        $$.type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(ConstValue(false));
        $$.unsigned_constant_var_node->append_child(false_node);
    };

// 错误处理机制
program: 
    program_head program_body error
    {
        // program -> program_head program_body
        // 缺少程序末尾的 '.'
        location_pointer_refresh();
        yyerror(real_ast,"expected '.' at the end of the program");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };
program_head:
    PROGRAM ID '(' error ')' ';'
    {
        // program -> PROGRAM ID ( error ) ;
        // program 声明后的参数格式错误
        // 例如：program main(555);
        location_pointer_refresh();
        yyerror(real_ast,"invalid argument expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        char *new_location_pointer = location_pointer + 2;
        fprintf(stderr,"\t| %s",new_location_pointer);
    };
const_declaration: 
    ID const_variable
    {
        // const_declaration -> ID const_variable
        // const 表达式缺少 '='
        // 例如：const a 5;
        location_pointer_refresh();
        yyerror(real_ast, "expected '=' between identifier and value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    | const_declaration ';' ID const_variable
    {
        // const_declaration -> const_declaration ; ID const_variable
        // 缺少 '='
        location_pointer_refresh();
        yyerror(real_ast, "expected '=' between identifier and value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    | ID '=' error
    {
        // const_declaration -> ID = error
        // 表达式右侧无效
        // 例如：const a = c;
        location_pointer_refresh();
        yyerror(real_ast, "invalid constant expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };
statement_list:
    variable error
    {
        location_pointer_refresh();
        yyerror(real_ast, "expected ':=' between variable and expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);        
    };
%%

void yyerror(AST* real_ast,const char *msg){
    if(yydebug || strcmp(msg,"syntax error")!=0)   // 当非debug模式且传入的是默认报错时不输出 
        fprintf(stderr,"%d,%ld:\033[01;31m \terror\033[0m : %s\n", line_count,cur_line_info.size(),msg);   
    error_flag = 1;
    real_ast->set_root(nullptr);
}
void yynote(std::string msg ,int line){
    fprintf(stderr,"%d:\033[01;32m \tnote\033[0m : previous definition of \"%s\" was here\n", line, msg.c_str());
}
void semantic_error(AST* real_ast,std::string msg,int line,int row){
    semantic_error_flag=1;
    real_ast->set_root(nullptr); 
    if (row)
        fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", line,row,msg.c_str());
    else
        fprintf(stderr,"%d:\033[01;31m \terror\033[0m : %s\n", line,msg.c_str());
}
void location_pointer_refresh(){
    int length = cur_line_info.size()-yyleng;
    if(length<0)
        length=0;
    memset(location_pointer,' ',length);
    memcpy(location_pointer+length,"^\n\0",3);
}
int yywrap(){
    return 1;
}
