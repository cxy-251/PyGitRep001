/*
 * @lc app=leetcode id=32 lang=cpp
 *
 * [32] Longest Valid Parentheses
 */

// @lc code=start
class Solution {
public:
    int longestValidParentheses(string s) {
        // stack<int> st;
        // st.push(-1);
        // int maxLen = 0;
        // for (int i = 0; i < s.length(); i++) {
        //     if (s[i] == '(') {
        //         st.push(i);
        //     } else {
        //         st.pop();
        //         if (st.empty()) {
        //             st.push(i);
        //         } else {
        //             maxLen = max(maxLen, i - st.top());
        //         }
        //     }
        // }

        int left = 0, right = 0, maxLen = 0;
        int n = s.length();
        for (int i = 0; i < n; i++) {
            if (s[i] == '(') left++;
            else right++;

            if (left == right) {
                maxLen = max(maxLen, 2 * right);
            } else if (right > left) {
                left = right = 0;
            }
        }

        left = right = 0;
        for (int i = n - 1; i >= 0; i--) {
            if (s[i] == '(') left++;
            else right++;

            if (left == right) {
                maxLen = max(maxLen, 2 * left);
            } else if (left > right) {
                left = right = 0;
            }
        }
        return maxLen;
    }
};
// @lc code=end

