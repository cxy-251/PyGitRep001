/*
 * @lc app=leetcode id=150 lang=cpp
 *
 * [150] Evaluate Reverse Polish Notation
 */

// @lc code=start
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> s;
        for (const string& t : tokens) {
            if (t == "+" || t == "-" || t == "*" || t == "/") {
                int val2 = s.top(); s.pop();
                int val1 = s.top(); s.pop();
                if (t == "+") s.push(val1 + val2);
                else if (t == "-") s.push(val1 - val2);
                else if (t == "*") s.push(val1 * val2);
                else if (t == "/") s.push(val1 / val2);
            } else {
                s.push(stoi(t));
            }
        }
        return s.top();
    }
};
// @lc code=end

