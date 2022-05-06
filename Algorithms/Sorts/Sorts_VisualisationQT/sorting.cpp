#include "sorting.h"
#include <QRandomGenerator>

Sorting::Sorting(QObject* parent) : QThread(parent){
}

void Sorting::run(){
    // first reset num_changes and num_comparisons
    num_changes = 0;
    num_comparisons = 0;

    // starts sorting
    if (algorithm == "Bubble Sort"){
        sort_bubble(arr,size);
    } else if (algorithm == "Selection Sort"){
        sort_selection(arr,size);
    } else if (algorithm == "Insertion Sort"){
        sort_insertion(arr,size);
    } else if (algorithm == "Quicksort"){
        sort_quick(arr,size,0,size-1);
    } else if (algorithm == "Cocktail Sort"){
        sort_cocktail(arr,size);
    } else if (algorithm == "Merge Sort"){
        sort_merge(arr,size,0,size-1);
    } else if (algorithm == "Heap Sort"){
        sort_heap(arr,size);
    } else if (algorithm == "Gnome Sort"){
        sort_gnome(arr,size);
    } else if (algorithm == "Counting Sort"){
        sort_counting(arr,size,MAX_NUMBER+1,1);     // base>maximum element of array, exp==1 is used for counting sort
    } else if (algorithm == "Radix Sort (Base 2)"){
        sort_radix(arr,size,2);
    } else if (algorithm == "Radix Sort (Base 10)"){
        sort_radix(arr,size,10);
    } else if (algorithm == "Comb Sort"){
        sort_comb(arr,size);
    } else if (algorithm == "Odd Even Sort"){
        sort_oddeven(arr,size);
    }

    // finish sorting
    // visualize to make sure correct num_comparisons and num_changes are shown
    color_size = 0;
    color_index = new int [color_size];
    visualize(arr,size,color_index,color_size);
    
    emit done();
}

// Shuffles the array
void Sorting::shuffle(){
    delete [] arr;
    arr = new  int[size];
    for (int i=0;i<size;++i){
        arr[i] = MAX_NUMBER*(i+1)/size;
    }
    if (shuffleType == "Random"){               // Random shuffling
        for (int i=0;i<size;++i){
            int j = QRandomGenerator::global()->bounded(0,i+1);
            swap(arr,size,i,j);
        }
    } else if (shuffleType == "Reverse"){       // Reverse order
        for (int i=0;i<size/2;++i){
            swap(arr,size,i,size-1-i);
        }
    } else if (shuffleType == "Almost sorted"){ // Swap two random elements
        int i = QRandomGenerator::global()->bounded(0,size-1);
        int j;
        do {
            j = QRandomGenerator::global()->bounded(0,size);
        } while (arr[i] == arr[j]);
        swap(arr,size,i,j);
    }

    // reset num_changes and num_comparisons
    num_changes = 0;
    num_comparisons = 0;
}

// Swap two chosen elements if they are different
void Sorting::swap(int* arr,int size,int i,int j){
    if (i>=size || j>=size || arr[i]==arr[j]){
        return;         // this line is to prevent unnecessary update of num_changes
    }
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    num_changes += 2;   // 2 elements (at index i and j) are changed
}

