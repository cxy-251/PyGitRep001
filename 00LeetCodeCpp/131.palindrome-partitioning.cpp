/*
 * @lc app=leetcode id=131 lang=cpp
 *
 * [131] Palindrome Partitioning
 */

// @lc code=start
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> currentPath;
        backtrack(s, 0, currentPath, result);
        return result;
    }
private:
    void backtrack(const string& s, int start, vector<string>& currentPath, vector<vector<string>>& result) {
        if (start == s.size()) {
            result.push_back(currentPath);
            return;
        }
        for (int i = start; i < s.size(); ++i) {
            if (isPalindrome(s, start, i)) {
                string sub = s.substr(start, i - start + 1);
                currentPath.push_back(sub);
                backtrack(s, i + 1, currentPath, result);
                currentPath.pop_back();
            }
        }
    }

    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
};
// @lc code=end

