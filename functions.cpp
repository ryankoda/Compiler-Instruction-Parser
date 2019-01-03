#include "functions.h"
#include <iostream>

using namespace std;

vector<ValueNode> table(2);
LexicalAnalyzer lexer;

struct StatementNode *parse_program()
{
	table.clear();
	struct StatementNode *program = NULL;
	Token k;
	k = lexer.GetToken();
	if (k.token_type == ID)
	{
		lexer.UngetToken(k);
		parse_var_section();
		program = parse_body();
	}
	/*while (program != NULL)
	{
		cout << program->type << " ";
		cout << "\n";
	}*/
	return program;
}

void parse_var_section()
{
	while (1)
	{
		struct ValueNode new_symbol;
		Token k = lexer.GetToken();
		if (k.token_type != ID)
		{
			if (k.token_type == COMMA)
				continue;
			else if (k.token_type == SEMICOLON)
				break;
			else
				exit(1);
		}
		new_symbol.name = k.lexeme;
		new_symbol.value = 0;
		table.push_back(new_symbol);
	}
}

struct StatementNode *parse_body()
{
	struct StatementNode *stmt_list = NULL;
	Token k = lexer.GetToken();
	if (k.token_type == LBRACE)
	{
		k = lexer.GetToken();
		if (k.token_type == ID || k.token_type == PRINT || k.token_type == WHILE || k.token_type == IF || k.token_type == SWITCH || k.token_type == FOR)
		{
			lexer.UngetToken(k);
			stmt_list = parse_stmt_list();
			//cout << stmt_list->assign_stmt->operand2->value << "\n";
			k = lexer.GetToken();
			if (k.token_type != RBRACE)
				exit(1);
		}
	}
	else
		exit(1);
	/*while (stmt_list != NULL)
	{
		cout << stmt_list->type << "\n";
		stmt_list = stmt_list->next;
	}*/
	return stmt_list;
}

struct StatementNode *parse_stmt_list()
{
	struct StatementNode *stmt = NULL;
	struct StatementNode *stmt_list = NULL;
	Token k = lexer.GetToken();
	Token temp = k;
	lexer.UngetToken(k);
	stmt = parse_stmt();
	//cout << stmt->assign_stmt->operand2->value << "\n";
	//cout << k.token_type << "\n";
	k = lexer.GetToken();
	//cout << k.token_type << "\n";
	//cout << k.token_type << "\n"; //SEMICOLON
	if (k.token_type == ID || k.token_type == PRINT || k.token_type == WHILE || k.token_type == IF || k.token_type == SWITCH || k.token_type == FOR)
	{
		lexer.UngetToken(k);
		//cout << k.token_type << "\n";
		stmt_list = parse_stmt_list();
		if ((stmt->type == IF_STMT) && (temp.token_type == SWITCH))
		{
			struct StatementNode *switch_head = stmt;
			while (1)
			{
				if (switch_head->type == NOOP_STMT)
					break;
				switch_head = switch_head->next;
			}
			switch_head->next = stmt_list;
		}
		else if (stmt->type == IF_STMT)
			stmt->next->next = stmt_list;
		else
			stmt->next = stmt_list;

		return stmt;
	}
	else
	{
		/*while (stmt != NULL)
		{
			cout << stmt->type << "\n";
			stmt = stmt->next;
		}*/
		lexer.UngetToken(k);
		return stmt;
	}
	return stmt;
}

struct StatementNode *parse_stmt()
{
	struct StatementNode *stmt = NULL;
	Token k = lexer.GetToken();
	if (k.token_type == ID)
	{
		lexer.UngetToken(k);
		stmt = parse_assign_stmt();
	}
	else if (k.token_type == PRINT)
		stmt = parse_print_stmt();
	else if (k.token_type == WHILE)
		stmt = parse_while_stmt();
	else if (k.token_type == IF)
		stmt = parse_if_stmt();
	else if (k.token_type == SWITCH)
		stmt = parse_switch_stmt();
	else if (k.token_type == FOR)
		stmt = parse_for_stmt();
	else
		exit(1);
	//cout << stmt->assign_stmt->left_hand_side->name << " " << stmt->assign_stmt->left_hand_side->value << "\n";
	//cout << stmt->assign_stmt->operand1->value << " " << stmt->assign_stmt->op << " " << stmt->assign_stmt->operand2->value << "\n";
	return stmt;
}

