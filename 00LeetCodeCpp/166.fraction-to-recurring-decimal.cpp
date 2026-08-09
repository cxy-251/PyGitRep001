/*
 * @lc app=leetcode id=166 lang=cpp
 *
 * [166] Fraction to Recurring Decimal
 */

// @lc code=start
class Solution {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";
        string res = "";
        if ((numerator < 0) ^ (denominator < 0)) {
            res += "-";
        }
        long long n = abs((long long)numerator);
        long long d = abs((long long)denominator);
        res += to_string(n / d);
        long long remainder = n % d;
        if (remainder == 0) return res;
        res += ".";
        unordered_map<long long, int> mp;
        while (remainder != 0) {
            if (mp.find(remainder) != mp.end()) {
                res.insert(mp[remainder], "(");
                res += ")";
                break;
            }
            mp[remainder] = res.size();
            remainder *= 10;
            res += to_string(remainder / d);
            remainder %= d;
        }
        return res;
    }
};
// @lc code=end

