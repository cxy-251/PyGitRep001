#
# @lc app=leetcode id=67 lang=python3
#
# [67] Add Binary
#
# https://leetcode.com/problems/add-binary/description/
#
# algorithms
# Easy (55.37%)
# Likes:    9865
# Dislikes: 1039
# Total Accepted:    1.8M
# Total Submissions: 3.3M
# Testcase Example:  '"11"\n"1"'
#
# Given two binary strings a and b, return their sum as a binary string.
# 
# 
# Example 1:
# Input: a = "11", b = "1"
# Output: "100"
# Example 2:
# Input: a = "1010", b = "1011"
# Output: "10101"
# 
# 
# Constraints:
# 
# 
# 1 <= a.length, b.length <= 10^4
# a and b consist only of '0' or '1' characters.
# Each string does not contain leading zeros except for the zero itself.
# 
# 
#

# @lc code=start
class Solution:
    def addBinary(self, a: str, b: str) -> str:
        # 初始化结果和进位
        result = []
        carry = 0

        # 从两个字符串的末尾开始逐位相加
        i, j = len(a) - 1, len(b) - 1
        while i >= 0 or j >= 0 or carry:
            # 获取当前位的值，如果索引超出范围则为 0
            digit_a = int(a[i]) if i >= 0 else 0
            digit_b = int(b[j]) if j >= 0 else 0

            # 计算当前位的和以及进位
            total = digit_a + digit_b + carry
            carry = total // 2  # 进位
            result.append(str(total % 2))  # 当前位的结果

            # 移动到下一位
            i -= 1
            j -= 1

        # 结果需要反转，因为是从低位到高位计算的
        return ''.join(result[::-1])
# @lc code=end

