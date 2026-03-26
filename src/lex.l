%option case-insensitive
%{   
#include "parser.h"
#include "log.h"
#include "parser.tab.h"
    
int line_count = 1;             /* 当前行号 */
int char_count = 1;             /* 当前列号 */
int last_line_count;            /* 当前行的前一个非空行号 */
std::string cur_line_info;      /* 当前行已读入信息 */
std::string last_line_info;     /* 当前行的前一个非空行信息 */
int lex_error_flag = 0;         /* 标志词法分析过程中是否出现错误 */

void wrap();
void process_error_text();
void process_anno();
void cache_line();
void print_lex_error(const char *msg);

void process_symbol();
void process_identifier();
void process_string_literal();
void process_integer();
void process_float();
void process_char();
void process_boolean();

extern "C"
{
    int yywrap();
    int yylex();
    void yyerror(const char *s);
}

%}

delim                   [ ]
ws                      {delim}+
letter                  [a-zA-Z]
digit                   [0-9]
digits                  {digit}+
fraction                (\.{digits}([eE][+-]?{digits})?)

/* 关键字 */
PROGRAM                 "program"
CONST                   "const"
RECORD                  "record"
ARRAY                   "array"
OF                      "of"
VAR                     "var"
FUNCTION                "function"
PROCEDURE               "procedure"
BEGIN_                  "begin"
END                     "end"
IF                      "if"
THEN                    "then"
ELSE                    "else"
WHILE                   "while"
FOR                     "for"
TO                      "to"
DOWNTO                  "downto"
DO                      "do"
READ                    "read"
WRITE                   "write"
NOT                     "not"
BASIC_TYPE              ("integer"|"real"|"boolean"|"char")
TRUE                    "true"
FALSE                   "false"


/* 常数 */
INT_NUM                 {digits}
REAL_NUM                {digits}{fraction}

/* 注释 */
ANNO_1                  (\{[^}]*\})      
ANNO_2                  ("//"[^\n]*)

/* 标识符 */
ID                      ({letter})({letter}|{digit}|[_])*

