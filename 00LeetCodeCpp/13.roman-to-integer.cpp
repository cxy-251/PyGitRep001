/*
 * @lc app=leetcode id=13 lang=cpp
 *
 * [13] Roman to Integer
 */

// @lc code=start
#include <string>

class Solution {
public:
    int romanToInt(string s) {
        auto val = [](char c)->int {
            switch (c) {
                case 'I': return 1;
                case 'V': return 5;
                case 'X': return 10;
                case 'L': return 50;
                case 'C': return 100;
                case 'D': return 500;
                case 'M': return 1000;
                default: return 0;
            }
        };
        int n = s.size();
        int res = 0;
        for (int i = 0; i < n; ++i) {
            int v = val(s[i]);
            if (i + 1 < n && val(s[i + 1]) > v) res -= v;
            else res += v;
        }
        return res;
    }
};
// @lc code=end
// ...existing code...