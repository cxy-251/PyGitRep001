#
# @lc app=leetcode id=29 lang=python3
#
# [29] Divide Two Integers
#
# https://leetcode.com/problems/divide-two-integers/description/
#
# algorithms
# Medium (18.24%)
# Likes:    5564
# Dislikes: 15073
# Total Accepted:    919.9K
# Total Submissions: 5M
# Testcase Example:  '10\n3'
#
# Given two integers dividend and divisor, divide two integers without using
# multiplication, division, and mod operator.
# 
# The integer division should truncate toward zero, which means losing its
# fractional part. For example, 8.345 would be truncated to 8, and -2.7335
# would be truncated to -2.
# 
# Return the quotient after dividing dividend by divisor.
# 
# Note: Assume we are dealing with an environment that could only store
# integers within the 32-bit signed integer range: [−2^31, 2^31 − 1]. For this
# problem, if the quotient is strictly greater than 2^31 - 1, then return 2^31
# - 1, and if the quotient is strictly less than -2^31, then return -2^31.
# 
# 
# Example 1:
# 
# 
# Input: dividend = 10, divisor = 3
# Output: 3
# Explanation: 10/3 = 3.33333.. which is truncated to 3.
# 
# 
# Example 2:
# 
# 
# Input: dividend = 7, divisor = -3
# Output: -2
# Explanation: 7/-3 = -2.33333.. which is truncated to -2.
# 
# 
# 
# Constraints:
# 
# 
# -2^31 <= dividend, divisor <= 2^31 - 1
# divisor != 0
# 
# 
#

# @lc code=start
class Solution:
    def divide(self, dividend: int, divisor: int) -> int:
        # 处理溢出情况
        INT_MAX = 2**31 - 1
        INT_MIN = -2**31
        if dividend == INT_MIN and divisor == -1:
            return INT_MAX

        # 确定结果的符号
        negative = (dividend < 0) != (divisor < 0)

        # 转换为正数进行计算
        dividend, divisor = abs(dividend), abs(divisor)
        quotient = 0

        # 使用位运算模拟除法
        while dividend >= divisor:
            temp, multiple = divisor, 1
            while dividend >= (temp << 1):
                temp <<= 1
                multiple <<= 1
            dividend -= temp
            quotient += multiple

        # 根据符号返回结果
        return -quotient if negative else quotient
# @lc code=end

