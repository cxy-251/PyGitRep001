/*
 * @lc app=leetcode id=3 lang=cpp
 *
 * [3] Longest Substring Without Repeating Characters
 */

// @lc code=start
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int res = 0;
        int left = 0;
        int last_pos[128] = {0};
        for (int right = 0; right < (int)s.length(); right++) {
            left = max(left, last_pos[s[right]]);
            res = max(res, right - left + 1);
            last_pos[s[right]] = right + 1;
        }
        return res;
    }
};
// @lc code=end

