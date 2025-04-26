#
# @lc app=leetcode id=19 lang=python3
#
# [19] Remove Nth Node From End of List
#
# https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/
#
# algorithms
# Medium (48.41%)
# Likes:    19861
# Dislikes: 851
# Total Accepted:    3.4M
# Total Submissions: 7M
# Testcase Example:  '[1,2,3,4,5]\n2'
#
# Given the head of a linked list, remove the n^th node from the end of the
# list and return its head.
# 
# 
# Example 1:
# 
# 
# Input: head = [1,2,3,4,5], n = 2
# Output: [1,2,3,5]
# 
# 
# Example 2:
# 
# 
# Input: head = [1], n = 1
# Output: []
# 
# 
# Example 3:
# 
# 
# Input: head = [1,2], n = 1
# Output: [1]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the list is sz.
# 1 <= sz <= 30
# 0 <= Node.val <= 100
# 1 <= n <= sz
# 
# 
# 
# Follow up: Could you do this in one pass?
# 
#

# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def removeNthFromEnd(self, head: Optional[ListNode], n: int) -> Optional[ListNode]:
        # 创建一个哑节点，指向头节点
        dummy = ListNode(0, head)
        first = dummy
        second = dummy

        # 让 first 指针先向前移动 n+1 步
        for _ in range(n + 1):
            first = first.next

        # 让 first 和 second 同时移动，直到 first 到达链表末尾
        while first:
            first = first.next
            second = second.next

        # 删除倒数第 n 个节点
        second.next = second.next.next

        # 返回头节点
        return dummy.next
        
# @lc code=end

