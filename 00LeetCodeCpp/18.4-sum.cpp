/*
 * @lc app=leetcode id=18 lang=cpp
 *
 * [18] 4Sum
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        return nSumTarget(nums, 4, 0, target);
    }

    vector<vector<int>> nSumTarget(vector<int>& nums, int n, int start, long target) {
        int sz = nums.size();
        vector<vector<int>> res;
        if (n < 2 || sz - start < n) return res;
        if (n == 2) {
            int left = start, right = sz - 1;
            while (left < right) {
                long sum = (long)nums[left] + nums[right];
                int val_left = nums[left], val_right = nums[right];
                if (sum < target) {
                    while (left < right && nums[left] == val_left) left++;
                } else if (sum > target) {
                    while (left < right && nums[right] == val_right) right--;
                } else {
                    res.push_back({val_left, val_right});
                    while (left < right && nums[left] == val_left) left++;
                    while (left < right && nums[right] == val_right) right--;
                }
            }
        } else {
            for (int i = start; i < sz; i++) {
                vector<vector<int>> sub = nSumTarget(nums, n - 1, i + 1, target - nums[i]);
                for (vector<int>& arr : sub) {
                    arr.push_back(nums[i]);
                    res.push_back(arr);
                }
                while (i < sz - 1 && nums[i] == nums[i + 1]) i++;
            }
        }
        return res;
    }
};
// @lc code=end

