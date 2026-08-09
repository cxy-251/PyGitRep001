/*
 * @lc app=leetcode id=273 lang=cpp
 *
 * [273] Integer to English Words
 */

// @lc code=start
class Solution {
public:
    string numberToWords(int num) {
        if (num == 0) return "Zero";

        string res = "";
        int i = 0; // 用于在 thousands 数组中索引 (Thousand, Million, Billion)

        while (num > 0) {
            // 每次处理 3 位数字
            if (num % 1000 != 0) {
                // 处理当前 3 位，并加上对应的单位（如 Thousand）
                string part = helper(num % 1000);
                string unit = thousands[i];
                
                // 构建当前 3 位组： [数字部分] + [单位]
                string currentGroup = part;
                if (unit != "") {
                    currentGroup += " " + unit;
                }

                // 将当前组拼接到最终结果的前面： [当前组] + [之前的结果]
                if (res == "") {
                    res = currentGroup;
                } else {
                    res = currentGroup + " " + res;
                }
            }
            num /= 1000;
            i++;
        }
    
        return trim(res);
    }

private:
    // 定义基础词库
    vector<string> below_20 = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", 
                               "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    vector<string> tens = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    vector<string> thousands = {"", "Thousand", "Million", "Billion"};
    // 辅助函数：处理 0-999 之间的数字
    string helper(int num) {
        if (num == 0) return "";
        if (num < 20) return below_20[num];
        if (num < 100) {
            string res = tens[num / 10];
            string rem = helper(num % 10);
            return rem == "" ? res : res + " " + rem;
        }
        
        // 百位处理：数字 + "Hundred" + 递归处理剩下的十位和个位
        string res = below_20[num / 100] + " Hundred";
        string rem = helper(num % 100);
        return rem == "" ? res : res + " " + rem;
    }
    // 【新增】修剪函数：删除字符串首尾的所有空格
    string trim(const string& s) {
        size_t first = s.find_first_not_of(' ');
        if (string::npos == first) return "";
        size_t last = s.find_last_not_of(' ');
        return s.substr(first, (last - first + 1));
    }
};
// @lc code=end

