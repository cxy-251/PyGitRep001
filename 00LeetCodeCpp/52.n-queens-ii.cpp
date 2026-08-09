/*
 * @lc app=leetcode id=52 lang=cpp
 *
 * [52] N-Queens II
 */

// @lc code=start
class Solution {
public:
    int totalNQueens(int n) {
        int count = 0;
        vector<int> col(n, -1);
        backtrack(n, 0, col, count);
        return count;
    }
private:
    void backtrack(int n, int row, vector<int>& col, int& count) {
        if (row == n) {
            count++;
            return;
        }
        for (int c = 0; c < n; ++c) {
            if (!isValid(col, row, c)) continue;
            col[row] = c;
            backtrack(n, row + 1, col, count);
            col[row] = -1;
        }
    }

    bool isValid(const vector<int>& col, int row, int c) const {
        for (int r = 0; r < row; ++r) {
            int cc = col[r];
            if (cc == c) return false;
            if (abs(cc - c) == abs(r - row)) return false;
        }
        return true;
    }
};
// @lc code=end

