/*
 * @lc app=leetcode id=6 lang=cpp
 *
 * [6] Zigzag Conversion
 */

// @lc code=start
class Solution {
public:
    string convert(string s, int numRows) {
        if (numRows < 2 || s.length() <= numRows) return s;

        // vector<string> rows(numRows);
        // int curRow = 0;
        // int direction = -1;

        // for (char c : s) {
        //     rows[curRow] += c;

        //     if (curRow == 0 || curRow == numRows - 1) {
        //         direction = -direction;
        //     }

        //     curRow += direction;
        // }

        // string res;
        // // res.reserve(s.length()); // 避免动态扩容搬家的时间
        // for (const string& row : rows) {
        //     res += row;
        // }

        string res;
        int n = s.length();
        int step = 2 * numRows - 2; // 一个周期的跨度

        for (int i = 0; i < numRows; i++) { // 逐行处理
            for (int j = 0; j + i < n; j += step) {
                // 1. 添加每一行都会有的第一个字符
                res += s[j + i];
                
                // 2. 如果是中间行，且第二个字符也在范围内
                if (i != 0 && i != numRows - 1 && j + step - i < n) {
                    res += s[j + step - i];
                }
            }
        }
        return res;
    }
};
// @lc code=end

