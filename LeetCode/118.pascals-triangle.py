#
# @lc app=leetcode id=118 lang=python3
#
# [118] Pascal's Triangle
#
# https://leetcode.com/problems/pascals-triangle/description/
#
# algorithms
# Easy (76.64%)
# Likes:    13680
# Dislikes: 505
# Total Accepted:    2.1M
# Total Submissions: 2.7M
# Testcase Example:  '5'
#
# Given an integer numRows, return the first numRows of Pascal's triangle.
# 
# In Pascal's triangle, each number is the sum of the two numbers directly
# above it as shown:
# 
# 
# Example 1:
# Input: numRows = 5
# Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
# Example 2:
# Input: numRows = 1
# Output: [[1]]
# 
# 
# Constraints:
# 
# 
# 1 <= numRows <= 30
# 
# 
#

# @lc code=start
class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        # 初始化结果列表
        result = []

        for i in range(numRows):
            # 创建当前行，初始值为 1
            row = [1] * (i + 1)
            # 填充当前行的中间部分
            for j in range(1, i):
                row[j] = result[i - 1][j - 1] + result[i - 1][j]
            # 将当前行加入结果
            result.append(row)

        return result
        
# @lc code=end

