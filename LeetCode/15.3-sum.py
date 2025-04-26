#
# @lc app=leetcode id=15 lang=python3
#
# [15] 3Sum
#
# https://leetcode.com/problems/3sum/description/
#
# algorithms
# Medium (36.67%)
# Likes:    32700
# Dislikes: 3080
# Total Accepted:    4.6M
# Total Submissions: 12.5M
# Testcase Example:  '[-1,0,1,2,-1,-4]'
#
# Given an integer array nums, return all the triplets [nums[i], nums[j],
# nums[k]] such that i != j, i != k, and j != k, and nums[i] + nums[j] +
# nums[k] == 0.
# 
# Notice that the solution set must not contain duplicate triplets.
# 
# 
# Example 1:
# 
# 
# Input: nums = [-1,0,1,2,-1,-4]
# Output: [[-1,-1,2],[-1,0,1]]
# Explanation: 
# nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0.
# nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0.
# nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0.
# The distinct triplets are [-1,0,1] and [-1,-1,2].
# Notice that the order of the output and the order of the triplets does not
# matter.
# 
# 
# Example 2:
# 
# 
# Input: nums = [0,1,1]
# Output: []
# Explanation: The only possible triplet does not sum up to 0.
# 
# 
# Example 3:
# 
# 
# Input: nums = [0,0,0]
# Output: [[0,0,0]]
# Explanation: The only possible triplet sums up to 0.
# 
# 
# 
# Constraints:
# 
# 
# 3 <= nums.length <= 3000
# -10^5 <= nums[i] <= 10^5
# 
# 
#

# @lc code=start
class Solution:
    def threeSum(self, nums: List[int]) -> List[List[int]]:
        # 排序数组
        nums.sort()
        result = []

        # 遍历数组，固定一个数
        for i in range(len(nums)):
            # 跳过重复的数字
            if i > 0 and nums[i] == nums[i - 1]:
                continue

            # 双指针查找剩余的两个数
            left, right = i + 1, len(nums) - 1
            while left < right:
                total = nums[i] + nums[left] + nums[right]
                if total == 0:
                    result.append([nums[i], nums[left], nums[right]])
                    # 跳过重复的左指针数字
                    while left < right and nums[left] == nums[left + 1]:
                        left += 1
                    # 跳过重复的右指针数字
                    while left < right and nums[right] == nums[right - 1]:
                        right -= 1
                    # 移动指针
                    left += 1
                    right -= 1
                elif total < 0:
                    left += 1
                else:
                    right -= 1

        return result
        
# @lc code=end

