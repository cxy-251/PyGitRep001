/*
 * @lc app=leetcode id=140 lang=cpp
 *
 * [140] Word Break II
 */

// @lc code=start
class Solution {
public:
    unordered_map<int, vector<string>> memo;
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        return solve(s, 0, dict);
    }
private:
    vector<string> solve(const string& s, int start, const unordered_set<string>& dict) {
        if (memo.count(start)) {
            return memo[start];
        }
        vector<string> results;
        int n = s.size();
        if (start == n) {
            results.push_back("");
            return results;
        }
        for (int i = start + 1; i <= n; ++i) {
            string word = s.substr(start, i - start);
            if (dict.count(word)) {
                vector<string> nextWays = solve(s, i, dict);
                for (const string& next : nextWays) {
                    string space = next.empty() ? "" : " ";
                    results.push_back(word + space + next);
                }
            }
        }
        return memo[start] = results;
    }
};
// @lc code=end

