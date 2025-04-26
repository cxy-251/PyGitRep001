#
# @lc app=leetcode id=22 lang=python3
#
# [22] Generate Parentheses
#
# https://leetcode.com/problems/generate-parentheses/description/
#
# algorithms
# Medium (76.76%)
# Likes:    22043
# Dislikes: 1027
# Total Accepted:    2.3M
# Total Submissions: 3M
# Testcase Example:  '3'
#
# Given n pairs of parentheses, write a function to generate all combinations
# of well-formed parentheses.
# 
# 
# Example 1:
# Input: n = 3
# Output: ["((()))","(()())","(())()","()(())","()()()"]
# Example 2:
# Input: n = 1
# Output: ["()"]
# 
# 
# Constraints:
# 
# 
# 1 <= n <= 8
# 
# 
#

# @lc code=start
class Solution:
    def generateParenthesis(self, n: int) -> List[str]:
        result = []

        # 定义回溯函数
        def backtrack(current: str, open_count: int, close_count: int):
            # 如果当前字符串长度等于 2 * n，加入结果
            if len(current) == 2 * n:
                result.append(current)
                return

            # 如果可以添加左括号
            if open_count < n:
                backtrack(current + "(", open_count + 1, close_count)

            # 如果可以添加右括号
            if close_count < open_count:
                backtrack(current + ")", open_count, close_count + 1)

        # 从空字符串开始回溯
        backtrack("", 0, 0)
        return result
        
# @lc code=end

