#
# @lc app=leetcode id=222 lang=python3
#
# [222] Count Complete Tree Nodes
#
# https://leetcode.com/problems/count-complete-tree-nodes/description/
#
# algorithms
# Easy (69.31%)
# Likes:    9084
# Dislikes: 573
# Total Accepted:    961.6K
# Total Submissions: 1.4M
# Testcase Example:  '[1,2,3,4,5,6]'
#
# Given the root of a complete binary tree, return the number of the nodes in
# the tree.
# 
# According to Wikipedia, every level, except possibly the last, is completely
# filled in a complete binary tree, and all nodes in the last level are as far
# left as possible. It can have between 1 and 2^h nodes inclusive at the last
# level h.
# 
# Design an algorithm that runs in less than O(n) time complexity.
# 
# 
# Example 1:
# 
# 
# Input: root = [1,2,3,4,5,6]
# Output: 6
# 
# 
# Example 2:
# 
# 
# Input: root = []
# Output: 0
# 
# 
# Example 3:
# 
# 
# Input: root = [1]
# Output: 1
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [0, 5 * 10^4].
# 0 <= Node.val <= 5 * 10^4
# The tree is guaranteed to be complete.
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
    def countNodes(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        # 计算左子树和右子树的高度
        left, right = root, root
        left_height = right_height = 0
        while left:
            left_height += 1
            left = left.left
        while right:
            right_height += 1
            right = right.right

        # 如果左右高度相等，说明是满二叉树
        if left_height == right_height:
            return (1 << left_height) - 1

        # 否则递归统计左右子树
        return 1 + self.countNodes(root.left) + self.countNodes(root.right)
# @lc code=end

