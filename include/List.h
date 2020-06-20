#include<stdio.h>
#include<stdlib.h>


typedef struct  node2
{
	void   *nValue;
	struct node2 *pNext;

}MyList;

typedef struct node3
{
	int nCount;
	MyList *pHead;
	MyList *pEnd;

}List;

void  L_Init(List **plist);
void  L_Push(List *plist,void* nValue);
void* L_Front(List *plist);
int   L_Pop(List *plist);
int   L_Isempty(List *plist);

