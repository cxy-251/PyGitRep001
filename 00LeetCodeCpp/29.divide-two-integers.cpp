/*
 * @lc app=leetcode id=29 lang=cpp
 *
 * [29] Divide Two Integers
 */

// @lc code=start
class Solution {
public:
    int divide(int dividend, int divisor) {
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        bool negative = (dividend > 0) ^ (divisor > 0);
        // long a = (long)dividend;
        // if (a < 0) a = -a;
        // long b = (long)divisor;
        // if (b < 0) b = -b;
        long a = (dividend < 0) ? -(long)dividend : (long)dividend;
        long b = (divisor < 0) ? -(long)divisor : (long)divisor;
        // int result = 0;
        long result = 0;
        for (int i = 31; i >= 0; i--) {
            if ((a >> i) >= b) {
                result += (1U << i);
                a -= (b << i);
            }
        }
        return negative ? (int)-result : (int)result;


        // // 1. 唯一会溢出的情况：-2147483648 / -1
        // if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        // // if (dividend == INT_MIN && divisor == 1) return INT_MIN;
        // if (dividend == 0) return 0; // 被除数为 0 直接返回

        // // 2. 记录最终结果的符号
        // bool negative = (dividend > 0) ^ (divisor > 0);

        // // 3. 全部转为负数（关键！）
        // // 即使是 INT_MIN，转负数后还是它自己，不会溢出
        // if (dividend > 0) dividend = -dividend;
        // if (divisor > 0) divisor = -divisor;

        // // int result = 0;
        // // unsigned int result = 0;
        // // long long result = 0;
        // long result = 0;

        // // 4. 倍增法（在负数世界里，比较符号是反着的）
        // for (int i = 31; i >= 0; i--) {
        //     // 在负数世界里，a >> i >= b 的意思是：
        //     // a 的绝对值足够大，能包住 (divisor * 2^i)
        //     // 注意：因为是负数，-10 <= -3 表示 -10 的绝对值更大
        //     if ((long)dividend >> i <= (long)divisor) { // 这里的逻辑对应 a/2^n >= b
        //         // 这里的判断其实是在模拟：dividend <= (divisor << i)
        //         // 只是为了防溢出，我们写成了位移形式
        //         if ((long)dividend <= ((long)divisor << i)) {
        //             //  result += (1U << i);
        //              result += (1L << i);
        //              dividend -= (divisor << i);
        //         }
        //     }
        // }
        // // if (result == 2147483648U) return -2147483648;
        // // if (result == 2147483648U) {
        // //     // 如果结果是 2147483648 且是负数，直接返回 INT_MIN
        // //     // 如果是正数，前面第 1 步已经处理了溢出
        // //     return negative ? INT_MIN : INT_MAX; 
        // // }
        // // return negative ? -(int)result : (int)result;

        // // long long final_res = negative ? -result : result;
        // long final_res = negative ? -result : result;

        // // 6. 最后根据 int 范围卡死结果
        // // if (final_res > INT_MAX) return INT_MAX;
        // // if (final_res < INT_MIN) return INT_MIN;
        // return (int)final_res;






        // // 1. 特殊情况：唯一会爆 INT_MAX 的情况
        // if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        // // 2. 确定符号
        // bool negative = (dividend > 0) ^ (divisor > 0);
        
        // // 3. 全员转负数（为了装下 INT_MIN，且计算过程不溢出）
        // // 使用 long 只是为了转换那一瞬间的绝对安全
        // long a = (dividend < 0) ? (long)dividend : -(long)dividend;
        // long b = (divisor < 0) ? (long)divisor : -(long)divisor;
        
        // // 4. 造梯子：把 divisor 翻倍的结果存起来
        // // 我们存的是：[b*1, b*2, b*4, b*8, ...] 且都是负数
        // vector<long> candidates = {b};
        // vector<long> counts = {1};
        
        // // 只要最后一个倍数还没超过 a（绝对值还没比 a 大），就继续翻倍
        // while (candidates.back() >= a - candidates.back()) { 
        //     candidates.push_back(candidates.back() + candidates.back());
        //     counts.push_back(counts.back() + counts.back());
        // }

        // long res = 0;
        // // 5. 从梯子最高处往下减（贪心）
        // for (int i = candidates.size() - 1; i >= 0; i--) {
        //     if (a <= candidates[i]) { // 负数中 a <= candidates 代表 a 的绝对值更大
        //         a -= candidates[i];
        //         res += counts[i];
        //     }
        // }
        
        // // 6. 还原符号
        // return negative ? (int)-res : (int)res;
    }
};
// @lc code=end

