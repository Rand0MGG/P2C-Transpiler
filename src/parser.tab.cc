/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 8 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"

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

#line 101 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROGRAM = 3,                    /* PROGRAM  */
  YYSYMBOL_FUNCTION = 4,                   /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 5,                  /* PROCEDURE  */
  YYSYMBOL_TO = 6,                         /* TO  */
  YYSYMBOL_DOWNTO = 7,                     /* DOWNTO  */
  YYSYMBOL_ARRAY = 8,                      /* ARRAY  */
  YYSYMBOL_CONST = 9,                      /* CONST  */
  YYSYMBOL_RECORD = 10,                    /* RECORD  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_THEN = 12,                      /* THEN  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_OF = 14,                        /* OF  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_DO = 16,                        /* DO  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_BEGIN_ = 18,                    /* BEGIN_  */
  YYSYMBOL_END = 19,                       /* END  */
  YYSYMBOL_ADDOP = 20,                     /* ADDOP  */
  YYSYMBOL_NOT = 21,                       /* NOT  */
  YYSYMBOL_PLUS = 22,                      /* PLUS  */
  YYSYMBOL_UMINUS = 23,                    /* UMINUS  */
  YYSYMBOL_CONSTASSIGNOP = 24,             /* CONSTASSIGNOP  */
  YYSYMBOL_ID = 25,                        /* ID  */
  YYSYMBOL_CHAR = 26,                      /* CHAR  */
  YYSYMBOL_INT_NUM = 27,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 28,                  /* REAL_NUM  */
  YYSYMBOL_BASIC_TYPE = 29,                /* BASIC_TYPE  */
  YYSYMBOL_RELOP = 30,                     /* RELOP  */
  YYSYMBOL_MULOP = 31,                     /* MULOP  */
  YYSYMBOL_STRING_ = 32,                   /* STRING_  */
  YYSYMBOL_VAR = 33,                       /* VAR  */
  YYSYMBOL_SUBCATALOG = 34,                /* SUBCATALOG  */
  YYSYMBOL_ASSIGNOP = 35,                  /* ASSIGNOP  */
  YYSYMBOL_WRITE = 36,                     /* WRITE  */
  YYSYMBOL_SEP = 37,                       /* SEP  */
  YYSYMBOL_READ = 38,                      /* READ  */
  YYSYMBOL_TRUE = 39,                      /* TRUE  */
  YYSYMBOL_FALSE = 40,                     /* FALSE  */
  YYSYMBOL_41_ = 41,                       /* ';'  */
  YYSYMBOL_42_ = 42,                       /* '.'  */
  YYSYMBOL_43_ = 43,                       /* '('  */
  YYSYMBOL_44_ = 44,                       /* ')'  */
  YYSYMBOL_45_ = 45,                       /* ','  */
  YYSYMBOL_46_ = 46,                       /* '='  */
  YYSYMBOL_47_ = 47,                       /* '['  */
  YYSYMBOL_48_ = 48,                       /* ']'  */
  YYSYMBOL_49_ = 49,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_program = 51,                   /* program  */
  YYSYMBOL_program_head = 52,              /* program_head  */
  YYSYMBOL_program_body = 53,              /* program_body  */
  YYSYMBOL_id_list = 54,                   /* id_list  */
  YYSYMBOL_const_declarations = 55,        /* const_declarations  */
  YYSYMBOL_const_declaration = 56,         /* const_declaration  */
  YYSYMBOL_const_variable = 57,            /* const_variable  */
  YYSYMBOL_num = 58,                       /* num  */
  YYSYMBOL_type = 59,                      /* type  */
  YYSYMBOL_record_body = 60,               /* record_body  */
  YYSYMBOL_standard_type = 61,             /* standard_type  */
  YYSYMBOL_periods = 62,                   /* periods  */
  YYSYMBOL_period = 63,                    /* period  */
  YYSYMBOL_var_declarations = 64,          /* var_declarations  */
  YYSYMBOL_var_declaration = 65,           /* var_declaration  */
  YYSYMBOL_subprogram_declarations = 66,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration = 67,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_body = 68,           /* subprogram_body  */
  YYSYMBOL_subprogram_head = 69,           /* subprogram_head  */
  YYSYMBOL_formal_parameter = 70,          /* formal_parameter  */
  YYSYMBOL_parameter_lists = 71,           /* parameter_lists  */
  YYSYMBOL_parameter_list = 72,            /* parameter_list  */
  YYSYMBOL_var_parameter = 73,             /* var_parameter  */
  YYSYMBOL_value_parameter = 74,           /* value_parameter  */
  YYSYMBOL_compound_statement = 75,        /* compound_statement  */
  YYSYMBOL_statement_list = 76,            /* statement_list  */
  YYSYMBOL_statement = 77,                 /* statement  */
  YYSYMBOL_variable_list = 78,             /* variable_list  */
  YYSYMBOL_variable = 79,                  /* variable  */
  YYSYMBOL_id_varparts = 80,               /* id_varparts  */
  YYSYMBOL_id_varpart = 81,                /* id_varpart  */
  YYSYMBOL_else_part = 82,                 /* else_part  */
  YYSYMBOL_updown = 83,                    /* updown  */
  YYSYMBOL_call_procedure_statement = 84,  /* call_procedure_statement  */
  YYSYMBOL_expression_list = 85,           /* expression_list  */
  YYSYMBOL_expression = 86,                /* expression  */
  YYSYMBOL_simple_expression = 87,         /* simple_expression  */
  YYSYMBOL_term = 88,                      /* term  */
  YYSYMBOL_factor = 89,                    /* factor  */
  YYSYMBOL_unsigned_const_variable = 90    /* unsigned_const_variable  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  201

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      43,    44,     2,     2,    45,     2,    42,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    41,
       2,    46,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    92,    92,   107,   116,   125,   136,   148,   158,   168,
     174,   183,   203,   223,   233,   242,   251,   262,   268,   275,
     287,   322,   340,   346,   353,   364,   383,   395,   407,   438,
     444,   466,   494,   521,   527,   540,   550,   561,   615,   663,
     671,   682,   695,   706,   716,   727,   742,   763,   772,   780,
     789,   820,   828,   836,   851,   865,   897,   903,   915,   931,
     941,   953,   985,  1066,  1075,  1095,  1107,  1120,  1126,  1135,
    1142,  1150,  1182,  1205,  1229,  1242,  1255,  1289,  1316,  1332,
    1352,  1381,  1408,  1418,  1441,  1464,  1474,  1514,  1524,  1535,
    1570,  1580,  1607,  1617,  1627,  1637,  1650,  1660,  1672,  1682,
    1691,  1702
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PROGRAM", "FUNCTION",
  "PROCEDURE", "TO", "DOWNTO", "ARRAY", "CONST", "RECORD", "IF", "THEN",
  "ELSE", "OF", "WHILE", "DO", "FOR", "BEGIN_", "END", "ADDOP", "NOT",
  "PLUS", "UMINUS", "CONSTASSIGNOP", "ID", "CHAR", "INT_NUM", "REAL_NUM",
  "BASIC_TYPE", "RELOP", "MULOP", "STRING_", "VAR", "SUBCATALOG",
  "ASSIGNOP", "WRITE", "SEP", "READ", "TRUE", "FALSE", "';'", "'.'", "'('",
  "')'", "','", "'='", "'['", "']'", "':'", "$accept", "program",
  "program_head", "program_body", "id_list", "const_declarations",
  "const_declaration", "const_variable", "num", "type", "record_body",
  "standard_type", "periods", "period", "var_declarations",
  "var_declaration", "subprogram_declarations", "subprogram_declaration",
  "subprogram_body", "subprogram_head", "formal_parameter",
  "parameter_lists", "parameter_list", "var_parameter", "value_parameter",
  "compound_statement", "statement_list", "statement", "variable_list",
  "variable", "id_varparts", "id_varpart", "else_part", "updown",
  "call_procedure_statement", "expression_list", "expression",
  "simple_expression", "term", "factor", "unsigned_const_variable", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-101)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-73)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,    28,    88,    63,   -16,  -101,    54,    13,    66,  -101,
       7,    92,    52,  -101,  -101,    78,  -101,    64,  -101,    69,
      86,   109,   109,  -101,  -101,  -101,    79,  -101,  -101,    87,
      24,    75,    58,   108,  -101,   120,   116,  -101,  -101,  -101,
    -101,   100,    18,    78,   160,   166,    60,   151,    63,  -101,
    -101,  -101,  -101,   188,  -101,   146,    78,  -101,  -101,  -101,
      25,    82,    82,   161,   161,   173,    17,    90,   156,  -101,
       5,  -101,     9,  -101,  -101,    66,  -101,  -101,   188,   183,
     162,    18,    15,   157,   164,   169,   169,   169,   170,  -101,
    -101,  -101,   161,  -101,  -101,   195,    67,   186,  -101,  -101,
     202,   184,   129,   -13,   161,   196,  -101,    60,  -101,   161,
     204,   189,    -7,  -101,  -101,    78,  -101,    78,    37,    80,
    -101,  -101,  -101,   191,  -101,  -101,   186,   186,   137,   180,
      60,   169,   169,   169,   161,   161,   169,    60,   161,    21,
      95,  -101,   200,   161,  -101,   103,   185,   122,  -101,  -101,
     192,  -101,  -101,   188,   188,   212,  -101,   191,    15,  -101,
     190,  -101,   126,  -101,   216,   186,   186,   186,    72,    72,
    -101,  -101,   168,  -101,   161,  -101,    84,  -101,   193,  -101,
     196,  -101,  -101,    18,  -101,  -101,  -101,  -101,    60,  -101,
    -101,  -101,   161,  -101,  -101,  -101,  -101,  -101,   214,    60,
    -101
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     9,     0,     1,     0,     0,    29,     5,
       0,     0,     0,    96,     2,     0,    33,     0,     8,     0,
       0,     0,     0,    16,    17,    18,     0,    98,    14,    10,
       0,     0,     0,     0,     4,     0,     0,    15,    13,   100,
      12,     0,     0,    30,     0,     0,    56,     0,     9,     6,
      97,     3,     7,     0,    99,     0,    22,    25,    32,    19,
       0,    39,    39,     0,     0,     0,    63,     0,     0,    52,
       0,    49,     0,    51,    34,    29,    35,    11,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,    63,    93,
      94,    95,     0,    92,    88,     0,    78,    82,    85,    87,
       0,     0,     0,    62,     0,     0,    47,    56,   101,     0,
       0,     0,     0,    27,    21,    24,    31,     0,     0,     0,
      42,    43,    44,     0,    38,    91,    83,    84,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    56,     0,    73,
       0,    75,     0,     0,    64,     0,    63,     0,    59,    48,
       0,    50,    36,     0,     0,     0,    45,     0,     0,    40,
       0,    61,     0,    90,    67,    79,    80,    81,    76,    77,
      86,    54,     0,    71,     0,    66,     0,    58,     0,    57,
       0,    28,    26,     0,    46,    41,    37,    89,    56,    53,
      69,    70,     0,    74,    65,    60,    20,    68,     0,    56,
      55
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,  -101,  -101,  -101,    -6,   187,  -101,    -8,   -10,   -79,
    -101,  -100,  -101,    85,   158,   176,  -101,  -101,  -101,  -101,
     172,  -101,    83,  -101,   119,   -27,  -101,   -88,  -101,   -46,
    -101,  -101,  -101,  -101,  -101,   -89,   -57,    44,   -66,   -72,
    -101
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     7,   118,     8,    12,   111,    93,    58,
      79,    59,   112,   113,    16,    31,    32,    47,    76,    48,
      83,   119,   120,   121,   122,    69,    70,    71,   147,    94,
     103,   144,   189,   192,    73,   140,   141,    96,    97,    98,
      99
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,    28,   116,    27,    20,    49,    95,   100,    17,    30,
     108,    37,    38,   125,    13,   145,    28,     1,    40,   149,
     126,   127,   -61,   160,   106,     9,    55,    10,    56,   142,
     -72,    28,    18,    54,   143,   129,   -72,    60,   154,   162,
      18,   155,   164,    28,   109,    77,   107,    57,   117,   171,
      30,    19,   151,     4,   176,    14,   -61,   184,   -72,   148,
     102,   150,    44,    45,   170,   165,   166,   167,    28,    36,
      36,    63,     6,    42,    81,    64,    46,    65,    46,    11,
      39,   172,    36,   152,   150,    66,   157,   131,     5,   132,
     133,   150,   131,    29,   132,   133,    67,   134,    68,    15,
     197,    21,    22,    18,   196,    23,    24,    25,    33,    60,
      34,   200,    41,   135,    21,    22,    43,   193,    23,    24,
      25,   158,    21,    22,   159,    82,    23,    24,    25,   174,
      35,    36,   194,   104,   195,   198,    24,    25,    26,   173,
     174,    52,   150,    28,    28,   181,    53,   177,   174,    50,
      85,    86,    87,   150,    88,    89,    24,    25,    85,    86,
      87,    51,    88,    89,    24,    25,   179,   180,    90,    91,
     187,   174,    92,   139,   190,   191,    90,    91,   168,   169,
      92,   161,    85,    86,    87,    61,    88,    89,    24,    25,
      85,    62,    74,    78,    88,    89,    24,    25,   101,   105,
      90,    91,   114,   115,    92,   124,   123,   130,    90,    91,
      21,    22,    92,   128,    23,    24,    25,   136,   137,   138,
      57,   146,    46,   153,   163,   175,   183,   109,   178,   188,
     199,   186,    80,   110,    84,    75,   156,   161,     0,   182,
       0,   185
};

static const yytype_int16 yycheck[] =
{
      46,    11,    81,    11,    10,    32,    63,    64,     1,    15,
       1,    21,    22,    85,     1,   104,    26,     3,    26,   107,
      86,    87,     1,   123,    19,    41,     8,    43,    10,    42,
      13,    41,    25,    41,    47,    92,    19,    43,    45,   128,
      25,    48,   130,    53,    35,    53,    41,    29,    33,   137,
      56,    44,   109,    25,   143,    42,    35,   157,    41,   105,
      43,   107,     4,     5,   136,   131,   132,   133,    78,    45,
      45,    11,     9,    49,    49,    15,    18,    17,    18,    25,
       1,   138,    45,   110,   130,    25,    49,    20,     0,    22,
      23,   137,    20,    41,    22,    23,    36,    30,    38,    33,
     188,    22,    23,    25,   183,    26,    27,    28,    44,   115,
      41,   199,    25,    46,    22,    23,    41,   174,    26,    27,
      28,    41,    22,    23,    44,    43,    26,    27,    28,    45,
      44,    45,    48,    43,   180,   192,    27,    28,    46,    44,
      45,    25,   188,   153,   154,   153,    46,    44,    45,    41,
      21,    22,    23,   199,    25,    26,    27,    28,    21,    22,
      23,    41,    25,    26,    27,    28,    44,    45,    39,    40,
      44,    45,    43,    44,     6,     7,    39,    40,   134,   135,
      43,    44,    21,    22,    23,    25,    25,    26,    27,    28,
      21,    25,    41,    47,    25,    26,    27,    28,    25,    43,
      39,    40,    19,    41,    43,    41,    49,    12,    39,    40,
      22,    23,    43,    43,    26,    27,    28,    31,    16,    35,
      29,    25,    18,    34,    44,    25,    14,    35,    43,    13,
      16,    41,    56,    75,    62,    48,   117,    44,    -1,   154,
      -1,   158
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    51,    52,    25,     0,     9,    53,    55,    41,
      43,    25,    56,     1,    42,    33,    64,     1,    25,    44,
      54,    22,    23,    26,    27,    28,    46,    57,    58,    41,
      54,    65,    66,    44,    41,    44,    45,    58,    58,     1,
      57,    25,    49,    41,     4,     5,    18,    67,    69,    75,
      41,    41,    25,    46,    57,     8,    10,    29,    59,    61,
      54,    25,    25,    11,    15,    17,    25,    36,    38,    75,
      76,    77,    79,    84,    41,    55,    68,    57,    47,    60,
      65,    49,    43,    70,    70,    21,    22,    23,    25,    26,
      39,    40,    43,    58,    79,    86,    87,    88,    89,    90,
      86,    25,    43,    80,    43,    43,    19,    41,     1,    35,
      64,    57,    62,    63,    19,    41,    59,    33,    54,    71,
      72,    73,    74,    49,    41,    89,    88,    88,    43,    86,
      12,    20,    22,    23,    30,    46,    31,    16,    35,    44,
      85,    86,    42,    47,    81,    85,    25,    78,    79,    77,
      79,    86,    75,    34,    45,    48,    74,    49,    41,    44,
      61,    44,    85,    44,    77,    88,    88,    88,    87,    87,
      89,    77,    86,    44,    45,    25,    85,    44,    43,    44,
      45,    57,    63,    14,    61,    72,    41,    44,    13,    82,
       6,     7,    83,    86,    48,    79,    59,    77,    86,    16,
      77
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    52,    52,    53,    54,    54,    55,
      55,    56,    56,    57,    57,    57,    57,    58,    58,    59,
      59,    59,    60,    60,    60,    61,    62,    62,    63,    64,
      64,    65,    65,    66,    66,    67,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    74,    75,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    78,
      78,    79,    79,    80,    80,    81,    81,    82,    82,    83,
      83,    84,    84,    84,    85,    85,    86,    86,    86,    87,
      87,    87,    87,    87,    87,    88,    88,    89,    89,    89,
      89,    89,    90,    90,    90,    90,    51,    52,    56,    56,
      56,    76
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     6,     5,     3,     4,     3,     1,     0,
       3,     5,     3,     2,     1,     2,     1,     1,     1,     1,
       6,     3,     0,     1,     2,     1,     3,     1,     3,     0,
       3,     5,     3,     0,     3,     2,     3,     6,     4,     0,
       3,     3,     1,     1,     1,     2,     3,     3,     3,     1,
       3,     1,     1,     5,     4,     8,     0,     4,     4,     1,
       3,     3,     2,     0,     2,     3,     2,     0,     2,     1,
       1,     4,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     2,     1,     3,     1,     1,     4,
       3,     2,     1,     1,     1,     1,     3,     6,     2,     4,
       3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (real_ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, real_ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, AST *real_ast)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (real_ast);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, AST *real_ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, real_ast);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, AST *real_ast)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], real_ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, real_ast); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, AST *real_ast)
{
  YY_USE (yyvaluep);
  YY_USE (real_ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (AST *real_ast)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: program_head program_body '.'  */
