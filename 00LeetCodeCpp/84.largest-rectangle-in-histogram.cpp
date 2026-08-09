/*
 * @lc app=leetcode id=84 lang=cpp
 *
 * [84] Largest Rectangle in Histogram
 */

// @lc code=start
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        if (n == 0) return 0;
        vector<int> left_smaller(n);
        vector<int> right_smaller(n);
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && heights[st.top()] >= heights[i]) {
                st.pop();
            }
            left_smaller[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }
        while (!st.empty()) st.pop();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && heights[st.top()] >= heights[i]) {
                st.pop();
            }
            right_smaller[i] = st.empty() ? n : st.top();
            st.push(i);
        }
        int maxArea = 0;
        for (int i = 0; i < n; ++i) {
            int width = right_smaller[i] - left_smaller[i] - 1;
            maxArea = std::max(maxArea, heights[i] * width);
        }
        return maxArea;
    }
};
// @lc code=end

