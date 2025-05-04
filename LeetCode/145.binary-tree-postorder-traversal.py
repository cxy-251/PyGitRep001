#
# @lc app=leetcode id=145 lang=python3
#
# [145] Binary Tree Postorder Traversal
#
# https://leetcode.com/problems/binary-tree-postorder-traversal/description/
#
# algorithms
# Easy (75.20%)
# Likes:    7386
# Dislikes: 215
# Total Accepted:    1.6M
# Total Submissions: 2.1M
# Testcase Example:  '[1,null,2,3]'
#
# Given the root of a binary tree, return the postorder traversal of its nodes'
# values.
# 
# 
# Example 1:
# 
# 
# Input: root = [1,null,2,3]
# 
# Output: [3,2,1]
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
# Output: [4,6,7,5,2,9,8,3,1]
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
# The number of the nodes in the tree is in the range [0, 100].
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
# class Solution:
#     def postorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
#         result = []

#         def postorder(node):
#             if not node:
#                 return
#             # 递归遍历左子树
#             postorder(node.left)
#             # 递归遍历右子树
#             postorder(node.right)
#             # 访问当前节点
#             result.append(node.val)

#         postorder(root)
#         return result


class Solution:
    def postorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
        if not root:
            return []

        stack, result = [root], []

        while stack:
            node = stack.pop()
            if node:
                # 先将当前节点值插入结果的开头
                result.insert(0, node.val)
                # 将左子节点压入栈（后处理）
                if node.left:
                    stack.append(node.left)
                # 将右子节点压入栈（先处理）
                if node.right:
                    stack.append(node.right)

        return result
# @lc code=end

