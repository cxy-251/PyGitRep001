#
# @lc app=leetcode id=228 lang=python3
#
# [228] Summary Ranges
#
# https://leetcode.com/problems/summary-ranges/description/
#
# algorithms
# Easy (52.76%)
# Likes:    4261
# Dislikes: 2311
# Total Accepted:    773.7K
# Total Submissions: 1.5M
# Testcase Example:  '[0,1,2,4,5,7]'
#
# You are given a sorted unique integer array nums.
# 
# A range [a,b] is the set of all integers from a to b (inclusive).
# 
# Return the smallest sorted list of ranges that cover all the numbers in the
# array exactly. That is, each element of nums is covered by exactly one of the
# ranges, and there is no integer x such that x is in one of the ranges but not
# in nums.
# 
# Each range [a,b] in the list should be output as:
# 
# 
# "a->b" if a != b
# "a" if a == b
# 
# 
# 
# Example 1:
# 
# 
# Input: nums = [0,1,2,4,5,7]
# Output: ["0->2","4->5","7"]
# Explanation: The ranges are:
# [0,2] --> "0->2"
# [4,5] --> "4->5"
# [7,7] --> "7"
# 
# 
# Example 2:
# 
# 
# Input: nums = [0,2,3,4,6,8,9]
# Output: ["0","2->4","6","8->9"]
# Explanation: The ranges are:
# [0,0] --> "0"
# [2,4] --> "2->4"
# [6,6] --> "6"
# [8,9] --> "8->9"
# 
# 
# 
# Constraints:
# 
# 
# 0 <= nums.length <= 20
# -2^31 <= nums[i] <= 2^31 - 1
# All the values of nums are unique.
# nums is sorted in ascending order.
# 
# 
#

# @lc code=start
class Solution:
    def summaryRanges(self, nums: List[int]) -> List[str]:
        if not nums:
            return []

        result = []
        start = nums[0]

        for i in range(1, len(nums)):
            # 如果当前数字不连续
            if nums[i] != nums[i - 1] + 1:
                # 添加当前范围到结果
                if start == nums[i - 1]:
                    result.append(str(start))
                else:
                    result.append(f"{start}->{nums[i - 1]}")
                # 更新起始点
                start = nums[i]

        # 添加最后一个范围
        if start == nums[-1]:
            result.append(str(start))
        else:
            result.append(f"{start}->{nums[-1]}")

        return result
        
# @lc code=end

