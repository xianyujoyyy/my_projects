
template <class element>
class deque {
public:
    typedef element* iterator;
    typedef iterator* map_pointer;

    deque(int _cap = 20);
    deque(int _cap = 20, const element& ele);

    ~deque();

    // copy control
    deque(const deque&);
    void operator=(const deque&);
    void operator=(const deque&&);
    // add
    void push_front(const element& ele);
    void push_back(const element& ele);
    iterator insert(iterator pos, const element& ele);
    // remove
    void pop_front();
    void pop_back();
    iterator erase(iterator pos);
    void clear();
    // capacity
    int capacity() const { return capacity_; }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    // iterator
    void begin() { return begin_; }
    void end() { return end_; }
    element& front() { return *begin_; }
    element& back() { return *end_; }
    iterator operator++();
    iterator operator++(int);

    void swap(const deque&);

private:
    iterator setNewBlock();
    int capacity_;
    int size_;
    iterator begin_;
    iterator end_;
    iterator cur_block_;
    pointer cur_;
    pointer first_;
    pointer last_;
    // 扩大map的方法是重新allocate空间。
    
};
