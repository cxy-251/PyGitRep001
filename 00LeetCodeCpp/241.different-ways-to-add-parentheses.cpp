/*
 * @lc app=leetcode id=241 lang=cpp
 *
 * [241] Different Ways to Add Parentheses
 */

// @lc code=start
class Solution {
    unordered_map<string, vector<int>> memo;
public:
    vector<int> diffWaysToCompute(string expression) {
        if (memo.count(expression)) {
            return memo[expression];
        }
        vector<int> res;
        for (int i = 0; i < expression.size(); ++i) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                string leftStr = expression.substr(0, i);
                string rightStr = expression.substr(i + 1);
                vector<int> leftResults = diffWaysToCompute(leftStr);
                vector<int> rightResults = diffWaysToCompute(rightStr);
                for (int l : leftResults) {
                    for (int r : rightResults) {
                        if (c == '+') res.push_back(l + r);
                        else if (c == '-') res.push_back(l - r);
                        else if (c == '*') res.push_back(l * r);
                    }
                }
            }
        }
        if (res.empty()) {
            res.push_back(stoi(expression));
        }
        return memo[expression] = res;
    }
};
// @lc code=end

