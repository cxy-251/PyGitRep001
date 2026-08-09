/*
 * @lc app=leetcode id=297 lang=cpp
 *
 * [297] Serialize and Deserialize Binary Tree
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (!root) return "#"; // 用 # 表示空节点
        
        // 当前节点值 + 分隔符 + 左子树序列 + 分隔符 + 右子树序列
        return to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
        
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }
private:
    // 辅助函数：利用 stringstream 依次读取被逗号分隔的 token
    TreeNode* deserializeHelper(stringstream& ss) {
        string val;
        if (!getline(ss, val, ',')) {
            return nullptr;
        }

        if (val == "#") {
            return nullptr; // 遇到 # 说明是空节点
        }

        // 创建当前节点，并递归构造左子树和右子树
        TreeNode* root = new TreeNode(stoi(val));
        root->left = deserializeHelper(ss);
        root->right = deserializeHelper(ss);
        
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
// @lc code=end

