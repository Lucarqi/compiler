%{
#include <string>
#include "ast/node.hpp"
#include "parser.hpp"

void yyerror(const char *s);
int yycolumn = 1;
#define TOKEN(X) (yylval.token = X)
#define SAVE_TOKEN (yylval.string = new std::string(yytext,yyleng))
//每次一个Pattern被匹配时运行,且优先级高于默认的action，处理location信息
#define YY_USER_ACTION  yylloc.first_line = yylineno;           \
                        yylloc.first_column = yycolumn;         \
                        for( auto i:std::string(yytext_ptr)){   \
                            if(i == '\n')                       \
                            {
                                yylineno++;
                                yycolumn = 1;
                            }else {                             \
                                yycolumn ++;                    \
                            }                                   \
                        }                                       \
                        yylloc.last_line = yylineno;            \
                        yylloc.last_column = yycolumn - 1;        
%}

%option noyywrap

%%
\/\/.*\n?               ;
"/*"                    {
                            int c;
                            while((c == yyinput())! = NULL)
                            {
                                if(c == "\n")
                                {
                                    yylineno++;
                                    yycolumn = 1;
                                }
                                else if(c == "*")
                                {
                                    if((c == yyinput()) == "/") break;
                                    else continue;
                                }
                            }
                        }   
[ ]                     ;
\n                      ;
[\t\r]                  ;
"if"                    return TOKEN(IF);
"else"                  return TOKEN(ELSE);
"while"                 return TOKEN(WHILE);
"break"                 return TOKEN(BREAK);
"continue"              return TOKEN(CONTINUE);
"const"                 return TOKEN(CONST);
"return"                return TOKEN(RETURN);
"int"                   return TOKEN(INT);
"void"                  return TOKEN(VOID);

"("                     return TOKEN(LPARENT);
")"                     return TOKEN(RPARENT);
"["                     return TOKEN(LBRACKET);
"]"                     return TOKEN(RBRACKET);
"{"                     return TOKEN(LBRACE);
"}"                     return TOKEN(RBRACE);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN;return IDENTIFIER;
[0-9]+                  SAVE_TOKEN;return INT_VALUE;
"0"[xX][0-9a-fA-F]+     SAVE_TOKEN;return INT_VALUE;

"="                     return TOKEN(ASSIGN);
"=="                    return TOKEN(EQ);
"!="                    return TOKEN(NQ);
"<"                     return TOKEN(LT);
"<="                    return TOKEN(LEQ);
">"                     return TOKEN(GT);
">="                    return TOKEN(GEQ);
"&&"                    return TOKEN(AND);
"||"                    return TOKEN(OR);

";"                     return TOKEN(SEMI);
","                     return TOKEN(COMMA);

"++"                    return TOKEN(PLUSPLUS);
"--"                    return TOKEN(MINUSMINUS);

"+"                     return TOKEN(PLUS);
"-"                     return TOKEN(MINUS);
"*"                     return TOKEN(MUL);
"/"                     return TOKEN(DIV);
"%"                     return TOKEN(MOD);
"!"                     return TOKEN(NOT);
.                       yyerror((std::string("Unknown token:")+yytext).c_str());yyterminate();
%%
