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
    d_programTypes.h
    Program Types 
    and functions for manipulating program types
*******************************************/

#ifndef D_PROGRAM_TYPES_H
#define D_PROGRAM_TYPES_H


/*******************************************/

struct vector
{
    double x;
    double y;
};

typedef struct vector vector;

/*******************************************
Generic Simple Linked list
*******************************************/

struct LListNode_t
{
    struct LListNode_t *next; //Next Node
    void *data;               //Node data
                              
};
typedef struct LListNode_t LListNode_t;
struct LList_t
{
    LListNode_t *node;  //Linked list nodes
    LListNode_t *last;  //Last node on the list
    //(...)
};
typedef struct LList_t LList_t;

/*******************************************
    Functions
*******************************************/

//private:
#ifdef PROGRAM_TYPES_C

#endif

//Public:
LList_t newLinkedList(void);                                //Returns an initialized linked list
int addLLNode(LList_t *list , void *data);                  //Add node to list
//int applyToList(const LList_t *list , void *func);        //Apply function to all the nodes on the list
int applyToList(const LList_t *list ,void (*func)(void *)); //Apply function to all the nodes on the list
int filterList(       LList_t *list ,int (*func)(void *));   //Apply filter function to all nodes
                                                            //if filter function returns != 0
                                                            //remove node
int freeLList(LList_t *list);                               //Free linked list
int printList(LList_t *list);

#endif 
