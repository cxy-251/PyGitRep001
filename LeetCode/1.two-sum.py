#
# @lc app=leetcode id=1 lang=python3
#
# [1] Two Sum
#
# 给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的两个整数，并返回它们的索引。

# 题目要求：
# 每种输入只会对应一个答案。
# 你不能重复使用同一个元素。
# 返回的结果可以是任意顺序。
# @lc code=start
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        # 1 O(n^2)
        # for i in range(len(nums)):
        #     a = target - nums[i]
        #     if a in nums[i + 1:]:
        #         return [i, nums.index(a, i + 1)]

        # 2 O(n)
        num_map = {}
        for i, num in enumerate(nums):
            complement = target - num
            if complement in num_map:
                return [num_map[complement], i]
            num_map[num] = i

# @lc code=end

