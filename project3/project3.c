/*******************************
Kushal Bhatt
ASU ID: 1211206721

lexer.c and linked list sorting codes have been modified and used
from Project 2
********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
char non_terminal[50][256];
int non_terminals_count=0;

void print_non_terminals()
{
    int i;
    for (i=0;i<non_terminals_count;i++)
        printf("%s ",non_terminal[i]);
    printf("\n");
}


bool is_non_terminal(char *symbol)
{
    int i;
    for (i=0;i<non_terminals_count;i++)
    {
        if(strcmp(non_terminal[i],symbol)== 0)
            return true;            
    }
    return false;
}

int get_index(char *symbol)
{
    int i;
    for (i=0;i<non_terminals_count;i++)
    {
        if(strcmp(non_terminal[i],symbol)== 0)
            break;            
    }
    return i;
}


struct terminal_info
{
    int count;
    char *value;
    struct terminal_info *next;
}*terminals;


//insert into linkedlist
void insert_terminal(char*val)
{
    struct terminal_info *temp,*iter,*temp2;
    temp=(struct terminal_info*)malloc(sizeof(struct terminal_info));    
    if(temp==NULL)
    {
        //out of memory <->terminate
        printf("****Out of memory::******");
        exit(0);
    }

    //else check if already present? then update count
        iter=terminals;
    while(iter != NULL)
    {
        if(strcmp(iter->value,val)== 0)
        {
            iter->count++;
            return;
        }
        iter=iter->next;
    }
            
    //assign_data
    temp->count=1;
    temp->value=val;
    temp->next=NULL;
    //printf("Terminal added\n");
    //first entry
    if(terminals==NULL)
    {
        terminals=temp;
        terminals->next=NULL;
        return;        
    }
    temp2=terminals;
    //else insertion at end
    while(temp2->next != NULL)
        temp2=temp2->next;
    
    temp2->next=temp;
    return;
}


int compare_(const void* f, const void* s)
{
   struct terminal_info* first;
   struct terminal_info* second;
   int to_return;
   first = (struct terminal_info*)f;
   second = (struct terminal_info*)s;
   to_return = strcmp(first->value, second->value);
   return to_return;
}

struct terminal_info* sort_terminals(struct terminal_info* head)
{
   /* I want to use quicksort, but qsort function takes in an array,
	  and I have a linked list. Must convert. */
   int num_entries = 0;
   int i;
   struct terminal_info* cur;
   struct terminal_info* array;
	  
   cur = head;
   while (cur != NULL)
   {
	  num_entries += 1;
	  cur = cur->next;
   }

   array = malloc(sizeof(struct terminal_info) * num_entries);

   cur = head;
   i = 0;
   while (cur != NULL)
   {
	  memcpy(&array[i], cur, sizeof(struct terminal_info));
	  cur = cur->next;
	  i += 1;
   }

   qsort(array, num_entries, sizeof(struct terminal_info), compare_);

   /* Put the entries back */
   cur = head;
   for (i = 0; i < num_entries; i++)
   {
      cur->value = array[i].value;
      cur->count=array[i].count;
	  cur = cur->next;
   }

   free(array);

   return head;
}

void print_terminal_info()
{
    struct terminal_info *temp=terminals;
    while(temp!=NULL)
    {
        printf("%s: %d\n",temp->value,temp->count);
        temp=temp->next;
    }
}


// where whole grammar will be saved for repeating iterations
struct grammar
{
    char *symbol;
    struct grammar *next;
}*head,*tail,*current=NULL;   //current is for parsing the saved grammar info


void insert_grammar_info(char *symbol)
{
    struct grammar *temp;
    temp=(struct grammar*)malloc(sizeof(struct grammar));
    //printf("\ninsert grammar::\n");
    
    if(temp==NULL)
    {
        //out of memory <->terminate
        printf("****Out of memory::******");
        exit(0);
    }
            
    //assign_data
    temp->symbol=strdup(symbol);
    temp->next=NULL;
    //printf("Terminal added\n");
    
    //first entry
    if(head==NULL)
    {
        head=tail=temp;        
        return;        
    }
    //else insertion at end   
    tail->next=temp;
    tail=temp;
    //free(temp);
    return;
}

char* getNextSymbol()
{
    char *c,*cur_symbol;
    if(current==NULL)
    {
        if(head==NULL)
            return NULL;
        current=head;
    }   
    cur_symbol=strdup(current->symbol);
    
    //update current for next iteration  FYI ## denotes end of pass 
    if(current==tail)
        current=head;
    else
        current=current->next;    
    return cur_symbol;
}

