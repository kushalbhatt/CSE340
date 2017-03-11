/***************************
Kushal Bhatt
ASUID# 1211206721
CSE 340  Project2 Part 2
Lexical Processing Code
****************************/
#include "lexer.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//using doubly_linked_list
struct node
{
    token_type type;
    int line_no;
    char *token_value;
    struct node* next;
    struct node* prev;
}*head;

//insert function
void insert_node(token_type t_type,int line_,char* value)
{
    struct node *temp;
    temp=(struct node*)malloc(sizeof(struct node));
    if(temp==NULL)
    {
        //out of memory <->terminate
        exit(0);
    }
    
    //assign_data
    temp->type=t_type;
    temp->line_no=line_;
    temp->token_value=value;
    temp->next=NULL;
    
    //first entry
    if(head==NULL)
    {
        head=temp;
        head->prev=NULL;
        return;        
    }
    
    //else insertion at end
    head->next=temp;
    temp->prev=head;
    head=temp;
    return;
}


//print data
void print_output()
{
    struct node *temp=head;
    while(temp!=NULL)
    {
        printf("\n%d ",temp->line_no);
        
        if(temp->type == 24)
            printf("NUM ");
        if(temp->type == 25)
            printf("ID ");
        
        printf("%s",temp->token_value); 
        temp=temp->prev;
    }
}

bool is_desired_ID()
{
    if((strcmp("cse340",current_token) == 0|| strcmp("programming",current_token)==0 || strcmp("language",current_token)==0))
        return true;
    else
       return false;
}

int main()
{
    //get tokens and save it on linked_list    
    while(getToken() != END_OF_FILE )
    {
        int token_;
        ungetToken();

        token_=getToken();
                
        printf("\t %d ",token_);
    }

    //print_output
    //print_output();
    return 0;
}
