#include<stdio.h>
#include<stdlib.h>

struct node
{
    int data;
    struct node *next;
}**head,**tail,*temp;

void initialise_array(int n)
{
    head=(struct node**)malloc(n * sizeof(struct node*));
    tail=(struct node**)malloc(n * sizeof(struct node*));
    //printf("init complete");
}

void print_array()
{
    int i=0;
    head[0]->data=1;
    printf("\t %p %d",head[0],head[0]->data);
    head[0]->next=head[1];
    
    head[1]->data=2;
    printf("\t %p %d %p",head[1],head[1]->data,head[0]->next);
    
    head[2]->data=3;
    printf("\t %p %d",head[2],head[0]->data);
    
    printf("\t %p",head[3]);
    
    printf("\t %p",head[4]);
}
int main()
{
    int x;
    printf("Enter a number :: ");
    scanf("%d",&x);
    initialise_array(x);
    print_array();
    
    return 0;
}

