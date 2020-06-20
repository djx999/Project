#include<List.h>


void L_Init(List **plist)
{
	*plist = (List*)malloc(sizeof(List));
	(*plist)->nCount = 0;
	(*plist)->pHead = NULL;
	(*plist)->pEnd = NULL;
}
void L_Push(List *plist,void* nValue)
{
	if(plist == NULL) return;
	MyList *pTemp = NULL;
	pTemp = (MyList*)malloc(sizeof(MyList));
	pTemp->nValue = nValue;
	pTemp->pNext = NULL;

	if(plist->pHead == NULL)
	{
		plist->pHead = pTemp;
		plist->pEnd = pTemp;
	}
	else
	{
		plist->pEnd->pNext = pTemp;
		plist->pEnd = pTemp;
	}
	plist->nCount++;

}
void* L_Front(List *plist)
{
	if(plist == NULL) return NULL;
	return plist->pHead->nValue;
}
int  L_Pop(List *plist)
{
	if(plist == NULL) return -1;
	MyList *pMark = plist->pHead;
	plist->pHead = plist->pHead->pNext;
	free(pMark);
	pMark = NULL;
	plist->nCount--;
}
int L_Isempty(List *plist)
{
	if(plist == NULL) return -1;
	return plist->nCount==0?1:0;
}

