#
# @lc app=leetcode id=7 lang=python3
#
# [7] Reverse Integer
#
# https://leetcode.com/problems/reverse-integer/description/
#
# algorithms
# Medium (30.01%)
# Likes:    14009
# Dislikes: 13751
# Total Accepted:    4M
# Total Submissions: 13.2M
# Testcase Example:  '123'
#
# Given a signed 32-bit integer x, return x with its digits reversed. If
# reversing x causes the value to go outside the signed 32-bit integer range
# [-2^31, 2^31 - 1], then return 0.
# 
# Assume the environment does not allow you to store 64-bit integers (signed or
# unsigned).
# 
# 
# Example 1:
# 
# 
# Input: x = 123
# Output: 321
# 
# 
# Example 2:
# 
# 
# Input: x = -123
# Output: -321
# 
# 
# Example 3:
# 
# 
# Input: x = 120
# Output: 21
# 
# 
# 
# Constraints:
# 
# 
# -2^31 <= x <= 2^31 - 1
# 
# 
#

# @lc code=start
class Solution:
    def reverse(self, x: int) -> int:
        # # 定义 32 位整数的范围
        # INT_MIN, INT_MAX = -2**31, 2**31 - 1

        # # 记录输入的符号
        # sign = -1 if x < 0 else 1
        # x = abs(x)

        # # 反转整数
        # reversed_x = 0
        # while x != 0:
        #     digit = x % 10  # 提取最后一位数字
        #     x //= 10  # 去掉最后一位数字

        #     # 检查是否会溢出
        #     if reversed_x > (INT_MAX - digit) // 10:
        #         return 0

        #     reversed_x = reversed_x * 10 + digit

        # return sign * reversed_x
    
        # ------------------------------------------------
    
        # 定义 32 位整数的范围
        INT_MIN, INT_MAX = -2**31, 2**31 - 1

        # 记录输入的符号
        sign = -1 if x < 0 else 1
        x = abs(x)

        # 提前计算边界值，避免重复计算
        boundary = INT_MAX // 10

        # 反转整数
        reversed_x = 0
        while x > 0:
            digit = x % 10  # 提取最后一位数字
            x //= 10  # 去掉最后一位数字

            # 检查是否会溢出
            if reversed_x > boundary or (reversed_x == boundary and digit > 7):  # 7 是 INT_MAX 的个位数
                return 0

            reversed_x = reversed_x * 10 + digit

        return sign * reversed_x
        
# @lc code=end

