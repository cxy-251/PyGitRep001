#
# @lc app=leetcode id=83 lang=python3
#
# [83] Remove Duplicates from Sorted List
#
# https://leetcode.com/problems/remove-duplicates-from-sorted-list/description/
#
# algorithms
# Easy (54.54%)
# Likes:    9204
# Dislikes: 336
# Total Accepted:    1.9M
# Total Submissions: 3.5M
# Testcase Example:  '[1,1,2]'
#
# Given the head of a sorted linked list, delete all duplicates such that each
# element appears only once. Return the linked list sorted as well.
# 
# 
# Example 1:
# 
# 
# Input: head = [1,1,2]
# Output: [1,2]
# 
# 
# Example 2:
# 
# 
# Input: head = [1,1,2,3,3]
# Output: [1,2,3]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the list is in the range [0, 300].
# -100 <= Node.val <= 100
# The list is guaranteed to be sorted in ascending order.
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
    def deleteDuplicates(self, head: Optional[ListNode]) -> Optional[ListNode]:
        # 如果链表为空或只有一个节点，直接返回
        if not head or not head.next:
            return head

        # 当前节点
        current = head

        # 遍历链表
        while current and current.next:
            if current.val == current.next.val:
                # 如果当前节点和下一个节点的值相同，跳过下一个节点
                current.next = current.next.next
            else:
                # 否则，移动到下一个节点
                current = current.next

        return head
# @lc code=end

