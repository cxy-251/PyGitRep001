#
# @lc app=leetcode id=870 lang=python3
#
# [870] Advantage Shuffle
#
# https://leetcode.com/problems/advantage-shuffle/description/
#
# algorithms
# Medium (53.20%)
# Likes:    1650
# Dislikes: 100
# Total Accepted:    74.3K
# Total Submissions: 139.3K
# Testcase Example:  '[2,7,11,15]\n[1,10,4,11]'
#
# You are given two integer arrays nums1 and nums2 both of the same length. The
# advantage of nums1 with respect to nums2 is the number of indices i for which
# nums1[i] > nums2[i].
# 
# Return any permutation of nums1 that maximizes its advantage with respect to
# nums2.
# 
# 
# Example 1:
# Input: nums1 = [2,7,11,15], nums2 = [1,10,4,11]
# Output: [2,11,7,15]
# Example 2:
# Input: nums1 = [12,24,8,32], nums2 = [13,25,32,11]
# Output: [24,32,8,12]
# 
# 
# Constraints:
# 
# 
# 1 <= nums1.length <= 10^5
# nums2.length == nums1.length
# 0 <= nums1[i], nums2[i] <= 10^9
# 
# 
#

# @lc code=start
class Solution:
    def advantageCount(self, nums1: List[int], nums2: List[int]) -> List[int]:
        # 对nums1排序
        nums1_sorted = sorted(nums1)
        # 对nums2按值降序排序，记录原下标
        nums2_sorted = sorted([(num, i) for i, num in enumerate(nums2)], reverse=True)
        n = len(nums1)
        res = [0] * n
        left, right = 0, n - 1

        for num2, idx in nums2_sorted:
            # 如果nums1最大的能赢nums2当前最大的，就用它
            if nums1_sorted[right] > num2:
                res[idx] = nums1_sorted[right]
                right -= 1
            else:
                # 否则用最小的“垫背”
                res[idx] = nums1_sorted[left]
                left += 1
        return res

# @lc code=end

