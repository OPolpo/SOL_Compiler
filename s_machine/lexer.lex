%{
#include <stdlib.h>
//#include "def.h"
//#include "parser.h"
#include "../s_compiler/code_gen.h"

Value lexval;
unsigned int line = 1;
%}
%option	noyywrap


spacing		([ \t])+
eol			\n
letter		[a-zA-Z]
digit		[0-9]
num 		{digit}+
charconst 	\'.\'
intconst 	-?{num}
realconst	-?{num}?"."{num}
stringconst \"([^\"])*\"

%%

SCODE			{return (S_SCODE);}
PUSH			{return (S_PUSH);}
GOTO			{return (S_GOTO);}
POP				{return (S_POP);}
NEWS			{return (S_NEWS);}
NEW				{return (S_NEW);}
LDC				{return (S_LDC);}
LDI				{return (S_LDI);}
LDS				{return (S_LDS);}
LDR				{return (S_LDR);}
LOD				{return (S_LOD);}
CAT				{return (S_CAT);}
LDA				{return (S_LDA);}
FDA				{return (S_FDA);}
EIL				{return (S_EIL);}
SIL				{return (S_SIL);}
IXA				{return (S_IXA);}
STO				{return (S_STO);}
IST				{return (S_IST);}
JMF				{return (S_JMF);}
JMP				{return (S_JMP);}
EQU				{return (S_EQU);}
NEQ				{return (S_NEQ);}
CGT				{return (S_CGT);}
IGT				{return (S_IGT);}
RGT				{return (S_RGT);}
SGT				{return (S_SGT);}
CGE				{return (S_CGE);}
IGE				{return (S_IGE);}
RGE				{return (S_RGE);}
SGE				{return (S_SGE);}
CLT				{return (S_CLT);}
ILT				{return (S_ILT);}
RLT				{return (S_RLT);}
SLT				{return (S_SLT);}
CLE				{return (S_CLE);}
ILE				{return (S_ILE);}
RLE				{return (S_RLE);}
SLE				{return (S_SLE);}
IN				{return (S_IN);}
IPLUS			{return (S_IPLUS);}
RPLUS			{return (S_RPLUS);}
IMINUS			{return (S_IMINUS);}
RMINUS			{return (S_RMINUS);}
ITIMES			{return (S_ITIMES);}
RTIMES			{return (S_RTIMES);}
IDIV			{return (S_IDIV);}
RDIV			{return (S_RDIV);}
IUMI			{return (S_IUMI);}
RUMI			{return (S_RUMI);}
NEG				{return (S_NEG);}
FWR				{return (S_FWR);}
WR				{return (S_WR);}
FRD				{return (S_FRD);}
RD				{return (S_RD);}
TOINT			{return (S_TOINT);}
TOREAL			{return (S_TOREAL);}
FREAD			{return (S_FREAD);}
READ			{return (S_READ);}
FWRITE			{return (S_FWRITE);}
WRITE			{return (S_WRITE);}
FUNC			{return (S_FUNC);}
RETURN			{return (S_RETURN);}
HALT			{return (S_HALT);}


{spacing}		;
{eol}			;
{intconst}		{return (0);}
{realconst}		{return (0);}
{charconst}		{return (0);}
{stringconst}	{return (0);}
.				{return (0);}

%%
