#ifndef HW4
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
	int price;
	int flag;
	int isLast;
	struct nodet* next;
}manaInTable;


void CreateProducts(FILE*, manotManager*);
void Error_Msg(char* str);
void PrintList(manotManager* head);
void PrintArr(manaInTable* head);
void AddItems(manotManager* manage, char* name, int addStock);
void OrderItem(int, char*, int, manaInTable**, manotManager*);
void RemoveItem(int, manaInTable**);
void RemoveTable(int, manaInTable**);
int dupe(char* name, manaInTable** tableArr);
void freeMemory(manotManager* manage);

#endif
