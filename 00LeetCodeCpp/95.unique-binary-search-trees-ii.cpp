/*
 * @lc app=leetcode id=95 lang=cpp
 *
 * [95] Unique Binary Search Trees II
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
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};
        return buildTrees(1, n);
    }
private:
    vector<TreeNode*> buildTrees(int start, int end) {
        vector<TreeNode*> res;
        if (start > end) {
            return res;
        }
        for (int i = start; i <= end; ++i) {
            vector<TreeNode*> leftSubtrees = buildTrees(start, i - 1);
            vector<TreeNode*> rightSubtrees = buildTrees(i + 1, end);
            if (leftSubtrees.empty() && rightSubtrees.empty()) {
                res.push_back(new TreeNode(i));
            } else if (leftSubtrees.empty()) {
                for (TreeNode* r : rightSubtrees) {
                    TreeNode* root = new TreeNode(i);
                    root->right = r;
                    res.push_back(root);
                }
            } else if (rightSubtrees.empty()) {
                for (TreeNode* l : leftSubtrees) {
                    TreeNode* root = new TreeNode(i);
                    root->left = l;
                    res.push_back(root);
                }
            } else {
                for (TreeNode* l : leftSubtrees) {
                    for (TreeNode* r : rightSubtrees) {
                        TreeNode* root = new TreeNode(i);
                        root->left = l;
                        root->right = r;
                        res.push_back(root);
                    }
                }
            }
        }
        return res;
    }
};
// @lc code=end

