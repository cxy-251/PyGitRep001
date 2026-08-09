/*
 * @lc app=leetcode id=164 lang=cpp
 *
 * [164] Maximum Gap
 */

// @lc code=start
class Solution {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        int minVal = nums[0], maxVal = nums[0];
        for (int x : nums) {
            minVal = min(minVal, x);
            maxVal = max(maxVal, x);
        }
        if (minVal == maxVal) return 0;
        int bucketSize = max(1, (maxVal - minVal) / (n - 1));
        int bucketCount = (maxVal - minVal) / bucketSize + 1;
        vector<int> bucketMin(bucketCount, INT_MAX);
        vector<int> bucketMax(bucketCount, INT_MIN);
        for (int x : nums) {
            int idx = (x - minVal) / bucketSize;
            bucketMin[idx] = min(bucketMin[idx], x);
            bucketMax[idx] = max(bucketMax[idx], x);
        }
        int maxGap = 0;
        int prevMax = minVal;
        for (int i = 0; i < bucketCount; ++i) {
            if (bucketMin[i] == INT_MAX) continue;
            maxGap = max(maxGap, bucketMin[i] - prevMax);
            prevMax = bucketMax[i];
        }
        return maxGap;
    }
};
// @lc code=end

