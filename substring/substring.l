%{
#include<stdio.h>
%}

%option caseless

%%
.*sthu.* { printf("Rejected\n"); }
.*  { printf("Accepted\n"); }
\n { printf("Enter the string: "); }
%%

int yywrap() { return 1; }

int main()
{
    printf("The substring hardcoded to check is sthu.\n");
    printf("Enter the string: ");
    yylex();   // start analysis
    return 0;
}

