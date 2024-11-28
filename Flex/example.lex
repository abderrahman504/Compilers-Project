

%{

#include <stdio.h>


int yywrap() { return 1; }


%}
letter 	[a-zA-Z]

digit  	[0-9]

digits 	[0-9]+

%%

boolean|int|float|if|else|while 		{printf("%s\n", yytext);}

{letter}({letter}|{digit})* 		{printf("id: %s\n", yytext);}

{digit}+|{digit}+\.{digits}(E{digits})?			{printf("num\n");}


==|!=|>=|<|<=		{printf("relop\n");}

=		{printf("assign\n");}

[;,\(\)\{\} \n]		{printf("%s\n", yytext);}

\+|\-		{printf("addop\n");}

[*/]		{printf("mulop\n");}

%%


int main(int argc, char **argv){
	char *in_file = argv[0];
	FILE *fh = fopen(in_file, "r");
	if (fh == NULL){
		printf("Couldn't open file %s\n", in_file);
		return 0;
	}
	yyin = fh;

	yylex();
	return 0;
}

