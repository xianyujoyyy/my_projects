
#ifndef BASE_STRINGPIECE_H
#define BASE_STRINGPIECE_H

#include <string.h>
#include <iosfwd>

#include "project1_websever/base/Types.h"

namespace NetLib {

class StringArg {
public:
    StringArg(const char* str) :
        str_(str) {}
    StringArg(const string& str) :
        str_(str.c_str()) {}

    const char* c_str() { return str_; }

private:
    const char* str_;
};

class StringPiece {
public:
    StringPiece() : prt_(NULL), length_(0) {}
    StringPiece(const char* str) :
        prt_(str), length_(static_cast<int>(strlen(str))) {}
    StringPiece(const unsigned char* str) :
        prt_(reinterpret_cast<const char*>(str)), length_(static_cast<int>(strlen(prt_))) {}
    StringPiece(const string& str) :
        prt_(str.data()), length_(static_cast<int>(str.size())) {}
    StringPiece(const char* offset, int len) :
        prt_(offset), length_(len) {}

    const char* data() { return prt_; }
    int size() { return length_; }
    bool empty() { return length_ == 0; }
    const char* begin() const { return prt_; }
    const char* end() const { return prt_ + length_; }

    void clear() { prt_ = NULL; length_ = 0; }
    void set(const char* buffer, int len) { prt_ = buffer; length_ = len; }
    void set(const char* str) { prt_ = str; length_ = static_cast<int>(strlen(str)); }
    void set(const void* buffer, int len) {
        prt_ = reinterpret_cast<const char*>(buffer);
        length_ = len;
    }

    char operator[](int i) { return prt_[i]; }

    void remove_prefix(int n) {
        prt_ += n;
        length_ -= n;
    }

    void remove_suffix(int n) {
        length_ -= n;
    }

    bool operator==(const StringPiece& x) const {
        return ((length_ == x.length_) &&
                (memcmp(prt_, x.prt_, length_) == 0));
    }

    bool operator!=(const StringPiece& x) const {
        return !(*this == x);
    }

    int compare(const StringPiece& x) const {
        int r = memcmp(prt_, x.prt_, length_ < x.length_ ? length_ : x.length_);
        if (r == 0) {
            if (length_ < x.length_) r = -1;
            else if (length_ > x.length_) r = 1;
        }
        return r;
    }

    string as_string() const {
        return string(data(), size());
    }

    void CopyToString(string* target) const {
        target->assign(prt_, length_);
    }

    bool starts_with(const StringPiece& x) const {
        return ((length_ > x.length_) && (memcmp(prt_, x.prt_, x.length_) == 0));
    }

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)    \
        bool operator cmp (const StringPiece& x) const \
        { int r = memcmp(prt_, x.prt_, length_ < x.length_ ? length_ : x.length_); \
          return ((r auxcmp 0) || (r == 0) && (length_ cmp x.length_)); }
    STRINGPIECE_BINARY_PREDICATE(<, <);
    STRINGPIECE_BINARY_PREDICATE(<=, <);
    STRINGPIECE_BINARY_PREDICATE(>, >);
    STRINGPIECE_BINARY_PREDICATE(>=, >);
#undef STRINGPIECE_BINARY_PREDICATE

private:
    const char* prt_;
    int         length_;
};

}   // namespace NetLib

#ifdef HAVE_TYPE_TRAITS
template<> struct __type_traits<NetLib::StringPiece> {
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_constuctor;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};
#endif

std::ostream& operator<<(std::ostream& o, const NetLib::StringPiece& piece);

#endif  // BASE_STRINGPIECE_H
