/*
 * @lc app=leetcode id=30 lang=cpp
 *
 * [30] Substring with Concatenation of All Words
 */

// @lc code=start
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (s.empty() || words.empty()) return res;
        int n = s.size(), m = words.size(), len = words[0].size();
        // unordered_map<string, int> wordCount;
        unordered_map<string_view, int> wordCount;
        for (const string& w : words) wordCount[w]++;

        for (int i = 0; i < len; i++) {
            int left = i, count = 0;
            // unordered_map<string, int> tempCount;
            unordered_map<string_view, int> tempCount;

            for (int j = i; j <= n - len; j += len) {
                // string w = s.substr(j, len);
                // 【优化点】：用 string_view 代替 substr
                // 这里没有任何内存分配和字符串拷贝
                string_view w(s.data() + j, len);
                if (wordCount.count(w)) {
                    tempCount[w]++;
                    count++;
                    while (tempCount[w] > wordCount[w]) {
                        // string leftWord = s.substr(left, len);
                        string_view leftWord(s.data() + left, len);
                        tempCount[leftWord]--;
                        count--;
                        left += len;
                    }
                    if (count == m) res.push_back(left);
                } else {
                    tempCount.clear();
                    count = 0;
                    left = j + len;
                }
            }
        }
        return res;
    }
};
// @lc code=end

