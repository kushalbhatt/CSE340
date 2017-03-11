/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Rida Bazzi, 2015, Adam Doupe, 2015
 ----------------------------------------------------------------------------*/
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include<map>
#include<unordered_map>
#include<algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "syntax.h"

/* ------------------------------------------------------- */
/* -------------------- LEXER SECTION -------------------- */
/* ------------------------------------------------------- */
using namespace std;
#define KEYWORDS  11

typedef enum
{
    END_OF_FILE = -1, VAR = 1, WHILE, INT, REAL, STRING, BOOLEAN,
    TYPE, LONG, DO, CASE, SWITCH,
    PLUS, MINUS, DIV, MULT, EQUAL,
    COLON, COMMA, SEMICOLON,
    LBRAC, RBRAC, LPAREN, RPAREN, LBRACE, RBRACE,
    NOTEQUAL, GREATER, LESS, LTEQ, GTEQ, DOT,
    ID, NUM, REALNUM,
    ERROR
} token_type;

const char *reserved[] = {"",
    "VAR", "WHILE", "INT", "REAL", "STRING", "BOOLEAN",
    "TYPE", "LONG", "DO", "CASE", "SWITCH",
    "+", "-", "/", "*", "=",
    ":", ",", ";",
    "[", "]", "(", ")", "{", "}",
    "<>", ">", "<", "<=", ">=", ".",
    "ID", "NUM", "REALNUM",
    "ERROR"
};

// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100
char token[MAX_TOKEN_LENGTH]; // token string
token_type t_type; // token type
bool activeToken = false;
int tokenLength;
int line_no = 1;
int line=1; // for error reporting

//keys for unknown variables U keeps track of how many are used so far
string unknown_types[30]={"my1","my2","my3","my4","my5","my6","my7","my8","my9","my10","my11",
"my12","my13","my14","my15","my16","my17","my18","my19","my20","my21","my22","my23","my24","my25","my26","my27","my28","my29","my30"};
int u=0;

void skipSpace()
{
    char c;

    c = getchar();
    line_no += (c == '\n');
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        line_no += (c == '\n');
    }
    ungetc(c, stdin);
}

int isKeyword(char *s)
{
    int i;

    for (i = 1; i <= KEYWORDS; i++)
    {
        if (strcmp(reserved[i], s) == 0)
        {
            return i;
        }
    }
    return false;
}

/*
 * ungetToken() simply sets a flag so that when getToken() is called
 * the old t_type is returned and the old token is not overwritten.
 * NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
 * AT LEAST ONE CALL TO getToken(). CALLING TWO ungetToken() WILL NOT
 * UNGET TWO TOKENS
 */
void ungetToken()
{
    activeToken = true;
}

token_type scan_number()
{
    char c;

    c = getchar();
    if (isdigit(c))
    {
        // First collect leading digits before dot
        // 0 is a NUM by itself
        if (c == '0')
        {
            token[tokenLength] = c;
            tokenLength++;
            token[tokenLength] = '\0';
        }
        else
        {
            while (isdigit(c))
            {
                token[tokenLength] = c;
                tokenLength++;
                c = getchar();
            }
            ungetc(c, stdin);
            token[tokenLength] = '\0';
        }
        // Check if leading digits are integer part of a REALNUM
        c = getchar();
        if (c == '.')
        {
            c = getchar();
            if (isdigit(c))
            {
                token[tokenLength] = '.';
                tokenLength++;
                while (isdigit(c))
                {
                    token[tokenLength] = c;
                    tokenLength++;
                    c = getchar();
                }
                token[tokenLength] = '\0';
                if (!feof(stdin))
                {
                    ungetc(c, stdin);
                }
                return REALNUM;
            }
            else
            {
                ungetc(c, stdin);
                c = '.';
                ungetc(c, stdin);
                return NUM;
            }
        }
        else
        {
            ungetc(c, stdin);
            return NUM;
        }
    }
    else
    {
        return ERROR;
    }
}

token_type scan_id_or_keyword()
{
    token_type the_type;
    int k;
    char c;

    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c))
        {
            token[tokenLength] = c;
            tokenLength++;
            c = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(c, stdin);
        }
        token[tokenLength] = '\0';
        k = isKeyword(token);
        if (k == 0)
        {
            the_type = ID;
        }
        else
        {
            the_type = (token_type) k;
        }
        return the_type;
    }
    else
    {
        return ERROR;
    }
}

token_type getToken()
{
    char c;

    if (activeToken)
    {
        activeToken = false;
        return t_type;
    }
    skipSpace();
    tokenLength = 0;
    c = getchar();
    switch (c)
    {
        case '.': return DOT;
        case '+': return PLUS;
        case '-': return MINUS;
        case '/': return DIV;
        case '*': return MULT;
        case '=': return EQUAL;
        case ':': return COLON;
        case ',': return COMMA;
        case ';': return SEMICOLON;
        case '[': return LBRAC;
        case ']': return RBRAC;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '{': return LBRACE;
        case '}': return RBRACE;
        case '<':
            c = getchar();
            if (c == '=')
            {
                return LTEQ;
            }
            else if (c == '>')
            {
                return NOTEQUAL;
            }
            else
            {
                ungetc(c, stdin);
                return LESS;
            }
        case '>':
            c = getchar();
            if (c == '=')
            {
                return GTEQ;
            }
            else
            {
                ungetc(c, stdin);
                return GREATER;
            }
        default:
            if (isdigit(c))
            {
                ungetc(c, stdin);
                return scan_number();
            }
            else if (isalpha(c))
            {
                ungetc(c, stdin);
                return scan_id_or_keyword();
            }
            else if (c == EOF)
            {
                return END_OF_FILE;
            }
            else
            {
                return ERROR;
            }
    }
}

/* ----------------------------------------------------------------- */
/* -------------------- SYNTAX ANALYSIS SECTION -------------------- */
/* ----------------------------------------------------------------- */

void syntax_error(const char* msg)
{
    printf("Syntax error while parsing %s line %d\n", msg, line_no);
    exit(1);
}

/* -------------------- PRINTING PARSE TREE -------------------- */
void print_parse_tree(struct programNode* program)
{
    print_decl(program->decl);
    print_body(program->body);
}

void print_decl(struct declNode* dec)
{
    if (dec->type_decl_section != NULL)
    {
        print_type_decl_section(dec->type_decl_section);
    }
    if (dec->var_decl_section != NULL)
    {
        print_var_decl_section(dec->var_decl_section);
    }
}

void print_body(struct bodyNode* body)
{
    printf("{\n");
    print_stmt_list(body->stmt_list);
    printf("}\n");
}

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection)
{
    printf("VAR\n");
    if (varDeclSection->var_decl_list != NULL)
    {
        print_var_decl_list(varDeclSection->var_decl_list);
    }
}

