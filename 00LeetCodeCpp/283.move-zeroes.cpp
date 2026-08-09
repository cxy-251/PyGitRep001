/*
 * @lc app=leetcode id=283 lang=cpp
 *
 * [283] Move Zeroes
 */

// @lc code=start
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0; // 指向下一个非零元素应该存放的位置

        for (int fast = 0; fast < nums.size(); ++fast) {
            // 如果快指针找到了非零元素
            if (nums[fast] != 0) {
                // 将其与慢指针位置的元素交换
                // 如果 slow == fast，相当于自己跟自己交换，不影响结果
                swap(nums[slow], nums[fast]);
                // 慢指针后移，为下一个非零元素腾出位置
                slow++;
            }
        }
    }
};
// @lc code=end

