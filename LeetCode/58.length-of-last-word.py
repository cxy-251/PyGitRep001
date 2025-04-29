#
# @lc app=leetcode id=58 lang=python3
#
# [58] Length of Last Word
#
# https://leetcode.com/problems/length-of-last-word/description/
#
# algorithms
# Easy (55.78%)
# Likes:    5684
# Dislikes: 324
# Total Accepted:    2.7M
# Total Submissions: 4.8M
# Testcase Example:  '"Hello World"'
#
# Given a string s consisting of words and spaces, return the length of the
# last word in the string.
# 
# A word is a maximal substring consisting of non-space characters only.
# 
# 
# Example 1:
# 
# 
# Input: s = "Hello World"
# Output: 5
# Explanation: The last word is "World" with length 5.
# 
# 
# Example 2:
# 
# 
# Input: s = "   fly me   to   the moon  "
# Output: 4
# Explanation: The last word is "moon" with length 4.
# 
# 
# Example 3:
# 
# 
# Input: s = "luffy is still joyboy"
# Output: 6
# Explanation: The last word is "joyboy" with length 6.
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 10^4
# s consists of only English letters and spaces ' '.
# There will be at least one word in s.
# 
# 
#

# @lc code=start
class Solution:
    def lengthOfLastWord(self, s: str) -> int:
        # 去掉字符串两端的空格
        s = s.strip()
        # 按空格分割字符串
        words = s.split(" ")
        # 返回最后一个单词的长度
        return len(words[-1])
# @lc code=end

