%option noyywrap
%option yylineno
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "parser.h"
%}

DIGIT_R   -?[0-9]+
KEYWORD_R \:[a-zA-Z0-9\+-\?]+
SYMBOL_R  [a-zA-Z0-9\+-\?]+
STRING_R  [\"]([^\"\\\n]|\\.|\\\n)*[\"]

%%
"("			{ return L_OPENP;		}
")"			{ return L_CLOSEP;		}
"{"			{ return L_OPENMAP;		}
"}"			{ return L_CLOSEMAP;		}
\.			{ return L_DOT;			} 
{DIGIT_R}		{ yylval.l_number = atoi(yytext);  return L_NUMBER;		}
{KEYWORD_R}	{ yylval.l_keyword = yytext;  return L_KEYWORD;		}
{SYMBOL_R}	{ yylval.l_symbol = yytext;  return L_SYMBOL;		}
{STRING_R} { yylval.l_string = yytext;  return L_STRING;        }
\\\\                    { return L_COMMENTOPEN;         }
(\r\n|\r|\n)		{ }
[ \t]			{ }
<<EOF>>	{return L_EOF;}
%%
