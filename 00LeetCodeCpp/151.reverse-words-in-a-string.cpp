/*
 * @lc app=leetcode id=151 lang=cpp
 *
 * [151] Reverse Words in a String
 */

// @lc code=start
class Solution {
public:
    string reverseWords(string s) {
        int n = s.length();
        int slow = 0;
        for (int fast = 0; fast < n; ++fast) {
            if (s[fast] != ' ') {
                if (slow != 0) s[slow++] = ' ';
                while (fast < n && s[fast] != ' ') {
                    s[slow++] = s[fast++];
                }
            }
        }
        s.resize(slow);
        reverse(s.begin(), s.end());
        int start = 0;
        for (int end = 0; end < s.length(); ++end) {
            if (s[end] == ' ') {
                reverse(s.begin() + start, s.begin() + end);
                start = end + 1;
            } else {
                if (end == s.length() - 1) {
                    reverse(s.begin() + start, s.begin() + end + 1);
                }
            }
        }
        return s;
    }
};
// @lc code=end

