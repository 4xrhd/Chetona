%{
#include <stdio.h>
#include <string.h>
%}

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*
STRING   \"[^"]*\"

%%

"aromvho"       { printf("KEYWORD: aromvho (main)\n"); }
"shomapti"      { printf("KEYWORD: shomapti (return)\n"); }
"dhoro"         { printf("KEYWORD: dhoro (auto)\n"); }
"var"           { printf("KEYWORD: var (auto)\n"); }
"shorto"        { printf("KEYWORD: shorto (if)\n"); }
"natuva"        { printf("KEYWORD: natuva (else if)\n"); }
"nahole"        { printf("KEYWORD: nahole (else if)\n"); }
"onno_thay"     { printf("KEYWORD: onno_thay (else)\n"); }
"ghurnon"       { printf("KEYWORD: ghurnon (while)\n"); }
"jotokhon"      { printf("KEYWORD: jotokhon (for)\n"); }
"shotto"        { printf("LITERAL: shotto (true)\n"); }
"mithya"        { printf("LITERAL: mithya (false)\n"); }

"shonkolon"     { printf("FUNCTION: shonkolon (add)\n"); }
"biyojon"       { printf("FUNCTION: biyojon (subtract)\n"); }
"gunon"         { printf("FUNCTION: gunon (multiply)\n"); }
"vabhon"        { printf("FUNCTION: vabhon (divide)\n"); }
"mod"           { printf("FUNCTION: mod (modulo)\n"); }

"shomota"       { printf("FUNCTION: shomota (equal)\n"); }
"boro"          { printf("FUNCTION: boro (greater)\n"); }
"choto"         { printf("FUNCTION: choto (less)\n"); }

"prodorshon"    { printf("IO: prodorshon (print)\n"); }
"grohon"        { printf("IO: grohon (input)\n"); }
"grohon_shobdo" { printf("IO: grohon_shobdo (string input)\n"); }
"grohon_shonkhya" { printf("IO: grohon_shonkhya (number input)\n"); }

{DIGIT}+("."{DIGIT}+)?   { printf("NUMBER: %s\n", yytext); }
{STRING}                 { printf("STRING: %s\n", yytext); }
{ID}                     { printf("IDENTIFIER: %s\n", yytext); }

"=="    { printf("OPERATOR: ==\n"); }
"!="    { printf("OPERATOR: !=\n"); }
"<="    { printf("OPERATOR: <=\n"); }
">="    { printf("OPERATOR: >=\n"); }
"<"     { printf("OPERATOR: <\n"); }
">"     { printf("OPERATOR: >\n"); }
"+"     { printf("OPERATOR: +\n"); }
"-"     { printf("OPERATOR: -\n"); }
"*"     { printf("OPERATOR: *\n"); }
"/"     { printf("OPERATOR: /\n"); }
"%"     { printf("OPERATOR: %%\n"); }
"="     { printf("OPERATOR: =\n"); }

"("     { printf("DELIMITER: (\n"); }
")"     { printf("DELIMITER: )\n"); }
"{"     { printf("DELIMITER: {\n"); }
"}"     { printf("DELIMITER: }\n"); }
";"     { printf("DELIMITER: ;\n"); }
","     { printf("DELIMITER: ,\n"); }

"//".*            { /* Skip single-line comment */ }
[ \t\r\n]+        { /* Skip whitespace */ }

.       { printf("UNKNOWN: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    yyin = input;
    
    printf("\n=== Chetona Lexer Output ===\n\n");
    yylex();
    printf("\n=== End of Output ===\n");
    
    fclose(input);
    return 0;
}
