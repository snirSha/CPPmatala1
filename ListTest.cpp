extern "C" {
#include "List.h"
}
#include "gtest/gtest.h"

const size_t MAX_TEST_OUTPUT_CHECK= 1000;

TEST(List, allocFree) {
	List* listPtr1= List_alloc();
	EXPECT_EQ(0, List_size(listPtr1));
	List* listPtr2= List_alloc();
	EXPECT_EQ(0, List_size(listPtr2));
	
	List_free(listPtr1);
	List_free(listPtr2);
	listPtr1= List_alloc();
	List_free(listPtr1);
	List_free(NULL);
}

TEST(List, insertFirst) {
	List* listPtr1= List_alloc();
	
	List_insertFirst(listPtr1, 2.2);
	EXPECT_DOUBLE_EQ(2.2, List_firstData(listPtr1));
	EXPECT_EQ(1, List_size(listPtr1));
	List_insertFirst(listPtr1, -32.2);
	EXPECT_DOUBLE_EQ(-32.2, List_firstData(listPtr1));
	EXPECT_EQ(2, List_size(listPtr1));
	
	List_free(listPtr1);
	listPtr1= List_alloc();
	EXPECT_EQ(0, List_size(listPtr1));
	List_insertFirst(listPtr1, 42.0);
	EXPECT_EQ(1, List_size(listPtr1));
	EXPECT_DOUBLE_EQ(42.0, List_firstData(listPtr1));
	
	List_free(listPtr1);
}					   

void TEST_LIST_PRINT_HELPER_FUNCTION(const double arr[], size_t arrLen,
									 const char* outputShouldBe) {

	EXPECT_GT(MAX_TEST_OUTPUT_CHECK, strlen(outputShouldBe));
	
	// redirecting output to file
	const char* tmpFileName= "ListPrintTestTmpFile.txt";
	FILE* screen= stdout;
	FILE* fileOut= fopen(tmpFileName, "w");
	EXPECT_TRUE(fileOut!=NULL);
	stdout= fileOut;

	// creating list and printing
	List* listPtr1= List_alloc();
	if (arr!=NULL) {
		for (size_t i=0; i<arrLen; ++i) {
			List_insertFirst(listPtr1, arr[i]);
		}
	}
	List_print(listPtr1);

	// redirecting output back and closing 
	stdout= screen;
	int fcloseFlag= fclose(fileOut);
	EXPECT_EQ(0, fcloseFlag);
	
	// reading input and checking that it is ok
	FILE* fileIn= fopen(tmpFileName, "r");
	EXPECT_TRUE(fileIn!=NULL);
	char output[MAX_TEST_OUTPUT_CHECK];
	char* outputFlag= fgets(output,
							// +1 for storing the '\0'
							// +1 to see that there is not too much output
							strlen(outputShouldBe)+1+1,
							fileIn);
	EXPECT_TRUE(outputFlag!=NULL);
	EXPECT_STREQ(outputShouldBe, output);
	fcloseFlag= fclose(fileIn);
	EXPECT_EQ(0, fcloseFlag);
	
	// cleaning file
	int removeFlag= remove(tmpFileName);
	EXPECT_EQ(0, removeFlag);

	List_free(listPtr1);
}

TEST(List, print) {

	TEST_LIST_PRINT_HELPER_FUNCTION(NULL, 0,
									"|| size:0\n");
	double arr1[]= {2.01, 1.5};
	TEST_LIST_PRINT_HELPER_FUNCTION(arr1, sizeof(arr1)/sizeof(*arr1),
									"(1.50)->(2.01)->|| size:2\n");
}

TEST(List, equal1) {
	List* listPtr1= List_alloc();
	List* listPtr2= List_alloc();
	EXPECT_EQ(0, List_equal(listPtr1, listPtr1));
	EXPECT_EQ(0, List_equal(listPtr2, listPtr2));
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));

	List_insertFirst(listPtr1, 0.0);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr1));
	EXPECT_NE(0, List_equal(listPtr1, listPtr2));
	EXPECT_NE(0, List_equal(listPtr2, listPtr1));

	List_insertFirst(listPtr1, 30.0);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr1));
	EXPECT_NE(0, List_equal(listPtr1, listPtr2));
	EXPECT_NE(0, List_equal(listPtr2, listPtr1));

	List_free(listPtr1);
	List_free(listPtr2);
}

TEST(List, equal2) {
	List* listPtr1= List_alloc();
	List* listPtr2= List_alloc();

	List_insertFirst(listPtr1, -3.2);
	List_insertFirst(listPtr2, -3.2);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));
	EXPECT_EQ(0, List_equal(listPtr2, listPtr1));

	List_insertFirst(listPtr1, 13.0);
	List_insertFirst(listPtr2, 13.0);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));
	EXPECT_EQ(0, List_equal(listPtr2, listPtr1));

	List_insertFirst(listPtr1, 23.0);
	List_insertFirst(listPtr2, 13.0);
	EXPECT_NE(0, List_equal(listPtr1, listPtr2));
	EXPECT_NE(0, List_equal(listPtr2, listPtr1));

	List_free(listPtr1);
	List_free(listPtr2);
}

TEST(List, clone) {
	List* listPtr1= List_alloc();
	List* listPtr2= List_clone(listPtr1);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));
	
	// change to one does not change the other
	List_insertFirst(listPtr1, 23.0);
	EXPECT_NE(0, List_equal(listPtr1, listPtr2));
	
	List_free(listPtr2);
	listPtr2= List_clone(listPtr1);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));

	List_insertFirst(listPtr1, -1123.5);
	List_free(listPtr2);
	listPtr2= List_clone(listPtr1);
	EXPECT_EQ(0, List_equal(listPtr1, listPtr2));

	// change to one does not change the other
	List_insertFirst(listPtr2, 23.0);
	EXPECT_NE(0, List_equal(listPtr1, listPtr2));

	List_free(listPtr1);
	List_free(listPtr2);
}
