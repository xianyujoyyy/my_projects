
#include "data_structure/CircularQueue.h"

template<class elemType>
void CircularQueue<elemType>::enQueue(elemType element) {
    if ((rear_ + 1) % max_size_ == front_) {
        doubleSpace();
    }
    rear_ = (rear_ + 1) % max_size_;
    queue_[rear_] = element;
}

template<class elemType>
void CircularQueue<elemType>::deQueue() {
    if (rear_ != front_) --rear_;
}

template<class elemType>
bool CircularQueue<elemType>::isEmpty() const {
    return rear_ == front_;
}

template<class elemType>
elemType CircularQueue<elemType>::getHead() const {
    return queue_[(front_ + 1) % max_size_];
}

template<class elemType>
void CircularQueue<elemType>::doubleSpace() {
    elemType* tmp = new elemType[2 * max_size_];
    int i = 0;
    for (; (front_ + i) % max_size_ != rear_; ++i) {
        tmp[i] = queue_[(front_ + i) % max_size_];
    }
    tmp[++i] = queue_[rear_];
    front_ = -1;
    rear_ = i;
    max_size_ *= 2;
    delete queue_;
    queue_ = tmp;
} 
