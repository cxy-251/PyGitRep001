/*
 * @lc app=leetcode id=205 lang=cpp
 *
 * [205] Isomorphic Strings
 */

// @lc code=start
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if (s.length() != t.length()) return false;
        unordered_map<char, char> s2t;
        unordered_map<char, char> t2s;
        for (int i = 0; i < s.length(); ++i) {
            char charS = s[i];
            char charT = t[i];
            if (s2t.count(charS) && s2t[charS] != charT) {
                return false;
            }
            if (t2s.count(charT) && t2s[charT] != charS) {
                return false;
            }
            s2t[charS] = charT;
            t2s[charT] = charS;
        }
        return true;
    }
};
// @lc code=end

