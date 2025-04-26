#
# @lc app=leetcode id=18 lang=python3
#
# [18] 4Sum
#
# https://leetcode.com/problems/4sum/description/
#
# algorithms
# Medium (37.81%)
# Likes:    11968
# Dislikes: 1457
# Total Accepted:    1.2M
# Total Submissions: 3.3M
# Testcase Example:  '[1,0,-1,0,-2,2]\n0'
#
# Given an array nums of n integers, return an array of all the unique
# quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:
# 
# 
# 0 <= a, b, c, d < n
# a, b, c, and d are distinct.
# nums[a] + nums[b] + nums[c] + nums[d] == target
# 
# 
# You may return the answer in any order.
# 
# 
# Example 1:
# 
# 
# Input: nums = [1,0,-1,0,-2,2], target = 0
# Output: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
# 
# 
# Example 2:
# 
# 
# Input: nums = [2,2,2,2,2], target = 8
# Output: [[2,2,2,2]]
# 
# 
# 
# Constraints:
# 
# 
# 1 <= nums.length <= 200
# -10^9 <= nums[i] <= 10^9
# -10^9 <= target <= 10^9
# 
# 
#

# @lc code=start
class Solution:
    def fourSum(self, nums: List[int], target: int) -> List[List[int]]:
        # 排序数组
        nums.sort()
        result = []

        # 遍历数组，固定两个数
        for i in range(len(nums)):
            # 跳过重复的第一个数
            if i > 0 and nums[i] == nums[i - 1]:
                continue
            for j in range(i + 1, len(nums)):
                # 跳过重复的第二个数
                if j > i + 1 and nums[j] == nums[j - 1]:
                    continue

                # 双指针查找剩余的两个数
                left, right = j + 1, len(nums) - 1
                while left < right:
                    total = nums[i] + nums[j] + nums[left] + nums[right]
                    if total == target:
                        result.append([nums[i], nums[j], nums[left], nums[right]])
                        # 跳过重复的左指针数字
                        while left < right and nums[left] == nums[left + 1]:
                            left += 1
                        # 跳过重复的右指针数字
                        while left < right and nums[right] == nums[right - 1]:
                            right -= 1
                        # 移动指针
                        left += 1
                        right -= 1
                    elif total < target:
                        left += 1
                    else:
                        right -= 1

        return result
    
# 适用场景
# 2Sum：固定 0 个数，直接使用双指针。
# 3Sum：固定 1 个数，双指针查找剩余两个数。
# 4Sum：固定 2 个数，双指针查找剩余两个数。
# 扩展到 kSum：固定 k-2 个数，双指针查找剩余两个数。

# 关键点
# 排序：确保双指针的移动方向正确。
# 去重：跳过重复的固定数和指针数字，避免重复结果。
# 双指针调整：根据总和与目标值的关系，移动指针以缩小搜索范围。

# 目的：通过循环固定一个或多个数，减少问题规模。
# 操作：
# 使用 for 循环固定一个数（如 nums[i]）。
# 如果需要固定多个数，使用嵌套循环（如 nums[j]）。
# 跳过重复的固定数，避免重复结果。

# left, right = 固定数的右侧起点, 数组末尾

# 目的：根据总和与目标值的关系，调整指针或记录结果。
# 操作：
# 如果总和等于目标值，记录结果，并跳过重复的数字：
# 如果总和小于目标值，移动左指针以增加总和：
# 如果总和大于目标值，移动右指针以减少总和：

# @lc code=end

