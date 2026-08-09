/*
 * @lc app=leetcode id=4 lang=cpp
 *
 * [4] Median of Two Sorted Arrays
 */

// @lc code=start
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        int m = nums2.size();

        int left = (n + m + 1) / 2;  // 左中位数位置
        int right = (n + m + 2) / 2; // 右中位数位置
        
        // 技巧：如果是奇数，left == right，逻辑统一；如果是偶数，求两个位置的平均值
        return (getKth(nums1, 0, n - 1, nums2, 0, m - 1, left) + 
                getKth(nums1, 0, n - 1, nums2, 0, m - 1, right)) * 0.5;
    }
private:
    // 在两个数组的指定区间内，寻找第 k 小的数
    int getKth(vector<int>& nums1, int start1, int end1, vector<int>& nums2, int start2, int end2, int k) {
        int len1 = end1 - start1 + 1;
        int len2 = end2 - start2 + 1;

        // 始终让 nums1 成为较短的数组，方便处理越界情况
        if (len1 > len2) return getKth(nums2, start2, end2, nums1, start1, end1, k);
        
        // 边界情况 1：短数组已经排空了，直接去长数组找
        if (len1 == 0) return nums2[start2 + k - 1];
        
        // 边界情况 2：找第 1 小的数，直接比两个数组的头，谁小就是谁
        if (k == 1) return min(nums1[start1], nums2[start2]);

        // 核心逻辑：二分，各自取第 k/2 个数进行比较
        // 注意：为了防止短数组长度不足 k/2，取 min(len1, k/2)
        int i = start1 + min(len1, k / 2) - 1;
        int j = start2 + min(len2, k / 2) - 1;

        if (nums1[i] > nums2[j]) {
            // nums2 的前半部分（j 之前）绝对不可能是第 k 小，砍掉！
            // 更新 k：减去被砍掉的长度
            return getKth(nums1, start1, end1, nums2, j + 1, end2, k - (j - start2 + 1));
        } else {
            // nums1 的前半部分（i 之前）绝对不可能是第 k 小，砍掉！
            return getKth(nums1, i + 1, end1, nums2, start2, end2, k - (i - start1 + 1));
        }
    }
};
// @lc code=end

