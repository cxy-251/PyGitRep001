/*
 * @lc app=leetcode id=92 lang=cpp
 *
 * [92] Reverse Linked List II
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode* dummy = new ListNode(0, head);
        ListNode* pre = dummy;
        for (int i = 0; i < left - 1; ++i) {
            pre = pre->next;
        }
        ListNode* cur = pre->next;
        for (int i = 0; i < right - left; ++i) {
            ListNode* nextNode = cur->next;
            cur->next = nextNode->next;
            nextNode->next = pre->next;
            pre->next = nextNode;
        }
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }
};
// @lc code=end

