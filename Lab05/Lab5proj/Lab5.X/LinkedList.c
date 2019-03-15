/*
 * File:   LinkedList.c
 * Author: sedamian
 *
 * Created on May 9, 2017, 3:09 AM
 */

#include <stdio.h>
#include "xc.h"
#include "BOARD.h"
#include "LinkedList.h"

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data) {
    ListItem *p = malloc(sizeof (ListItem));
    //  data = p;
    p->data = data;
    if (p == NULL) {
        return NULL;
    }
    return p;

}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item) {
        ListItem *p = item;
        //  data = p;
        char *m = p->data;
        
    
       // p = item->data;
        if (p == NULL) {
            return NULL;
        }
        if (p->nextItem == NULL && p->previousItem == NULL){
            free(p);
        }
        else if(p->previousItem == NULL){
            p->nextItem->previousItem = NULL;
            free(p);
        }
        else if (p->nextItem == NULL){
            p->previousItem->nextItem = NULL;
            free(p);
        }
        else if (p == NULL) {
            return NULL;
        }
        else {
            p->nextItem->previousItem = p->previousItem;
            p->previousItem->nextItem = p->nextItem;
            free(p);
            
        }
        
        
        
        return m;

}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list) {
    int ret;
   // ret = 0;

     ListItem  *a= LinkedListGetFirst(list);
     ListItem *p = a->nextItem;
     
     if (a == NULL){
         return 0;
     }
     
     
    ret = 1;
    while ( p!=NULL){
        p=p->nextItem;
        ret++;
    }
     return ret;


}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return The first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list) {
    
    ListItem *ret;
    
    ret = list;
    
    while ( ret->previousItem != NULL){
         
        ret = ret->previousItem;
    
    }
    
    return ret;


}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, InsertAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data) {

    ListItem *UCSC;
    UCSC = LinkedListNew(data);
    if (UCSC != NULL){
        
        if (item->nextItem != NULL){
            
            UCSC->previousItem = item;
            UCSC->nextItem = item->nextItem;
            item->nextItem->previousItem = UCSC;
            item ->nextItem = UCSC;
            
        }
        else {
            UCSC->nextItem = NULL;
            UCSC->previousItem = item;
            item->nextItem = UCSC;
        }
        
    }
    else {
        return NULL;                // Else returns Null
    }
    return UCSC;

}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. It is used
 * within LinkedListSort() for swapping items, but probably isn't too useful otherwise. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
        
    if (firstItem == NULL && secondItem == NULL){
        return STANDARD_ERROR;
    }
    else {
        char *cher;
        cher = firstItem->data;
        firstItem->data = secondItem->data;
        secondItem->data = cher;
        return SUCCESS;
    }
    

}

/**
 * LinkedListSort() performs a selection sort on list to sort the elements into decending order. It
 * makes no guarantees of the addresses of the list items after sorting, so any ListItem referenced
 * before a call to LinkedListSort() and after may contain different data as only the data pointers
 * for the ListItems in the list are swapped. This function sorts the strings in ascending order
 * first by size (with NULL data pointers counting as 0-length strings) and then alphabetically
 * ascending order. So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog,
 * duck, goat]. LinkedListSort() returns SUCCESS if sorting was possible. If passed a NULL pointer
 * for either argument, it will do nothing and return STANDARD_ERROR.
 *
 * @param list Any element in the list to sort.
 * @return SUCCESS if successful or STANDARD_ERROR is passed NULL pointers.
 */
int LinkedListSort(ListItem *list) {
    ListItem *b, *d;

    ListItem *a = list ;
    if (list != NULL) {

        
        for (a = LinkedListGetFirst(list); a->nextItem != NULL;a = a->nextItem) {
            for (b = a->nextItem; b != NULL; b = b->nextItem) {


                
                if (a->data == NULL){           // if null break
                    break;
                }
                else if (b->data == NULL) {
                    d =  LinkedListGetFirst(list);
                    LinkedListSwapData(b, d);

                }
                else if (strlen(a->data) > strlen(b->data)) {
                    LinkedListSwapData(b, a);
                } 
                else if (((strlen(a->data) == (strlen(b->data))))&& ((strcmp(b->data, a->data)) < 0)) {
                        LinkedListSwapData(a, b);
                    }
                } 
            }
           // a = a->nextItem;
            //i = i->nextItem;
        return SUCCESS;
        }

        
     else {
        return STANDARD_ERROR;
 
    }
}


/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list) {

    if (list == NULL) {
        return STANDARD_ERROR;
    } else {
        printf("\n{");
        while (list != NULL) {
            if (list->data != NULL) {
                printf("%s", list->data);

                //                return SUCCESS;
            } else {
                printf("(NULL)");
            }
            list = list->nextItem;
            printf(", ");
        }
        printf("}\n");
        return SUCCESS;
    }

}