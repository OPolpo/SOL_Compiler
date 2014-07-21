/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CHAR = 258,
     INT = 259,
     REAL = 260,
     STRING = 261,
     BOOL = 262,
     STRUCT = 263,
     VECTOR = 264,
     OF = 265,
     TYPE = 266,
     VAR = 267,
     CONST = 268,
     FUNC = 269,
     TOINT = 270,
     TOREAL = 271,
     F_BEGIN = 272,
     F_END = 273,
     REL_OP = 274,
     EQ = 275,
     NE = 276,
     LE = 277,
     GE = 278,
     IN = 279,
     LOG_OP = 280,
     AND = 281,
     OR = 282,
     NOT = 283,
     IF = 284,
     THEN = 285,
     ELSE = 286,
     ELSIF = 287,
     ENDIF = 288,
     WHILE = 289,
     DO = 290,
     ENDWHILE = 291,
     FOR = 292,
     TO = 293,
     ENDFOR = 294,
     FOREACH = 295,
     ENDFOREACH = 296,
     RETURN = 297,
     READ = 298,
     RD = 299,
     WRITE = 300,
     WR = 301,
     ASSIGN = 302,
     ERROR = 303,
     INTCONST = 304,
     REALCONST = 305,
     CHARCONST = 306,
     STRCONST = 307,
     BOOLCONST = 308,
     ID = 309,
     UMINUS = 310
   };
#endif
/* Tokens.  */
#define CHAR 258
#define INT 259
#define REAL 260
#define STRING 261
#define BOOL 262
#define STRUCT 263
#define VECTOR 264
#define OF 265
#define TYPE 266
#define VAR 267
#define CONST 268
#define FUNC 269
#define TOINT 270
#define TOREAL 271
#define F_BEGIN 272
#define F_END 273
#define REL_OP 274
#define EQ 275
#define NE 276
#define LE 277
#define GE 278
#define IN 279
#define LOG_OP 280
#define AND 281
#define OR 282
#define NOT 283
#define IF 284
#define THEN 285
#define ELSE 286
#define ELSIF 287
#define ENDIF 288
#define WHILE 289
#define DO 290
#define ENDWHILE 291
#define FOR 292
#define TO 293
#define ENDFOR 294
#define FOREACH 295
#define ENDFOREACH 296
#define RETURN 297
#define READ 298
#define RD 299
#define WRITE 300
#define WR 301
#define ASSIGN 302
#define ERROR 303
#define INTCONST 304
#define REALCONST 305
#define CHARCONST 306
#define STRCONST 307
#define BOOLCONST 308
#define ID 309
#define UMINUS 310




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

