/*
 * @lc app=leetcode id=38 lang=cpp
 *
 * [38] Count and Say
 */

// @lc code=start
class Solution {
public:
    string countAndSay(int n) {
        if (n == 1) return "1";
        string res = "1";
        for (int i = 2; i <= n; i++) {
            // 使用 stringstream 替代普通的 string 拼接
            stringstream ss;
            // string next = "";
            int len = res.length();
            for (int j = 0; j < len; ) {
                int start = j;
                while (j < len && res[j] == res[start]) {
                    j++;
                }
                // 直接将次数和数字流入缓冲区，避免中途产生临时 string 对象
                ss << (j - start) << res[start];
                // next += to_string(j - start) + res[start];
            }
            res = ss.str(); // 最后一并转回 string
            // res = next;
        }
        return res;
    }
};
// @lc code=end

