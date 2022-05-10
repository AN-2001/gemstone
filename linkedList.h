#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "status.h"


typedef void *ListVal;
typedef ListVal (*ListValCopy)(ListVal val);
typedef void (*ListValFree)(ListVal val);
typedef struct linkedList_t *LinkedList;
typedef void (*ListFunc)(ListVal item, void *arg);


// creates an empty linked list
LinkedList listCreate(ListValCopy copyVal, ListValFree freeVal);

// inserts newVal at the start of the list
StatusType listInsert(LinkedList list, ListVal newVal);

// clears the linked list
void listClear(LinkedList list);

// iterate over the list
void listIter(LinkedList list, ListFunc func, void *arg);

// frees the list
void listFree(LinkedList list);
#endif // LINKED_LIST_H