#line 93 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // prgram -> program_head program_body '.'
	    ProgramNode* node = new ProgramNode();  // 创建 ProgramNode 节点
        // 添加 program_head 和 program_body 节点到 real_ast 中
        node->append_child((yyvsp[-2].program_head_node)); // $1 是 Bison 特有语法，引用产生式右侧的第一个符号
        node->append_child((yyvsp[-1].program_body_node)); // 同理，$2 引用产生式右侧的第二个符号

        if((!error_flag) && (!semantic_error_flag) && (!lex_error_flag)){  // 如果语法、语义和词法没有错误
            real_ast->set_root(node);   // 设置 real_ast 的根节点
            real_ast->set_valid(true);  // 设置 real_ast 的有效性
        }
        delete top_table_set;  // 删除当前符号表
    }
#line 1613 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 3: /* program_head: PROGRAM ID '(' id_list ')' ';'  */
#line 107 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                                   {
        // program_head -> PROGRAM ID '(' id_list ')' ';'
    	if(error_flag)
	        break;
        (yyval.program_head_node) = new ProgramHeadNode();  // 创建 ProgramHeadNode 节点
        LeafNode* leaf_node = new LeafNode((yyvsp[-4].token_info).value);  // 创建 LeafNode 节点
        (yyval.program_head_node)->append_child(leaf_node);  // 将 LeafNode 节点添加到 ProgramHeadNode 节点中
        table_set_queue.push(top_table_set);  // 将当前符号表添加到符号表栈中
        real_ast->libs()->Preset(table_set_queue.top()->symbols());  // 将当前符号表的符号添加到 real_ast 的符号表中
    }
#line 1628 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 4: /* program_head: PROGRAM ID '(' ')' ';'  */
#line 116 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                                {
        // program_head -> PROGRAM ID '('  ')' ';'
	    if(error_flag)
	        break;
        (yyval.program_head_node) = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode((yyvsp[-3].token_info).value);
        (yyval.program_head_node)->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    }
#line 1643 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 5: /* program_head: PROGRAM ID ';'  */
#line 125 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                       {
        // program_head -> PROGRAM ID ';'
        if(error_flag)
            break;
        (yyval.program_head_node) = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode((yyvsp[-1].token_info).value);
        (yyval.program_head_node)->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    }
#line 1658 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 6: /* program_body: const_declarations var_declarations subprogram_declarations compound_statement  */
#line 137 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                                               {
        // program_body -> const_declarations var_declarations subprogram_declarations compound_statement
        if(error_flag)
            break;
        (yyval.program_body_node) = new ProgramBodyNode();
        (yyval.program_body_node)->append_child((yyvsp[-3].const_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-2].variable_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-1].subprogram_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[0].compound_statement_node));
    }
#line 1673 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 7: /* id_list: id_list ',' ID  */
#line 148 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                   { 
        // id_list -> id_list ',' ID
        (yyvsp[-2].id_list_node_info).list_ref->push_back(std::make_pair((yyvsp[0].token_info).value.get<string>(),(yyvsp[0].token_info).column_num));
        (yyval.id_list_node_info).list_ref = (yyvsp[-2].id_list_node_info).list_ref;
        (yyval.id_list_node_info).id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        if(error_flag)
            break;
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value);
        (yyval.id_list_node_info).id_list_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.id_list_node_info).id_list_node->append_child(leaf_node);
    }
