#
# @lc app=leetcode id=94 lang=python3
#
# [94] Binary Tree Inorder Traversal
#
# https://leetcode.com/problems/binary-tree-inorder-traversal/description/
#
# algorithms
# Easy (78.22%)
# Likes:    14062
# Dislikes: 837
# Total Accepted:    3.1M
# Total Submissions: 3.9M
# Testcase Example:  '[1,null,2,3]'
#
# Given the root of a binary tree, return the inorder traversal of its nodes'
# values.
# 
# 
# Example 1:
# 
# 
# Input: root = [1,null,2,3]
# 
# Output: [1,3,2]
# 
# Explanation:
# 
# 
# 
# 
# Example 2:
# 
# 
# Input: root = [1,2,3,4,5,null,8,null,null,6,7,9]
# 
# Output: [4,2,6,5,7,1,3,9,8]
# 
# Explanation:
# 
# 
# 
# 
# Example 3:
# 
# 
# Input: root = []
# 
# Output: []
# 
# 
# Example 4:
# 
# 
# Input: root = [1]
# 
# Output: [1]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [0, 100].
# -100 <= Node.val <= 100
# 
# 
# 
# Follow up: Recursive solution is trivial, could you do it iteratively?
#

# @lc code=start
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def inorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
        # result = []

        # def inorder(node):
        #     if not node:
        #         return
        #     # 递归遍历左子树
        #     inorder(node.left)
        #     # 访问当前节点
        #     result.append(node.val)
        #     # 递归遍历右子树
        #     inorder(node.right)

        # inorder(root)
        # return result
    
        result = []
        stack = []
        current = root

        while current or stack:
            # 一直向左走，将节点压入栈
            while current:
                stack.append(current)
                current = current.left
            # 弹出栈顶节点并访问
            current = stack.pop()
            result.append(current.val)
            # 转向右子树
            current = current.right

        return result
# @lc code=end

