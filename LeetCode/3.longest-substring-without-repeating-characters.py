#
# @lc app=leetcode id=3 lang=python3
#
# [3] Longest Substring Without Repeating Characters
#
# https://leetcode.com/problems/longest-substring-without-repeating-characters/description/
#
# algorithms
# Medium (36.55%)
# Likes:    41680
# Dislikes: 2015
# Total Accepted:    7.2M
# Total Submissions: 19.8M
# Testcase Example:  '"abcabcbb"'
#
# Given a string s, find the length of the longest substring without duplicate
# characters.
# 
# 
# Example 1:
# 
# 
# Input: s = "abcabcbb"
# Output: 3
# Explanation: The answer is "abc", with the length of 3.
# 
# 
# Example 2:
# 
# 
# Input: s = "bbbbb"
# Output: 1
# Explanation: The answer is "b", with the length of 1.
# 
# 
# Example 3:
# 
# 
# Input: s = "pwwkew"
# Output: 3
# Explanation: The answer is "wke", with the length of 3.
# Notice that the answer must be a substring, "pwke" is a subsequence and not a
# substring.
# 
# 
# 
# Constraints:
# 
# 
# 0 <= s.length <= 5 * 10^4
# s consists of English letters, digits, symbols and spaces.
# 
# 
#

# @lc code=start
class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
        char_set = set()  # 用于存储当前窗口中的字符
        left = 0  # 左指针，表示窗口的起始位置
        max_length = 0  # 记录最长子串的长度

        for right in range(len(s)):  # 右指针，表示窗口的结束位置
            while s[right] in char_set:  # 如果右指针指向的字符在窗口中已存在
                char_set.remove(s[left])  # 移除左指针指向的字符
                left += 1  # 左指针右移，缩小窗口
            char_set.add(s[right])  # 将右指针指向的字符加入窗口
            max_length = max(max_length, right - left + 1)  # 更新最长子串长度

        return max_length

        # n = len(s)
        # if n == 0:
        #     return 0
        
        # char_index_map = {}
        # max_length = 0
        # left = 0

        # for right in range(n):
        #     if s[right] in char_index_map:
        #         left = max(left, char_index_map[s[right]] + 1)
        #     char_index_map[s[right]] = right
        #     max_length = max(max_length, right - left + 1)

        # return max_length

# @lc code=end

