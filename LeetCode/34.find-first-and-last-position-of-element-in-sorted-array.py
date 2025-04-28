#
# @lc app=leetcode id=34 lang=python3
#
# [34] Find First and Last Position of Element in Sorted Array
#
# https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/description/
#
# algorithms
# Medium (46.39%)
# Likes:    21699
# Dislikes: 570
# Total Accepted:    2.6M
# Total Submissions: 5.6M
# Testcase Example:  '[5,7,7,8,8,10]\n8'
#
# Given an array of integers nums sorted in non-decreasing order, find the
# starting and ending position of a given target value.
# 
# If target is not found in the array, return [-1, -1].
# 
# You must write an algorithm with O(log n) runtime complexity.
# 
# 
# Example 1:
# Input: nums = [5,7,7,8,8,10], target = 8
# Output: [3,4]
# Example 2:
# Input: nums = [5,7,7,8,8,10], target = 6
# Output: [-1,-1]
# Example 3:
# Input: nums = [], target = 0
# Output: [-1,-1]
# 
# 
# Constraints:
# 
# 
# 0 <= nums.length <= 10^5
# -10^9 <= nums[i] <= 10^9
# nums is a non-decreasing array.
# -10^9 <= target <= 10^9
# 
# 
#

# @lc code=start
class Solution:
    def searchRange(self, nums: List[int], target: int) -> List[int]:
        def find_bound(is_first: bool) -> int:
            left, right = 0, len(nums) - 1
            while left <= right:
                mid = (left + right) // 2
                if nums[mid] == target:
                    if is_first:
                        # 如果是找第一个位置，继续向左搜索
                        if mid == 0 or nums[mid - 1] != target:
                            return mid
                        right = mid - 1
                    else:
                        # 如果是找最后一个位置，继续向右搜索
                        if mid == len(nums) - 1 or nums[mid + 1] != target:
                            return mid
                        left = mid + 1
                elif nums[mid] < target:
                    left = mid + 1
                else:
                    right = mid - 1
            return -1

        # 找第一个和最后一个位置
        first = find_bound(True)
        if first == -1:
            return [-1, -1]  # 如果第一个位置不存在，直接返回 [-1, -1]
        last = find_bound(False)
        return [first, last]
# @lc code=end

