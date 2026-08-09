/*
 * @lc app=leetcode id=229 lang=cpp
 *
 * [229] Majority Element II
 */

// @lc code=start
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        if (nums.empty()) return {};
        int cand1 = 0, cand2 = 0;
        int count1 = 0, count2 = 0;
        for (int n : nums) {
            if (n == cand1) {
                count1++;
            } else if (n == cand2) {
                count2++;
            } else if (count1 == 0) {
                cand1 = n;
                count1 = 1;
            } else if (count2 == 0) {
                cand2 = n;
                count2 = 1;
            } else {
                count1--;
                count2--;
            }
        }
        vector<int> result;
        int actualCount1 = 0, actualCount2 = 0;
        for (int n : nums) {
            if (n == cand1) actualCount1++;
            else if (n == cand2) actualCount2++;
        }
        int nSize = nums.size();
        if (actualCount1 > nSize / 3) result.push_back(cand1);
        if (actualCount2 > nSize / 3) result.push_back(cand2);
        return result;
    }
};
// @lc code=end

