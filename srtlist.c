#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "srtlist.h"

#define UNUSED(x) (void)x

struct srtlist 
{
    dlist_ty *dlist;
    compare_data_func sort;
};


srtlist_ty *SrtlistCreate(compare_data_func sort, void *param)
{
	srtlist_ty *srtlist = NULL;
	UNUSED(param);
	srtlist = (srtlist_ty*)malloc(sizeof(srtlist_ty));
	if(NULL == srtlist)
	{
		return NULL;
	}
	srtlist -> dlist = DlistCreate();
	if(NULL == srtlist -> dlist)
	{
		return NULL;
	}
	
	srtlist -> sort = sort;
	
	return srtlist;
}


void SrtlistDestroy(srtlist_ty *srtlist)
{
	assert(NULL != srtlist);
	DlistDestroy(srtlist -> dlist);
	free(srtlist);
}


srtlist_iterator_ty SrtlistInsert(srtlist_ty *srtlist, void *data)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != srtlist);
	
	if(SrtlistIsEmpty(srtlist))
	{
		DlistInsertBefore(DlistEnd(srtlist -> dlist) , data);
		result = SrtlistBegin(srtlist);
		return result;	
	}
	
	result = SrtlistBegin(srtlist);
	
	while(srtlist -> sort(SrtlistGetData(result) , data , NULL) <= 0)
	{
		result = SrtlistNext(result);
		if(1 == SrtlistIterIsEqual(result , SrtlistEnd(srtlist)))
		{
			break;
		}
	}
	
	DlistInsertBefore(result.dlist_iterator , data);
	
	#ifndef NDBUG
	result.srtlist = srtlist->dlist;
	#endif
	return result;

}


srtlist_iterator_ty SrtlistRemove(srtlist_iterator_ty iterator)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != iterator.dlist_iterator);
	
	result.dlist_iterator = DlistRemove(iterator.dlist_iterator);
	result.srtlist = NULL;
	return result;
}


srtlist_iterator_ty SrtlistBegin(const srtlist_ty *srtlist)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != srtlist);
	result.dlist_iterator = DlistBegin(srtlist -> dlist);
	result.srtlist = NULL;
	
	#ifndef NDBUG
	result.srtlist = srtlist->dlist;
	#endif
	return result;
}


srtlist_iterator_ty SrtlistEnd(const srtlist_ty *srtlist)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != srtlist);
	result.dlist_iterator = DlistEnd(srtlist -> dlist);
	result.srtlist = NULL;
	
	#ifndef NDBUG
	result.srtlist = srtlist->dlist;
	#endif
	return result;
}


srtlist_iterator_ty SrtlistNext(srtlist_iterator_ty iterator)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != iterator.dlist_iterator);
	result.dlist_iterator = DlistNext(iterator.dlist_iterator);
	result.srtlist = NULL;
	return result;
}


srtlist_iterator_ty SrtlistPrev(srtlist_iterator_ty iterator)
{
	srtlist_iterator_ty result = {NULL , NULL};
	assert(NULL != iterator.dlist_iterator);
	result.dlist_iterator = DlistPrev(iterator.dlist_iterator);
	result.srtlist = NULL;
	return result;	
}


void *SrtlistGetData(srtlist_iterator_ty iterator)
{
	assert(NULL != iterator.dlist_iterator);
	return DlistGetData(iterator.dlist_iterator);	
}


size_t SrtlistSize(const srtlist_ty *srtlist)
{
	assert(NULL != srtlist);
	return DlistSize(srtlist -> dlist);
}


int SrtlistIsEmpty(const srtlist_ty *srtlist)
{
	assert(NULL != srtlist);
	return DlistIsEmpty(srtlist -> dlist);
}


int SrtlistIterIsEqual(srtlist_iterator_ty iterator1, srtlist_iterator_ty iterator2)
{
	assert(NULL != iterator1.dlist_iterator);
	assert(NULL != iterator2.dlist_iterator);
	
	return DlistIterIsEqual(iterator1.dlist_iterator , iterator2.dlist_iterator);
}


