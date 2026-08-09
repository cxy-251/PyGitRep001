/*
 * @lc app=leetcode id=65 lang=cpp
 *
 * [65] Valid Number
 */

// @lc code=start
class Solution {
public:
    bool isNumber(string s) {
        string pattern = R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)";
        
        regex re(pattern);
        return regex_match(s, re);
        // bool seenDigit = false;
        // bool seenDot = false;
        // bool seenExponent = false;
        // for (int i = 0; i < s.size(); i++) {
        //     char c = s[i];
        //     if (isdigit(c)) {
        //         seenDigit = true;
        //     } else if (c == '+' || c == '-') {
        //         if (i > 0 && s[i - 1] != 'e' && s[i - 1] != 'E') {
        //             return false;
        //         }
        //     } else if (c == 'e' || c == 'E') {
        //         if (seenExponent || !seenDigit) {
        //             return false;
        //         }
        //         seenExponent = true;
        //         seenDigit = false;
        //     } else if (c == '.') {
        //         if (seenDot || seenExponent) {
        //             return false;
        //         }
        //         seenDot = true;
        //     } else {
        //         return false;
        //     }
        // }
        // return seenDigit;
    }
};
// @lc code=end