// to be deleted just for debugging
void print_grammar()
{
    struct grammar *temp=head;
    while(temp!=NULL)
    {  
        printf("\t %s",temp->symbol);
        if(strcmp(temp->symbol,"#")==0)
            printf("\n");
        temp=temp->next;
    }
}


//FIRST SET DATA STRUCTURE
struct FIRST
{
    char *item;
    struct FIRST *next;
}**Head,**Tail;

//FOLLOW SET DATA STRUCTURE
struct FOLLOW
{
    char *item;
    struct FOLLOW *next;
}**Follow_Head,**Follow_Tail;


void initialize_sets(int n)
{ 
    Head=(struct FIRST**)malloc(n * sizeof(struct FIRST*));
    Tail=(struct FIRST**)malloc(n * sizeof(struct FIRST*));
    Follow_Head=(struct FOLLOW**)malloc(n * sizeof(struct FOLLOW*));
    Follow_Tail=(struct FOLLOW**)malloc(n * sizeof(struct FOLLOW*));
}


int add_to_FIRST(int index,char *value)
{
    //printf("\n Add to First:: ");
    struct FIRST *temp,*iter;
    
    iter=Head[index];
    //check if already present no duplicate entries needed
    while(iter!=NULL)
    {
        if(strcmp(iter->item,value)==0)
            return 0;  // valaue already present no change
        iter=iter->next;
    }
    
    temp=(struct FIRST *)malloc(sizeof(struct FIRST));
    
    if(temp==NULL)
    {
        //out of memory <->terminate
        printf("****Out of memory::******");
        exit(0);
    }
            
    //assign_data
    temp->item=strdup(value);
    temp->next=NULL;
    //printf("Terminal added\n");
    
    //first entry
    if(Head[index]==NULL)
    {
        Head[index]=Tail[index]=temp;        
        return 1;        
    }
    //else insertion at end   
    Tail[index]->next=temp;
    Tail[index]=temp;
    //free(temp);
    //printf("\t %s added to index= %d",value,index);
    return 1;  // denoted that value was added == change occured
}

int copy_FIRST_A_to_FIRST_B(int index_A,int index_B)
{
    //for each item of a call add_to_First(B)
    int change= 0;
    //printf("\n Copy %d to %d ",index_A,index_B);
    //nothing to add
    if(Head[index_A]==NULL)
        return 0;
        
    struct FIRST *temp=Head[index_A];
    while(temp != NULL)
    {
        //do not copy epsilon
        if(strcmp(temp->item,"#") != 0)
            change+=add_to_FIRST(index_B,temp->item);
        temp=temp->next;
    }
    //printf("\t%d\t", change);
    return change;
}

bool epsilon_in_first_of(int index)
{
    struct FIRST *temp = Head[index];
    while(temp!=NULL)
    {
        if(strcmp(temp->item,"#")==0)
        {
            //printf("\t Epsilon is there in FIRST of %d\t",index);
            return true;
        }
        temp=temp->next;
    }
    return false;
}


void calculate_FIRST_sets()
{
    int change = 1;
    //keep iterating untill nothing changes
    char *prev_token,*cur_token,*non_t;
    
    while(change != 0)
    {   
        change=0; // initialize change
        cur_token=NULL;
        
        NEXT_RULE:
        //printf("\n\n New grammar rule");
        //first symbol will be a non-terminal        
        if(cur_token==NULL)
            non_t = strdup(getNextSymbol());
        else
            non_t=cur_token;
                        
        prev_token = strdup(getNextSymbol()); //ARROW  variablr for keeping rack of epsi
        cur_token=strdup(getNextSymbol());
        
        //printf("\t non_t = %s cur_ = %s",non_t,cur_token);       
        
        UPDATE_FIRST:
        if(strcmp(cur_token,"#")==0)
        {
            //null production
            add_to_FIRST(get_index(non_t),"#");
        }
        else if(!is_non_terminal(cur_token)) // add to first(non_t)
        {
            change+= add_to_FIRST(get_index(non_t),cur_token);
        }
        else
        {
            /*****
                STEP 1::
                COPY FIRST(A) - {epsilon}
                
                if there is null in FIRST(A) then getNextSymbol
                    if next_symbol is terminal add to first(cur) and break from here 
                    else if that is a non-termin then go to STEP 1
                if you reach "#" then add eps to FIRST(cur) 
            ******/
            change+= copy_FIRST_A_to_FIRST_B(get_index(cur_token),get_index(non_t));
            
            if(epsilon_in_first_of(get_index(cur_token)))
            {
                cur_token=getNextSymbol();
                goto UPDATE_FIRST;
            }
        }
                     
        //cur_token=getNextSymbol();
        while(strcmp(cur_token,"#")!=0)
        {
            //do nothing skip
            cur_token=getNextSymbol();
        }
        
        cur_token=getNextSymbol();
        if(strcmp(cur_token,"##")==0)
            continue;
        else 
            goto NEXT_RULE;            
        //if you reach here then ## is encountered so next pass begin                    
    } 
    //printf("Completed first sets :D");
}


