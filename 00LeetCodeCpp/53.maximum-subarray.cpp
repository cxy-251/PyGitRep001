/*
 * @lc app=leetcode id=53 lang=cpp
 *
 * [53] Maximum Subarray
 */

// @lc code=start
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int max_so_far = INT_MIN;
        int current_sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            current_sum += nums[i];
            if (current_sum > max_so_far) {
                max_so_far = current_sum;
            }
            if (current_sum < 0) {
                current_sum = 0;
            }
        }
        return max_so_far;
    }
};
// @lc code=end

