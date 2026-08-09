/*
 * @lc app=leetcode id=152 lang=cpp
 *
 * [152] Maximum Product Subarray
 */

// @lc code=start
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if (nums.empty()) return 0;
        int max_so_far = nums[0];
        int min_so_far = nums[0];
        int global_max = nums[0];
        for (size_t i = 1; i < nums.size(); ++i) {
            int x = nums[i];
            if (x < 0) {
                swap(max_so_far, min_so_far);
            }
            max_so_far = max(x, max_so_far * x);
            min_so_far = min(x, min_so_far * x);
            global_max = max(global_max, max_so_far);
        }
        return global_max;
    }
};
// @lc code=end

