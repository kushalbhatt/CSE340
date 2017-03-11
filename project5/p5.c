/****************************************
Kushal Bhatt
ASu ID # 1211206721
CSE 340 POPL Project 5
*****************************************/
#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//contains all the declared variables
struct vars 
{
    struct ValueNode *var;
    struct vars *next;    
}*head=NULL,*tail=NULL;

void add_to_vars(struct ValueNode *v)
{
    struct vars *node = (struct vars*)malloc(sizeof(struct vars));
    node->var=v;
    node->next=NULL;

    if(head==NULL)//first entry
    {
        head=tail=node;     
    }
    else  //insert at the end
    {
    tail->next=node;
    tail=node;
    }
}

struct ValueNode* find_var(char *v)
{
    struct vars *temp=head;
    while(temp!=NULL)
    {
        if(strcmp(temp->var->name,v)==0)
            return temp->var;
       temp=temp->next;
    }
    printf("Not found \n");
    return NULL;
}

void syntax_error()
{
    printf("Syntax error");
    exit(0);
}

void parse_var_section()
{
    int t_type=getToken();
    
    while(t_type==ID)
    {
        struct ValueNode *v= malloc(sizeof(struct ValueNode));
        v->value=0;
        v->name= strdup(token);
        add_to_vars(v);
        
        t_type=getToken();
        if(t_type == COMMA)
        {
            t_type=getToken(); //repeat while another id
        }
        else if(t_type == SEMICOLON)   
        {
            if(getToken()==LBRACE)
                return;
        }
        else
            syntax_error();
    }
    
    syntax_error(); //program should not reach here if it's correct :P

}

struct StatementNode * parse_stmt();

struct ValueNode* parse_operand()
{
   struct ValueNode *v;
   int t_type=getToken(); //operand1
   
        if(t_type==ID)
            v = find_var(strdup(token));//find the valuenode with id token
        else if(t_type==NUM)
        {
            //create a new Valuenode ans assign value to NUM
            v = malloc(sizeof(struct ValueNode));
            v->value=atoi(token);
            v->name= "";         
        }
        else
            syntax_error();
    return v;
}


struct AssignmentStatement* parse_asgn_stmt()
{
    struct AssignmentStatement* asgn=malloc(sizeof(struct AssignmentStatement));
    int t_type=getToken();
    
    asgn->left_hand_side = find_var(strdup(token));
    t_type=getToken();
    if(t_type==EQUAL)
    {
        asgn->operand1=parse_operand();
        
        t_type=getToken();
        if(t_type == SEMICOLON)
        {
            asgn->operand2=NULL;
            asgn->op=0;
            return asgn;
        }
        else //must be an operator not checking for syntax error
        {
            asgn->op= t_type;
            asgn->operand2=parse_operand();
             t_type=getToken();
            if(t_type == SEMICOLON)
                return asgn;
            else
                syntax_error();
        }
    }
    else
        syntax_error();
    return NULL; //dummy return
}

struct PrintStatement* parse_print_stmt()
{
    struct PrintStatement* p = malloc(sizeof(struct PrintStatement));
    int t_type= getToken();
    
    if(t_type==ID)
    {
        p->id = find_var(strdup(token));        
        t_type= getToken();
        if(t_type == SEMICOLON)
                return p;
            else
                syntax_error();
    }
    else
        syntax_error();
    return NULL; // dummy return
}


struct IfStatement* parse_if_stmt()
{
     struct IfStatement* i=malloc(sizeof( struct IfStatement));
     int t_type;
     i->condition_operand1= parse_operand();

     t_type=getToken();
     if(t_type== GREATER || t_type== LESS || t_type== NOTEQUAL ) 
     {
         i->condition_op = t_type;         
         i->condition_operand2 = parse_operand();
         
         t_type=getToken();
         if(t_type== LBRACE)
         {
            i->true_branch=parse_stmt();
            //create  NO-OP node
            struct StatementNode* noop=malloc(sizeof(struct StatementNode));
            noop->type = NOOP_STMT;
            noop->next=NULL;
            struct StatementNode* temp=i->true_branch;
            while(temp->next!=NULL)
                temp=temp->next;
            temp->next=noop;
                        
            i->false_branch=noop;
            return i;
         }
         else
            syntax_error();
     }
     else
        syntax_error();    
   
}

struct StatementNode* parse_switch(struct StatementNode** label)
{
   int t_type=getToken();
   struct ValueNode* key;
   struct StatementNode * stmt=malloc(sizeof(struct StatementNode));
   stmt->next=NULL;
   struct StatementNode * prev=NULL;
      
