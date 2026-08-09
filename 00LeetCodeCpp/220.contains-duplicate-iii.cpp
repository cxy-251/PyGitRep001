/*
 * @lc app=leetcode id=220 lang=cpp
 *
 * [220] Contains Duplicate III
 */

// @lc code=start
class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        if (indexDiff <= 0 || valueDiff < 0) return false;
        set<long long> window;
        for (int i = 0; i < nums.size(); ++i) {
            long long x = nums[i];
            auto it = window.lower_bound(x - valueDiff);
            if (it != window.end() && *it <= x + valueDiff) {
                return true;
            }
            window.insert(x);
            if (window.size() > indexDiff) {
                window.erase((long long)nums[i - indexDiff]);
            }
        }
        return false;
    }
};
// @lc code=end