void print_first_sets()
{
    int i;
    for(i=0;i<non_terminals_count;i++)
    {
        printf("FIRST(%s) = {",non_terminal[i]);
        struct FIRST *temp=Head[i];
        while(temp!=NULL)
        {
            if(temp==Head[i])
                printf(" %s",temp->item);
            else
                printf(", %s",temp->item);
            temp=temp->next;
        }
        free(temp);
        printf(" }\n");
    }
}

/////////follow_sets/////////

int add_to_follow(int index,char *value)
{
    //printf("\n Add to First:: ");
    struct FOLLOW *temp,*iter;
    
    iter=Follow_Head[index];
    //check if already present no duplicate entries needed
    while(iter!=NULL)
    {
        if(strcmp(iter->item,value)==0)
            return 0;  // valaue already present no change
        iter=iter->next;
    }
    
    temp=(struct FOLLOW *)malloc(sizeof(struct FOLLOW));
    
    if(temp==NULL)
    {
        //out of memory <->terminate
        printf("****Out of memory::******");
        exit(0);
    }
            
    //assign_data
    temp->item=strdup(value);
    temp->next=NULL;
    //printf("Terminal added\n");
    
    //first entry
    if(Follow_Head[index]==NULL)
    {
        Follow_Head[index]=Follow_Tail[index]=temp;        
        return 1;        
    }
    //else insertion at end   
    Follow_Tail[index]->next=temp;
    Follow_Tail[index]=temp;
    //free(temp);
    //printf("\t %s added to index= %d",value,index);
    return 1;  // denoted that value was added == change occured
}

int copy_to_follow(int index_A,int index_B,int what) //what =0 copy follow 1=first
{
    //for each item of A call add_to_follow(B)
    int change= 0;
    struct FOLLOW *temp;
    struct FIRST *temp2;
    //printf("\n Copy %d to %d mode =%d",index_A,index_B,what);
    //nothing to add
    if(what==0)
    {
        if(Follow_Head[index_A]==NULL)
            return 0;
        temp=Follow_Head[index_A];
    }
    else
    {
        if(Head[index_A]==NULL)
            return 0;
        temp2= Head[index_A];
    }
       
    if(what==0)
    {
        while(temp != NULL)
        {
            //do not copy epsilon
            if(strcmp(temp->item,"#") != 0)
                change+=add_to_follow(index_B,temp->item);
            temp=temp->next;
        }
    }
    else
    {
        while(temp2 != NULL)
        {
            //do not copy epsilon
            if(strcmp(temp2->item,"#") != 0)
                change+=add_to_follow(index_B,temp2->item);
            temp2=temp2->next;
        }
    }
    //printf("\t%d\t", change);
    return change;
}