void print_var_decl_list(struct var_decl_listNode* varDeclList)
{
    print_var_decl(varDeclList->var_decl);
    if (varDeclList->var_decl_list != NULL)
    {
        print_var_decl_list(varDeclList->var_decl_list);
    }
}

void print_var_decl(struct var_declNode* varDecl)
{
    print_id_list(varDecl->id_list);
    printf(": ");
    print_type_name(varDecl->type_name);
    printf(";\n");
}

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection)
{
    printf("TYPE\n");
    if (typeDeclSection->type_decl_list != NULL)
    {
        print_type_decl_list(typeDeclSection->type_decl_list);
    }
}

void print_type_decl_list(struct type_decl_listNode* typeDeclList)
{
    print_type_decl(typeDeclList->type_decl);
    if (typeDeclList->type_decl_list != NULL)
    {
        print_type_decl_list(typeDeclList->type_decl_list);
    }
}

void print_type_decl(struct type_declNode* typeDecl)
{
    print_id_list(typeDecl->id_list);
    printf(": ");
    print_type_name(typeDecl->type_name);
    printf(";\n");
}

void print_type_name(struct type_nameNode* typeName)
{
    if (typeName->type != ID)
    {
        printf("%s ", reserved[typeName->type]);
    }
    else
    {
        printf("%s ", typeName->id);
    }
}

void print_id_list(struct id_listNode* idList)
{
    printf("%s ", idList->id);
    if (idList->id_list != NULL)
    {
        printf(", ");
        print_id_list(idList->id_list);
    }
}

void print_stmt_list(struct stmt_listNode* stmt_list)
{
    print_stmt(stmt_list->stmt);
    if (stmt_list->stmt_list != NULL)
    {
        print_stmt_list(stmt_list->stmt_list);
    }

}

void print_assign_stmt(struct assign_stmtNode* assign_stmt)
{
    printf("%s ", assign_stmt->id);
    printf("= ");
    print_expression_prefix(assign_stmt->expr);
    printf("; \n");
}

void print_stmt(struct stmtNode* stmt)
{
    switch (stmt->stmtType)
    {
        case ASSIGN:
            print_assign_stmt(stmt->assign_stmt);
            break;
        case WHILE:
            print_while_stmt(stmt->while_stmt);
            break;
        case DO:
            print_do_stmt(stmt->while_stmt);
            break;
        case SWITCH:
            print_switch_stmt(stmt->switch_stmt);
            break;
    }
}

void print_expression_prefix(struct exprNode* expr)
{
    if (expr->tag == EXPR)
    {
        printf("%s ", reserved[expr->op]);
        print_expression_prefix(expr->leftOperand);
        print_expression_prefix(expr->rightOperand);
    }
    else if (expr->tag == PRIMARY)
    {
        if (expr->primary->tag == ID)
        {
            printf("%s ", expr->primary->id);
        }
        else if (expr->primary->tag == NUM)
        {
            printf("%d ", expr->primary->ival);
        }
        else if (expr->primary->tag == REALNUM)
        {
            printf("%.4f ", expr->primary->fval);
        }
    }
}

void print_while_stmt(struct while_stmtNode* while_stmt)
{
    // TODO: implement this for your own debugging purposes
}

void print_do_stmt(struct while_stmtNode* do_stmt)
{
    // TODO: implement this for your own debugging purposes
}

void print_condition(struct conditionNode* condition)
{
    // TODO: implement this for your own debugging purposes
}

void print_case(struct caseNode* cas)
{
    // TODO: implement this for your own debugging purposes
}

void print_case_list(struct case_listNode* case_list)
{
    // TODO: implement this for your own debugging purposes
}

void print_switch_stmt(struct switch_stmtNode* switc)
{
    // TODO: implement this for your own debugging purposes
}

/* -------------------- PARSING AND BUILDING PARSE TREE -------------------- */

// Note that the following function is not
// called case because case is a keyword in C/C++
struct caseNode* cas()
{
    // TODO: implement this for extra credit
        struct caseNode* case_node;
    t_type=getToken();
    
    if(t_type == CASE)
    {
        case_node=ALLOC(struct caseNode);
        t_type=getToken();
        if(t_type == NUM)
        {
            case_node->num=atoi(token);
            t_type=getToken();    
            if(t_type == COLON)
            {
                case_node->body = body();
                return case_node;
            }
            else
                syntax_error("Case. COLON expected!");            
        }
        else
            syntax_error("Case. NUM expected!");
    }
    else
        syntax_error("Case. Expected!");
    return NULL;
}

struct case_listNode* case_list()
{
    // TODO: implement this for extra credit
    struct case_listNode* case_ls;
    t_type=getToken();
    
    if(t_type == CASE)
    {
        ungetToken();
        case_ls=ALLOC(struct case_listNode);
        case_ls->cas=cas();
        t_type=getToken();
    
        if(t_type == CASE)
        {
            ungetToken();
            case_ls->case_list=case_list();          
        }
        else if(t_type==RBRACE)
        { 
            ungetToken();          
            case_ls->case_list=NULL;            
        }
        else
        {
            syntax_error("case list. CASE or RBRACE expected!");
        }
        return case_ls;
    }
    else
    {
        syntax_error("Case List. Expected!");
    }
    return NULL;
}

struct switch_stmtNode* switch_stmt()
{
    // TODO: implement this for extra credit
    struct switch_stmtNode* switch_node;
    t_type=getToken();
    
    if(t_type == SWITCH)
    {
        switch_node=ALLOC(struct switch_stmtNode);
        t_type=getToken();
        if(t_type == ID)
        {
            switch_node->id = strdup(token);
            t_type=getToken();
            if(t_type == LBRACE)
            {
                switch_node->case_list=case_list();
                
                t_type=getToken();
                if(t_type == RBRACE)
                    return switch_node;
                else
                    syntax_error("Switch. RBRACE Expected!");
            }
            else
                syntax_error("Switch. LBRACE Expected!");
        }
        else
        {
            syntax_error("switch. ID expected!");
        }
    }
    else
    {
        syntax_error("SWITCH Expected!");
    }
    return NULL;
}

struct while_stmtNode* do_stmt()
{
    // TODO: implement this
    struct while_stmtNode* whilestmt;
    t_type=getToken();
    
    if(t_type == DO)
    {
        whilestmt=ALLOC(struct while_stmtNode);           
        whilestmt->body = body();
        t_type=getToken();
        if(t_type == WHILE)
        {
            whilestmt->condition = condition();
            t_type=getToken();
            
            if(t_type == SEMICOLON)
                return whilestmt;             
            else
                syntax_error("Do. SEMICOLON expected after do while!");
        }
        else
        {
            syntax_error("Do. WHILE expected after do - body -!");
        }
    }
    else
    {
        syntax_error("Do body while expected");
    }
    return NULL;
}

struct primaryNode* primary()
{
    // TODO: implement this
    struct primaryNode* prim;
    t_type=getToken();
    
