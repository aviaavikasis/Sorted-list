#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h>  /* printf */
#include "srtlist.h"

#define UNUSED(x) (void)x

void SrtlistCreateAndDestroyTest(void);
void SrtlistInsertAndGetDataTest(void);
void SrtlistRemoveTest(void); 
void SrtlistSizeTest(void);
void SrtlistIsEmptyTest(void);
void SrtlistIterIsEqualTest(void);
void SrtlistForEachTest(void);
void SrtlistFindTest(void);
void SrtlistMergeTest(void);
void SrtlistFindIfTest(void);



/* help functions for find and foreach tests */
int SumList(void* num , void* result);
int MultList(void* num , void* result);
int FindNumInList(const void* num , void* value);
int FindEvenNumbers(const void* num , void* value);
int IncreaseOrderCompare(void *data1 , void *data2 , void *param);



int main()
{
	SrtlistCreateAndDestroyTest();
	SrtlistInsertAndGetDataTest();
	SrtlistRemoveTest(); 
	SrtlistSizeTest();
	SrtlistIsEmptyTest();
	SrtlistIterIsEqualTest();
	SrtlistForEachTest();
	SrtlistFindTest();
	SrtlistMergeTest();
	SrtlistFindIfTest();
	printf("\n\n->->->->->->->->-> success!!! <-<-<-<-<-<-<-<-<-<\n\n\n");
	
	return 0;
}


void SrtlistCreateAndDestroyTest(void)
{
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	SrtlistDestroy(srtlist);
}


void SrtlistInsertAndGetDataTest(void)
{

	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	int i = 0;
	int arr[15] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	srtlist_iterator_ty result = {NULL , NULL};
	
	for(i = 0 ; i < 15 ; ++i)
	{
		SrtlistInsert(srtlist , (void*)(arr + i));	
	}
	
	result = SrtlistBegin(srtlist);
	
	for (i = 1 ; i <= 15 ; ++i)
	{
		/*assert(*((int*)SrtlistGetData(result)) == i);*/
		result = SrtlistNext(result);
	}
		
	SrtlistDestroy(srtlist);
	
}


void SrtlistRemoveTest(void)
{
	int a = 3;
	int b = 4;
	int c = 2;  
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	SrtlistInsert(srtlist , (void*)&a);	
	SrtlistInsert(srtlist , (void*)&b);
	SrtlistInsert(srtlist , (void*)&c);
	SrtlistRemove(SrtlistBegin(srtlist));
	assert(*((int*)SrtlistGetData(SrtlistBegin(srtlist))) == 3);
	SrtlistDestroy(srtlist);
}




void SrtlistSizeTest(void)
{

	int arr[10] = {1,2,3,4,5,6,7,8,9,10};
	int i = 0;
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	
	for(i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist , (void*)(arr + i));	
	}

	assert(SrtlistSize(srtlist) == 10);
	SrtlistRemove(SrtlistBegin(srtlist));
	assert(SrtlistSize(srtlist) == 9);
	SrtlistDestroy(srtlist);
	
}


void SrtlistIsEmptyTest(void)
{
	int a = 3;
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	
	assert(SrtlistIsEmpty(srtlist) == 1);
	SrtlistInsert(srtlist , (void*)&a);	
	assert(SrtlistIsEmpty(srtlist) == 0);
	SrtlistDestroy(srtlist);
}


void SrtlistIterIsEqualTest(void)
{

	int a = 3;
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	srtlist_iterator_ty iterator1 = {NULL , NULL};
	srtlist_iterator_ty iterator2 = {NULL , NULL};
	SrtlistInsert(srtlist , (void*)&a);
	iterator1 = SrtlistBegin(srtlist);
	iterator2 = SrtlistEnd(srtlist);	
	assert(SrtlistIterIsEqual(iterator1 , iterator2) == 0);
	assert(SrtlistIterIsEqual(SrtlistNext(iterator1) , iterator2) == 1);
	
	SrtlistDestroy(srtlist);

}


