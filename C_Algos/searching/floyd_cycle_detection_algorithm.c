 
#include <assert.h>   /// for assert
#include <inttypes.h> /// for uint32_t
#include <stdio.h>    /// for IO operations


uint32_t duplicateNumber(const uint32_t *in_arr, size_t n)
{
    if (n <= 1) {  // to find duplicate in an array its size should be atleast 2
        return -1;
    }
    uint32_t tortoise = in_arr[0];  ///< variable tortoise is used for the longer
                                    ///< jumps in the array
    uint32_t hare = in_arr[0];  ///< variable hare is used for shorter jumps in the array
    do {                                   // loop to enter the cycle
        tortoise = in_arr[tortoise];       // tortoise is moving by one step
        hare = in_arr[in_arr[hare]];       // hare is moving by two steps
    } while (tortoise != hare);
    tortoise = in_arr[0];
    while (tortoise != hare) {             // loop to find the entry point of cycle
        tortoise = in_arr[tortoise];
        hare = in_arr[hare];
    }
    return tortoise;
}

static void test()
{
    uint32_t arr[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610}; // input array
    size_t n = sizeof(arr) / sizeof(int);

    printf("1st test... ");
    uint32_t index = duplicateNumber(arr, n); // calling the duplicateNumber function to check which number occurs twice in the array
    assert(index == 1); // the number which occurs twice is 1 or not
    printf("passed\n");
}

int main()
{
    test();  // run self-test implementations
    return 0;
}
