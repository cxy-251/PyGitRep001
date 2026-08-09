/*
 * @lc app=leetcode id=56 lang=cpp
 *
 * [56] Merge Intervals
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> merged;
        merged.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); ++i) {
            vector<int>& lastMerged = merged.back();
            int currentStart = intervals[i][0];
            int currentEnd = intervals[i][1];
            if (currentStart <= lastMerged[1]) {
                lastMerged[1] = max(lastMerged[1], currentEnd);
            } else {
                merged.push_back(intervals[i]);
            }
        }
        return merged;
    }
};
// @lc code=end

