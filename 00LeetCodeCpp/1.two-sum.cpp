/*
 * @lc app=leetcode id=1 lang=cpp
 *
 * [1] Two Sum
 */

// @lc code=start
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> num_to_index;
        for (int i = 0; i < nums.size(); ++i) {
            int tmp = target - nums[i];
            auto it = num_to_index.find(tmp);
            if (it != num_to_index.end()) {
                return {it->second, i};
            }
            num_to_index[nums[i]] = i;
        }
        return {};
    }
};
// @lc code=end