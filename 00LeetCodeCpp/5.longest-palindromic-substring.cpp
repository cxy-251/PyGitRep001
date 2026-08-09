/*
 * @lc app=leetcode id=5 lang=cpp
 *
 * [5] Longest Palindromic Substring
 */

// @lc code=start
class Solution {
public:
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
        return right - left - 1;

    }
    string longestPalindrome(string s) {
        // if (s.length() < 2) return s;
        // int start = 0, maxLen = 0;

        // for (int idx = 0; idx < s.length(); idx++) {
        //     if (s.length() - idx <= maxLen / 2) break;
        //     int curLen = max(expandAroundCenter(s, idx, idx),
        //                      expandAroundCenter(s, idx, idx + 1));

        //     if (curLen > maxLen) {
        //         maxLen = curLen;
        //         start = idx - (curLen - 1) / 2;
        //     }
        // }

        /* */
        // int n = s.length();
        // if (n < 2) return s;
        
        // int start = 0, maxLen = 1;
        // for (int i = 0; i < n; ) {
        //     // 剪枝：剩余字符不够长了，直接收工
        //     if (n - i <= maxLen / 2) break;

        //     int left = i, right = i;
        //     // 技巧：跳过中间所有重复字符（如 "baaaaab" 中的 "aaaaa"）
        //     while (right < n - 1 && s[right + 1] == s[right]) {
        //         right++;
        //     }
            
        //     // 下一次循环直接从重复字符后面开始，大大减少循环次数
        //     i = right + 1;

        //     // 从这个“中心块”向两边扩散
        //     while (left > 0 && right < n - 1 && s[left - 1] == s[right + 1]) {
        //         left--;
        //         right++;
        //     }

        //     int curLen = right - left + 1;
        //     if (curLen > maxLen) {
        //         start = left;
        //         maxLen = curLen;
        //     }
        // }

        /* Manacher */
        // 1. 预处理：插入 #
        string t = "$#";
        for (char c : s) { t += c; t += '#'; }
        t += '@'; // 首尾不同字符防止越界

        int n = t.size();
        vector<int> p(n, 0);
        int C = 0, R = 0, maxLen = 0, centerIndex = 0;

        for (int i = 1; i < n - 1; i++) {
            // 【核心代码】利用对称性初始化 P[i]
            if (i < R) p[i] = min(R - i, p[2 * C - i]);
            
            // 尝试继续向外扩散
            while (t[i + p[i] + 1] == t[i - p[i] - 1]) p[i]++;

            // 更新中心和最右边界
            if (i + p[i] > R) {
                C = i;
                R = i + p[i];
            }

            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIndex = i;
            }
        }
        // 计算原字符串起始位置并返回
        int start = (centerIndex - maxLen) / 2;
        return s.substr(start, maxLen);
    }
};
// @lc code=end

