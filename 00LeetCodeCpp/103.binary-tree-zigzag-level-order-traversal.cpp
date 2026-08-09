/*
 * @lc app=leetcode id=103 lang=cpp
 *
 * [103] Binary Tree Zigzag Level Order Traversal
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
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root) return {};
        vector<vector<int>> result;
        queue<TreeNode*> q;
        q.push(root);
        bool is_reverse = false;
        while (!q.empty()) {
            int size = q.size();
            vector<int> current_level(size);
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q.front();
                q.pop();
                if (!is_reverse) {
                    current_level[i] = node->val;
                } else {
                    current_level[size - 1 - i] = node->val;
                }
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(current_level);
            is_reverse = !is_reverse;
        }
        return result;
    }
};
// @lc code=end