void calculate_follow_sets()
{    
    //printf("\n inside calc_follow");
    int change = 1,i=0;
    //keep iterating untill nothing changes
    char *prev_token,*cur_token,*non_t,*next_token,*cur_symbol;
    
    while(change != 0)
    {
        //printf("****While starts*******\n");
        change=0; // initialize change
        for (i=0;i<non_terminals_count;i++)
        {
            //strcpy(cur_symbol,non_terminal[i]);        
            cur_token=NULL;
        //printf("\t Checking for %s\n",non_terminal[i]);
        NEXT_RULE:
        //printf("\n\n New grammar rule");
        //first symbol will be a non-terminal        
        if(cur_token==NULL)
            non_t = getNextSymbol();
        else
            non_t=cur_token;
                        
        prev_token = getNextSymbol(); //ARROW  variable 
        cur_token=  getNextSymbol();
        
        //find the node on RHS if any
        UPDATE_FOLLOW:
        while(strcmp(cur_token,non_terminal[i])!=0 && (strcmp(cur_token,"#")!=0))
        {
            cur_token=getNextSymbol();
            if(strcmp(cur_token,"#")==0)
                break;
        }
        //printf("\n cur_token=%s non_terminal=%s non_t=%s",cur_token,cur_symbol,non_t);
        //printf("\n non_t = %s cur_ = %s\n",non_t,cur_token);       
        if(strcmp(cur_token,"#")==0)
        {
            //null production or end of production
            //add_to_FIRST(get_index(non_t),"#");
        }
        else if(is_non_terminal(cur_token)) 
        {
            //check next symbol if next is # then copy follow(non_t) to (cur)
            CHECK_NEXT_SYMBOLS:            
            next_token=getNextSymbol();
            //printf("\n next = %s cur_ = %s",next_token,cur_token);   
            if(strcmp(next_token,"#")==0)
            {
                change+=copy_to_follow(get_index(non_t),get_index(non_terminal[i]),0);               
            }
            else
            { 
                //if next is a terminal then add that to follow(cur)
                if(!is_non_terminal(next_token))
                {
                    change+=add_to_follow(get_index(non_terminal[i]),next_token);
                    //complete the grammar rule till #
                    cur_token=getNextSymbol();
                    goto UPDATE_FOLLOW;
                }
                else
                {
                    //next is also non_terminal then check FIRST Sets
                    change+=copy_to_follow(get_index(next_token),get_index(non_terminal[i]),1);
                    
                    if(epsilon_in_first_of(get_index(next_token)))
                        goto CHECK_NEXT_SYMBOLS;
                    else
                    { 
                         cur_token=next_token;
                         goto UPDATE_FOLLOW;                       
                    }
                }
            }
        }
        else
        {
            //terminal go to next symbol
            cur_token=getNextSymbol();
            goto UPDATE_FOLLOW;
        }
                
        //reach here after scanning a # go to next rule
        cur_token=getNextSymbol();
        if(strcmp(cur_token,"##")!=0)
         {
            goto NEXT_RULE;            
         }
         //printf(" reached ##\n");          
       }//end of for                 
    }
    //printf("Completed follow sets :D");
}

void print_follow_sets()
{
    int i;
    for(i=0;i<non_terminals_count;i++)
    {
        printf("FOLLOW(%s) = {",non_terminal[i]);
        struct FOLLOW *temp=Follow_Head[i];
        while(temp!=NULL)
        {
            if(temp==Follow_Head[i])
                printf(" %s",temp->item);
            else
                printf(", %s",temp->item);
           temp=temp->next;
        }
        printf(" }\n");
        free(temp);        
    }
}

int compare_entries_first(const void* f, const void* s)
{
   struct FIRST* first;
   struct FIRST* second;
   int to_return;
   first = (struct FIRST*)f;
   second = (struct FIRST*)s;
   to_return = strcmp(first->item, second->item);
   return to_return;
}

struct FIRST* sort_entries_first(struct FIRST* head)
{
   /* I want to use quicksort, but qsort function takes in an array,
	  and I have a linked list. Must convert. */
   int num_entries = 0;
   int i;
   struct FIRST* cur;
   struct FIRST* array;
	  
   cur = head;
   while (cur != NULL)
   {
	  num_entries += 1;
	  cur = cur->next;
   }

   array = malloc(sizeof(struct FIRST) * num_entries);

   cur = head;
   i = 0;
   while (cur != NULL)
   {
	  memcpy(&array[i], cur, sizeof(struct FIRST));
	  cur = cur->next;
	  i += 1;
   }

   qsort(array, num_entries, sizeof(struct FIRST), compare_entries_first);

   /* Put the entries back */
   cur = head;
   for (i = 0; i < num_entries; i++)
   {
      cur->item = array[i].item;
	  cur = cur->next;
   }

   free(array);

   return head;
}


int compare_entries_follow(const void* f, const void* s)
{
   struct FOLLOW* first;
   struct FOLLOW* second;
   int to_return;
   first = (struct FOLLOW*)f;
   second = (struct FOLLOW*)s;
   to_return = strcmp(first->item, second->item);
   return to_return;
}

struct FOLLOW* sort_entries_follow(struct FOLLOW* head)
{
   /* I want to use quicksort, but qsort function takes in an array,
	  and I have a linked list. Must convert. */
   int num_entries = 0;
   int i;
   struct FOLLOW* cur;
   struct FOLLOW* array;
	  
