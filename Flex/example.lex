

%{

#include <stdio.h>

%}

letter 	[a-zA-Z]

digit  	[0-9]

digits 	[0-9]+

%%

boolean|int|float|if|else|while 		{printf("%s\n", yytext);}

{letter}({letter}|{digit})* 		{printf("id\n");}

{digit}+|{digit}+\.{digits}(E{digits})?			{printf("num\n");}


==|!=|>=|<|<=		{printf("relop\n");}

=		{printf("assign\n");}

[;,\(\)\{\}]		{printf("%s\n", yytext);}

\+|\-		{printf("addop\n");}

[*/]		{printf("mulop\n");}

[ \n] 		{ }

%%


int main(int argc, char **argv){
	
	char *in_file = argv[1];
	FILE *fh = fopen(in_file, "r");
	if (fh == NULL){
		printf("Couldn't open file %s\n", in_file);
		return 0;
	}
	yyin = fh;

	yylex();
	return 0;
}