/* 字符 */
CHAR                    (\'([^\\\'\n]|(\\.))\')

/*字符串*/
STRING_                 (\'([^\\\'\n]|(\\.)){2,}\')


/* 运算符 */
PLUS                    "+"
UMINUS                  "-"
CONSTASSIGNOP           "="
RELOP                   ("<>"|"<"|"<="|">"|">=")
ADDOP                   "or"
MULOP                   ("*"|"/"|"div"|"mod"|"and")
ASSIGNOP                ":="

/* 分隔符 */
SEP                     (";"|"."|","|":")
SUBCATALOG              ".."

%% 
"\t"                    { cache_line(); char_count += 4; }
"\r"                    {  }
"\n"                    { wrap(); }
{ws}                    { cache_line(); char_count += yyleng; }

{PROGRAM}               { cache_line(); process_identifier(); return PROGRAM; }
{CONST}                 { cache_line(); process_identifier(); return CONST; }

{ARRAY}                 { cache_line(); process_identifier(); return ARRAY; }
{RECORD}                { cache_line(); process_identifier(); return RECORD;}
{OF}                    { cache_line(); process_identifier(); return OF; }
{VAR}                   { cache_line(); process_identifier(); return VAR; }
{FUNCTION}              { cache_line(); process_identifier(); return FUNCTION; }
{PROCEDURE}             { cache_line(); process_identifier(); return PROCEDURE; }
{BEGIN_}                { cache_line(); process_identifier(); return BEGIN_; }
{END}                   { cache_line(); process_identifier(); return END; }
{IF}                    { cache_line(); process_identifier(); return IF; }
{THEN}                  { cache_line(); process_identifier(); return THEN; }
{ELSE}                  { cache_line(); process_identifier(); return ELSE; }
{WHILE}                 { cache_line(); process_identifier(); return WHILE; }
{FOR}                   { cache_line(); process_identifier(); return FOR; }
{TO}                    { cache_line(); process_identifier(); return TO; }
{DOWNTO}                { cache_line(); process_identifier(); return DOWNTO; }
{DO}                    { cache_line(); process_identifier(); return DO; }
{WRITE}                 { cache_line(); process_identifier(); return WRITE;}
{READ}                  { cache_line(); process_identifier(); return READ;}

{NOT}                   { cache_line(); process_identifier(); return NOT; }
{BASIC_TYPE}            { cache_line(); process_identifier(); return BASIC_TYPE; }
{TRUE}                  { cache_line(); process_boolean(); return TRUE; }
{FALSE}                 { cache_line(); process_boolean(); return FALSE; }
{PLUS}                  { cache_line(); process_symbol(); return PLUS; }
{UMINUS}                { cache_line(); process_symbol(); return UMINUS; }

{INT_NUM}               { cache_line(); process_integer(); return INT_NUM; }
{REAL_NUM}              { cache_line(); process_float(); return REAL_NUM; }

{ANNO_1}                { process_anno(); }
{ANNO_2}                { process_anno(); }

{CHAR}                  { cache_line(); process_char(); return CHAR; }
{STRING_}               { cache_line(); process_string_literal(); return STRING_; }

{RELOP}                 { cache_line(); process_symbol(); return RELOP; }
{ADDOP}                 { cache_line(); process_symbol(); return ADDOP; }
{MULOP}                 { cache_line(); process_symbol(); return MULOP; }
{ASSIGNOP}              { cache_line(); process_symbol(); return ASSIGNOP; }

{ID}                    { cache_line(); process_identifier(); return ID; }

{CONSTASSIGNOP}         { cache_line(); process_symbol(); return yytext[0]; }
{SEP}                   { cache_line(); process_symbol(); return yytext[0]; }
{SUBCATALOG}            { cache_line(); process_symbol(); return SUBCATALOG; }

("[")                   { cache_line(); process_symbol(); return yytext[0];} /* 处理左中括号 */
("(")                   { cache_line(); process_symbol(); return yytext[0];} /* 处理左小括号 */
("]")                   { cache_line(); process_symbol(); return yytext[0];} /* 处理右中括号 */
(")")                   { cache_line(); process_symbol(); return yytext[0];} /* 处理右小括号 */




('')                        { cache_line(); print_lex_error("错误:字符为空"); } /* 空字符报错 */

('\n')                      { cache_line(); print_lex_error("错误:字符中换行"); process_error_text(); } /* 字符换行错误 */

('[^']*(\n)[^']*[^'\n]')    { cache_line(); print_lex_error("错误:字符串中换行"); process_error_text(); } /* 字符串中包含换行符报错 */
              
(\{[^}]*)                   { cache_line(); print_lex_error("错误:{}未闭合"); process_error_text(); } /* 未闭合的注释报错{这是一个未闭合的注释 */

('[^']*)                    { cache_line(); print_lex_error("错误:''未闭合"); process_error_text(); } /* 未闭合的字符串报错'这是一个未闭合的字符串 */

.                           { cache_line(); print_lex_error("illegal character"); char_count++; } /* "兜底"规则，用于处理所有未被其他规则匹配的字符 */



%%
void wrap()/* 处理换行的函数 */
{
    if(cur_line_info.size() != 0){ /* 如果当前行不为空 */
        last_line_info = cur_line_info;/* 将当前行信息保存到last_line_info */
        cur_line_info.clear();/* 清空当前行信息 */
        last_line_count = line_count;/* 将当前行号保存到last_line_count */
    }
    line_count++; /* 行号加1 */
    char_count = 1; /* 列号重置为1 */
}

void process_error_text()/* 处理错误（\n）情况下的文本内容的函数 */
{
    for(int i = 0; i < strlen(yytext); i++)/* 遍历当前行的文本内容 */
    {
        if(yytext[i] == '\n') wrap();/* 如果当前行的文本内容中包含换行符，则调用wrap()函数 */
        else char_count++;/* 否则，列号加1 */
    }  
}

void process_anno()/* 处理注释的函数，记录注释信息到日志 */
{
    log_debug("ANNO\t\t%s\t\t\t%d\t\t%d", yytext, line_count, char_count);/* 记录注释信息到日志 */
    for(int i = 0; i < strlen(yytext); i++)/* 遍历当前行的文本内容 */
    {
        if(yytext[i] == '\n') wrap();/* 如果当前行的文本内容中包含换行符，则调用wrap()函数 */
        else char_count++;/* 否则，列号加1 */
    }
}

void cache_line()/* 用来缓存当前行信息的函数，用于调试 */
{
    cur_line_info += yytext;/* 将当前行的文本内容保存到cur_line_info */
    if(yydebug){/* 如果开启了调试模式，则打印当前行的文本内容 */
        std::cout<<"cur_line_info is "<<cur_line_info<<std::endl;/* 打印当前行的文本内容 */
    }
}

void print_lex_error(const char *msg)/* 用来打印词法错误信息的函数 */
{
    lex_error_flag = 1;/* 设置词法错误标志 */
    fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s - %s\n",        /* %d,%d: - 行号,列号 \033[01;31m - 红色加粗文本（错误信息）\terror - 错误标记 \033[0m - 重置文本格式 : %s - %s - 错误信息和导致错误的文本 */
        line_count,/* 行号 */
        char_count,/* 列号 */
        msg,/* 错误信息 */
        yytext/* 当前行的文本内容 */
    );
}

void process_symbol() {
    std::string str = std::string(yytext); /*将yytext转换为字符串*/
    if (isalpha(str[0])) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower); /*将字符串转换为小写*/
    }
    yylval.token_info.value.set(str); /*将字符串设置为token的值*/
    yylval.token_info.line_num = line_count; /*设置token的行号*/
    yylval.token_info.column_num = char_count; /*设置token的列号*/
    yylval.token_info.length = yyleng; /*设置token的长度*/

    char_count += yyleng; /*更新列号*/
}

void process_identifier()/* 用于处理关键字、标识符等 */
{
    std::string str = std::string(yytext);/* 将yytext转换为字符串 */
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);/* 将字符串转换为小写 */
    yylval.token_info.value.set(str);/* 将字符串设置为token的值 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

void process_string_literal()/* 用于处理字符串常量 */
{
    std::string str = std::string(yytext);/* 将yytext转换为字符串 */
    str = str.substr(1, str.length() - 2);  /* 去掉字符串两端的单引号 */
    yylval.token_info.value.set(str);/* 将字符串设置为token的值 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

void process_integer()/* 用于处理整数常量 */
{
    yylval.token_info.value.set(std::stoi(yytext));/* 将字符串转换为整数 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

void process_float()/* 用于处理浮点数常量 */
{
    yylval.token_info.value.set(std::stof(yytext));/* 将字符串转换为浮点数 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

void process_char()/* 用于处理字符常量 */
{
    yylval.token_info.value.set(yytext[1]); /* 将字符串转换为字符 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

void process_boolean()/* 用于处理布尔常量 */
{
    yylval.token_info.value.set(yytext[0] == 'T');/* 将字符串转换为布尔值 */
    yylval.token_info.line_num = line_count;/* 设置token的行号 */
    yylval.token_info.column_num = char_count;/* 设置token的列号 */
    yylval.token_info.length = yyleng;/* 设置token的长度 */

    char_count += yyleng;/* 更新列号 */
}

int yywrap()
{
    return 1;
}
