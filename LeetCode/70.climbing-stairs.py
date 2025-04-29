#
# @lc app=leetcode id=70 lang=python3
#
# [70] Climbing Stairs
#
# https://leetcode.com/problems/climbing-stairs/description/
#
# algorithms
# Easy (53.41%)
# Likes:    23049
# Dislikes: 953
# Total Accepted:    4.1M
# Total Submissions: 7.7M
# Testcase Example:  '2'
#
# You are climbing a staircase. It takes n steps to reach the top.
# 
# Each time you can either climb 1 or 2 steps. In how many distinct ways can
# you climb to the top?
# 
# 
# Example 1:
# 
# 
# Input: n = 2
# Output: 2
# Explanation: There are two ways to climb to the top.
# 1. 1 step + 1 step
# 2. 2 steps
# 
# 
# Example 2:
# 
# 
# Input: n = 3
# Output: 3
# Explanation: There are three ways to climb to the top.
# 1. 1 step + 1 step + 1 step
# 2. 1 step + 2 steps
# 3. 2 steps + 1 step
# 
# 
# 
# Constraints:
# 
# 
# 1 <= n <= 45
# 
# 
#

# @lc code=start
class Solution:
    def climbStairs(self, n: int) -> int:
        # if n <= 2:
        #     return n  # 如果只有 1 或 2 阶楼梯，返回 n

        # # 初始化 dp 数组
        # dp = [0] * (n + 1)
        # dp[1], dp[2] = 1, 2  # 第一阶有 1 种方法，第二阶有 2 种方法

        # # 动态规划计算每一阶的爬法
        # for i in range(3, n + 1):
        #     dp[i] = dp[i - 1] + dp[i - 2]  # 当前阶的爬法等于前两阶爬法之和

        # return dp[n]
    
        if n <= 2:
            return n

        prev, curr = 1, 2  # 初始化前两阶的爬法
        for i in range(3, n + 1):
            prev, curr = curr, prev + curr  # 更新前两阶的爬法

        return curr
# @lc code=end

