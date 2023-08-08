#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
	char* name;
	int stock;
	int price;
	struct node* next;
}manot;

typedef struct manotManager
{
	manot* head;
	manot* tail;
}manotManager;

typedef struct nodet
{
	char* name;
	int quan;
	struct nodet* next;
}manaInTable;

void CreateProducts(FILE*, manotManager*);
void Error_Msg(char* str);
void PrintList(manotManager* head);
void PrintArr(manaInTable* head);
void AddItems(manotManager* manage, char* name, int addStock);
void OrderItem(int, char*, int, manaInTable**, manotManager*);
void RemoveItem(int, manaInTable**);

int main() 
{
	FILE* in;
	manotManager* manage;
	manaInTable* tableArr[50] = {NULL};
	manage = (manotManager*)malloc(sizeof(manotManager));
	if ((in = fopen("Manot.txt","r")) == NULL)
		Error_Msg("input error");
	CreateProducts(in,manage);
	fclose(in);
	AddItems(manage, "Steak", 100);
	OrderItem(5, "Fish", 3, tableArr, manage);
	OrderItem(5, "Steak", 10, tableArr, manage);
	RemoveItem(5, tableArr);
	PrintArr(tableArr[4]);
	PrintList(manage);
	
	return 0;
}

//prints error msg and exit the program
void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}

void CreateProducts(FILE* in, manotManager* manage)//converts file to linked list
{
	manot * temp, *current;
	manage->head = NULL;
	char temp_name[51];
	int len;
	temp = (manot*)malloc(sizeof(manot));
	if (temp == NULL)
		Error_Msg("\nERROR ALLOCATING NAME"); 
	while (fscanf(in,"%s%d%d" ,temp_name, &temp->stock, &temp->price) != EOF)//get details from file
	{
		//dynamically allocate name
		len = strlen(temp_name);
		temp->name = (char*)malloc(len * sizeof(char));
		if (temp->name == NULL)
			Error_Msg("\nERROR ALLOCATING NAME");
		strcpy(temp->name, temp_name);

		//check if there are 2 names that are the same
		current = manage->head;
		while (current != NULL)
		{
			if (strcmp(current->name, temp->name) == 0)
			{
				printf("\nERROR 2 MANOT WITH THE SAME NAME");
				free(current->name);
				continue;
			}
			current = current->next;
		}

		//check if one of the stocks is unvalid
		if (temp->stock < 0)
		{
			printf("\nONE OF THE STOCKS IS UNVAILD");
			free(current->name);
			continue;
		}

		//check if one of the prices is unvalid
		if (temp->price < 0)
		{
			printf("\nONE OF THE PRICES IS UNVAILD");
			free(current->name);
			continue;
		}

		//add next node to end of list
		if (manage->head == NULL)
		{
			manage->head = temp;
			manage->tail = manage->head;
			manage->head->next = NULL;
		}
		else
		{
			manage->tail->next = temp;
			manage->tail = manage->tail->next;
			manage->tail->next = NULL;
		}
		

		temp = (manot*)malloc(sizeof(manot));
		if (temp == NULL)
			Error_Msg("\nERROR ALLOCATING NAME");
	}
	return manage->head;
}

void AddItems(manotManager* manage,char* name, int addStock)//add stock to an item
{
	manot* temp = manage->head;
	if (addStock < 0)//check if stock needed to add is above 0
	{
		printf("\nADD STOCK TO ITEM CANNOT BE BELOW 0");
		return;
	}
	
	//runs for the length of the list
	while (temp != NULL)
	{
		if (strcmp(temp->name, name) == 0)//check if the current dish name is the name that needs to add stock to
		{
			temp->stock += addStock;
			return;
			break;
		}
		temp = temp->next;
	}
	printf("\nERROR FINDING PRODUCT TO ADD STOCK TO (INVAILD NAME)");//no dish with that name was found
}

//creates a node in a linked list for the order the order the function gets
void OrderItem(int TableNumber, char* ProductName, int Quantity, manaInTable** tableArr, manotManager* manager)
{
	int i, len;
	manot* temp=manager->head;
	manaInTable* head, *tabletemp;
	
	//check if such a table exists
	if (TableNumber > 50 || TableNumber <= 1)
	{
		printf("\nTABLE NUMBER FOR ORDER IS UNVALID");
		return;
	}

	//check if there is enough of the requested dish in the kitchen
	while (temp != NULL)
	{
		if ((temp->stock < Quantity) && (strcmp(temp->name, ProductName) == 0))
		{
			printf("\nnot enough of dish in stock");
			return;
		}
		temp = temp->next;
	}

	//input the order into a linked list in the table array
	head = tableArr[TableNumber-1];
	temp = manager->head;
	tabletemp = head;
	while (temp != NULL)
	{
		if (strcmp(temp->name, ProductName) == 0)//check if the order product name is in the kitchen
		{
			temp->stock -= Quantity;
			tabletemp = (manaInTable*)malloc(sizeof(manaInTable));
			if (tabletemp == NULL)
				Error_Msg("\nERROR ALLOCATING TABLE");

			//dynamically allocate name
			len = strlen(temp->name);
			tabletemp->name = (char*)malloc(len * sizeof(char));
			if (tabletemp->name == NULL)
				Error_Msg("\nERROR ALLOCATING NAME");
			strcpy(tabletemp->name, temp->name);

			if (tableArr[TableNumber] == NULL)//check if this table has ordered this dish before
				tabletemp->quan = 0;
			tabletemp->quan += Quantity;
			tabletemp->next = head;
			head = tabletemp;
		}
		temp = temp->next;
	}
	tableArr[TableNumber-1] = head;
}

void RemoveItem(int TableNumber, manaInTable** tableArr)
{
	char product[51];
	manaInTable* head, * temp;
	//check if such a table exists
	if (TableNumber > 50 || TableNumber <= 1)
	{
		printf("\nTABLE NUMBER FOR ORDER IS UNVALID");
		return;
	}  

	//check if the table has orderd anyting
	if (tableArr[TableNumber-1] == NULL)
	{
		printf("\nTHE TABLE HASEND ORDERD ANYTING YET");
		return;
	}

	head = tableArr[TableNumber-1];
	temp = head;
	head = head->next;
	strcpy(product, temp->name);
	free(temp);
	printf("Dish %s was removed from the table", product);
	tableArr[TableNumber - 1] = head;
}


void PrintList(manotManager* manage)
{
	manot* temp = manage->head;
	while (temp != NULL)
	{
		printf("\n%s %d %d",temp->name,temp->stock,temp->price);
		temp = temp->next;
	}
}

void PrintArr(manaInTable* head)
{
	manaInTable* temp = head;
	while (temp != NULL)
	{
		printf("\n%s %d", temp->name, temp->quan);
		temp = temp->next;
	}
}