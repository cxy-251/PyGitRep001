/*
 * @lc app=leetcode id=76 lang=cpp
 *
 * [76] Minimum Window Substring
 */

// @lc code=start
class Solution {
public:
    string minWindow(string s, string t) {
        if (s.empty() || t.empty()) return "";
        int need[128] = {0};
        int window[128] = {0};
        int uniqueCharsInT = 0;
        for (char c : t) {
            if (need[c] == 0) uniqueCharsInT++;
            need[c]++;
        }
        int left = 0, right = 0, valid = 0;
        int start = 0, minLen = INT_MAX;
        while (right < s.length()) {
            int rChar = (unsigned char)s[right];
            right++;
            if (need[rChar] > 0) {
                window[rChar]++;
                if (window[rChar] == need[rChar]) valid++;
            }
            while (valid == uniqueCharsInT) {
                if (right - left < minLen) {
                    start = left;
                    minLen = right - left;
                }
                int lChar = (unsigned char)s[left];
                left++;
                if (need[lChar] > 0) {
                    if (window[lChar] == need[lChar]) valid--;
                    window[lChar]--;
                }
            }
        }
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
// @lc code=end

