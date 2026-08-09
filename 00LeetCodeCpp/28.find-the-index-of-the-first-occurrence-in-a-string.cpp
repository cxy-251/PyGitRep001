// ...existing code...
/*
 * @lc app=leetcode id=28 lang=cpp
 *
 * [28] Find the Index of the First Occurrence in a String
 */

// @lc code=start
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) return 0;
        int n = haystack.size(), m = needle.size();
        // 构建 LPS（最长相同前后缀）数组
        vector<int> lps(m, 0);
        for (int i = 1, len = 0; i < m; ++i) {
            while (len > 0 && needle[i] != needle[len]) len = lps[len - 1];
            if (needle[i] == needle[len]) ++len;
            lps[i] = len;
        }
        // KMP 搜索
        int j = 0; // needle 指针
        for (int i = 0; i < n; ++i) {
            while (j > 0 && haystack[i] != needle[j]) j = lps[j - 1];
            if (haystack[i] == needle[j]) ++j;
            if (j == m) return i - m + 1;
        }
        return -1;
    }
};
// @lc code=end
// ...existing code...