#line 1689 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 8: /* id_list: ID  */
#line 158 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
           {
        // id_list -> ID
        (yyval.id_list_node_info).list_ref = new std::vector<std::pair<std::string,int>>();
        (yyval.id_list_node_info).list_ref->push_back(std::make_pair((yyvsp[0].token_info).value.get<string>(),(yyvsp[0].token_info).column_num));
        if(error_flag)
            break;
        (yyval.id_list_node_info).id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value);
        (yyval.id_list_node_info).id_list_node->append_child(leaf_node);
    }
#line 1704 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 9: /* const_declarations: %empty  */
#line 168 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                    {
        // const_declarations -> empty
        if(error_flag)
            break;
        (yyval.const_declarations_node) = new ConstDeclarationsNode();
    }
#line 1715 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 10: /* const_declarations: CONST const_declaration ';'  */
#line 175 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // const_declarations -> CONST const_declaration ';'
        if(error_flag)
            break;
        (yyval.const_declarations_node) = new ConstDeclarationsNode(); 
        (yyval.const_declarations_node)->append_child((yyvsp[-1].const_declaration_node));
    }
#line 1727 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 11: /* const_declaration: const_declaration ';' ID '=' const_variable  */
#line 184 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // const_declaration -> const_declaration ';' ID '=' const_variable
        if(error_flag)
            break;
        if (!(yyvsp[0].value_node_info).is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol((yyvsp[-2].token_info).value.get<string>(),(yyvsp[0].value_node_info).value,(yyvsp[-2].token_info).line_num);

        if(!table_set_queue.top()->InsertSymbol((yyvsp[-2].token_info).value.get<string>(),symbol)){
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
        } else{
            (yyval.const_declaration_node) = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION,(yyvsp[0].value_node_info).type_ptr);
            (yyval.const_declaration_node)->append_child((yyvsp[-4].const_declaration_node));
            LeafNode* leaf_node = new LeafNode((yyvsp[-2].token_info).value);
            (yyval.const_declaration_node)->append_child(leaf_node);
            (yyval.const_declaration_node)->append_child((yyvsp[0].value_node_info).const_variable_node);
        }
    }
#line 1751 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 12: /* const_declaration: ID '=' const_variable  */
#line 204 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // const_declaration -> ID '=' const_variable
        if(error_flag)
            break;
        if (!(yyvsp[0].value_node_info).is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol((yyvsp[-2].token_info).value.get<string>(),(yyvsp[0].value_node_info).value,(yyvsp[-2].token_info).line_num);

        if(!table_set_queue.top()->InsertSymbol((yyvsp[-2].token_info).value.get<string>(),symbol)){
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
        } else {
            (yyval.const_declaration_node) = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE,(yyvsp[0].value_node_info).type_ptr);
            LeafNode* leaf_node = new LeafNode((yyvsp[-2].token_info).value);
            (yyval.const_declaration_node)->append_child(leaf_node);
            (yyval.const_declaration_node)->append_child((yyvsp[0].value_node_info).const_variable_node);
        }
    }
#line 1774 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 13: /* const_variable: UMINUS num  */
#line 224 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {  
        // const_variable -> UMINUS num
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        (yyvsp[0].value_node_info).value.set_unimus();
        (yyval.value_node_info).value = (yyvsp[0].value_node_info).value;
        if(error_flag)
            break; 
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value);
    }
#line 1788 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 14: /* const_variable: num  */
#line 234 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // const_variable -> num
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        (yyval.value_node_info).value = (yyvsp[0].value_node_info).value;
        if(error_flag)
            break; 
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value);
    }
#line 1801 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 15: /* const_variable: PLUS num  */
#line 243 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {  
        // const_variable -> PLUS num
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        (yyval.value_node_info).value = (yyvsp[0].value_node_info).value;
        if(error_flag)
            break; 
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value);
    }
#line 1814 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 16: /* const_variable: CHAR  */
#line 252 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // const_variable -> CHAR
        (yyval.value_node_info).type_ptr = TYPE_CHAR;
        (yyval.value_node_info).value = (yyvsp[0].token_info).value;
        if(error_flag)
            break; 
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].token_info).value);

    }
#line 1828 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 17: /* num: INT_NUM  */
#line 263 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // num -> INT_NUM
        (yyval.value_node_info).type_ptr = TYPE_INT;
        (yyval.value_node_info).value = (yyvsp[0].token_info).value;
    }
#line 1838 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 18: /* num: REAL_NUM  */
#line 269 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // num -> REAL_NUM
        (yyval.value_node_info).type_ptr = TYPE_REAL;
        (yyval.value_node_info).value = (yyvsp[0].token_info).value;
    }
#line 1848 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 19: /* type: standard_type  */
#line 276 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // type -> standard_type
        (yyval.type_node_info).main_type = (TypeAttr::MainType)0;
        (yyval.type_node_info).type_ptr = (yyvsp[0].standared_type_node_info).type_ptr;
        if(error_flag)
            break;
        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        (yyval.type_node_info).base_type_node = (yyval.type_node_info).type_node;
        (yyval.type_node_info).type_node->set_base_type_node((yyval.type_node_info).type_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[0].standared_type_node_info).standard_type_node);
    }
#line 1864 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 20: /* type: ARRAY '[' periods ']' OF type  */
#line 288 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // type -> ARRAY '[' periods ']' OF type
        (yyval.type_node_info).main_type = (TypeAttr::MainType)1;
        (yyval.type_node_info).base_type_node = (yyvsp[0].type_node_info).base_type_node;
        (yyval.type_node_info).bounds = (yyvsp[-3].periods_node_info).bounds;
        if ((yyvsp[-3].periods_node_info).bounds){
            auto merged_bounds = new std::vector<ArrayType::ArrayBound>();
            for (auto i : *((yyvsp[-3].periods_node_info).bounds)){
                merged_bounds->push_back(i);
            }
            auto basic_type = (yyvsp[0].type_node_info).type_ptr;
            if((yyvsp[0].type_node_info).type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
                for (auto i : *((yyvsp[0].type_node_info).bounds)){
                    merged_bounds->push_back(i);
                    (yyval.type_node_info).bounds->push_back(i);
                }
                basic_type = (yyvsp[0].type_node_info).type_ptr->DynamicCast<ArrayType>()->base_type();
            }
            PtrCollect((yyvsp[0].type_node_info).type_ptr);
            (yyval.type_node_info).type_ptr = new ArrayType(basic_type, *merged_bounds);
            
            delete merged_bounds;
        }
        if(error_flag)
            break; 
        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        (yyval.type_node_info).type_node->set_base_type_node((yyvsp[0].type_node_info).base_type_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[-3].periods_node_info).periods_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[0].type_node_info).type_node);
        delete (yyvsp[0].type_node_info).bounds;
        if ((yyvsp[0].type_node_info).record_info){
            delete (yyvsp[0].type_node_info).record_info;
        }
    }
#line 1903 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 21: /* type: RECORD record_body END  */
#line 323 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // type -> RECORD record_body END
        (yyval.type_node_info).main_type = (TypeAttr::MainType)2;
        (yyval.type_node_info).record_info = (yyvsp[-1].record_node_info).record_info;
        if ((yyvsp[-1].record_node_info).record_info){
            (yyval.type_node_info).type_ptr = new RecordType(*((yyvsp[-1].record_node_info).record_info));
        } else{
             (yyval.type_node_info).type_ptr = new RecordType();
        }
        if(error_flag)
            break; 
        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        (yyval.type_node_info).base_type_node = (yyval.type_node_info).type_node;
        (yyval.type_node_info).type_node->append_child((yyvsp[-1].record_node_info).record_body_node);
        (yyval.type_node_info).type_node->set_base_type_node((yyval.type_node_info).type_node);
    }
#line 1924 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 22: /* record_body: %empty  */
#line 340 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // record_body -> empty
        (yyval.record_node_info).record_info = new std::unordered_map<std::string, TypeTemplate*>();
        if(error_flag)
            break;
        (yyval.record_node_info).record_body_node = new RecordBodyNode();
    }
#line 1936 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 23: /* record_body: var_declaration  */
#line 346 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                        {
	(yyval.record_node_info).record_info = (yyvsp[0].variable_declaration_node_info).record_info;
	if(error_flag)
	    break;
	(yyval.record_node_info).record_body_node = new RecordBodyNode();
	(yyval.record_node_info).record_body_node->append_child((yyvsp[0].variable_declaration_node_info).variable_declaration_node);
	delete (yyvsp[0].variable_declaration_node_info).pos_info;
    }
#line 1949 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 24: /* record_body: var_declaration ';'  */
#line 354 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // record_body -> var_declaration ';'
        (yyval.record_node_info).record_info = (yyvsp[-1].variable_declaration_node_info).record_info;
        if(error_flag)
            break;
        (yyval.record_node_info).record_body_node = new RecordBodyNode();
        (yyval.record_node_info).record_body_node->append_child((yyvsp[-1].variable_declaration_node_info).variable_declaration_node);
        delete (yyvsp[-1].variable_declaration_node_info).pos_info;
    }
#line 1963 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 25: /* standard_type: BASIC_TYPE  */
#line 365 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // standard_type -> BASIC_TYPE
        string typestr = (yyvsp[0].token_info).value.get<string>();
        if (typestr == "integer"){
            (yyval.standared_type_node_info).type_ptr = TYPE_INT;
        } else if(typestr == "real"){
            (yyval.standared_type_node_info).type_ptr = TYPE_REAL;
        } else if(typestr == "boolean"){
            (yyval.standared_type_node_info).type_ptr = TYPE_BOOL;
        } else{
            (yyval.standared_type_node_info).type_ptr = TYPE_CHAR;
        }
        if(error_flag)
            break;
        (yyval.standared_type_node_info).standard_type_node = new BasicTypeNode();
        (yyval.standared_type_node_info).standard_type_node->set_type(dynamic_cast<BasicType*>((yyval.standared_type_node_info).type_ptr));
    }
#line 1985 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 26: /* periods: periods ',' period  */
#line 384 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // periods -> periods ',' period
        (yyval.periods_node_info).bounds = (yyvsp[-2].periods_node_info).bounds;
        (yyval.periods_node_info).bounds->push_back(*((yyvsp[0].period_node_info).bound));
        if(error_flag)
            break;
        (yyval.periods_node_info).periods_node = new PeriodsNode();
        (yyval.periods_node_info).periods_node->append_child((yyvsp[-2].periods_node_info).periods_node);
        (yyval.periods_node_info).periods_node->append_child((yyvsp[0].period_node_info).period_node);
        delete (yyvsp[0].period_node_info).bound;
    }
#line 2001 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 27: /* periods: period  */
#line 396 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // periods -> period
        (yyval.periods_node_info).bounds = new std::vector<ArrayType::ArrayBound>();
        (yyval.periods_node_info).bounds->push_back(*((yyvsp[0].period_node_info).bound));
        if(error_flag)
            break;
        (yyval.periods_node_info).periods_node = new PeriodsNode();
        (yyval.periods_node_info).periods_node->append_child((yyvsp[0].period_node_info).period_node);
        delete (yyvsp[0].period_node_info).bound;
    }