struct StatementNode *parse_assign_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	Token k = lexer.GetToken();
	//cout << k.lexeme << "\n";
	if (k.token_type == ID)
	{
		//lexer.UngetToken(k);
		stmt->type = ASSIGN_STMT;
		stmt->assign_stmt = (struct AssignmentStatement*)malloc(sizeof(struct AssignmentStatement));
		int target = -1;
		//Check to see if k.lexeme is in the symbol table
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				//cout << table[i].name << "\n";
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			/*temp->name = g_sym_tab[target].name;
			stmt->assign_stmt->left_hand_side = temp;*/
			//stmt->assign_stmt->left_hand_side = &(g_sym_tab[target]);
			struct ValueNode *temp = (struct ValueNode*)malloc(sizeof(ValueNode));
			stmt->assign_stmt->left_hand_side = (struct ValueNode*)malloc(sizeof(struct ValueNode));
			//cout << temp->name << " " << temp->value << "\n";

			stmt->assign_stmt->left_hand_side = &(table[target]);
			//cout << stmt->assign_stmt->left_hand_side->name << " " << stmt->assign_stmt->left_hand_side->value << "\n";
			k = lexer.GetToken();
			if (k.token_type == EQUAL)
			{
				k = lexer.GetToken();
				if (k.token_type == ID || k.token_type == NUM)
				{
					/*struct ValueNode *temp = { 0 };
					temp->name = k.
					stmt->assign_stmt->operand1 =*/
					if (k.token_type == ID)
					{
						int target = -1;
						for (int i = 0; i < table.size(); i++)
						{
							//if (table[i].name.compare(k.lexeme) == 0)
							if (table[i].name == k.lexeme)
							{
								target = i;
								break;
							}
						}
						if (target != -1)
						{
							stmt->assign_stmt->operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
							stmt->assign_stmt->operand1 = &(table[target]);
							//stmt->assign_stmt->operand2 = NULL;
							stmt->assign_stmt->op = OPERATOR_NONE;
							stmt->assign_stmt->operand2 = NULL;
						}
						else
							exit(1);
					}
					else if (k.token_type == NUM)
					{
						//cout << k.lexeme << "\n";
						int i;
						i = stoi(k.lexeme);
						stmt->assign_stmt->operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
						stmt->assign_stmt->operand1->value = i;
						//cout << stmt->assign_stmt->operand1->value << "\n";
						stmt->assign_stmt->op = OPERATOR_NONE;
						stmt->assign_stmt->operand2 = NULL;
					}
				}
				else
					exit(1);
				k = lexer.GetToken();
				if (k.token_type == PLUS || k.token_type == MINUS || k.token_type == MULT || k.token_type == DIV)
				{
					//cout << k.token_type << "\n";
					//FILL OUT STMT->ASSIGN_STMT->OPERAND2
					if (k.token_type == PLUS)
						stmt->assign_stmt->op = OPERATOR_PLUS;
					else if (k.token_type == MINUS)
						stmt->assign_stmt->op = OPERATOR_MINUS;
					else if (k.token_type == MULT)
						stmt->assign_stmt->op = OPERATOR_MULT;
					else if (k.token_type == DIV)
						stmt->assign_stmt->op = OPERATOR_DIV;
					k = lexer.GetToken();
					if (k.token_type == ID)
					{
						int target = -1;
						for (int i = 0; i < table.size(); i++)
						{
							//if (table[i].name.compare(k.lexeme) == 0)
							if (table[i].name == k.lexeme)
							{
								target = i;
								break;
							}
						}
						if (target != -1)
						{
							stmt->assign_stmt->operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
							stmt->assign_stmt->operand2 = &(table[target]);
							//cout << stmt->assign_stmt->operand2->value << "\n";
						}
						else
							exit(1);
					}
					else if (k.token_type == NUM)
					{

						//stmt->assign_stmt->operand2->value = atoi(k.lexeme.c_str());
						stmt->assign_stmt->operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
						stmt->assign_stmt->operand2->value = stoi(k.lexeme);
						//cout << stmt->assign_stmt->operand2->value << "\n";
					}
					else
						exit(1);
					k = lexer.GetToken();

				}
				else if (k.token_type != SEMICOLON)
					exit(1);
			}
			else
				exit(1);
		}
		else
			exit(1);
	}
	else
		exit(1);
	//cout << stmt->assign_stmt->left_hand_side->name << " " << stmt->assign_stmt->left_hand_side->value << "\n";
	//cout << stmt->assign_stmt->operand1->value << " " << stmt->assign_stmt->op << " " << stmt->assign_stmt->operand2->value << "\n";
	//cout << k.token_type << "\n";
	return stmt;
}

