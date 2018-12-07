#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

varList_t *pHead;
/*
 * Function:  SymTable_createNode 
 * ------------------
 * Creates a new node in heap
 * Assigns variable attributes to the new node
 * 
 * @parameter: varName - Variable Name
   @parameter: value - value to be assigned to the variable
 * @return: pointer to the node
 */
static varList_t *SymTable_createNode(char *var_name)
{
    varList_t *temp;
                                    
    temp = (varList_t *)malloc(sizeof(varList_t));
    
    if (temp == NULL)
    {
        printf("SymTable: var %s malloc failed!", var_name);
        exit(EXIT_FAILURE); 
    }                        
    
    temp->next = NULL;  
    temp->count = 1;
    temp->regName = 0;
    strcpy(temp->var_name, var_name);                                                         
   
    return temp;
}

/*
 * Function:  SymTable_addToList 
 * ------------------
 * Adds a new node to the symbol table
 * 
 * @parameter: node - pointer to the node to be added
 * @return: void
 */
static void SymTable_addToList (varList_t *node)
{
    //add the new node to the last free slot in singly linked list                                                                
    if (pHead == NULL)
    {
        pHead = node;                                                
    }
    else
    {
        varList_t *cursor;
        
        cursor  = pHead;                                             
        
        while (cursor->next != NULL)
        {
            cursor = cursor->next;                                  
        }
        
        cursor->next = node;                                        
    }
}

/*
 * Function:  SymTable_createVar 
 * ------------------
 * Adds a new variable to the linkedlist based symbol table. The newly created nodes are always
 * added to the end of the singly linked list
 * 
 * @parameter: varName - Variable Name
   @parameter: value - value to be assigned to the variable
 * @return: void
 */
static void SymTable_createVar(char *varName)
{
    varList_t *node;
    
    node = SymTable_createNode(varName);
    
    SymTable_addToList(node);                       
}

/*
 * Function:  SymTable_getNode 
 * --------------------
 * Searches the symbol table for the specified variable
 *
 *  returns: pointer to the specified node if the node is found
 *           else returns NULL pointer
 */
varList_t * SymTable_getNode(char *varName)
{
    varList_t *cursor;
    
    cursor = pHead;
    
    while (cursor != NULL)
    {
        if (strcmp(varName, cursor->var_name) == 0)
        {
            return cursor;
        }

        cursor = cursor->next;
    }
   
    return NULL;

}

void SymTable_addVariable(char *varName)
{
    varList_t *cursor;

    if ( (cursor = SymTable_getNode(varName)) != NULL)
    {
        cursor->count++;
    }
    else
    {
        SymTable_createVar(varName);
    }
}

/*
 * Function:  SymTable_freeList 
 * --------------------
 * Frees up the dynamically allocated nodes 
 *
 * @return: void
 */
void SymTable_traverseList(FILE *fp_report)
{
    varList_t *cursor;

    cursor = pHead;

    fprintf(fp_report, "Variable Allocation baseed on most frequently used first: \n");
    
    while(cursor != NULL)
    {
        char loc[20];
        if (cursor->regName != 0)
        {
            sprintf(loc, "register r%d", cursor->regName);
        }
        else
        {
            sprintf(loc, "memory @%s", cursor->var_name);
        }
        fprintf(fp_report, "variable %s (frequency = %d) -> %s\n", cursor->var_name, cursor->count, loc);
        cursor = cursor->next;
    }
}

/*
 * Function:  SymTable_freeList 
 * --------------------
 * Frees up the dynamically allocated nodes 
 *
 * @return: void
 */
int SymTable_getLen(void)
{
    varList_t *cursor;
    int listLen = 0;

    cursor = pHead;
    
    while(cursor != NULL)
    {
        listLen++;
        
        cursor = cursor->next;
    }

    return listLen;
}

/*
 * Function:  SymTable_freeList 
 * --------------------
 * Frees up the dynamically allocated nodes 
 *
 * @return: void
 */
void SymTable_freeList(void)
{
    varList_t *cursor;
    varList_t *prev;
    
    prev = NULL;
    cursor = pHead;
    
    while(cursor != NULL)
    {
        prev = cursor;
        cursor = cursor->next;

        free(prev);
    }
}