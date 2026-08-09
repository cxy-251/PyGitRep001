/*
 * @lc app=leetcode id=17 lang=cpp
 *
 * [17] Letter Combinations of a Phone Number
 */

// @lc code=start
class Solution {
private:
    // 数字到字母的映射数组，下标 0 和 1 留空
    const vector<string> mapping = {
        "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
    };
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> res;
        string current = "";
        backtrack(res, digits, current, 0);
        return res;
    }

    void backtrack(vector<string>& res, const string& digits, string& current, int index) {
        if (index == digits.size()) {
            res.push_back(current);
            return;
        }
        string letters = mapping[digits[index] - '0'];
        for (char c : letters) {
            current.push_back(c);
            backtrack(res, digits, current, index + 1);
            current.pop_back();
        }
    }
};
// @lc code=end

