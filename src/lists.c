#include "lists.h"

#include "stdlib.h"
#include <stdio.h>
#include "raymath.h"

Vector2LinkedList createLinkedList()
{
    return (Vector2LinkedList) {
        .length = 0
    };
}


void pushLinkedList(Vector2LinkedList* linkedList, Vector2 value)
{
    linkedList->length++;

    if (linkedList->length-1 == 0)
    {
        struct LinkedListElement* elem = malloc(sizeof(struct LinkedListElement));
        elem->value = value;
        elem->nextListElem = NULL;
        linkedList->firstElem = elem;

        return;
    }

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<linkedList->length-2; i++) 
    {
        elem = elem->nextListElem;
    }
    struct LinkedListElement* newElem = malloc(sizeof(struct LinkedListElement));
    newElem->value = value;
    newElem->nextListElem = NULL;
    elem->nextListElem = newElem;
}

Vector2 popLinkedList(Vector2LinkedList* linkedList)
{
    if(linkedList->length == 0)
    {
        return Vector2Zero();
    }

    linkedList->length--;

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<linkedList->length-1; i++) 
    {
        elem = elem->nextListElem;
    }
    
    Vector2 value = elem->nextListElem->value;
   
    free (elem->nextListElem);

    elem->nextListElem = NULL;

    return value;
}


Vector2 getLinkedList(Vector2LinkedList* linkedList, int index)
{
    if(linkedList->length == 0)
    {
        return Vector2Zero();
    }

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<index; i++) 
    {
        elem = elem->nextListElem;
    }

    return elem->value;
}

void changeLinkedList(Vector2LinkedList* linkedList, int index, Vector2 value)
{
    if(linkedList->length == 0)
    {
        return;
    }

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<index; i++) 
    {
        elem = elem->nextListElem;
    }

    elem->value = value;
}


void removeLinkedList(Vector2LinkedList* linkedList, int index)
{
    if(linkedList->length == 0)
    {
        return;
    }

    linkedList->length--;

    if(index == 0)
    {
        struct LinkedListElement* deleteElem = linkedList->firstElem;
        linkedList->firstElem = linkedList->firstElem->nextListElem;
        free (deleteElem);
        return;
    }

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<index-1; i++) 
    {
        elem = elem->nextListElem;
    }

    struct LinkedListElement* deleteElem = elem->nextListElem;
    elem->nextListElem = elem->nextListElem->nextListElem;
    free (deleteElem);    
}

void printLinkedList(Vector2LinkedList* linkedList)
{
    printf("[");

    struct LinkedListElement* elem = linkedList->firstElem;
    for(int i=0; i<linkedList->length; i++) 
    {
        printf("%d, ", elem->value);
        elem = elem->nextListElem;
    }

    printf("]\n");
}


IntArrayList createArrayList()
{
    int defaultLenght = 10;

    return (IntArrayList) 
    {
        .length = defaultLenght,
        .highestUsedIndex = 0,
        .array = malloc(sizeof(int)*defaultLenght)
    };
}

int getIntArrayListItem(IntArrayList* list, int index)
{
    if(index >= list->highestUsedIndex)
    {
        return;
    }

    return list->array[index];
}

void setIntArrayListItem(IntArrayList* list, int index, int value)
{
    while(index >= list->length)
    {
        resizeIntArrayList(list, list->length*2);
    }

    list->array[index] = value;

    if(index > list->highestUsedIndex)
    {
        list->highestUsedIndex = index;
    }
}

void appendIntArrayList(IntArrayList* list, int value)
{
    setIntArrayListItem(list, list->highestUsedIndex+1, value);
}

void resizeIntArrayList(IntArrayList* list, int newSize)
{
    list->length *= 2;
    
    int* oldArray = list->array;
    list->array = malloc(sizeof(int)*list->length);

    for (int i=0; i<list->length/2; i++)
    {
        list->array[i] = oldArray[i];
    }
    free(oldArray);
}



