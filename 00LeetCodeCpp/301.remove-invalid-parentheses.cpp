/*
 * @lc app=leetcode id=301 lang=cpp
 *
 * [301] Remove Invalid Parentheses
 */

// @lc code=start
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> result;
        if (s.empty()) return result;

        queue<string> q;
        unordered_set<string> visited;

        q.push(s);
        visited.insert(s);

        bool found = false; // 标记是否在当前层找到了有效字符串

        while (!q.empty()) {
            int levelSize = q.size();
            
            // 遍历当前层的所有可能状态
            for (int i = 0; i < levelSize; ++i) {
                string curr = q.front();
                q.pop();

                if (isValid(curr)) {
                    result.push_back(curr);
                    found = true; // 标记找到了，后续层级不再需要探索
                }

                // 如果本层已经找到了有效字符串，则不需要再生成下一层（删除更多字符）
                if (found) continue;

                // 生成下一层状态：尝试删除每一个括号
                for (int j = 0; j < curr.length(); ++j) {
                    if (curr[j] != '(' && curr[j] != ')') continue;

                    // 构建新字符串：删除索引 j 处的字符
                    string next = curr.substr(0, j) + curr.substr(j + 1);
                    if (visited.find(next) == visited.end()) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }

            // 如果在当前层找到了一个或多个有效字符串，直接退出循环
            if (found) break;
        }

        return result;
    }
    // 辅助函数：检查字符串是否为有效括号组合
    bool isValid(string s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') {
                count--;
                if (count < 0) return false; // 右括号比左括号多
            }
        }
        return count == 0;
    }
};
// @lc code=end