    if(t_type == ID || t_type == NUM || t_type == REALNUM)
    {
        prim=ALLOC(struct primaryNode);
        prim->tag = t_type;
        if(t_type ==  ID)
        {
            prim->id= strdup(token);
        }
        else if(t_type == NUM)
        {
            prim->ival= atoi(token);
        }
        else
        {
            prim->ival= atof(token);
        }
        return prim;
    }
    else
    {
          syntax_error("Primary. ID, NUM or REALNUM expected");
    }
    return NULL;
}

struct conditionNode* condition()
{
    // TODO: implement this
    struct conditionNode* cond;
    t_type=getToken();
    
    if(t_type == ID || t_type == NUM || t_type == REALNUM)
    {
        cond=ALLOC(struct conditionNode);
        ungetToken();
        cond->left_operand = primary();
        
        t_type=getToken();
        
        if(t_type == GREATER || t_type== NOTEQUAL || t_type== LESS || t_type== LTEQ || t_type== GTEQ) 
        {
            cond->relop = t_type;
            
            cond->right_operand=primary();
            return cond;
        }
        else if(t_type == SEMICOLON || t_type == LBRACE)
        {
            ungetToken();  //let do_stmt or WHILE take care of this
            cond->right_operand=NULL;
            return cond;
        }
        else
        {
            syntax_error("Condition. RELOP expected");
        }
    }
    else
    {
        syntax_error("Condition. NUM, REALNUM, or ID, expected");
    }
    return NULL;
}

struct while_stmtNode* while_stmt()
{
    // TODO: implement this
    struct while_stmtNode* whilestmt;
    t_type=getToken();
    
    if(t_type == WHILE)
    {
        t_type=getToken();
        if(t_type == ID || t_type == NUM || t_type == REALNUM) //condition
        {
            whilestmt = ALLOC(struct while_stmtNode);
            ungetToken();
            whilestmt->condition = condition();
            whilestmt->body = body();
            return whilestmt;
        }
        else
        {
            syntax_error("WHILE. NUM, REALNUM, or ID, expected in condition");
        }
        
    }//end of outer if
    
    else
    {
        syntax_error("factor. NUM, REALNUM, or ID, expected");
    }
    return NULL; // control never reaches here, this is just for the sake of GCC    
}

struct exprNode* factor()
{
    struct exprNode* facto;

