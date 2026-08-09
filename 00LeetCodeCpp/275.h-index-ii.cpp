/*
 * @lc app=leetcode id=275 lang=cpp
 *
 * [275] H-Index II
 */

// @lc code=start
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        int left = 0, right = n - 1;
        int ans = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            // 从 mid 到 n-1 共有 (n - mid) 篇论文
            // 这些论文的引用次数都 >= citations[mid]
            int h_candidate = n - mid;

            if (citations[mid] >= h_candidate) {
                // 找到了一个符合条件的 h，记录下来
                ans = h_candidate;
                // 尝试寻找更大的 h，即尝试更小的 mid
                right = mid - 1;
            } else {
                // 当前引用次数太低，无法支持这么大的 h，需要向右移动
                left = mid + 1;
            }
        }
        return ans;
    }
};
// @lc code=end

