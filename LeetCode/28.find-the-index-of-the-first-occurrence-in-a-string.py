#
# @lc app=leetcode id=28 lang=python3
#
# [28] Find the Index of the First Occurrence in a String
#
# https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/description/
#
# algorithms
# Easy (44.66%)
# Likes:    6628
# Dislikes: 489
# Total Accepted:    3.2M
# Total Submissions: 7.2M
# Testcase Example:  '"sadbutsad"\n"sad"'
#
# Given two strings needle and haystack, return the index of the first
# occurrence of needle in haystack, or -1 if needle is not part of haystack.
# 
# 
# Example 1:
# 
# 
# Input: haystack = "sadbutsad", needle = "sad"
# Output: 0
# Explanation: "sad" occurs at index 0 and 6.
# The first occurrence is at index 0, so we return 0.
# 
# 
# Example 2:
# 
# 
# Input: haystack = "leetcode", needle = "leeto"
# Output: -1
# Explanation: "leeto" did not occur in "leetcode", so we return -1.
# 
# 
# 
# Constraints:
# 
# 
# 1 <= haystack.length, needle.length <= 10^4
# haystack and needle consist of only lowercase English characters.
# 
# 
#

# @lc code=start
class Solution:
    def strStr(self, haystack: str, needle: str) -> int:
        # 获取 haystack 和 needle 的长度
        n, m = len(haystack), len(needle)

        # 如果 needle 为空，返回 0
        if m == 0:
            return 0

        # 遍历 haystack，检查每个子串是否等于 needle
        for i in range(n - m + 1):
            if haystack[i:i + m] == needle:
                return i

        # 如果没有找到，返回 -1
        return -1
# @lc code=end