    t_type = getToken();
    if (t_type == LPAREN)
    {
        facto = expr();
        t_type = getToken();
        if (t_type == RPAREN)
        {
            return facto;
        }
        else
        {
            syntax_error("factor. RPAREN expected");
        }
    }
    else if (t_type == NUM)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = NUM;
        facto->primary->ival = atoi(token);
        return facto;
    }
    else if (t_type == REALNUM)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = REALNUM;
        facto->primary->fval = atof(token);
        return facto;
    }
    else if (t_type == ID)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = ID;
        facto->primary->id = strdup(token);
        return facto;
    }
    else
    {
        syntax_error("factor. NUM, REALNUM, or ID, expected");
    }
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct exprNode* term()
{
    struct exprNode* ter;
    struct exprNode* f;

    t_type = getToken();
    if (t_type == ID || t_type == LPAREN || t_type == NUM || t_type == REALNUM)
    {
        ungetToken();
        f = factor();
        t_type = getToken();
        if (t_type == MULT || t_type == DIV)
        {
            ter = ALLOC(struct exprNode);
            ter->op = t_type;
            ter->leftOperand = f;
            ter->rightOperand = term();
            ter->tag = EXPR;
            ter->primary = NULL;
            return ter;
        }
        else if (t_type == SEMICOLON || t_type == PLUS ||
                 t_type == MINUS || t_type == RPAREN)
        {
            ungetToken();
            return f;
        }
        else
        {
            syntax_error("term. MULT or DIV expected");
        }
    }
    else
    {
        syntax_error("term. ID, LPAREN, NUM, or REALNUM expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct exprNode* expr()
{
    struct exprNode* exp;
    struct exprNode* t;

    t_type = getToken();
    if (t_type == ID || t_type == LPAREN || t_type == NUM || t_type == REALNUM)
    {
        ungetToken();
        t = term();
        t_type = getToken();
        if (t_type == PLUS || t_type == MINUS)
        {
            exp = ALLOC(struct exprNode);
            exp->op = t_type;
            exp->leftOperand = t;
            exp->rightOperand = expr();
            exp->tag = EXPR;
            exp->primary = NULL;
            return exp;
        }
        else if (t_type == SEMICOLON || t_type == MULT ||
                 t_type == DIV || t_type == RPAREN)
        {
            ungetToken();
            return t;
        }
        else
        {
            syntax_error("expr. PLUS, MINUS, or SEMICOLON expected");
        }
    }
    else
    {
        syntax_error("expr. ID, LPAREN, NUM, or REALNUM expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct assign_stmtNode* assign_stmt()
{
    struct assign_stmtNode* assignStmt;

    t_type = getToken();
    if (t_type == ID)
    {
        assignStmt = ALLOC(struct assign_stmtNode);
        assignStmt->id = strdup(token);
        t_type = getToken();
        if (t_type == EQUAL)
        {
            assignStmt->expr = expr();
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                return assignStmt;
            }
            else
            {
                syntax_error("asign_stmt. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("assign_stmt. EQUAL expected");
        }
    }
    else
    {
        syntax_error("assign_stmt. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct stmtNode* stmt()
{
    struct stmtNode* stm;

    stm = ALLOC(struct stmtNode);
    t_type = getToken();
    if (t_type == ID) // assign_stmt
    {
        ungetToken();
        stm->assign_stmt = assign_stmt();
        stm->stmtType = ASSIGN;
    }
    else if (t_type == WHILE) // while_stmt
    {
        ungetToken();
        stm->while_stmt = while_stmt();
        stm->stmtType = WHILE;
    }
    else if (t_type == DO)  // do_stmt
    {
        ungetToken();
        stm->while_stmt = do_stmt();
        stm->stmtType = DO;
    }
    else if (t_type == SWITCH) // switch_stmt
    {
        ungetToken();
        stm->switch_stmt = switch_stmt();
        stm->stmtType = SWITCH;
    }
    else
    {
        syntax_error("stmt. ID, WHILE, DO or SWITCH expected");
    }
    return stm;
}

struct stmt_listNode* stmt_list()
{
    struct stmt_listNode* stmtList;

    t_type = getToken();
    if (t_type == ID || t_type == WHILE ||
        t_type == DO || t_type == SWITCH)
    {
        ungetToken();
        stmtList = ALLOC(struct stmt_listNode);
        stmtList->stmt = stmt();
        t_type = getToken();
        if (t_type == ID || t_type == WHILE ||
            t_type == DO || t_type == SWITCH)
        {
            ungetToken();
            stmtList->stmt_list = stmt_list();
            return stmtList;
        }
        else // If the next token is not in FOLLOW(stmt_list),
            // let the caller handle it.
        {
            ungetToken();
            stmtList->stmt_list = NULL;
            return stmtList;
        }
    }
    else
    {
        syntax_error("stmt_list. ID, WHILE, DO or SWITCH expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct bodyNode* body()
{
    struct bodyNode* bod;

    t_type = getToken();
    if (t_type == LBRACE)
    {
        bod = ALLOC(struct bodyNode);
        bod->stmt_list = stmt_list();
        t_type = getToken();
        if (t_type == RBRACE)
        {
            return bod;
        }
        else
        {
            syntax_error("body. RBRACE expected");
        }
    }
    else
    {
        syntax_error("body. LBRACE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_nameNode* type_name()
{
    struct type_nameNode* tName;

    tName = ALLOC(struct type_nameNode);
    t_type = getToken();
    if (t_type == ID || t_type == INT || t_type == REAL ||
        t_type == STRING || t_type == BOOLEAN || t_type == LONG)
    {
        tName->type = t_type;
        if (t_type == ID)
        {
            tName->id = strdup(token);
        }
        else
        {
            tName->id = NULL;
        }
        return tName;
    }
    else
    {
        syntax_error("type_name. type name expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct id_listNode* id_list()
{
    struct id_listNode* idList;

    idList = ALLOC(struct id_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        idList->id = strdup(token);
        t_type = getToken();
        if (t_type == COMMA)
        {
            idList->id_list = id_list();
            return idList;
        }
        else if (t_type == COLON)
        {
            ungetToken();
            idList->id_list = NULL;
            return idList;
        }
        else
        {
            syntax_error("id_list. COMMA or COLON expected");
        }
    }
    else
    {
        syntax_error("id_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_declNode* type_decl()
{
    struct type_declNode* typeDecl;

    typeDecl = ALLOC(struct type_declNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        typeDecl->id_list = id_list();
        t_type = getToken();
        if (t_type == COLON)
        {
            typeDecl->type_name = type_name();
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                return typeDecl;
            }
            else
            {
                syntax_error("type_decl. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("type_decl. COLON expected");
        }
    }
    else
    {
        syntax_error("type_decl. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_declNode* var_decl()
{
    struct var_declNode* varDecl;

    varDecl = ALLOC(struct var_declNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        varDecl->id_list = id_list();
        t_type = getToken();
        if (t_type == COLON)
        {
            varDecl->type_name = type_name();
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                return varDecl;
            }
            else
            {
                syntax_error("var_decl. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("var_decl. COLON expected");
        }
    }
    else
    {
        syntax_error("var_decl. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_decl_listNode* var_decl_list()
{
    struct var_decl_listNode* varDeclList;

    varDeclList = ALLOC(struct var_decl_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        varDeclList->var_decl = var_decl();
        t_type = getToken();
        if (t_type == ID)
        {
            ungetToken();
            varDeclList->var_decl_list = var_decl_list();
            return varDeclList;
        }
        else
        {
            ungetToken();
            varDeclList->var_decl_list = NULL;
            return varDeclList;
        }
    }
    else
    {
        syntax_error("var_decl_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_decl_listNode* type_decl_list()
{
    struct type_decl_listNode* typeDeclList;

    typeDeclList = ALLOC(struct type_decl_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        typeDeclList->type_decl = type_decl();
        t_type = getToken();
        if (t_type == ID)
        {
            ungetToken();
            typeDeclList->type_decl_list = type_decl_list();
            return typeDeclList;
        }
        else
        {
            ungetToken();
            typeDeclList->type_decl_list = NULL;
            return typeDeclList;
        }
    }
    else
    {
        syntax_error("type_decl_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_decl_sectionNode* var_decl_section()
{
    struct var_decl_sectionNode *varDeclSection;

    varDeclSection = ALLOC(struct var_decl_sectionNode);
    t_type = getToken();
    if (t_type == VAR)
    {
        // no need to ungetToken()
        varDeclSection->var_decl_list = var_decl_list();
        return varDeclSection;
    }
    else
    {
        syntax_error("var_decl_section. VAR expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_decl_sectionNode* type_decl_section()
{
    struct type_decl_sectionNode *typeDeclSection;

    typeDeclSection = ALLOC(struct type_decl_sectionNode);
    t_type = getToken();
    if (t_type == TYPE)
    {
        typeDeclSection->type_decl_list = type_decl_list();
        return typeDeclSection;
    }
    else
    {
        syntax_error("type_decl_section. TYPE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct declNode* decl()
{
    struct declNode* dec;

    dec = ALLOC(struct declNode);
    dec->type_decl_section = NULL;
    dec->var_decl_section = NULL;
    t_type = getToken();
    if (t_type == TYPE)
    {
        ungetToken();
        dec->type_decl_section = type_decl_section();
        t_type = getToken();
        if (t_type == VAR)
        {
            // type_decl_list is epsilon
            // or type_decl already parsed and the
            // next token is checked
            ungetToken();
            dec->var_decl_section = var_decl_section();
        }
        else
        {
            ungetToken();
            dec->var_decl_section = NULL;
        }
        return dec;
    }
    else
    {
        dec->type_decl_section = NULL;
        if (t_type == VAR)
        {
            // type_decl_list is epsilon
            // or type_decl already parsed and the
            // next token is checked
            ungetToken();
            dec->var_decl_section = var_decl_section();
            return dec;
        }
        else
        {
            if (t_type == LBRACE)
            {
                ungetToken();
                dec->var_decl_section = NULL;
                return dec;
            }
            else
            {
                syntax_error("decl. LBRACE expected");
            }
        }
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct programNode* program()
{
    struct programNode* prog;

    prog = ALLOC(struct programNode);
    t_type = getToken();
    if (t_type == TYPE || t_type == VAR || t_type == LBRACE)
    {
        ungetToken();
        prog->decl = decl();
        prog->body = body();
        return prog;
    }
    else
    {
        syntax_error("program. TYPE or VAR or LBRACE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

//----------------------------------------------------//
//----------------------------------------------------//
//                Semantics
//------------------------------------------------------//

unordered_map<string,string> types;

unordered_map<string,string> table;

vector<string> printables;

int get_type_of(string symbol)
{
    unordered_map<string,string>::iterator itr;
    string key;
    for (itr = types.begin(); itr != types.end(); ++itr)
    {
        if (itr->first == symbol)
        {
            key = itr->second;
           //cout<<"\t\tKey of "<<symbol<<" is " <<key<<endl;
            break;
        }
       if(itr->second == symbol)
            key="type";  //bug fix will try better solution
    }
    if(key=="INT")
        return 3;
    else if(key=="REAL")
        return 4;
   else if(key=="STRING")
           return 5;
  else if(key=="BOOLEAN")
        return 6;
   else if(key=="LONG")
        return 8;
   else if(key.empty())
        return 0;
   else 
        return 9;
}

string get_key_customtype(string symbol)
{
    unordered_map<string,string>::iterator itr;
    string key;
    for (itr = types.begin(); itr != types.end(); ++itr)
    {
        if (itr->first == symbol)
        {
            key = itr->second;
            //cout<<"\t\tKey of "<<symbol<<" is " <<key<<endl;
            return key;
        }
    }
    return"";   
}

int already_declared(string symbol)
{
   //check in all the sets
    return get_type_of(symbol);
   //return the set_type in which it is declared -- 0 -- if not_yet Declared
}

int already_declared_variable(string symbol)
{
    unordered_map<string,string>::iterator itr;
    string key;
    for (itr = table.begin(); itr != table.end(); itr++)
    {
        if (itr->second == symbol || itr->first == symbol)
        {
            key = itr->second;
            //cout<<"\t\tKey of var "<<symbol<<" is " <<key<<endl;
            break;
        }
    }
    
    if(key=="INT")
        return 3;
    else if(key=="REAL")
        return 4;
   else if(key=="STRING")
           return 5;
  else if(key=="BOOLEAN")
        return 6;
   else if(key=="LONG")
        return 8;
   else if(key.empty())
        return 0;
   else if(key==symbol) 
        return 9;
    else        
        return -9; //denotes repeating type/ var 'or' an unknown type 
}

string get_key_customvariable(string symbol)
{
    unordered_map<string,string>::iterator itr;
    string key;
    for (itr = table.begin(); itr != table.end(); itr++)
    {
        if (itr->first == symbol)
        {
            key = itr->second;
            //cout<<"\t\tKey of "<<symbol<<" is " <<key<<endl;
            return key;
        }
    }
    return "";   
}
//// sorry for the confusing terminology but due to code refactoring this happened :(
void add_to_set(int set_type,string symbol)
{
    string key;
        switch (set_type)
        {
            case INT: key="INT"; break;
            case REAL: key="REAL"; break;
            case STRING: key="STRING"; break;
            case BOOLEAN: key="BOOLEAN"; break;
            case LONG: key="LONG"; break;            
        }       
       types.insert(make_pair(symbol,key));
       printables.push_back(symbol);
}

void add_to_custom(string key,string value)
{
    types.insert(make_pair(value,key)); // name , type
    printables.push_back(value);
}

void add_to_customtype_var(string key,string value)
{
    table.insert(make_pair(value,key)); // name , type i.e. kushal INT
    printables.push_back(value);
}

void add_to_declared_var_set(int set_type,string symbol)
{
    string key;
        switch (set_type)
        {
            case INT: key="INT"; break;
            case REAL: key="REAL"; break;
            case STRING: key="STRING"; break;
            case BOOLEAN: key="BOOLEAN"; break;
            case LONG: key="LONG"; break;    
        }
         table.insert(make_pair(symbol,key));
         printables.push_back(symbol);         
}

void update_new_var_type(string new_type,string var,int built_in)
{
    //cout<<"update new va type : var "<<var<<" type = "<<new_type<<endl;
    if(var == "" || var.empty())
        return;
    switch (built_in)
        {
            case INT: new_type="INT"; break;
            case REAL: new_type="REAL"; break;
            case STRING: new_type="STRING"; break;
            case BOOLEAN: new_type="BOOLEAN"; break;
            case LONG: new_type="LONG"; break;    
        }    
    if(new_type.empty())
        return;            
    table.insert(make_pair(var,new_type));    
}

void update_related_nodes(string node,string key) //tobe updated , new-Type
{
    size_t found = node.find("my");
    if(found != string::npos && node == key)
        return;
    unordered_map<string,string>::iterator itr;
    string change[10];
    string vars[20];
    int j=0,v=0;    
    //cout<<"\t\tUpdate related nodes : nodes = "<<node<<"new type = "<<key<<endl;
    for (itr = table.begin(); itr != table.end(); itr++)
    {
        if (itr->second == node)
        {  
            vars[v++]=itr->first;
        }
    }
    for(int i=0;i<v;i++)
    {
            table.erase(vars[i]);
            table.insert(make_pair(vars[i],key));
    }       
    
    for (itr = types.begin(); itr != types.end(); itr++)
    {
        if (itr->second == node)
        {  
            change[j++]=itr->first;            
        }        
    }
    string parent_type = get_key_customtype(node);
    if(j>0)
    {    
        types.erase(node); 
        types.insert(make_pair(node,key));
    }   
    for(int i=0;i<j;i++)
    {
        types.erase(change[i]); 
        types.insert(make_pair(change[i],key)); //updating custom type 's type        
        update_related_nodes(change[i],key);
    }     
    
    //string parent_type = get_key_customtype(node);
    //cout<<"\t\t\tParent type=="<<parent_type<<endl;
    
    if(parent_type.empty()||parent_type== "INT"|| parent_type=="BOOLEAN"||parent_type=="STRING"||parent_type=="REAL"||
        parent_type=="LONG"|| (found != string::npos))
            return;
    update_related_nodes(parent_type,key);
}

void update_type(int new_type,string symbol)
{    
    /*
        update_types:
        get the old type ... if it is not built in type then . . .
              update that type to new_type as well
              repeat if that type is also not built-in
    */
    string old_key = get_key_customvariable(symbol);
    string key;
        switch (new_type)
        {
            case INT: key="INT"; break;
            case REAL: key="REAL"; break;
            case STRING: key="STRING"; break;
            case BOOLEAN: key="BOOLEAN"; break;
            case LONG: key="LONG"; break;    
        }
        if(old_key == key || symbol==""||symbol.empty())
            {
                //cout<<"\treturn no need to update %s"<<symbol<<endl;
                return;
            }
        table.erase(symbol);
        table.insert(make_pair(symbol,key));
        //cout<<"\n\t\tUpdated "<<symbol<<" 's type to : "<<already_declared_variable(symbol)<<endl;
    /*
        to do:: get all related types for old_key
          i.e. types that have old_key as there "key" (other scenario will be handled in below code)
          update those types to new
    */
    
        //types.erase(old_key);
        types.insert(make_pair(old_key,key)); //updating custom type 's type
     
        update_related_nodes(old_key,key);    
}
void print_set(string type)
{
    unordered_map<string,string>::iterator itr;    
    //custome types if type
   // cout<<"\n\tPrint_set :: "<<type<<endl;
    for (itr = types.begin(); itr != types.end(); itr++)
    {
        if (itr->second == type)
        {
            cout<<"\t\t"<< itr->first <<endl;
        }
    }
    
    //vars of type
    for (itr = table.begin(); itr != table.end(); itr++)
    {
        if (itr->second == type)
        {
            cout<<"\t\t"<<itr->first<<endl;
        }
    }
}

void throw_error(string err, string symbol)
{
    cout<<"ERROR CODE "<<err<<" "<<symbol<<endl;
    exit(1);
}

void check_type_declaration(struct type_decl_sectionNode *typeDeclSection)
{
    //passed parseTree->decl->type_decl_section
    if(typeDeclSection==NULL)
        line--;
    else
    {
           if(typeDeclSection->type_decl_list != NULL)
            {
                struct type_decl_listNode* typeDeclList= typeDeclSection->type_decl_list;
                    do
                    {
                        struct type_declNode* typeDecl= typeDeclList->type_decl;
                         //fetch all the ids
                         struct id_listNode* idList = typeDecl->id_list;
                         string list[50];
                         int j=0;
                         line++; 
                         while(idList != NULL)
                         {
                             list[j++] = idList->id;
                             idList = idList->id_list;
                         }
                         int type_ = typeDecl -> type_name->type;
                         
                         if(type_ == ID)
                             type_= get_type_of(typeDecl -> type_name->id);
                            
                             if(!(type_>=3 && type_<=8))
                             {
                                string type_key = typeDecl -> type_name->id;
                                if(type_ == 0) //new user_declared type
                                {
                                   //assign it a newly unknown type
                                    type_key=unknown_types[u];                                    
                                    types.insert(make_pair(typeDecl -> type_name->id,type_key));
                                    u++;
                                }
                                //if(type_ == 9) //already_declared_Custom_type
                                  //  get_key_customtype(typeDecl -> type_name->id);
                               
                                    //printf("type = %d so adding to custom type: \n",type_);
                                    for(int i=0;i<j;i++)
                                     {
                                        if(already_declared(list[i])== 0)
                                        {                                            
                                            add_to_custom(type_key,list[i]);                                            
                                        }
                                        else
                                            throw_error("1.2",list[i]);
                                     }
                                     if(type_==0)
                                        printables.push_back(typeDecl -> type_name->id);
                                                               
                             }  
                            else
                            {  
                                //printf("Type= %d \n",type_);                       
                                 for(int i=0;i<j;i++)
                                 {
                                    int already_ = already_declared(list[i]);
                                    if(already_ == 0)
                                        add_to_set(type_,list[i]);
                                    else if (already_ == 9)
                                        throw_error("1.2",list[i]);
                                    else
                                        throw_error("1.1",list[i]);
                                 }
                           }
                      typeDeclList = typeDeclList->type_decl_list;
                    }while(typeDeclList != NULL);                  
            }
    }

}

void check_var_declaration(struct var_decl_sectionNode *varDeclSection)
{
    //passed parseTree->decl->type_decl_section
    if(varDeclSection==NULL)
        line--;
    else
    {
           if(varDeclSection->var_decl_list != NULL)
            {
                struct var_decl_listNode* varDeclList= varDeclSection->var_decl_list;
                    do
                    {
                        struct var_declNode* varDecl= varDeclList->var_decl;
                         //fetch all the ids
                         struct id_listNode* idList = varDecl->id_list;
                         string vars[50];
                         int j=0;
                         line++;
                         while(idList != NULL)
                         {
                             vars[j++] = idList->id;
                             idList = idList->id_list;
                         }
                         int type_ = varDecl -> type_name->type;
                         if(type_ == ID)
                            type_= get_type_of(varDecl -> type_name->id);

                             if(!(type_>=3 && type_<=8))
                             {
                                    //variable used as type name then error
                                    int var_as_type = already_declared_variable(varDecl -> type_name->id);
                                    //printf("\tvar_as_type = %d\n",var_as_type);                                    
                                    if(var_as_type!= 0)
                                    {
                                        throw_error("2.2",varDecl -> type_name->id);
                                    }
                                    
                                    //printf("adding to custom: \n");  
                                    int custom_type = already_declared(varDecl -> type_name->id);
                                    int add_to_printables = custom_type;
                                    string type_key = "";
                                        if(custom_type == 0) //new user_declared type
                                        {
                                           //assign it a newly unknown type
                                            type_key=unknown_types[u];                                    
                                            types.insert(make_pair(varDecl -> type_name->id,type_key));
                                            u++;                                             
                                            custom_type = 9;
                                        }
                                                                                      
                                    for(int i=0;i<j;i++)
                                     {                                        
                                         int already_ = already_declared(vars[i]);
                                                                             
                                        //check if already declared variable
                                    
                                        int dupl_var = already_declared_variable(vars[i]);  
                                        //cout<<"\t\tis  "<<vars[i]<<" dupl_var? = "<<dupl_var<<endl;                                   
                                        if(already_ != 0 || dupl_var==9)
                                            throw_error("1.3",vars[i]);
                                        
                                        if(dupl_var != 0)
                                            throw_error("2.1",vars[i]);
                                        
                                        //if type is custom type than get the parent type and use that as a key
                                        
                                        if(custom_type == 9)
                                        {
                                          //get key 
                                          type_key=get_key_customtype(varDecl -> type_name->id);                                          
                                          add_to_customtype_var(type_key,vars[i]);  
                                        }
                                        else
                                        {
                                            //proceed normally
                                            // need to remove this code not sure                                          
                                            add_to_customtype_var(varDecl -> type_name->id,vars[i]);
                                            
                                        }
                                        
                                           
                                     }    
                                     
                                if (add_to_printables == 0)
                                      printables.push_back(varDecl -> type_name->id);                                          
                                                                
                             }  //add_to_declared_var_set
                            else
                            {  
                                //printf("Type= %d \n",type_);                       
                                 for(int i=0;i<j;i++)
                                 {
                                    //check if already declared as a type explicitely
                                    int already_ = already_declared(vars[i]);
                                    
                                    //check if already declared variable
                                    int dupl_var = already_declared_variable(vars[i]);
                                    if(already_ != 0 || dupl_var==9)
                                            throw_error("1.3",vars[i]);
                                        
                                        if(dupl_var != 0)
                                            throw_error("2.1",vars[i]);
                                    //add_to_customtype_var(varDecl -> type_name->id,vars[i]);
                                    add_to_declared_var_set(type_,vars[i]);
                                 }
                           }
                      varDeclList = varDeclList->var_decl_list;
                    }while(varDeclList != NULL);                  
            }
    }    
}

void type_mismatch_error(char *code)
{    
    printf("TYPE MISMATCH %d %s",line,code);
    exit(1);
}

void check_body_type_mismatch(struct bodyNode* body);
void check_while_stmt(struct while_stmtNode *while_stmt);
void check_do_stmt(struct while_stmtNode *while_stmt);

string custom_var_key="";

int compare_types(int t1,int t2,string key1,string key2,char* error,char *var1,char *var2)
{
    //cout<<"Compare Types "<<t1<<" "<<t2<<" keys: k1: "<<key1<<" k2: "<<key2<<" Var1 = "<<var1<<" Var2 = "<<var2<<endl;
    t1=abs(t1);
    t2=abs(t2);
    if((t1+t2)==18)
        {
             //make their types same
                if(key1!=key2)
                {
                                     
                     update_related_nodes(key2,key1); // in short making them same key2 -> key1
                }
                    
               custom_var_key=key1;
                return 9;
                      
        }               
        
        //update custom variable's type with known type
       if(t1+t2 < 18) // anyone is custom type var
        {
            if(t1==9)
            {
                //printf("\tUpdate %s typye to %d \n",var1,t2);
                if(strlen(var1)!=0)
                    update_type(t2,var1);               
                else
                {
                    switch (t2)
                    {
                        case INT: key2="INT"; break;
                        case REAL: key2="REAL"; break;
                        case STRING: key2="STRING"; break;
                        case BOOLEAN: key2="BOOLEAN"; break;
                        case LONG: key2="LONG"; break;    
                    }
                    update_related_nodes(key1,key2);
                }
                return t2;
            }
            else if(t1==0)
            {
                update_new_var_type(key2,var1,t2); //newly declared variable
            }           
            else if(t2==9)
            {
                if(strlen(var2)!=0)
                      update_type(t1,var2); 
                else
                {
                    switch (t1)
                    {
                        case INT: key1="INT"; break;
                        case REAL: key1="REAL"; break;
                        case STRING: key1="STRING"; break;
                        case BOOLEAN: key1="BOOLEAN"; break;
                        case LONG: key1="LONG"; break;    
                    }
                    update_related_nodes(key2,key1);
                }
                return t1;
            }
            else if(t2==0)
            {
                update_new_var_type(key1,var2,t1); //newly declared variable
            }
            else if(t1==t2) //built in types check
                return t1;        
            else
                type_mismatch_error(error);
                 
        } 
        if(t1==0,t2==0) //both are newly declared
        {
            //declare them with same unknown type
            cout<<"\n\t Both are new vars . . their type will be: "<<unknown_types[u]<<endl;
            if(var1!=NULL && var1!="")
                table.insert(make_pair(var1,unknown_types[u]));
            if(var2!=NULL && var2!="")
                table.insert(make_pair(var2,unknown_types[u]));
             custom_var_key=unknown_types[u];
              u++;//set it to next unknwon type as one got consumed
          return 9;
        }
     return -1; //timepass return :P
}

int get_type_of_primarynode(struct primaryNode* primary)
{
    if (primary->tag == ID)
        {
            if(already_declared(primary->id)!=0)
                    throw_error("1.4",primary->id); //type used as VAR
            
            int type_ = already_declared_variable(primary->id);
            if(type_ == 9 || type_ == -9)
            {               
                //set the key in global variable 
                custom_var_key=get_key_customvariable(primary->id);
            }
            if(type_ == 0)
            {
                //new VAR used first time
                //need to confirm
                vector<string>::iterator it;
                it = find (printables.begin(), printables.end(), primary->id);
                //to avoid duplicate entries
                if(it == printables.end())
                    printables.push_back(primary->id);
                 table.insert(make_pair(primary->id,unknown_types[u]));
                 custom_var_key=unknown_types[u];
                 u++;
               return -9;
            }
            return type_;
        }
        else if (primary->tag == NUM)
        {
            return INT;
        }
        else
            return REAL;
}

int get_type_of_expression(struct exprNode* expr)
{
    if (expr->tag == EXPR)
    {
        int t1,t2;
        string key1="",key2="";
        t1 = get_type_of_expression(expr->leftOperand);
        if(t1==-9 || t1==9)
        {
            key1=custom_var_key;
            custom_var_key="";
        }
        t2 = get_type_of_expression(expr->rightOperand);
        if(t2==-9 || t2==9)
        {
            key2=custom_var_key;
            custom_var_key="";
        }
        char *var1="",*var2="";
        if(expr->leftOperand->tag != EXPR)
                    var1= expr->leftOperand->primary->id;
        if(expr->rightOperand->tag != EXPR)
                    var2= expr->rightOperand->primary->id;
                    
       return compare_types(t1,t2,key1,key2,"C2",var1,var2);
    }
    else
    {
         return get_type_of_primarynode(expr->primary);        
    }
    
    return -1;
}

void check_assign_stmt(struct assign_stmtNode *asgn)
{
    string key1="",key2="";
        int left_type = already_declared_variable(asgn->id);
         if(left_type == 0)
        {
                //new VAR declared and used
                vector<string>::iterator it;
                it = find (printables.begin(), printables.end(), asgn->id);
                //to avoid duplicate entries add only if first occurance
                if(it == printables.end())
                    printables.push_back(asgn->id);
                 table.insert(make_pair(asgn->id,unknown_types[u]));
                 custom_var_key=unknown_types[u];
                 u++;
                left_type=-9;        
        }
        
        if(left_type== -9 || left_type == 9)
            key1=get_key_customvariable(asgn->id);
        
        int right_type=get_type_of_expression(asgn->expr);
        if(right_type == -9||right_type == 9)
        {
            key2=custom_var_key;
            custom_var_key="";
        }
        
        /*
            if left_type == 9  get the key of that custom type
            if right_type !=9  throw error
            else get key for right_type and 
                compare left and right keys
            
        */
        int stmt_type = compare_types(left_type,right_type,key1,key2,"C1",asgn->id,"");  
       
}

void check_condition_stmt(struct conditionNode* condition)
{
    int left_type,right_type;
    string key1,key2; 
    left_type = get_type_of_primarynode(condition->left_operand);
    if(left_type == -9)
        {
            key1=custom_var_key;
            custom_var_key="";           
        }
    if(condition->right_operand != NULL)
    {
        right_type = get_type_of_primarynode(condition->right_operand);
        if(right_type == -9)
        {
            key2=custom_var_key;
            custom_var_key="";
        }
        //cout<<"\n\tcheck condition left ="<<left_type<<" right = "<<right_type<<endl;
        compare_types(left_type,right_type,key1,key2,"C3",condition->left_operand->id,condition->right_operand->id); 
    }
    else
    {
        //left must be a BOOLEAN
        if(left_type == -9)
        {     
            update_related_nodes(key1,"BOOLEAN"); //chnage custom type to boolean
            left_type = BOOLEAN;
        }
        if(left_type != BOOLEAN)
            type_mismatch_error("C4");
    }         
}



void check_switch_stmt(struct switch_stmtNode* switch_stmt)
{
    string id =  switch_stmt->id;
    int type_ = already_declared_variable(id);     
    if(type_ == 0)
    {
                vector<string>::iterator it;
                it = find (printables.begin(), printables.end(), switch_stmt->id);
                //to avoid duplicate entries
                if(it == printables.end())
                    printables.push_back(switch_stmt->id);
                table.insert(make_pair(switch_stmt->id,"INT"));
                type_ = INT;
    }
    if(type_ == 9 || type_ == -9 )
    {
        update_related_nodes(get_key_customvariable(switch_stmt->id),"INT");
        type_=INT;
    }
    if(type_ != INT)
        type_mismatch_error("C5"); 
    
    struct case_listNode* case_list = switch_stmt->case_list;
    do
    {
      line++; //case x { 
      check_body_type_mismatch(case_list->cas->body);
      line++; // }
      case_list=case_list->case_list;
    }while(case_list!=NULL);
}

void check_body_type_mismatch(struct bodyNode* body)
{
        struct stmt_listNode* stmt_list = body->stmt_list;
        do
        {
            struct stmtNode* stmt = stmt_list->stmt;
            
            switch (stmt->stmtType)
            {
                case ASSIGN:
                    line++; //single line statement
                    check_assign_stmt(stmt->assign_stmt);
                    break;
                case WHILE:
                    line++; //while (condition) {                    
                    check_while_stmt(stmt->while_stmt);
                    line++; //}
                    break;
                case DO:
                    line++; // DO {
                    check_do_stmt(stmt->while_stmt);
                    //line++; //}
                    break;
                case SWITCH:
                    line++; //switch id {                    
                    check_switch_stmt(stmt->switch_stmt);
                    line++; //}
                    break;
            }
            
            stmt_list= stmt_list->stmt_list;
        }while(stmt_list != NULL);
}

void check_while_stmt(struct while_stmtNode *while_stmt)
{
    check_condition_stmt(while_stmt -> condition);
    check_body_type_mismatch(while_stmt -> body);   
    
}

void check_do_stmt(struct while_stmtNode *while_stmt)
{
    // do {} WHILE condition ;
    check_body_type_mismatch(while_stmt -> body);
    line++;//} WHILE cond ;
    check_condition_stmt(while_stmt -> condition);
      
}


/******************************************
--------------- OUTPUT --------------------
*******************************************/
vector<string> integers;  //3
vector<string> reals; //4
vector<string> strings; //5
vector<string> booleans; //6
vector<string> longs; //8
vector<string> declared; //10
vector<string> unknowns;
void prepare_output()
{
    vector<string>::iterator itr = printables.begin();
    while( itr != printables.end())
    {
        //cout<<*itr<<"\t";
        int type = get_type_of(*itr);
        if(type==0) //not a type but a VAR
        {
            type = already_declared_variable(*itr);
        }

        switch (type)
        {
            case INT: integers.push_back(*itr); break;
            case REAL: reals.push_back(*itr); break;
            case STRING: strings.push_back(*itr); break;
            case BOOLEAN: booleans.push_back(*itr);break;
            case LONG: longs.push_back(*itr);break;
            //case 9  : 
            //case -9 : declared.push_back(*itr);break; //not built ins
            default: unknowns.push_back(*itr);
        }      
        itr++;
    }
}

void print_output()
{
    vector<string>::iterator itr = booleans.begin();
    printf("BOOLEAN ");
    while( itr != booleans.end())
    {
        cout<<*itr<<" ";
        itr++;
    }
    printf("#\n");
    
        printf("INT ");
        
        itr=integers.begin();
    while( itr != integers.end())
    {
        cout<<*itr<<" ";
        itr++;
    }
    printf("#\n");
    
    printf("LONG ");
    itr=longs.begin();
    while( itr != longs.end())
    {
        cout<<*itr<<" ";
        itr++;
    }
    printf("#\n");
    
        printf("REAL ");
            itr=reals.begin();
    while( itr != reals.end())
    {
        cout<<*itr<<" ";
        itr++;
    }
    printf("#\n");
    
                itr=strings.begin();
                printf("STRING ");
    while( itr != strings.end())
    {
        cout<<*itr<<" ";
        itr++;
    }
    printf("#\n");
    
    itr=unknowns.begin();
    if(itr != unknowns.end())
    {
        string next;    
        while(unknowns.size()!= 0)
        {   
            next = unknowns[0];
            string cur_type = get_key_customtype(next);
            if(cur_type.empty() || cur_type=="")
                cur_type = get_key_customvariable(next);
            //cout<<"cur_type = "<<cur_type;
            vector<string> output; //temp vector
            unordered_map<string,string>::iterator it;        
            for (it = types.begin(); it != types.end(); it++)
            {            
                if (it->second == cur_type)
                {  
                    output.push_back(it->first);    
                }
            }         
            for (it = table.begin(); it != table.end(); it++)
            {
                if (it->second == cur_type)
                {  
                    output.push_back(it->first);    
                }
            }
            
            vector<string> dummy=unknowns; //temp vector
            int size = dummy.size();
            int outsize = output.size();
            for(int j=0;j<size;j++)
            {
               for(int k=0;k<outsize;k++)
               {
                    if(output[k]==dummy[j])
                    {
                        cout<<dummy[j]<<" ";
                        unknowns.erase(remove(unknowns.begin(), unknowns.end(), dummy[j]),unknowns.end());    
                    }
               }
            }
            if(outsize!=0)
                printf("#\n"); 
        }
    }
   /*for(int i=0;i<u;i++)
   {
        vector<string> output; //temp vector
        unordered_map<string,string>::iterator it;        
        for (it = types.begin(); it != types.end(); it++)
        {            
            if (it->second == unknown_types[i])
            {  
                output.push_back(it->first);    
            }
        }         
        for (it = table.begin(); it != table.end(); it++)
        {
            if (it->second == unknown_types[i])
            {  
                output.push_back(it->first);    
            }
        }
        
        int size = unknowns.size();
        int outsize=output.size();
        for(int j=0;j<size;j++)
        {
           for(int k=0;k<outsize;k++)
           {
                if(output[k]==unknowns[j])
                    cout<<unknowns[j]<<" ";
           }
        }
        if(outsize!=0)
            printf("#\n");
    }
    */
    /*itr=declared.begin(); // unknown
    if(itr != declared.end())
    {                
        while( itr != declared.end())
        {
            cout<<*itr<<" ";
            itr++;
        }
        printf("#\n");
    }
    
    itr=unknowns.begin();
    if(itr != unknowns.end())
    {                
        while( itr != unknowns.end())
        {
            cout<<*itr<<" ";
            itr++;
        }
        printf("#\n");
    }
    
      */  
}


int main()
{
    struct programNode* parseTree;
    parseTree = program();
    // TODO: remove the next line after you complete the parser
    //print_parse_tree(parseTree); // This is just for debugging purposes
    // TODO: do type checking & print output according to project specification
    
    check_type_declaration(parseTree->decl->type_decl_section);
    line++; //VAR
    //printf("Var starts at line : %d",line);
    check_var_declaration(parseTree->decl->var_decl_section);
    //print_set("INT");
    //print_set("LONG");
    
    /*cout<<"\nPrintables:: ";
    vector<string>::iterator itr = printables.begin();
    while( itr != printables.end())
    {
        cout<<*itr<<"\t";
        itr++;
    }*/
    
    //body checking :P
        //printf("Body lbrace starts at line : %d",line);
    line++;//lbrace
    check_body_type_mismatch(parseTree->body);
    line++; //rbrace
    
    prepare_output();
    print_output();
    return 0;
}
