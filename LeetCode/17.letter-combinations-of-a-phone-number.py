#
# @lc app=leetcode id=17 lang=python3
#
# [17] Letter Combinations of a Phone Number
#
# https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/
#
# algorithms
# Medium (63.38%)
# Likes:    19596
# Dislikes: 1061
# Total Accepted:    2.5M
# Total Submissions: 3.9M
# Testcase Example:  '"23"'
#
# Given a string containing digits from 2-9 inclusive, return all possible
# letter combinations that the number could represent. Return the answer in any
# order.
# 
# A mapping of digits to letters (just like on the telephone buttons) is given
# below. Note that 1 does not map to any letters.
# 
# 
# Example 1:
# 
# 
# Input: digits = "23"
# Output: ["ad","ae","af","bd","be","bf","cd","ce","cf"]
# 
# 
# Example 2:
# 
# 
# Input: digits = ""
# Output: []
# 
# 
# Example 3:
# 
# 
# Input: digits = "2"
# Output: ["a","b","c"]
# 
# 
# 
# Constraints:
# 
# 
# 0 <= digits.length <= 4
# digits[i] is a digit in the range ['2', '9'].
# 
# 
#

# @lc code=start
class Solution:
    def letterCombinations(self, digits: str) -> List[str]:
        if not digits:
            return []

        # 数字到字母的映射
        phone_map = {
            "2": "abc", "3": "def", "4": "ghi", "5": "jkl",
            "6": "mno", "7": "pqrs", "8": "tuv", "9": "wxyz"
        }

        result = []

        # 回溯函数
        def backtrack(index: int, path: str):
            # 如果路径长度等于输入数字长度，加入结果
            if index == len(digits):
                result.append(path)
                return

            # 获取当前数字对应的字母
            letters = phone_map[digits[index]]
            for letter in letters:
                # 递归调用，继续处理下一个数字
                backtrack(index + 1, path + letter)

        # 从第一个数字开始回溯
        backtrack(0, "")
        return result
        
# @lc code=end

