#include "HW4.h"

//prints error msg and exit the program
void Error_Msg(char* str)
{
	printf("%s\n", str);
	exit(1);
}

void CreateProducts(FILE* in, manotManager* manage)//converts file to linked list
{
	manot* temp, * current;
	manage->head = NULL;
	char temp_name[51];
	int len;
	temp = (manot*)malloc(sizeof(manot));
	if (temp == NULL)
		Error_Msg("ERROR ALLOCATING NAME\n");
	while (fscanf(in, "%s%d%d", temp_name, &temp->stock, &temp->price) != EOF)//get details from file
	{
		//dynamically allocate name
		len = strlen(temp_name);
		temp->name = (char*)malloc(len * sizeof(char));
		if (temp->name == NULL)
		{
			freeMemory(manage);
			Error_Msg("ERROR ALLOCATING NAME\n");
		}
		strcpy(temp->name, temp_name);

		//check if there are 2 names that are the same
		current = manage->head;
		while (current != NULL)
		{
			if (strcmp(current->name, temp->name) == 0)
			{
				printf("ERROR 2 MANOT WITH THE SAME NAME\n");
				free(current->name);
				continue;
			}
			current = current->next;
		}

		//check if one of the stocks is unvalid
		if (temp->stock < 0)
		{
			printf("ONE OF THE STOCKS IS UNVAILD\n");
			free(current->name);
			continue;
		}

		//check if one of the prices is unvalid
		if (temp->price < 0)
		{
			printf("ONE OF THE PRICES IS UNVAILD\n");
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
		{
			freeMemory(manage);
			Error_Msg("ERROR ALLOCATING NAME\n");
		}
	}
	printf("The kitchen was created\n");
	return manage->head;
}

void AddItems(manotManager* manage, char* name, int addStock)//add stock to an item
{
	manot* temp = manage->head;
	if (addStock < 0)//check if stock needed to add is above 0
	{
		printf("ADD STOCK TO ITEM CANNOT BE BELOW 0\n");
		return;
	}

	//runs for the length of the list
	while (temp != NULL)
	{
		if (strcmp(temp->name, name) == 0)//check if the current dish name is the name that needs to add stock to
		{
			temp->stock += addStock;
			printf("%d %s were added to the kitchen\n", addStock, name);
			return;
		}
		temp = temp->next;
	}
	printf("We dont have %s in the kitchen\n", name);//no dish with that name was found
}

//creates a node in a linked list for the order the order the function gets
void OrderItem(int TableNumber, char* ProductName, int Quantity, manaInTable** tableArr, manotManager* manager)
{
	int i, len;
	manot* temp = manager->head;
	manaInTable* head, * tabletemp;

	//check if such a table exists
	if (TableNumber > 50 || TableNumber < 1)
	{
		printf("Table doesnt exist\n");
		return;
	}

	//check if there is enough of the requested dish in the kitchen
	while (temp != NULL)
	{
		if ((temp->stock < Quantity) && (strcmp(temp->name, ProductName) == 0))
		{
			printf("not enough of dish in stock\n");
			return;
		}
		temp = temp->next;
	}

	//input the order into a linked list in the table array
	head = tableArr[TableNumber - 1];
	temp = manager->head;
	tabletemp = head;
	while (temp != NULL)
	{
		if (strcmp(temp->name, ProductName) == 0)//check if the order product name is in the kitchen
		{
			temp->stock -= Quantity;
			tabletemp = (manaInTable*)malloc(sizeof(manaInTable));
			if (tabletemp == NULL)
			{
				freeMemory(manager);
				Error_Msg("ERROR ALLOCATING TABLE\n");
			}
			//dynamically allocate name
			len = strlen(temp->name);
			tabletemp->name = (char*)malloc(len + 1 * sizeof(char));
			if (tabletemp->name == NULL)
				Error_Msg("ERROR ALLOCATING NAME\n");
			strcpy(tabletemp->name, temp->name);

			if (tableArr[TableNumber] == NULL)//check if this table has ordered this dish before
				tabletemp->quan = 0;
			tabletemp->quan += Quantity;
			tabletemp->price = temp->price;
			tabletemp->flag = 0;
			tabletemp->isLast = 1;
			if (head != NULL)
				head->isLast = 0;
			tabletemp->next = head;
			head = tabletemp;
		}
		temp = temp->next;
	}
	tableArr[TableNumber - 1] = head;
	printf("%d %s were added to table number %d\n", Quantity, ProductName, TableNumber);
}

void RemoveItem(int TableNumber, manaInTable** tableArr)
{
	char product[51];
	int quan = 0;
	manaInTable* head, * temp;
	//check if such a table exists
	if (TableNumber > 50 || TableNumber <= 1)
	{
		printf("Table does not exist\n");
		return;
	}

	//check if the table has orderd anyting
	if (tableArr[TableNumber - 1] == NULL)
	{
		printf("The table %d hasent orderd yet\n", TableNumber);
		return;
	}

	//check if the table wants to return the last order
	if (tableArr[TableNumber - 1]->isLast == 0)
	{
		printf("The table %d Can only return the last order\n", TableNumber);
		return;
	}

	//remove last order
	head = tableArr[TableNumber - 1];
	temp = head;
	head = head->next;
	strcpy(product, temp->name);
	quan = temp->quan;
	free(temp);
	printf("%d %s were returned to the kitchen from table %d\n", quan, product, TableNumber);
	tableArr[TableNumber - 1] = head;
}

void RemoveTable(int TableNumber, manaInTable** tableArr)
{
	manot* temp;
	int sum = 0, tip, count = 0;
	//check if such a table exists
	if (TableNumber > 50 || TableNumber < 1)
	{
		printf("Table does not exist\n");
		return;
	}

	//check if the table has orderd anyting
	if (tableArr[TableNumber - 1] == NULL)
	{
		printf("The table %d hasent orderd yet\n", TableNumber);
		return;
	}

	while (tableArr[TableNumber - 1] != NULL)//clears the table orders
	{
		if ((tableArr[TableNumber - 1]->flag == 0))//checks if this is the last order of the same dish
		{
			count = dupe(tableArr[TableNumber - 1]->name, tableArr[TableNumber - 1]);
			printf("%d %s ", count, tableArr[TableNumber - 1]->name);
			sum += count * tableArr[TableNumber - 1]->price;
			temp = tableArr[TableNumber - 1];
			tableArr[TableNumber - 1] = tableArr[TableNumber - 1]->next;
			free(temp->name);
			free(temp);
		}
		else//if the dish name was already in the table it was already calculated to the price
		{
			temp = tableArr[TableNumber - 1];
			tableArr[TableNumber - 1] = tableArr[TableNumber - 1]->next;
			free(temp);
		}

	}
	tip = sum * 0.15;
	printf(". %d nis+%d nis for tips, please!\n", sum, tip);
}

int dupe(char* name, manaInTable* head)//check for dupes in order names and returns the overall quantity
{
	manaInTable* temp = head;
	int quan = 0;
	while (temp != NULL)
	{
		if (strcmp(name, temp->name) == 0)//if the manot has the same name
		{
			quan += temp->quan;
			temp->flag = 1;
		}
		temp = temp->next;
	}
	return quan;
}

void freeMemory(manotManager* manage)//frees memory
{
	manotManager* temp;
	while (manage->head != NULL)
	{
		temp = manage->head;
		free(temp->head->name);
		manage->head = manage->head->next;
		free(temp);
	}
}