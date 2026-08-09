/*
 * @lc app=leetcode id=99 lang=cpp
 *
 * [99] Recover Binary Search Tree
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
    void recoverTree(TreeNode* root) {
        traverse(root);
        if (first && second) {
            swap(first->val, second->val);
        }
    }
private:
    TreeNode* first = nullptr;
    TreeNode* second = nullptr;
    TreeNode* prev = nullptr;
    void traverse(TreeNode* curr) {
        if (curr == nullptr) return;
        traverse(curr->left);
        if (prev != nullptr && curr->val < prev->val) {
            if (first == nullptr) {
                first = prev;
            }
            second = curr;
        }
        prev = curr;
        traverse(curr->right);
    }
};
// @lc code=end

