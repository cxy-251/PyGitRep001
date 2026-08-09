/*
 * @lc app=leetcode id=149 lang=cpp
 *
 * [149] Max Points on a Line
 */

// @lc code=start
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;
        int globalMax = 0;
        for (int i = 0; i < n; ++i) {
            unordered_map<string, int> slopeMap;
            int duplicate = 1;
            int currentMax = 0;
            for (int j = i + 1; j < n; ++j) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];
                if (dx == 0 && dy == 0) {
                    duplicate++;
                    continue;
                }
                int common = gcd(dx, dy);
                dx /= common;
                dy /= common;
                if (dx < 0) {
                    dx = -dx;
                    dy = -dy;
                } else if (dx == 0) {
                    dy = abs(dy) ? 1 : 0;
                }
                string slope = to_string(dx) + "/" + to_string(dy);
                slopeMap[slope]++;
                currentMax = max(currentMax, slopeMap[slope]);
            }
            globalMax = max(globalMax, currentMax + duplicate);
        }
        return globalMax;
    }
};
// @lc code=end

