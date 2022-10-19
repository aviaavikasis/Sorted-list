#ifndef __ILRD_OL127_128_SORTED_LINKED_LIST_H__
#define __ILRD_OL127_128_SORTED_LINKED_LIST_H__

#include <stddef.h> /* size_t */
#include "dlist.h" /* dlist lib */

typedef int (*compare_data_func)(void *data1, void *data2 , void *params);
typedef struct srtlist srtlist_ty;
typedef struct srtlist_iterator srtlist_iterator_ty;

struct srtlist_iterator 
{
    dlist_iterator_ty dlist_iterator;
#ifndef NDBUG
    dlist_ty *srtlist;
#endif
};

srtlist_ty *SrtlistCreate(compare_data_func sort, void *param);

void SrtlistDestroy(srtlist_ty *srtlist); 

srtlist_iterator_ty SrtlistInsert(srtlist_ty *srtlist, void *data);

srtlist_iterator_ty SrtlistRemove(srtlist_iterator_ty iterator); 

srtlist_iterator_ty SrtlistBegin(const srtlist_ty *srtlist); 

srtlist_iterator_ty SrtlistEnd(const srtlist_ty *srtlist);

srtlist_iterator_ty SrtlistNext(srtlist_iterator_ty iterator);

srtlist_iterator_ty SrtlistPrev(srtlist_iterator_ty iterator);

void *SrtlistGetData(srtlist_iterator_ty iterator);

size_t SrtlistSize(const srtlist_ty *srtlist);

int SrtlistIsEmpty(const srtlist_ty *srtlist);

int SrtlistIterIsEqual(srtlist_iterator_ty iterator1, 
                      srtlist_iterator_ty iterator2);

int SrtlistForEach(srtlist_iterator_ty from,
                   srtlist_iterator_ty to, 
                   action_ty action_func,
                   void *param);

srtlist_iterator_ty SrtlistFindIf(srtlist_iterator_ty from, 
                                  srtlist_iterator_ty to,
                                  is_match_ty match_func,
                                  void *param);

srtlist_iterator_ty SrtlistFind(const srtlist_ty *srtlist, 
                               srtlist_iterator_ty from,
                               srtlist_iterator_ty to, 
                               void *data_to_find);

void SrtlistMerge(srtlist_ty *dest_list , srtlist_ty *src_list);

#endif /* __ILRD_OL127_128_SORTED_LINKED_LIST_H__ */ 
