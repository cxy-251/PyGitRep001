/*
 * @lc app=leetcode id=279 lang=cpp
 *
 * [279] Perfect Squares
 */

// @lc code=start
class Solution {
public:
    int numSquares(int n) {
        // dp[i] 表示组成整数 i 所需的最少完全平方数数量
        // 初始化为 n，因为最坏情况是全部由 1 组成 (1+1...+1)，数量为 n
        vector<int> dp(n + 1, n);
        
        // 基础情况
        dp[0] = 0;

        // 外层循环：计算从 1 到 n 的每一个 dp 值
        for (int i = 1; i <= n; ++i) {
            // 内层循环：尝试所有小于等于 i 的完全平方数 j*j
            for (int j = 1; j * j <= i; ++j) {
                // 状态转移：当前数量 = 减去一个平方数后的数量 + 1
                dp[i] = min(dp[i], dp[i - j * j] + 1);
            }
        }

        return dp[n];
    }
};
// @lc code=end

