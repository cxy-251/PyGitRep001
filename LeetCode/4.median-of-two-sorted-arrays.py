#
# @lc app=leetcode id=4 lang=python3
#
# [4] Median of Two Sorted Arrays
#
# https://leetcode.com/problems/median-of-two-sorted-arrays/description/
#
# algorithms
# Hard (43.24%)
# Likes:    29819
# Dislikes: 3354
# Total Accepted:    3.3M
# Total Submissions: 7.6M
# Testcase Example:  '[1,3]\n[2]'
#
# Given two sorted arrays nums1 and nums2 of size m and n respectively, return
# the median of the two sorted arrays.
# 
# The overall run time complexity should be O(log (m+n)).
# 
# 
# Example 1:
# 
# 
# Input: nums1 = [1,3], nums2 = [2]
# Output: 2.00000
# Explanation: merged array = [1,2,3] and median is 2.
# 
# 
# Example 2:
# 
# 
# Input: nums1 = [1,2], nums2 = [3,4]
# Output: 2.50000
# Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.
# 
# 
# 
# Constraints:
# 
# 
# nums1.length == m
# nums2.length == n
# 0 <= m <= 1000
# 0 <= n <= 1000
# 1 <= m + n <= 2000
# -10^6 <= nums1[i], nums2[i] <= 10^6
# 
# 
#

# @lc code=start
class Solution:
    def findMedianSortedArrays(self, nums1: List[int], nums2: List[int]) -> float:
        # nums = nums1 + nums2
        # nums.sort()
        # n = len(nums)
        # if n % 2 == 0:
        #     return (nums[n // 2 - 1] + nums[n // 2]) / 2
        # else:
        #     return nums[n // 2]


        # len1, len2 = len(nums1), len(nums2)
        # total_len = len1 + len2
        # mid = total_len // 2

        # i, j = 0, 0  # 分别表示 nums1 和 nums2 的指针
        # med1, med2 = 0, 0

        # for _ in range(mid + 1):
        #     med1 = med2
        #     if i < len1 and (j >= len2 or nums1[i] < nums2[j]):
        #         med2 = nums1[i]
        #         i += 1
        #     else:
        #         med2 = nums2[j]
        #         j += 1

        # if total_len % 2 == 0:
        #     return (med1 + med2) / 2
        # else:
        #     return med2
        
        
        def find_kth_element(k):
            index1, index2 = 0, 0  # 分别表示 nums1 和 nums2 的起始索引
            while True:
                # 边界情况
                if index1 == len(nums1):  # nums1 已经耗尽
                    return nums2[index2 + k - 1]
                if index2 == len(nums2):  # nums2 已经耗尽
                    return nums1[index1 + k - 1]
                if k == 1:  # 找到第 1 小的元素
                    return min(nums1[index1], nums2[index2])

                # 正常情况：比较两个数组的第 k//2 个元素
                new_index1 = min(index1 + k // 2 - 1, len(nums1) - 1)
                new_index2 = min(index2 + k // 2 - 1, len(nums2) - 1)
                pivot1, pivot2 = nums1[new_index1], nums2[new_index2]

                if pivot1 <= pivot2:
                    k -= (new_index1 - index1 + 1)
                    index1 = new_index1 + 1
                else:
                    k -= (new_index2 - index2 + 1)
                    index2 = new_index2 + 1

        total_len = len(nums1) + len(nums2)
        if total_len % 2 == 1:  # 奇数长度
            return find_kth_element(total_len // 2 + 1)
        else:  # 偶数长度
            return (find_kth_element(total_len // 2) + find_kth_element(total_len // 2 + 1)) / 2
        
# @lc code=end

