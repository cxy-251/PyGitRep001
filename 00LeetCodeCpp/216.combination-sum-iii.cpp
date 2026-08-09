/*
 * @lc app=leetcode id=216 lang=cpp
 *
 * [216] Combination Sum III
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        result.clear();
        path.clear();
        backtrack(1, n, k);
        return result;
    }
private:
    vector<vector<int>> result;
    vector<int> path;
    void backtrack(int start, int target, int k) {
        if (path.size() == k) {
            if (target == 0) {
                result.push_back(path);
            }
            return;
        }
        for (int i = start; i <= 9; ++i) {
            if (i > target) break;
            if ((9 - i + 1) < (k - path.size())) break;
            path.push_back(i);
            backtrack(i + 1, target - i, k);
            path.pop_back();
        }
    }
};
// @lc code=end