void SrtlistForEachTest(void)
{
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	action_ty action_func1 = &SumList;
	action_ty action_func2 = &MultList;
	int i = 0;
	int sum_of_list = 0;
	int mult_of_list = 1;

	int arr[10] = {2,2,2,2,2,2,2,2,2,2};
	
	for (i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist , (void*)(arr + i));
	}
	
	SrtlistForEach(SrtlistBegin(srtlist) , SrtlistEnd(srtlist) , action_func1 ,  (void*)&sum_of_list);
	assert(sum_of_list == 20);
	
	SrtlistForEach(SrtlistBegin(srtlist) , SrtlistEnd(srtlist) , action_func2 ,  (void*)&mult_of_list);
	assert(mult_of_list == 1024);
	SrtlistDestroy(srtlist);
}


void SrtlistFindTest(void)
{

	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	
	srtlist_iterator_ty iterator = {NULL , NULL};
	int test_value = 4;
	int i = 0;

	int arr[10] = {1,2,3,4,5,6,7,8,9,10};
	
	for (i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist , (void*)(arr + i));
	}
	
	iterator = SrtlistFind(srtlist , SrtlistBegin(srtlist) , SrtlistEnd(srtlist) ,  (void*)&test_value);
	assert(*((int*)SrtlistGetData(iterator)) == 4);
	
	test_value = 11;
	iterator = SrtlistFind(srtlist , SrtlistBegin(srtlist) , SrtlistEnd(srtlist) ,  (void*)&test_value);
	assert(SrtlistIterIsEqual(iterator ,SrtlistEnd(srtlist)) == 1);

	SrtlistDestroy(srtlist);

}


void SrtlistMergeTest(void)
{
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist1 = SrtlistCreate(comp_func , NULL);
	srtlist_ty *srtlist2 = SrtlistCreate(comp_func , NULL);
	
	int i = 0;
	srtlist_iterator_ty iterator = {NULL , NULL};

	int arr1[10] = {1,3,5,7,9,11,13,15,17,19};
	int arr2[10] = {2,4,6,8,10,12,14,16,18,20};
	
	for (i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist1 , (void*)(arr1 + i));
	}	
	
	for (i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist2 , (void*)(arr2 + i));
	}	
	
	SrtlistMerge(srtlist1 , srtlist2);
	iterator = SrtlistBegin(srtlist1);
	
	for (i = 1 ; i <= 19 ; ++i)
	{
		assert(*((int*)SrtlistGetData(iterator)) == i);
		iterator = SrtlistNext(iterator);
	}
	

	SrtlistDestroy(srtlist1);
	SrtlistDestroy(srtlist2);	

}


void SrtlistFindIfTest(void)
{
	compare_data_func comp_func = &IncreaseOrderCompare;
	srtlist_ty *srtlist = SrtlistCreate(comp_func , NULL);
	is_match_ty match_func = &FindNumInList;
	srtlist_iterator_ty iterator = {NULL , NULL};
	int test_value = 4;
	int i = 0;

	int arr[10] = {1,2,3,4,5,6,7,8,9,10};
	
	for (i = 0 ; i < 10 ; ++i)
	{
		SrtlistInsert(srtlist , (void*)(arr + i));
	}
	
	iterator = SrtlistFindIf(SrtlistBegin(srtlist) , SrtlistEnd(srtlist) , match_func ,  (void*)&test_value);
	assert(*((int*)SrtlistGetData(iterator)) == 4);
	
	test_value = 11;
	iterator = SrtlistFindIf(SrtlistBegin(srtlist) , SrtlistEnd(srtlist) , match_func ,  (void*)&test_value);
	assert(SrtlistIterIsEqual(iterator ,SrtlistEnd(srtlist)) == 1);

	SrtlistDestroy(srtlist);

}





int SumList(void* num , void* result)
{
	*((int*)result) += *((int*)num);
	return 0;
}

int MultList(void* num , void* result)
{
	*((int*)result) *= *((int*)num);
	return 0;
}

int FindNumInList(const void* num , void* value)
{
	return (*((int*)num) == *((int*)value)) ? 1 : 0;
}

int FindEvenNumbers(const void* num , void *value)
{
	UNUSED(value);
	return (*((int*)num) % 2 == 0);
}

int IncreaseOrderCompare(void *data1 , void *data2 , void *param)
{
	UNUSED(param);
	return *((int*)data1) - *((int*)data2);
}













