#
# @lc app=leetcode id=24 lang=python3
#
# [24] Swap Nodes in Pairs
#
# https://leetcode.com/problems/swap-nodes-in-pairs/description/
#
# algorithms
# Medium (66.76%)
# Likes:    12435
# Dislikes: 475
# Total Accepted:    1.6M
# Total Submissions: 2.4M
# Testcase Example:  '[1,2,3,4]'
#
# Given a linked list, swap every two adjacent nodes and return its head. You
# must solve the problem without modifying the values in the list's nodes
# (i.e., only nodes themselves may be changed.)
# 
# 
# Example 1:
# 
# 
# Input: head = [1,2,3,4]
# 
# Output: [2,1,4,3]
# 
# Explanation:
# 
# 
# 
# 
# Example 2:
# 
# 
# Input: head = []
# 
# Output: []
# 
# 
# Example 3:
# 
# 
# Input: head = [1]
# 
# Output: [1]
# 
# 
# Example 4:
# 
# 
# Input: head = [1,2,3]
# 
# Output: [2,1,3]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the list is in the range [0, 100].
# 0 <= Node.val <= 100
# 
# 
#

# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def swapPairs(self, head: Optional[ListNode]) -> Optional[ListNode]:
        # 创建一个哑节点，指向头节点
        dummy = ListNode(-1)
        dummy.next = head
        prev = dummy

        # 遍历链表，交换相邻节点
        while prev.next and prev.next.next:
            # 定义要交换的两个节点
            first = prev.next
            second = first.next

            # 交换节点
            first.next = second.next
            second.next = first
            prev.next = second

            # 移动 prev 指针，准备下一次交换
            prev = first

        # 返回新的头节点
        return dummy.next
# @lc code=end

