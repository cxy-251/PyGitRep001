/*
 * @lc app=leetcode id=79 lang=cpp
 *
 * [79] Word Search
 */

// @lc code=start
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty()) return false;
        int rows = board.size(), cols = board[0].size();
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (dfs(board, r, c, word, 0)) return true;
            }
        }
        return false;
    }
private:
    bool dfs(vector<vector<char>>& board, int r, int c, const string& word, int idx) {
        if (idx == static_cast<int>(word.size())) return true;
        if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size()) return false;
        if (board[r][c] != word[idx]) return false;
        char tmp = board[r][c];
        board[r][c] = '\0';
        bool found = dfs(board, r + 1, c, word, idx + 1) ||
                     dfs(board, r - 1, c, word, idx + 1) ||
                     dfs(board, r, c + 1, word, idx + 1) ||
                     dfs(board, r, c - 1, word, idx + 1);
        board[r][c] = tmp;
        return found;
    }
};
// @lc code=end

