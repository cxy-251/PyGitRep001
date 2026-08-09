/*
 * @lc app=leetcode id=295 lang=cpp
 *
 * [295] Find Median from Data Stream
 */

// @lc code=start
class MedianFinder {
public:
    MedianFinder() {
        
    }
    
    void addNum(int num) {
        // 步骤 1: 先将数字放入最大堆
        maxHeap.push(num);
        
        // 步骤 2: 将最大堆的顶端元素（较小部分的上限）转移到最小堆
        // 这样可以保证 minHeap 里的所有元素都大于 maxHeap 里的所有元素
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        // 步骤 3: 维持平衡。我们约定 maxHeap 的元素个数可以比 minHeap 多 1 个，但不能少
        if (maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            // 如果总数是奇数，maxHeap 顶端就是中位数
            return maxHeap.top();
        } else {
            // 如果总数是偶数，中位数是两个堆顶的平均值
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
private:
    // maxHeap 存储较小的一半数字，堆顶是较小部分的最大的数
    priority_queue<int> maxHeap; 
    // minHeap 存储较大的一半数字，堆顶是较大部分的最小的数
    priority_queue<int, vector<int>, greater<int>> minHeap; 
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */
// @lc code=end

