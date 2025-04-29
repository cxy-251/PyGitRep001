#
# @lc app=leetcode id=101 lang=python3
#
# [101] Symmetric Tree
#
# https://leetcode.com/problems/symmetric-tree/description/
#
# algorithms
# Easy (58.86%)
# Likes:    16010
# Dislikes: 412
# Total Accepted:    2.4M
# Total Submissions: 4.1M
# Testcase Example:  '[1,2,2,3,4,4,3]'
#
# Given the root of a binary tree, check whether it is a mirror of itself
# (i.e., symmetric around its center).
# 
# 
# Example 1:
# 
# 
# Input: root = [1,2,2,3,4,4,3]
# Output: true
# 
# 
# Example 2:
# 
# 
# Input: root = [1,2,2,null,3,null,3]
# Output: false
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the tree is in the range [1, 1000].
# -100 <= Node.val <= 100
# 
# 
# 
# Follow up: Could you solve it both recursively and iteratively?
#

# @lc code=start
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def isSymmetric(self, root: Optional[TreeNode]) -> bool:
        # def isMirror(t1: Optional[TreeNode], t2: Optional[TreeNode]) -> bool:
        #     # 如果两个子树都为空，返回 True
        #     if not t1 and not t2:
        #         return True
        #     # 如果只有一个子树为空，或者值不同，返回 False
        #     if not t1 or not t2 or t1.val != t2.val:
        #         return False
        #     # 递归检查：左子树的左和右子树的右，左子树的右和右子树的左
        #     return isMirror(t1.left, t2.right) and isMirror(t1.right, t2.left)

        # # 检查根节点的左右子树是否是镜像
        # return isMirror(root, root)
    
        if not root:
            return True

        # 使用队列存储节点对
        queue = [(root.left, root.right)]

        while queue:
            t1, t2 = queue.pop(0)
            # 如果两个节点都为空，继续检查下一个
            if not t1 and not t2:
                continue
            # 如果只有一个节点为空，或者值不同，返回 False
            if not t1 or not t2 or t1.val != t2.val:
                return False
            # 将对应的子节点对加入队列
            queue.append((t1.left, t2.right))
            queue.append((t1.right, t2.left))

        return True
# @lc code=end

