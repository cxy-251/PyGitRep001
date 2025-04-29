#
# @lc app=leetcode id=110 lang=python3
#
# [110] Balanced Binary Tree
#
# https://leetcode.com/problems/balanced-binary-tree/description/
#
# algorithms
# Easy (54.86%)
# Likes:    11313
# Dislikes: 762
# Total Accepted:    1.9M
# Total Submissions: 3.5M
# Testcase Example:  '[3,9,20,null,null,15,7]'
#
# Given a binary tree, determine if it is height-balanced.
# 
# 
# Example 1:
# 
# 
# Input: root = [3,9,20,null,null,15,7]
# Output: true
# 
# 
# Example 2:
# 
# 
# Input: root = [1,2,2,3,3,null,null,4,4]
# Output: false
# 
# 
# Example 3:
# 
# 
# Input: root = []
# Output: true
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [0, 5000].
# -10^4 <= Node.val <= 10^4
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
    def isBalanced(self, root: Optional[TreeNode]) -> bool:
        def check(node):
            # 如果节点为空，高度为 0，且是平衡的
            if not node:
                return 0, True
            
            # 递归检查左子树
            left_height, left_balanced = check(node.left)
            # 递归检查右子树
            right_height, right_balanced = check(node.right)
            
            # 当前节点是否平衡
            balanced = left_balanced and right_balanced and abs(left_height - right_height) <= 1
            
            # 当前节点的高度
            height = 1 + max(left_height, right_height)
            
            return height, balanced
        
        # 检查整棵树是否平衡
        _, is_balanced = check(root)
        return is_balanced
# @lc code=end

