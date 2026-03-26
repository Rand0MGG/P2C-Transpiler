/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MNT_D_BUPT_INTERNET_REF_P2C-Transpiler_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_MNT_D_BUPT_INTERNET_REF_P2C-Transpiler_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../src/parser.y"

  #include "parser.h"   /* 你的大 struct 定义 */

#line 53 "/mnt/d/bupt/internet_ref/P2C-Transpiler/build/../include/parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROGRAM = 258,                 /* PROGRAM  */
    FUNCTION = 259,                /* FUNCTION  */
    PROCEDURE = 260,               /* PROCEDURE  */
    TO = 261,                      /* TO  */
    DOWNTO = 262,                  /* DOWNTO  */
    ARRAY = 263,                   /* ARRAY  */
    CONST = 264,                   /* CONST  */
    RECORD = 265,                  /* RECORD  */
    IF = 266,                      /* IF  */
    THEN = 267,                    /* THEN  */
    ELSE = 268,                    /* ELSE  */
    OF = 269,                      /* OF  */
    WHILE = 270,                   /* WHILE  */
    DO = 271,                      /* DO  */
    FOR = 272,                     /* FOR  */
    BEGIN_ = 273,                  /* BEGIN_  */
    END = 274,                     /* END  */
    ADDOP = 275,                   /* ADDOP  */
    NOT = 276,                     /* NOT  */
    PLUS = 277,                    /* PLUS  */
    UMINUS = 278,                  /* UMINUS  */
    CONSTASSIGNOP = 279,           /* CONSTASSIGNOP  */
    ID = 280,                      /* ID  */
    CHAR = 281,                    /* CHAR  */
    INT_NUM = 282,                 /* INT_NUM  */
    REAL_NUM = 283,                /* REAL_NUM  */
    BASIC_TYPE = 284,              /* BASIC_TYPE  */
    RELOP = 285,                   /* RELOP  */
    MULOP = 286,                   /* MULOP  */
    STRING_ = 287,                 /* STRING_  */
    VAR = 288,                     /* VAR  */
    SUBCATALOG = 289,              /* SUBCATALOG  */
    ASSIGNOP = 290,                /* ASSIGNOP  */
    WRITE = 291,                   /* WRITE  */
    SEP = 292,                     /* SEP  */
    READ = 293,                    /* READ  */
    TRUE = 294,                    /* TRUE  */
    FALSE = 295                    /* FALSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  YYSTYPE  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (AST *real_ast);


#endif /* !YY_YY_MNT_D_BUPT_INTERNET_REF_P2C-Transpiler_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED  */
