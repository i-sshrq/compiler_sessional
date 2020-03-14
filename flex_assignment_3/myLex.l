%option noyywrap
%option yylineno


%{
    #include <stdio.h>
    #include <string.h>
    int word_count = 0;
%}

delim           [ \t\n]
comment         [/][/].*
whitespace      {delim}+
digit           [0-9]
digits          {digit}+
letter          [A-Za-z]
number          {digits}(.{digits})?(E[+-]?{digits})?
id              _?{letter}+{letter}*_*{digit}*_*{letter}*
parenthesis     [)(]
curlybraces     [}{]
relop           [><]?=?
assignop        =
arithop         [-+*/%]+=?
comma           ,
semicolon       ;
colon           :
str             \".*\"

%%
{whitespace}    { }
{comment}       { }
{semicolon}     {fprintf(yyout, "<SEMICOLON, ;>\tLine num = %d\n", yylineno);}
{colon}         {fprintf(yyout, "<COLON, :>\tLine num = %d\n", yylineno);}
exit            { return 0; }
if              {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
else            {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
else\ if        {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
for             {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
while           {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
switch          {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
case            {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
int             {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
float           {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
char            {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
double          {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
break           {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
default         {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
fprintf          {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
scanf           {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}
main            {fprintf(yyout, "<KEYWORD, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}


{str}           {
                    int len = strlen(yytext);
                    char * withoutQuotes = malloc(sizeof(char)*(len+1));
                    strcpy(withoutQuotes, yytext);
                    withoutQuotes[len-1] = '\0';
                    withoutQuotes = withoutQuotes + 1;
                    fprintf(yyout, "<STRINGLITERAL, %s>\tLine num = %d\n", withoutQuotes, yylineno); 
                    word_count++;
                }

{id}            {fprintf(yyout, "<ID, %s>\tLine num = %d\n", yytext, yylineno);  word_count++;}
{number}        {fprintf(yyout, "<NUMBER, %s>\tLine num = %d\n", yytext, yylineno); word_count++;}

{parenthesis}   {fprintf(yyout, "<PAREN, %s>\tLine num = %d\n", yytext, yylineno);}
{curlybraces}   {fprintf(yyout, "<CURL, %s>\tLine num = %d\n", yytext, yylineno);}
{relop}         {fprintf(yyout, "<RELOP, %s>\tLine num = %d\n", yytext, yylineno);}
{arithop}       {fprintf(yyout, "<ARITHMETIC, %s>\tLine num = %d\n", yytext, yylineno);}
{assignop}      {fprintf(yyout, "<ASSIGNOP, %s>\tLine num = %d\n", yytext, yylineno);}

{comma}         {fprintf(yyout, "<COMMA, ,>\tLine num = %d\n", yylineno);}

&&              {fprintf(yyout, "<LOGICALOP, %s>\tLine num = %d\n", "&&", yylineno);}
\|\|            {fprintf(yyout, "<LOGICALOP, %s>\tLine num = %d\n", "||", yylineno);}
!               {fprintf(yyout, "<LOGICALOP, %s>\tLine num = %d\n", "!", yylineno);}



%%
int main(){
    yyin = fopen("input.txt", "r");
    if(!yyin){
        yyin = stdin;
    }
    yyout = fopen("output.txt", "w");
    yylex();
    fclose(yyin);
    fclose(yyout);
}