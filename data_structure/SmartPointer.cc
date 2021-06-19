#include <atomic>

template<typename T>
struct pointer {
    T* pointer_;
    std::atomic<int> use_count_;
    pointer(const T* _p)
        : pointer_(_p) {
            use_count_.store(1);
        }
    ~pointer() {
        delete pointer_;
    }
};

// shared_prt
template<typename T>
class shared_prt {
public:
    shared_prt() {}

    shared_prt(const T* _pointer)
        : p_(new pointer(_pointer)) {}

    shared_prt(const shared_prt& other)
        : p_(other.p_) {
            ++(p_->use_count);
        }

    void operator=(const shared_prt& rhs) {
        if (*this == rhs) return;
        if (p_) {
        --(p_->use_count_);
        if (p_->use_count_ == 0) {
            delete p_;
        }
        }
        p_ = rhs.p_;
        if (p_) ++(p_->use_count_);
        return;
    }

    T* get() { if (p_) return p_->pointer_; }

private:
    pointer* p_ = nullptr;

};
