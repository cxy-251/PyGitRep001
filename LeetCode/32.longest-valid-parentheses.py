#
# @lc app=leetcode id=32 lang=python3
#
# [32] Longest Valid Parentheses
#
# https://leetcode.com/problems/longest-valid-parentheses/description/
#
# algorithms
# Hard (35.91%)
# Likes:    12775
# Dislikes: 431
# Total Accepted:    890.2K
# Total Submissions: 2.5M
# Testcase Example:  '"(()"'
#
# Given a string containing just the characters '(' and ')', return the length
# of the longest valid (well-formed) parentheses substring.
# 
# 
# Example 1:
# 
# 
# Input: s = "(()"
# Output: 2
# Explanation: The longest valid parentheses substring is "()".
# 
# 
# Example 2:
# 
# 
# Input: s = ")()())"
# Output: 4
# Explanation: The longest valid parentheses substring is "()()".
# 
# 
# Example 3:
# 
# 
# Input: s = ""
# Output: 0
# 
# 
# 
# Constraints:
# 
# 
# 0 <= s.length <= 3 * 10^4
# s[i] is '(', or ')'.
# 
# 
#

# @lc code=start
class Solution:
    def longestValidParentheses(self, s: str) -> int:
        # 初始化栈和最大长度
        stack = [-1]  # 栈中存储索引，初始值为 -1
        max_length = 0

        # 遍历字符串
        for i, char in enumerate(s):
            if char == '(':
                # 遇到左括号，压入栈中
                stack.append(i)
            else:
                # 遇到右括号，弹出栈顶
                stack.pop()
                if not stack:
                    # 如果栈为空，将当前索引压入栈
                    stack.append(i)
                else:
                    # 计算有效括号的长度
                    max_length = max(max_length, i - stack[-1])

        return max_length
# @lc code=end

