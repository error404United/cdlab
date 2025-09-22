/*lex code to determine whether input is an identifier or not*/
%option noyywrap
%%
^[a-zA-Z_][a-zA-Z0-9_]* printf("Valid Identifier");
^[^a-zA-Z _]            printf("Invalid Identifier");
.*                      printf("Invalid Indentifier");
%%

main()
{
        yylex();
}
