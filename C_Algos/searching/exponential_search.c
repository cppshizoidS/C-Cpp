#include <assert.h>  /// for assert
#include <inttypes.h>  /// for int64_t, uint16_t

#define ELEMENT -10

int64_t binary_search(const int64_t* arr, const uint16_t l_index, const uint16_t r_index, const int64_t n); ///< used to perform the binary search over the given array
int64_t exponential_search(const int64_t* arr, const uint16_t length, const int64_t n); ///< used to perform the exponential search over the given array
static void test(); ///< used to run the self-test implementations

/
int64_t exponential_search(const int64_t* arr, const uint16_t length, const int64_t n) 
{
    if ( length == 0 ) { return -1; }
    // find the upperbound
    uint32_t upper_bound = 1;
    while ( upper_bound <= length && arr[upper_bound] < n ) { upper_bound = upper_bound * 2; }
    // calculate the range ( between lower_boud and upper_bound )
    uint16_t lower_bound = upper_bound/2;
    if ( upper_bound > length ) { upper_bound = length; }
    // apply the binary search in the range
    return binary_search(arr, lower_bound, upper_bound, n);
}

int64_t binary_search(const int64_t* arr, const uint16_t l_index, const uint16_t r_index, const int64_t n) 
{
    // calculate the middle index of the array
    uint16_t middle_index = l_index + ( r_index - l_index ) / 2;
    // base cases
    if ( l_index > r_index ) { return -1; }
    if ( arr[middle_index] == n ) { return middle_index; }
    // recursion
    if ( arr[middle_index] > n ) { return binary_search(arr, l_index, middle_index-1, n); } // left
    return binary_search(arr, middle_index+1, r_index, n); // right
}

int main() 
{
    test();  // run self-test implementations
    return 0;
}

static void test()
{
    // empty array
    int64_t arr_empty[] = {};
    assert(exponential_search(arr_empty, 0, 10) == -1);
    // elent not found
    int64_t arr_found[] = {1, 2, 3};
    assert(exponential_search(arr_found, 3, 10) == -1);
    // element found in an array of length 1
    int64_t arr_one[] = {1};
    assert(exponential_search(arr_found, 1, 1) == 0);
    // find the first element in an array of length 2
    int64_t arr_first_2[] = {1, 2};
    assert(exponential_search(arr_first_2, 2, 1) == 0);
    // find the last element in an array of length 2
    int64_t arr_last_2[] = {1, 2};
    assert(exponential_search(arr_last_2, 2, 2) == 1);
    // find the first element in an array of length n
    int64_t arr_first_n[] = {-1, 2, 4, 6, 8};
    assert(exponential_search(arr_first_n, 5, -1) == 0);
    // find the last element in an array of length n
    int64_t arr_last_n[] = {-1, 2, 4, 6, 8};
    assert(exponential_search(arr_last_n, 5, 8) == 4);
    // find an element in an array of length n
    int64_t arr_middle[] = {-1, 2, 4, 6, 8};
    assert(exponential_search(arr_middle, 5, 6) == 3);
}
