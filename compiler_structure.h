#pragma once
#include <string>

struct ValueNode
{
	std::string name;
	int value;
};

struct GotoStatement
{
	struct StatementNode * target;
};

enum ArithmeticOperatorType {
	OPERATOR_NONE = 123,
	OPERATOR_PLUS,
	OPERATOR_MINUS,
	OPERATOR_MULT,
	OPERATOR_DIV
};

struct AssignmentStatement
{
	struct ValueNode * left_hand_side;
	struct ValueNode * operand1;
	struct ValueNode * operand2;
	ArithmeticOperatorType op;
};

struct PrintStatement
{
	struct ValueNode * id;
};

enum ConditionalOperatorType {
	CONDITION_GREATER = 345,
	CONDITION_LESS,
	CONDITION_NOTEQUAL
};

struct IfStatement
{
	struct ValueNode * condition_operand1;
	struct ValueNode * condition_operand2;
	ConditionalOperatorType condition_op;
	struct StatementNode * true_branch;
	struct StatementNode * false_branch;
};

enum StatementType
{
	NOOP_STMT = 1000,
	PRINT_STMT,
	ASSIGN_STMT,
	IF_STMT,
	GOTO_STMT
};

struct StatementNode
{
	StatementType type;
	union
	{
		struct AssignmentStatement * assign_stmt;
		struct PrintStatement * print_stmt;
		struct IfStatement * if_stmt;
		struct GotoStatement * goto_stmt;
	};

	struct StatementNode * next;
};

void debug(const char* format, ...);
struct StatementNode * parse_generate_intermediate_representation();