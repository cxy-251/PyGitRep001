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
        if len(s) <= 1:
            return s

        start, max_length = 0, 0

        def expand_around_center(left: int, right: int):
            nonlocal start, max_length
            while left >= 0 and right < len(s) and s[left] == s[right]:
                current_length = right - left + 1
                if current_length > max_length:
                    start = left
                    max_length = current_length
                left -= 1
                right += 1

        for i in range(len(s)):
            # 奇数长度回文
            expand_around_center(i, i)
            # 偶数长度回文
            expand_around_center(i, i + 1)

        return s[start:start + max_length]
                    

# @lc code=end

