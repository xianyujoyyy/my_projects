
#include "data_structure/HuffmanTree.h"

void HfTree::getCode(HfCode*& res) {
    res = new HfCode[length_ / 2];
    int j = 0;
    for (int i = length_ / 2; i < length_; ++i) {
        while (queue_[i].parent != 0) {
            res[i].word = queue_[i].word;
            if (queue_[queue_[i].parent].left == i) {
                res[i].code = "0" + res[i].code;
            }
            else res[i].code = "1" + res[i].code;
        }
    }
}
