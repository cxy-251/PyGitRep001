/*
 * @lc app=leetcode id=59 lang=cpp
 *
 * [59] Spiral Matrix II
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> matrix(n, vector<int>(n));
        int top = 0, bottom = n - 1, left = 0, right = n - 1;
        int num = 1;
        while (num <= n * n) {
            // Right
            for (int i = left; i <= right; ++i) matrix[top][i] = num++;
            top++;
            // Down
            for (int i = top; i <= bottom; ++i) matrix[i][right] = num++;
            right--;
            // Left
            if (top <= bottom) {
                for (int i = right; i >= left; --i) matrix[bottom][i] = num++;
                bottom--;
            }
            // Up
            if (left <= right) {
                for (int i = bottom; i >= top; --i) matrix[i][left] = num++;
                left++;
            }
        }
        return matrix;
    }
};
// @lc code=end

