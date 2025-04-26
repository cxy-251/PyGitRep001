#
# @lc app=leetcode id=25 lang=python3
#
# [25] Reverse Nodes in k-Group
#
# https://leetcode.com/problems/reverse-nodes-in-k-group/description/
#
# algorithms
# Hard (62.41%)
# Likes:    14510
# Dislikes: 749
# Total Accepted:    1.2M
# Total Submissions: 1.9M
# Testcase Example:  '[1,2,3,4,5]\n2'
#
# Given the head of a linked list, reverse the nodes of the list k at a time,
# and return the modified list.
# 
# k is a positive integer and is less than or equal to the length of the linked
# list. If the number of nodes is not a multiple of k then left-out nodes, in
# the end, should remain as it is.
# 
# You may not alter the values in the list's nodes, only nodes themselves may
# be changed.
# 
# 
# Example 1:
# 
# 
# Input: head = [1,2,3,4,5], k = 2
# Output: [2,1,4,3,5]
# 
# 
# Example 2:
# 
# 
# Input: head = [1,2,3,4,5], k = 3
# Output: [3,2,1,4,5]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in the list is n.
# 1 <= k <= n <= 5000
# 0 <= Node.val <= 1000
# 
# 
# 
# Follow-up: Can you solve the problem in O(1) extra memory space?
# 
#

# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def reverseKGroup(self, head: Optional[ListNode], k: int) -> Optional[ListNode]:
        # 创建一个哑节点，指向头节点
        dummy = ListNode(0)
        dummy.next = head
        prev_group = dummy

        while True:
            # 检查是否有足够的节点进行翻转
            kth = self.get_kth_node(prev_group, k)
            if not kth:
                break

            # 记录下一组的起点
            next_group = kth.next

            # 翻转当前组
            prev, curr = kth.next, prev_group.next
            while curr != next_group:
                temp = curr.next
                curr.next = prev
                prev = curr
                curr = temp

            # 更新 prev_group 的 next 和 kth 的 next
            temp = prev_group.next
            prev_group.next = kth
            prev_group = temp

        return dummy.next

    def get_kth_node(self, curr: ListNode, k: int) -> Optional[ListNode]:
        # 找到当前组的第 k 个节点
        while curr and k > 0:
            curr = curr.next
            k -= 1
        return curr
# @lc code=end