   cur = head;
   while (cur != NULL)
   {
	  num_entries += 1;
	  cur = cur->next;
   }

   array = malloc(sizeof(struct FOLLOW) * num_entries);

   cur = head;
   i = 0;
   while (cur != NULL)
   {
	  memcpy(&array[i], cur, sizeof(struct FOLLOW));
	  cur = cur->next;
	  i += 1;
   }

   qsort(array, num_entries, sizeof(struct FOLLOW), compare_entries_follow);

   /* Put the entries back */
   cur = head;
   for (i = 0; i < num_entries; i++)
   {
      cur->item = array[i].item;
	  cur = cur->next;
   }

   free(array);

   return head;
}


int main (int argc, char* argv[])
{
    int task,i=0;
    if (argc < 2)
    {
        printf("Error: missing argument\n");
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    // TODO: Read the input grammar at this point
    int t_token=getToken();   
 
    while(t_token != END_OF_FILE && t_token!= HASH)
    {
        //process first line and save non-terminals
        strcpy(non_terminal[non_terminals_count],current_token);       
        non_terminals_count++;
        t_token=getToken();
    }    

    //productions start now
        //printf("Check for productions::\n");
        int no_terminals_per_rule=0,k=0;
        char terminals_this_rule[20][20];        
        bool add=true;
        while(getToken() != DOUBLE_HASH)
        {
            ungetToken();
            int token_=getToken();    
            add=true;
                /* make sure                 
                      
            
            Start Updating grammar storage here
            insert whatever is coming in
            */
            insert_grammar_info(current_token);
            
            //save the terminals details
            switch(token_)
            {
                case ID: 
                    if(!is_non_terminal(current_token))
                    {
                        //add new terminal or update count
                        for(k=0;k<no_terminals_per_rule;k++)
                        {
                            if(strcmp(terminals_this_rule[k],current_token)==0)
                                add=false;
                        }
                        if(add==true)
                        {
                            insert_terminal(strdup(current_token));
                            strcpy(terminals_this_rule[no_terminals_per_rule],current_token);
                            no_terminals_per_rule++;
                        }
                    }
                    break;
                case HASH:
                     //reset current grammar rule stats
                     memset(terminals_this_rule, 0, 400);
                     no_terminals_per_rule=0;                     
                     break;
                                   
                default: break;
            }
        }

        //save Double hash denoting end of grammar rules        
        insert_grammar_info(current_token);

    /*
       Hint: You can modify and use the lexer from previous project
       to read the input. Note that there are only 4 token types needed
       for reading the input in this project.

       WARNING: You will need to modify lexer.c and lexer.h to support
                the project 3 input language.
     */
     
     initialize_sets(non_terminals_count);

    switch (task) {
        case 0:
        //printf("\nTask = 0\n");
        print_non_terminals();
        terminals=sort_terminals(terminals);
        print_terminal_info();
        //printf("\n Grammar rules are:: ");
        //print_grammar();
            // TODO: Output information about the input grammar
            break;

        case 1:
            // TODO: Calculate FIRST sets for the input grammar
            // Hint: You better do the calculation in a function and call it here!
            // TODO: Output the FIRST sets in the exact order and format required
                
                calculate_FIRST_sets();
                for(i=0;i<non_terminals_count;i++)
                    Head[i]=sort_entries_first(Head[i]);
                
                print_first_sets();
            break;

        case 2:
            // TODO: Calculate FIRST sets for the input grammar
            // TODO: Calculate FOLLOW sets for the input grammar
            // TODO: Output the FOLLOW sets in the exact order and format required
            // initialise follow set of start symbol with a $
            add_to_follow(0,"$");
            //printf(" Calculating first\n");          
            calculate_FIRST_sets();
            //printf(" completed first Calculating follow\n");          
            calculate_follow_sets();
            //printf(" Calculated follow\n");          
             for(i=0;i<non_terminals_count;i++)
             {
                    //printf("Sorting %d set\n",i);
                  Follow_Head[i]=sort_entries_follow(Follow_Head[i]);
             } 
            print_follow_sets();
            break;

        default:
            //printf("Error: unrecognized task number %d\n", task);
            break;
    }
    free(Head);
    free(Tail);
    free(Follow_Head);
    free(Follow_Tail);
    return 0;
}
