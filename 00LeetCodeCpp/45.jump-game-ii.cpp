/*
 * @lc app=leetcode id=45 lang=cpp
 *
 * [45] Jump Game II
 */

// @lc code=start
class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return 0;
        int jumps = 0;
        int cur_end = 0;
        int max_pos = 0;
        for (int i = 0; i < n - 1; ++i) {
            max_pos = max(max_pos, i + nums[i]);
            if (i == cur_end) {
                jumps++;
                cur_end = max_pos;
                if (cur_end >= n - 1) break;
            }
        }
        return jumps;
    }
};
// @lc code=end