// Divide an array into two smaller arrays of similar sizes and merge them into one, to be used in Merge Sort
void Sorting::merge(int* arr,int size,int start,int end){
    int div = (start+end)/2;        // index of last element to be put into the first splitted part  (i.e. left array)
    int lsize = div-start+1;        // size of left index
    int rsize = end-div;            // size  of right array

    // construction of left and right array
    int* L = new int[lsize];
    int* R = new int[rsize];

    for (int i = 0; i<lsize; ++i){
        L[i] = arr[start+i];
    }
    for (int i = 0; i<rsize;  ++i){
        R[i] = arr[div+1+i];
    }

    //merging of left and right array
    int left = 0;
    int right = 0;
    int merged = start;

    while (left<lsize && right<rsize){
        ++num_comparisons;
        if (L[left] <= R[right]){
            if (arr[merged] != L[left]){
                arr[merged] = L[left];
                ++num_changes;
            }
            ++left;
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = merged;
            color_index[1] = start-1+lsize+right;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        } else {
            if (arr[merged] != R[right]){
                arr[merged] = R[right];
                ++num_changes;
            }
            ++right;
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = merged;
            color_index[1] = start-1+lsize+right;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        ++merged;
    }

    while (left<lsize){
        if (arr[merged] != L[left]){
            arr[merged] = L[left];
            ++num_changes;
        }
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = merged;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        ++left;
        ++merged;
    }
    while (right<rsize){
        if (arr[merged] != R[right]){
            arr[merged] = R[right];
            ++num_changes;
        }
        //visualize
        color_size = 2;
        color_index = new int[color_size];
        color_index[0] = merged;
        color_index[1] = start-1+lsize+right;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        ++right;
        ++merged;
    }
    delete [] L;
    delete [] R;
}

// Heapify, to be used in Heap Sort.
void  Sorting::heapify(int *arr, int size, int heapsize, int i){
    int maxindex = i;
    int left = 2*i+1;
    int right = 2*i+2;

    if (left<heapsize){
        ++num_comparisons;
        if (arr[left]>arr[maxindex]){
            maxindex = left;
        }
    }

    if (right<heapsize){
        ++num_comparisons;
        if (arr[right]>arr[maxindex]){
            maxindex = right;
        }
    }

    if (maxindex != i){
        swap(arr,size,i,maxindex);
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = i;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        heapify(arr,size,heapsize,maxindex);

    }
}

//Find the correct place and insert the element into the place, to be used in Insertion Sort
void Sorting::insert(int* arr,int size,int i){
    if (i>size-1){
        return;
    }

    int index = i;
    // finding correct relative place to insert, linear search is used here
    for (index=i;index>0;--index){
        ++num_comparisons;
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = index;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        if (arr[index-1]<arr[i]){
            break;      // quit the loop once correct place is found
        }
    }
    // insert
    int temp = arr[i];
    for (int j = i;j>index;--j){
        if (arr[j] != arr[j-1]){
            ++num_changes;
        }
        arr[j] = arr[j-1];
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = j;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
    }
    if (arr[index] != temp){
        ++num_changes;
    }
    arr[index] = temp;
    //visualize
    color_size = 1;
    color_index = new int[color_size];
    color_index[0] = index;
    visualize(arr,size,color_index,color_size);
    delete [] color_index;
}

//create array of specific size
void Sorting::createArray(){
    arr = new int[size];
    for (int i=0;i<size;++i){
        arr[i] = MAX_NUMBER*(i+1)/size;
    }
    shuffle();
    int index[1] = {0};
    emit changed(arr,size,index,1);
}

//Bubble Sort
void Sorting::sort_bubble(int *arr, int size){
    bool changed = true;
    int maxindex = size-1;
    while (changed){        // if nothing is changed in this iteration, the sorting is finished
        changed = false;
        for (int i=0;i<maxindex;++i){
            ++num_comparisons;
            if (arr[i]>arr[i+1]){
                swap(arr,size,i,i+1);
                changed = true;
            }
            //visualize
            color_size = 2;
            color_index= new int[color_size];
            color_index[0] = i;
            color_index[1] = i+1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        --maxindex;     // maximum element is at correct position, no need to consider it in the next iteration
    }
}

//Insertion Sort
void Sorting::sort_insertion(int* arr, int size){
    for (int i=1;i<size;++i){
        insert(arr,size,i);
    }
}

//Selection Sort
void Sorting::sort_selection(int *arr, int size){
    for (int i=0;i<size;++i){
        // find the index with minimum element
        int minindex = i;
        int minvalue = arr[i];
        for (int j=i+1;j<size;++j){
            ++num_comparisons;
            if (arr[j]<minvalue){
                minindex = j;
                minvalue = arr[j];
            }
            //visualize
            color_size = 3;
            color_index = new int[color_size];
            color_index[0] = i;
            color_index[1] = j;
            color_index[2] = minindex;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        swap(arr,size,i,minindex);
    }
}

//Quicksort
void Sorting::sort_quick(int*arr,int size,int start,int end){
    if (start >= end){
        return;
    }

    int pivot = arr[end];       // rightmost element is chosen as pivot
    int left = start;
    int right = end-1;

    while (true){
        ++num_comparisons;
        while (arr[left]<pivot && left<end){
            //visualize
            color_size = 1;
            color_index = new int[color_size];
            color_index[0] = left;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;

            ++left;
            ++num_comparisons;
        }
        ++num_comparisons;
        while (arr[right]>=pivot && right>left){
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = left;
            color_index[1] =  right;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
            --right;
            ++num_comparisons;
        }
        if (left == end){
            sort_quick(arr,size,start,end-1);
            break;
        } else if (left == right){      // partition process occurs here
            swap(arr,size,left,end);
            sort_quick(arr,size,start,left-1);
            sort_quick(arr,size,left+1,end);
            break;
        } else {
            swap(arr,size,left,right);
            continue;
        }
    }
}

//Cocktail sort (aka Cocktail Shaker Sort)
void Sorting::sort_cocktail(int* arr, int size){
    int start = 0;
    int end = size-1;
    bool changed;
    while (start<end){
        changed = false;
        for (int i=start;i<end;++i){
            ++num_comparisons;
            if (arr[i]>arr[i+1]){
                swap(arr,size,i,i+1);
                changed = true;
            }
            //visualize
            color_size = 1;
            color_index = new int[color_size];
            color_index[0] = i+1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        --end;
        if (changed == false){
            break;
        }
        for (int i=end;i>start;--i){
            ++num_comparisons;
            if (arr[i]<arr[i-1]){
                swap(arr,size,i-1,i);
                changed = true;
            }
            //visualize
            color_size = 1;
            color_index = new int[color_size];
            color_index[0] = i-1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        if (changed == false){
            break;
        }
        ++start;
    }
}

//Merge Sort
void Sorting::sort_merge(int *arr, int size, int start, int end){
    if (start<end){
        int div = (start+end)/2;
        sort_merge(arr,size,start,div);
        sort_merge(arr,size,div+1,end);
        merge(arr,size,start,end);
    }
}

//Heapsort
void Sorting::sort_heap(int *arr, int size){
    for (int i = size/2-1; i>=0; --i){
        heapify(arr,size,size,i);
    }

    for (int i=size-1;i>=0;--i){
        swap(arr,size,0,i);
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = i;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        heapify(arr,size,i,0);
    }
}

//Gnome Sort
void Sorting::sort_gnome(int *arr, int size){
    int i=0;
    while (i<size){
        if (i == 0){
            i=1;
        }
        ++num_comparisons;
        if (arr[i]>=arr[i-1]){
            ++i;
        } else {
            swap(arr,size,i-1,i);       // swap until the relative order is correct
            --i;
        }
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = i;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
    }
}

void Sorting::sort_counting(int* arr, int size,int base,int exp){       // base and exp are for radix sort
    int* temp = new int[size];
    int* count = new int[base];
    for (int i=0;i<base;++i){
        count[i] = 0;
    }
    for (int i=0;i<size;++i){
        count[(arr[i]/exp)%base]++;
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = i;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
    }

    for (int i=1;i<base;++i){
        count[i] += count[i-1];
    }

    for (int i=size-1;i>=0;--i){
        temp[count[(arr[i]/exp)%base]-1] = arr[i];
        --count[(arr[i]/exp)%base];
    }

    for (int i=0;i<size;++i){
        if (arr[i] !=temp[i]){
            arr[i] = temp[i];
            ++num_changes;
        }
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = i;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
    }
    delete [] temp;
    delete [] count;
}

//Radix Sort (Least Significant Digit)
void Sorting::sort_radix(int *arr, int size, int base){
    for (int exp=1;MAX_NUMBER/exp>0;exp*=base){
        sort_counting(arr,size,base,exp);
    }
}

//Comb Sort
void Sorting::sort_comb(int* arr,int size){
    int gap = size*13/20;       // to make initial gap is about half of the size of array
    bool changed = true;

    while (changed){
        gap = gap*10/13;        // shrink the gap
        if (gap < 1){
            gap = 1;
            changed = false;
        }

        int i=0;
        while (i+gap < size){
            ++num_comparisons;
            if (arr[i]>arr[i+gap]){
                swap(arr,size,i,i+gap);
                changed = true;
            }
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = i;
            color_index[1] = i+gap;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;

            ++i;
        }
    }
}

//Odd Even Sort
void Sorting::sort_oddeven(int *arr, int size){
    bool changed = true;
    while (changed){
        changed = false;
        for (int i=1;i<size-1;i+=2){        // odd,even pair
            ++num_comparisons;
            if (arr[i]>arr[i+1]){
                swap(arr,size,i,i+1);
                changed = true;
            }
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = i;
            color_index[1] = i+1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }

        for (int i=0;i<size-1;i+=2){        // even,odd pair
            ++num_comparisons;
            if (arr[i]>arr[i+1]){
                swap(arr,size,i,i+1);
                changed = true;
            }
            //visualize
            color_size = 2;
            color_index = new int[color_size];
            color_index[0] = i;
            color_index[1] = i+1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
    }
}

void Sorting::visualize(int* arr,int size_arr,int* colorindex,int size_color){
    emit changed(arr,size_arr,colorindex,size_color);
    this->usleep(animDelay);
}
