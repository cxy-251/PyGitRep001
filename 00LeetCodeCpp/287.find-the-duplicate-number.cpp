/*
 * @lc app=leetcode id=287 lang=cpp
 *
 * [287] Find the Duplicate Number
 */

// @lc code=start
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        // 1. 第一阶段：寻找快慢指针的相遇点
        int slow = nums[0];
        int fast = nums[0];
        
        // 必须先走一步，否则 while 条件 slow != fast 一开始就不成立
        do {
            slow = nums[slow];            // 慢指针走一步
            fast = nums[nums[fast]];     // 快指针走两步
        } while (slow != fast);
        
        // 2. 第二阶段：寻找环的入口点
        slow = nums[0];                  // 慢指针回到起点
        while (slow != fast) {
            slow = nums[slow];           // 两人同时走一步
            fast = nums[fast];
        }
        
        return slow; // 相遇点即为重复数字
    }
};
// @lc code=end

