/*
 * @lc app=leetcode id=126 lang=cpp
 *
 * [126] Word Ladder II
 */

// @lc code=start
class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        vector<vector<string>> res;
        if (dict.find(endWord) == dict.end()) {
            return res;
        }
        unordered_map<string, int> dist;
        unordered_map<string, vector<string>> adj;
        queue<string> q;
        q.push(beginWord);
        dist[beginWord] = 0;
        bool found = false;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curr = q.front();
                q.pop();
                if (found && curr == endWord) break;
                string next_word = curr;
                for (int j = 0; j < next_word.size(); ++j) {
                    char original_char = next_word[j];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        next_word[j] = c;
                        if (dict.count(next_word)) {
                            if (dist.find(next_word) == dist.end()) {
                                dist[next_word] = dist[curr] + 1;
                                adj[next_word].push_back(curr);
                                q.push(next_word);
                                if (next_word == endWord) found = true;
                            } else if (dist[next_word] == dist[curr] + 1) {
                                adj[next_word].push_back(curr);
                            }
                        }
                    }
                    next_word[j] = original_char;
                }
            }
            if (found) break;
        }
        if (dist.find(endWord) == dist.end()) {
            return res;
        }
        vector<string> path = {endWord};
        dfs(endWord, beginWord, adj, path, res);
        return res;
    }
private:
    void dfs(const string& curr, const string& beginWord,
             unordered_map<string, vector<string>>& adj,
             vector<string>& path, vector<vector<string>>& res) {
        if (curr == beginWord) {
            vector<string> completePath = path;
            reverse(completePath.begin(), completePath.end());
            res.push_back(completePath);
            return;
        }
        if (adj.count(curr)) {
            for (const string& prev : adj[curr]) {
                path.push_back(prev);
                dfs(prev, beginWord, adj, path, res);
                path.pop_back();
            }
        }
    }
};
// @lc code=end

