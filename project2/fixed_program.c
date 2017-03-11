/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Adam Doupe, 2016
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 255

struct entry {
   char* name;
   int amount;
   struct entry* next;
};

char current_token[MAX_TOKEN_LENGTH];

char* parse_word()
{
   int i = 0;
   int done = 0;
   while (!done)
   {
	  char c;
	  c = getc(stdin);
	  if (isspace(c))
	  {
		 done = 1;
		 ungetc(c, stdin);
	  }
	  else if (feof(stdin))
	  {
		 done = 1;
	  }
	  else
	  {
		 current_token[i] = c;
		 i++;
		 if (i == MAX_TOKEN_LENGTH)
		 {
			printf("Error, token length was too big\n");
			return NULL;
		 }
	  }
   }
   current_token[i] = '\0';
   return current_token;
}

void skip_space()
{
    char c;

    c = getchar();
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
    }
    // return character to input buffer if eof is not reached
    if (!feof(stdin))
    {
        ungetc(c,stdin);
    }
}


struct entry* parse_input()
{
   struct entry* head = NULL;
   struct entry* cur = NULL;
   struct entry* prev = NULL;
   int done = 0;

   while (!done)
   {
	  char *input;
	  char nam[256];
	  input = parse_word();
	  if (input == NULL)
	  {
		 done = 1;
	  }
	  else if (feof(stdin))
	  {
		 done = 1;
	  }
	  else
	  {
	     //char *nam;
         //nam=(char*)malloc(sizeof(char));
	     //strcpy(nam,input_1);
	 
		 cur = (struct entry*)malloc(sizeof(struct entry));
         //cur->name=nam;
         /***
           fix to maintain cur->name 
         ***/
         cur->name=strdup(input);
         cur->next = NULL;
		 
		 if(!prev)
		 {
		    prev = (struct entry*)malloc(sizeof(struct entry));
		    prev->next = NULL;
         }
         
		 skip_space();
		 input = parse_word();   
		 if (input == NULL)
		 {
			free(cur->name);
			free(cur);
			done = 1;
		 }
		 else
		 {
			int i, len;
			len = strlen(input);
			if (len == 0)
			{
			   printf("Error, amount must be a number\n");
			   return NULL;
			}
			for (i = 0; i < len; i++)
			{
			   if (!isdigit(input[i]))
			   {
				  printf("Error, amount must be a number\n");
				  return NULL;
			   }
			}
			cur->amount = atoi(input);
			if (head == NULL)
			{
			    /**
			        Update Prev Pointer
			    **/
			   head = cur;			
			   prev=head;
			}
			else
			{
			
			   prev->next = cur;
			   prev = cur;
			}
			skip_space();
		 }
	  }
   }

   // head points to the start of the list
    return head;
}

int compare_entries(const void* f, const void* s)
{
   struct entry* first;
   struct entry* second;
   int to_return;
   first = (struct entry*)f;
   second = (struct entry*)s;
   to_return = strcmp(first->name, second->name);
   if (to_return == 0)
   {
        // fix for ascending order 
	  to_return = first->amount - second->amount;
   }
   return to_return;
}

struct entry* sort_entries(struct entry* head)
{
   /* I want to use quicksort, but qsort function takes in an array,
	  and I have a linked list. Must convert. */
   int num_entries = 0;
   int i;
   struct entry* cur;
   struct entry* array;
	  
   cur = head;
   while (cur != NULL)
   {
	  num_entries += 1;
	  cur = cur->next;
   }

   array = malloc(sizeof(struct entry) * num_entries);

   cur = head;
   i = 0;
   while (cur != NULL)
   {
	  memcpy(&array[i], cur, sizeof(struct entry));
	  cur = cur->next;
	  i += 1;
   }

   qsort(array, num_entries, sizeof(struct entry), compare_entries);

   /* Put the entries back */
   cur = head;
   for (i = 0; i < num_entries; i++)
   {
      cur->name = array[i].name;
	  cur->amount = array[i].amount;
	  cur = cur->next;
   }

   free(array);

   return head;
}

void print_entries(struct entry* head)
{
   struct entry* cur;
   cur = head;
   while (cur != NULL)
   {
	  printf("%s %d\n", cur->name, cur->amount);
	  cur = cur->next;
   }
}

int main()
{
   struct entry* head = NULL;

   head = parse_input();

   if (head == NULL)
   {
	  printf("Error, no input received.\n");
	  return -1;
   }

   head = sort_entries(head);

   print_entries(head);
   
   return 0;
}
