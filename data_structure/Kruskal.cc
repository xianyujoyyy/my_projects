
// priority_queue + union and find

#include <vector>
#include <iostream>
#include <queue>

using namespace std;

struct Node {
    int x;
    int y;
    int w;
    bool operator<(const Node& other) {
        return w < other.w;
    }
};

void Kruskal(vector<Node>& g) {
    priority_queue<Node, vector<Node>, greater<Node>()> pq;
    for (int i = 0; i < g.size(); ++i) pq.push(g[i]);
    while (!pq.empty()) {
        int x = pq.top().x;
        int y = pq.top().y;
        if (find(y) != x) {
            union(x, parent[y]);
            cout << x << y << endl;
        }
        pq.pop();
    }
    return;
}
