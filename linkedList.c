#include "linkedList.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#define nodeCreate(init) (_nodeCreate(init, list->copyVal)) 
#define nodeFree(node) (_nodeFree(node, list->freeVal)) 
typedef struct listNode_t{
	ListVal val;
	struct listNode_t *next;
	struct listNode_t *prev;
}*ListNode;

static ListNode _nodeCreate(ListVal init, ListValCopy copy){
	ListNode ret = (ListNode)malloc(sizeof(*ret));	
	if(!ret)
		return NULL;
	ret->next = ret->prev = ret;	
	ret->val = copy(init);
	if(!ret->val && init){
		free(ret);
		return NULL;
	}
	return ret;
}


static void _nodeFree(ListNode node, ListValFree freeVal){
	if(!node || !freeVal)
		return;
	if(!node->val){
		free(node);
		return;
	}
	freeVal(node->val);
	free(node);
}


struct linkedList_t{
	ListNode sentinel;	
	ListValCopy copyVal;
	ListValFree freeVal;
};

LinkedList listCreate(ListValCopy copy, ListValFree free){
	if(!copy)
		return NULL;

	LinkedList list = (LinkedList)malloc(sizeof(*list));
	if(!list)
		return NULL;
	list->copyVal = copy;
	list->freeVal = free;
	list->sentinel = nodeCreate(NULL);
	if(!list->sentinel){
		free(list);
		return NULL;
	}
	return list;
}

StatusType listInsert(LinkedList list, ListVal newVal){
	if(!list)
		return INVALID_INPUT;
	if(!list->copyVal)
		return FAILURE;

	ListNode newNode = nodeCreate(newVal);
	newNode->next = list->sentinel->next;
	newNode->prev = list->sentinel;
	list->sentinel->next->prev = newNode;
	list->sentinel->next = newNode;
	return SUCCESS;
}

static StatusType listDeleteNode(LinkedList list, ListNode toDelete){
	if(!list)
		return INVALID_INPUT;
	if(!list->freeVal)
		return FAILURE;
	if(!toDelete)
		return FAILURE;
	toDelete->prev->next = toDelete->next;
	toDelete->next->prev = toDelete->prev;
	nodeFree(toDelete);
	return SUCCESS;
}
void listIter(LinkedList list, ListFunc func, void *arg){
	if(!list || !func || !arg)
		return;
	ListNode curr = list->sentinel->next;
	while(curr != list->sentinel){
		func(curr->val, arg);
		curr = curr->next;
	}
}
void listClear(LinkedList list){
	if(!list)
		return;
	while(list->sentinel->next != list->sentinel){
		listDeleteNode(list, list->sentinel->next);
	}
}

void listFree(LinkedList list){
	if(!list)
		return;
	ListNode curr = list->sentinel->next;
	while(curr != list->sentinel){
		ListNode next = curr->next;	
		nodeFree(curr);
		curr = next;
	}
	nodeFree(list->sentinel);
	free(list);
}
