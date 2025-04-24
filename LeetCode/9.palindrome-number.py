#
# @lc app=leetcode id=9 lang=python3
#
# [9] Palindrome Number
#
# https://leetcode.com/problems/palindrome-number/description/
#
# algorithms
# Easy (58.87%)
# Likes:    13858
# Dislikes: 2805
# Total Accepted:    6.2M
# Total Submissions: 10.5M
# Testcase Example:  '121'
#
# Given an integer x, return true if x is a palindrome, and false otherwise.
# 
# 
# Example 1:
# 
# 
# Input: x = 121
# Output: true
# Explanation: 121 reads as 121 from left to right and from right to left.
# 
# 
# Example 2:
# 
# 
# Input: x = -121
# Output: false
# Explanation: From left to right, it reads -121. From right to left, it
# becomes 121-. Therefore it is not a palindrome.
# 
# 
# Example 3:
# 
# 
# Input: x = 10
# Output: false
# Explanation: Reads 01 from right to left. Therefore it is not a
# palindrome.
# 
# 
# 
# Constraints:
# 
# 
# -2^31 <= x <= 2^31 - 1
# 
# 
# 
# Follow up: Could you solve it without converting the integer to a string?
#

# @lc code=start
class Solution:
    def isPalindrome(self, x: int) -> bool:
        # 负数和以 0 结尾的非零数不可能是回文
        if x < 0 or (x % 10 == 0 and x != 0):
            return False

        reversed_half = 0
        while x > reversed_half:
            # 提取最后一位数字并将其添加到反转的数字中
            reversed_half = reversed_half * 10 + x % 10
            x //= 10

        # 如果数字是回文，则原始数字的前半部分等于反转的后半部分
        # 或者在数字长度为奇数时，去掉中间数字后仍相等
        return x == reversed_half or x == reversed_half // 10
        
# @lc code=end