#line 2016 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 28: /* period: const_variable SUBCATALOG const_variable  */
#line 408 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {     
        // period -> const_variable SUBCATALOG const_variable
        int arr_len=0;
        (yyval.period_node_info).bound = new ArrayType::ArrayBound();
        if ((yyvsp[-2].value_node_info).type_ptr == TYPE_INT&&(yyvsp[0].value_node_info).type_ptr == TYPE_INT){
            arr_len = ((yyvsp[0].value_node_info).value - (yyvsp[-2].value_node_info).value).get<int>();
            (yyval.period_node_info).bound-> type_ = TYPE_INT;
            (yyval.period_node_info).bound->lb_ = (yyvsp[-2].value_node_info).value.get<int>();
            (yyval.period_node_info).bound->ub_ = (yyvsp[0].value_node_info).value.get<int>();
        } else if((yyvsp[-2].value_node_info).type_ptr == TYPE_CHAR&&(yyvsp[0].value_node_info).type_ptr == TYPE_CHAR){
            arr_len = (int)((yyvsp[0].value_node_info).value - (yyvsp[-2].value_node_info).value).get<char>();
            (yyval.period_node_info).bound-> type_ = TYPE_CHAR;
            (yyval.period_node_info).bound->lb_ = int((yyvsp[-2].value_node_info).value.get<int>());
            (yyval.period_node_info).bound->ub_ = int((yyvsp[0].value_node_info).value.get<int>());
        } else {
            semantic_error(real_ast,"array bound should be integer or char",(yyvsp[-1].token_info).line_num,(yyvsp[-1].token_info).column_num);
        }
        if(arr_len < 0){
            arr_len = 0;
            semantic_error(real_ast,"array bound should be positive",(yyvsp[-1].token_info).line_num,(yyvsp[-1].token_info).column_num);
        }
        if(error_flag){
            break;
        }
        (yyval.period_node_info).period_node =new PeriodNode();
        (yyval.period_node_info).period_node->set_len(arr_len+1);
        (yyval.period_node_info).period_node->append_child((yyvsp[-2].value_node_info).const_variable_node);
        (yyval.period_node_info).period_node->append_child((yyvsp[0].value_node_info).const_variable_node);
    }
#line 2050 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 29: /* var_declarations: %empty  */
#line 438 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // var_declarations -> empty
        if(error_flag)
            break;
        (yyval.variable_declarations_node) = new VariableDeclarationsNode();
    }
#line 2061 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 30: /* var_declarations: VAR var_declaration ';'  */
#line 445 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // var_declarations -> VAR var_declaration ';'
        if(error_flag)
            break;
        for (auto i : *((yyvsp[-1].variable_declaration_node_info).record_info)){
            int line = (yyvsp[-1].variable_declaration_node_info).pos_info->find(i.first)->second.first;
            int row = (yyvsp[-1].variable_declaration_node_info).pos_info->find(i.first)->second.second;
            SymbolTemplate *obj = new SymbolTemplate(i.first, i.second,line);
            if(!table_set_queue.top()->InsertSymbol(i.first,obj)){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line,row);
                yynote(i.first,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.first)->decl_line());
            }
        }
        (yyval.variable_declarations_node) = new VariableDeclarationsNode();
        (yyval.variable_declarations_node)->append_child((yyvsp[-1].variable_declaration_node_info).variable_declaration_node);
        delete (yyvsp[-1].variable_declaration_node_info).pos_info;
        if ((yyvsp[-1].variable_declaration_node_info).record_info){
            delete (yyvsp[-1].variable_declaration_node_info).record_info;
        }
    }
#line 2086 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 31: /* var_declaration: var_declaration ';' id_list ':' type  */
#line 467 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // var_declaration -> var_declaration ';' id_list ':' type
         if(error_flag)
            break;   
        (yyval.variable_declaration_node_info).record_info = (yyvsp[-4].variable_declaration_node_info).record_info;
        (yyval.variable_declaration_node_info).pos_info = (yyvsp[-4].variable_declaration_node_info).pos_info;
        for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
            auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, (yyvsp[0].type_node_info).type_ptr));
            (yyval.variable_declaration_node_info).pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-4].variable_declaration_node_info).variable_declaration_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[0].type_node_info).type_node);
        delete (yyvsp[-2].id_list_node_info).list_ref;
        if((yyvsp[0].type_node_info).bounds) {
            delete (yyvsp[0].type_node_info).bounds;
        }
        if((yyvsp[0].type_node_info).record_info) {
            delete (yyvsp[0].type_node_info).record_info;
        }
        PtrCollect((yyvsp[0].type_node_info).type_ptr);

    }
#line 2118 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 32: /* var_declaration: id_list ':' type  */
#line 495 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // var_declaration -> id_list ':' type
        if(error_flag)
           break;
        (yyval.variable_declaration_node_info).record_info = new std::unordered_map<std::string, TypeTemplate*>();
        (yyval.variable_declaration_node_info).pos_info = new std::unordered_map<std::string, std::pair<int,int>>();
        for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
            auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, (yyvsp[0].type_node_info).type_ptr));
            (yyval.variable_declaration_node_info).pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[0].type_node_info).type_node);
        delete (yyvsp[-2].id_list_node_info).list_ref;
        if((yyvsp[0].type_node_info).bounds) {
            delete (yyvsp[0].type_node_info).bounds;
        }
        if((yyvsp[0].type_node_info).record_info) {
            delete (yyvsp[0].type_node_info).record_info;
        }
        PtrCollect((yyvsp[0].type_node_info).type_ptr);
    }
#line 2148 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 33: /* subprogram_declarations: %empty  */
#line 521 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_declarations -> empty
        if(error_flag)
            break;
        (yyval.subprogram_declarations_node) = new SubprogramDeclarationsNode();
    }
#line 2159 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 34: /* subprogram_declarations: subprogram_declarations subprogram_declaration ';'  */
#line 528 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration ';'
        if(error_flag)
            break;
        (yyval.subprogram_declarations_node) = new SubprogramDeclarationsNode();
        (yyval.subprogram_declarations_node)->append_child((yyvsp[-2].subprogram_declarations_node));
        (yyval.subprogram_declarations_node)->append_child((yyvsp[-1].subprogram_declaration_node));
        TableSet* top = table_set_queue.top();
        table_set_queue.pop();
        delete top;
    }
#line 2175 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 35: /* subprogram_declaration: subprogram_head subprogram_body  */
#line 541 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_declaration -> subprogram_head subprogram_body
        if(error_flag)
            break;
        (yyval.subprogram_declaration_node) = new SubprogramDeclarationNode();
        (yyval.subprogram_declaration_node)->append_child((yyvsp[-1].subprogram_head_node));
        (yyval.subprogram_declaration_node)->append_child((yyvsp[0].subprogram_body_node));
    }
#line 2188 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 36: /* subprogram_body: const_declarations var_declarations compound_statement  */
#line 551 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_body -> const_declarations var_declarations compound_statement
        if(error_flag)
            break;
        (yyval.subprogram_body_node) = new SubprogramBodyNode();
        (yyval.subprogram_body_node)->append_child((yyvsp[-2].const_declarations_node));
        (yyval.subprogram_body_node)->append_child((yyvsp[-1].variable_declarations_node));
        (yyval.subprogram_body_node)->append_child((yyvsp[0].compound_statement_node));
    }
#line 2202 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 37: /* subprogram_head: FUNCTION ID formal_parameter ':' standard_type ';'  */
#line 562 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_head -> FUNCTION ID formal_parameter ':' standard_type ';'
        FunctionSymbol* tmp ;
        if((yyvsp[-3].formal_parameter_node_info).parameters){
            tmp = new FunctionSymbol((yyvsp[-4].token_info).value.get<string>(), (yyvsp[-1].standared_type_node_info).type_ptr, (yyvsp[-4].token_info).line_num, *(yyvsp[-3].formal_parameter_node_info).parameters);
        } else {
            tmp = new FunctionSymbol((yyvsp[-4].token_info).value.get<string>(), (yyvsp[-1].standared_type_node_info).type_ptr, (yyvsp[-4].token_info).line_num);
        }
        if (!table_set_queue.top()->InsertSymbol((yyvsp[-4].token_info).value.get<string>(), tmp)){
            string tn = (yyvsp[-4].token_info).value.get<string>();
            semantic_error(real_ast,"redefinition of function '"+tn+"'",(yyvsp[-4].token_info).line_num,(yyvsp[-4].token_info).column_num);
            yynote((yyvsp[-4].token_info).value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-4].token_info).value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet((yyvsp[-4].token_info).value.get<string>(), table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        string tag = (yyvsp[-4].token_info).value.get<string>();
        table_set_queue.top()->InsertSymbol(tag, tmp2);
        SymbolTemplate* tmp3 = new SymbolTemplate("internal_"+tag, (yyvsp[-1].standared_type_node_info).type_ptr, (yyvsp[-4].token_info).line_num);
        table_set_queue.top()->InsertSymbol("internal_"+tag, tmp3);
        if ((yyvsp[-3].formal_parameter_node_info).parameters){
            int cnt = 0;
            for (auto i : *((yyvsp[-3].formal_parameter_node_info).parameters)){
                SymbolTemplate *tmp = new SymbolTemplate(i.name,i.type, (yyvsp[-4].token_info).line_num);
                if (i.mode == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->InsertSymbol(i.name, tmp)){
                    int line = (yyvsp[-3].formal_parameter_node_info).pos_info->at(cnt).first;
                    int row = (yyvsp[-3].formal_parameter_node_info).pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.name +"'",line,row);
                    yynote(i.name,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.name)->decl_line());
                }
                cnt++;
            }
        }
        if(error_flag)
            break;
        (yyval.subprogram_head_node) = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::FUNCTION);
        (yyval.subprogram_head_node)->set_id((yyvsp[-4].token_info).value.get<string>());
        LeafNode *leaf_node = new LeafNode((yyvsp[-4].token_info).value);
        (yyval.subprogram_head_node)->append_child(leaf_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-3].formal_parameter_node_info).formal_parameter_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-1].standared_type_node_info).standard_type_node);
        if((yyvsp[-3].formal_parameter_node_info).parameters){
            delete (yyvsp[-3].formal_parameter_node_info).parameters;
        }
        if((yyvsp[-3].formal_parameter_node_info).pos_info){
            delete (yyvsp[-3].formal_parameter_node_info).pos_info;
        }
    }
