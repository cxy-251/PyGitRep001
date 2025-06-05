#
# @lc app=leetcode id=203 lang=python3
#
# [203] Remove Linked List Elements
#
# https://leetcode.com/problems/remove-linked-list-elements/description/
#
# algorithms
# Easy (51.41%)
# Likes:    8680
# Dislikes: 269
# Total Accepted:    1.4M
# Total Submissions: 2.6M
# Testcase Example:  '[1,2,6,3,4,5,6]\n6'
#
# Given the head of a linked list and an integer val, remove all the nodes of
# the linked list that has Node.val == val, and return the new head.
# 
# 
# Example 1:
# 
# 
# Input: head = [1,2,6,3,4,5,6], val = 6
# Output: [1,2,3,4,5]
# 
# 
# Example 2:
# 
# 
# Input: head = [], val = 1
# Output: []
# 
# 
# Example 3:
# 
# 
# Input: head = [7,7,7,7], val = 7
# Output: []
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the list is in the range [0, 10^4].
# 1 <= Node.val <= 50
# 0 <= val <= 50
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
    def removeElements(self, head: Optional[ListNode], val: int) -> Optional[ListNode]:
        dummy = ListNode(0, head)
        cur = dummy
        while cur.next:
            if cur.next.val == val:
                cur.next = cur.next.next
            else:
                cur = cur.next
        return dummy.next

# @lc code=end

