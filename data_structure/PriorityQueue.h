
template<class elemType>
class PriorityQueue {
public:
    explicit PriorityQueue(int _capacity = 100)
        : capacity_(_capacity),
          size_(0) {
            queue_ = new elemType[capacity_];
        }
    PriorityQueue(elemType* _queue, int _size)
        : capacity_(2 * _size),
          size_(_size) {
            queue_ = new elemType[capacity_];
            for (int i = 0; i < _size; ++i) {
                queue_[i + 1] = _queue[i];
            }
            buildHeap();
        }
    ~PriorityQueue() { delete [] queue_; }

    void buildHeap();

    void enQueue(elemType element);
    elemType deQueue();
    elemType getHead() const { return queue_[1]; }
    bool isEmpty() const { return size_ == 0; }

private:
    void percolateDown(int i);
    void doubleSpace();
    
    int capacity_;
    int size_;
    elemType* queue_;

};
