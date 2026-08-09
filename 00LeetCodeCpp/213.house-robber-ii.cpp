/*
 * @lc app=leetcode id=213 lang=cpp
 *
 * [213] House Robber II
 */

// @lc code=start
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        int res1 = robLinear(nums, 0, n - 2);
        int res2 = robLinear(nums, 1, n - 1);
        return max(res1, res2);
    }
private:
    int robLinear(const vector<int>& nums, int start, int end) {
        int prev2 = 0;
        int prev1 = 0;
        for (int i = start; i <= end; ++i) {
            int temp = prev1;
            prev1 = max(prev1, prev2 + nums[i]);
            prev2 = temp;
        }
        return prev1;
    }
};
// @lc code=end

