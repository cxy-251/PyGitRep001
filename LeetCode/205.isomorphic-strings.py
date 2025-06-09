#
# @lc app=leetcode id=205 lang=python3
#
# [205] Isomorphic Strings
#
# https://leetcode.com/problems/isomorphic-strings/description/
#
# algorithms
# Easy (46.57%)
# Likes:    9734
# Dislikes: 2205
# Total Accepted:    1.8M
# Total Submissions: 3.8M
# Testcase Example:  '"egg"\n"add"'
#
# Given two strings s and t, determine if they are isomorphic.
# 
# Two strings s and t are isomorphic if the characters in s can be replaced to
# get t.
# 
# All occurrences of a character must be replaced with another character while
# preserving the order of characters. No two characters may map to the same
# character, but a character may map to itself.
# 
# 
# Example 1:
# 
# 
# Input: s = "egg", t = "add"
# 
# Output: true
# 
# Explanation:
# 
# The strings s and t can be made identical by:
# 
# 
# Mapping 'e' to 'a'.
# Mapping 'g' to 'd'.
# 
# 
# 
# Example 2:
# 
# 
# Input: s = "foo", t = "bar"
# 
# Output: false
# 
# Explanation:
# 
# The strings s and t can not be made identical as 'o' needs to be mapped to
# both 'a' and 'r'.
# 
# 
# Example 3:
# 
# 
# Input: s = "paper", t = "title"
# 
# Output: true
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 5 * 10^4
# t.length == s.length
# s and t consist of any valid ascii character.
# 
# 
#

# @lc code=start
class Solution:
    def isIsomorphic(self, s: str, t: str) -> bool:
        mapping_s_t = {}
        mapping_t_s = {}
        for a, b in zip(s, t):
            if (a in mapping_s_t and mapping_s_t[a] != b) or (b in mapping_t_s and mapping_t_s[b] != a):
                return False
            mapping_s_t[a] = b
            mapping_t_s[b] = a
        return True
# @lc code=end

