/*
 * @lc app=leetcode id=187 lang=cpp
 *
 * [187] Repeated DNA Sequences
 */

// @lc code=start
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.length() < 10) {
            return {};
        }
        unordered_map<string, int> counts;
        vector<string> result;
        for (int i = 0; i <= (int)s.length() - 10; ++i) {
            string sub = s.substr(i, 10);
            counts[sub]++;
            if (counts[sub] == 2) {
                result.push_back(sub);
            }
        }
        return result;
    }
};
// @lc code=end

