//
//  def.h
//
//
//  Created by Ilaria Martinelli on 07/05/14.
//
//

#ifndef _def_h
#define _def_h

typedef union{
  int intval;
  double realval;
  char *stringval;
  enum {FALSE, TRUE} bval;
} Value;


#define CHAR	256
#define INT		257
#define REAL	258
#define STRING	259
#define BOOL	260

#define STRUCT	261
#define VECTOR	262
#define OF		263

#define TYPE	264
#define VAR		265
#define CONST	266

#define FUNC	267
#define TOINT	268
#define TOREAL	269
#define BEGIN	301
#define END		302

#define REL_OP	270
#define EQ		271
#define NE		272
#define LT		273
#define LE		274
#define GT		275
#define GE		276
#define IN 		277

#define LOG_OP	278
#define AND		279
#define OR		280
#define NOT		281

#define IF		282
#define THEN	283
#define ELSE	284
#define ELSIF	285
#define ENDIF	286

#define WHILE	287
#define DO		288
#define ENDWHILE 289

#define FOR 	290
#define TO 		291
#define ENDFOR 	292

#define FOREACH 293
#define ENDFOREACH 294

#define RETURN	295

#define READ	296
#define RD 		297

#define WRITE 	298
#define WR 		299

#define ASSIGN	300
//302

#define ERROR	303


#define INT_CONST       304
#define REAL_CONST      305
#define CHAR_CONST      306
#define STRING_CONST    307
#define BOOL_CONST      308


//#define TRUE	
//#define FALSE

#endif