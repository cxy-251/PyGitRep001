/*
 * @lc app=leetcode id=90 lang=cpp
 *
 * [90] Subsets II
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> currentPath;
        sort(nums.begin(), nums.end());
        backtrack(nums, 0, currentPath, result);
        return result;
    }
private:
    void backtrack(vector<int>& nums, int startIndex, vector<int>& currentPath, vector<vector<int>>& result) {
        result.push_back(currentPath);
        for (int i = startIndex; i < nums.size(); ++i) {
            if (i > startIndex && nums[i] == nums[i - 1]) {
                continue;
            }
            currentPath.push_back(nums[i]);
            backtrack(nums, i + 1, currentPath, result);
            currentPath.pop_back();
        }
    }
};
// @lc code=end

