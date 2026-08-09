// 执行命令：chmod +x 0collection1-array.cpp && ./0collection1-array.cpp
#if 0
#这一段是给 shell 读的
g++ -std=c++17 "$0" -o /tmp/leetcode_test && /tmp/leetcode_test && exit
#endif

#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
using namespace std;

namespace VectorBasic
{
    void test() {
        vector<string> v;
        v.push_back("123");
        cout << v.size() << endl;
        for (auto it = v.begin(); it != v.end();) {
            if (*it == "123") {
                it = v.erase(it);
            } else {
                ++it;
            }
        }
        v.erase(remove(v.begin(), v.end(), "123"), v.end());
        list<string> l;
        l.push_front("abc");
        list<pair<int, int>> lp;
        auto it = l.begin();
        advance(it, 0);
    }
} // namespace VectorBasic

int main() {
    VectorBasic::test();
    return 0;
}