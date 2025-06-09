#
# @lc app=leetcode id=257 lang=python3
#
# [257] Binary Tree Paths
#
# https://leetcode.com/problems/binary-tree-paths/description/
#
# algorithms
# Easy (66.04%)
# Likes:    6909
# Dislikes: 327
# Total Accepted:    872.1K
# Total Submissions: 1.3M
# Testcase Example:  '[1,2,3,null,5]'
#
# Given the root of a binary tree, return all root-to-leaf paths in any order.
# 
# A leaf is a node with no children.
# 
# 
# Example 1:
# 
# 
# Input: root = [1,2,3,null,5]
# Output: ["1->2->5","1->3"]
# 
# 
# Example 2:
# 
# 
# Input: root = [1]
# Output: ["1"]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [1, 100].
# -100 <= Node.val <= 100
# 
# 
#

# @lc code=start
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def binaryTreePaths(self, root: Optional[TreeNode]) -> List[str]:
        res = []
        def dfs(node, path):
            if not node.left and not node.right:
                res.append(path)
                return
            if node.left:
                dfs(node.left, path + "->" + str(node.left.val))
            if node.right:
                dfs(node.right, path + "->" + str(node.right.val))
        if root:
            dfs(root, str(root.val))
        return res
# @lc code=end

