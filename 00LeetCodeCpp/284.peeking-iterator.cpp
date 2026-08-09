/*
 * @lc app=leetcode id=284 lang=cpp
 *
 * [284] Peeking Iterator
 */

// @lc code=start
/*
 * Below is the interface for Iterator, which is already defined for you.
 * **DO NOT** modify the interface for Iterator.
 *
 *  class Iterator {
 *		struct Data;
 * 		Data* data;
 *  public:
 *		Iterator(const vector<int>& nums);
 * 		Iterator(const Iterator& iter);
 *
 * 		// Returns the next element in the iteration.
 *		int next();
 *
 *		// Returns true if the iteration has more elements.
 *		bool hasNext() const;
 *	};
 */

class PeekingIterator : public Iterator {
public:
	PeekingIterator(const vector<int>& nums) : Iterator(nums) {
	    // Initialize any member here.
	    // **DO NOT** save a copy of nums and manipulate it directly.
	    // You should only use the Iterator interface methods.
	    // 在构造时就进行第一次预读
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) {
            nextVal = Iterator::next();
        } else {
            nextVal = 0; // 默认值
        }
	}
	
    // Returns the next element in the iteration without advancing the iterator.
	int peek() {
        return nextVal;
	}
	
	// hasNext() and next() should behave the same as in the Iterator interface.
	// Override them if needed.
	int next() {
        int current = nextVal; // 保存当前要返回的值
        
        // 提前预读下一个元素，更新缓存
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) {
            nextVal = Iterator::next();
        }
        
        return current;
	}
	
	bool hasNext() const {
        return hasNextVal;
	}
private:
    int nextVal;       // 缓存下一个元素
    bool hasNextVal;   // 标记缓存中是否有有效值
};
// @lc code=end

