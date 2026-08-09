/*
 * @lc app=leetcode id=132 lang=cpp
 *
 * [132] Palindrome Partitioning II
 */

// @lc code=start
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        if (n <= 1) return 0;
        vector<vector<bool>> isPal(n, vector<bool>(n, false));
        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    if (len <= 2 || isPal[i + 1][j - 1]) {
                        isPal[i][j] = true;
                    }
                }
            }
        }
        vector<int> dp(n);
        for (int i = 0; i < n; ++i) {
            if (isPal[0][i]) {
                dp[i] = 0;
            } else {
                int minCuts = i;
                for (int j = 1; j <= i; ++j) {
                    if (isPal[j][i]) {
                        minCuts = min(minCuts, dp[j - 1] + 1);
                    }
                }
                dp[i] = minCuts;
            }
        }
        return dp[n - 1];
    }
};
// @lc code=end

