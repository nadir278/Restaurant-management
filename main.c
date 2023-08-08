#include "HW4.h"

int main() 
{
	FILE* in,* inst;
	manotManager* manage, *temp;
	manaInTable* tableArr[50] = {NULL};
	int funcNum, size, tableNum;
	char mana[51];
	manage = (manotManager*)malloc(sizeof(manotManager));
	if ((in = fopen("Manot.txt","r")) == NULL)
		Error_Msg("input error");

	if ((inst = fopen("Instructions.txt","r")) == NULL)
		Error_Msg("input error");

	while (fscanf(inst,"%d",&funcNum) != EOF)
	{
		if (funcNum==1)
			CreateProducts(in, manage);
		if (funcNum==2)
		{
			fscanf(inst, "%s%d", &mana, &size);
			AddItems(manage, mana, size);
		}
		if (funcNum==3)
		{
			fscanf(inst, "%d%s%d", &tableNum, &mana, &size);
			OrderItem(tableNum, mana, size, tableArr, manage);
		}
		if (funcNum==4)
		{
			fscanf(inst, "%d",&tableNum);
			RemoveItem(tableNum, tableArr);
		}
		if (funcNum==5)
		{
			fscanf(inst, "%d", &tableNum);
			RemoveTable(tableNum, tableArr);
		}
	}
	fclose(in);
	fclose(inst);
	while (manage->head != NULL)
	{
		temp = manage->head;
		manage->head = manage->head->next;
		free(temp);
	}
	return 0;
}


