#
# @lc app=leetcode id=111 lang=python3
#
# [111] Minimum Depth of Binary Tree
#
# https://leetcode.com/problems/minimum-depth-of-binary-tree/description/
#
# algorithms
# Easy (50.24%)
# Likes:    7532
# Dislikes: 1336
# Total Accepted:    1.4M
# Total Submissions: 2.8M
# Testcase Example:  '[3,9,20,null,null,15,7]'
#
# Given a binary tree, find its minimum depth.
# 
# The minimum depth is the number of nodes along the shortest path from the
# root node down to the nearest leaf node.
# 
# Note: A leaf is a node with no children.
# 
# 
# Example 1:
# 
# 
# Input: root = [3,9,20,null,null,15,7]
# Output: 2
# 
# 
# Example 2:
# 
# 
# Input: root = [2,null,3,null,4,null,5,null,6]
# Output: 5
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [0, 10^5].
# -1000 <= Node.val <= 1000
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
    def minDepth(self, root: Optional[TreeNode]) -> int:
        # 如果树为空，深度为 0
        # if not root:
        #     return 0
        
        # # 如果左子树为空，递归计算右子树的最小深度
        # if not root.left:
        #     return 1 + self.minDepth(root.right)
        
        # # 如果右子树为空，递归计算左子树的最小深度
        # if not root.right:
        #     return 1 + self.minDepth(root.left)
        
        # # 左右子树都不为空，返回左右子树最小深度的较小值加 1
        # return 1 + min(self.minDepth(root.left), self.minDepth(root.right))
    
        if not root:
            return 0
        
        queue = deque([(root, 1)])  # 队列存储节点和对应的深度
        
        while queue:
            node, depth = queue.popleft()
            
            # 如果遇到叶子节点，返回当前深度
            if not node.left and not node.right:
                return depth
            
            # 将子节点加入队列，并记录深度
            if node.left:
                queue.append((node.left, depth + 1))
            if node.right:
                queue.append((node.right, depth + 1))
# @lc code=end

