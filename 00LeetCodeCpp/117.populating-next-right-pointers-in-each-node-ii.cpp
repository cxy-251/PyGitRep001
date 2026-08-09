/*
 * @lc app=leetcode id=117 lang=cpp
 *
 * [117] Populating Next Right Pointers in Each Node II
 */

// @lc code=start
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        Node* curr_level = root;
        while (curr_level) {
            Node* dummy = new Node(0);
            Node* tail = dummy;
            Node* iter = curr_level;
            while (iter) {
                if (iter->left) {
                    tail->next = iter->left;
                    tail = tail->next;
                }
                if (iter->right) {
                    tail->next = iter->right;
                    tail = tail->next;
                }
                iter = iter->next;
            }
            curr_level = dummy->next;
        }
        return root;
        // if (!root) return nullptr;
        // queue<Node*> q;
        // q.push(root);
        // while (!q.empty()) {
        //     int size = q.size();
        //     Node* prev = nullptr;
        //     for (int i = 0; i < size; ++i) {
        //         Node* curr = q.front();
        //         q.pop();
        //         if (prev != nullptr) {
        //             prev->next = curr;
        //         }
        //         prev = curr;
        //         if (curr->left) q.push(curr->left);
        //         if (curr->right) q.push(curr->right);
        //     }
        //     prev->next = nullptr;
        // }
        // return root;
    }
};
// @lc code=end

