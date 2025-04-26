#
# @lc app=leetcode id=20 lang=python3
#
# [20] Valid Parentheses
#
# https://leetcode.com/problems/valid-parentheses/description/
#
# algorithms
# Easy (42.03%)
# Likes:    25584
# Dislikes: 1872
# Total Accepted:    5.9M
# Total Submissions: 14.1M
# Testcase Example:  '"()"'
#
# Given a string s containing just the characters '(', ')', '{', '}', '[' and
# ']', determine if the input string is valid.
# 
# An input string is valid if:
# 
# 
# Open brackets must be closed by the same type of brackets.
# Open brackets must be closed in the correct order.
# Every close bracket has a corresponding open bracket of the same type.
# 
# 
# 
# Example 1:
# 
# 
# Input: s = "()"
# 
# Output: true
# 
# 
# Example 2:
# 
# 
# Input: s = "()[]{}"
# 
# Output: true
# 
# 
# Example 3:
# 
# 
# Input: s = "(]"
# 
# Output: false
# 
# 
# Example 4:
# 
# 
# Input: s = "([])"
# 
# Output: true
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 10^4
# s consists of parentheses only '()[]{}'.
# 
# 
#

# @lc code=start
class Solution:
    def isValid(self, s: str) -> bool:
        # 定义括号的映射关系
        bracket_map = {')': '(', '}': '{', ']': '['}
        stack = []

        # 遍历字符串中的每个字符
        for char in s:
            if char in bracket_map:
                # 如果是右括号，检查栈顶是否匹配
                top_element = stack.pop() if stack else '#'
                if bracket_map[char] != top_element:
                    return False
            else:
                # 如果是左括号，压入栈中
                stack.append(char)

        # 如果栈为空，则括号匹配有效
        return not stack
        
# @lc code=end

