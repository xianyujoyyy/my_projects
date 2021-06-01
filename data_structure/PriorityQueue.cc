
#include "data_structure/PriorityQueue.h"

template<class elemType>
void PriorityQueue<elemType>::buildHeap() {
    for (int i = size_ / 2; i > 0; --i) {
        percolateDown(i);
    }
}

template<class elemType>
void PriorityQueue<elemType>::enQueue(elemType element) {
    if (size_ + 1 == capacity_) doubleSpace();
    ++size_;
    queue_[size_] = element;
    int hole = size_;
    while (hole / 2 > 0) {
        elemType x = queue_[hole];
        if (x < queue_[hole / 2]) {
            queue_[hole] = queue_[hole / 2];
            queue_[hole] = x;
        }
        hole /= 2;
    }
}

template<class elemType>
elemType PriorityQueue<elemType>::deQueue() {
    elemType x = queue_[1];
    queue_[1] = queue_[size_];
    --size_;
    percolateDown(1);
    return x;
}

template<class elemType>
void PriorityQueue<elemType>::percolateDown(int i) {
    int hole = i;
    while (hole * 2 <= size_) {
        int tmp = hole * 2;
        if (tmp + 1 <= size_ && queue_[tmp + 1] < queue_[hole]) {
            ++tmp;
        }
        elemType x = queue_[hole];
        queue_[hole] = queue_[tmp];
        queue_[tmp] = x;
        hole = tmp;
    }
}

template<class elemType>
void PriorityQueue<elemType>::doubleSpace() {
    elemType* tmp = new elemType[size_ * 2];
    for (int i = 1; i <= size_; ++i) {
        tmp[i] = queue_[i];
    }
    delete [] queue_;
    queue_ = tmp;
    capacity_ = size_ * 2;
    ++size_;
}