#line 2260 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 38: /* subprogram_head: PROCEDURE ID formal_parameter ';'  */
#line 616 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // subprogram_head -> PROCEDURE ID formal_parameter ';'
        if(error_flag)
            break;
        FunctionSymbol* tmp ;
        if((yyvsp[-1].formal_parameter_node_info).parameters){
            tmp = new FunctionSymbol((yyvsp[-2].token_info).value.get<string>(), nullptr, (yyvsp[-2].token_info).line_num, *(yyvsp[-1].formal_parameter_node_info).parameters);
        } else {
            tmp = new FunctionSymbol((yyvsp[-2].token_info).value.get<string>(), nullptr, (yyvsp[-2].token_info).line_num);
        }
        
        if (!table_set_queue.top()->InsertSymbol((yyvsp[-2].token_info).value.get<string>(), tmp)){
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"redefinition of procedure '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
            yynote((yyvsp[-2].token_info).value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-2].token_info).value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet((yyvsp[-2].token_info).value.get<string>(),table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        table_set_queue.top()->InsertSymbol((yyvsp[-2].token_info).value.get<string>(), tmp2);
        if ((yyvsp[-1].formal_parameter_node_info).parameters){
            int cnt = 0;
            for (auto i : *((yyvsp[-1].formal_parameter_node_info).parameters)){
                SymbolTemplate *tmp = new SymbolTemplate(i.name,i.type, (yyvsp[-2].token_info).line_num);
                if (i.mode == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->InsertSymbol(i.name, tmp)){
                    int line = (yyvsp[-1].formal_parameter_node_info).pos_info->at(cnt).first;
                    int row = (yyvsp[-1].formal_parameter_node_info).pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.name +"'",line,row);
                    yynote(i.name,table_set_queue.top()->SearchEntry<SymbolTemplate>(i.name)->decl_line());
                }
                cnt++;
            }
        }
        
        (yyval.subprogram_head_node) = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode((yyvsp[-2].token_info).value);
        (yyval.subprogram_head_node)->append_child(leaf_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-1].formal_parameter_node_info).formal_parameter_node);
        delete (yyvsp[-1].formal_parameter_node_info).parameters;
        delete (yyvsp[-1].formal_parameter_node_info).pos_info;
    }
#line 2311 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 39: /* formal_parameter: %empty  */
#line 663 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // formal_parameter -> empty
        (yyval.formal_parameter_node_info).parameters = new std::vector<FunctionSymbol::ParameterInfo>();
        (yyval.formal_parameter_node_info).pos_info = new std::vector<std::pair<int,int>>();
        if(error_flag)
            break;
        (yyval.formal_parameter_node_info).formal_parameter_node = new FormalParamNode();
    }
#line 2324 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 40: /* formal_parameter: '(' parameter_lists ')'  */
#line 672 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // formal_parameter -> '(' parameter_lists ')'
        (yyval.formal_parameter_node_info).parameters = (yyvsp[-1].parameter_lists_node_info).parameters;
        (yyval.formal_parameter_node_info).pos_info = (yyvsp[-1].parameter_lists_node_info).pos_info;
        if(error_flag)
            break;
        (yyval.formal_parameter_node_info).formal_parameter_node = new FormalParamNode();
        (yyval.formal_parameter_node_info).formal_parameter_node->append_child((yyvsp[-1].parameter_lists_node_info).param_lists_node);
    }
#line 2338 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 41: /* parameter_lists: parameter_lists ';' parameter_list  */
#line 683 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // parameter_lists -> parameter_lists ';' parameter_list
        (yyval.parameter_lists_node_info).parameters = (yyvsp[-2].parameter_lists_node_info).parameters;
        (yyval.parameter_lists_node_info).pos_info = (yyvsp[-2].parameter_lists_node_info).pos_info;
        (yyval.parameter_lists_node_info).parameters->insert((yyval.parameter_lists_node_info).parameters->end(), (yyvsp[0].parameter_list_node_info).parameters->begin(), (yyvsp[0].parameter_list_node_info).parameters->end());
        (yyval.parameter_lists_node_info).pos_info->insert((yyval.parameter_lists_node_info).pos_info->end(),(yyvsp[0].parameter_list_node_info).pos_info->begin(), (yyvsp[0].parameter_list_node_info).pos_info->end());
        if(error_flag)
            break;
        (yyval.parameter_lists_node_info).param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[-2].parameter_lists_node_info).param_lists_node);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[0].parameter_list_node_info).param_list_node);
    }
#line 2355 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 42: /* parameter_lists: parameter_list  */
#line 696 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {  
        // parameter_lists -> parameter_list
        (yyval.parameter_lists_node_info).parameters = (yyvsp[0].parameter_list_node_info).parameters;
        (yyval.parameter_lists_node_info).pos_info = (yyvsp[0].parameter_list_node_info).pos_info;
        if(error_flag)
            break;
        (yyval.parameter_lists_node_info).param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[0].parameter_list_node_info).param_list_node);
    }
#line 2369 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 43: /* parameter_list: var_parameter  */
#line 707 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // parameter_list -> var_parameter
        (yyval.parameter_list_node_info).parameters = (yyvsp[0].var_parameter_node_info).parameters;
        (yyval.parameter_list_node_info).pos_info = (yyvsp[0].var_parameter_node_info).pos_info;
        if(error_flag)
            break;
        (yyval.parameter_list_node_info).param_list_node = new ParamListNode();
        (yyval.parameter_list_node_info).param_list_node->append_child((yyvsp[0].var_parameter_node_info).var_parameter_node);
    }
#line 2383 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 44: /* parameter_list: value_parameter  */
#line 717 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // parameter_list -> value_parameter
        (yyval.parameter_list_node_info).parameters = (yyvsp[0].value_parameter_node_info).parameters;
        (yyval.parameter_list_node_info).pos_info = (yyvsp[0].value_parameter_node_info).pos_info;
        if(error_flag)
            break;
        (yyval.parameter_list_node_info).param_list_node = new ParamListNode();
        (yyval.parameter_list_node_info).param_list_node->append_child((yyvsp[0].value_parameter_node_info).value_parameter_node);
    }
#line 2397 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 45: /* var_parameter: VAR value_parameter  */
#line 728 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // var_parameter -> VAR value_parameter
        int para_len = (yyvsp[0].value_parameter_node_info).parameters->size();
        for (int i = 0; i < para_len; i++){
            (yyvsp[0].value_parameter_node_info).parameters->at(i).mode = FunctionSymbol::PARAM_MODE::REFERENCE;
        }
        (yyval.var_parameter_node_info).parameters = (yyvsp[0].value_parameter_node_info).parameters;
        (yyval.var_parameter_node_info).pos_info = (yyvsp[0].value_parameter_node_info).pos_info;
        if(error_flag)
            break;
        (yyval.var_parameter_node_info).var_parameter_node = new VarParamNode();
        (yyval.var_parameter_node_info).var_parameter_node->append_child((yyvsp[0].value_parameter_node_info).value_parameter_node);
    }
#line 2415 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 46: /* value_parameter: id_list ':' standard_type  */
#line 743 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // value_parameter -> id_list ':' standard_type
        (yyval.value_parameter_node_info).parameters = new std::vector<FunctionSymbol::ParameterInfo>();
        (yyval.value_parameter_node_info).pos_info = new std::vector<std::pair<int,int>>();
        for(auto id_entry : *(yyvsp[-2].id_list_node_info).list_ref){ // Assuming $1.list_ref is a vector of pairs like std::pair<std::string, int>
            FunctionSymbol::ParameterInfo param_info;
            param_info.name = id_entry.first; // or however the name is accessed from id_entry
            param_info.type = (yyvsp[0].standared_type_node_info).type_ptr;    // Assuming $3.type_ptr is BasicType*
            param_info.mode = FunctionSymbol::PARAM_MODE::VALUE;
            (yyval.value_parameter_node_info).parameters->push_back(param_info);
        }
        
        if(error_flag)
            break;
        (yyval.value_parameter_node_info).value_parameter_node = new ValueParamNode();
        (yyval.value_parameter_node_info).value_parameter_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.value_parameter_node_info).value_parameter_node->append_child((yyvsp[0].standared_type_node_info).standard_type_node);
        delete (yyvsp[-2].id_list_node_info).list_ref;
    }
#line 2439 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 47: /* compound_statement: BEGIN_ statement_list END  */
#line 764 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // compound_statement -> BEGIN_ statement_list END
        if(error_flag)
            break;
        (yyval.compound_statement_node) = new CompoundStatementNode();
        (yyval.compound_statement_node)->append_child((yyvsp[-1].statement_list_node));
    }
#line 2451 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 48: /* statement_list: statement_list ';' statement  */
#line 773 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement_list -> statement_list ';' statement
        if(error_flag)
            break;
        (yyval.statement_list_node) = new StatementListNode();
        (yyval.statement_list_node)->append_child((yyvsp[-2].statement_list_node));
        (yyval.statement_list_node)->append_child((yyvsp[0].statement_node));
    }
#line 2464 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 49: /* statement_list: statement  */
#line 781 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement_list -> statement
        if(error_flag)
            break;
        (yyval.statement_list_node) = new StatementListNode();
        (yyval.statement_list_node)->append_child((yyvsp[0].statement_node));
    }
#line 2476 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 50: /* statement: variable ASSIGNOP expression  */
#line 790 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        //statement -> variable ASSIGNOP expression
        if(error_flag)
            break;
        bool var_flag = ((yyvsp[-2].variable_node_info).type_ptr==TYPE_REAL && (yyvsp[0].expression_node_info).type_ptr==TYPE_INT) || ((yyvsp[-2].variable_node_info).type_ptr && (yyvsp[-2].variable_node_info).type_ptr->isSameAs((yyvsp[0].expression_node_info).type_ptr));
        bool str_flag = ((yyvsp[-2].variable_node_info).type_ptr != TYPE_ERROR &&
        		 (yyvsp[-2].variable_node_info).type_ptr->StringLike() &&
        		 (yyvsp[0].expression_node_info).type_ptr==TYPE_STRINGLIKE);
        if(!var_flag && !str_flag){
            string tn1 = type_name((yyvsp[-2].variable_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].expression_node_info).type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
            break;
        }
        std::string func_name = table_set_queue.top()->tag();

        if(func_name == *(yyvsp[-2].variable_node_info).name){
            (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
            if (!(yyvsp[-2].variable_node_info).is_lvalue){
                semantic_error(real_ast,"lvalue required as left operand of assignment",(yyvsp[-1].token_info).line_num,(yyvsp[-1].token_info).column_num);
            }
        }
        if(error_flag)
            break;
        (yyval.statement_node)->append_child((yyvsp[-2].variable_node_info).variable_node);
        (yyval.statement_node)->append_child((yyvsp[0].expression_node_info).expression_node);
        delete (yyvsp[-2].variable_node_info).name;
    }
#line 2511 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 51: /* statement: call_procedure_statement  */
#line 821 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> call_procedure_statement
        if(error_flag)
            break;
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        (yyval.statement_node)->append_child((yyvsp[0].procedure_call_node));
    }
#line 2523 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 52: /* statement: compound_statement  */
#line 829 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> compound_statement
        if(error_flag)
            break;
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[0].compound_statement_node));
    }
#line 2535 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 53: /* statement: IF expression THEN statement else_part  */
#line 837 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // statement -> IF expression THEN statement else_part
        if(error_flag)
            break;
        //类型检查
        if(!((yyvsp[-3].expression_node_info).type_ptr && (yyvsp[-3].expression_node_info).type_ptr->isSameAs(TYPE_BOOL))){
            string tn = type_name((yyvsp[-3].expression_node_info).type_ptr);
            semantic_error(real_ast,"IF quantity cannot be '"+tn+"'",line_count,0);
        }
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-3].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[-1].statement_node));
        (yyval.statement_node)->append_child((yyvsp[0].else_node));
    }
