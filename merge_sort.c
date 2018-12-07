#include <stdio.h>
#include "list.h"
#include "merge_sort.h"

varList_t* SortedMerge(varList_t *a, varList_t *b) ; 
void FrontBackSplit(varList_t * input, varList_t **left, varList_t **right) ; 

void FrontBackSplit(varList_t * input, varList_t **left, varList_t **right) 
{ 
    varList_t *fast; 
    varList_t *slow;

    slow = input; 
    fast = input->next;
  
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast) 
    { 
        fast = fast->next; 
        if (fast) 
        { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
  
    /* 'slow' is before the midpoint in the list, so split it in two 
    at that point. */
    *left = input; 
    *right = slow->next; 
    slow->next = NULL; 
} 

void MergeSort(varList_t **listHead) 
{ 
    varList_t *pHead = *listHead;
    varList_t *a; 
    varList_t *b; 

    /* Base case -- length 0 or 1 */
    if ((pHead == NULL) || (pHead->next == NULL)) 
    { 
        return; 
    } 
   
    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(pHead, &a, &b); 
  
    /* Recursively sort the sublists */
    MergeSort(&a); 
    MergeSort(&b); 
  
    /* answer = merge the two sorted lists together */
    *listHead = SortedMerge(a, b); 
} 

varList_t* SortedMerge(varList_t *a, varList_t *b) 
{ 
    varList_t *result = NULL; 
  
    /* Base cases */
    if (a == NULL) 
        return(b); 
    else if (b == NULL) 
        return(a); 
  
    /* Pick either a or b, and recur */
    if (a->count >= b->count) 
    { 
        result = a; 
        result->next = SortedMerge(a->next, b); 
    } 
    else
    { 
        result = b; 
        result->next = SortedMerge(a, b->next); 
    }

    return(result); 
} 

