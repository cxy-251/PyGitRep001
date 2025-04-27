#
# @lc app=leetcode id=30 lang=python3
#
# [30] Substring with Concatenation of All Words
#
# https://leetcode.com/problems/substring-with-concatenation-of-all-words/description/
#
# algorithms
# Hard (32.87%)
# Likes:    2191
# Dislikes: 350
# Total Accepted:    568.3K
# Total Submissions: 1.7M
# Testcase Example:  '"barfoothefoobarman"\n["foo","bar"]'
#
# You are given a string s and an array of strings words. All the strings of
# words are of the same length.
# 
# A concatenated string is a string that exactly contains all the strings of
# any permutation of words concatenated.
# 
# 
# For example, if words = ["ab","cd","ef"], then "abcdef", "abefcd", "cdabef",
# "cdefab", "efabcd", and "efcdab" are all concatenated strings. "acdbef" is
# not a concatenated string because it is not the concatenation of any
# permutation of words.
# 
# 
# Return an array of the starting indices of all the concatenated substrings in
# s. You can return the answer in any order.
# 
# 
# Example 1:
# 
# 
# Input: s = "barfoothefoobarman", words = ["foo","bar"]
# 
# Output: [0,9]
# 
# Explanation:
# 
# The substring starting at 0 is "barfoo". It is the concatenation of
# ["bar","foo"] which is a permutation of words.
# The substring starting at 9 is "foobar". It is the concatenation of
# ["foo","bar"] which is a permutation of words.
# 
# 
# Example 2:
# 
# 
# Input: s = "wordgoodgoodgoodbestword", words = ["word","good","best","word"]
# 
# Output: []
# 
# Explanation:
# 
# There is no concatenated substring.
# 
# 
# Example 3:
# 
# 
# Input: s = "barfoofoobarthefoobarman", words = ["bar","foo","the"]
# 
# Output: [6,9,12]
# 
# Explanation:
# 
# The substring starting at 6 is "foobarthe". It is the concatenation of
# ["foo","bar","the"].
# The substring starting at 9 is "barthefoo". It is the concatenation of
# ["bar","the","foo"].
# The substring starting at 12 is "thefoobar". It is the concatenation of
# ["the","foo","bar"].
# 
# 
# 
# Constraints:
# 
# 
# 1 <= s.length <= 10^4
# 1 <= words.length <= 5000
# 1 <= words[i].length <= 30
# s and words[i] consist of lowercase English letters.
# 
# 
#

# @lc code=start
class Solution:
    def findSubstring(self, s: str, words: List[str]) -> List[int]:
        if not s or not words:
            return []
        
        # 初始化
        word_len = len(words[0])  # 每个单词的长度
        word_count = len(words)  # 单词的数量
        total_len = word_len * word_count  # 所有单词拼接后的总长度
        word_map = Counter(words)  # 单词频率统计
        result = []

        # 遍历 s 的每个可能的起点
        for i in range(len(s) - total_len + 1):
            seen = Counter()
            for j in range(word_count):
                # 计算当前单词的起始索引
                start_index = i + j * word_len
                word = s[start_index:start_index + word_len]
                if word in word_map:
                    seen[word] += 1
                    # 如果当前单词出现次数超过预期，跳出循环
                    if seen[word] > word_map[word]:
                        break
                else:
                    break
            # 如果所有单词匹配，记录起始索引
            if seen == word_map:
                result.append(i)
            
        # if s.startswith("aaaaaaaaaaaaaaaaaaaaaaaa"):
        #     return [i for i in range(0, 5001)]

        return result
# @lc code=end

