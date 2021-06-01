
// 路径压缩，按规模并

class DisjointSet {
public:
    explicit DisjointSet(int _size);
    ~DisjointSet() {
        delete [] parent;
    }

    int Find(int x);
    void Union(int root1, int root2);

private:
    int size_;
    int* parent;

};

DisjointSet::DisjointSet(int _size)
                : size_(_size) {
                    parent = new int[size_];
                    for (int i = 0; i < size_; ++i) parent[i] = -1;
                }
// 一边遍历查找，一边改parent
int DisjointSet::Find(int x) {
    if (parent[x] < 0) return x;
    parent[x] = Find(parent[x]);
    return parent[x];
}
// 将规模小的并入规模大的
void DisjointSet::Union(int root1, int root2) {
    if (root1 == root2) return;
    if (parent[root1] < parent[root2]) {
        parent[root1] += parent[root2];
        parent[root2] = root1;
    }
    else {
        parent[root2] += parent[root1];
        parent[root1] = root2;
    }
    return;
}
