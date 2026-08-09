/*
 * @lc app=leetcode id=278 lang=cpp
 *
 * [278] First Bad Version
 */

// @lc code=start
// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        int left = 1;
        int right = n;
        while (left < right) {
            // 重点 1：防止整数溢出
            // 不要写成 (left + right) / 2，因为 left + right 可能会超过 int 的最大值 (2^31 - 1)
            int mid = left + (right - left) / 2;
            if (isBadVersion(mid)) {
                // 如果 mid 是坏版本，说明第一个坏版本在 mid 或 mid 的左边
                // 所以我们将搜索范围缩小到 [left, mid]
                right = mid;
            } else {
                // 如果 mid 是好版本，说明第一个坏版本一定在 mid 的右边
                // 所以我们将搜索范围缩小到 [mid + 1, right]
                left = mid + 1;
            }
        }
        // 当 left == right 时，我们就找到了第一个坏版本
        return left;
    }
};
// @lc code=end