int SrtlistForEach(srtlist_iterator_ty from, srtlist_iterator_ty to, 
                                          action_ty action_func, void *param)
{
	assert(NULL != from.dlist_iterator);
	assert(NULL != to.dlist_iterator);
	assert(NULL != action_func);
	#ifndef NDEBUG
		assert(from.srtlist == to.srtlist);
	#endif
	
	return DlistForEach(from.dlist_iterator, to.dlist_iterator, action_func, param);
}


srtlist_iterator_ty SrtlistFind(const srtlist_ty *srtlist, 
                               srtlist_iterator_ty from,
                               srtlist_iterator_ty to, 
                               void *data_to_find)
{
	assert(NULL != from.dlist_iterator);
	assert(NULL != to.dlist_iterator);
	#ifndef NDEBUG
		assert(from.srtlist == to.srtlist);
	#endif

	while(!SrtlistIterIsEqual(from , to) && srtlist -> sort(SrtlistGetData(from) , data_to_find, NULL) != 0)
	{
		from = SrtlistNext(from);	
	}	
	return from;
}                                                     


static int IterInEnd(srtlist_iterator_ty iterator , srtlist_ty *srtlist)
{
	return (1 == SrtlistIterIsEqual(iterator, SrtlistEnd(srtlist)));
}


void SrtlistMerge(srtlist_ty *dest_list , srtlist_ty *src_list)
{
	srtlist_iterator_ty from_src_iter = {NULL , NULL};
	srtlist_iterator_ty to_src_iter = {NULL , NULL};
	srtlist_iterator_ty dest_iter = {NULL , NULL};
	assert(NULL != dest_list);
	assert(NULL != src_list);
	
	if (SrtlistIsEmpty(src_list) || SrtlistIsEmpty(dest_list))
	{
		return;
	}
	
	from_src_iter = SrtlistBegin(src_list);
	to_src_iter = SrtlistBegin(src_list);
	dest_iter = SrtlistBegin(dest_list);
	
	while(0 == IterInEnd(to_src_iter , src_list) && 0 == IterInEnd(dest_iter , dest_list))
	{
		while(dest_list->sort(SrtlistGetData(dest_iter) , SrtlistGetData(to_src_iter) , NULL) >= 0)
		{
			to_src_iter = SrtlistNext(to_src_iter);
		}
		
		if(0 == SrtlistIterIsEqual(from_src_iter , to_src_iter))
		{
			DlistSplice(dest_iter.dlist_iterator , from_src_iter.dlist_iterator , to_src_iter.dlist_iterator);
			from_src_iter = to_src_iter;
		}
	
		dest_iter = SrtlistNext(dest_iter);
	}
	
	while(0 == IterInEnd(to_src_iter , src_list))
	{
		DlistInsertBefore(SrtlistEnd(dest_list).dlist_iterator , SrtlistGetData(to_src_iter));
		to_src_iter = SrtlistNext(to_src_iter);
	}
		
}


srtlist_iterator_ty SrtlistFindIf(srtlist_iterator_ty from, 
                                  srtlist_iterator_ty to,
                                  is_match_ty match_func,
                                  void *param)
{
	assert(NULL != from.dlist_iterator);
	assert(NULL != to.dlist_iterator);
	assert(NULL != match_func);
	
	#ifndef NDEBUG
		assert(from.srtlist == to.srtlist);
	#endif
	
	while(0 == SrtlistIterIsEqual(from , to) && match_func(SrtlistGetData(from), param) != 1)
	{
		from = SrtlistNext(from);	
	}	
	return from;
}                               







/*
void SrtlistMerge(srtlist_ty *dest_list , srtlist_ty *src_list)
{
	srtlist_iterator_ty src_iter = {NULL , NULL};
	assert(NULL != dest_list);
	assert(NULL != src_list);
	
	if (SrtlistIsEmpty(src_list))
	{
		return;
	}
	
	src_iter = SrtlistBegin(src_list);
	
	while(!SrtlistIterIsEqual(src_iter , SrtlistEnd(src_list)))
	{
		SrtlistInsert(dest_list , SrtlistGetData(src_iter));
		src_iter = SrtlistNext(src_iter);
	}
}
*/


