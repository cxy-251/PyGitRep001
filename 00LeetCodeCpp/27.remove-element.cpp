// ...existing code...
/*
 * @lc app=leetcode id=27 lang=cpp
 *
 * [27] Remove Element
 */

// @lc code=start
#include <vector>
using namespace std;

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int write = 0;
        for (int read = 0; read < (int)nums.size(); ++read) {
            if (nums[read] != val) {
                nums[write++] = nums[read];
            }
        }
        return write;
    }
};
// @lc code=end
// ...existing code...