/*
 * @lc app=leetcode id=77 lang=cpp
 *
 * [77] Combinations
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        if (k > n) return result;
        vector<int> comb(k);
        for (int i = 0; i < k; ++i) comb[i] = i + 1;
        while (true) {
            result.push_back(comb);
            int i = k - 1;
            while (i >= 0 && comb[i] == n - k + i + 1) {
                i--;
            }
            if (i < 0) break;
            comb[i]++;
            for (int j = i + 1; j < k; ++j) {
                comb[j] = comb[j - 1] + 1;
            }
        }
        return result;
        // vector<vector<int>> result;
        // if (k > n) return result;
        // for (int mask = 0; mask < (1 << n); ++mask) {
        //     if (__builtin_popcount(mask) == k) {
        //         vector<int> combination;
        //         for (int i = 0; i < n; ++i) {
        //             if ((mask >> i) & 1) {
        //                 combination.push_back(i + 1);
        //             }
        //         }
        //         result.push_back(combination);
        //     }
        // }
        // return result;
    }
};
// @lc code=end

