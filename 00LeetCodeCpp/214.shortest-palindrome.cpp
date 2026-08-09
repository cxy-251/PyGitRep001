/*
 * @lc app=leetcode id=214 lang=cpp
 *
 * [214] Shortest Palindrome
 */

// @lc code=start
class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;
        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());
        string t = s + "#" + rev_s;
        int n = t.length();
        vector<int> next(n, 0);
        for (int i = 1, j = 0; i < n; ++i) {
            while (j > 0 && t[i] != t[j]) {
                j = next[j - 1];
            }
            if (t[i] == t[j]) {
                j++;
            }
            next[i] = j;
        }
        int longestPalinePrefixLen = next[n - 1];
        string suffix = s.substr(longestPalinePrefixLen);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};
// @lc code=end

