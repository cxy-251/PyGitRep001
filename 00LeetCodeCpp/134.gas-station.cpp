/*
 * @lc app=leetcode id=134 lang=cpp
 *
 * [134] Gas Station
 */

// @lc code=start
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalGas = 0;
        int currentGas = 0;
        int startNode = 0;
        for (int i = 0; i < gas.size(); ++i) {
            int diff = gas[i] - cost[i];
            totalGas += diff;
            currentGas += diff;
            if (currentGas < 0) {
                startNode = i + 1;
                currentGas = 0;
            }
        }
        if (totalGas < 0) {
            return -1;
        }
        return startNode;
    }
};
// @lc code=end

