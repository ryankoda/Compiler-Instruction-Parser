#include "compiler_structure.h"
#include "lexicon.h"

extern struct StatementNode *parse_program();
extern void parse_var_section();
extern struct StatementNode *parse_body();
extern struct StatementNode *parse_stmt_list();
extern struct StatementNode *parse_stmt();
extern struct StatementNode *parse_assign_stmt();
extern struct StatementNode *parse_print_stmt();
extern struct StatementNode *parse_if_stmt();
extern struct StatementNode *parse_while_stmt();
extern struct StatementNode *parse_switch_stmt();
extern struct StatementNode *parse_for_stmt();
extern void parse_case_list(int target, struct StatementNode **stmt);
extern void parse_case(int target, struct StatementNode **stmt);
extern void parse_default_case(struct StatementNode *stmt);