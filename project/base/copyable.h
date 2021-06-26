
#ifndef PROJECT_BASE_COPYABLE_H
#define PROJECT_BASE_COPYABLE_H

#include <string>

using std::string;

namespace NetLib {

class copyable {
protected:
    copyable() = default;
    ~copyable() = default;
};

}   // namespace NetLib

#endif  // PROJECT_BASE_COPYABLE_H
