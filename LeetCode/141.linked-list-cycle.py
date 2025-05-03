#
# @lc app=leetcode id=141 lang=python3
#
# [141] Linked List Cycle
#
# https://leetcode.com/problems/linked-list-cycle/description/
#
# algorithms
# Easy (52.18%)
# Likes:    16387
# Dislikes: 1495
# Total Accepted:    3.9M
# Total Submissions: 7.4M
# Testcase Example:  '[3,2,0,-4]\n1'
#
# Given head, the head of a linked list, determine if the linked list has a
# cycle in it.
# 
# There is a cycle in a linked list if there is some node in the list that can
# be reached again by continuously following the next pointer. Internally, pos
# is used to denote the index of the node that tail's next pointer is connected
# to. Note that pos is not passed as a parameter.
# 
# Return true if there is a cycle in the linked list. Otherwise, return
# false.
# 
# 
# Example 1:
# 
# 
# Input: head = [3,2,0,-4], pos = 1
# Output: true
# Explanation: There is a cycle in the linked list, where the tail connects to
# the 1st node (0-indexed).
# 
# 
# Example 2:
# 
# 
# Input: head = [1,2], pos = 0
# Output: true
# Explanation: There is a cycle in the linked list, where the tail connects to
# the 0th node.
# 
# 
# Example 3:
# 
# 
# Input: head = [1], pos = -1
# Output: false
# Explanation: There is no cycle in the linked list.
# 
# 
# 
# Constraints:
# 
# 
# The number of the nodes in the list is in the range [0, 10^4].
# -10^5 <= Node.val <= 10^5
# pos is -1 or a valid index in the linked-list.
# 
# 
# 
# Follow up: Can you solve it using O(1) (i.e. constant) memory?
# 
#

# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution:
    def hasCycle(self, head: Optional[ListNode]) -> bool:
        # 初始化快慢指针
        slow, fast = head, head

        # 快指针每次移动两步，慢指针每次移动一步
        while fast and fast.next:
            slow = slow.next
            fast = fast.next.next

            # 如果快慢指针相遇，说明有环
            if slow == fast:
                return True

        # 如果快指针到达链表末尾，说明没有环
        return False
        
# @lc code=end

