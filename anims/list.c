/******************************************************************
*    		Animation Package for Linked Lists		  *
*			Sreenivas Gukal				  *
*								  *
*		       File "list.c"				  *
******************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtango.h"

/* Animation Calls */

void init(), ins(), del();

static NAME_FUNCT map[] =  {{"Init", 1, {VOID, (FPTR)init}},
			    {"Insert", 1, {VOID, (FPTR)ins}},
			    {"Delete", 1, {VOID, (FPTR)del}}};

struct node 
	{
	char *data;
	struct node *next;
	} *head;

//extern char *malloc();

main()
{
	char op[20], str[20], str1[20], str2[20];
	struct node *ptr, *ptr2;
	int flag;
	int node_no;

		/* Initialize */

	TANGOalgoOp(map, "BEGIN");
	TANGOalgoOp(map, "Init");

	head = NULL;
        printf("Manipulating a linked list\n\n");
        printf("Options are: insert / delete / quit\n");
	printf("Next Op : ");
	scanf("%s",op);
	while (strcmp(op,"quit")) 
	    {
	    if (!strcmp(op,"insert"))
		{
		printf("Enter Data : ");
		scanf("%s",str);
		node_no = 0;
		ptr = (struct node *)calloc(1,sizeof(struct node));
//		ptr->data = calloc(strlen(str),sizeof(char));
//		strcpy(ptr->data, str);
		ptr->data=strdup(str);
		printf("Enter where to insert : start / end / after <node>\n");
		scanf("%s",str1);
		if (!strcmp(str1,"start"))
			{
			ptr->next = head;
			head = ptr;
			TANGOalgoOp(map, "Insert", str, node_no);
			}
		else if (!strcmp(str1,"end"))
			{
			ptr->next = NULL;
			if (head == NULL)
				{
				head = ptr;
				}
			else
				{
				ptr2 = head;
				++node_no;
				while (ptr2->next != NULL)
					{
					++node_no;
					ptr2 = ptr2->next;
					}
				ptr2->next = ptr;
				}
			TANGOalgoOp(map, "Insert", str, node_no);
			}
		else if (!strcmp(str1,"after"))
			{
			scanf("%s",str2);
			ptr2 = head;
			while ((ptr2 != NULL)&&(strcmp(str2,ptr2->data)))
				{
				++node_no;
				ptr2 = ptr2->next;
				}
			++node_no;
			if (ptr2 != NULL)
				{
				ptr->next = ptr2->next;
				ptr2->next = ptr;
				TANGOalgoOp(map, "Insert", str, node_no);
				}
			else
				printf("No node with data as %s\n",str2);
			}
		else
			printf("Incorrect specification - Please try again\n");
	    	}
	    else if (!strcmp(op,"delete"))
		{
		int flag = 0;

		printf("Enter data -- All nodes which match will be deleted\n");
		scanf("%s",str1);
		node_no = 0;
		ptr = head;
		ptr2 = NULL;
		while (ptr != NULL)
			{
			++node_no;
			if (!strcmp(str1,ptr->data))
				{
				if (ptr2 == NULL)
					head = head->next;
				else
					ptr2->next = ptr->next;
				TANGOalgoOp(map, "Delete", node_no,flag);
				--node_no;
				flag = node_no;
				}
			else
				ptr2 = ptr;
			ptr = ptr->next;
			}
		}
	    else
		printf("Improper Operation -- Enter insert / delete / quit\n");
	    printf("Next Op : ");
	    scanf("%s",op);
	    }
	TANGOalgoOp(map, "END");
}
