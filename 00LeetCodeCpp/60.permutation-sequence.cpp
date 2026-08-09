/*
 * @lc app=leetcode id=60 lang=cpp
 *
 * [60] Permutation Sequence
 */

// @lc code=start
class Solution {
public:
    string getPermutation(int n, int k) {
        vector<int> factorial(n + 1, 1);
        for (int i = 1; i <= n; ++i) {
            factorial[i] = factorial[i - 1] * i;
        }
        vector<int> numbers;
        for (int i = 1; i <= n; ++i) {
            numbers.push_back(i);
        }
        string result = "";
        k = k - 1;
        for (int i = n; i >= 1; --i) {
            int group_size = factorial[i - 1];
            int index = k / group_size;
            result += to_string(numbers[index]);
            numbers.erase(numbers.begin() + index);
            k %= group_size;
        }
        return result;
    }
};
// @lc code=end

