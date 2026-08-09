/*
 * @lc app=leetcode id=106 lang=cpp
 *
 * [106] Construct Binary Tree from Inorder and Postorder Traversal
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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        inorderMap.clear();
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap[inorder[i]] = i;
        }
        return buildTreeHelper(inorder, postorder,
            0, inorder.size() - 1, 0, postorder.size() - 1);
    }
private:
    unordered_map<int, int> inorderMap;
    TreeNode* buildTreeHelper(const vector<int>& inorder,
                              const vector<int>& postorder,
                              int inStart, int inEnd,
                              int postStart, int postEnd) {
        if (inStart > inEnd || postStart > postEnd) {
            return nullptr;
        }
        int rootVal = postorder[postEnd];
        TreeNode* root = new TreeNode(rootVal);
        int rootIndexInorder = inorderMap[rootVal];
        int leftSubtreeSize = rootIndexInorder - inStart;
        root->left = buildTreeHelper(inorder, postorder,
            inStart, rootIndexInorder - 1, postStart, postStart + leftSubtreeSize - 1);
        root->right = buildTreeHelper(inorder, postorder,
            rootIndexInorder + 1, inEnd, postStart + leftSubtreeSize, postEnd - 1);
        return root;
    }
};
// @lc code=end

