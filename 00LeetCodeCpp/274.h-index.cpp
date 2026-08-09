/*
 * @lc app=leetcode id=274 lang=cpp
 *
 * [274] H-Index
 */

// @lc code=start
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        vector<int> counter(n + 1, 0);
        for (int c : citations) {
            if (c >= n) {
                counter[n]++;
            } else {
                counter[c]++;
            }
        }
        int totalPapers = 0;
        for (int h = n; h >= 0; --h) {
            totalPapers += counter[h];
            if (totalPapers >= h) {
                return h;
            }
        }
        return 0;
    }
};
// @lc code=end

