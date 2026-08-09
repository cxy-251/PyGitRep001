// ...existing code...
/*
 * @lc app=leetcode id=20 lang=cpp
 *
 * [20] Valid Parentheses
 */

// @lc code=start
#include <string>
#include <stack>
#include <unordered_map>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        unordered_map<char,char> pairs = {{')','('}, {'}','{'}, {']','['}};
        for (char c : s) {
            if (pairs.count(c)) {                // 右括号：检查栈顶是否匹配
                if (st.empty() || st.top() != pairs[c]) return false;
                st.pop();
            } else {                             // 左括号：入栈
                st.push(c);
            }
        }
        return st.empty();                       // 全部匹配则栈空
    }
};
// @lc code=end
// ...existing code...