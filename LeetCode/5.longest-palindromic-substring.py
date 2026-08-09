#
# @lc app=leetcode id=5 lang=python3
#
# [5] Longest Palindromic Substring
#
# https://leetcode.com/problems/longest-palindromic-substring/description/
#
# algorithms
# Medium (35.50%)
# Likes:    30589
# Dislikes: 1887
# Total Accepted:    3.8M
# Total Submissions: 10.6M
# Testcase Example:  '"babad"'
#
# Given a string s, return the longest palindromic substring in s.
# 
# 
# Example 1:
# 
# 
# Input: s = "babad"
# Output: "bab"
# Explanation: "aba" is also a valid answer.
# 
# 
# Example 2:
# 
# 
# Input: s = "cbbd"
# Output: "bb"
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 1000
# s consist of only digits and English letters.
# 
# 
#

# @lc code=start
class Solution:
    def longestPalindrome(self, s: str) -> str:
        if len(s) <= 1:  # 如果字符串长度为 1 或更短，直接返回
            return s

        start, max_length = 0, 0  # 记录最长回文子串的起始位置和长度

        # 定义一个内部函数，用于从中心扩展
        def expand_around_center(left: int, right: int):
            nonlocal start, max_length  # 使用外部变量
            # 当左右指针在字符串范围内，且字符相等时，继续扩展
            while left >= 0 and right < len(s) and s[left] == s[right]:
                current_length = right - left + 1  # 当前回文子串的长度
                if current_length > max_length:  # 如果当前回文更长，更新结果
                    start = left
                    max_length = current_length
                left -= 1  # 左指针左移
                right += 1  # 右指针右移

        # 遍历字符串中的每个字符
        for i in range(len(s)):
            # 奇数长度回文（中心是一个字符）
            expand_around_center(i, i)
            # 偶数长度回文（中心是两个字符之间）
            expand_around_center(i, i + 1)

        # 返回最长回文子串
        return s[start:start + max_length]
# @lc code=end

