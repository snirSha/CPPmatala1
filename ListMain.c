#include "List.h"

int main(int argc, char* argv[]) {

	double array[]={2.15,4,6,12,77,99};
	int i;

	List* listPtr1= List_alloc();
	for(i=0; i<(sizeof(array)/sizeof(array[0])); ++i) {
		List_insertFirst(listPtr1,array[i]);
	}
	List_print(listPtr1);
				  
	List* listPtr2;
	listPtr2= List_clone(listPtr1);
	List_print(listPtr2);
	  
	List_free(listPtr1);
	List_free(listPtr2);
	  
	return 0;
}
