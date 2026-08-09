/*
 * @lc app=leetcode id=110 lang=cpp
 *
 * [110] Balanced Binary Tree
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
    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
private:
    int checkHeight(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        int left_h = checkHeight(root->left);
        int right_h = checkHeight(root->right);
        if (left_h == -1 || right_h == -1) {
            return -1;
        }
        if (abs(left_h - right_h) > 1) {
            return -1;
        }
        return 1 + max(left_h, right_h);
    }
};
// @lc code=end

