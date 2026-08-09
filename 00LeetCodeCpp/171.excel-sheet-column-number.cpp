/*
 * @lc app=leetcode id=171 lang=cpp
 *
 * [171] Excel Sheet Column Number
 */

// @lc code=start
class Solution {
public:
    int titleToNumber(string columnTitle) {
        long long result = 0;
        for (char c : columnTitle) {
            int val = c - 'A' + 1;
            result = result * 26 + val;
        }
        return (int)result;
    }
};
// @lc code=end

