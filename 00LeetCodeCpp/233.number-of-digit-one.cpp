/*
 * @lc app=leetcode id=233 lang=cpp
 *
 * [233] Number of Digit One
 */

// @lc code=start
class Solution {
public:
    int countDigitOne(int n) {
        if (n <= 0) return 0;
        long long count = 0;
        for (long long i = 1; i <= n; i *= 10) {
            long long higher = n / (i * 10);
            int curr = (n / i) % 10;
            long long lower = n % i;
            if (curr > 1) {
                count += (higher + 1) * i;
            } else if (curr == 1) {
                count += (higher * i) + (lower + 1);
            } else {
                count += higher * i;
            }
        }
        return (int)count;
    }
};
// @lc code=end

