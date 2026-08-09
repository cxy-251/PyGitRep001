/*
 * @lc app=leetcode id=44 lang=cpp
 *
 * [44] Wildcard Matching
 */

// @lc code=start
class Solution {
public:
    bool isMatch(string s, string p) {
        int sIdx = 0, pIdx = 0;
        int starIdx = -1, sMatch = 0;
        while (sIdx < s.length()) {
            if (pIdx < p.length() && (p[pIdx] == '?' || p[pIdx] == s[sIdx])) {
                sIdx++;
                pIdx++;
            } else if (pIdx < p.length() && p[pIdx] == '*') {
                starIdx = pIdx;
                sMatch = sIdx;
                pIdx++;
            } else if (starIdx != -1) {
                pIdx = starIdx + 1;
                sMatch++;
                sIdx = sMatch;
            } else {
                return false;
            }
        }
        while (pIdx < p.length() && p[pIdx] == '*') {
            pIdx++;
        }
        return pIdx == p.length();
    }
};
// @lc code=end