#line 2554 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 54: /* statement: WHILE expression DO statement  */
#line 852 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> WHILE expression DO statement
        if(error_flag)
            break;
        if(!((yyvsp[-2].expression_node_info).type_ptr && (yyvsp[-2].expression_node_info).type_ptr->isSameAs(TYPE_BOOL))){
            string tn = type_name((yyvsp[-2].expression_node_info).type_ptr);
            semantic_error(real_ast,"WHILE quantity cannot be '"+tn+"'",line_count,0);
        }
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-2].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[0].statement_node));

    }
#line 2572 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 55: /* statement: FOR ID ASSIGNOP expression updown expression DO statement  */
#line 866 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> FOR ID ASSIGNOP expression updown expression DO statement
        if(error_flag)
            break;
        //类型检查
        SymbolTemplate *tmp = table_set_queue.top()->SearchEntry<SymbolTemplate>((yyvsp[-6].token_info).value.get<string>());
        if(tmp==nullptr){
            string tn = (yyvsp[-6].token_info).value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared",(yyvsp[-6].token_info).line_num,(yyvsp[-6].token_info).column_num);
            break;
        }
        if((!(tmp->type() && tmp->type()->isSpecificBasicType()))||(!(tmp->type() && tmp->type()->isSameAs((yyvsp[-4].expression_node_info).type_ptr)))){
            string tn1 = type_name(tmp->type());
            string tn2 = type_name((yyvsp[-4].expression_node_info).type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
        }

        if((!((yyvsp[-4].expression_node_info).type_ptr && (yyvsp[-4].expression_node_info).type_ptr->isSameAs((yyvsp[-2].expression_node_info).type_ptr)))||((yyvsp[-4].expression_node_info).type_ptr && (yyvsp[-4].expression_node_info).type_ptr->isSameAs(TYPE_REAL))){
            string tn1 = type_name((yyvsp[-4].expression_node_info).type_ptr);
            string tn2 = type_name((yyvsp[-2].expression_node_info).type_ptr);
            semantic_error(real_ast,"invalid updown type from '"+tn1+"' to '"+tn2+"'",line_count,0);
        }
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::FOR_STATEMENT);
        LeafNode *id_node = new LeafNode((yyvsp[-6].token_info).value);
        (yyval.statement_node)->append_child(id_node);
        (yyval.statement_node)->append_child((yyvsp[-4].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[-3].updown_node));
        (yyval.statement_node)->append_child((yyvsp[-2].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[0].statement_node));
    }
#line 2607 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 56: /* statement: %empty  */
#line 897 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> empty
        if(error_flag)
            break;
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::EPSILON);
    }
#line 2618 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 57: /* statement: READ '(' variable_list ')'  */
#line 904 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> READ '(' variable_list ')'
        if(error_flag)
            break;
        if(!(yyvsp[-1].variable_list_node_info).variable_list_node->set_types((yyvsp[-1].variable_list_node_info).type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in READ",(yyvsp[-3].token_info).line_num,(yyvsp[-3].token_info).column_num);
        }  
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::READ_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-1].variable_list_node_info).variable_list_node);
        delete (yyvsp[-1].variable_list_node_info).type_ptr_list;
    }
#line 2634 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 58: /* statement: WRITE '(' expression_list ')'  */
#line 916 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // statement -> WRITE '(' expression_list ')'
        if(error_flag)
            break;
        if(!(yyvsp[-1].expression_list_node_info).expression_list_node->set_types((yyvsp[-1].expression_list_node_info).type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in WRITE",(yyvsp[-3].token_info).line_num,(yyvsp[-3].token_info).column_num);
        }
        
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::WRITE_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        delete (yyvsp[-1].expression_list_node_info).type_ptr_list;
        delete (yyvsp[-1].expression_list_node_info).is_lvalue_list;
    }
#line 2652 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 59: /* variable_list: variable  */
#line 932 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    { 
        // variable_list -> variable
        (yyval.variable_list_node_info).type_ptr_list = new std::vector<TypeTemplate*>();
        (yyval.variable_list_node_info).type_ptr_list->push_back((yyvsp[0].variable_node_info).type_ptr);
        if(error_flag)
            break;
        (yyval.variable_list_node_info).variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE);
        (yyval.variable_list_node_info).variable_list_node->append_child((yyvsp[0].variable_node_info).variable_node);
        if((yyvsp[0].variable_node_info).name) delete (yyvsp[0].variable_node_info).name;
    }
#line 2667 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 60: /* variable_list: variable_list ',' variable  */
#line 941 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
                                  {
        // variable_list -> variable_list ',' variable
        (yyval.variable_list_node_info).type_ptr_list = (yyvsp[-2].variable_list_node_info).type_ptr_list;
        (yyval.variable_list_node_info).type_ptr_list->push_back((yyvsp[0].variable_node_info).type_ptr);
        if(error_flag)
            break;
        (yyval.variable_list_node_info).variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE_LIST_VARIABLE);
        (yyval.variable_list_node_info).variable_list_node->append_child((yyvsp[-2].variable_list_node_info).variable_list_node);
        (yyval.variable_list_node_info).variable_list_node->append_child((yyvsp[0].variable_node_info).variable_node);
        if((yyvsp[0].variable_node_info).name) delete (yyvsp[0].variable_node_info).name;
    }
#line 2683 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 61: /* variable: ID '(' ')'  */
#line 954 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // variable -> ID '('')'
        if (error_flag) break;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-2].token_info).value.get<string>());
        (yyval.variable_node_info).type_ptr = TYPE_ERROR;
        (yyval.variable_node_info).is_lvalue = false;
        if(tmp == nullptr){
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
            break;
        }
        if(tmp->type() != nullptr && tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION){
            if(!tmp->AssertHasNoParams()){
                string tn = (yyvsp[-2].token_info).value.get<string>();
                string param = tmp->ParamName();
	        semantic_error(real_ast,"too few arguments to function '"+tn+"' (expected '("+param+")')",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
                break;
            }
            (yyval.variable_node_info).type_ptr = tmp->type();
            (yyval.variable_node_info).name = new std::string((yyvsp[-2].token_info).value.get<string>());
            real_ast->libs()->Call(*((yyval.variable_node_info).name));
            string name = (yyvsp[-2].token_info).value.get<string>()+"()";
            (yyval.variable_node_info).variable_node = new VariableNode();
            LeafNode *id_node = new LeafNode(name);
            (yyval.variable_node_info).variable_node->append_child(id_node);
        } else {
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
        }
         
    }
#line 2719 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 62: /* variable: ID id_varparts  */
#line 986 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // variable -> ID id_varparts
        if(error_flag)
            break;
        SymbolTemplate *tmp = table_set_queue.top()->SearchEntry<SymbolTemplate>((yyvsp[-1].token_info).value.get<string>());
        string name = (yyvsp[-1].token_info).value.get<string>();
        (yyval.variable_node_info).name = new std::string((yyvsp[-1].token_info).value.get<string>());
        (yyval.variable_node_info).type_ptr = TYPE_ERROR;
        if(tmp == nullptr) {
            semantic_error(real_ast,"'"+name+ "' undeclared",(yyvsp[-1].token_info).line_num,(yyvsp[-1].token_info).column_num);
            break;
        } else {
            //类型检查
            (yyval.variable_node_info).is_lvalue = true;
            if (SymbolTemplate::SYMBOL_TYPE::CONST == tmp->symbol_type()){
                tmp = dynamic_cast<ConstSymbol*>(tmp);
                (yyval.variable_node_info).is_lvalue = false;
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
                (yyval.variable_node_info).is_lvalue = false;
                real_ast->libs()->Call(tmp->name());
            } else {
                if (tmp->type()->template_type() == TypeTemplate::TYPE::ARRAY && !error_flag){
                    std::vector<ArrayType::ArrayBound> bounds = dynamic_cast<ArrayType*>(tmp->type())->bounds();
                    (yyvsp[0].id_varparts_node_info).id_varparts_node->set_lb(bounds);
                }
            }
            (yyval.variable_node_info).type_ptr = (yyvsp[0].id_varparts_node_info).AccessCheck(tmp->type());
            if((yyval.variable_node_info).type_ptr==nullptr){
                string tn1 = type_name(tmp->type());
                if(tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::FUNCTION ||
                   tmp->symbol_type() == SymbolTemplate::SYMBOL_TYPE::CONST ||
                   tmp->type() == TYPE_ERROR ||
                   tmp->type()->template_type() == TypeTemplate::TYPE::BASIC){
                    if((yyvsp[0].id_varparts_node_info).var_parts != nullptr && (yyvsp[0].id_varparts_node_info).var_parts->size() != 0){
                    	semantic_error(real_ast,"requested value is nither array nor record (have '"+tn1+"')",line_count,0);
                    }
                }else{
                    string tn2;
                    std::vector<VarParts>* vp = (yyvsp[0].id_varparts_node_info).var_parts;
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

        (yyval.variable_node_info).variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode(name);
        (yyval.variable_node_info).variable_node->append_child(id_node);
        (yyval.variable_node_info).variable_node->append_child((yyvsp[0].id_varparts_node_info).id_varparts_node);
        for (auto i : *((yyvsp[0].id_varparts_node_info).var_parts)){
            delete i.subscript;
        }
        delete (yyvsp[0].id_varparts_node_info).var_parts;
    }
#line 2802 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 63: /* id_varparts: %empty  */
#line 1066 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // id_varparts -> empty.
        if(error_flag)
            break;
        (yyval.id_varparts_node_info).var_parts = new std::vector<VarParts>();
        if(error_flag)
            break;
        (yyval.id_varparts_node_info).id_varparts_node = new IDVarPartsNode();
    }
#line 2816 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 64: /* id_varparts: id_varparts id_varpart  */
#line 1076 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // id_varparts -> id_varparts id_varpart.
        if(error_flag)
            break;
        if((yyvsp[-1].id_varparts_node_info).var_parts){
            (yyval.id_varparts_node_info).var_parts = (yyvsp[-1].id_varparts_node_info).var_parts;
        } else {
            (yyval.id_varparts_node_info).var_parts = new std::vector<VarParts>();
        }
        
        (yyval.id_varparts_node_info).var_parts->push_back(*((yyvsp[0].id_varpart_node_info).var_part));

        (yyval.id_varparts_node_info).id_varparts_node = new IDVarPartsNode();
        (yyval.id_varparts_node_info).id_varparts_node->append_child((yyvsp[-1].id_varparts_node_info).id_varparts_node);
        (yyval.id_varparts_node_info).id_varparts_node->append_child((yyvsp[0].id_varpart_node_info).id_varpart_node);
        delete (yyvsp[0].id_varpart_node_info).var_part;
    }
#line 2838 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 65: /* id_varpart: '[' expression_list ']'  */
#line 1096 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // id_varpart -> [expression_list].
        (yyval.id_varpart_node_info).var_part= new VarParts();
        (yyval.id_varpart_node_info).var_part->flag = 0;//数组
        (yyval.id_varpart_node_info).var_part->subscript = (yyvsp[-1].expression_list_node_info).type_ptr_list;
        if(error_flag)
            break;
        (yyval.id_varpart_node_info).id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::EXP_LIST);
        (yyval.id_varpart_node_info).id_varpart_node->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        delete (yyvsp[-1].expression_list_node_info).is_lvalue_list;
    }
