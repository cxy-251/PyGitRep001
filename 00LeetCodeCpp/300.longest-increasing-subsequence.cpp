/*
 * @lc app=leetcode id=300 lang=cpp
 *
 * [300] Longest Increasing Subsequence
 */

// @lc code=start
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;

        // tails 数组用于存储当前所有递增子序列的最小末尾元素
        // tails[i] 是长度为 i+1 的子序列的最小结尾
        vector<int> tails;

        for (int x : nums) {
            // 使用二分查找找到 tails 中第一个 >= x 的元素位置
            // lower_bound 返回的是指向该元素的迭代器
            auto it = lower_bound(tails.begin(), tails.end(), x);

            if (it == tails.end()) {
                // 如果 x 比 tails 所有的元素都大，直接添加到末尾
                tails.push_back(x);
            } else {
                // 否则，用 x 替换掉第一个 >= x 的元素
                // 这样可以使得长度为 (dist+1) 的子序列末尾更小
                *it = x;
            }
        }

        // tails 的最终长度即为最长递增子序列的长度
        return tails.size();
    }
};
// @lc code=end

