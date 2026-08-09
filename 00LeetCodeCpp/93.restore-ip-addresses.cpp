/*
 * @lc app=leetcode id=93 lang=cpp
 *
 * [93] Restore IP Addresses
 */

// @lc code=start
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        vector<string> path;
        backtrack(s, 0, path, result);
        return result;
    }
private:
    void backtrack(const string& s, int start, vector<string>& path, vector<string>& result) {
        if (path.size() == 4) {
            if (start == s.length()) {
                string ip = path[0] + "." + path[1] + "." + path[2] + "." + path[3];
                result.push_back(ip);
            }
            return;
        }
        int remainingSegments = 4 - path.size();
        int remainingChars = s.length() - start;
        if (remainingChars < remainingSegments || remainingChars > remainingSegments * 3) {
            return;
        }
        for (int len = 1; len <= 3; ++len) {
            if (start + len > s.length()) break;
            string segment = s.substr(start, len);
            if (isValid(segment)) {
                path.push_back(segment);
                backtrack(s, start + len, path, result);
                path.pop_back();
            }
        }
    }

    bool isValid(const string& segment) {
        if (segment.length() > 1 && segment[0] == '0') {
            return false;
        }

        int val = stoi(segment);
        if (val > 255) {
            return false;
        }
        return true;
    }
};
// @lc code=end