#line 2854 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 66: /* id_varpart: '.' ID  */
#line 1108 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // id_varpart -> .id.
        (yyval.id_varpart_node_info).var_part= new VarParts();
        (yyval.id_varpart_node_info).var_part->flag = 1;//结构体
        (yyval.id_varpart_node_info).var_part->name = (yyvsp[0].token_info).value.get<string>();
        if(error_flag)
            break;
        (yyval.id_varpart_node_info).id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::_ID);
        LeafNode *id_node = new LeafNode((yyvsp[0].token_info).value);
        (yyval.id_varpart_node_info).id_varpart_node->append_child(id_node);
    }
#line 2870 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 67: /* else_part: %empty  */
#line 1120 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // else_part -> empty.
        if(error_flag)
            break;
        (yyval.else_node) = new ElseNode(ElseNode::GrammarType::EPSILON);
    }
#line 2881 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 68: /* else_part: ELSE statement  */
#line 1127 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // else_part -> ELSE statement.
        if(error_flag)
            break;
        (yyval.else_node) = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        (yyval.else_node)->append_child((yyvsp[0].statement_node));
    }
#line 2893 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 69: /* updown: TO  */
#line 1136 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // updown -> TO.
        if(error_flag)
            break;
        (yyval.updown_node) = new UpdownNode(true);
    }
#line 2904 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 70: /* updown: DOWNTO  */
#line 1143 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // updown -> DOWNTO.
        if(error_flag)
            break;
        (yyval.updown_node) = new UpdownNode(false);
    }
#line 2915 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 71: /* call_procedure_statement: ID '(' expression_list ')'  */
#line 1151 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // call_procedure_statement -> id (expression_list).
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-3].token_info).value.get<string>());
        if(tmp == nullptr) {
            string tn = (yyvsp[-3].token_info).value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",(yyvsp[-3].token_info).line_num,(yyvsp[-3].token_info).column_num);
            break;
        }
        if(!tmp || !tmp->AssertParamsMatch(*((yyvsp[-1].expression_list_node_info).type_ptr_list),*((yyvsp[-1].expression_list_node_info).is_lvalue_list))){
            string tn = (yyvsp[-3].token_info).value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*((yyvsp[-1].expression_list_node_info).type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
        }
        if(error_flag)
            break;
        (yyval.procedure_call_node) = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode((yyvsp[-3].token_info).value);
        (yyval.procedure_call_node)->append_child(id_node);
        (yyval.procedure_call_node)->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        (yyvsp[-1].expression_list_node_info).expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete (yyvsp[-1].expression_list_node_info).is_lvalue_list;
        delete (yyvsp[-1].expression_list_node_info).type_ptr_list;
    }
#line 2951 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 72: /* call_procedure_statement: ID  */
#line 1183 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // call_procedure_statement -> id.
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[0].token_info).value.get<string>());
        if(tmp == nullptr) {
            string tn = (yyvsp[0].token_info).value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",(yyvsp[0].token_info).line_num,(yyvsp[0].token_info).column_num);
            break;
        } else {
            //函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertHasNoParams()){
                string tn = (yyvsp[0].token_info).value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too few arguments to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
            }
        }
        if(error_flag)
            break;
        (yyval.procedure_call_node) = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode((yyvsp[0].token_info).value);
        (yyval.procedure_call_node)->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    }
#line 2978 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 73: /* call_procedure_statement: ID '(' ')'  */
#line 1206 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // call_procedure_statement -> id().
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-2].token_info).value.get<string>());
        if(tmp == nullptr) {
            string tn = (yyvsp[-2].token_info).value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",(yyvsp[-2].token_info).line_num,(yyvsp[-2].token_info).column_num);
            break;
        } else {
            // 函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertHasNoParams()){
                string tn = (yyvsp[-2].token_info).value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too many arguments to procedure '"+tn+"' (expected '("+param+")')",(yyvsp[-2].token_info).line_num,0);
            }
        }
        if(error_flag)
            break;
        (yyval.procedure_call_node) = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode((yyvsp[-2].token_info).value);
        (yyval.procedure_call_node)->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    }
#line 3005 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 74: /* expression_list: expression_list ',' expression  */
#line 1230 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // expression_list -> expression_list ',' expression
        (yyval.expression_list_node_info).type_ptr_list = (yyvsp[-2].expression_list_node_info).type_ptr_list;
        (yyval.expression_list_node_info).type_ptr_list->push_back((yyvsp[0].expression_node_info).type_ptr);
        (yyval.expression_list_node_info).is_lvalue_list = (yyvsp[-2].expression_list_node_info).is_lvalue_list;
        (yyval.expression_list_node_info).is_lvalue_list->push_back((yyvsp[0].expression_node_info).is_lvalue);
        if(error_flag)
            break;
        (yyval.expression_list_node_info).expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[-2].expression_list_node_info).expression_list_node);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[0].expression_node_info).expression_node);
    }
#line 3022 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 75: /* expression_list: expression  */
#line 1243 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // expression_list -> expression
        (yyval.expression_list_node_info).type_ptr_list = new std::vector<TypeTemplate*>();
        (yyval.expression_list_node_info).type_ptr_list->push_back((yyvsp[0].expression_node_info).type_ptr);
        (yyval.expression_list_node_info).is_lvalue_list = new std::vector<bool>();
        (yyval.expression_list_node_info).is_lvalue_list->push_back((yyvsp[0].expression_node_info).is_lvalue);
        if(error_flag)
            break;
        (yyval.expression_list_node_info).expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[0].expression_node_info).expression_node);
    }
#line 3038 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 76: /* expression: simple_expression RELOP simple_expression  */
#line 1256 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // expression -> simple_expression RELOP simple_expression.
        if(error_flag)
            break;
        // 类型检查
        //从这里开始进行运算检查
        if((!((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->isSpecificBasicType()))||(!((yyvsp[0].simple_expression_node_info).type_ptr && (yyvsp[0].simple_expression_node_info).type_ptr->isSpecificBasicType())))
        {
            string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].simple_expression_node_info).type_ptr);
            string tn3 = (yyvsp[-1].token_info).value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
    auto result=((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[-2].simple_expression_node_info).type_ptr)->computeWith(static_cast<BasicType*>((yyvsp[0].simple_expression_node_info).type_ptr), (yyvsp[-1].token_info).value.get<string>()) : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
	    string tn2 = type_name((yyvsp[0].simple_expression_node_info).type_ptr);
	    string tn3 = (yyvsp[-1].token_info).value.get<string>();
	    semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        (yyval.expression_node_info).is_lvalue = false;
        (yyval.expression_node_info).type_ptr = result;
        
        std::string relop = (yyvsp[-1].token_info).value.get<string>();
        if((yyvsp[-1].token_info).value.get<string>() == "<>") {
            relop = "!=";
        }
        (yyval.expression_node_info).expression_node = new ExpressionNode();
        (yyval.expression_node_info).expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue(relop));
        (yyval.expression_node_info).expression_node->append_child(relop_node);
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
    }
#line 3076 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 77: /* expression: simple_expression '=' simple_expression  */
#line 1290 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // expression -> simple_expression '=' simple_expression.
        // 类型检查
        if((!((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->isSpecificBasicType()))||(!((yyvsp[0].simple_expression_node_info).type_ptr && (yyvsp[0].simple_expression_node_info).type_ptr->isSpecificBasicType()))){
           string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
           string tn2 = type_name((yyvsp[0].simple_expression_node_info).type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[-2].simple_expression_node_info).type_ptr)->computeWith(static_cast<BasicType*>((yyvsp[0].simple_expression_node_info).type_ptr), "=") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
           string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
           string tn2 = type_name((yyvsp[0].simple_expression_node_info).type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        (yyval.expression_node_info).is_lvalue = false;
        (yyval.expression_node_info).type_ptr = result;

        if(error_flag)
            break;
        (yyval.expression_node_info).expression_node = new ExpressionNode();
        (yyval.expression_node_info).expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue("=="));
        (yyval.expression_node_info).expression_node->append_child(relop_node);
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
    }
#line 3107 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 78: /* expression: simple_expression  */
#line 1317 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // expression -> simple_expression.
        (yyval.expression_node_info).type_ptr = (yyvsp[0].simple_expression_node_info).type_ptr;
        (yyval.expression_node_info).is_lvalue = (yyvsp[0].simple_expression_node_info).is_lvalue;
        if(error_flag)
            break;
        if((yyval.expression_node_info).type_ptr && (yyval.expression_node_info).type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
            (yyval.expression_node_info).expression_node = new ExpressionNode(ExpressionNode::TargetType::VAR_ARRAY);
        } else {
            (yyval.expression_node_info).expression_node = new ExpressionNode();
        }
        
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
    }
