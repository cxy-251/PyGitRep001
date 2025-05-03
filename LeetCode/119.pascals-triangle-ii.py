#
# @lc app=leetcode id=119 lang=python3
#
# [119] Pascal's Triangle II
#
# https://leetcode.com/problems/pascals-triangle-ii/description/
#
# algorithms
# Easy (65.66%)
# Likes:    5023
# Dislikes: 358
# Total Accepted:    1M
# Total Submissions: 1.6M
# Testcase Example:  '3'
#
# Given an integer rowIndex, return the rowIndex^th (0-indexed) row of the
# Pascal's triangle.
# 
# In Pascal's triangle, each number is the sum of the two numbers directly
# above it as shown:
# 
# 
# Example 1:
# Input: rowIndex = 3
# Output: [1,3,3,1]
# Example 2:
# Input: rowIndex = 0
# Output: [1]
# Example 3:
# Input: rowIndex = 1
# Output: [1,1]
# 
# 
# Constraints:
# 
# 
# 0 <= rowIndex <= 33
# 
# 
# 
# Follow up: Could you optimize your algorithm to use only O(rowIndex) extra
# space?
# 
#

# @lc code=start
class Solution:
    def getRow(self, rowIndex: int) -> List[int]:
        # 初始化第一行
        row = [1]
        
        # 逐步生成每一行
        for i in range(1, rowIndex + 1):
            # 从后往前更新当前行，避免覆盖
            row.append(0)  # 为了方便计算，先扩展一位
            for j in range(i, 0, -1):
                row[j] += row[j - 1]
        
        return row
        
# @lc code=end

