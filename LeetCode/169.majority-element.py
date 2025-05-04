#
# @lc app=leetcode id=169 lang=python3
#
# [169] Majority Element
#
# https://leetcode.com/problems/majority-element/description/
#
# algorithms
# Easy (65.57%)
# Likes:    20809
# Dislikes: 727
# Total Accepted:    4.2M
# Total Submissions: 6.4M
# Testcase Example:  '[3,2,3]'
#
# Given an array nums of size n, return the majority element.
# 
# The majority element is the element that appears more than ⌊n / 2⌋ times. You
# may assume that the majority element always exists in the array.
# 
# 
# Example 1:
# Input: nums = [3,2,3]
# Output: 3
# Example 2:
# Input: nums = [2,2,1,1,1,2,2]
# Output: 2
# 
# 
# Constraints:
# 
# 
# n == nums.length
# 1 <= n <= 5 * 10^4
# -10^9 <= nums[i] <= 10^9
# 
# 
# 
# Follow-up: Could you solve the problem in linear time and in O(1) space?
#

# @lc code=start
class Solution:
    def majorityElement(self, nums: List[int]) -> int:
        # 初始化候选元素和计数器
        candidate = None
        count = 0

        # 遍历数组
        for num in nums:
            if count == 0:
                # 更新候选元素
                candidate = num
            # 更新计数器
            count += 1 if num == candidate else -1

        return candidate
        
# @lc code=end