struct StatementNode *parse_print_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	Token k = lexer.GetToken();
	if (k.token_type == ID)
	{
		struct PrintStatement *print_stmt = (struct PrintStatement*)malloc(sizeof(struct PrintStatement));
		stmt->type = PRINT_STMT;
		stmt->print_stmt = print_stmt;
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
			stmt->print_stmt->id = &(table[target]);
		else
			exit(1);
		k = lexer.GetToken();
		if (k.token_type != SEMICOLON)
			exit(1);
	}
	else
		exit(1);
	return stmt;
}

struct StatementNode *parse_if_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	struct IfStatement *tempIf = (struct IfStatement*)malloc(sizeof(struct IfStatement));
	stmt->type = IF_STMT;
	stmt->if_stmt = tempIf;
	Token k = lexer.GetToken();
	if (k.token_type == ID)
	{
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			tempIf->condition_operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
			tempIf->condition_operand1 = &(table[target]);
		}
		else
			exit(1);
	}
	else if (k.token_type == NUM)
	{
		tempIf->condition_operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
		tempIf->condition_operand1->value = stoi(k.lexeme);
	}
	else
		exit(1);
	k = lexer.GetToken();
	if (k.token_type == GREATER || k.token_type == LESS || k.token_type == NOTEQUAL)
	{
		if (k.token_type == GREATER)
			tempIf->condition_op = CONDITION_GREATER;
		else if (k.token_type == LESS)
			tempIf->condition_op = CONDITION_LESS;
		else if (k.token_type == NOTEQUAL)
			tempIf->condition_op = CONDITION_NOTEQUAL;
	}
	else
		exit(1);

	k = lexer.GetToken();
	if (k.token_type == ID)
	{
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			tempIf->condition_operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
			tempIf->condition_operand2 = &(table[target]);
		}
		else
			exit(1);
	}
	else if (k.token_type == NUM)
	{
		tempIf->condition_operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
		tempIf->condition_operand2->value = stoi(k.lexeme);
	}
	else
		exit(1);
	tempIf->true_branch = parse_body();
	struct StatementNode *no_op_stmt = (struct StatementNode*)malloc(sizeof(StatementNode));
	no_op_stmt->type = NOOP_STMT;
	struct StatementNode *head = tempIf->true_branch;
	while (1)
	{
		if (head->next == NULL)
			break;
		head = head->next;
	}
	head->next = no_op_stmt;
	tempIf->false_branch = no_op_stmt;
	stmt->next = no_op_stmt;
	return stmt;
}

struct StatementNode *parse_for_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	//struct IfStatement *tempIf = (struct IfStatement*)malloc(sizeof(struct IfStatement));
	struct AssignmentStatement *assign = (struct AssignmentStatement*)malloc(sizeof(struct AssignmentStatement));
	//stmt->type = ASSIGN_STMT;
	//stmt->assign_stmt = assign;
	Token k = lexer.GetToken();
	//cout << k.token_type << "\n";
	if (k.token_type == LPAREN)
	{
		k = lexer.GetToken();
		//cout << k.token_type << "\n";
		if (k.token_type == ID)
		{
			lexer.UngetToken(k);
			//stmt = parse_assign_stmt();
			stmt = parse_assign_stmt();
			k = lexer.GetToken();
			//cout << stmt->assign_stmt->left_hand_side->name << "\n";
			//stmt->
		}
		else
			exit(1);
	}
	else
		exit(1);
}

