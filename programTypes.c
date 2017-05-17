/*
    OverHeated alpha 0.2
    Copyright (C) 2015-2017 BrokenTestType

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/*******************************************
    programTypes.c
    Functions for manipulating program types
    (defined on d_programTypes.h)
*******************************************/

//
#include "stdio.h"
#include "stdlib.h"
//
#include "d_programTypes.h"
//

#ifndef PROGRAM_TYPES_C
#define PROGRAM_TYPES_C
#endif

/*******************************************
    newLinkedList
    Returns a new linked list with initialized fields
*******************************************/

static LList_t templateLinkedList;
//
LList_t newLinkedList(void)
{
    return templateLinkedList;
}

/*******************************************
    addLLNode
    Add linked list node
*******************************************/

int addLLNode(LList_t *list , void *data)
{
    LListNode_t *newNode = malloc( sizeof(LListNode_t));
    newNode->data = data;
    newNode->next = NULL;

    if(list->node == NULL)
    {
        list->node = newNode;
        list->last = newNode;
        return 0;
    }
    //else
    list->last->next = newNode;
    list->last = newNode;
    
    return 0;
}

/*******************************************
    applyToList
*******************************************/


int applyToList(const LList_t *list ,void (*func)(void *) /*void *func*/)
{
    LListNode_t *cNode;     //CurrentNode
    int i = 0;
    for( cNode = list->node;
         cNode != NULL;
         cNode = cNode->next)
    {
        (*( void (*) (void *))(func))(cNode->data);
        i++;
    }
    return 0;
}

/*******************************************
    filterList
    Apply filter function to all nodes
    if filter function returns != 0
    remove node
*******************************************/

int filterList(LList_t *list ,int (*func)(void *) /*void *func*/)
{
    LListNode_t *cNode;     //CurrentNode
    LListNode_t *lastNode = NULL;
    LListNode_t *nextNode;
    int funcRet = 0;
    for( cNode = list->node;
         cNode != NULL;
         cNode = nextNode)
    {
        nextNode = cNode->next;
        funcRet = (*( int (*) (void *))(func))(cNode->data);
        if(funcRet)
        {//Remove node
            //If it is the first node
            if(lastNode == NULL)
            {
                list->node = cNode->next;
            }
            else
            {
                lastNode->next = cNode->next;
            }
            //If it is the last node
            if(cNode == list->last)
            {
                list->last = lastNode;
            }     
            //free node memory
            free(cNode);
        }
        else
            lastNode = cNode;
    }/*end for*/
    return 0;
}

/*******************************************
    freeLList
*******************************************/

int freeLList(LList_t *list)
{
    LListNode_t *cNode;     //CurrentNode
    LListNode_t *nextNode;

    for( cNode = list->node;
         cNode != NULL;
         cNode = nextNode)
    {
        nextNode = cNode->next;
        free(cNode);
    }

    list->node = NULL;
    list->last = NULL;
    
    return 0;
}

/*******************************************
    printList
*******************************************/

int printList(LList_t *list)
{
    LListNode_t *cNode;     //CurrentNode
    printf("List:\n");
    int i = 0;
    for( cNode = list->node;
         cNode != NULL;
         cNode = cNode->next)
    {
        printf("\tNode %d , Address: %x\n", i , cNode->data);
        i++;
    }
    
    return 0;
}

