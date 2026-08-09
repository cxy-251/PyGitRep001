/*
 * @lc app=leetcode id=51 lang=cpp
 *
 * [51] N-Queens
 */

// @lc code=start
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        vector<int> col(n, -1);
        backtrack(n, 0, col, res);
        return res;
    }
private:
    void backtrack(int n, int row, vector<int>& col, vector<vector<string>>& res) {
        if (row == n) {
            res.push_back(buildBoard(col, n));
            return;
        }

        for (int c = 0; c < n; ++c) {
            if (!isValid(col, row, c)) continue;
            col[row] = c;
            backtrack(n, row + 1, col, res);
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

    vector<string> buildBoard(const vector<int>& col, int n) const {
        vector<string> board(n, string(n, '.'));
        for (int r = 0; r < n; ++r) {
            board[r][col[r]] = 'Q';
        }
        return board;
    }
};
// @lc code=end

