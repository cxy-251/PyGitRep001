#
# @lc app=leetcode id=16 lang=python3
#
# [16] 3Sum Closest
#
# https://leetcode.com/problems/3sum-closest/description/
#
# algorithms
# Medium (46.70%)
# Likes:    10929
# Dislikes: 589
# Total Accepted:    1.5M
# Total Submissions: 3.1M
# Testcase Example:  '[-1,2,1,-4]\n1'
#
# Given an integer array nums of length n and an integer target, find three
# integers in nums such that the sum is closest to target.
# 
# Return the sum of the three integers.
# 
# You may assume that each input would have exactly one solution.
# 
# 
# Example 1:
# 
# 
# Input: nums = [-1,2,1,-4], target = 1
# Output: 2
# Explanation: The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
# 
# 
# Example 2:
# 
# 
# Input: nums = [0,0,0], target = 1
# Output: 0
# Explanation: The sum that is closest to the target is 0. (0 + 0 + 0 = 0).
# 
# 
# 
# Constraints:
# 
# 
# 3 <= nums.length <= 500
# -1000 <= nums[i] <= 1000
# -10^4 <= target <= 10^4
# 
# 
#

# @lc code=start
class Solution:
    def threeSumClosest(self, nums: List[int], target: int) -> int:
        # 排序数组
        nums.sort()
        closest_sum = float('inf')  # 初始化为正无穷大

        # 遍历数组，固定一个数
        for i in range(len(nums)):
            # 双指针查找剩余的两个数
            left, right = i + 1, len(nums) - 1
            while left < right:
                current_sum = nums[i] + nums[left] + nums[right]

                # 如果当前和更接近目标值，则更新 closest_sum
                if abs(current_sum - target) < abs(closest_sum - target):
                    closest_sum = current_sum

                # 根据当前和与目标值的关系移动指针
                if current_sum < target:
                    left += 1
                elif current_sum > target:
                    right -= 1
                else:
                    # 如果 current_sum == target，直接返回结果
                    return current_sum

        return closest_sum
        
# @lc code=end

