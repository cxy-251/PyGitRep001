#
# @lc app=leetcode id=168 lang=python3
#
# [168] Excel Sheet Column Title
#
# https://leetcode.com/problems/excel-sheet-column-title/description/
#
# algorithms
# Easy (43.08%)
# Likes:    5791
# Dislikes: 871
# Total Accepted:    619.5K
# Total Submissions: 1.4M
# Testcase Example:  '1'
#
# Given an integer columnNumber, return its corresponding column title as it
# appears in an Excel sheet.
# 
# For example:
# 
# 
# A -> 1
# B -> 2
# C -> 3
# ...
# Z -> 26
# AA -> 27
# AB -> 28 
# ...
# 
# 
# 
# Example 1:
# 
# 
# Input: columnNumber = 1
# Output: "A"
# 
# 
# Example 2:
# 
# 
# Input: columnNumber = 28
# Output: "AB"
# 
# 
# Example 3:
# 
# 
# Input: columnNumber = 701
# Output: "ZY"
# 
# 
# 
# Constraints:
# 
# 
# 1 <= columnNumber <= 2^31 - 1
# 
# 
#

# @lc code=start
class Solution:
    def convertToTitle(self, columnNumber: int) -> str:
        result = []
        
        while columnNumber > 0:
            # 计算当前字符
            columnNumber -= 1
            remainder = columnNumber % 26
            result.append(chr(remainder + ord('A')))
            # 更新 columnNumber
            columnNumber //= 26
        
        # 结果需要反转，因为从低位到高位生成
        return ''.join(result[::-1])
        
# @lc code=end

