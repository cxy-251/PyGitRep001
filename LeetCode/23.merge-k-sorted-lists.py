#
# @lc app=leetcode id=23 lang=python3
#
# [23] Merge k Sorted Lists
#
# https://leetcode.com/problems/merge-k-sorted-lists/description/
#
# algorithms
# Hard (56.17%)
# Likes:    20262
# Dislikes: 751
# Total Accepted:    2.4M
# Total Submissions: 4.3M
# Testcase Example:  '[[1,4,5],[1,3,4],[2,6]]'
#
# You are given an array of k linked-lists lists, each linked-list is sorted in
# ascending order.
# 
# Merge all the linked-lists into one sorted linked-list and return it.
# 
# 
# Example 1:
# 
# 
# Input: lists = [[1,4,5],[1,3,4],[2,6]]
# Output: [1,1,2,3,4,4,5,6]
# Explanation: The linked-lists are:
# [
# ⁠ 1->4->5,
# ⁠ 1->3->4,
# ⁠ 2->6
# ]
# merging them into one sorted list:
# 1->1->2->3->4->4->5->6
# 
# 
# Example 2:
# 
# 
# Input: lists = []
# Output: []
# 
# 
# Example 3:
# 
# 
# Input: lists = [[]]
# Output: []
# 
# 
# 
# Constraints:
# 
# 
# k == lists.length
# 0 <= k <= 10^4
# 0 <= lists[i].length <= 500
# -10^4 <= lists[i][j] <= 10^4
# lists[i] is sorted in ascending order.
# The sum of lists[i].length will not exceed 10^4.
# 
# 
#
from heapq import heappush, heappop
from typing import List, Optional
# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def mergeKLists(self, lists: List[Optional[ListNode]]) -> Optional[ListNode]:
        # 定义最小堆
        heap = []

        # 将每个链表的头节点加入堆中
        for i, node in enumerate(lists):
            if node:
                heappush(heap, (node.val, i, node))

        # 创建一个哑节点，方便操作
        dummy = ListNode(-1)
        current = dummy

        # 从堆中取出最小值，构建结果链表
        while heap:
            val, i, node = heappop(heap)
            current.next = node
            current = current.next
            if node.next:
                heappush(heap, (node.next.val, i, node.next))

        return dummy.next
# @lc code=end

