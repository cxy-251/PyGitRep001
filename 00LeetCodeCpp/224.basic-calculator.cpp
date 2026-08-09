/*
 * @lc app=leetcode id=224 lang=cpp
 *
 * [224] Basic Calculator
 */

// @lc code=start
class Solution {
public:
    int calculate(string s) {
        stack<long> st;
        long res = 0;
        long num = 0;
        long sign = 1;
        for (long i = 0; i < s.length(); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            } else if (c == '+') {
                res += sign * num;
                num = 0;
                sign = 1;
            } else if (c == '-') {
                res += sign * num;
                num = 0;
                sign = -1;
            } else if (c == '(') {
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            } else if (c == ')') {
                res += sign * num;
                num = 0;
                long prevSign = st.top();
                st.pop();
                res *= prevSign;
                long prevRes = st.top();
                st.pop();
                res += prevRes;
            }
        }
        res += sign * num;
        return (int)res;
    }
};
// @lc code=end

