/*
 * @lc app=leetcode id=37 lang=cpp
 *
 * [37] Sudoku Solver
 */

// @lc code=start
class Solution {
    // 三个记事本，记录数字是否被占用
    bool row[9][10] = {false};
    bool col[9][10] = {false};
    bool box[9][10] = {false};
public:
    void solveSudoku(vector<vector<char>>& board) {
        // 1. 先扫描一遍，把初始数字登记在案
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '0';
                    row[i][num] = col[j][num] = box[(i/3)*3 + j/3][num] = true;
                }
            }
        }
        backtrack(board, 0, 0);
        // backtrack(board);
    }

    bool backtrack(vector<vector<char>>& board, int r, int c) {
        // 自动换行逻辑：走到列末尾就跳到下一行开头
        if (c == 9) return backtrack(board, r + 1, 0);
        if (r == 9) return true; // 全部填完
        if (board[r][c] != '.') return backtrack(board, r, c + 1);

        int b_idx = (r / 3) * 3 + (c / 3);
        for (int num = 1; num <= 9; num++) {
            // O(1) 检查：直接看记事本，不用去扫棋盘
            if (!row[r][num] && !col[c][num] && !box[b_idx][num]) {
                // 做选择
                board[r][c] = num + '0';
                row[r][num] = col[c][num] = box[b_idx][num] = true;

                if (backtrack(board, r, c + 1)) return true;

                // 回溯：擦掉选择，本子上也要改回来
                board[r][c] = '.';
                row[r][num] = col[c][num] = box[b_idx][num] = false;
            }
        }
        return false;
    }
// private:
//     bool backtrack(vector<vector<char>>& board) {
//         for (int i = 0; i < 9; i++) {
//             for (int j = 0; j < 9; j++) {
//                 if (board[i][j] != '.') continue; // 跳过已填好的

//                 for (char c = '1'; c <= '9'; c++) {
//                     if (isValid(board, i, j, c)) {
//                         board[i][j] = c; // 1. 做选择
//                         if (backtrack(board)) return true; // 2. 递归：如果成功了就一直返回 true
//                         board[i][j] = '.'; // 3. 回溯：失败了就撤销选择
//                     }
//                 }
//                 return false; // 1-9 都试过了不行，说明这条路死胡同
//             }
//         }
//         return true; // 所有格子都填完了
//     }

//     // 检查在 (row, col) 填入字符 c 是否合法
//     bool isValid(vector<vector<char>>& board, int r, int c, char val) {
//         for (int i = 0; i < 9; i++) {
//             if (board[r][i] == val) return false; // 查行
//             if (board[i][c] == val) return false; // 查列
//             if (board[(r/3)*3 + i/3][(c/3)*3 + i%3] == val) return false; // 查宫格
//         }
//         return true;
//     }
};
// @lc code=end

