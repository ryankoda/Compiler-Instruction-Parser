# Compiler/Instruction Parser
Implementation of a small compiler
The grammar is as follows:

program		->	var_section body
var_section	->	id_list SEMICOLON
id_list		->	ID COMMA id_list | ID
body		->	LBRACE stmt_list RBRACE
stmt_list	->	stmt stmt_list | stmt
stmt		->	assign_stmt | print_stmt | while_stmt | if_stmt | switch_stmt | for_stmt
assign_stmt ->	ID EQUAL primary SEMICOLON
assign_stmt ->	ID EQUAL expr SEMICOLON
expr		->	primary op primary
primary		->	ID | NUM
op			->	PLUS | MINUS | MULT | DIV
print_stmt	->	print ID SEMICOLON
while_stmt	->	WHILE condition body
if_stmt		->	IF condition body
condition	->	primary relop primary
relop		->	GREATER | LESS | NOTEQUAL
switch_stmt	->	SWITCH ID LBRACE case_list RBRACE
switch_stmt ->	SWITCH ID LRBACE case_list default_case RBRACE
for_stmt	->	FOR LPAREN assign_stmt condition SEMICOLON assign_stmt RPAREN body
case_list	->	case case_list | case
case		->	CASE NUM COLON BODY
default_case->	DEFAULT COLON BODY