#line 3126 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 79: /* simple_expression: simple_expression ADDOP term  */
#line 1333 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // simple_expression -> simple_expression or term.
        //类型检查
        if((yyvsp[-2].simple_expression_node_info).type_ptr!=(yyvsp[0].term_node_info).type_ptr){
            string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].term_node_info).type_ptr);
            semantic_error(real_ast,"invalid operands to binary or (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        (yyval.simple_expression_node_info).is_lvalue = false;
        (yyval.simple_expression_node_info).type_ptr = (yyvsp[-2].simple_expression_node_info).type_ptr;

        if(error_flag)
            break;
        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *addop_node = new LeafNode(ConstValue("||"));
        (yyval.simple_expression_node_info).simple_expression_node->append_child(addop_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3150 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 80: /* simple_expression: simple_expression PLUS term  */
#line 1353 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    { 
        // 类型检查
        // simple_expression -> simple_expression + term.
        (yyval.simple_expression_node_info).is_lvalue = false;
        if(error_flag)
            break;
        if((!((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->isSpecificBasicType()))||(!((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->isSpecificBasicType())))
        {
           string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
           string tn2 = type_name((yyvsp[0].term_node_info).type_ptr);
           semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
           (yyval.simple_expression_node_info).type_ptr = (yyvsp[-2].simple_expression_node_info).type_ptr;
        }
        else{
            auto result=((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[-2].simple_expression_node_info).type_ptr)->computeWith(static_cast<BasicType*>((yyvsp[0].term_node_info).type_ptr), "+") : TYPE_ERROR);
            if(result==TYPE_ERROR){
                 string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
                 string tn2 = type_name((yyvsp[0].term_node_info).type_ptr);
                 semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
            }
            (yyval.simple_expression_node_info).type_ptr = result;
        }
        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        (yyval.simple_expression_node_info).simple_expression_node->append_child(plus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3183 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 81: /* simple_expression: simple_expression UMINUS term  */
#line 1382 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        (yyval.simple_expression_node_info).is_lvalue = false;
        if(error_flag)
            break;
        // 类型检查
        // simple_expression -> simple_expression - term.
         if((!((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->isSpecificBasicType()))||(!((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->isSpecificBasicType())))
         {
            string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].term_node_info).type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
auto result=((yyvsp[-2].simple_expression_node_info).type_ptr && (yyvsp[-2].simple_expression_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[-2].simple_expression_node_info).type_ptr)->computeWith(static_cast<BasicType*>((yyvsp[0].term_node_info).type_ptr), "-") : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name((yyvsp[-2].simple_expression_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].term_node_info).type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        (yyval.simple_expression_node_info).type_ptr = result;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        (yyval.simple_expression_node_info).simple_expression_node->append_child(minus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3214 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 82: /* simple_expression: term  */
#line 1409 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // simple_expression -> term.
        (yyval.simple_expression_node_info).type_ptr = (yyvsp[0].term_node_info).type_ptr;
        (yyval.simple_expression_node_info).is_lvalue = (yyvsp[0].term_node_info).is_lvalue;
        if(error_flag)
            break;
        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3228 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 83: /* simple_expression: PLUS term  */
#line 1419 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // simple_expression -> + term.
        //类型检查
        if(!((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->isSpecificBasicType())){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }

       auto result=((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[0].term_node_info).type_ptr)->computeUnary("-") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }
        (yyval.simple_expression_node_info).is_lvalue = false;
        (yyval.simple_expression_node_info).type_ptr = result;

        if(error_flag)
            break;
        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        (yyval.simple_expression_node_info).simple_expression_node->append_child(plus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3255 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 84: /* simple_expression: UMINUS term  */
#line 1442 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // simple_expression -> - term.
        //类型检查
        if(!((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->isSpecificBasicType()))
        {
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        auto result=((yyvsp[0].term_node_info).type_ptr && (yyvsp[0].term_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[0].term_node_info).type_ptr)->computeUnary("-") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        (yyval.simple_expression_node_info).is_lvalue = false;
        (yyval.simple_expression_node_info).type_ptr = result;
        if(error_flag)
            break;
        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        (yyval.simple_expression_node_info).simple_expression_node->append_child(minus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
    }
#line 3281 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 85: /* term: factor  */
#line 1465 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // term -> factor.
        (yyval.term_node_info).type_ptr = (yyvsp[0].factor_node_info).type_ptr;
        (yyval.term_node_info).is_lvalue = (yyvsp[0].factor_node_info).is_lvalue;
        if(error_flag)
            break;
        (yyval.term_node_info).term_node = new TermNode();
        (yyval.term_node_info).term_node->append_child((yyvsp[0].factor_node_info).factor_node);
    }
#line 3295 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 86: /* term: term MULOP factor  */
#line 1475 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {  
        // term -> term mulop factor.
        // 类型检查
        if((!((yyvsp[-2].term_node_info).type_ptr && (yyvsp[-2].term_node_info).type_ptr->isSpecificBasicType()))||(!((yyvsp[0].factor_node_info).type_ptr && (yyvsp[0].factor_node_info).type_ptr->isSpecificBasicType())))
        {
            string tn1 = type_name((yyvsp[-2].term_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].factor_node_info).type_ptr);
            string tn3 = (yyvsp[-1].token_info).value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=((yyvsp[-2].term_node_info).type_ptr && (yyvsp[-2].term_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[-2].term_node_info).type_ptr)->computeWith(static_cast<BasicType*>((yyvsp[0].factor_node_info).type_ptr), (yyvsp[-1].token_info).value.get<string>()) : TYPE_ERROR);
        if(result==TYPE_ERROR){
            string tn1 = type_name((yyvsp[-2].term_node_info).type_ptr);
            string tn2 = type_name((yyvsp[0].factor_node_info).type_ptr);
            string tn3 = (yyvsp[-1].token_info).value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        (yyval.term_node_info).is_lvalue = false;
        (yyval.term_node_info).type_ptr = result;
        
        std::string mulop = (yyvsp[-1].token_info).value.get<string>();
        if(mulop == "/" && !error_flag){
            (yyvsp[-2].term_node_info).term_node->set_op_div(true);
        } else if(mulop == "div"){
            mulop = "/";
        } else if (mulop == "mod"){
            mulop = "%";
        } else if (mulop == "and"){
            mulop = "&&";
        }
        if(error_flag)
            break;
        (yyval.term_node_info).term_node = new TermNode();
        (yyval.term_node_info).term_node->append_child((yyvsp[-2].term_node_info).term_node);
        LeafNode *mulop_node = new LeafNode(ConstValue(mulop));
        (yyval.term_node_info).term_node->append_child(mulop_node);
        (yyval.term_node_info).term_node->append_child((yyvsp[0].factor_node_info).factor_node);
    }
#line 3338 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 87: /* factor: unsigned_const_variable  */
#line 1515 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // factor -> unsigned_const_variable.
        (yyval.factor_node_info).type_ptr = (yyvsp[0].unsigned_constant_var_node_info).type_ptr;
        (yyval.factor_node_info).is_lvalue = false;
        if(error_flag)
            break;
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::UCONST_VAR);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].unsigned_constant_var_node_info).unsigned_constant_var_node);
    }
#line 3352 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 88: /* factor: variable  */
#line 1525 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // factor -> variable.
        (yyval.factor_node_info).type_ptr = (yyvsp[0].variable_node_info).type_ptr;
        (yyval.factor_node_info).is_lvalue = (yyvsp[0].variable_node_info).is_lvalue;
        if(error_flag)
            break;
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::VARIABLE);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].variable_node_info).variable_node);
        if((yyvsp[0].variable_node_info).name) delete (yyvsp[0].variable_node_info).name;
    }
#line 3367 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 89: /* factor: ID '(' expression_list ')'  */
#line 1536 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        if(error_flag)
            break;
        (yyval.factor_node_info).is_lvalue = false;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>((yyvsp[-3].token_info).value.get<string>());
        if(tmp == nullptr) {
            semantic_error(real_ast,"undefined function '" + (yyvsp[-3].token_info).value.get<string>() + "'",(yyvsp[-3].token_info).line_num,(yyvsp[-3].token_info).column_num);
            break;
        }else if(!tmp->AssertParamsMatch(*((yyvsp[-1].expression_list_node_info).type_ptr_list),*((yyvsp[-1].expression_list_node_info).is_lvalue_list))){
            string tn = (yyvsp[-3].token_info).value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*((yyvsp[-1].expression_list_node_info).type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to function '"+tn+"' (expected '("+param+")')",line_count,0);
            break;
        }
        //if(error_flag)
        //   break;
        (yyval.factor_node_info).type_ptr = tmp->type();
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode((yyvsp[-3].token_info).value);
        (yyval.factor_node_info).factor_node->append_child(id_node);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        (yyvsp[-1].expression_list_node_info).expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete (yyvsp[-1].expression_list_node_info).type_ptr_list;
        delete (yyvsp[-1].expression_list_node_info).is_lvalue_list;

    }
#line 3406 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 90: /* factor: '(' expression ')'  */
#line 1571 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // factor -> (expression).
        (yyval.factor_node_info).type_ptr = (yyvsp[-1].expression_node_info).type_ptr;
        (yyval.factor_node_info).is_lvalue = false;
        if(error_flag)
            break;
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::EXP);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[-1].expression_node_info).expression_node);
    }
#line 3420 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 91: /* factor: NOT factor  */
#line 1581 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // factor -> not factor.
        // 类型检查
        if(!((yyvsp[0].factor_node_info).type_ptr && (yyvsp[0].factor_node_info).type_ptr->isSpecificBasicType()))
        {
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
auto result=((yyvsp[0].factor_node_info).type_ptr && (yyvsp[0].factor_node_info).type_ptr->template_type() == TypeTemplate::TYPE::BASIC ? static_cast<BasicType*>((yyvsp[0].factor_node_info).type_ptr)->computeUnary("not") : TYPE_ERROR);
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
        (yyval.factor_node_info).is_lvalue = false;
        (yyval.factor_node_info).type_ptr = result;

        if(error_flag)
            break;
        if(result==TYPE_BOOL){
            (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::NOT_BOOL);
        }
         if(result==TYPE_INT){
            (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::NOT_INT);
        }
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].factor_node_info).factor_node);
    }
#line 3450 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 92: /* unsigned_const_variable: num  */
#line 1608 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // unsigned_const_variable -> num
        (yyval.unsigned_constant_var_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        if(error_flag)
            break;
        LeafNode *num_node = new LeafNode((yyvsp[0].value_node_info).value);
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(num_node);
    }
#line 3464 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 93: /* unsigned_const_variable: CHAR  */
#line 1618 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // unsigned_const_variable -> 'LETTER'
        (yyval.unsigned_constant_var_node_info).type_ptr = TYPE_CHAR;
        if(error_flag)
            break;
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode((yyvsp[0].token_info).value);
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(char_node);
    }
#line 3478 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 94: /* unsigned_const_variable: TRUE  */
#line 1628 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // unsigned_const_variable -> true
        (yyval.unsigned_constant_var_node_info).type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(ConstValue(true));
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(true_node);
    }
#line 3492 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 95: /* unsigned_const_variable: FALSE  */
#line 1638 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {   
        // unsigned_const_variable -> false
        (yyval.unsigned_constant_var_node_info).type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(ConstValue(false));
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(false_node);
    }
#line 3506 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 96: /* program: program_head program_body error  */
#line 1651 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // program -> program_head program_body
        // 缺少程序末尾的 '.'
        location_pointer_refresh();
        yyerror(real_ast,"expected '.' at the end of the program");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
#line 3519 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 97: /* program_head: PROGRAM ID '(' error ')' ';'  */
#line 1661 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // program -> PROGRAM ID ( error ) ;
        // program 声明后的参数格式错误
        // 例如：program main(555);
        location_pointer_refresh();
        yyerror(real_ast,"invalid argument expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        char *new_location_pointer = location_pointer + 2;
        fprintf(stderr,"\t| %s",new_location_pointer);
    }
#line 3534 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 98: /* const_declaration: ID const_variable  */
#line 1673 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // const_declaration -> ID const_variable
        // const 表达式缺少 '='
        // 例如：const a 5;
        location_pointer_refresh();
        yyerror(real_ast, "expected '=' between identifier and value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
#line 3548 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 99: /* const_declaration: const_declaration ';' ID const_variable  */
#line 1683 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // const_declaration -> const_declaration ; ID const_variable
        // 缺少 '='
        location_pointer_refresh();
        yyerror(real_ast, "expected '=' between identifier and value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
#line 3561 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 100: /* const_declaration: ID '=' error  */
#line 1692 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        // const_declaration -> ID = error
        // 表达式右侧无效
        // 例如：const a = c;
        location_pointer_refresh();
        yyerror(real_ast, "invalid constant expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
#line 3575 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;

  case 101: /* statement_list: variable error  */
#line 1703 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"
    {
        location_pointer_refresh();
        yyerror(real_ast, "expected ':=' between variable and expression");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);        
    }
#line 3586 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"
    break;


#line 3590 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.tab.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (real_ast, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, real_ast);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, real_ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (real_ast, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, real_ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, real_ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1709 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"


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
