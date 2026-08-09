/*
 * @lc app=leetcode id=282 lang=cpp
 *
 * [282] Expression Add Operators
 */

// @lc code=start
class Solution {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> result;
        if (num.empty()) return result;
        
        // path: 记录当前的表达式字符串
        // index: 当前处理到 num 的哪个位置
        // currentVal: 当前表达式计算出的值
        // prevOperand: 上一个操作数（用于处理乘法优先级）
        backtrack(result, num, target, "", 0, 0, 0);
        return result;
    }

private:
    void backtrack(vector<string>& result, const string& num, int target, 
                   string path, int index, long long currentVal, long long prevOperand) {
        
        // 递归终止条件：处理完所有数字
        if (index == num.length()) {
            if (currentVal == target) {
                result.push_back(path);
            }
            return;
        }

        // 尝试截取不同长度的数字
        for (int i = index; i < num.length(); ++i) {
            // 处理前导零：如果当前截取的数字是以 '0' 开头且长度大于 1，则非法
            if (i > index && num[index] == '0') break;

            // 将截取的字符串转为 long long
            string part = num.substr(index, i - index + 1);
            long long currNum = stoll(part);

            if (index == 0) {
                // 第一个数字，直接递归，不需要运算符
                backtrack(result, num, target, part, i + 1, currNum, currNum);
            } else {
                // 尝试三种运算符
                
                // 1. 加法
                backtrack(result, num, target, path + "+" + part, i + 1, 
                          currentVal + currNum, currNum);

                // 2. 减法
                backtrack(result, num, target, path + "-" + part, i + 1, 
                          currentVal - currNum, -currNum);

                // 3. 乘法 (核心：处理优先级)
                // 撤销上一步的操作，加上 (prev * curr)
                backtrack(result, num, target, path + "*" + part, i + 1, 
                          (currentVal - prevOperand) + (prevOperand * currNum), 
                          prevOperand * currNum);
            }
        }
    }
};
// @lc code=end

