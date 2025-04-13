#
# @lc app=leetcode id=2 lang=python3
#
# [2] Add Two Numbers
#
# https://leetcode.com/problems/add-two-numbers/description/
#
# algorithms
# Medium (45.73%)
# Likes:    33243
# Dislikes: 6681
# Total Accepted:    5.6M
# Total Submissions: 12.3M
# Testcase Example:  '[2,4,3]\n[5,6,4]'
#
# You are given two non-empty linked lists representing two non-negative
# integers. The digits are stored in reverse order, and each of their nodes
# contains a single digit. Add the two numbers and return the sum as a linked
# list.
# 
# You may assume the two numbers do not contain any leading zero, except the
# number 0 itself.
# 
# 
# Example 1:
# 
# 
# Input: l1 = [2,4,3], l2 = [5,6,4]
# Output: [7,0,8]
# Explanation: 342 + 465 = 807.
# 
# 
# Example 2:
# 
# 
# Input: l1 = [0], l2 = [0]
# Output: [0]
# 
# 
# Example 3:
# 
# 
# Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
# Output: [8,9,9,9,0,0,0,1]
# 
# 
# 
# Constraints:
# 
# 
# The number of nodes in each linked list is in the range [1, 100].
# 0 <= Node.val <= 9
# It is guaranteed that the list represents a number that does not have leading
# zeros.
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
    def addTwoNumbers(self, l1: Optional[ListNode], l2: Optional[ListNode]) -> Optional[ListNode]:
        # 1
        # head = ListNode(0)
        # cur = head
        # tp = 0
        # while l1 or l2:
        #     vl1 = l1.val if l1 else 0
        #     vl2 = l2.val if l2 else 0
        #     vl3 = vl1 + vl2 + tp
        #     tp = vl3 // 10
        #     vl3 = vl3 % 10
        #     cur.next = ListNode(vl3)
        #     cur = cur.next
        #     if l1:
        #         l1 = l1.next
        #     if l2:
        #         l2 = l2.next
        # if tp:
        #     cur.next = ListNode(tp)
        # return head.next
        # failed
        # while l1.next or l2.next:
        #     if l1.next and l2.next:
        #         l3.val = l1.val + l2.val + tp
        #         tp = l3.val // 10
        #         l3.val = l3.val % 10
        #         l1 = l1.next
        #         l2 = l2.next
        #         l3 = l3.next
        #         continue
        #     elif l1.next:
        #         l3.val = l1.val + tp
        #         tp = l3.val // 10
        #         l3.val = l3.val % 10
        #         l1 = l1.next
        #         l3 = l3.next
        #         continue
        #     elif l2.next:
        #         l3.val = l2.val + tp
        #         tp = l3.val // 10
        #         l3.val = l3.val % 10
        #         l2 = l2.next
        #         l3 = l3.next
        #         continue
        # if tp:
        #     l3.next = ListNode(tp)
        
        # lst = []
        # while cur.next:
        #     lst.append(cur.val)
        #     cur = cur.next

        # return cur.next
    
        # 2
        # dummy = ListNode(0)  # 哑节点，方便构造结果链表
        # current = dummy  # 当前指针
        # carry = 0  # 进位

        # # 遍历两个链表，直到两个链表都为空
        # while l1 or l2 or carry:
        #     val1 = l1.val if l1 else 0  # 获取 l1 当前节点的值
        #     val2 = l2.val if l2 else 0  # 获取 l2 当前节点的值

        #     # 计算当前位的和以及进位
        #     total = val1 + val2 + carry
        #     carry = total // 10
        #     current.next = ListNode(total % 10)  # 创建新节点存储当前位的值
        #     current = current.next  # 移动指针

        #     # 移动 l1 和 l2 的指针
        #     if l1:
        #         l1 = l1.next
        #     if l2:
        #         l2 = l2.next

        # return dummy.next  # 返回结果链表的头节点

        head = ListNode(0)
        cur = head
        carry = 0
        while l1 or l2 or carry:
            val1, l1 = (l1.val, l1.next) if l1 else (0, None)
            val2, l2 = (l2.val, l2.next) if l2 else (0, None)
            total = val1 + val2 + carry
            carry = total // 10
            cur.next = ListNode(total % 10)
            cur = cur.next
        return head.next
# @lc code=end

