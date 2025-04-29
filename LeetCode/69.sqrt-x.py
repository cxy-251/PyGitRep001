#
# @lc app=leetcode id=69 lang=python3
#
# [69] Sqrt(x)
#
# https://leetcode.com/problems/sqrtx/description/
#
# algorithms
# Easy (40.16%)
# Likes:    8816
# Dislikes: 4575
# Total Accepted:    2.5M
# Total Submissions: 6.3M
# Testcase Example:  '4'
#
# Given a non-negative integer x, return the square root of x rounded down to
# the nearest integer. The returned integer should be non-negative as well.
# 
# You must not use any built-in exponent function or operator.
# 
# 
# For example, do not use pow(x, 0.5) in c++ or x ** 0.5 in python.
# 
# 
# 
# Example 1:
# 
# 
# Input: x = 4
# Output: 2
# Explanation: The square root of 4 is 2, so we return 2.
# 
# 
# Example 2:
# 
# 
# Input: x = 8
# Output: 2
# Explanation: The square root of 8 is 2.82842..., and since we round it down
# to the nearest integer, 2 is returned.
# 
# 
# 
# Constraints:
# 
# 
# 0 <= x <= 2^31 - 1
# 
# 
#

# @lc code=start
class Solution:
    def mySqrt(self, x: int) -> int:
        if x < 2:
            return x  # 对于 0 和 1，平方根就是它本身

        left, right = 1, x // 2  # 搜索范围从 1 到 x // 2
        while left <= right:
            mid = (left + right) // 2
            if mid * mid == x:
                return mid  # 找到精确的平方根
            elif mid * mid < x:
                left = mid + 1  # 搜索右半部分
            else:
                right = mid - 1  # 搜索左半部分

        return right  # 返回平方根的整数部分
# @lc code=end