struct StatementNode *parse_while_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	struct IfStatement *tempIf = (struct IfStatement*)malloc(sizeof(struct IfStatement));
	stmt->type = IF_STMT;
	stmt->if_stmt = tempIf;
	Token k = lexer.GetToken();
	if (k.token_type == ID)
	{
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			tempIf->condition_operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
			tempIf->condition_operand1 = &(table[target]);
		}
		else
			exit(1);
	}
	else if (k.token_type == NUM)
	{
		tempIf->condition_operand1 = (struct ValueNode*)malloc(sizeof(ValueNode));
		tempIf->condition_operand1->value = stoi(k.lexeme);
	}
	else
		exit(1);
	k = lexer.GetToken();
	if (k.token_type == GREATER || k.token_type == LESS || k.token_type == NOTEQUAL)
	{
		if (k.token_type == GREATER)
			tempIf->condition_op = CONDITION_GREATER;
		else if (k.token_type == LESS)
			tempIf->condition_op = CONDITION_LESS;
		else if (k.token_type == NOTEQUAL)
			tempIf->condition_op = CONDITION_NOTEQUAL;
	}
	else
		exit(1);

	k = lexer.GetToken();
	if (k.token_type == ID)
	{
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			tempIf->condition_operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
			tempIf->condition_operand2 = &(table[target]);
		}
		else
			exit(1);
	}
	else if (k.token_type == NUM)
	{
		tempIf->condition_operand2 = (struct ValueNode*)malloc(sizeof(ValueNode));
		tempIf->condition_operand2->value = stoi(k.lexeme);
	}
	else
		exit(1);
	tempIf->true_branch = parse_body();
	struct StatementNode *go_stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	struct GotoStatement *go_node = (struct GotoStatement*)malloc(sizeof(struct GotoStatement));
	go_stmt->type = GOTO_STMT;
	go_node->target = stmt;
	go_stmt->goto_stmt = go_node;
	struct StatementNode *head = tempIf->true_branch;
	while (1)
	{
		if (head->next == NULL)
			break;
		head = head->next;
	}
	head->next = go_stmt;
	struct StatementNode *no_op_stmt = (struct StatementNode*)malloc(sizeof(StatementNode));
	no_op_stmt->type = NOOP_STMT;
	tempIf->false_branch = no_op_stmt;
	stmt->next = no_op_stmt;
	return stmt;
}

struct StatementNode *parse_switch_stmt()
{
	struct StatementNode *stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
	//struct IfStatement *tempif = (struct IfStatement*)malloc(sizeof(struct IfStatement));
	//struct ValueNode temp; 
	Token k = lexer.GetToken();
	//cout << table[0].value << "\n";
	if (k.token_type == ID)
	{
		int target = -1;
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].name == k.lexeme)
			{
				target = i;
				break;
			}
		}
		if (target != -1)
		{
			//tempif->condition_operand1 = (struct ValueNode*)malloc(sizeof(struct ValueNode));
			//tempif->condition_operand1 = &(table[target]);
			//temp.name = k.lexeme;
			k = lexer.GetToken();
			if (k.token_type == LBRACE)
			{
				stmt->type = NOOP_STMT;
				k = lexer.GetToken();
				lexer.UngetToken(k);
				if (k.token_type == CASE)
				{
					parse_case_list(target, &stmt);
				}
				else
					exit(1);
				k = lexer.GetToken();
				if (k.token_type == DEFAULT)
					parse_default_case(stmt);
				else
					lexer.UngetToken(k);
				k = lexer.GetToken();
				if (k.token_type != RBRACE)
					exit(1);
			}
			else
				exit(1);
		}
		else
			exit(1);
	}
	else
		exit(1);
	return stmt;
}

