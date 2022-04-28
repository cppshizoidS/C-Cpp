#include <stdio.h>   /// for IO operations
#include <assert.h>  /// for assert


int sentinel_linear_search( int arr[], int len, int key ){
	if(key == arr[len-1]){
		return len-1;
	}
	
	int temp = arr[len-1]; 
	arr[len-1] = key;
	
	int i = 0;
	while (arr[len-1] != arr[i]) {
		i++;
	}
	
	arr[len-1] = temp;
	
	return i != len-1 ? i : -1;
	 
}


static void test(){
	int n,i;
	n = 5;
	/* init array */
	int arr[] = { 1, 2, 2, 6, 99, 100, 999 };

	assert(sentinel_linear_search( arr, n, 1 )==0);
	assert(sentinel_linear_search( arr, n, 2 )==1);
	assert(sentinel_linear_search( arr, n, 6 )==3);
	assert(sentinel_linear_search( arr, n, 101 )==-1);
	printf("All test cases have successfully passed!\n");
} 


int main(){
	test();  // run self-test implementations
	return 0;
}
