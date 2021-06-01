
#include <string>
#include <limits.h>

using std::string;

class HfTree {
public:
    struct HfCode {
        char word;
        string code = "";
    };

    HfTree(char* _words, int* _weight, int _size)
        : length_(2 * _size) {
            queue_ = new HfNode[length_];
            for (int i = _size; i < length_; ++i) {
                queue_[i] = HfNode(_words[i - _size], _weight[i - _size]);
            }
            // build HfTree
            for (int i = _size - 1; i > 0; --i) {
                int mn1 = INT_MAX, mn2 = INT_MAX;
                int l = 0, r = 0;
                for (int j = i + 1; j < length_; ++j) {
                    if (queue_[j].parent == 0) {
                        if (queue_[j].weight <= mn1) {
                            mn2 = mn1;
                            mn2 = queue_[j].weight;
                            r = l;
                            l = j;
                        }
                        else if (queue_[j].weight < mn2) {
                            mn2 = queue_[j].weight;
                            r = j;
                        }
                    }
                }
                queue_[i].weight = mn1 + mn2;
                queue_[i].parent = 0;
                queue_[i].left = l;
                queue_[i].right = r;
            }
        }
    ~HfTree() { delete [] queue_; }

    void getCode(HfCode*&);

private:
    struct HfNode {
        char word;
        int weight;
        int left, right, parent;
        HfNode() = default;
        HfNode(char _word, int _w)
            : word(_word),
              weight(_w),
              left(0),
              right(0),
              parent(0) {}
    };
    
    HfNode* queue_;
    int length_;

};
