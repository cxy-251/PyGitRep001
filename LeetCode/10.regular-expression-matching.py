#
# @lc app=leetcode id=10 lang=python3
#
# [10] Regular Expression Matching
#
# https://leetcode.com/problems/regular-expression-matching/description/
#
# algorithms
# Hard (29.07%)
# Likes:    12651
# Dislikes: 2277
# Total Accepted:    1.1M
# Total Submissions: 3.9M
# Testcase Example:  '"aa"\n"a"'
#
# Given an input string s and a pattern p, implement regular expression
# matching with support for '.' and '*' where:
# 
# 
# '.' Matches any single character.​​​​
# '*' Matches zero or more of the preceding element.
# 
# 
# The matching should cover the entire input string (not partial).
# 
# 
# Example 1:
# 
# 
# Input: s = "aa", p = "a"
# Output: false
# Explanation: "a" does not match the entire string "aa".
# 
# 
# Example 2:
# 
# 
# Input: s = "aa", p = "a*"
# Output: true
# Explanation: '*' means zero or more of the preceding element, 'a'. Therefore,
# by repeating 'a' once, it becomes "aa".
# 
# 
# Example 3:
# 
# 
# Input: s = "ab", p = ".*"
# Output: true
# Explanation: ".*" means "zero or more (*) of any character (.)".
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 20
# 1 <= p.length <= 20
# s contains only lowercase English letters.
# p contains only lowercase English letters, '.', and '*'.
# It is guaranteed for each appearance of the character '*', there will be a
# previous valid character to match.
# 
# 
#

# @lc code=start
class Solution:
    def isMatch(self, s: str, p: str) -> bool:
        # 获取字符串和模式的长度
        m, n = len(s), len(p)

        # 创建一个 (m+1) x (n+1) 的 DP 表，dp[i][j] 表示 s[:i] 和 p[:j] 是否匹配
        dp = [[False] * (n + 1) for _ in range(m + 1)]

        # 空字符串和空模式匹配
        dp[0][0] = True

        # 处理模式 p 的前缀可以匹配空字符串的情况（例如 "a*" 或 "a*b*"）
        for j in range(2, n + 1):
            if p[j - 1] == '*':
                dp[0][j] = dp[0][j - 2]

        # 填充 DP 表
        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if p[j - 1] == s[i - 1] or p[j - 1] == '.':
                    # 当前字符匹配，继承 dp[i-1][j-1]
                    dp[i][j] = dp[i - 1][j - 1]
                elif p[j - 1] == '*':
                    # '*' 匹配零个或多个前一个字符
                    dp[i][j] = dp[i][j - 2] or (dp[i - 1][j] and (s[i - 1] == p[j - 2] or p[j - 2] == '.'))

        # 返回最终结果
        return dp[m][n]
# s = "aab"
# p = "c*a*b"
# dp[i][j]	p[0]	p[1]	p[2]	p[3]	p[4]	p[5]
# s[0]	    T	    F     	T    	F    	T    	F
# s[1]	    F	    F     	F    	T    	T    	F
# s[2]	    F	    F     	F    	T    	T    	F
# s[3]	    F	    F     	F    	F    	F    	T
# @lc code=end

