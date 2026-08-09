/*
 * @lc app=leetcode id=78 lang=cpp
 *
 * [78] Subsets
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        result.push_back({});
        for (int num : nums) {
            int size = result.size();
            for (int i = 0; i < size; ++i) {
                vector<int> current = result[i];
                current.push_back(num);
                result.push_back(current);
            }
        }
        return result;
    }
};
// @lc code=end

