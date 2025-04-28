#
# @lc app=leetcode id=38 lang=python3
#
# [38] Count and Say
#
# https://leetcode.com/problems/count-and-say/description/
#
# algorithms
# Medium (58.19%)
# Likes:    4698
# Dislikes: 8806
# Total Accepted:    1.2M
# Total Submissions: 2M
# Testcase Example:  '1'
#
# The count-and-say sequence is a sequence of digit strings defined by the
# recursive formula:
# 
# 
# countAndSay(1) = "1"
# countAndSay(n) is the run-length encoding of countAndSay(n - 1).
# 
# 
# Run-length encoding (RLE) is a string compression method that works by
# replacing consecutive identical characters (repeated 2 or more times) with
# the concatenation of the character and the number marking the count of the
# characters (length of the run). For example, to compress the string "3322251"
# we replace "33" with "23", replace "222" with "32", replace "5" with "15" and
# replace "1" with "11". Thus the compressed string becomes "23321511".
# 
# Given a positive integer n, return the n^th element of the count-and-say
# sequence.
# 
# 
# Example 1:
# 
# 
# Input: n = 4
# 
# Output: "1211"
# 
# Explanation:
# 
# 
# countAndSay(1) = "1"
# countAndSay(2) = RLE of "1" = "11"
# countAndSay(3) = RLE of "11" = "21"
# countAndSay(4) = RLE of "21" = "1211"
# 
# 
# 
# Example 2:
# 
# 
# Input: n = 1
# 
# Output: "1"
# 
# Explanation:
# 
# This is the base case.
# 
# 
# 
# Constraints:
# 
# 
# 1 <= n <= 30
# 
# 
# 
# Follow up: Could you solve it iteratively?
#

# @lc code=start
class Solution:
    def countAndSay(self, n: int) -> str:
        # 基础情况
        if n == 1:
            return "1"

        # 获取前一个序列
        prev = self.countAndSay(n - 1)
        result = []
        count = 1

        # 遍历前一个序列，生成当前序列
        for i in range(1, len(prev)):
            if prev[i] == prev[i - 1]:
                count += 1  # 当前字符重复，计数加一
            else:
                # 当前字符不同，将前一个字符的计数和字符加入结果
                result.append(str(count))
                result.append(prev[i - 1])
                count = 1  # 重置计数

        # 处理最后一个字符
        result.append(str(count))
        result.append(prev[-1])

        return "".join(result)
# @lc code=end

