
// empty:front == rear
// full:front == (rear + 1) % maxSize

template<class elemType>
class CircularQueue {
public:
    explicit CircularQueue(int _max_size = 10)
        : front_(-1),
          rear_(-1),
          max_size_(_max_size) {
              queue_ = new elemType[max_size_];
        }
    ~CircularQueue() {
        delete [] queue_;
    }

    void enQueue(elemType element);
    void deQueue();

    bool isEmpty() const;

    elemType getHead() const;

private:
    void doubleSpace();
    
    elemType* queue_;
    int       front_;
    int       rear_;
    int       max_size_;

};