void parse_case_list(int target, struct StatementNode **stmt)
{
	Token k = lexer.GetToken();
	if (k.token_type == CASE)
	{
		parse_case(target, stmt);
		k = lexer.GetToken();
		lexer.UngetToken(k);
		if (k.token_type == CASE)
			parse_case_list(target, stmt);
	}
}

void parse_case(int target, struct StatementNode **stmt)
{
	struct StatementNode *noop_stmt = NULL;
	struct StatementNode *bef_noop_stmt = NULL;
	struct StatementNode *stmt_head = *stmt;
	while (1)
	{
		if (stmt_head->next == NULL)
			break;
		if (stmt_head->next->next == NULL)
			break;
		stmt_head = stmt_head->next;
	}
	if (stmt_head->next == NULL)
		noop_stmt = stmt_head;
	else
	{
		bef_noop_stmt = stmt_head;
		noop_stmt = stmt_head->next;
	}

	Token k = lexer.GetToken();
	if (k.token_type == NUM)
	{
		struct ValueNode temp;
		temp.value = stoi(k.lexeme);
		//table.push_back(temp);
		//cout << table.size() << "\n";
		//cout << table[0].name << " " << table[0].value << "\n";
		k = lexer.GetToken();
		if (k.token_type == COLON)
		{
			struct StatementNode *if_stmt = (struct StatementNode*)malloc(sizeof(struct StatementNode));
			struct IfStatement *if_node = (struct IfStatement*)malloc(sizeof(struct IfStatement));
			if_stmt->type = IF_STMT;
			if_stmt->if_stmt = if_node;
			if_node->condition_operand1 = &(table[target]);
			//cout << table[target].name << "\n";
			//cout << table[target].value << "\n";
			//cout << "\n" << if_node->condition_operand1->name << "\n";
			if_node->condition_operand2 = (struct ValueNode*)malloc(sizeof(struct ValueNode));
			if_node->condition_operand2 = &temp;
			//cout << temp.value << "\n";
			if_node->condition_op = CONDITION_NOTEQUAL;
			if_node->false_branch = parse_body();

			struct StatementNode *false_branch_head = if_node->false_branch;
			while (1)
			{
				if (false_branch_head->next == NULL)
					break;
				false_branch_head = false_branch_head->next;
			}
			false_branch_head->next = noop_stmt;
			if (bef_noop_stmt != NULL)
			{
				bef_noop_stmt->next = if_stmt;
				if (bef_noop_stmt->type == IF_STMT)
					bef_noop_stmt->if_stmt->true_branch = if_stmt;
				else
					exit(1);
			}
			else
				*stmt = if_stmt;
			if_node->true_branch = noop_stmt;
			if_stmt->next = noop_stmt;
		}
		else
			exit(1);
	}
	else
		exit(1);
}

void parse_default_case(struct StatementNode *stmt)
{
	struct StatementNode *noop_stmt = NULL;
	struct StatementNode *bef_noop_stmt = NULL;
	struct StatementNode *stmt_head = stmt;
	while (1)
	{
		if (stmt_head->next->next == NULL)
			break;
		stmt_head = stmt_head->next;
	}
	bef_noop_stmt = stmt_head;
	noop_stmt = stmt_head->next;
	Token k = lexer.GetToken();
	if (k.token_type == COLON)
	{
		struct StatementNode *def_stmt_list = parse_body();
		struct StatementNode *def_stmt_head = def_stmt_list;
		if (bef_noop_stmt->type == IF_STMT)
			bef_noop_stmt->if_stmt->true_branch = def_stmt_list;
		else
			exit(1);
		while (1)
		{
			if (def_stmt_head->next == NULL)
				break;
			def_stmt_head = def_stmt_head->next;
		}
		def_stmt_head->next = noop_stmt;
	}
	else
		exit(1);
}



struct StatementNode *parse_generate_intermediate_representation()
{
	struct StatementNode *program = parse_program();
	return program;
}