
#include <vector>
#include <limits.h>

using namespace std;

// select sort n^2 
void selectSort(vector<int>& array) {
    int len = array.size();
    for (int i = 0; i < len; ++i) {
        int mn = INT_MAX, pos = i;
        for (int j = i + 1; j < len; ++j) {
            if (array[j] < mn) {
                mn = array[j];
                pos = j;
            }
        }
        swap(array[i], array[pos]);
    }
    return;
}

// insert sort n^2
void insertSort(vector<int>& array) {
    int len = array.size();
    for (int i = 1; i < len; ++i) {
        int tmp = array[i], j = i - 1;
        for (; j >= 0; --j) {
            if (array[j] > tmp) array[j + 1] = array[j];
            else break;
        }
        array[j + 1] = tmp;
    }
    return;
}

// shell sort n^1.25~1.6n^1.25
void shellSort(vector<int>& array) {
    int len = array.size();
    int step = len / 2;
    while (step > 0) {
        for (int i = step; i < len; ++i) {
            int tmp = array[i], j = i - step;
            for (; j >= 0; j -= step) {
                if (array[j] > tmp) array[j + step] = array[j];
                else break;
            } 
            array[j + step] = tmp;
        }
        step /= 2;
    }
    return;
}

// merge sort nlogn
void merge(vector<int>& array, int l, int mid, int r) {
    vector<int> subarray1(array.begin() + l, array.begin() + mid);
    vector<int> subarray2(array.begin() + mid + 1, array.begin() + r);
    int i = 0, j = 0, k = l;
    while (i < subarray1.size() && j < subarray2.size()) {
        if (subarray1[i] <= subarray2[j]) {
            array[k] = subarray1[i];
            ++i;
        } 
        else {
            array[k] = subarray2[j];
            ++j;
        }
        ++k;
    }
    while (i < subarray1.size()) {
        array[k] = subarray1[i];
        ++i;
        ++k;
    }
    while (j < subarray2.size()) {
        array[k] = subarray2[j];
        ++j;
        ++k;
    }
    return;
}

void _merge_sort(vector<int>& array, int l, int r) {
    if (r - l <= 0) return;
    int mid = l + (r - l) / 2;
    _merge_sort(array, l, mid);
    _merge_sort(array, mid + 1, r);
    merge(array, l, mid, r);
    return;
}

void mergeSort(vector<int>& array) {
    int l = 0, r = array.size(), mid = l + (r - l) / 2;
    _merge_sort(array, l, mid);
    _merge_sort(array, mid + 1, r);
    merge(array, l, mid, r);
    return;
}

// quick sort nlogn
void _quickSort(vector<int>& array, int l, int r) {
    if (l >= r) return;
    int tmp = array[l], i = l, j = r;
    while (i < j) {
        while (i < j && array[j] >= tmp) --j;
        if (i < j) array[i++] = array[j];
        while (i < j && array[i] < tmp) ++i;
        if (i < j) array[j--] = array[i]; 
    }
    array[i] = tmp;
    _quickSort(array, l, i - 1);
    _quickSort(array, i + 1, r);
    return;
}

void quickSort(vector<int>& array) {
    _quickSort(array, 0, (int)array.size() - 1);
    return;
}

// bubble sort n^2q
void bubbleSort(vector<int>& array) {
    bool flag = false;
    int len = array.size();
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (array[j] < array[j]) {
                swap(array[i], array[j]);
                flag = true;
            }
            if (!flag) break;
        }
    }
    return;
}

// heap sort(priority_queue) nlogn

// bucket sort 

