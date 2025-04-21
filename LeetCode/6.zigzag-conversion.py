#
# @lc app=leetcode id=6 lang=python3
#
# [6] Zigzag Conversion
#
# https://leetcode.com/problems/zigzag-conversion/description/
#
# algorithms
# Medium (51.08%)
# Likes:    8405
# Dislikes: 15285
# Total Accepted:    1.7M
# Total Submissions: 3.3M
# Testcase Example:  '"PAYPALISHIRING"\n3'
#
# The string "PAYPALISHIRING" is written in a zigzag pattern on a given number
# of rows like this: (you may want to display this pattern in a fixed font for
# better legibility)
# 
# 
# P   A   H   N
# A P L S I I G
# Y   I   R
# 
# 
# And then read line by line: "PAHNAPLSIIGYIR"
# 
# Write the code that will take a string and make this conversion given a
# number of rows:
# 
# 
# string convert(string s, int numRows);
# 
# 
# 
# Example 1:
# 
# 
# Input: s = "PAYPALISHIRING", numRows = 3
# Output: "PAHNAPLSIIGYIR"
# 
# 
# Example 2:
# 
# 
# Input: s = "PAYPALISHIRING", numRows = 4
# Output: "PINALSIGYAHRPI"
# Explanation:
# P     I    N
# A   L S  I G
# Y A   H R
# P     I
# 
# 
# Example 3:
# 
# 
# Input: s = "A", numRows = 1
# Output: "A"
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 1000
# s consists of English letters (lower-case and upper-case), ',' and '.'.
# 1 <= numRows <= 1000
# 
# 
#

# @lc code=start
class Solution:
    def convert(self, s: str, numRows: int) -> str:
        # 如果只有一行或者字符串长度小于等于行数，直接返回原字符串
        if numRows == 1 or numRows >= len(s):
            return s

        # 创建一个列表，每一行对应一个字符串
        rows = [''] * numRows
        current_row = 0  # 当前行
        going_down = False  # 方向标志

        # 遍历字符串，将字符放入对应的行
        for char in s:
            rows[current_row] += char
            # 如果到达第一行或最后一行，改变方向
            if current_row == 0 or current_row == numRows - 1:
                going_down = not going_down
            # 根据方向移动到下一行
            current_row += 1 if going_down else -1

        # 将所有行拼接成最终结果
        return ''.join(rows)
        
# @lc code=end

