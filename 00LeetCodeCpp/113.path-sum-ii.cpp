/*
 * @lc app=leetcode id=113 lang=cpp
 *
 * [113] Path Sum II
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> result;
        vector<int> currentPath;
        backtrack(root, targetSum, currentPath, result);
        return result;
    }
private:
    void backtrack(TreeNode* node, int remainingSum, vector<int>& currentPath, vector<vector<int>>& result) {
        if (node == nullptr) {
            return;
        }
        currentPath.push_back(node->val);
        if (node->left == nullptr && node->right == nullptr) {
            if (node->val == remainingSum) {
                result.push_back(currentPath);
            }
        } else {
            backtrack(node->left, remainingSum - node->val, currentPath, result);
            backtrack(node->right, remainingSum - node->val, currentPath, result);
        }
        currentPath.pop_back();
    }
};
// @lc code=end

