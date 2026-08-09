/*
 * @lc app=leetcode id=105 lang=cpp
 *
 * [105] Construct Binary Tree from Preorder and Inorder Traversal
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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderMap.clear();
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap[inorder[i]] = i;
        }
        return buildTreeHelper(preorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
    }
private:
    unordered_map<int, int>inorderMap;
    TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd, int inStart, int inEnd) {
        if (preStart > preEnd || inStart > inEnd) {
            return nullptr;
        }
        int rootVal = preorder[preStart];
        TreeNode* root = new TreeNode(rootVal);
        int rootIndexInorder = inorderMap[rootVal];
        int leftSubtreeSize = rootIndexInorder - inStart;
        root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftSubtreeSize, inStart, rootIndexInorder - 1);
        root->right = buildTreeHelper(preorder, preStart + leftSubtreeSize + 1, preEnd, rootIndexInorder + 1, inEnd);
        return root;
    }
};
// @lc code=end

