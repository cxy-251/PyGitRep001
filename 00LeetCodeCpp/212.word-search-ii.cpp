/*
 * @lc app=leetcode id=212 lang=cpp
 *
 * [212] Word Search II
 */

// @lc code=start
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    string word = "";
    ~TrieNode() {
        for (int i = 0; i < 26; ++i) {
            if (children[i]) delete children[i];
        }
    }
};
class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        if (board.empty() || board[0].empty()) return {};
        rows = board.size();
        cols = board[0].size();
        TrieNode* root = new TrieNode();
        for (const string& w : words) {
            TrieNode* curr = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!curr->children[idx]) curr->children[idx] = new TrieNode();
                curr = curr->children[idx];
            }
            curr->word = w;
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dfs(board, i, j, root);
            }
        }
        delete root;
        return result;
    }
private:
    int rows, cols;
    vector<string> result;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node) {
        char ch = board[r][c];
        int idx = ch - 'a';
        if (ch == '#' || !node->children[idx]) {
            return;
        }
        TrieNode* nextNode = node->children[idx];
        if (nextNode->word != "") {
            result.push_back(nextNode->word);
            nextNode->word = "";
        }
        board[r][c] = '#';
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                dfs(board, nr, nc, nextNode);
            }
        }
        board[r][c] = ch;
    }
};
// @lc code=end

