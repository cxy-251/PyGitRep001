/*
 * @lc app=leetcode id=55 lang=cpp
 *
 * [55] Jump Game
 */

// @lc code=start
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int farthest_reachable = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (i > farthest_reachable) {
                return false;
            }
            farthest_reachable = max(farthest_reachable, i + nums[i]);
            if (farthest_reachable >= nums.size() - 1) {
                return true;
            }
        }
        return true;
    }
};
// @lc code=end

