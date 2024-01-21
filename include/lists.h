#ifndef LISTS_H_
#define LISTS_H_

#include "raylib.h"

struct LinkedListElement
{
    Vector2 value;
    struct LinkedListElement* nextListElem;
};


typedef struct Vector2LinkedList
{
    int length;
    struct LinkedListElement* firstElem;
} Vector2LinkedList;


Vector2LinkedList createLinkedList();
void pushLinkedList(Vector2LinkedList* linkedList, Vector2 value);
Vector2 popLinkedList(Vector2LinkedList* linkedList);
Vector2 getLinkedList(Vector2LinkedList* linkedList, int index);
void changeLinkedList(Vector2LinkedList* linkedList, int index, Vector2 value);
void removeLinkedList(Vector2LinkedList* linkedList, int index);
void printLinkedList(Vector2LinkedList* linkedList);



typedef struct IntArrayList
{
    int* array;
    int length;
    int highestUsedIndex;
}IntArrayList;


IntArrayList createArrayList();
int getIntArrayListItem(IntArrayList* list, int index);
void setIntArrayListItem(IntArrayList* list, int index, int value);
void resizeIntArrayList(IntArrayList* list, int newSize);
void appendIntArrayList(IntArrayList* list, int value);




#endif