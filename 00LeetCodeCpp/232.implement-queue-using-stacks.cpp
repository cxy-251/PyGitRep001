/*
 * @lc app=leetcode id=232 lang=cpp
 *
 * [232] Implement Queue using Stacks
 */

// @lc code=start
class MyQueue {
public:
    MyQueue() {
        
    }
    
    void push(int x) {
        st_in.push(x);
    }
    
    int pop() {
        transfer();
        int val = st_out.top();
        st_out.pop();
        return val;
    }
    
    int peek() {
        transfer();
        return st_out.top();
    }
    
    bool empty() {
        return st_in.empty() && st_out.empty();
    }
private:
    stack<int> st_in;
    stack<int> st_out;
    void transfer() {
        if (st_out.empty()) {
            while (!st_in.empty()) {
                st_out.push(st_in.top());
                st_in.pop();
            }
        }
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */
// @lc code=end

