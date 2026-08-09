/*
 * @lc app=leetcode id=22 lang=cpp
 *
 * [22] Generate Parentheses
 */

// @lc code=start
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string current = "";
        backtrack(res, current, 0, 0, n);
        return res;
    }

private:
    /**
     * left: 当前已使用的左括号数量
     * right: 当前已使用的右括号数量
     * n: 括号对数
     */
    void backtrack(vector<string>& res, string& current, int left, int right, int n) {
        if (current.size() == 2 * n) {
            res.push_back(current);
            return;
        }
        if (left < n) {
            current.push_back('(');
            backtrack(res, current, left + 1, right, n);
            current.pop_back();
        }
        if (right < left) {
            current.push_back(')');
            backtrack(res, current, left, right + 1, n);
            current.pop_back();
        }
    }
};
// @lc code=end

