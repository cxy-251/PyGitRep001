/*
 * @lc app=leetcode id=289 lang=cpp
 *
 * [289] Game of Life
 */

// @lc code=start
class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int rows = board.size();
        int cols = board[0].size();

        // 8 个方向的偏移量，用于检查周围邻居
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // 第一遍遍历：计算状态并编码
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int liveNeighbors = 0;

                // 检查 8 个方向的邻居
                for (int k = 0; k < 8; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];

                    // 边界检查，且判断原状态是否为活 (1 或 2)
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && 
                        (board[ni][nj] == 1 || board[ni][nj] == 2)) {
                        liveNeighbors++;
                    }
                }

                // 根据规则更新状态 (编码)
                if (board[i][j] == 1) {
                    if (liveNeighbors < 2 || liveNeighbors > 3) {
                        board[i][j] = 2; // 活 -> 死
                    }
                } else if (board[i][j] == 0) {
                    if (liveNeighbors == 3) {
                        board[i][j] = 3; // 死 -> 活
                    }
                }
            }
        }

        // 第二遍遍历：将编码还原为最终状态 (0 或 1)
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (board[i][j] == 1 || board[i][j] == 3) {
                    board[i][j] = 1;
                } else {
                    board[i][j] = 0;
                }
            }
        }
    }
};
// @lc code=end

