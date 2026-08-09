/*
 * @lc app=leetcode id=299 lang=cpp
 *
 * [299] Bulls and Cows
 */

// @lc code=start
class Solution {
public:
    string getHint(string secret, string guess) {
        int bulls = 0;
        int cows = 0;
        
        // 使用数组记录数字 0-9 出现的次数
        // secretCount: 记录 secret 中未匹配成 bull 的数字频率
        // guessCount: 记录 guess 中未匹配成 bull 的数字频率
        vector<int> secretCount(10, 0);
        vector<int> guessCount(10, 0);
        
        for (int i = 0; i < secret.length(); ++i) {
            if (secret[i] == guess[i]) {
                // 位置和数字都对，是 Bull
                bulls++;
            } else {
                // 位置不对，先记录数字出现的次数，后面用来算 Cow
                secretCount[secret[i] - '0']++;
                guessCount[guess[i] - '0']++;
            }
        }
        
        // 计算 Cow: 对于每一个数字 0-9，
        // 在 secret 和 guess 中都出现且没被算作 bull 的部分，
        // 取两者的最小值即为该数字能贡献的 cow 数量。
        for (int i = 0; i < 10; ++i) {
            cows += min(secretCount[i], guessCount[i]);
        }
        
        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};
// @lc code=end