   if(t_type == ID )
   {
        key = find_var(strdup(token));  //switched variable always be operand 1 for all if stmts
           //printf("Switch called for key =  %s\n",token);
        t_type=getToken();
        if(t_type==LBRACE)
        {
            t_type=getToken();
            while(t_type==CASE)
            {
                struct StatementNode * case_node=NULL;
                if(prev==NULL) //first if this will be out return node
                {
                    case_node=stmt;
                }
                else
                {
                    case_node=malloc(sizeof(struct StatementNode));
                }
                    
                case_node->next=*label; //experimental
                case_node->type=IF_STMT;
                case_node->if_stmt=malloc(sizeof( struct IfStatement));
                case_node->if_stmt->condition_op = NOTEQUAL;
                 case_node->if_stmt->true_branch = *label;  //initialise with pointing to label will be updated if required   
                if(prev!=NULL)
                {
                    prev->if_stmt->true_branch = case_node; //last if's false pointing to this if
                    //experimental
                    prev->next = case_node;                    
                }
                case_node->if_stmt->condition_operand1= key;
                
                case_node->if_stmt->condition_operand2= parse_operand(); //NUM
                getToken(); // COLON
                t_type=getToken();
                if(t_type== LBRACE)
                {
                    case_node->if_stmt->false_branch=parse_stmt();
                    
                    struct StatementNode* temp=case_node->if_stmt->false_branch;
                    while(temp->next!=NULL)
                      {                       
                       temp=temp->next;
                      }                    
                    /*struct StatementNode* go_to=malloc(sizeof(struct StatementNode));
                    go_to->type = GOTO_STMT;
                    go_to->next=NULL;
                    go_to->goto_stmt = malloc(sizeof(struct GotoStatement));
                    go_to->goto_stmt->target = *label; //goto should point to the label node noop 
                    */
                    temp->next=*label; //set true_branch's next node a noop stmt                    
                    prev = case_node;
                } // 1 case compleeted
                else
                    syntax_error();//LBRACE EXPECTED
                
                t_type=getToken();
            }//end of while
            
            if(t_type == DEFAULT)
            {
                //printf("Default starts\n");
                getToken(); //COLON                
                getToken();// {
                struct StatementNode * dflt=malloc(sizeof(struct StatementNode));
                
                //experimental
                dflt->next=*label;                
                
                dflt=parse_stmt();                
                prev->if_stmt->true_branch = dflt;
                prev->next = dflt;
                struct StatementNode* temp=dflt;
                 
                while(temp->next!=NULL)
                     temp=temp->next;
                /*struct StatementNode* go_to=malloc(sizeof(struct StatementNode));
                    go_to->type = GOTO_STMT;
                    go_to->next=NULL;
                    go_to->goto_stmt = malloc(sizeof(struct GotoStatement));
                    go_to->goto_stmt->target = *label; //goto should point to the label node noop 
                  */  
                    temp->next=*label;
                    // printf("Default's next type is  %d\n",temp->next->type);
                                    t_type=getToken();
            }
            if(t_type==RBRACE) //end of switch }
            {
                //printf("Switch completed . . fingers crossed");
                return stmt;
            }
            else
                syntax_error();//expect RBRACE
          }
            else
                syntax_error();
        }
        else
            syntax_error();
   
   return NULL; //dummy return      
}


//make sure you process { before calling this fn
struct StatementNode * parse_stmt()
{
    struct StatementNode * stmt=malloc(sizeof(struct StatementNode));
    struct StatementNode* noop;
    stmt->next=NULL;
    
     int t_type=getToken();
//     printf("\nParsing %d stmt \n",t_type);
     switch(t_type)
     {        
        case ID: stmt->type= ASSIGN_STMT;
                 ungetToken();
                 stmt->assign_stmt=parse_asgn_stmt();                 
                 break; 

        case PRINT: stmt->type = PRINT_STMT;
                    stmt->print_stmt=parse_print_stmt();
                    break;

        case WHILE: stmt->type=IF_STMT;
                    stmt->if_stmt=parse_if_stmt();
                    
                    struct StatementNode* temp=stmt->if_stmt->true_branch;
                    while(temp->next->next!=NULL) //point to 1 node before noop 
                        temp=temp->next;
                        
                   struct StatementNode* go_to=malloc(sizeof(struct StatementNode));
                    go_to->type = GOTO_STMT;
                    go_to->next=NULL;
                    go_to->goto_stmt = malloc(sizeof(struct GotoStatement));
                    go_to->goto_stmt->target = stmt; 
                    
                    temp->next=go_to;
                    stmt->next= stmt->if_stmt->false_branch; //pointing to noop                    
                    stmt->next->next=parse_stmt();
                    return stmt;
                    break;

        case IF:    stmt->type = IF_STMT;
                    stmt->if_stmt=parse_if_stmt();
                    stmt->next = stmt->if_stmt->false_branch; //pointing to noop                    
                    stmt->next->next=parse_stmt();
                    return stmt;                    
                    //printf("if parsed\n");
                    break;
        case SWITCH:    
                        noop=malloc(sizeof(struct StatementNode));
                        noop->type = NOOP_STMT;
                        noop->next=NULL;           
                        //noop is the sync point / label                
                        stmt = parse_switch(&noop); //parse_switch must return the whole if ladder 's head
                        //noop marks the end of switch
                        noop->next=parse_stmt();
                        return stmt;
                     break;
        
        case RBRACE: return NULL; break;

        case EOF:  return NULL;

        default: syntax_error();
     } 
    stmt->next=parse_stmt();
    return stmt;  
}

struct StatementNode* parse_generate_intermediate_representation()
{
     struct StatementNode * program; 
     //var section
     parse_var_section();
     //lbrace already processed
     //{ body }        
      program=parse_stmt();
    
}



