// 执行命令：chmod +x 0collection1-301.cpp && ./0collection1-301.cpp
#if 0
#这一段是给 shell 读的
g++ -std=c++17 "$0" -o /tmp/leetcode_test && /tmp/leetcode_test && exit
#endif

#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <regex>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <numeric>
#include <list>
#include <set>
using namespace std;

// 公共工具命名空间
namespace Utils {
    // 这里的 ListNode 定义需要根据题目要求统一，或者使用全局定义
    // 假设我们使用你之前定义的 ListNode 结构
    template <typename T>
    auto createList(const vector<int>& arr) {
        if (arr.empty()) return (T*)nullptr;
        T* dummy = new T(0);
        T* curr = dummy;
        for (int x : arr) {
            curr->next = new T(x);
            curr = curr->next;
        }
        T* head = dummy->next;
        delete dummy;
        return head;
    }

    template <typename T>
    void printList(T* head) {
        T* curr = head;
        while (curr) {
            cout << curr->val << (curr->next ? " -> " : "");
            curr = curr->next;
        }
        cout << " (null)" << endl;
    }

    template <typename T>
    void freeList(T* head) {
        while (head) {
            T* temp = head;
            head = head->next;
            delete temp;
        }
    }

    template <typename T>
    void printVec(const vector<T>& v, bool newline = true) {
        cout << "[";
        for (int i = 0; i < v.size(); ++i) {
            cout << v[i] << (i == v.size() - 1 ? "" : ", ");
        }
        cout << "]";
        if (newline) cout << endl;
    }

    template <typename T>
    void printVec2(const vector<vector<T>>& v) {
        cout << "[";
        for (size_t i = 0; i < v.size(); ++i) {
            printVec(v[i], i != v.size() - 1);
        }
        cout << "]" << endl;
    }
} // namespace Utils

// --- 第 1 题：两数之和 ---
namespace Question1
{
    class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            unordered_map<int, int> num_to_index;
            for (int i = 0; i < nums.size(); ++i) {
                int tmp = target - nums[i];
                auto it = num_to_index.find(tmp);
                if (it != num_to_index.end()) {
                    return {it->second, i};
                }
                num_to_index[nums[i]] = i;
            }
            return {};
        }
    };

    template <typename T>
    void printVec(const vector<T>& v) {
        cout << "[";
        for (int i = 0; i < v.size(); ++i) {
            cout << v[i] << (i == v.size() - 1 ? "" : ", ");
        }
        cout << "]" << endl;
    }

    void test() {
        Solution sol;
        vector<int> nums = {2, 7, 11, 15};
        int target = 18;

        vector<int> result = sol.twoSum(nums, target);

        printVec(result);
    }
} // namespace Question01

// --- 第 2 题 ---
namespace Question2
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    
    class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            ListNode dummy(0);
            ListNode* cur = &dummy;
            int carry = 0;
            while (l1 != nullptr || l2 != nullptr || carry) {
                int sum = carry + (l1 ? l1->val : 0) + (l2 ? l2->val : 0);
                carry = sum / 10;
                cur->next = new ListNode(sum % 10);
                cur = cur->next;
                if (l1) l1 = l1->next;
                if (l2) l2 = l2->next;
            }
            return dummy.next;
        }
    };

    ListNode* createList(vector<int> nums) {
        ListNode dummy;
        ListNode* cur = &dummy;
        for (int x : nums) {
            cur->next = new ListNode(x);
            cur = cur->next;
        }
        return dummy.next;
    }

    void printList(ListNode* head) {
        while (head) {
            cout << head->val << (head->next ? " -> " : "");
            head = head->next;
        }
        cout << endl;
    }

    void test() {
        Solution sol;
        ListNode* l1 = createList({2, 4, 3});
        ListNode* l2 = createList({5, 6, 4});

        ListNode* res = sol.addTwoNumbers(l1, l2);
        printList(l1);
        printList(l2);
        printList(res);
    }
} // namespace Question2

// --- 第 3 题 ---
namespace Question3
{
    class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
            int res = 0;
            int left = 0;
            int last_pos[128] = {0};
            for (int right = 0; right < (int)s.length(); right++) {
                left = max(left, last_pos[s[right]]);
                res = max(res, right - left + 1);
                last_pos[s[right]] = right + 1;
            }
            return res;
        }
    };

    void test() {
        Solution sol;
        printf("%d\n" ,sol.lengthOfLongestSubstring("abcdac"));
    }
} // namespace Question3

// --- 第 4 题 ---
namespace Question4
{
    class Solution {
    public:
        double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
            int n = nums1.size();
            int m = nums2.size();

            int left = (n + m + 1) / 2;  // 左中位数位置
            int right = (n + m + 2) / 2; // 右中位数位置
            
            // 技巧：如果是奇数，left == right，逻辑统一；如果是偶数，求两个位置的平均值
            return (getKth(nums1, 0, n - 1, nums2, 0, m - 1, left) + 
                    getKth(nums1, 0, n - 1, nums2, 0, m - 1, right)) * 0.5;
        }

    private:
        // 在两个数组的指定区间内，寻找第 k 小的数
        int getKth(vector<int>& nums1, int start1, int end1, vector<int>& nums2, int start2, int end2, int k) {
            int len1 = end1 - start1 + 1;
            int len2 = end2 - start2 + 1;

            // 始终让 nums1 成为较短的数组，方便处理越界情况
            if (len1 > len2) return getKth(nums2, start2, end2, nums1, start1, end1, k);
            
            // 边界情况 1：短数组已经排空了，直接去长数组找
            if (len1 == 0) return nums2[start2 + k - 1];
            
            // 边界情况 2：找第 1 小的数，直接比两个数组的头，谁小就是谁
            if (k == 1) return min(nums1[start1], nums2[start2]);

            // 核心逻辑：二分，各自取第 k/2 个数进行比较
            // 注意：为了防止短数组长度不足 k/2，取 min(len1, k/2)
            int i = start1 + min(len1, k / 2) - 1;
            int j = start2 + min(len2, k / 2) - 1;

            if (nums1[i] > nums2[j]) {
                // nums2 的前半部分（j 之前）绝对不可能是第 k 小，砍掉！
                // 更新 k：减去被砍掉的长度
                return getKth(nums1, start1, end1, nums2, j + 1, end2, k - (j - start2 + 1));
            } else {
                // nums1 的前半部分（i 之前）绝对不可能是第 k 小，砍掉！
                return getKth(nums1, i + 1, end1, nums2, start2, end2, k - (i - start1 + 1));
            }
        }
    };

    void test() {
        Solution sol;
        vector<int> nums1 = {2, 7, 11, 15};
        vector<int> nums2 = {2, 7, 10, 15};
        std::cout << sol.findMedianSortedArrays(nums1, nums2) << std::endl;
    }
} // namespace Question4

// --- 第 5 题 ---
namespace Question5
{
    class Solution {
    public:
        int expandAroundCenter(const string& s, int left, int right) {
            while (left >= 0 && right < s.length() && s[left] == s[right]) {
                left--;
                right++;
            }
            return right - left - 1;

        }
        string longestPalindrome(string s) {
            // if (s.length() < 2) return s;
            // int start = 0, maxLen = 0;

            // for (int idx = 0; idx < s.length(); idx++) {
            //     if (s.length() - idx <= maxLen / 2) break;
            //     int curLen = max(expandAroundCenter(s, idx, idx),
            //                      expandAroundCenter(s, idx, idx + 1));

            //     if (curLen > maxLen) {
            //         maxLen = curLen;
            //         start = idx - (curLen - 1) / 2;
            //     }
            // }
            int n = s.length();
            if (n < 2) return s;
            
            int start = 0, maxLen = 1;
            for (int i = 0; i < n; ) {
                // 剪枝：剩余字符不够长了，直接收工
                if (n - i <= maxLen / 2) break;

                int left = i, right = i;
                // 技巧：跳过中间所有重复字符（如 "baaaaab" 中的 "aaaaa"）
                while (right < n - 1 && s[right + 1] == s[right]) {
                    right++;
                }
                
                // 下一次循环直接从重复字符后面开始，大大减少循环次数
                i = right + 1;

                // 从这个“中心块”向两边扩散
                while (left > 0 && right < n - 1 && s[left - 1] == s[right + 1]) {
                    left--;
                    right++;
                }

                int curLen = right - left + 1;
                if (curLen > maxLen) {
                    start = left;
                    maxLen = curLen;
                }
            }
            return s.substr(start, maxLen);
        }
    };

    void test() {
        Solution sol;
        std::cout << sol.longestPalindrome("aaababbba") << std::endl;
    }
} // namespace Question5

// --- 第 6 题 ---
namespace Question6
{
    class Solution {
    public:
        string convert(string s, int numRows) {
            if (numRows < 2 || s.length() <= numRows) return s;

            vector<string> rows(numRows);
            int curRow = 0;
            int direction = -1;

            for (char c : s) {
                rows[curRow] += c;

                if (curRow == 0 || curRow == numRows - 1) {
                    direction = -direction;
                }

                curRow += direction;
            }

            string res;
            res.reserve(s.length()); // 避免动态扩容搬家的时间
            for (const string& row : rows) {
                res += row;
            }
            return res;
        }
    };

    void test() {
        Solution sol;
        cout << sol.convert("test", 2) << endl;
    }
} // namespace Question6

// --- 第 7 题 ---
namespace Question7
{
    class Solution {
    public:
        int reverse(int x) {
            int res = 0;
            while (x != 0) {
                int pop = x % 10;
                x /= 10;
                // INT_MAX 是 2147483647，INT_MIN 是 -2147483648
                if (res > INT_MAX / 10 || (res == INT_MAX / 10 && pop > 7)) return 0;
                if (res < INT_MIN / 10 || (res == INT_MIN / 10 && pop < -8)) return 0;

                res = res * 10 + pop;
            }
            return res;
        }
    };

    void test() {
        Solution sol;
        cout << sol.reverse(INT_MAX) << endl;
    }
} // namespace Question7

// --- 第 8 题 ---
namespace Question8
{
    class Automaton {
        // 状态转换表：横轴代表 [空格, 符号, 数字, 其他]
        // 纵轴代表 [start, signed, in_number, end]
        unordered_map<string, vector<string>> table = {
            {"start",     {"start", "signed", "in_number", "end"}},
            {"signed",    {"end",   "end",    "in_number", "end"}},
            {"in_number", {"end",   "end",    "in_number", "end"}},
            {"end",       {"end",   "end",    "end",       "end"}}
        };

        string state = "start";
        int get_col(char c) {
            if (isspace(c)) return 0;
            if (c == '+' || c == '-') return 1;
            if (isdigit(c)) return 2;
            return 3;
        }
    public:
        int sign = 1;
        long long ans = 0;
        void get(char c) {
            state = table[state][get_col(c)];
            if (state == "in_number") {
                ans = ans * 10 + c - '0';
                if (sign == 1) {
                    if (ans > INT_MAX) ans = INT_MAX;
                } else {
                    if (ans > (long long)INT_MAX + 1) ans = (long long)INT_MAX;
                }
            } else if (state == "signed") {
                sign = (c == '+') ? 1 : -1;
            }
        }
        bool isEnd() { return state == "end"; }
    };

    class Solution {
    public:
        int myAtoi(string s) {
            Automaton automaton;
            for (char c : s) {
                automaton.get(c);
                if (automaton.isEnd()) break;
            }
            return (int)(automaton.sign * automaton.ans);

            // int i = 0, n = s.length();
            // while (i < n && s[i] == ' ') i++;
            // if (i == n) return 0;

            // int sign = 1;
            // if (s[i] == '+' || s[i] == '-') {
            //     if (s[i] == '-') sign = -1;
            //     i++;
            // }

            // int res = 0;
            // while (i < n && isdigit(s[i])) {
            //     int digit = s[i] - '0';
            //     if (res > INT_MAX / 10 || (res == INT_MAX / 10 && digit > 7)) {
            //         return (sign == 1) ? INT_MAX : INT_MIN;
            //     }

            //     res = res * 10 + digit;
            //     i++;
            // }
            // return res * sign;
        }
    };

    void test() {
        Solution sol;
        cout << sol.myAtoi("1234") << endl;
    }
} // namespace Question8

// --- 第 9 题 ---
namespace Question9
{
    class Solution {
    public:
        bool isPalindrome(int x) {
            if (x < 0) return false;
            if (x % 10 == 0 && x != 0) return false;

            int reverted = 0;
            while (x > reverted) {
                reverted = reverted * 10 + x % 10;
                x /= 10;
            }
            return x == reverted || x == reverted / 10;
        }
    };

    void test() {
        Solution sol;
        cout << (sol.isPalindrome(1234) ? "true" : "false") << endl;
    }
} // namespace Question9

// --- 第 10 题 ---
namespace Question10
{
    class Solution {
    public:
        bool isMatch(string s, string p) {
            int m = s.length(), n = p.length();
            vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
            dp[0][0] = true;
            for (int j = 2; j <= n; j++) {
                if (p[j - 1] == '*') {
                    dp[0][j] = dp[0][j - 2];
                }
            }
            for (int i = 1; i <= m; i++) {
                for (int j = 1; j <= n; j++) {
                    if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                        dp[i][j] = dp[i - 1][j - 1];
                    } else if (p[j - 1] == '*') {
                        dp[i][j] = dp[i][j - 2];
                        if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                            dp[i][j] = dp[i][j] || dp[i - 1][j];
                        }
                    }
                }
            }
            return dp[m][n];
        }
    };

    void test() {
        Solution sol;
        cout << (sol.isMatch("aaaa", "a*") ? "true" : "false") << endl;
    }
} // namespace Question10

// --- 第 11 题 ---
namespace Question11
{
    class Solution {
    public:
        int maxArea(vector<int>& height) {
            int left = 0;
            int right = height.size() - 1;
            int max_area = 0;
            while (left < right) {
                int current_height = min(height[left], height[right]);
                int current_area = (right - left) * current_height;
                max_area = max(max_area, current_area);

                if (height[left] < height[right]) {
                    left++;
                } else {
                    right--;
                }
            }
            return max_area;
        }
    };

    void test() {
        Solution sol;
        vector<int> a = {1,2,3,6,8,6,3,2,1};
        cout << sol.maxArea(a) << endl;
    }
} // namespace Question11

// --- 第 12 题 ---
namespace Question12
{
    class Solution {
    public:
        string intToRoman(int num) {
            pair<int, string> mapping[] = {
                {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
                {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
                {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"},
                {1, "I"}
            };
            string res = "";
            for (const auto& [value, symbol] : mapping) {
                while (num >= value) {
                    res += symbol;
                    num -= value;
                }
                if (num == 0) break;
            }
            return res;
        }
    };

    void test() {
        Solution sol;
        cout << sol.intToRoman(29) << endl;
    }
} // namespace Question12

// --- 第 13 题 ---
namespace Question13
{
    class Solution {
    public:
        int romanToInt(string s) {
            auto val = [](char c)->int {
                switch (c) {
                    case 'I': return 1;
                    case 'V': return 5;
                    case 'X': return 10;
                    case 'L': return 50;
                    case 'C': return 100;
                    case 'D': return 500;
                    case 'M': return 1000;
                    default: return 0;
                }
            };
            int n = s.size();
            int res = 0;
            for (int i = 0; i < n; ++i) {
                int v = val(s[i]);
                if (i + 1 < n && val(s[i + 1]) > v) res -= v;
                else res += v;
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        cout << sol.romanToInt("XXVILCDM") << endl;
    }
} // namespace Question13

// --- 第 14 题 ---
namespace Question14
{
    class Solution {
    public:
        string longestCommonPrefix(vector<string>& strs) {
            if (strs.empty()) return "";
            string prefix = strs[0];
            for (size_t i = 1; i < strs.size(); ++i) {
                size_t j = 0;
                while (j < prefix.size() && j < strs[i].size() && prefix[j] == strs[i][j]) {
                    ++j;
                }
                prefix.resize(j);
                if (prefix.empty()) break;
            }
            return prefix;
        }
    };
    void test() {
        Solution sol;
        vector<string> strs = {"abc", "acd", "adc"};
        cout << sol.longestCommonPrefix(strs) << endl;
    }
} // namespace Question14

// --- 第 15 题 ---
namespace Question15
{
    class Solution {
    public:
        vector<vector<int>> threeSum(vector<int>& nums) {
            int n = nums.size();
            sort(nums.begin(), nums.end());
            vector<vector<int>> res;
            for (int i = 0; i < n - 2; ++i) {
                if (nums[i] > 0) break;
                if (i > 0 && nums[i] == nums[i - 1]) continue;
                int left = i + 1;
                int right = n - 1;
                while (left < right) {
                    int sum = nums[i] + nums[left] + nums[right];
                    if (sum < 0) {
                        left++;
                    } else if (sum > 0) {
                        right--;
                    } else {
                        res.push_back({nums[i], nums[left], nums[right]});
                        while (left < right && nums[left] == nums[left + 1]) left++;
                        while (left < right && nums[right] == nums[right - 1]) right--;
                        left++;
                        right--;
                    }
                }
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> cases = {
            {-1, 0, 1, 2, -1, -4}, // 标准情况
            {0, 1, 1},             // 无解情况
            {0, 0, 0, 0}           // 全零情况
        };

        for (auto& nums : cases) {
            // 打印输入
            cout << "Input: [";
            for (int i = 0; i < nums.size(); ++i) {
                cout << nums[i] << (i == nums.size() - 1 ? "" : ",");
            }
            cout << "]" << endl;

            // 获取结果
            vector<vector<int>> result = sol.threeSum(nums);

            // 打印输出（嵌套遍历）
            cout << "Output: [";
            for (int i = 0; i < result.size(); ++i) {
                cout << "[";
                for (int j = 0; j < result[i].size(); ++j) {
                    cout << result[i][j] << (j == result[i].size() - 1 ? "" : ",");
                }
                cout << "]" << (i == result.size() - 1 ? "" : ", ");
            }
            cout << "]" << endl << "-------------------" << endl;
        }
    }
} // namespace Question15

// --- 第 16 题 ---
namespace Question16
{
    class Solution {
    public:
        int threeSumClosest(vector<int>& nums, int target) {
            int n = nums.size();
            sort(nums.begin(), nums.end());
            int closestSum = nums[0] + nums[1] + nums[2];
            for (int i = 0; i < n - 2; ++i) {
                if (i > 0 && nums[i] == nums[i - 1]) continue;
                int left = i + 1;
                int right = n - 1;
                while (left < right) {
                    int currentSum = nums[i] + nums[left] + nums[right];
                    if (currentSum == target) return target;
                    if (abs(currentSum - target) < abs(closestSum - target)) {
                        closestSum = currentSum;
                    }
                    if (currentSum < target) {
                        left++;
                    } else {
                        right--;
                    }
                }
            }
            return closestSum;
        }
    };
    void test() {
        Solution sol;
        vector<int> nums = {1, 2, 4, 6, 4, 3, 2};
        cout << sol.threeSumClosest(nums, 3) << endl;
    }
} // namespace Question16

// --- 第 17 题 ---
namespace Question17
{
    class Solution {
    private:
        // 数字到字母的映射数组，下标 0 和 1 留空
        const vector<string> mapping = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };
    public:
        vector<string> letterCombinations(string digits) {
            if (digits.empty()) return {};
            vector<string> res;
            string current = "";
            backtrack(res, digits, current, 0);
            return res;
        }

        void backtrack(vector<string>& res, const string& digits, string& current, int index) {
            if (index == digits.size()) {
                res.push_back(current);
                return;
            }
            string letters = mapping[digits[index] - '0'];
            for (char c : letters) {
                current.push_back(c);
                backtrack(res, digits, current, index + 1);
                current.pop_back();
            }
        }
    };

    void test() {
        Solution sol;
        vector<string> inputs = {"23", "", "2"};

        for (const string& digits : inputs) {
            cout << "Input digits: \"" << digits << "\"" << endl;
            
            vector<string> result = sol.letterCombinations(digits);
            
            cout << "Combinations (" << result.size() << "): [";
            for (int i = 0; i < result.size(); ++i) {
                cout << "\"" << result[i] << "\"" << (i == result.size() - 1 ? "" : ", ");
            }
            cout << "]" << endl;
            cout << "--------------------------" << endl;
        }
    }
} // namespace Question17

// --- 第 18 题 ---
namespace Question18
{
    class Solution {
    public:
        vector<vector<int>> fourSum(vector<int>& nums, int target) {
            sort(nums.begin(), nums.end());
            return nSumTarget(nums, 4, 0, target);
        }

        vector<vector<int>> nSumTarget(vector<int>& nums, int n, int start, long target) {
            int sz = nums.size();
            vector<vector<int>> res;
            if (n < 2 || sz - start < n) return res;
            if (n == 2) {
                int left = start, right = sz - 1;
                while (left < right) {
                    long sum = (long)nums[left] + nums[right];
                    int val_left = nums[left], val_right = nums[right];
                    if (sum < target) {
                        while (left < right && nums[left] == val_left) left++;
                    } else if (sum > target) {
                        while (left < right && nums[right] == val_right) right--;
                    } else {
                        res.push_back({val_left, val_right});
                        while (left < right && nums[left] == val_left) left++;
                        while (left < right && nums[right] == val_right) right--;
                    }
                }
            } else {
                for (int i = start; i < sz; i++) {
                    vector<vector<int>> sub = nSumTarget(nums, n - 1, i + 1, target - nums[i]);
                    for (vector<int>& arr : sub) {
                        arr.push_back(nums[i]);
                        res.push_back(arr);
                    }
                    while (i < sz - 1 && nums[i] == nums[i + 1]) i++;
                }
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> cases = {
            {-1, 0, 1, 2, -1, -4, 4, 3},
            {0, 1, 1, 5, -1, -2, -7},            
            {0, 0, 0, 0}          
        };

        for (auto& nums : cases) {
            // 打印输入
            cout << "Input: [";
            for (int i = 0; i < nums.size(); ++i) {
                cout << nums[i] << (i == nums.size() - 1 ? "" : ",");
            }
            cout << "]" << endl;

            // 获取结果
            vector<vector<int>> result = sol.fourSum(nums, 3);

            // 打印输出（嵌套遍历）
            cout << "Output: [";
            for (int i = 0; i < result.size(); ++i) {
                cout << "[";
                for (int j = 0; j < result[i].size(); ++j) {
                    cout << result[i][j] << (j == result[i].size() - 1 ? "" : ",");
                }
                cout << "]" << (i == result.size() - 1 ? "" : ", ");
            }
            cout << "]" << endl << "-------------------" << endl;
        }
    }
} // namespace Question18

// --- 第 19 题 ---
namespace Question19
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };

    class Solution {
    public:
        ListNode* removeNthFromEnd(ListNode* head, int n) {
            ListNode* dummy = new ListNode(0, head);
            ListNode* fast = dummy;
            ListNode* slow = dummy;

            for (int i = 0; i <= n; ++i) {
                fast = fast->next;
            }

            while (fast != nullptr) {
                fast = fast->next;
                slow = slow->next;
            }

            ListNode* nodeToDelete = slow->next;
            slow->next = slow->next->next;

            delete nodeToDelete;

            ListNode* newHead = dummy->next;
            delete dummy;
            return newHead;
        }
    };

    // 辅助函数：根据数组创建链表
    ListNode* createList(vector<int> arr) {
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        for (int x : arr) {
            curr->next = new ListNode(x);
            curr = curr->next;
        }
        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }

    // 辅助函数：打印链表
    void printList(ListNode* head) {
        ListNode* curr = head;
        while (curr) {
            cout << curr->val << (curr->next ? " -> " : "");
            curr = curr->next;
        }
        cout << endl;
    }

    // 辅助函数：销毁链表释放内存
    void freeList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void test() {
        Solution sol;
        
        // 测试用例 1: 删除倒数第 2 个
        vector<int> arr1 = {1, 2, 3, 4, 5};
        int n1 = 2;
        ListNode* head1 = createList(arr1);
        cout << "Original: "; printList(head1);
        head1 = sol.removeNthFromEnd(head1, n1);
        cout << "After removing 倒数第 " << n1 << ": "; printList(head1);
        freeList(head1);
        cout << "-------------------" << endl;

        // 测试用例 2: 删除头节点 (倒数第 1 个)
        vector<int> arr2 = {1};
        int n2 = 1;
        ListNode* head2 = createList(arr2);
        cout << "Original: "; printList(head2);
        head2 = sol.removeNthFromEnd(head2, n2);
        cout << "After removing 倒数第 " << n2 << ": "; printList(head2);
        freeList(head2);
        cout << "-------------------" << endl;

        // 测试用例 3: 删除头节点 (倒数第 2 个)
        vector<int> arr3 = {1, 2};
        int n3 = 2;
        ListNode* head3 = createList(arr3);
        cout << "Original: "; printList(head3);
        head3 = sol.removeNthFromEnd(head3, n3);
        cout << "After removing 倒数第 " << n3 << ": "; printList(head3);
        freeList(head3);
    }
} // namespace Question19

// --- 第 20 题 ---
namespace Question20
{
    class Solution {
    public:
        bool isValid(string s) {
            stack<char> st;
            unordered_map<char,char> pairs = {{')','('}, {'}','{'}, {']','['}};
            for (char c : s) {
                if (pairs.count(c)) {                // 右括号：检查栈顶是否匹配
                    if (st.empty() || st.top() != pairs[c]) return false;
                    st.pop();
                } else {                             // 左括号：入栈
                    st.push(c);
                }
            }
            return st.empty();                       // 全部匹配则栈空
        }
    };
    void test() {
        Solution sol;
        cout << (sol.isValid("[](){){}()}") ? "true" : "false") << endl;
    }
} // namespace Question20

// --- 第 21 题 ---
namespace Question21
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    class Solution {
    public:
        ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
            ListNode dummy(0);
            ListNode* tail = &dummy;
            while (list1 && list2) {
                if (list1->val < list2->val) {
                    tail->next = list1;
                    list1 = list1->next;
                } else {
                    tail->next = list2;
                    list2 = list2->next;
                }
                tail = tail->next;
            }
            tail->next = list1 ? list1 : list2;
            return dummy.next;
        }
    };
    // 辅助函数：根据数组创建链表
    ListNode* createList(vector<int> arr) {
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        for (int x : arr) {
            curr->next = new ListNode(x);
            curr = curr->next;
        }
        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }

    // 辅助函数：打印链表
    void printList(ListNode* head) {
        ListNode* curr = head;
        while (curr) {
            cout << curr->val << (curr->next ? " -> " : "");
            curr = curr->next;
        }
        cout << endl;
    }

    // 辅助函数：销毁链表释放内存
    void freeList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void test() {
        Solution sol;
        
        vector<int> arr1 = {1, 2, 3, 4, 5};
        vector<int> arr2 = {1, 2, 3, 4, 5};
        ListNode* head1 = createList(arr1);
        ListNode* head2 = createList(arr2);
        ListNode* head3 = sol.mergeTwoLists(head1, head2);
        cout << "Result: "; printList(head3);
        freeList(head3);
        head1 = head2 = head3 = nullptr;

        // arr1 = {1, 3, 5};
        // arr2 = {1, 2, 4};
        // ListNode* head1 = createList(arr1);
        // ListNode* head2 = createList(arr2);
        // ListNode* head3;
        // head3 = sol.mergeTwoLists(head1, head2);
        // cout << "Original: "; printList(head1); printList(head2);
        // cout << "Result: "; printList(head3);
        // cout << "-------------------" << endl;
        // freeList(head1);
        // freeList(head2);
        // freeList(head3);
    }
} // namespace Question21

// --- 第 22 题 ---
namespace Question22
{
    class Solution {
    public:
        vector<string> generateParenthesis(int n) {
            vector<string> res;
            string current = "";
            backtrack(res, current, 0, 0, n);
            return res;
        }
    
    private:
        /**
         * left: 当前已使用的左括号数量
         * right: 当前已使用的右括号数量
         * n: 括号对数
         */
        void backtrack(vector<string>& res, string& current, int left, int right, int n) {
            if (current.size() == 2 * n) {
                res.push_back(current);
                return;
            }
            if (left < n) {
                current.push_back('(');
                backtrack(res, current, left + 1, right, n);
                current.pop_back();
            }
            if (right < left) {
                current.push_back(')');
                backtrack(res, current, left, right + 1, n);
                current.pop_back();
            }
        }
    };
    void test() {
        Solution sol;
        // 测试不同的 n 值
        vector<int> cases = {1, 2, 3};

        for (int n : cases) {
            cout << "Input n = " << n << endl;
            
            vector<string> result = sol.generateParenthesis(n);
            
            // 打印结果数量
            cout << "Total combinations: " << result.size() << endl;
            
            // 打印具体的括号组合
            cout << "[";
            for (int i = 0; i < result.size(); ++i) {
                cout << "\"" << result[i] << "\"" << (i == result.size() - 1 ? "" : ", ");
            }
            cout << "]" << endl;
            cout << "--------------------------" << endl;
        }
    }
} // namespace Question22

// --- 第 23 题 ---
namespace Question23
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    class Solution {
    public:
        struct compare {
            bool operator()(ListNode* a, ListNode* b) {
                return a->val > b->val;
            }
        };
        ListNode* mergeKLists(vector<ListNode*>& lists) {
            priority_queue<ListNode*, vector<ListNode*>, compare> pq;
            for (auto node : lists) {
                if (node) pq.push(node);
            }
            ListNode* dummy = new ListNode(0);
            ListNode* tail = dummy;
            while (!pq.empty()) {
                ListNode* minNode = pq.top();
                pq.pop();
                tail->next = minNode;
                tail = tail->next;
                if (minNode->next) {
                    pq.push(minNode->next);
                }
            }
            ListNode* res = dummy->next;
            delete dummy;
            return res;
        }
    };

    void test() {
        Solution sol;
        using namespace Utils;
        vector<ListNode*> lists = {
            createList<ListNode>({1, 4, 5}),
            createList<ListNode>({1, 3, 4}),
            createList<ListNode>({2, 6})
        };

        ListNode* res = sol.mergeKLists(lists);
        cout << "res: "; printList(res);
        freeList(res);
    }
} // namespace Question23

// --- 第 24 题 ---
namespace Question24
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    class Solution {
    public:
        ListNode* swapPairs(ListNode* head) {
            ListNode* dummy = new ListNode(0, head);
            ListNode* temp = dummy;

            while (temp->next != nullptr && temp->next->next != nullptr) {
                ListNode* node1 = temp->next;
                ListNode* node2 = temp->next->next;
                temp->next = node2;
                node1->next = node2->next;
                node2->next = node1;
                temp = node1;
            }
            ListNode* res = dummy->next;
            delete dummy;
            return res;
        }
    };
    void test() {
        Solution sol;
        using namespace Utils;
        ListNode* list = createList<ListNode>({1,2,3,4,5,6,7,8,9});
        printList(sol.swapPairs(list));
        freeList(list);
    }
} // namespace Question24

// --- 第 25 题 ---
namespace Question25
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    class Solution {
    public:
        ListNode* reverseKGroup(ListNode* head, int k) {
            if (head == nullptr) return nullptr;

            ListNode* a = head;
            ListNode* b = head;
            for (int i = 0; i < k; i++) {
                if (b == nullptr) return head;
                b = b->next;
            }
            ListNode* newHead = reverse(a, b);
            a->next = reverseKGroup(b, k);
            return newHead;
        }

        ListNode* reverse(ListNode* a, ListNode* b) {
            ListNode* pre = nullptr;
            ListNode* cur = a;
            while (cur != b) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            return pre;
        }
    };
    void test() {
        Solution sol;
        using namespace Utils;
        ListNode* list = createList<ListNode>({1,2,3,4,5,6,7,8,9});
        printList(sol.reverseKGroup(list, 4));
        freeList(list);
    }
} // namespace Question25

// --- 第 26 题 ---
namespace Question26
{
    class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            if (nums.empty()) return 0;
            int write = 1;
            for (int read = 1; read < (int)nums.size(); ++read) {
                if (nums[read] != nums[read - 1]) {
                    nums[write++] = nums[read];
                }
            }
            return write;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {1, 2, 3, 4, 5, 5};
        cout << sol.removeDuplicates(list) << endl;
    }
} // namespace Question26

// --- 第 27 题 ---
namespace Question27
{
    class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            int write = 0;
            for (int read = 0; read < (int)nums.size(); ++read) {
                if (nums[read] != val) {
                    nums[write++] = nums[read];
                }
            }
            return write;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {1, 2, 3, 4, 5, 5};
        cout << sol.removeElement(list, 5) << endl;
    }
} // namespace Question27

// --- 第 28 题 ---
namespace Question28
{
    class Solution {
    public:
        int strStr(string haystack, string needle) {
            if (needle.empty()) return 0;
            int n = haystack.size(), m = needle.size();
            // 构建 LPS（最长相同前后缀）数组
            vector<int> lps(m, 0);
            for (int i = 1, len = 0; i < m; ++i) {
                while (len > 0 && needle[i] != needle[len]) len = lps[len - 1];
                if (needle[i] == needle[len]) ++len;
                lps[i] = len;
            }
            // KMP 搜索
            int j = 0; // needle 指针
            for (int i = 0; i < n; ++i) {
                while (j > 0 && haystack[i] != needle[j]) j = lps[j - 1];
                if (haystack[i] == needle[j]) ++j;
                if (j == m) return i - m + 1;
            }
            return -1;
        }
    };
    void test() {
        Solution sol;
        cout << sol.strStr("XXVILCDM", "VIK") << endl;
    }
} // namespace Question28

// --- 第 29 题 ---
namespace Question29
{
    class Solution {
    public:
        int divide(int dividend, int divisor) {
            if (dividend == INT_MIN && divisor == -1) return INT_MAX;
            bool negative = (dividend > 0) ^ (divisor > 0);
            long a = abs((long)dividend);
            long b = abs((long)divisor);
            long result = 0;
            for (int i = 31; i >= 0; i--) {
                if ((a >> i) >= b) {
                    result += (1U << i);
                    a -= (b << i);
                }
            }
            return negative ? (int)-result : (int)result;
        }
    };
    void test() {
        Solution sol;
        cout << sol.divide(2147483647, -2147483648) << endl;
    }
} // namespace Question29

// --- 第 30 题 ---
namespace Question30
{
    class Solution {
    public:
        vector<int> findSubstring(string s, vector<string>& words) {
            vector<int> res;
            if (s.empty() || words.empty()) return res;
            int n = s.size(), m = words.size(), len = words[0].size();
            unordered_map<string, int> wordCount;
            for (const string& w : words) wordCount[w]++;

            for (int i = 0; i < len; i++) {
                int left = i, count = 0;
                unordered_map<string, int> tempCount;
                for (int j = i; j <= n - len; j += len) {
                    string w = s.substr(j, len);
                    if (wordCount.count(w)) {
                        tempCount[w]++;
                        count++;
                        while (tempCount[w] > wordCount[w]) {
                            string leftWord = s.substr(left, len);
                            tempCount[leftWord]--;
                            count--;
                            left += len;
                        }
                        if (count == m) res.push_back(left);
                    } else {
                        tempCount.clear();
                        count = 0;
                        left = j + len;
                    }
                }
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<string> list = {"foo","bar","the"};
        vector<int> res = sol.findSubstring("ffoobarthetthebarfoo", list);
        using namespace Utils;
        printVec(res);
    }
} // namespace Question30

// --- 第 31 题 ---
namespace Question31
{
    class Solution {
    public:
        void nextPermutation(vector<int>& nums) {
            int n = nums.size();
            if (n <= 1) return;
            int i = n - 2;
            while (i >= 0 && nums[i] >= nums[i + 1]) {
                i--;
            }
            if (i >= 0) {
                int j = n - 1;
                while (j > i && nums[j] <= nums[i]) {
                    j--;
                }
                swap(nums[i], nums[j]);
            }
            reverse(nums.begin() + i + 1, nums.end());
        }
    };
    // void test() {
    //     Solution sol;
    //     cout << sol.romanToInt("XXVILCDM") << endl;
    // }
} // namespace Question31

// --- 第 32 题 ---
namespace Question32
{
    class Solution {
    public:
        int longestValidParentheses(string s) {
            stack<int> st;
            st.push(-1);
            int maxLen = 0;
            for (int i = 0; i < s.length(); i++) {
                if (s[i] == '(') {
                    st.push(i);
                } else {
                    st.pop();
                    if (st.empty()) {
                        st.push(i);
                    } else {
                        maxLen = max(maxLen, i - st.top());
                    }
                }
            }
            return maxLen;
        }
    };
    void test() {
        Solution sol;
        cout << sol.longestValidParentheses("()())") << endl;
    }
} // namespace Question32

// --- 第 33 题 ---
namespace Question33
{
    class Solution {
    public:
        int search(vector<int>& nums, int target) {
            int left = 0, right = nums.size() - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (nums[mid] == target) return mid;

                if (nums[left] <= nums[mid]) {
                    if (nums[left] <= target && target < nums[mid]) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                } else {
                    if (nums[mid] < target && target <= nums[right]) {
                        left = mid + 1;
                    } else {
                        right = mid - 1;
                    }
                }
            }
            return -1;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {4,5,6,7,0,1,2};
        cout << sol.search(list, 0) << endl;
    }
} // namespace Question33

// --- 第 34 题 ---
namespace Question34
{
    class Solution {
    public:
        vector<int> searchRange(vector<int>& nums, int target) {
            int first = findPosition(nums, target, true);
            int last = findPosition(nums, target, false);
            return {first, last};
        }
    private:
        int findPosition(vector<int>& nums, int target, bool findFirst) {
            int n = nums.size();
            int left = 0, right = n - 1;
            int ans = -1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (nums[mid] == target) {
                    ans = mid;
                    if (findFirst) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                } else if (nums[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            return ans;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {5, 7, 7, 8, 8, 10};
        vector<int> res = sol.searchRange(list, 8);
        using namespace Utils;
        printVec(res);
    }
} // namespace Question34

// --- 第 35 题 ---
namespace Question35
{
    class Solution {
    public:
        int searchInsert(vector<int>& nums, int target) {
            int l = 0, r = (int)nums.size() - 1;
            while (l <= r) {
                int m = l + (r - l) / 2;
                if (nums[m] == target) return m;
                if (nums[m] < target) l = m + 1;
                else r = m - 1;
            }
            return l; // 插入位置（lower_bound）
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {5, 7, 7, 8, 8, 10};
        cout << sol.searchInsert(list, 4) << endl;
    }
} // namespace Question35

// --- 第 36 题 ---
namespace Question36
{
    class Solution {
    public:
        bool isValidSudoku(vector<vector<char>>& board) {
            int row[9][10] = {0};
            int col[9][10] = {0};
            int box[9][10] = {0};

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (board[i][j] == '.') continue;
                    int num = board[i][j] - '0';
                    int box_idx = (i/3) * 3 + (j/3);
                    if (row[i][num] || col[j][num] || box[box_idx][num]) {
                        return false;
                    }
                    row[i][num] = col[j][num] = box[box_idx][num] = 1;
                }
            }
            return true;
        }
    };
    void test() {
        Solution sol;
        // 用例 1: 合法数独
        vector<vector<char>> board1 = {
            {'5','3','.','.','7','.','.','.','.'},
            {'6','.','.','1','9','5','.','.','.'},
            {'.','9','8','.','.','.','.','6','.'},
            {'8','.','.','.','6','.','.','.','3'},
            {'4','.','.','8','.','3','.','.','1'},
            {'7','.','.','.','2','.','.','.','6'},
            {'.','6','.','.','.','.','2','8','.'},
            {'.','.','.','4','1','9','.','.','5'},
            {'.','.','.','.','8','.','.','7','9'}
        };
        cout << "Test 1: " << (sol.isValidSudoku(board1) ? "Valid" : "Invalid") << endl; 
        // 输出: Valid

        // 用例 2: 不合法数独（左上角宫格有两个 '8'）
        vector<vector<char>> board2 = board1;
        board2[0][0] = '8'; // 修改点：原本是 5，改成 8，与 [2][2] 的 8 冲突
        cout << "Test 2: " << (sol.isValidSudoku(board2) ? "Valid" : "Invalid") << endl; 
        // 输出: Invalid
    }
} // namespace Question36

// --- 第 37 题 ---
namespace Question37
{
    class Solution {
    public:
        void solveSudoku(vector<vector<char>>& board) {
            backtrack(board);
        }
        bool backtrack(vector<vector<char>>& board) {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (board[i][j] != '.') continue;
                    for (char c = '1'; c <= '9'; c++) {
                        if (isValidSudoku(board)) {
                            board[i][j] = c;
                            if (backtrack(board)) return true;
                            board[i][j] = '.';
                        }
                    }
                    return false;
                }
            }
            return true;
        }
        bool isValidSudoku(vector<vector<char>>& board) {
            int row[9][10] = {0};
            int col[9][10] = {0};
            int box[9][10] = {0};

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (board[i][j] == '.') continue;
                    int num = board[i][j] - '0';
                    int box_idx = (i/3) * 3 + (j/3);
                    if (row[i][num] || col[j][num] || box[box_idx][num]) {
                        return false;
                    }
                    row[i][num] = col[j][num] = box[box_idx][num] = 1;
                }
            }
            return true;
        }
    };
    void test() {
        Solution sol;
        // 一个待解的数独题目（'.' 代表空格）
        vector<vector<char>> board = {
            {'5','3','.','.','7','.','.','.','.'},
            {'6','.','.','1','9','5','.','.','.'},
            {'.','9','8','.','.','.','.','6','.'},
            {'8','.','.','.','6','.','.','.','3'},
            {'4','.','.','8','.','3','.','.','1'},
            {'7','.','.','.','2','.','.','.','6'},
            {'.','6','.','.','.','.','2','8','.'},
            {'.','.','.','4','1','9','.','.','5'},
            {'.','.','.','.','8','.','.','7','9'}
        };

        cout << "--- 求解前的数独 ---" << endl;
        for (auto& row : board) {
            for (char c : row) cout << c << " ";
            cout << endl;
        }

        sol.solveSudoku(board);

        cout << "\n--- 求解后的数独 ---" << endl;
        for (auto& row : board) {
            for (char c : row) cout << c << " ";
            cout << endl;
        }
    }
} // namespace Question37

// --- 第 38 题 ---
namespace Question38
{
    class Solution {
    public:
        string countAndSay(int n) {
            if (n == 1) return "1";
            string res = "1";
            for (int i = 2; i <= n; i++) {
                string next = "";
                int len = res.length();
                for (int j = 0; j < len; ) {
                    int start = j;
                    while (j < len && res[j] == res[start]) {
                        j++;
                    }
                    next += to_string(j - start) + res[start];
                }
                res = next;
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        cout << sol.countAndSay(9) << endl;
    }
} // namespace Question38

// --- 第 39 题 ---
namespace Question39
{
    class Solution {
    public:
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            vector<vector<int>> res;
            vector<int> path;
            sort(candidates.begin(), candidates.end());
            backtrack(candidates, target, 0, path, res);
            return res;
        }
    private:
        void backtrack(vector<int>& candidates, int target, int start, vector<int>& path, vector<vector<int>>& res) {
            if (target == 0) {
                res.push_back(path);
                return;
            }
            for (int i = start; i < candidates.size(); i++) {
                if (target - candidates[i] < 0) break;
                path.push_back(candidates[i]);
                backtrack(candidates, target - candidates[i], i, path, res);
                path.pop_back();
            }
        }
    };
    void test() {
        Solution sol;
        vector<int> c = {2, 3, 6, 7};
        auto res = sol.combinationSum(c, 7);
        
        // 预期输出: [2,2,3], [7]
        for (const auto& comb : res) {
            cout << "[";
            for (int i = 0; i < comb.size(); i++) {
                cout << comb[i] << (i == comb.size() - 1 ? "" : ",");
            }
            cout << "]" << endl;
        }
    }
} // namespace Question39

// --- 第 40 题 ---
namespace Question40
{
    class Solution {
    public:
        vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
            vector<vector<int>> res;
            vector<int> path;
            sort(candidates.begin(), candidates.end());
            backtrack(candidates, target, 0, path, res);
            return res;
        }
    private:
        void backtrack(vector<int>& candidates, int target, int start, vector<int>& path, vector<vector<int>>& res) {
            if (target == 0) {
                res.push_back(path);
                return;
            }
            for (int i = start; i < candidates.size(); i++) {
                if (target - candidates[i] < 0) break;
                if (i > start && candidates[i] == candidates[i - 1]) continue;
                path.push_back(candidates[i]);
                backtrack(candidates, target - candidates[i], i + 1, path, res);
                path.pop_back();
            }
        }
    };
    void test() {
        Solution sol;
        vector<int> c = {2, 3, 6, 7};
        auto res = sol.combinationSum2(c, 7);
        
        // 预期输出: [2,2,3], [7]
        for (const auto& comb : res) {
            cout << "[";
            for (int i = 0; i < comb.size(); i++) {
                cout << comb[i] << (i == comb.size() - 1 ? "" : ",");
            }
            cout << "]" << endl;
        }
    }
} // namespace Question40

// --- 第 41 题 ---
namespace Question41
{
    class Solution {
    public:
        int firstMissingPositive(vector<int>& nums) {
            int n = nums.size();
            for (int i = 0; i < n; i++) {
               while (nums[i] > 0 && nums[i] <=n && nums[i] != nums[nums[i] - 1]) {
                   swap(nums[i], nums[nums[i] - 1]);
               }
            }
            for (int i = 0; i < n; ++i) {
                if (nums[i] != i + 1) {
                    return i + 1;
                }
            }
            return n + 1;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {1, 2, 4, 56};
        cout << sol.firstMissingPositive(list) << endl;
    }
} // namespace Question41

// --- 第 42 题 ---
namespace Question42
{
    class Solution {
    public:
        int trap(vector<int>& height) {
            int left = 0, right = height.size() - 1;
            int left_max = 0, right_max = 0;
            int res = 0;
            while (left < right) {
                if (height[left] < height[right]) {
                    if (height[left] >= left_max) {
                        left_max = height[left];
                    } else {
                        res += left_max - height[left];
                    }
                    left++;
                } else {
                    if (height[right] >= right_max) {
                        right_max = height[right];
                    } else {
                        res += right_max - height[right];
                    }
                    right--;
                }
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {0,1,0,2,1,0,1,3,2,1,2,1};
        cout << sol.trap(list) << endl;
    }
} // namespace Question42

// --- 第 43 题 ---
namespace Question43
{
    class Solution {
    public:
        string multiply(string num1, string num2) {
            if (num1 == "0" || num2 == "0") return "0";
            int m = num1.size(), n = num2.size();
            vector<int> res(m + n, 0);
            for (int i = m - 1; i >= 0; i--) {
                for (int j = n - 1; j >= 0; j--) {
                    int mul = (num1[i] - '0') * (num2[j] - '0');
                    int sum = mul + res[i + j + 1];
                    res[i + j + 1] = sum % 10;
                    res[i + j] += sum / 10;
                }
            }
            string result = "";
            for (int p : res) {
                if (!(result.empty() && p == 0)) {
                    result.push_back(p + '0');
                }
            }
            return result.empty() ? "0" : result;
        }
    };
    void test() {
        Solution sol;
        cout << sol.multiply("123", "123") << endl;
    }
} // namespace Question43

// --- 第 44 题 ---
namespace Question44
{
    class Solution {
    public:
        bool isMatch(string s, string p) {
            int sIdx = 0, pIdx = 0;
            int starIdx = -1, sMatch = 0;
            while (sIdx < s.length()) {
                if (pIdx < p.length() && (p[pIdx] == '?' || p[pIdx] == s[sIdx])) {
                    sIdx++;
                    pIdx++;
                } else if (pIdx < p.length() && p[pIdx] == '*') {
                    starIdx = pIdx;
                    sMatch = sIdx;
                    pIdx++;
                } else if (starIdx != -1) {
                    pIdx = starIdx + 1;
                    sMatch++;
                    sIdx = sMatch;
                } else {
                    return false;
                }
            }
            while (pIdx < p.length() && p[pIdx] == '*') {
                pIdx++;
            }
            return pIdx == p.length();
        }
    };
    void test() {
        Solution sol;
        cout << (sol.isMatch("abc", "ab?") ? "true" : "false") << endl;
    }
} // namespace Question44

// --- 第 45 题 ---
namespace Question45
{
    class Solution {
    public:
        int jump(vector<int>& nums) {
            int n = nums.size();
            if (n <= 1) return 0;
            int jumps = 0;
            int cur_end = 0;
            int max_pos = 0;
            for (int i = 0; i < n - 1; ++i) {
                max_pos = max(max_pos, i + nums[i]);
                if (i == cur_end) {
                    jumps++;
                    cur_end = max_pos;
                    if (cur_end >= n - 1) break;
                }
            }
            return jumps;
        }
    };
    void test() {
        Solution sol;
        vector<int> list = {1,2,0,1,3,2,1,2,1};
        cout << sol.jump(list) << endl;
    }
} // namespace Question45

// --- 第 46 题 ---
namespace Question46
{
    class Solution {
    public:
        vector<vector<int>> permute(vector<int>& nums) {
            vector<vector<int>> res;
            vector<int> path;
            vector<bool> used(nums.size(), false);
            backtrack(nums, used, path, res);
            return res;
        }
    private:
        void backtrack(vector<int>& nums, vector<bool>& used, vector<int>& path, vector<vector<int>>& res) {
            if (path.size() == nums.size()) {
                res.push_back(path);
                return;
            }
            for (int i = 0; i < nums.size(); i++) {
                if (used[i]) continue;
                used[i] = true;
                path.push_back(nums[i]);
                backtrack(nums, used, path, res);
                path.pop_back();
                used[i] = false;
            }
        }
    };
    void test() {
        Solution sol;
        vector<int> nums = {1, 2, 3};
        vector<vector<int>> res = sol.permute(nums);

        cout << "Input: [1,2,3]" << endl;
        cout << "Output: [" << endl;
        
        // 第一层循环：遍历每一个排列 (vector<int>)
        for (int i = 0; i < res.size(); i++) {
            cout << "  [";
            // 第二层循环：遍历排列里的每一个数字
            for (int j = 0; j < res[i].size(); j++) {
                cout << res[i][j] << (j == res[i].size() - 1 ? "" : ",");
            }
            cout << "]" << (i == res.size() - 1 ? "" : ",") << endl;
        }
        
        cout << "]" << endl;
    }
} // namespace Question46

// --- 第 47 题 ---
namespace Question47
{
    class Solution {
    public:
        vector<vector<int>> permuteUnique(vector<int>& nums) {
            vector<vector<int>> res;
            vector<int> path;
            vector<bool> used(nums.size(), false);
            sort(nums.begin(), nums.end());
            backtrack(nums, used, path, res);
            return res;
        }

    private:
        void backtrack(vector<int>& nums, vector<bool>& used, vector<int>& path,
                       vector<vector<int>>& res) {
            if (path.size() == nums.size()) {
                res.push_back(path);
                return;
            }
            for (int i = 0; i < nums.size(); i++) {
                if (used[i]) continue;
                if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) {
                    continue;
                }
                used[i] = true;
                path.push_back(nums[i]);
                backtrack(nums, used, path, res);
                path.pop_back();
                used[i] = false;
            }
        }
    };

    void test() {
        Solution sol;
        vector<int> nums = {1, 1, 2};
        vector<vector<int>> res = sol.permuteUnique(nums);
        using namespace Utils;
        for (const auto& row : res) {
            printVec(row);
        }
    }
} // namespace Question47

// --- 第 48 题 ---
namespace Question48
{
    class Solution {
    public:
        void rotate(vector<vector<int>>& matrix) {
            int n = matrix.size();
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    swap(matrix[i][j], matrix[j][i]);
                }
            }
            for (int i = 0; i < n; i++) {
                reverse(matrix[i].begin(), matrix[i].end());
            }
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> matrix = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        using namespace Utils;
        cout << "Input Matrix:" << endl;
        printVec2(matrix);

        sol.rotate(matrix);

        cout << "\nOutput Matrix (Rotated 90 degrees):" << endl;
        printVec2(matrix);
    }
} // namespace Question48

// --- 第 49 题 ---
namespace Question49
{
    class Solution {
    public:
        vector<vector<string>> groupAnagrams(vector<string>& strs) {
            unordered_map<string, vector<string>> mp;
            for (string& s : strs) {
                string key = s;
                sort(key.begin(), key.end());
                mp[key].push_back(s);
            }
            vector<vector<string>> res;
            for (auto it = mp.begin(); it != mp.end(); ++it) {
                res.push_back(it->second);
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
        vector<vector<string>> res = sol.groupAnagrams(strs);
        using namespace Utils;
        cout << "Input: "; printVec(strs);
        cout << "Output: "; printVec2(res);
    }
} // namespace Question49

// --- 第 50 题 ---
namespace Question50
{
    class Solution {
    public:
        double myPow(double x, int n) {
            long long N = n;
            if (N < 0) {
                x = 1 / x;
                N = -N;
            }
            return fastPow(x, N);
        }
    private:
        double fastPow(double x, long long n) {
            if (n == 0) {
                return 1.0;
            }
            double half = fastPow(x, n / 2);
            if (n % 2 == 0) {
                return half * half;
            } else {
                return half * half * x;
            }
        }
    };
    void test() {
        Solution sol;
        // 测试用例 1: x = 2.0, n = 10 -> 1024
        cout << "Input: x = 2.0, n = 10 | Output: " << sol.myPow(2.0, 10) << endl;
        // 测试用例 2: x = 2.1, n = -2 -> 0.226...
        cout << "Input: x = 2.1, n = -2 | Output: " << sol.myPow(2.1, -2) << endl;
        // 测试用例 3: x = 2, n = INT_MIN (测试溢出处理)
        cout << "Input: x = 2.0, n = -2147483648 | Output: " << sol.myPow(2.0, -2147483648) << endl;
    }
} // namespace Question50

// --- 第 51 题 ---
namespace Question51
{
    class Solution {
    public:
        vector<vector<string>> solveNQueens(int n) {
            vector<vector<string>> res;
            vector<int> col(n, -1);
            backtrack(n, 0, col, res);
            return res;
        }
    private:
        void backtrack(int n, int row, vector<int>& col, vector<vector<string>>& res) {
            if (row == n) {
                res.push_back(buildBoard(col, n));
                return;
            }

            for (int c = 0; c < n; ++c) {
                if (!isValid(col, row, c)) continue;
                col[row] = c;
                backtrack(n, row + 1, col, res);
                col[row] = -1;
            }
        }

        bool isValid(const vector<int>& col, int row, int c) const {
            for (int r = 0; r < row; ++r) {
                int cc = col[r];
                if (cc == c) return false;
                if (abs(cc - c) == abs(r - row)) return false;
            }
            return true;
        }

        vector<string> buildBoard(const vector<int>& col, int n) const {
            vector<string> board(n, string(n, '.'));
            for (int r = 0; r < n; ++r) {
                board[r][col[r]] = 'Q';
            }
            return board;
        }
    };
    void test() {
        Solution sol;
        int n = 4;
        auto res = sol.solveNQueens(n);

        cout << "Input: n = " << n << endl;
        cout << "Output:" << endl;
        using namespace Utils;
        printVec2(res);
    }
} // namespace Question51

// --- 第 52 题 ---
namespace Question52
{
    class Solution {
    public:
        int totalNQueens(int n) {
            int count = 0;
            vector<int> col(n, -1);
            backtrack(n, 0, col, count);
            return count;
        }
    private:
        void backtrack(int n, int row, vector<int>& col, int& count) {
            if (row == n) {
                count++;
                return;
            }
            for (int c = 0; c < n; ++c) {
                if (!isValid(col, row, c)) continue;
                col[row] = c;
                backtrack(n, row + 1, col, count);
                col[row] = -1;
            }
        }

        bool isValid(const vector<int>& col, int row, int c) const {
            for (int r = 0; r < row; ++r) {
                int cc = col[r];
                if (cc == c) return false;
                if (abs(cc - c) == abs(r - row)) return false;
            }
            return true;
        }
    };
    void test() {
        Solution sol;
        int n_values[] = {1, 4, 5, 8};
        for (int n : n_values) {
            int result = sol.totalNQueens(n);
            cout << "Input: m = " << n << " | Output (Total Solutions): "
                 << result << endl;
        }
    }
} // namespace Question52

// --- 第 53 题 ---
namespace Question53
{
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int max_so_far = INT_MIN;
            int current_sum = 0;
            for (int i = 0; i < nums.size(); i++) {
                current_sum += nums[i];
                if (current_sum > max_so_far) {
                    max_so_far = current_sum;
                }
                if (current_sum < 0) {
                    current_sum = 0;
                }
            }
            return max_so_far;
        }
    };
    void test() {
        Solution sol;
        vector<int> nums1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        vector<int> nums2 = {1};
        
        cout << "Input: [-2, 1, -3, 4, -1, 2, 1, -5, 4] | Output: " << sol.maxSubArray(nums1) << endl;
        cout << "Input: [1] | Output: " << sol.maxSubArray(nums2) << endl;
    }
} // namespace Question53

// --- 第 54 题 ---
namespace Question54
{
    class Solution {
    public:
        vector<int> spiralOrder(vector<vector<int>>& matrix) {
            if (matrix.empty()) return {};
            vector<int> res;
            int top = 0;
            int bottom = matrix.size() - 1;
            int left = 0;
            int right = matrix[0].size() - 1;

            while (top <= bottom && left <= right) {
                for (int i = left; i <= right; ++i) {
                    res.push_back(matrix[top][i]);
                }
                top++;
                for (int i = top; i <= bottom; ++i) {
                    res.push_back(matrix[i][right]);
                }
                right--;
                if (top <= bottom) {
                    for (int i = right; i >= left; --i) {
                        res.push_back(matrix[bottom][i]);
                    }
                    bottom--;
                }
                if (left <= right) {
                    for (int i = bottom; i >= top; --i) {
                        res.push_back(matrix[i][left]);
                    }
                    left++;
                }
            }
            return res;
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> matrix1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        vector<vector<int>> matrix2 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
        using namespace Utils;
        cout << "Input: " << endl;
        printVec2(matrix1);
        vector<int> res1 = sol.spiralOrder(matrix1);
        cout << "Output: " << endl;
        printVec(res1);

        cout << "Input: " << endl;
        printVec2(matrix2);
        vector<int> res2 = sol.spiralOrder(matrix2);
        cout << "Output: " << endl;
        printVec(res2);
    }
} // namespace Question54

// --- 第 55 题 ---
namespace Question55
{
    class Solution {
    public:
        bool canJump(vector<int>& nums) {
            int farthest_reachable = 0;
            for (int i = 0; i < nums.size(); ++i) {
                if (i > farthest_reachable) {
                    return false;
                }
                farthest_reachable = max(farthest_reachable, i + nums[i]);
                if (farthest_reachable >= nums.size() - 1) {
                    return true;
                }
            }
            return true;
        }
    };
    void test() {
        Solution sol;
        vector<int> nums1 = {2, 3, 1, 1, 4};
        cout << "Input: [2,3,1,1,4] | Output: " << sol.canJump(nums1) << endl; // Expected: 1 (true)

        vector<int> nums2 = {3, 2, 1, 0, 4};
        cout << "Input: [3,2,1,0,4] | Output: " << sol.canJump(nums2) << endl; // Expected: 0 (false)
    }
} // namespace Question55

// --- 第 56 题 ---
namespace Question56
{
    class Solution {
    public:
        vector<vector<int>> merge(vector<vector<int>>& intervals) {
            if (intervals.empty()) return {};
            sort(intervals.begin(), intervals.end());
            vector<vector<int>> merged;
            merged.push_back(intervals[0]);
            for (int i = 1; i < intervals.size(); ++i) {
                vector<int>& lastMerged = merged.back();
                int currentStart = intervals[i][0];
                int currentEnd = intervals[i][1];
                if (currentStart <= lastMerged[1]) {
                    lastMerged[1] = max(lastMerged[1], currentEnd);
                } else {
                    merged.push_back(intervals[i]);
                }
            }
            return merged;
        }
    };
    void test() {
        Solution sol;
        // Case 1: Overlapping intervals
        vector<vector<int>> input1 = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
        // Expected: [[1, 6], [8, 10], [15, 18]]
        vector<vector<int>> res1 = sol.merge(input1);

        // Case 2: Intervals that touch (e.g., [1,4] and [4,5])
        vector<vector<int>> input2 = {{1, 4}, {4, 5}};
        // Expected: [[1, 5]]
        vector<vector<int>> res2 = sol.merge(input2);

        cout << "Input: " << endl;
        using namespace Utils;
        printVec2(input1);
        printVec2(input2);
        cout << "Output: " << endl;
        printVec2(res1);
        printVec2(res2);
    }
} // namespace Question56

// --- 第 57 题 ---
namespace Question57
{
    class Solution {
    public:
        vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
            vector<vector<int>> result;
            int n = intervals.size();
            int i = 0;
            while (i < n && intervals[i][1] < newInterval[0]) {
                result.push_back(intervals[i]);
                i++;
            }
            while (i < n && intervals[i][0] <= newInterval[1]) {
                newInterval[0] = min(newInterval[0], intervals[i][0]);
                newInterval[1] = max(newInterval[1], intervals[i][1]);
                i++;
            }
            result.push_back(newInterval);
            while (i < n) {
                result.push_back(intervals[i]);
                i++;
            }
            return result;
        }
    };
    void test() {
        Solution sol;
        // Case 1: Normal insertion in the middle with merge
        vector<vector<int>> input1 = {{1, 3}, {6, 9}};
        vector<int> newInterval1 = {2, 5};
        // Process: [1,3] overlaps with [2,5] -> merged to [1,5]. 
        // Result: [[1, 5], [6, 9]]
        vector<vector<int>> res1 = sol.insert(input1, newInterval1);
        cout << "Case 1 Output: ";
        using namespace Utils;
        printVec2(res1);

        // Case 2: Insertion at the very beginning
        vector<vector<int>> input2 = {{5, 10}, {12, 15}};
        vector<int> newInterval2 = {1, 3};
        // Result: [[1, 3], [5, 10], [12, 15]]
        vector<vector<int>> res2 = sol.insert(input2, newInterval2);
        cout << "Case 2 Output: ";
        printVec2(res2);
    }
} // namespace Question57

// --- 第 58 题 ---
namespace Question58
{
    class Solution {
    public:
        int lengthOfLastWord(string s) {
            int n = s.length();
            int length = 0;
            int i = n - 1;
            while (i >= 0 && s[i] == ' ') {
                i--;
            }
            while (i >= 0 && s[i] != ' ') {
                length++;
                i--;
            }
            return length;
        }
    };
    void test() {
        Solution sol;

        // Case 1: Standard case with trailing spaces
        string s1 = "Helloo World  ";
        // Expected: 5
        cout << "Input: \"" << s1 << "\" | Output: " << sol.lengthOfLastWord(s1) << endl;

        // Case 2: Single word
        string s2 = "a";
        // Expected: 1
        cout << "Input: \"" << s2 << "\" | Output: " << sol.lengthOfLastWord(s2) << endl;

        // Case 3: Multiple spaces between words
        string s3 = "    fly me   to   the moon  ";
        // Expected: 4 ("moon")
        cout << "Input: \"" << s3 << "\" | Output: " << sol.lengthOfLastWord(s3) << endl;
    }
} // namespace Question58

// --- 第 59 题 ---
namespace Question59
{
    class Solution {
    public:
        vector<vector<int>> generateMatrix(int n) {
            vector<vector<int>> matrix(n, vector<int>(n));
            int top = 0, bottom = n - 1, left = 0, right = n - 1;
            int num = 1;
            while (num <= n * n) {
                // Right
                for (int i = left; i <= right; ++i) matrix[top][i] = num++;
                top++;
                // Down
                for (int i = top; i <= bottom; ++i) matrix[i][right] = num++;
                right--;
                // Left
                if (top <= bottom) {
                    for (int i = right; i >= left; --i) matrix[bottom][i] = num++;
                    bottom--;
                }
                // Up
                if (left <= right) {
                    for (int i = bottom; i >= top; --i) matrix[i][left] = num++;
                    left++;
                }
            }
            return matrix;
        }
    };
    void test() {
        Solution sol;

        // Case 1: n = 3
        int n1 = 3;
        vector<vector<int>> res1 = sol.generateMatrix(n1);
        cout << "Input: n = 3" << endl;
        for (const auto& row : res1) {
            for (int val : row) cout << val << "\t";
            cout << endl;
        }

        // Case 2: n = 1
        int n2 = 1;
        vector<vector<int>> res2 = sol.generateMatrix(n2);
        cout << "\nInput: n = 1" << endl;
        for (const auto& row : res2) {
            for (int val : row) cout << val << "\t";
            cout << endl;
        }
    }
} // namespace Question59

// --- 第 60 题 ---
namespace Question60
{
    class Solution {
    private:
        int counter = 0;
        string result = "";
        void backtrack(vector<int>& nums, vector<bool>& used, string& current_path, int k) {
            if (current_path.length() == nums.size()) {
                counter++;
                if (counter == k) {
                    result = current_path;
                }
                return;
            }
            if (!result.empty()) return;
            for (int i = 0; i < nums.size(); ++i) {
                if (!used[i]) {
                    used[i] = true;
                    current_path.push_back(nums[i] + '0');
                    backtrack(nums, used, current_path, k);
                    current_path.pop_back();
                    used[i] = false;
                }
            }
        }
    public:
        string getPermutation(int n, int k) {
            vector<int> nums;
            for (int i = 1; i <= n; ++i) nums.push_back(i);
            vector<bool> used(n, false);
            string current_path = "";
            counter = 0;
            result = "";
            backtrack(nums, used, current_path, k);
            return result;
        }
        string getPermutation2(int n, int k) {
            vector<int> factorial(n + 1, 1);
            for (int i = 1; i <= n; ++i) {
                factorial[i] = factorial[i - 1] * i;
            }
            vector<int> numbers;
            for (int i = 1; i <= n; ++i) {
                numbers.push_back(i);
            }
            string result = "";
            k = k - 1;
            for (int i = n; i >= 1; --i) {
                int group_size = factorial[i - 1];
                int index = k / group_size;
                result += to_string(numbers[index]);
                numbers.erase(numbers.begin() + index);
                k %= group_size;
            }
            return result;
        }
    };
    void test() {
        Solution sol;
        // Case 1: n = 3, k = 3
        // Permutations of {1,2,3}: "123", "132", "213", "231", "312", "321"
        // Expected: "213"
        cout << "Input: n = 3, k = 3 | Output: " << sol.getPermutation(3, 3) << endl;

        // Case 2: n = 4, k = 9
        // Expected: "2314"
        cout << "Input: n = 4, k = 9 | Output: " << sol.getPermutation(4, 9) << endl;
        
        // Case 3: n = 1, k = 1
        // Expected: "1"
        cout << "Input: n = 1, k = 1 | Output: " << sol.getPermutation(1, 1) << endl;
    }
} // namespace Question60

// --- 第 61 题 ---
namespace Question61
{
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
    class Solution {
    public:
        ListNode* rotateRight(ListNode* head, int k) {
            if (!head || !head->next || k == 0) {
                return head;
            }
            int length = 1;
            ListNode* tail = head;
            while (tail->next != nullptr) {
                tail = tail->next;
                length++;
            }
            k = k % length;
            if (k == 0) {
                return head;
            }
            tail->next = head;
            ListNode* new_tail = head;
            for (int i = 0; i < length - k - 1; ++i) {
                new_tail = new_tail->next;
            }
            ListNode* new_head = new_tail->next;
            new_tail->next = nullptr;
            return new_head;
        }
    };
    void test() {
        Solution sol;
        using namespace Utils;
        ListNode* list = createList<ListNode>({1, 2, 3, 4, 5});
        printList(sol.rotateRight(list, 2));
        // cout << sol.romanToInt("XXVILCDM") << endl;
    }
} // namespace Question61

// --- 第 62 题 ---
namespace Question62
{
    class Solution {
    public:
        int uniquePaths2D(int m, int n) {
            vector<vector<int>> dp(m, vector<int>(n, 1));
            for (int i = 1; i < m; ++i) {
                for (int j = 1; j < n; ++j) {
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                }
            }
            return dp[m - 1][n - 1];
        }
        int uniquePaths1D(int m, int n) {
            vector<int> dp(n, 1);
            for (int i = 1; i < m; ++i) {
                for (int j = 1; j < n; ++j) {
                    dp[j] += dp[j - 1];
                }
            }
            return dp[n - 1];
        }
        int uniquePathsMath(int m, int n) {
            long long result = 1;
            int N = m + n - 2;
            int K = min(m - 1, n - 1);
            for (int i = 1; i <= K; ++i) {
                result = result * (N - i + 1) / i;
            }
            return (int)result;
        }
    };
    void test() {
        Solution sol;
        cout << sol.uniquePaths2D(3, 2) << endl;
        cout << sol.uniquePaths1D(3, 2) << endl;
        cout << sol.uniquePathsMath(3, 2) << endl;
    }
} // namespace Question62

// --- 第 63 题 ---
namespace Question63
{
    class Solution {
    public:
        int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
            if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
            int m = obstacleGrid.size();
            int n = obstacleGrid[0].size();
            if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1) {
                return 0;
            }
            vector<int> dp(n, 0);
            dp[0] = 1;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (obstacleGrid[i][j] == 1) {
                        dp[j] = 0;
                    } else {
                        if (j > 0) {
                            dp[j] += dp[j - 1];
                        }
                    }
                }
            }
            return (int)dp[n - 1];
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> grid = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        cout << "Output: " << sol.uniquePathsWithObstacles(grid) << endl;
    }
} // namespace Question63

// --- 第 64 题 ---
namespace Question64
{
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size();
            int n = grid[0].size();

            for (int j = 1; j < n; j++) {
                grid[0][j] += grid[0][j - 1];
            }

            for (int i = 1; i < m; i++) {
                grid[i][0] += grid[i - 1][0];
            }

            for (int i = 1; i < m; i++) {
                for (int j = 1; j < n; j++) {
                    grid[i][j] += min(grid[i - 1][j], grid[i][j - 1]);
                }
            }
            return grid[m - 1][n - 1];
        }
    };
    void test() {
        Solution sol;
        vector<vector<int>> grid = {{4, 10, 8}, {1, 20, 6}, {1, 30, 5}};
        cout << "Output: " << sol.minPathSum(grid) << endl;
    }
} // namespace Question64

// --- 第 65 题 ---
namespace Question65
{
    class Solution {
    public:
        bool isNumber(string s) {
            bool seenDigit = false;
            bool seenDot = false;
            bool seenExponent = false;
            for (int i = 0; i < s.size(); i++) {
                char c = s[i];
                if (isdigit(c)) {
                    seenDigit = true;
                } else if (c == '+' || c == '-') {
                    if (i > 0 && s[i - 1] != 'e' && s[i - 1] != 'E') {
                        return false;
                    }
                } else if (c == 'e' || c == 'E') {
                    if (seenExponent || !seenDigit) {
                        return false;
                    }
                    seenExponent = true;
                    seenDigit = false;
                } else if (c == '.') {
                    if (seenDot || seenExponent) {
                        return false;
                    }
                    seenDot = true;
                } else {
                    return false;
                }
            }
            return seenDigit;
        }
        bool isNumber2(string s) {
            string pattern = R"(^[+-]?(\d+(\.\d*)?|\.d+)([eE][+-]?\d+)?$)";
            regex re(pattern);
            return regex_match(s, re);
        }
    };
    void test() {
        Solution sol;
        struct TestCase {
            string input;
            bool expected;
        };

        vector<TestCase> cases = {
            {"2", true},
            {"0", true},
            {"abc", false},
            {"1a", false},
            {"e3", false},
            {"e", false},
            {"e5", false}, // Wait, e is not a number. But "1e5" is.
            {"3.e2", true},
            {".1", true},
            {"3.", true},
            {"-.9", true},
            {"--6", false},
            {"-1e5", true},
            {"--", false}
        };
        cout << "Starting Tests..." << endl;
        for (const auto& tc : cases) {
            bool result = sol.isNumber(tc.input);
            cout << "Input: \"" << tc.input << "\" | Expected: " 
                << (tc.expected ? "true" : "false") 
                << " | Result: " << (result ? "true" : "false")
                << (result == tc.expected ? " [PASS]" : " [FAIL]") << endl;
        }
    }
} // namespace Question65

// --- 第 66 题 ---
namespace Question66
{
    class Solution {
    public:
        vector<int> plusOne(vector<int>& digits) {
            int n = digits.size();
            for (int i = n - 1; i >= 0; i--) {
                if (digits[i] < 9) {
                    digits[i]++;
                    return digits;
                }
                digits[i] = 0;
            }
            digits.insert(digits.begin(), 1);
            return digits;
        }
    };
    void test() {
        Solution sol;    // Test Cases: Input -> Expected Output
        struct TestCase {
            vector<int> input;
            vector<int> expected;
        };

        vector<TestCase> cases = {
            {{1, 2, 3}, {1, 2, 4}},     // Standard case
            {{4, 3, 2, 1}, {4, 3, 2, 2}}, // Standard case
            {{9}, {1, 0}},               // Carry-over (size increases)
            {{9, 9}, {1, 0, 0}},         // Full carry-over (size increases)
            {{1, 9}, {2, 0}}             // Internal carry-over
        };

        cout << "Starting Tests..." << endl;
        for (auto& tc : cases) {
            vector<int> result = sol.plusOne(tc.input);

            bool match = (result == tc.expected);
            cout << "Input: [";
            for(int i=0; i<tc.input.size(); ++i) cout << tc.input[i] << (i == tc.input.size()-1 ? "" : ",");
            cout << "] | Result: [";
            for(int i=0; i<result.size(); ++i) cout << result[i] << (i == result.size()-1 ? "" : ",");
            cout << "] | " << (match ? "[PASS]" : "[FAIL]") << endl;
        }
    }
} // namespace Question66

// --- 第 67 题 ---
namespace Question67
{
    class Solution {
    public:
        string addBinary(string a, string b) {
            string result = "";
            int i = a.size() - 1;
            int j = b.size() - 1;
            int carry = 0;
            while (i >= 0 || j >= 0 || carry > 0) {
                int sum = carry;
                if (i >= 0) {
                    sum += a[i] - '0';
                    i--;
                }
                if (j >= 0) {
                    sum += b[j] - '0';
                    j--;
                }
                result.push_back((sum % 2) + '0');
                carry = sum / 2;
            }
            reverse(result.begin(), result.end());
            return result;
        }
    };
    void test() {
        Solution sol;
        struct TestCase {
            string a;
            string b;
            string expected;
        };

        vector<TestCase> cases = {
            {"11", "1", "100"},       // 3 + 1 = 4
            {"1010", "1011", "10101"}, // 10 + 11 = 21
            {"0", "0", "0"},          // 0 + 0 = 0
            {"111", "111", "1110"}    // 7 + 7 = 14
        };

        cout << "Starting Binary Addition Tests..." << endl;
        for (const auto& tc : cases) {
            string res = sol.addBinary(tc.a, tc.b);
            bool pass = (res == tc.expected);
            cout << "Input: " << tc.a << " + " << tc.b 
                << " | Result: " << res 
                << " | " << (pass ? "[PASS]" : "[FAIL]") << endl;
        }
    }
} // namespace Question67

// --- 第 68 题 ---
namespace Question68
{
    class Solution {
    public:
        vector<string> fullJustify(vector<string>& words, int maxWidth) {
            vector<string> result;
            int i = 0;
            int n = words.size();
            while (i < n) {
                int j = i;
                int current_words_len = 0;
                while (j < n && current_words_len + (j - i) + (int)words[j].length() <= maxWidth) {
                    current_words_len += words[j].length();
                    j++;
                }
                int num_words = j - i;
                int total_spaces = maxWidth - current_words_len;
                string line = "";
                if (j == n || num_words == 1) {
                    for (int k = i; k < j; ++k) {
                        line += words[k];
                        if (k < j - 1) line += " ";
                    }
                    line.append(maxWidth - line.length(), ' ');
                } else {
                    int gaps = num_words - 1;
                    int spaces_per_gap = total_spaces / gaps;
                    int extra_spaces = total_spaces % gaps;
                    for (int k = i; k < j; ++k) {
                        line += words[k];
                        if (k < j - 1) {
                            int spaces_to_add = spaces_per_gap + (k - i < extra_spaces ? 1 : 0);
                            line.append(spaces_to_add, ' ')
                            ;
                        }
                    }
                }
                result.push_back(line);
                i = j;
            }
            return result;
        }
    };
    void test() {
        Solution sol;
        vector<string> words = {"This", "is", "an", "example", "of", "text", "justification."};
        int maxWidth = 16;

        vector<string> res = sol.fullJustify(words, maxWidth);
        for (const string& s : res) {
            cout << "\"" << s << "\"" << endl;
        }
    }
} // namespace Question68

// --- 第 69 题 ---
namespace Question69
{
    class Solution {
    public:
        int mySqrt(int x) {
            if (x < 2) return x;
            int left = 1, right = x / 2;
            int ans = 0;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (mid <= x / mid) {
                    ans = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            return ans;
        }

        int mySqrtNewton(int x) {
            if (x < 2) return x;
            long long r = x;
            while (r * r > x) {
                r = (r + x / r) / 2;
            }
            return (int)r;
        }
    };
    void test() {
        Solution sol;
        // Test Cases
        vector<int> inputs = {4, 8, 15, 2147483647};
        
        cout << "Input: [4, 8, 15, 2147483647]" << endl;
        cout << "Binary Search Results: ";
        for (int val : inputs) {
            cout << sol.mySqrt(val) << " ";
        }
        cout << endl;

        cout << "Newton's Method Results: ";
        for (int val : inputs) {
            cout << sol.mySqrtNewton(val) << " ";
        }
        cout << endl;
    }
} // namespace Question69

// --- 第 70 题 ---
namespace Question70
{
    class Solution {
    public:
        int bruteForce(int n) {
            if (n <= 2) return n;
            return bruteForce(n - 1) + bruteForce(n - 2);
        }
        int dpArray(int n) {
            if (n <= 2) return n;
            vector<int> dp(n + 1);
            dp[1] = 1;
            dp[2] = 2;
            for (int i = 3; i <= n; ++i) {
                dp[i] = dp[i - 1] + dp[i - 2];
            }
            return dp[n];
        }
        int climbStairs(int n) {
            if (n <= 2) return n;
            int first = 1;
            int second = 2;
            for (int i = 3; i <= n; ++i) {
                int current = first + second;
                first = second;
                second = current;
            }
            return second;
        }
    };
    void test() {
        Solution sol;
    
        // Test Cases
        int test_cases[] = {1, 2, 3, 4, 5, 10};
        
        cout << "Running Tests for Climbing Stairs:" << endl;
        cout << "----------------------------------" << endl;

        for (int n : test_cases) {
            cout << "n = " << n << ":" << endl;
            cout << "  Brute Force Result:      " << sol.bruteForce(n) << endl;
            cout << "  DP Array Result:         " << sol.dpArray(n) << endl;
            cout << "  Optimized (Final) Result: " << sol.climbStairs(n) << endl;
            cout << "----------------------------------" << endl;
        }
    }
} // namespace Question70

// --- 第 71 题 ---
namespace Question71
{
    class Solution {
    public:
        string simplifyPath(string path) {
            vector<string> stack;
            stringstream ss(path);
            string component;

            while (getline(ss, component, '/')) {
                if (component == "" || component == ".") {
                    continue;
                } else if (component == "..") {
                    if (!stack.empty()) {
                        stack.pop_back();
                    }
                } else {
                    stack.push_back(component);
                }
            }
            string result = "/";
            for (int i = 0; i < stack.size(); ++i) {
                result += stack[i];
                if (i != stack.size() - 1) {
                    result += "/";
                }
            }
            return result;
        }

        string simplifyPathBruteForce(string path) {
            vector<string> dirs;
            int n = path.length();
            for (int i = 0; i < n; ++i) {
                if (path[i] == '/') continue;
                string part = "";
                while (i < n && path[i] != '/') {
                    part += path[i];
                    i++;
                }
                if (part == ".") {
                    continue;
                } else if (part == "..") {
                    if (!dirs.empty()) dirs.pop_back();
                } else {
                    dirs.push_back(part);
                }
            }
            string res = "";
            for (const string& s : dirs) {
                res += "/" + s;
            }
            return res.empty() ? "/" : res;
        }
    };
    void test() {
        Solution sol;
        cout << (sol.simplifyPath("/home/") == "/home") << endl;
    }
} // namespace Question71

// --- 第 72 题 ---
namespace Question72
{
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            int m = word1.length();
            int n = word2.length();
            vector<vector<int>> dp(m + 1, vector<int>(n + 1));
            for (int i = 0; i <= m; ++i) dp[i][0] = i;
            for (int j = 0; j <= n; ++j) dp[0][j] = j;
            for (int i = 1; i <= m; ++i) {
                for (int j = 1; j <= n; ++j) {
                    if (word1[i - 1] == word2[j - 1]) {
                        dp[i][j] = dp[i - 1][j - 1];
                    } else {
                        dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
                    }
                }
            }
            return dp[m][n];
        }
        
        int minDistanceOptimized(string word1, string word2) {
            int m = word1.length();
            int n = word2.length();
            if (m < n) return minDistanceOptimized(word2, word1);
            vector<int> dp(n + 1);
            for (int j = 0; j <= n; ++j) dp[j] = j;
            for (int i = 1; i <= m; ++i) {
                int prev_diag = dp[0];
                dp[0] = i;
                for (int j = 1; j <= n; ++j) {
                    int temp = dp[j];
                    if (word1[i - 1] == word2[j - 1]) {
                        dp[j] = prev_diag;
                    } else {
                        dp[j] = 1 + min({prev_diag, dp[j], dp[j - 1]});
                    }
                    prev_diag = temp;
                }
            }
            return dp[n];
        }
    };
    void test() {
        Solution sol;
        cout << sol.minDistance("horse", "ros");
        cout << sol.minDistanceOptimized("horse", "ros") << endl;
    }
} // namespace Question72

// --- 第 73 题 ---
namespace Question73
{
class Solution {
public:
    void setZeroesBruteForce(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        vector<bool> row_flag(m, false);
        vector<bool> col_flag(n, false);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    row_flag[i] = true;
                    col_flag[j] = true;
                }
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (row_flag[i] || col_flag[j]) {
                    matrix[i][j] = 0;
                }
            }
        }
    }

    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        if (m == 0) return;
        int n = matrix[0].size();
        bool firstRowZero = false;
        bool firstColZero = false;
        for (int j = 0; j < n; ++j) {
            if (matrix[0][j] == 0) {
                firstRowZero = true;
                break;
            }
        }
        for (int i = 0; i < m; ++i) {
            if (matrix[i][0] == 0) {
                firstColZero = true;
                break;
            }
        }
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
            }
        }
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                    matrix[i][j] = 0;
                }
            }
        }
        if (firstRowZero) {
            for (int j = 0; j < n; ++j) {
                matrix[0][j] = 0;
            }
        }
        if (firstColZero) {
            for (int i = 0; i < m; ++i) {
                matrix[i][0] = 0;
            }
        }
    }
};
void test() {
    Solution sol;
    vector<vector<int>> matrix = {{0,1,2,0},{3,4,5,2},{6,7,8,1}};
    sol.setZeroes(matrix);
    using namespace Utils;
    printVec2(matrix);

    vector<vector<int>> matrix2 = {{0,1,2,0},{3,4,5,2},{6,7,8,1}};
    sol.setZeroesBruteForce(matrix2);
    printVec2(matrix2);

}
} // namespace Question73

// --- 第 74 题 ---
namespace Question74
{
class Solution {
public:
    bool searchMatrixBruteForce(vector<vector<int>>& matrix, int target) {
        for (const auto& row : matrix) {
            for (int val : row) {
                if (val == target) return true;
            }
        }
        return false;
    }

    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int m = matrix.size();
        int n = matrix[0].size();
        int left = 0;
        int right = m * n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int row = mid / n;
            int col = mid % n;
            if (matrix[row][col] == target) {
                return true;
            } else if (matrix[row][col] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return false;
    }

    bool searchMatrixTwoStep(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int m = matrix.size();
        int n = matrix[0].size();
        int low = 0, high = m - 1;
        int targetRow = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (matrix[mid][0] <= target && target <= matrix[mid][n - 1]) {
                targetRow = mid;
                break;
            } else if (matrix[mid][0] > target) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        if (targetRow == -1) return false;
        low = 0, high = n - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (matrix[targetRow][mid] == target) return true;
            else if (matrix[targetRow][mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return false;
    }
};
void test() {
    Solution sol;
    vector<vector<int>> matrix = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    cout << sol.searchMatrix(matrix, 3);
    cout << sol.searchMatrixBruteForce(matrix, 3);
    cout << sol.searchMatrixTwoStep(matrix, 3) << endl;
}
} // namespace Question74

// --- 第 75 题 ---
namespace Question75
{
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int count0 = 0, count1 = 0, count2 = 0;
        for (int num : nums) {
            if (num == 0) count0++;
            else if (num == 1) count1++;
            else count2++;
        }
        int i = 0;
        while (count0--) nums[i++] = 0;
        while (count1--) nums[i++] = 1;
        while (count2--) nums[i++] = 2;
    }
    void sortColors2(vector<int>& nums) {
        int low = 0;
        int mid = 0;
        int high = nums.size() - 1;
        while (mid <= high) {
            if (nums[mid] == 0) {
                swap(nums[low], nums[mid]);
                low++;
                mid++;
            } else if (nums[mid] == 1) {
                mid++;
            } else {
                swap(nums[mid], nums[high]);
                high--;
            }
        }
    }
};
void test() {
    Solution sol;
    vector<int> nums = {2, 0, 2, 1, 1, 0};
    sol.sortColors(nums);
    using namespace Utils;
    printVec(nums);
    nums = {2, 0, 2, 1, 1, 0};
    sol.sortColors2(nums);
    printVec(nums);
}
} // namespace Question75

// --- 第 76 题 ---
namespace Question76
{
class Solution {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> need;
        unordered_map<char, int> window;
        for (char c : t) {
            need[c]++;
        }
        int left = 0, right = 0;
        int valid = 0;
        int start = 0;
        int minLen = INT_MAX;
        int requiredNum = need.size();
        while (right < s.length()) {
            char c = s[right];
            right++;
            if (need.count(c)) {
                window[c]++;
                if (window[c] == need[c]) {
                    valid++;
                }
            }
            while (valid == requiredNum) {
                if (right - left < minLen) {
                    start = left;
                    minLen = right - left;
                }
                char d = s[left];
                left++;
                if (need.count(d)) {
                    if (window[d] == need[d]) {
                        valid--;
                    }
                    window[d]--;
                }
            }
        }
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }

    string minWindowOptimizedArray(string s, string t) {
        if (s.empty() || t.empty()) return "";
        int need[128] = {0};
        int window[128] = {0};
        int uniqueCharsInT = 0;
        for (char c : t) {
            if (need[c] == 0) uniqueCharsInT++;
            need[c]++;
        }
        int left = 0, right = 0, valid = 0;
        int start = 0, minLen = INT_MAX;
        while (right < s.length()) {
            int rChar = (unsigned char)s[right];
            right++;
            if (need[rChar] > 0) {
                window[rChar]++;
                if (window[rChar] == need[rChar]) valid++;
            }
            while (valid == uniqueCharsInT) {
                if (right - left < minLen) {
                    start = left;
                    minLen = right - left;
                }
                int lChar = (unsigned char)s[left];
                left++;
                if (need[lChar] > 0) {
                    if (window[lChar] == need[lChar]) valid--;
                    window[lChar]--;
                }
            }
        }
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
void test() {
    Solution sol;
    cout << sol.minWindowOptimizedArray("ADOBECODEBANC", "ABC") << endl;
    cout << sol.minWindow("ADOBECODEBANC", "ABC") << endl;
}
} // namespace Question76

// --- 第 77 题 ---
namespace Question77
{
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> currentPath;
        backtrack(1, n, k, currentPath, result);
        return result;
    }

    vector<vector<int>> combineBitManipulation(int n, int k) {
        vector<vector<int>> result;
        if (k > n) return result;
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (__builtin_popcount(mask) == k) {
                vector<int> combination;
                for (int i = 0; i < n; ++i) {
                    if ((mask >> i) & 1) {
                        combination.push_back(i + 1);
                    }
                }
                result.push_back(combination);
            }
        }
        return result;
    }

    vector<vector<int>> combineLexicograhical(int n, int k) {
        vector<vector<int>> result;
        if (k > n) return result;
        vector<int> comb(k);
        for (int i = 0; i < k; ++i) comb[i] = i + 1;
        while (true) {
            result.push_back(comb);
            int i = k - 1;
            while (i >= 0 && comb[i] == n - k + i + 1) {
                i--;
            }
            if (i < 0) break;
            comb[i]++;
            for (int j = i + 1; j < k; ++j) {
                comb[j] = comb[j - 1] + 1;
            }
        }
        return result;
    }
private:
    void backtrack(int start, int n, int k, vector<int>& currentPath, vector<vector<int>>& result) {
        if (currentPath.size() == k) {
            result.push_back(currentPath);
            return;
        }
        for (int i = start; i <= n - (k - currentPath.size()) + 1; ++i) {
            currentPath.push_back(i);
            backtrack(i + 1, n, k, currentPath, result);
            currentPath.pop_back();
        }
    }
};
void test() {
    Solution sol;
    vector<vector<int>> res = sol.combine(4, 2);
    vector<vector<int>> res1 = sol.combineBitManipulation(4, 2);
    vector<vector<int>> res2 = sol.combineLexicograhical(4, 2);
    using namespace Utils;
    printVec2(res);
    printVec2(res1);
    printVec2(res2);
}
} // namespace Question77

// --- 第 78 题 ---
namespace Question78
{
class Solution {
public:
    vector<vector<int>> subsetsBacktrack(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(result, current, nums, 0);
        return result;
    }

    vector<vector<int>> subsetsIterative(vector<int>& nums) {
        vector<vector<int>> result;
        result.reserve(1 << nums.size());
        for (int i = 0; i < (1 << nums.size()); ++i) {
            vector<int> subset;
            for (int j = 0; j < nums.size(); ++j) {
                if ((i >> j) & 1) {
                    subset.push_back(nums[j]);
                }
            }
            result.push_back(subset);
        }
        return result;
    }

    vector<vector<int>> subsetsOptimal(vector<int>& nums) {
        vector<vector<int>> result;
        result.push_back({});
        for (int num : nums) {
            int size = result.size();
            for (int i = 0; i < size; ++i) {
                vector<int> current = result[i];
                current.push_back(num);
                result.push_back(current);
            }
        }
        return result;
    }
private:
    void backtrack(vector<vector<int>>& result, vector<int>& current, const vector<int>& nums, int index) {
        result.push_back(current);
        for (int i = index; i < nums.size(); ++i) {
            current.push_back(nums[i]);
            backtrack(result, current, nums, i + 1);
            current.pop_back();
        }
    }
};
void test() {
    Solution sol;
    vector<int> nums = {1, 2, 3};
    using namespace Utils;
    printVec2(sol.subsetsBacktrack(nums));
    printVec2(sol.subsetsIterative(nums));
    printVec2(sol.subsetsOptimal(nums));
}
} // namespace Question78

// --- 第 79 题 ---
namespace Question79
{
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty()) return false;
        int rows = board.size(), cols = board[0].size();
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (dfs(board, r, c, word, 0)) return true;
            }
        }
        return false;
    }
private:
    bool dfs(vector<vector<char>>& board, int r, int c, const string& word, int idx) {
        if (idx == static_cast<int>(word.size())) return true;
        if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size()) return false;
        if (board[r][c] != word[idx]) return false;
        char tmp = board[r][c];
        board[r][c] = '\0';
        bool found = dfs(board, r + 1, c, word, idx + 1) ||
                     dfs(board, r - 1, c, word, idx + 1) ||
                     dfs(board, r, c + 1, word, idx + 1) ||
                     dfs(board, r, c - 1, word, idx + 1);
        board[r][c] = tmp;
        return found;
    }
};
void test() {
    Solution sol;
    vector<vector<char>> board = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };

    cout << sol.exist(board, "ABCCED") << endl;
}
} // namespace Question79

// --- 第 80 题 ---
namespace Question80
{
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() <= 2) return nums.size();
        for (size_t i = 2; i < nums.size(); ++i) {
            int count = 1;
            for (size_t j = i - 1; j >= 0 && j < nums.size(); --j) {
                if (nums[j] == nums[i]) count++;
            }
            if (count > 2) {
                nums.erase(nums.begin() + i);
                --i;
            }
        }
        return nums.size();
    }
    int removeDuplicatesOptimal(vector<int>& nums) {
        if (nums.size() <= 2) return nums.size();
        int i = 2;
        for (int j = 2; j < nums.size(); ++j) {
            if (nums[j]!= nums[i - 2]) {
                nums[i] = nums[j];
                i++;
            }
        }
        return i;
    }
};
void test() {
    Solution sol;
    vector<int> nums = {1, 1, 1, 2, 2, 3};
    cout << sol.removeDuplicates(nums) << endl;
    cout << sol.removeDuplicatesOptimal(nums) << endl;
}
} // namespace Question80

// --- 第 81 题 ---
namespace Question81
{
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return true;
            }
            if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
                left++;
                right--;
            } else if (nums[left] <= nums[mid]) {
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else {
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    if (nums[mid] < target && target <= nums[right]) {
                        left = mid + 1;
                    } else {
                        right = mid - 1;
                    }
                }
            }
        }
        return false;
    }
};
void test() {
    Solution sol;
    // Test Case 1: Standard rotation
    vector<int> nums1 = {2, 5, 6, 0, 0, 1, 2};
    // Expected: true (target 0 is present)
    cout << boolalpha << sol.search(nums1, 0) << endl;

    // Test Case 2: Target not present
    vector<int> nums2 = {2, 5, 6, 0, 0, 1, 2};
    // Expected: false (target 3 is not present)
    cout << boolalpha << sol.search(nums2, 3) << endl;

    // Test Case 3: Ambiguous case (duplicates at boundaries)
    vector<int> nums3 = {1, 0, 1, 1, 1};
    // Expected: true (target 0 is present)
    cout << boolalpha << sol.search(nums3, 0) << endl;

    // Test Case 4: Empty/Single element (implicit edge cases)
    vector<int> nums4 = {1};
    // Expected: true
    cout << boolalpha << sol.search(nums4, 1) << endl;
}
} // namespace Question81

// --- 第 82 题 ---
namespace Question82
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return nullptr;
        ListNode* dummy = new ListNode(0, head);
        ListNode* prev = dummy;
        while (head != nullptr) {
            if (head->next != nullptr && head->val == head ->next->val) {
                while (head->next != nullptr && head->val == head->next->val) {
                    ListNode* temp = head;
                    head = head->next;
                }
                prev->next = head->next;
            } else {
                prev = prev->next;
            }
            head = head->next;
        }
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }

    ListNode* deleteDuplicatesIterative(ListNode* head) {
        ListNode* dummy = new ListNode(0, head);
        ListNode* prev = dummy;
        ListNode* curr = head;
        while (curr != nullptr) {
            if (curr->next != nullptr && curr->val == curr->next->val) {
                int duplicateVal = curr->val;
                while (curr != nullptr && curr->val == duplicateVal) {
                    ListNode* toDelete = curr;
                    curr = curr->next;
                }
                prev->next = curr;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        ListNode* res = dummy->next;
        delete dummy;
        return res;
    }
};
void test() {
    Solution sol;
    using namespace Utils;
    ListNode* list = createList<ListNode>({1, 2, 3, 3, 4, 5});
    printList(sol.deleteDuplicates(list));
    printList(sol.deleteDuplicatesIterative(list));
}
} // namespace Question82

// --- 第 83 题 ---
namespace Question83
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return nullptr;
        ListNode* current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->val == current->next->val) {
                ListNode* temp = current->next;
                current->next = current->next->next;
            } else {
                current = current->next;
            }
        }
        return head;
    }

    ListNode* deleteDuplicatesRecursive(ListNode* head) {
        if (!head || !head->next) {
            return head;
        }
        head->next = deleteDuplicatesRecursive(head->next);
        if (head->val == head->next->val) {
            return head->next;
        }
        return head;
    }
};
void test() {
    Solution sol;
    using namespace Utils;
    ListNode* list = createList<ListNode>({1, 2, 3, 3, 4, 5});
    printList(sol.deleteDuplicates(list));
    printList(sol.deleteDuplicatesRecursive(list));
}
} // namespace Question83

// --- 第 84 题 ---
namespace Question84
{
class Solution {
public:
    int largestRectangleAreaMonotonicStack(vector<int>& heights) {
        int n = heights.size();
        stack<int> st;
        int maxArea = 0;
        for (int i = 0; i <= n; ++i) {
            int currentHeight = (i == n) ? 0 : heights[i];
            while (!st.empty() && currentHeight < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int width = st.empty() ? i : (i - st.top() - 1);
                maxArea = std::max(maxArea, height * width);
            }
            st.push(i);
        }
        return maxArea;
    }

    int largestRectangleAreaLinearScan(vector<int>& heights) {
        int n = heights.size();
        if (n == 0) return 0;
        vector<int> left_smaller(n);
        vector<int> right_smaller(n);
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && heights[st.top()] >= heights[i]) {
                st.pop();
            }
            left_smaller[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }
        while (!st.empty()) st.pop();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && heights[st.top()] >= heights[i]) {
                st.pop();
            }
            right_smaller[i] = st.empty() ? n : st.top();
            st.push(i);
        }
        int maxArea = 0;
        for (int i = 0; i < n; ++i) {
            int width = right_smaller[i] - left_smaller[i] - 1;
            maxArea = std::max(maxArea, heights[i] * width);
        }
        return maxArea;
    }
};
void test() {
    Solution sol;
    vector<int> heights = {4, 5, 3, 2, 1, 7, 8, 3, 1, 10, 90};
    cout << sol.largestRectangleAreaLinearScan(heights) << endl;
    cout << sol.largestRectangleAreaMonotonicStack(heights) << endl;
}
} // namespace Question84

// --- 第 85 题 ---
namespace Question85
{
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int rows = matrix.size();
        int cols = matrix[0].size();
        vector<int> heights(cols, 0);
        int maxTotalArea = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (matrix[i][j] == '1') {
                    heights[j] += 1;
                } else {
                    heights[j] = 0;
                }
            }
            maxTotalArea = max(maxTotalArea, largestRectangleArea(heights));
        }
        return maxTotalArea;
    }

    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        stack<int> st;
        int maxArea = 0;
        for (int i = 0; i <= n; ++i) {
            int currentHeight = (i == n) ? 0 : heights[i];
            while (!st.empty() && currentHeight < heights[st.top()]) {
                int h = heights[st.top()];
                st.pop();
                int w = st.empty() ? i : (i - st.top() - 1);
                maxArea = max(maxArea, h * w);
            }
            st.push(i);
        }
        return maxArea;
    }
};
void test() {
    Solution sol;
    vector<vector<char>> matrix = {
        {'1', '0', '1', '0', '0'},
        {'1', '0', '1', '1', '1'},
        {'1', '1', '1', '1', '1'},
        {'1', '0', '1', '1', '1'}
    };
    cout << sol.maximalRectangle(matrix) << endl;
}
} // namespace Question85

// --- 第 86 题 ---
namespace Question86
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode* smallHead = new ListNode(0);
        ListNode* largeHead = new ListNode(0);
        ListNode* small = smallHead;
        ListNode* large = largeHead;
        ListNode* curr = head;
        while (curr != nullptr) {
            if (curr->val < x) {
                small->next = curr;
                small = small->next;
            } else {
                large->next = curr;
                large = large->next;
            }
            curr = curr->next;
        }
        large->next = nullptr;
        small->next = largeHead->next;
        ListNode* result = smallHead->next;
        delete smallHead;
        delete largeHead;
        return result;
    }
};
void test() {
    Solution sol;
    using namespace Utils;
    ListNode* head = createList<ListNode>({1, 4, 3, 2, 5});
    printList(sol.partition(head, 3));
}
} // namespace Question86

// --- 第 87 题 ---
namespace Question87
{
class Solution {
public:
    bool isScramble(string s1, string s2) {
        if (s1 == s2) return true;
        if (s1.length() != s2.length()) return false;
        string key = s1 + "#" + s2;
        if (memo.count(key)) return memo[key];
        int n = s1.length();
        vector<int> count(26, 0);
        for (int i = 0; i < n; ++i) {
            count[s1[i] - 'a']++;
            count[s2[i] - 'a']--;
        }
        for (int i = 0; i < 26; ++i) {
            if (count[i] != 0) return memo[key] = false;
        }
        for (int i = 1; i < n; ++i) {
            bool noSwap = isScramble(s1.substr(0, i), s2.substr(0, i)) &&
                          isScramble(s1.substr(i), s2.substr(i));
            if (noSwap) return memo[key] = true;
            bool swap = isScramble(s1.substr(0, i), s2.substr(n - i)) &&
                          isScramble(s1.substr(i), s2.substr(0, n - i));
            if (swap) return memo[key] = true;
        }
        return memo[key] = false;
    }
private:
    unordered_map<string, bool> memo;
};
void test() {
    Solution sol;
    cout << boolalpha <<sol.isScramble("great", "rgeat") << endl;
}
} // namespace Question87

// --- 第 88 题 ---
namespace Question88
{
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1;
        int j = n - 1;
        int k = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j]) {
                nums1[k--] = nums1[i--];
            } else {
                nums1[k--] = nums2[j--];
            }
        }
        while (j >= 0) {
            nums1[k--] = nums2[j--];
        }
    }
};
void test() {
    Solution sol;
    vector<int> nums1 = {1, 2, 3, 0, 0, 0};
    int m = 3;
    vector<int> nums2 = {2, 5, 6};
    int n = 3;

    sol.merge(nums1, m, nums2, n);
    // 预期输出: 1 2 2 3 5 6
    cout << "Test Case 1: ";
    for (int num : nums1) cout << num << " ";
    cout << endl;
}
} // namespace Question88

// --- 第 89 题 ---
namespace Question89
{
class Solution {
public:
    vector<int> grayCode(int n) {
        int size = 1 << n;
        vector<int> result;
        result.reserve(size);
        for (int i = 0; i < size; ++i) {
            result.push_back(i ^ (i >> 1));
        }
        return result;
    }

    vector<int> grayCodeReflective(int n) {
        vector<int> res = {0};
        for (int i = 0; i < n; ++i) {
            int current_size = res.size();
            for (int j = current_size - 1; j >= 0; --j) {
                res.push_back(res[j] | (1 << i));
            }
        }
        return res;
    }
};
void test() {
    Solution sol;
    vector<int> res1 = sol.grayCode(2);
    for (int val : res1) cout << val << " ";
    cout << endl;
    vector<int> res2 = sol.grayCodeReflective(3);
    for (int val : res2) cout << val << " ";
    cout << endl;
}
} // namespace Question89

// --- 第 90 题 ---
namespace Question90
{
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> currentPath;
        sort(nums.begin(), nums.end());
        backtrack(nums, 0, currentPath, result);
        return result;
    }
private:
    void backtrack(vector<int>& nums, int startIndex, vector<int>& currentPath, vector<vector<int>>& result) {
        result.push_back(currentPath);
        for (int i = startIndex; i < nums.size(); ++i) {
            if (i > startIndex && nums[i] == nums[i - 1]) {
                continue;
            }
            currentPath.push_back(nums[i]);
            backtrack(nums, i + 1, currentPath, result);
            currentPath.pop_back();
        }
    }
};
void test() {
    Solution sol;
    vector<int> nums1 = {1, 2, 2};
    vector<vector<int>> res1 = sol.subsetsWithDup(nums1);

    for (const auto& subset : res1) {
        cout << "[ ";
        for (int num : subset) cout << num << " ";
        cout << "] ";
    }
    cout << endl << endl;
}
} // namespace Question90

// --- 第 91 题 ---
namespace Question91
{
class Solution {
public:
    int numDecodings(string s) {
        int n = s.length();
        if (n == 0 || s[0] == '0') return 0;
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            int singleDigit = s[i - 1] - '0';
            if (singleDigit >= 1 && singleDigit <= 9) {
                dp[i] += dp[i - 1];
            }
            int doubleDigit = stoi(s.substr(i - 2, 2));
            if (doubleDigit >= 10 && doubleDigit <= 26) {
                dp[i] += dp[i - 2];
            }
        }
        return dp[n];
    }
};
void test() {
    Solution sol;
    cout << "Test Case s='9': " << sol.numDecodings("9") << endl;
    cout << "Test Case s='226': " << sol.numDecodings("226") << endl;
}
} // namespace Question91

// --- 第 92 题 ---
namespace Question92
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
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
void test() {
    Solution sol;
    using namespace Utils;
    ListNode* list = createList<ListNode>({1, 2, 3, 4, 5});
    printList(sol.reverseBetween(list, 2, 4));
}
} // namespace Question92

// --- 第 93 题 ---
namespace Question93
{
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        vector<string> path;
        backtrack(s, 0, path, result);
        return result;
    }
private:
    void backtrack(const string& s, int start, vector<string>& path, vector<string>& result) {
        if (path.size() == 4) {
            if (start == s.length()) {
                string ip = path[0] + "." + path[1] + "." + path[2] + "." + path[3];
                result.push_back(ip);
            }
            return;
        }
        int remainingSegments = 4 - path.size();
        int remainingChars = s.length() - start;
        if (remainingChars < remainingSegments || remainingChars > remainingSegments * 3) {
            return;
        }
        for (int len = 1; len <= 3; ++len) {
            if (start + len > s.length()) break;
            string segment = s.substr(start, len);
            if (isValid(segment)) {
                path.push_back(segment);
                backtrack(s, start + len, path, result);
                path.pop_back();
            }
        }
    }

    bool isValid(const string& segment) {
        if (segment.length() > 1 && segment[0] == '0') {
            return false;
        }

        int val = stoi(segment);
        if (val > 255) {
            return false;
        }
        return true;
    }
};
void test() {
    Solution sol;
    vector<string> res1 = sol.restoreIpAddresses("255259");
    for (const string& ip : res1) cout << ip << endl;
}
} // namespace Question93

// --- 第 94 题 ---
namespace Question94
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<int> inorderTraversalRecursive(TreeNode* root) {
        vector<int> res;
        traverse(root, res);
        return res;
    }

    vector<int> inorderTraversalIterative(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> st;
        TreeNode* curr = root;
        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            res.push_back(curr->val);
            curr = curr->right;
        }
        return res;
    }
private:
    void traverse(TreeNode* node, vector<int>& res) {
        if (node == nullptr) return;
        traverse(node->left, res);
        res.push_back(node->val);
        traverse(node->right, res);
    }
};
void test() {
    Solution sol;

    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);

    vector<int> res1 = sol.inorderTraversalIterative(root);
    vector<int> res2 = sol.inorderTraversalRecursive(root);

    for (int x : res1) cout << x << " ";
    cout << endl;
    for (int x : res2) cout << x << " ";
    cout << endl;
    delete root->right->left;
    delete root->right;
    delete root;
}
} // namespace Question94

// --- 第 95 题 ---
namespace Question95
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};
        return buildTrees(1, n);
    }
private:
    vector<TreeNode*> buildTrees(int start, int end) {
        vector<TreeNode*> res;
        if (start > end) {
            return res;
        }
        for (int i = start; i <= end; ++i) {
            vector<TreeNode*> leftSubtrees = buildTrees(start, i - 1);
            vector<TreeNode*> rightSubtrees = buildTrees(i + 1, end);
            if (leftSubtrees.empty() && rightSubtrees.empty()) {
                res.push_back(new TreeNode(i));
            } else if (leftSubtrees.empty()) {
                for (TreeNode* r : rightSubtrees) {
                    TreeNode* root = new TreeNode(i);
                    root->right = r;
                    res.push_back(root);
                }
            } else if (rightSubtrees.empty()) {
                for (TreeNode* l : leftSubtrees) {
                    TreeNode* root = new TreeNode(i);
                    root->left = l;
                    res.push_back(root);
                }
            } else {
                for (TreeNode* l : leftSubtrees) {
                    for (TreeNode* r : rightSubtrees) {
                        TreeNode* root = new TreeNode(i);
                        root->left = l;
                        root->right = r;
                        res.push_back(root);
                    }
                }
            }
        }
        return res;
    }
};
void test() {
    Solution sol;
    vector<TreeNode*> trees = sol.generateTrees(3);
    cout << "Generated " << trees.size() << endl;
}
} // namespace Question95

// --- 第 96 题 ---
namespace Question96
{
class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            for (int j = 1; j <= i; ++j) {
                dp[i] += dp[j - 1] * dp[i - j];
            }
        }
        return dp[n];
    }
};
void test() {
    Solution sol;
    cout << sol.numTrees(4) << endl;
}
} // namespace Question96

// --- 第 97 题 ---
namespace Question97
{
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.length();
        int n = s2.length();
        if (m + n != s3.length()) {
            return false;
        }
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;
        for (int i = 1; i <= m; ++i) {
            dp[i][0] = dp[i - 1][0] && (s1[i - 1] == s3[i - 1]);
        }
        for (int j = 1; j <= n; ++j) {
            dp[0][j] = dp[0][j - 1] && (s2[j - 1] == s3[j - 1]);
        }
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                bool fromS1 = dp[i - 1][j] && (s1[i - 1] == s3[i + j - 1]);
                bool fromS2 = dp[i][j - 1] && (s2[j - 1] == s3[i + j - 1]);
                dp[i][j] = fromS1 || fromS2;
            }
        }
        return dp[m][n];
    }
};
void test() {
    Solution sol;
    cout << boolalpha << sol.isInterleave("aab", "abc", "aabbc") << endl;
}
} // namespace Question97

// --- 第 98 题 ---
namespace Question98
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    bool isValidBST2(TreeNode* root) {
        return validate(root, LONG_MIN, LONG_MAX);
    }
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        if (!isValidBST(root->left)) return false;
        if (root->val <= prev) return false;
        prev = root->val;
        return isValidBST(root->right);
    }
private:
    long long prev = LONG_MIN;
    bool validate(TreeNode* node, long long min_val, long long max_val) {
        if (node == nullptr) {
            return true;
        }
        if (node->val <= min_val || node->val >= max_val) {
            return false;
        }
        return validate(node->left, min_val, node->val) &&
               validate(node->right, node->val, max_val);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question98

// --- 第 99 题 ---
namespace Question99
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    void recoverTree(TreeNode* root) {
        traverse(root);
        if (first && second) {
            swap(first->val, second->val);
        }
    }
private:
    TreeNode* first = nullptr;
    TreeNode* second = nullptr;
    TreeNode* prev = nullptr;
    void traverse(TreeNode* curr) {
        if (curr == nullptr) return;
        traverse(curr->left);
        if (prev != nullptr && curr->val < prev->val) {
            if (first == nullptr) {
                first = prev;
            }
            second = curr;
        }
        prev = curr;
        traverse(curr->right);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question99

// --- 第 100 题 ---
namespace Question100
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (p == nullptr && q == nullptr) {
            return true;
        }
        if (p == nullptr || q == nullptr) {
            return false;
        }
        if (p->val != q->val) {
            return false;
        }
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question100

// --- 第 101 题 ---
namespace Question101
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (root == nullptr) return true;
        return check(root->left, root->right);
    }
private:
    bool check(TreeNode* p, TreeNode* q) {
        if (p == nullptr && q == nullptr) return true;
        if (p == nullptr || q == nullptr) return false;
        if (p->val != q->val) return false;
        return check(p->left, q->right) && check(p->right, q->left);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question101

// --- 第 102 题 ---
namespace Question102
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (root == nullptr) {
            return result;
        }
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            vector<int> currentLevel;
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q.front();
                q.pop();
                currentLevel.push_back(node->val);
                if (node->left != nullptr) {
                    q.push(node->left);
                }
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }
            result.push_back(currentLevel);
        }
        return result;
    }
};
void test() {
    Solution sol;
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    vector<vector<int>> res = sol.levelOrder(root);
    using namespace Utils;
    printVec2(res);
}
} // namespace Question102

// --- 第 103 题 ---
namespace Question103
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root) return {};
        vector<vector<int>> result;
        queue<TreeNode*> q;
        q.push(root);
        bool is_reverse = false;
        while (!q.empty()) {
            int size = q.size();
            vector<int> current_level(size);
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q.front();
                q.pop();
                if (!is_reverse) {
                    current_level[i] = node->val;
                } else {
                    current_level[size - 1 - i] = node->val;
                }
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(current_level);
            is_reverse = !is_reverse;
        }
        return result;
    }
};
void test() {
    Solution sol;
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    vector<vector<int>> res = sol.zigzagLevelOrder(root);
    using namespace Utils;
    printVec2(res);
}
} // namespace Question103

// --- 第 104 题 ---
namespace Question104
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);
        return max(leftDepth, rightDepth) + 1;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question104

// --- 第 105 题 ---
namespace Question105
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderMap.clear();
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap[inorder[i]] = i;
        }
        return buildTreeHelper(preorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
    }
private:
    unordered_map<int, int>inorderMap;
    TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd, int inStart, int inEnd) {
        if (preStart > preEnd || inStart > inEnd) {
            return nullptr;
        }
        int rootVal = preorder[preStart];
        TreeNode* root = new TreeNode(rootVal);
        int rootIndexInorder = inorderMap[rootVal];
        int leftSubtreeSize = rootIndexInorder - inStart;
        root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftSubtreeSize, inStart, rootIndexInorder - 1);
        root->right = buildTreeHelper(preorder, preStart + leftSubtreeSize + 1, preEnd, rootIndexInorder + 1, inEnd);
        return root;
    }   
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question105

// --- 第 106 题 ---
namespace Question106
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        inorderMap.clear();
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap[inorder[i]] = i;
        }
        return buildTreeHelper(inorder, postorder,
            0, inorder.size() - 1, 0, postorder.size() - 1);
    }
private:
    unordered_map<int, int> inorderMap;
    TreeNode* buildTreeHelper(const vector<int>& inorder,
                              const vector<int>& postorder,
                              int inStart, int inEnd,
                              int postStart, int postEnd) {
        if (inStart > inEnd || postStart > postEnd) {
            return nullptr;
        }
        int rootVal = postorder[postEnd];
        TreeNode* root = new TreeNode(rootVal);
        int rootIndexInorder = inorderMap[rootVal];
        int leftSubtreeSize = rootIndexInorder - inStart;
        root->left = buildTreeHelper(inorder, postorder,
            inStart, rootIndexInorder - 1, postStart, postStart + leftSubtreeSize - 1);
        root->right = buildTreeHelper(inorder, postorder,
            rootIndexInorder + 1, inEnd, postStart + leftSubtreeSize, postEnd - 1);
        return root;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question106

// --- 第 107 题 ---
namespace Question107
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        if (!root) {
            return {};
        }
        queue<TreeNode*> q;
        q.push(root);
        vector<vector<int>> result;
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevelNodes;
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                currentLevelNodes.push_back(node->val);
                if (node->left) {
                    q.push(node->left);
                }
                if (node->right) {
                    q.push(node->right);
                }
            }
            result.push_back(currentLevelNodes);
        }
        reverse(result.begin(), result.end());
        return result;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question107

// --- 第 108 题 ---
namespace Question108
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return buildBSTHelper(nums, 0, nums.size() - 1);
    }
private:
    TreeNode* buildBSTHelper(const vector<int>& nums, int start, int end) {
        if (start > end) {
            return nullptr;
        }
        int mid = start + (end - start) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = buildBSTHelper(nums, start, mid - 1);
        root->right = buildBSTHelper(nums, mid + 1, end);
        return root;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question108

// --- 第 109 题 ---
namespace Question109
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) {
            return nullptr;
        }
        vector<int> nums;
        ListNode* current = head;
        while (current != nullptr) {
            nums.push_back(current->val);
            current = current->next;
        }
        return buildBSTHelper(nums, 0, nums.size() - 1);
    }
private:
    TreeNode* buildBSTHelper(const vector<int>& nums, int start, int end) {
        if (start > end) {
            return nullptr;
        }
        int mid = start + (end - start) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = buildBSTHelper(nums, start, mid - 1);
        root->right = buildBSTHelper(nums, mid + 1, end);
        return root;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question109

// --- 第 110 题 ---
namespace Question110
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    bool isBanlanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
private:
    int checkHeight(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        int left_h = checkHeight(root->left);
        int right_h = checkHeight(root->right);
        if (left_h == -1 || right_h == -1) {
            return -1;
        }
        if (abs(left_h - right_h) > 1) {
            return -1;
        }
        return 1 + max(left_h, right_h);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question110

// --- 第 111 题 ---
namespace Question111
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int minDepthBFS(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        queue<pair<TreeNode*, int>> q;
        q.push({root, 1});
        while (!q.empty()) {
            auto [node, depth] = q.front();
            q.pop();
            if (node->left == nullptr && node->right == nullptr) {
                return depth;
            }
            if (node->left != nullptr) {
                q.push({node->left, depth + 1});
            }
            if (node->right != nullptr) {
                q.push({node->right, depth + 1});
            }
        }
        return 0;
    }

    int minDepthDFS(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        if (root->left == nullptr && root->right == nullptr) {
            return 1;
        }
        int left_depth = minDepthDFS(root->left);
        int right_depth = minDepthDFS(root->right);
        if (root->left == nullptr) {
            return right_depth + 1;
        }
        if (root->right == nullptr) {
            return left_depth + 1;
        }
        return min(left_depth, right_depth) + 1;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question111

// --- 第 112 题 ---
namespace Question112
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (root == nullptr) {
            return false;
        }
        if (root->left == nullptr && root->right == nullptr) {
            return root->val == targetSum;
        }
        int nextSum = targetSum - root->val;
        return hasPathSum(root->left, nextSum) || hasPathSum(root->right, nextSum);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question112

// --- 第 113 题 ---
namespace Question113
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> result;
        vector<int> currentPath;
        backtrack(root, targetSum, currentPath, result);
        return result;
    }
private:
    void backtrack(TreeNode* node, int remainingSum, vector<int>& currentPath, vector<vector<int>>& result) {
        if (node == nullptr) {
            return;
        }
        currentPath.push_back(node->val);
        if (node->left == nullptr && node->right == nullptr) {
            if (node->val == remainingSum) {
                result.push_back(currentPath);
            }
        } else {
            backtrack(node->left, remainingSum - node->val, currentPath, result);
            backtrack(node->right, remainingSum - node->val, currentPath, result);
        }
        currentPath.pop_back();
    }
};
void test() {
    Solution sol;
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(6);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);
    root->right->right = new TreeNode(7);

    vector<vector<int>> result = sol.pathSum(root, 22);
    for (const auto& path : result) {
        cout << "[";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i == path.size() - 1 ? "" : ",");
        }
        cout << "] ";
    }
    cout << endl;
}
} // namespace Question113

// --- 第 114 题 ---
namespace Question114
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    void flatten(TreeNode* root) {
        if (root == nullptr) return;
        flatten(root->right);
        flatten(root->left);
        root->right = prev;
        root->left = nullptr;
        prev = root;
    }
private:
    TreeNode* prev = nullptr;
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question114

// --- 第 115 题 ---
namespace Question115
{
class Solution {
public:
    int numDistinct(string s, string t) {
        int m = s.length();
        int n = t.length();
        vector<vector<unsigned long long>> dp(m + 1, vector<unsigned long long>(n + 1, 0));
        for (int i = 0; i <= m; ++i) {
            dp[i][0] = 1;
        }
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }
        return (int)dp[m][n];
    }

    int numDistinct2(string s, string t) {
        int m = s.length();
        int n = t.length();
        vector<unsigned long long> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= m; ++i) {
            for (int j = n; j >= 1; --j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[j] = dp[j] + dp[j - 1];
                }
            }
        }
        return (int)dp[n];
    }
};
void test() {
    Solution sol;
    cout << sol.numDistinct("rabbbit", "rabbit") << endl;
    cout << sol.numDistinct2("rabbbit", "rabbit") << endl;
}
} // namespace Question115

// --- 第 116 题 ---
namespace Question116
{
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
class Solution {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        Node* leftmost = root;
        while (leftmost->left) {
            Node* curr = leftmost;
            while (curr) {
                curr->left->next = curr->right;
                if (curr->next) {
                    curr->right->next = curr->next->left;
                }
                curr = curr->next;
            }
            leftmost = leftmost->left;
        }
        return root;
    }
};
void test() {
    Solution sol;
    // 测试用例：构造一个完美二叉树并测试
    //      1 -> 2 -> 3 -> NULL
    //     / \  / \ / \
    //    4   5 6   7
    cout << "Starting test..." << endl;
    Node* root = new Node(1);
    cout << "Starting test...2" << endl;
    root->left = new Node(2);
    cout << "Starting test...3" << endl;
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    cout << "Starting test...4" << endl;
    sol.connect(root);
    // 验证结果
    cout << "Starting test...5" << endl;
    // cout << "Level 1: " << root->val << " -> " << root->next->val << endl; 
    
    if (root != nullptr) {
        cout << "Level 1: " << root->val;
        if (root->next != nullptr) {
            cout << " -> " << root->next->val;
        } else {
            cout << " -> (end of line)";
        }
        cout << endl;
    } else {
        cout << "Root is null" << endl;
    }

    cout << "Starting test...6" << endl;
    // 预期: 1 -> 2 -> 3
    cout << "Level 2: " << root->left->val << " -> " << nullptr << endl; // 4 -> 5 -> 6 -> 7
}
} // namespace Question116

// --- 第 117 题 ---
namespace Question117
{
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
class Solution {
public:
    Node* connectBFS(Node* root) {
        if (!root) return nullptr;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            Node* prev = nullptr;
            for (int i = 0; i < size; ++i) {
                Node* curr = q.front();
                q.pop();
                if (prev != nullptr) {
                    prev->next = curr;
                }
                prev = curr;
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            prev->next = nullptr;
        }
        return root;
    }

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
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question117

// --- 第 118 题 ---
namespace Question118
{
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res;
        for (int i = 0; i < numRows; ++i) {
            vector<int> row(i + 1, 1);
            for (int j = 1; j < i; ++j) {
                row[j] = res[i - 1][j - 1] + res[i - 1][j];
            }
            res.push_back(row);
        }
        return res;
    }
};
void test() {
    Solution sol;
    int n = 5;
    vector<vector<int>> result = sol.generate(n);
    for (const auto& row : result) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}
} // namespace Question118

// --- 第 119 题 ---
namespace Question119
{
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> row(rowIndex + 1, 0);
        row[0] = 1;
        for (int i = 1; i <= rowIndex; ++i) {
            for (int j = i; j >= 1; --j) {
                row[j] = row[j] + row[j - 1];
            }
        }
        return row;
    }
};
void test() {
    Solution sol;
    vector<int> res = sol.getRow(10);
    for (int x : res) cout << x << " ";
    cout << endl;
}
} // namespace Question119

// --- 第 120 题 ---
namespace Question120
{
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<int> dp = triangle[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            for (int j = 0; j <= i; ++j) {
                dp[j] = triangle[i][j] + min(dp[j], dp[j + 1]);
            }
        }
        return dp[0];
    }
};
void test() {
    Solution sol;
    // 测试用例 1: [[2],[3,4],[6,5,7],[4,1,8,3]] -> 结果应该是 11 (2+3+5+1)
    vector<vector<int>> t1 = {{2}, {3, 4}, {6, 5, 7}, {4, 1, 8, 3}};
    cout << "Test 1: " << sol.minimumTotal(t1) << endl; // Output: 11

    // 测试用例 2: [[0],[1,2],[3,4,5],[6,7,8,9]] -> 结果应该是 11 (0+1+3+7)
    vector<vector<int>> t2 = {{0}, {1, 2}, {3, 4, 5}, {6, 7, 8, 9}};
    cout << "Test 2: " << sol.minimumTotal(t2) << endl; // Output: 11
}
} // namespace Question120

// --- 第 121 题 ---
namespace Question121
{
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        int minPrice = prices[0];
        int maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            if (prices[i] < minPrice) {
                minPrice = prices[i];
            } else {
                int currentProfit = prices[i] - minPrice;
                if (currentProfit > maxProfit) {
                    maxProfit = currentProfit;
                }
            }
        }
        return maxProfit;
    }
};
void test() {
    Solution sol;
    
    // 测试用例 1: [7,1,5,3,6,4] -> 结果为 5 (在价格为 1 时买入，在 6 时卖出)
    vector<int> p1 = {7, 1, 5, 3, 6, 4};
    cout << "Test 1: " << sol.maxProfit(p1) << endl; // Expected: 5

    // 测试用例 2: [7,6,4,3,1] -> 结果为 0 (价格一直在跌，无法获利)
    vector<int> p2 = {7, 6, 4, 3, 1};
    cout << "Test 2: " << sol.maxProfit(p2) << endl; // Expected: 0
    
    // 测试用例 3: [2,4,1] -> 结果为 2 (在 2 买入，在 4 卖出)
    vector<int> p3 = {2, 4, 1};
    cout << "Test 3: " << sol.maxProfit(p3) << endl; // Expected: 2
}
} // namespace Question121

// --- 第 122 题 ---
namespace Question122
{
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int totalProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            if (prices[i] > prices[i - 1]) {
                totalProfit += (prices[i] - prices[i - 1]);
            }
        }
        return totalProfit;
    }
};
void test() {
    Solution sol;

    // 测试用例 1: [7,1,5,3,6,4]
    // 利润：(5-1) + (6-3) = 4 + 3 = 7
    vector<int> p1 = {7, 1, 5, 3, 6, 4};
    cout << "Test 1: " << sol.maxProfit(p1) << endl; // Expected: 7

    // 测试用例 2: [1,2,3,4,5]
    // 利润：(2-1) + (3-2) + (4-3) + (5-4) = 1+1+1+1 = 4
    // 这等同于第一天买，第五天卖的利润
    vector<int> p2 = {1, 2, 3, 4, 5};
    cout << "Test 2: " << sol.maxProfit(p2) << endl; // Expected: 4

    // 测试用例 3: [7,6,4,3,1]
    // 价格一直跌，利润为 0
    vector<int> p3 = {7, 6, 4, 3, 1};
    cout << "Test 3: " << sol.maxProfit(p3) << endl; // Expected: 0
}
} // namespace Question122

// --- 第 123 题 ---
namespace Question123
{
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        int buy1 = -prices[0];
        int sell1 = 0;
        int buy2 = -prices[0];
        int sell2 = 0;
        for (int i = 1; i < prices.size(); ++i) {
            buy1 = max(buy1, -prices[i]);
            sell1 = max(sell1, buy1 + prices[i]);
            buy2 = max(buy2, sell1 - prices[i]);
            sell2 = max(sell2, buy2 + prices[i]);
        }
        return sell2;
    }
};
void test() {
    Solution sol;
    vector<int> list = {1,2,3,4,5,6,7};
    cout << sol.maxProfit(list) << endl;
}
} // namespace Question123

// --- 第 124 题 ---
namespace Question124
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int maxPathSum(TreeNode* root) {
        maxSum = INT_MIN;
        maxGain(root);
        return maxSum;
    }
private:
    int maxSum = INT_MIN;
    int maxGain(TreeNode* node) {
        if (node == nullptr) return 0;
        int leftGain = max(0, maxGain(node->left));
        int rightGain = max(0, maxGain(node->right));
        int currentPathSum = node->val + leftGain + rightGain;
        maxSum = max(maxSum, currentPathSum);
        return node->val + max(leftGain, rightGain);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question124

// --- 第 125 题 ---
namespace Question125
{
class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0;
        int right = s.length() - 1;
        while (left < right) {
            while (left < right && !isalnum(s[left])) {
                left++;
            }
            while (left < right && !isalnum(s[right])) {
                right--;
            }
            if (left < right) {
                if (tolower(s[left]) != tolower(s[right])) {
                    return false;
                }
                left++;
                right--;
            }
        }
        return true;
    }
};
void test() {
    Solution sol;
    cout << boolalpha << sol.isPalindrome("A man, a plan, a canal: Panama") << endl; // 预期: true
  
}
} // namespace Question125

// --- 第 126 题 ---
namespace Question126
{
class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        vector<vector<string>> res;
        if (dict.find(endWord) == dict.end()) {
            return res;
        }
        unordered_map<string, int> dist;
        unordered_map<string, vector<string>> adj;
        queue<string> q;
        q.push(beginWord);
        dist[beginWord] = 0;
        bool found = false;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curr = q.front();
                q.pop();
                if (found && curr == endWord) break;
                string next_word = curr;
                for (int j = 0; j < next_word.size(); ++j) {
                    char original_char = next_word[j];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        next_word[j] = c;
                        if (dict.count(next_word)) {
                            if (dist.find(next_word) == dist.end()) {
                                dist[next_word] = dist[curr] + 1;
                                adj[next_word].push_back(curr);
                                q.push(next_word);
                                if (next_word == endWord) found = true;
                            } else if (dist[next_word] == dist[curr] + 1) {
                                adj[next_word].push_back(curr);
                            }
                        }
                    }
                    next_word[j] = original_char;
                }
            }
            if (found) break;
        }
        if (dist.find(endWord) == dist.end()) {
            return res;
        }
        vector<string> path = {endWord};
        dfs(endWord, beginWord, adj, path, res);
        return res;
    }
private:
    void dfs(const string& curr, const string& beginWord,
             unordered_map<string, vector<string>>& adj,
             vector<string>& path, vector<vector<string>>& res) {
        if (curr == beginWord) {
            vector<string> completePath = path;
            reverse(completePath.begin(), completePath.end());
            res.push_back(completePath);
            return;
        }
        if (adj.count(curr)) {
            for (const string& prev : adj[curr]) {
                path.push_back(prev);
                dfs(prev, beginWord, adj, path, res);
                path.pop_back();
            }
        }
    }
};
void test() {
    Solution sol;
    vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};
    vector<vector<string>> result = sol.findLadders("hit", "cog", wordList);
    // Expected: [["hit","hot","dot","dog","cog"],["hit","hot","lot","log","cog"]]
    for (const auto& path : result) {
        for (const string& word : path) {
            cout << word << " ";
        }
        cout << endl;
    }
}
} // namespace Question126

// --- 第 127 题 ---
namespace Question127
{
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (dict.find(endWord) == dict.end()) {
            return 0;
        }
        queue<pair<string, int>> q;
        q.push({beginWord, 1});
        unordered_set<string> visited;
        visited.insert(beginWord);
        while (!q.empty()) {
            string currentWord = q.front().first;
            int distance = q.front().second;
            q.pop();
            if (currentWord == endWord) {
                return distance;
            }
            for (int i = 0; i < currentWord.size(); ++i) {
                char originalChar = currentWord[i];
                for (char c = 'a'; c <= 'z'; ++c) {
                    if (c == originalChar) continue;
                    currentWord[i] = c;
                    if (dict.count(currentWord) && !visited.count(currentWord)) {
                        visited.insert(currentWord);
                        q.push({currentWord, distance + 1});
                    }
                }
                currentWord[i] = originalChar;
            }
        }
        return 0;
    }

    int ladderLengthOptimaized(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (dict.find(endWord) == dict.end()) return 0;
        unordered_set<string> beginSet, endSet;
        beginSet.insert(beginWord);
        endSet.insert(endWord);
        int distance = 1;
        while (!beginSet.empty() && !endSet.empty()) {
            if (beginSet.size() > endSet.size()) {
                swap(beginSet, endSet);
            }
            unordered_set<string> nextSet;
            for (string word : beginSet) {
                for (int i = 0; i < word.size(); ++i) {
                    char originalChar = word[i];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (c == originalChar) continue;
                        word[i] = c;
                        if (endSet.count(word)) {
                            return distance + 1;
                        }
                        if (dict.count(word)) {
                            nextSet.insert(word);
                            dict.erase(word);
                        }
                    }
                    word[i] = originalChar;
                }
            }
            beginSet = std::move(nextSet);
            distance++;
        }
        return 0;
    }
};
void test() {
    Solution sol;
    vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};
    string beginWord = "hit";
    string endWord = "cog";
    cout << "Shortest path length: " << sol.ladderLength(beginWord, endWord, wordList) << endl; 
    cout << "Shortest path length: " << sol.ladderLengthOptimaized(beginWord, endWord, wordList) << endl; 
}
} // namespace Question127

// --- 第 128 题 ---
namespace Question128
{
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> numSet(nums.begin(), nums.end());
        int longestStreak = 0;
        for (int num : numSet) {
            if (numSet.find(num - 1) == numSet.end()) {
                int currentNum = num;
                int currentStreak = 1;
                while (numSet.find(currentNum + 1) != numSet.end()) {
                    currentNum += 1;
                    currentStreak += 1;
                }
                longestStreak = max(longestStreak, currentStreak);
            }
        }
        return longestStreak;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    cout << "Test 1: " << sol.longestConsecutive(nums1) << " (Expected: 4)" << endl; 
    // 解释: [1, 2, 3, 4]

    // 测试用例 2
    vector<int> nums2 = {0, 3, 7, 2, 5, 8, 4, 6, 0};
    cout << "Test 2: " << sol.longestConsecutive(nums2) << " (Expected: 9)" << endl; 
    // 解释: [0, 1, 2, 3, 4, 5, 6, 7, 8] (注意0重复了)

    // 测试用例 3 (空数组)
    vector<int> nums3 = {};
    cout << "Test 3: " << sol.longestConsecutive(nums3) << " (Expected: 0)" << endl;
}
} // namespace Question128

// --- 第 129 题 ---
namespace Question129
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int sumNumbers(TreeNode* root) {
        return dfs(root, 0);
    }
private:
    int dfs(TreeNode* node, int currentSum) {
        if (node == nullptr) {
            return 0;
        }
        int sum = currentSum * 10 + node->val;
        if (node->left == nullptr && node->right == nullptr) {
            return sum;
        }
        return dfs(node->left, sum) + dfs(node->right, sum);
    }
};
void test() {
    Solution sol;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(5);

    cout << "Sum of root-to-leaf numbers: " << sol.sumNumbers(root) << endl; 
}
} // namespace Question129

// --- 第 130 题 ---
namespace Question130
{
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;
        int rows = board.size();
        int cols = board[0].size();
        for (int i = 0; i < rows; ++i) {
            dfs(board, i, 0);
            dfs(board, i, cols - 1);
        }
        for (int j = 0; j < cols; ++j) {
            dfs(board, 0, j);
            dfs(board, rows - 1, j);
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (board[i][j] == 'O') {
                    board[i][j] = 'X';
                } else if (board[i][j] == '#') {
                    board[i][j] = 'O';
                }
            }
        }
    }
private:
    void dfs(vector<vector<char>>& board, int r, int c) {
        int rows = board.size();
        int cols = board[0].size();
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != 'O') {
            return;
        }
        board[r][c] = '#';
        dfs(board, r + 1, c);
        dfs(board, r - 1, c);
        dfs(board, r, c + 1);
        dfs(board, r, c - 1);
    }
};
void test() {
    Solution sol;
    vector<vector<char>> board1 = {
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'X'},
        {'X', 'O', 'X', 'X'}
    };
    
    cout << "Original Board 1:" << endl;
    using namespace Utils;
    printVec2(board1);
    sol.solve(board1);
    cout << "Solved Board 1:" << endl;
    printVec2(board1);
}
} // namespace Question130

// --- 第 131 题 ---
namespace Question131
{
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> currentPath;
        backtrack(s, 0, currentPath, result);
        return result;
    }
private:
    void backtrack(const string& s, int start, vector<string>& currentPath, vector<vector<string>>& result) {
        if (start == s.size()) {
            result.push_back(currentPath);
            return;
        }
        for (int i = start; i < s.size(); ++i) {
            if (isPalindrome(s, start, i)) {
                string sub = s.substr(start, i - start + 1);
                currentPath.push_back(sub);
                backtrack(s, i + 1, currentPath, result);
                currentPath.pop_back();
            }
        }
    }

    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
};
void test() {
    Solution sol;
    string s1 = "aab";
    vector<vector<string>> res1 = sol.partition(s1);
    cout << "Input: aab" << endl;
    for (auto& path : res1) {
        cout << "[ ";
        for (auto& str : path) cout << str << " ";
        cout << "]" << endl;
    }
}
} // namespace Question131

// --- 第 132 题 ---
namespace Question132
{
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        if (n <= 1) return 0;
        vector<vector<bool>> isPal(n, vector<bool>(n, false));
        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    if (len <= 2 || isPal[i + 1][j - 1]) {
                        isPal[i][j] = true;
                    }
                }
            }
        }
        vector<int> dp(n);
        for (int i = 0; i < n; ++i) {
            if (isPal[0][i]) {
                dp[i] = 0;
            } else {
                int minCuts = i;
                for (int j = 1; j <= i; ++j) {
                    if (isPal[j][i]) {
                        minCuts = min(minCuts, dp[j - 1] + 1);
                    }
                }
                dp[i] = minCuts;
            }
        }
        return dp[n - 1];
    }
};
void test() {
    Solution sol;
    string s1 = "aab";
    cout << "Input: aab, Min Cuts: " << sol.minCut(s1) << " (Expected: 1)" << endl; 
}
} // namespace Question132

// --- 第 133 题 ---
namespace Question133
{
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() { val = 0; neighbors = vector<Node*>(); }
    Node(int _val) { val = _val; neighbors = vector<Node*>(); }
    Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
};
class Solution {
public:
    unordered_map<Node*, Node*> visited;
    Node* cloneGraph(Node* node) {
        if (node == nullptr) return nullptr;
        if (visited.find(node) != visited.end()) {
            return visited[node];
        }
        Node* cloneNode = new Node(node->val);
        visited[node] = cloneNode;
        for (Node* neighbor : node->neighbors) {
            cloneNode->neighbors.push_back(cloneGraph(neighbor));
        }
        return cloneNode;
    }
};
void test() {
    // 构建图: 1 - 2, 1 - 3, 2 - 4, 3 - 4, 4 - 1 (含环)
    Node* n1 = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);
    
    n1->neighbors = {n2, n3, n4};
    n2->neighbors = {n1, n4};
    n3->neighbors = {n1, n4};
    n4->neighbors = {n1, n2, n3};

    Solution sol;
    Node* clonedRoot = sol.cloneGraph(n1);

    cout << "Root value: " << clonedRoot->val << endl;
    cout << "Root neighbors count: " << clonedRoot->neighbors.size() << endl;
    cout << "Cloned root is different from original: " << (clonedRoot != n1 ? "Yes" : "No") << endl;
    
    // 验证深拷贝：修改原图不影响克隆图
    n1->val = 999;
    cout << "After modifying original, cloned root value: " << clonedRoot->val << " (Expected: 1)" << endl;
}
} // namespace Question133

// --- 第 134 题 ---
namespace Question134
{
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalGas = 0;
        int currentGas = 0;
        int startNode = 0;
        for (int i = 0; i < gas.size(); ++i) {
            int diff = gas[i] - cost[i];
            totalGas += diff;
            currentGas += diff;
            if (currentGas < 0) {
                startNode = i + 1;
                currentGas = 0;
            }
        }
        if (totalGas < 0) {
            return -1;
        }
        return startNode;
    }
};
void test() {
    Solution sol;

    // 测试用例 1: 有解
    vector<int> gas1 = {1, 2, 3, 4, 5};
    vector<int> cost1 = {3, 4, 5, 1, 2};
    cout << "Test 1: " << sol.canCompleteCircuit(gas1, cost1) << " (Expected: 3)" << endl; 
    // 解释: 从 index 3 出发 (gas=4, cost=1) -> 剩余 3; (gas=5, cost=2) -> 剩余 6 ...

    // 测试用例 2: 无解
    vector<int> gas2 = {2, 3, 4};
    vector<int> cost2 = {3, 4, 3};
    cout << "Test 2: " << sol.canCompleteCircuit(gas2, cost2) << " (Expected: -1)" << endl;
}
} // namespace Question134

// --- 第 135 题 ---
namespace Question135
{
class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        if (n <= 1) return n;
        vector<int> candies(n, 1);
        for (int i = 1; i < n; ++i) {
            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }
        for (int i = n - 2; i >= 0; --i) {
            if (ratings[i] > ratings[i + 1]) {
                candies[i] = max(candies[i], candies[i + 1] + 1);
            }
        }
        int totalCandies = accumulate(candies.begin(), candies.end(), 0);
        return totalCandies;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> r1 = {1, 0, 2};
    cout << "Test 1 [1,0,2]: " << sol.candy(r1) << " (Expected: 5)" << endl;
    // 过程: [1,1,1] -> [1,1,2] -> [2,1,2] -> sum=5

    // 测试用例 2
    vector<int> r2 = {1, 2, 2};
    cout << "Test 2 [1,2,2]: " << sol.candy(r2) << " (Expected: 4)" << endl;
    // 过程: [1,1,1] -> [1,2,1] -> [1,2,1] -> sum=4 (注意评分相等不需要更多糖)

    // 测试用例 3
    vector<int> r3 = {1, 3, 4, 5, 2};
    cout << "Test 3 [1,3,4,5,2]: " << sol.candy(r3) << " (Expected: 11)" << endl;
    // 过程: [1,1,1,1,1] -> [1,2,3,4,1] -> [1,2,3,4,1] (5比2大, 4+1=5, 
    // 但此时5已经在第4位, 检查 i=3: ratings[3]=5 > ratings[4]=2 -> candies[3] = max(4, 1+1) = 4)
    // 最终: [1, 2, 3, 4, 1] -> sum=11. 等等，再算一遍：
    // L->R: [1, 2, 3, 4, 1]
    // R->L: i=3: 5>2, candies[3]=max(4, 1+1)=4; i=2: 4>5 False; i=1: 3>4 False...
    // 最终: [1, 2, 3, 4, 1] = 11.
}
} // namespace Question135

// --- 第 136 题 ---
namespace Question136
{
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        for (int num : nums) {
            result ^= num;
        }
        return result;
    }
};
void test() {
    Solution sol;
    vector<int> nums1 = {2, 2, 1};
    cout << "Test 1 [2,2,1]: " << sol.singleNumber(nums1) << " (Expected: 1)" << endl;
    // 计算过程: 0 ^ 2 ^ 2 ^ 1 = (2 ^ 2) ^ 1 = 0 ^ 1 = 1
}
} // namespace Question136

// --- 第 137 题 ---
namespace Question137
{
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        for (int i = 0; i < 32; ++i) {
            int count = 0;
            for (int num : nums) {
                if ((num >> i) & 1) {
                    count++;
                }
            }
            if (count % 3 != 0) {
                result |= (1 << i);
            }
        }
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {2, 2, 3, 2};
    cout << "Test 1 [2,2,3,2]: " << sol.singleNumber(nums1) << " (Expected: 3)" << endl;
    // 分析: 
    // 2: 0010
    // 2: 0010
    // 3: 0011
    // 2: 0010
    // 统计位: 第0位有1个'1', 第1位有4个'1'
    // mod 3: 第0位(1%3=1)->1, 第1位(4%3=1)->1
    // 结果: 0011 (二进制) = 3 (十进制)

    // 测试用例 2
    vector<int> nums2 = {0, 1, 0, 1, 0, 1, 99};
    cout << "Test 2: " << sol.singleNumber(nums2) << " (Expected: 99)" << endl;
}
} // namespace Question137

// --- 第 138 题 ---
namespace Question138
{
class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = nullptr;
        random = nullptr;
    }
};
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        Node* curr = head;
        while (curr) {
            Node* newNode = new Node(curr->val);
            newNode->next = curr->next;
            curr->next = newNode;
            curr = newNode->next;
        }
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }
        Node* dummy = new Node(0);
        Node* copyCurr = dummy;
        curr = head;
        while (curr) {
            Node* nextNode = curr->next;
            Node* copyNode = nextNode;
            copyCurr->next = copyNode;
            copyCurr = copyNode;
            curr->next = nextNode->next;
            curr = curr->next;
        }
        return dummy->next;
    }

    Node* copyRandomListHashMap(Node* head) {
        if (!head) return nullptr;
        unordered_map<Node*, Node*> nodeMap;
        Node* curr = head;
        while (curr) {
            nodeMap[curr] = new Node(curr->val);
            curr = curr->next;
        }
        curr = head;
        while (curr) {
            Node* copyNode = nodeMap[curr];
            copyNode->next = nodeMap[curr->next];
            copyNode->random = nodeMap[curr->random];
            curr = curr->next;
        }
        return nodeMap[head];
    }
};
void printList(Node* head) {
    Node* curr = head;
    while (curr) {
        int randVal = curr->random ? curr->random->val : -1;
        cout << "[" << curr->val << ", rand:" << randVal << "] -> ";
        curr = curr->next;
    }
    cout << "nullptr" << endl;
}
void test() {
    // 构建链表: 7 -> 13 -> 11 -> 10
    // 7.random = nullptr; 13.random = 7; 11.random = 13; 10.random = 11
    Node* n1 = new Node(7);
    Node* n2 = new Node(13);
    Node* n3 = new Node(11);
    Node* n4 = new Node(10);
    
    n1->next = n2; n2->next = n3; n3->next = n4;
    n1->random = nullptr;
    n2->random = n1;
    n3->random = n2;
    n4->random = n3;

    Solution sol;
    Node* clonedHead = sol.copyRandomList(n1);

    cout << "Original List:" << endl;
    printList(n1);
    cout << "Cloned List:" << endl;
    printList(clonedHead);
}
} // namespace Question138

// --- 第 139 题 ---
namespace Question139
{
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n];
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    string s1 = "leetcode";
    vector<string> dict1 = {"leet", "code"};
    cout << "Test 1 [leetcode]: " << (sol.wordBreak(s1, dict1) ? "True" : "False") << " (Expected: True)" << endl;
}
} // namespace Question139

// --- 第 140 题 ---
namespace Question140
{
class Solution {
public:
    unordered_map<int, vector<string>> memo;
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        return solve(s, 0, dict);
    }
private:
    vector<string> solve(const string& s, int start, const unordered_set<string>& dict) {
        if (memo.count(start)) {
            return memo[start];
        }
        vector<string> results;
        int n = s.size();
        if (start == n) {
            results.push_back("");
            return results;
        }
        for (int i = start + 1; i <= n; ++i) {
            string word = s.substr(start, i - start);
            if (dict.count(word)) {
                vector<string> nextWays = solve(s, i, dict);
                for (const string& next : nextWays) {
                    string space = next.empty() ? "" : " ";
                    results.push_back(word + space + next);
                }
            }
        }
        return memo[start] = results;
    }
};
void test() {
    Solution sol;
    
    string s1 = "pineapplepenapple";
    vector<string> dict1 = {"apple", "pen", "applepen", "pine", "pineapple"};
    
    cout << "Input: pineapplepenapple" << endl;
    vector<string> res1 = sol.wordBreak(s1, dict1);
    for (const string& sentence : res1) {
        cout << "\"" << sentence << "\"" << endl;
    }
    // 预期输出: 
    // "pine apple pen apple"
    // "pineapple pen apple"
    // "pine applepen apple"
}
} // namespace Question140

// --- 第 141 题 ---
namespace Question141
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return false;
        }
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                return true;
            }
        }
        return false;
    }
};
void test() {
    Solution sol;

    // 测试用例 1: 有环链表 3 -> 2 -> 0 -> -4 (回到 2)
    ListNode* node1 = new ListNode(3);
    ListNode* node2 = new ListNode(2);
    ListNode* node3 = new ListNode(0);
    ListNode* node4 = new ListNode(-4);
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node2; // 创建环

    cout << "Test Case 1 (Cycle): " << (sol.hasCycle(node1) ? "True" : "False") << endl;

    // 测试用例 2: 无环链表 1 -> 2
    ListNode* node5 = new ListNode(1);
    ListNode* node6 = new ListNode(2);
    node5->next = node6;

    cout << "Test Case 2 (No Cycle): " << (sol.hasCycle(node5) ? "True" : "False") << endl;
}
} // namespace Question141

// --- 第 142 题 ---
namespace Question142
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return nullptr;
        }
        ListNode* slow = head;
        ListNode* fast = head;
        bool hasCycle = false;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                hasCycle = true;
                break;
            }
        }
        if (!hasCycle) {
            return nullptr;
        }
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};
void test() {
    Solution sol;

    // 构建测试用例: 3 -> 2 -> 0 -> -4 -> (回到 2)
    ListNode* node1 = new ListNode(3);
    ListNode* node2 = new ListNode(2);
    ListNode* node3 = new ListNode(0);
    ListNode* node4 = new ListNode(-4);
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node2; // 环入口是 node2

    ListNode* result = sol.detectCycle(node1);
    if (result == node2) {
        cout << "Test Case 1: Success! Entrance found at node with value " << result->val << endl;
    } else {
        cout << "Test Case 1: Failed!" << endl;
    }

    // 测试用例 2: 无环 1 -> 2
    ListNode* node5 = new ListNode(1);
    ListNode* node6 = new ListNode(2);
    node5->next = node6;

    ListNode* result2 = sol.detectCycle(node5);
    if (result2 == nullptr) {
        cout << "Test Case 2: Success! No cycle detected." << endl;
    } else {
        cout << "Test Case 2: Failed!" << endl;
    }
}
} // namespace Question142

// --- 第 143 题 ---
namespace Question143
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next || !head->next->next) return;
        ListNode* slow = head, *fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode* second = slow->next;
        slow->next = nullptr;
        ListNode* prev = nullptr;
        ListNode* curr = second;
        while (curr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        ListNode* secondHalf = prev;
        ListNode* firstHalf = head;
        while (secondHalf) {
            ListNode* tmp1 = firstHalf->next;
            ListNode* tmp2 = secondHalf->next;
            firstHalf->next = secondHalf;
            secondHalf->next = tmp1;
            firstHalf = tmp1;
            secondHalf = tmp2;
        }
    }
};
void test() {
    Solution sol;
    using namespace Utils;
    ListNode* list1 = createList<ListNode>({1, 2, 3, 4, 5});
    sol.reorderList(list1);
    printList(list1);
}
} // namespace Question143

// --- 第 144 题 ---
namespace Question144
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        traverse(root, res);
        return res;
    }
    void traverse(TreeNode* node, vector<int>& res) {
        if (node == nullptr) return;
        res.push_back(node->val);
        traverse(node->left, res);
        traverse(node->right, res);
    }

    vector<int> preorderTraversalIterative(TreeNode* root) {
        vector<int> res;
        if (root == nullptr) return res;
        stack<TreeNode*> s;
        s.push(root);
        while (!s.empty()) {
            TreeNode* node = s.top();
            s.pop();
            res.push_back(node->val);
            if (node->right) s.push(node->right);
            if (node->left) s.push(node->left);
        }
        return res;
    }
};
void test() {
    Solution sol;
    using namespace Utils;
    // 构建二叉树: 
    //     1
    //    / \
    //   2   3
    //    \
    //     4
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(4);

    cout << "Recursive Result: ";
    printVec(sol.preorderTraversal(root)); // 预期: [1, 2, 4, 3]

    cout << "Iterative Result: ";
    printVec(sol.preorderTraversalIterative(root)); // 预期: [1, 2, 4, 3]
}
} // namespace Question144

// --- 第 145 题 ---
namespace Question145
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> res;
        traverse(root, res);
        return res;
    }
    void traverse(TreeNode* node, vector<int>& res) {
        if (node == nullptr) return;
        traverse(node->left, res);
        traverse(node->right, res);
        res.push_back(node->val);
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question145

// --- 第 146 题 ---
namespace Question146
{
class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {}
    
    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return -1;
        }
        cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);
        return cacheMap[key]->second;
    }
    
    void put(int key, int value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            cacheMap[key]->second = value;
            cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);
            return;
        }
        if (cacheList.size() == cap) {
            int lastKey = cacheList.back().first;
            cacheMap.erase(lastKey);
            cacheList.pop_back();
        }
        cacheList.push_front({key, value});
        cacheMap[key] = cacheList.begin();
    }
private:
    int cap;
    list<pair<int, int>> cacheList;
    unordered_map<int, list<pair<int, int>>::iterator> cacheMap;
};
void test() {
    LRUCache lru(2); // 容量为 2

    lru.put(1, 1); // 缓存: {1=1}
    lru.put(2, 2); // 缓存: {2=2, 1=1}
    cout << "get(1): " << lru.get(1) << endl;    // 返回 1, 缓存: {1=1, 2=2}
    
    lru.put(3, 3); // 驱逐 key 2, 缓存: {3=3, 1=1}
    cout << "get(2): " << lru.get(2) << endl;    // 返回 -1 (未找到)
    
    lru.put(4, 4); // 驱逐 key 1, 缓存: {4=4, 3=3}
    cout << "get(1): " << lru.get(1) << endl;    // 返回 -1 (未找到)
    cout << "get(3): " << lru.get(3) << endl;    // 返回 3, 缓存: {3=3, 4=4}
    cout << "get(4): " << lru.get(4) << endl;    // 返回 4, 缓存: {4=4, 3=3}
}
} // namespace Question146

// --- 第 147 题 ---
namespace Question147
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode* dummy = new ListNode(0);
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* nextTemp = curr->next;
            ListNode* prev = dummy;
            while (prev->next != nullptr && prev->next->val < curr->val) {
                prev = prev->next;
            }
            curr->next = prev->next;
            prev->next = curr;
            curr = nextTemp;
        }
        ListNode* sortedHead = dummy->next;
        delete dummy;
        return sortedHead;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question147

// --- 第 148 题 ---
namespace Question148
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) {
            return head;
        }
        ListNode* mid = getMid(head);
        ListNode* left = head;
        ListNode* right = mid->next;
        mid->next = nullptr;
        ListNode* sortedLeft = sortList(left);
        ListNode* sortedRight = sortList(right);
        return merge(sortedLeft, sortedRight);
    }
private:
    ListNode* getMid(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question148

// --- 第 149 题 ---
namespace Question149
{
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;
        int globalMax = 0;
        for (int i = 0; i < n; ++i) {
            unordered_map<string, int> slopeMap;
            int duplicate = 1;
            int currentMax = 0;
            for (int j = i + 1; j < n; ++j) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];
                if (dx == 0 && dy == 0) {
                    duplicate++;
                    continue;
                }
                int common = gcd(dx, dy);
                dx /= common;
                dy /= common;
                if (dx < 0) {
                    dx = -dx;
                    dy = -dy;
                } else if (dx == 0) {
                    dy = abs(dy) ? 1 : 0;
                }
                string slope = to_string(dx) + "/" + to_string(dy);
                slopeMap[slope]++;
                currentMax = max(currentMax, slopeMap[slope]);
            }
            globalMax = max(globalMax, currentMax + duplicate);
        }
        return globalMax;
    }
};
void test() {
    Solution sol;

    // 测试用例 1: [[1,1],[2,2],[3,3]] -> 3
    vector<vector<int>> p1 = {{1, 1}, {2, 2}, {3, 3}};
    cout << "Test Case 1: " << sol.maxPoints(p1) << " (Expected: 3)" << endl;

    // 测试用例 2: [[1,1],[3,2],[5,3],[4,1],[2,3],[1,5]] -> 4
    vector<vector<int>> p2 = {{1, 1}, {3, 2}, {5, 3}, {4, 1}, {2, 3}, {1, 5}};
    cout << "Test Case 2: " << sol.maxPoints(p2) << " (Expected: 4)" << endl;
}
} // namespace Question149

// --- 第 150 题 ---
namespace Question150
{
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> s;
        for (const string& t : tokens) {
            if (t == "+" || t == "-" || t == "*" || t == "/") {
                int val2 = s.top(); s.pop();
                int val1 = s.top(); s.pop();
                if (t == "+") s.push(val1 + val2);
                else if (t == "-") s.push(val1 - val2);
                else if (t == "*") s.push(val1 * val2);
                else if (t == "/") s.push(val1 / val2);
            } else {
                s.push(stoi(t));
            }
        }
        return s.top();
    }
};
void test() {
    Solution sol;
    // 测试用例 1: ["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) = 9
    vector<string> tokens1 = {"2", "1", "+", "3", "*"};
    cout << "Test Case 1: " << sol.evalRPN(tokens1) << " (Expected: 9)" << endl;

    // 测试用例 2: ["4", "13", "5", "/", "+"] -> (4 + (13 / 5)) = 6
    vector<string> tokens2 = {"4", "13", "5", "/", "+"};
    cout << "Test Case 2: " << sol.evalRPN(tokens2) << " (Expected: 6)" << endl;

    // 测试用例 3: ["10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"]
    // 预期结果: 22
    vector<string> tokens3 = {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"};
    cout << "Test Case 3: " << sol.evalRPN(tokens3) << " (Expected: 22)" << endl;
}
} // namespace Question150

// --- 第 151 题 ---
namespace Question151
{
class Solution {
public:
    string reverseWords(string s) {
        int n = s.length();
        int slow = 0;
        for (int fast = 0; fast < n; ++fast) {
            if (s[fast] != ' ') {
                if (slow != 0) s[slow++] = ' ';
                while (fast < n && s[fast] != ' ') {
                    s[slow++] = s[fast++];
                }
            }
        }
        s.resize(slow);
        reverse(s.begin(), s.end());
        int start = 0;
        for (int end = 0; end < s.length(); ++end) {
            if (s[end] == ' ') {
                reverse(s.begin() + start, s.begin() + end);
                start = end + 1;
            } else {
                if (end == s.length() - 1) {
                    reverse(s.begin() + start, s.begin() + end + 1);
                }
            }
        }
        return s;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: "the sky is blue" -> "blue is sky the"
    string s1 = "the sky is blue";
    cout << "Original: \"" << s1 << "\"\nResult: \"" << sol.reverseWords(s1) << "\"" << endl;

    // 测试用例 2: "  hello world  " -> "world hello"
    string s2 = "  hello world  ";
    cout << "Original: \"" << s2 << "\"\nResult: \"" << sol.reverseWords(s2) << "\"" << endl;

    // 测试用例 3: "a good   example" -> "example good a"
    string s3 = "a good   example";
    cout << "Original: \"" << s3 << "\"\nResult: \"" << sol.reverseWords(s3) << "\"" << endl;
}
} // namespace Question151

// --- 第 152 题 ---
namespace Question152
{
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if (nums.empty()) return 0;
        int max_so_far = nums[0];
        int min_so_far = nums[0];
        int global_max = nums[0];
        for (size_t i = 1; i < nums.size(); ++i) {
            int x = nums[i];
            if (x < 0) {
                swap(max_so_far, min_so_far);
            }
            max_so_far = max(x, max_so_far * x);
            min_so_far = min(x, min_so_far * x);
            global_max = max(global_max, max_so_far);
        }
        return global_max;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: [2, 3, -2, 4] -> 6
    vector<int> nums1 = {2, 3, -2, 4};
    cout << "Test Case 1: " << sol.maxProduct(nums1) << " (Expected: 6)" << endl;

    // 测试用例 2: [-2, 0, -1] -> 0
    vector<int> nums2 = {-2, 0, -1};
    cout << "Test Case 2: " << sol.maxProduct(nums2) << " (Expected: 0)" << endl;

    // 测试用例 3: [-2, 3, -4] -> 24 ((-2)*3*(-4))
    vector<int> nums3 = {-2, 3, -4};
    cout << "Test Case 3: " << sol.maxProduct(nums3) << " (Expected: 24)" << endl;
}
} // namespace Question152

// --- 第 153 题 ---
namespace Question153
{
class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > nums[right]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return nums[left];
    }
};
void test() {
    Solution sol;
    vector<int> case1 = {3, 4, 5, 1, 2};
    cout << "Case 1 [3,4,5,1,2]: Expected 1, Got " << sol.findMin(case1) << endl;

    vector<int> case2 = {4, 5, 6, 7, 0, 1, 2};
    cout << "Case 2 [4,5,6,7,0,1,2]: Expected 0, Got " << sol.findMin(case2) << endl;

    vector<int> case3 = {11, 13, 15, 17};
    cout << "Case 3 [11,13,15,17]: Expected 11, Got " << sol.findMin(case3) << endl;
}
} // namespace Question153

// --- 第 154 题 ---
namespace Question154
{
class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > nums[right]) {
                left = mid + 1;
            } else if (nums[mid] < nums[right]) {
                right = mid;
            } else {
                right--;
            }
        }
        return nums[left];
    }
};
void test() {
    Solution sol;
    vector<int> case1 = {1, 3, 5};
    cout << "Case 1 [1,3,5]: Expected 1, Got " << sol.findMin(case1) << endl;

    vector<int> case2 = {2, 2, 2, 0, 1};
    cout << "Case 2 [2,2,2,0,1]: Expected 0, Got " << sol.findMin(case2) << endl;

    vector<int> case3 = {10, 1, 10, 10, 10};
    cout << "Case 3 [10,1,10,10,10]: Expected 1, Got " << sol.findMin(case3) << endl;
}
} // namespace Question154

// --- 第 155 题 ---
namespace Question155
{
class MinStack {
public:
    MinStack() {
        
    }
    
    void push(int val) {
        dataStack.push(val);
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        } else {
            minStack.push(minStack.top());
        }
    }
    
    void pop() {
        dataStack.pop();
        minStack.pop();
    }
    
    int top() {
        return dataStack.top();
    }
    
    int getMin() {
        return minStack.top();
    }
private:
    stack<int> dataStack;
    stack<int> minStack;
};
void test() {
    MinStack ms;
    
    ms.push(-2);
    ms.push(0);
    ms.push(-3);
    
    cout << "getMin(): " << ms.getMin() << " (Expected: -3)" << endl; // 返回 -3
    ms.pop();
    cout << "top(): " << ms.top() << " (Expected: 0)" << endl;       // 返回 0
    cout << "getMin(): " << ms.getMin() << " (Expected: -2)" << endl; // 返回 -2
}
} // namespace Question155

// --- 第 160 题 ---
namespace Question160
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) return nullptr;
        ListNode* pA = headA;
        ListNode* pB = headB;
        while (pA != pB) {
            pA = (pA == nullptr) ? headB : pA->next;
            pB = (pB == nullptr) ? headA : pB->next;
        }
        return pA;
    }
};
// 辅助函数：创建相交链表用于测试
ListNode* createIntersection(vector<int> listA, vector<int> listB, int intersectIdxA, int intersectIdxB, vector<int> common) {
    ListNode* headA = new ListNode(0);
    ListNode* currA = headA;
    for (int i = 0; i < intersectIdxA; ++i) {
        currA->next = new ListNode(listA[i]);
        currA = currA->next;
    }

    ListNode* commonHead = new ListNode(0);
    ListNode* currCommon = commonHead;
    for (int val : common) {
        currCommon->next = new ListNode(val);
        currCommon = currCommon->next;
    }
    ListNode* actualCommonHead = commonHead->next;

    currA->next = actualCommonHead;

    ListNode* headB = new ListNode(0);
    ListNode* currB = headB;
    for (int i = 0; i < intersectIdxB; ++i) {
        currB->next = new ListNode(listB[i]);
        currB = currB->next;
    }
    currB->next = actualCommonHead;

    return headA; // 这里的逻辑为了简化，直接返回，实际需注意内存管理
}

void test() {
    Solution sol;

    // 构建相交链表: 
    // A: 1 -> 2 -> 3 \
    //                6 -> 7 (相交部分)
    // B: 4 -> 5 ---- /
    ListNode* common = new ListNode(6);
    common->next = new ListNode(7);

    ListNode* headA = new ListNode(1);
    headA->next = new ListNode(2);
    headA->next->next = new ListNode(3);
    headA->next->next->next = common;

    ListNode* headB = new ListNode(4);
    headB->next = new ListNode(5);
    headB->next->next = common;

    ListNode* result = sol.getIntersectionNode(headA, headB);
    if (result) {
        cout << "Intersected at node with value: " << result->val << " (Expected: 6)" << endl;
    } else {
        cout << "No intersection" << endl;
    }
}
} // namespace Question160

// --- 第 162 题 ---
namespace Question162
{
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[mid + 1]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {1, 2, 3, 1};
    cout << "Test 1: Expected 2, Got " << sol.findPeakElement(nums1) << endl; // 元素 3 是峰值，索引为 2

    // 测试用例 2
    vector<int> nums2 = {1, 2, 1, 3, 5, 6, 4};
    int result2 = sol.findPeakElement(nums2);
    cout << "Test 2: Expected 1 or 5, Got " << result2 << endl; // 元素 2 或 6 都是峰值

    // 测试用例 3: 单元素
    vector<int> nums3 = {1};
    cout << "Test 3: Expected 0, Got " << sol.findPeakElement(nums3) << endl;

    // 测试用例 4: 严格递增
    vector<int> nums4 = {1, 2, 3, 4, 5};
    cout << "Test 4: Expected 4, Got " << sol.findPeakElement(nums4) << endl;
}
} // namespace Question162

// --- 第 164 题 ---
namespace Question164
{
class Solution {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        int minVal = nums[0], maxVal = nums[0];
        for (int x : nums) {
            minVal = min(minVal, x);
            maxVal = max(maxVal, x);
        }
        if (minVal == maxVal) return 0;
        int bucketSize = max(1, (maxVal - minVal) / (n - 1));
        int bucketCount = (maxVal - minVal) / bucketSize + 1;
        vector<int> bucketMin(bucketCount, INT_MAX);
        vector<int> bucketMax(bucketCount, INT_MIN);
        for (int x : nums) {
            int idx = (x - minVal) / bucketSize;
            bucketMin[idx] = min(bucketMin[idx], x);
            bucketMax[idx] = max(bucketMax[idx], x);
        }
        int maxGap = 0;
        int prevMax = minVal;
        for (int i = 0; i < bucketCount; ++i) {
            if (bucketMin[i] == INT_MAX) continue;
            maxGap = max(maxGap, bucketMin[i] - prevMax);
            prevMax = bucketMax[i];
        }
        return maxGap;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {3, 6, 9, 1};
    cout << "Test 1: Expected 3, Got " << sol.maximumGap(nums1) << endl; 
    // 排序后 [1, 3, 6, 9]，间距为 2, 3, 3 -> 最大 3

    // 测试用例 2
    vector<int> nums2 = {10};
    cout << "Test 2: Expected 0, Got " << sol.maximumGap(nums2) << endl;

    // 测试用例 3
    vector<int> nums3 = {1, 10};
    cout << "Test 3: Expected 9, Got " << sol.maximumGap(nums3) << endl;
    
    // 测试用例 4
    vector<int> nums4 = {1, 1, 1};
    cout << "Test 4: Expected 0, Got " << sol.maximumGap(nums4) << endl;
}
} // namespace Question164

// --- 第 165 题 ---
namespace Question165
{
class Solution {
public:
    int compareVersion(string version1, string version2) {
        int n1 = version1.length();
        int n2 = version2.length();
        int i = 0, j = 0;
        while (i < n1 || j < n2) {
            long num1 = 0;
            long num2 = 0;
            while (i < n1 && version1[i] != '.') {
                num1 = num1 * 10 + (version1[i] - '0');
                i++;
            }
            while (j < n2 && version2[j] != '.') {
                num2 = num2 * 10 + (version2[j] - '0');
                j++;
            }
            if (num1 > num2) return 1;
            if (num1 < num2) return -1;
            i++;
            j++;
        }
        return 0;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    cout << "Test 1 (1.01, 1.001): Expected 0, Got " << sol.compareVersion("1.01", "1.001") << endl;

    // 测试用例 2
    cout << "Test 2 (1.0, 1): Expected 0, Got " << sol.compareVersion("1.0", "1") << endl;

    // 测试用例 3
    cout << "Test 3 (0.1, 1.1): Expected -1, Got " << sol.compareVersion("0.1", "1.1") << endl;

    // 测试用例 4
    cout << "Test 4 (1.2, 1.10): Expected -1, Got " << sol.compareVersion("1.2", "1.10") << endl;
    
    // 测试用例 5
    cout << "Test 5 (1.0.1, 1): Expected 1, Got " << sol.compareVersion("1.0.1", "1") << endl;
}
} // namespace Question165

// --- 第 166 题 ---
namespace Question166
{
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
void test() {
    Solution sol;// 测试用例 1: 有限小数
    cout << "Test 1 (1, 2): Expected 0.5, Got " << sol.fractionToDecimal(1, 2) << endl;

    // 测试用例 2: 整数
    cout << "Test 2 (4, 2): Expected 2, Got " << sol.fractionToDecimal(4, 2) << endl;

    // 测试用例 3: 循环小数
    cout << "Test 3 (2, 3): Expected 0.(6), Got " << sol.fractionToDecimal(2, 3) << endl;

    // 测试用例 4: 复杂循环小数
    cout << "Test 4 (1, 6): Expected 0.1(6), Got " << sol.fractionToDecimal(1, 6) << endl;

    // 测试用例 5: 负数处理
    cout << "Test 5 (-50, 8): Expected -6.25, Got " << sol.fractionToDecimal(-50, 8) << endl;

    // 测试用例 6: 边界情况 INT_MIN
    cout << "Test 6 (INT_MIN, -1): Expected 2147483648, Got " << sol.fractionToDecimal(-2147483648, -1) << endl;
}
} // namespace Question166

// --- 第 167 题 ---
namespace Question167
{
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0;
        int right = numbers.size() - 1;
        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum == target) {
                return {left + 1, right + 1};
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
        return {};
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    vector<int> res1 = sol.twoSum(nums1, target1);
    cout << "Test 1: Expected [1, 2], Got [" << res1[0] << ", " << res1[1] << "]" << endl;

    // 测试用例 2
    vector<int> nums2 = {2, 3, 4};
    int target2 = 6;
    vector<int> res2 = sol.twoSum(nums2, target2);
    cout << "Test 2: Expected [1, 3], Got [" << res2[0] << ", " << res2[1] << "]" << endl;

    // 测试用例 3
    vector<int> nums3 = {-1, 0};
    int target3 = -1;
    vector<int> res3 = sol.twoSum(nums3, target3);
    cout << "Test 3: Expected [1, 2], Got [" << res3[0] << ", " << res3[1] << "]" << endl;
}
} // namespace Question167

// --- 第 168 题 ---
namespace Question168
{
class Solution {
public:
    string convertToTitle(int columnNumber) {
        string res = "";
        while (columnNumber > 0) {
            columnNumber--;
            char c = 'A' + (columnNumber % 26);
            res += c;
            columnNumber /= 26;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 单个字母
    cout << "Test 1 (1): Expected A, Got " << sol.convertToTitle(1) << endl;

    // 测试用例 2: 临界点 Z
    cout << "Test 2 (26): Expected Z, Got " << sol.convertToTitle(26) << endl;

    // 测试用例 3: 两个字母 AB
    cout << "Test 3 (28): Expected AB, Got " << sol.convertToTitle(28) << endl;

    // 测试用例 4: 复杂情况 ZY
    cout << "Test 4 (701): Expected ZY, Got " << sol.convertToTitle(701) << endl;
    
    // 测试用例 5: 更大数值
    cout << "Test 5 (16384): Expected FXG, Got " << sol.convertToTitle(16384) << endl;
}
} // namespace Question168

// --- 第 169 题 ---
namespace Question169
{
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0;
        int count = 0;
        for (int num : nums) {
            if (count == 0) {
                candidate = num;
            }
            if (num == candidate) {
                count++;
            } else {
                count--;
            }
        }
        return candidate;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {3, 2, 3};
    cout << "Test 1: Expected 3, Got " << sol.majorityElement(nums1) << endl;

    // 测试用例 2
    vector<int> nums2 = {2, 2, 1, 1, 1, 2, 2};
    cout << "Test 2: Expected 2, Got " << sol.majorityElement(nums2) << endl;

    // 测试用例 3: 单元素
    vector<int> nums3 = {1};
    cout << "Test 3: Expected 1, Got " << sol.majorityElement(nums3) << endl;

    // 测试用例 4: 只有两种元素，多数元素在后
    vector<int> nums4 = {1, 1, 2, 2, 2};
    cout << "Test 4: Expected 2, Got " << sol.majorityElement(nums4) << endl;
}
} // namespace Question169

// --- 第 171 题 ---
namespace Question171
{
class Solution {
public:
    int titleToNumber(string columnTitle) {
        long long result = 0;
        for (char c : columnTitle) {
            int val = c - 'A' + 1;
            result = result * 26 + val;
        }
        return (int)result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 单字母
    cout << "Test 1 (A): Expected 1, Got " << sol.titleToNumber("A") << endl;

    // 测试用例 2: 单字母 Z
    cout << "Test 2 (Z): Expected 26, Got " << sol.titleToNumber("Z") << endl;

    // 测试用例 3: 两个字母 AA
    cout << "Test 3 (AA): Expected 27, Got " << sol.titleToNumber("AA") << endl;

    // 测试用例 4: 两个字母 AB
    cout << "Test 4 (AB): Expected 28, Got " << sol.titleToNumber("AB") << endl;

    // 测试用例 5: 复杂情况 ZY
    cout << "Test 5 (ZY): Expected 701, Got " << sol.titleToNumber("ZY") << endl;
    
    // 测试用例 6: 长字符串
    cout << "Test 6 (FXG): Expected 16384, Got " << sol.titleToNumber("FXG") << endl;
}
} // namespace Question171

// --- 第 172 题 ---
namespace Question172
{
class Solution {
public:
    int trailingZeroes(int n) {
        int count = 0;
        while (n >= 5) {
            count += n / 5;
            n /= 5;
        }
        return count;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 3! = 6 -> 0个零
    cout << "Test 1 (3): Expected 0, Got " << sol.trailingZeroes(3) << endl;

    // 测试用例 2: 5! = 120 -> 1个零
    cout << "Test 2 (5): Expected 1, Got " << sol.trailingZeroes(5) << endl;

    // 测试用例 3: 25! -> 25/5 + 25/25 = 5 + 1 = 6个零
    cout << "Test 3 (25): Expected 6, Got " << sol.trailingZeroes(25) << endl;

    // 测试用例 4: 100! -> 100/5 + 100/25 = 20 + 4 = 24个零
    cout << "Test 4 (100): Expected 24, Got " << sol.trailingZeroes(100) << endl;
}
} // namespace Question172

// --- 第 173 题 ---
namespace Question173
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class BSTIterator {
public:
    BSTIterator(TreeNode* root) {
        pushLeft(root);
    }
    
    int next() {
        TreeNode* node = s.top();
        s.pop();
        if (node->right != nullptr) {
            pushLeft(node->right);
        }
        return node->val;
    }
    
    bool hasNext() {
        return !s.empty();
    }
private:
    stack<TreeNode*> s;
    void pushLeft(TreeNode* node) {
        while (node != nullptr) {
            s.push(node);
            node = node->left;
        }
    }
};
void test() {
    // 构建如下 BST:
    //      7
    //     / \
    //    3   15
    //   / \    \
    //  1   6    20
    TreeNode* root = new TreeNode(7);
    root->left = new TreeNode(3);
    root->right = new TreeNode(15);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(6);
    root->right->right = new TreeNode(20);

    BSTIterator iterator(root);
    cout << "BST In-order traversal: ";
    while (iterator.hasNext()) {
        cout << iterator.next() << " ";
    }
    cout << endl; 
    // Expected: 1 3 6 7 15 20 
}
} // namespace Question173

// --- 第 174 题 ---
namespace Question174
{
class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int m = dungeon.size();
        int n = dungeon[0].size();
        vector<vector<int>> dp(m, vector<int>(n));
        dp[m - 1][n - 1] = max(1, 1 - dungeon[m - 1][n - 1]);
        for (int j = n - 2; j >= 0; --j) {
            dp[m - 1][j] = max(1, dp[m - 1][j + 1] - dungeon[m - 1][j]);
        }
        for (int i = m - 2; i >= 0; --i) {
            dp[i][n - 1] = max(1, dp[i + 1][n - 1] - dungeon[i][n - 1]);
        }
        for (int i = m - 2; i >= 0; --i) {
            for (int j = n - 2; j >= 0; --j) {
                int min_next = min(dp[i + 1][j],dp[i][j + 1]);
                dp[i][j] = max(1, min_next - dungeon[i][j]);
            }
        }
        return dp[0][0];
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<vector<int>> dungeon1 = {{-2, -3, 3}, {-5, -10, 1}, {10, 30, -5}};
    cout << "Test 1: " << sol.calculateMinimumHP(dungeon1) << " (Expected: 7)" << endl;

    // 测试用例 2
    vector<vector<int>> dungeon2 = {{0}};
    cout << "Test 2: " << sol.calculateMinimumHP(dungeon2) << " (Expected: 1)" << endl;
}
} // namespace Question174

// --- 第 179 题 ---
namespace Question179
{
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        vector<string> strs;
        for (int num : nums) {
            strs.push_back(to_string(num));
        }
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a + b > b + a;
        });
        if (strs[0] == "0") {
            return "0";
        }
        string res = "";
        for (const string& s : strs) {
            res += s;
        }
        return res;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {10, 2};
    cout << "Test 1: " << sol.largestNumber(nums1) << " (Expected: 210)" << endl;

    // 测试用例 2
    vector<int> nums2 = {3, 30, 34, 5, 9};
    cout << "Test 2: " << sol.largestNumber(nums2) << " (Expected: 9534330)" << endl;

    // 测试用例 3
    vector<int> nums3 = {0, 0};
    cout << "Test 3: " << sol.largestNumber(nums3) << " (Expected: 0)" << endl;
}
} // namespace Question179

// --- 第 187 题 ---
namespace Question187
{
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.length() < 10) {
            return {};
        }
        unordered_map<string, int> counts;
        vector<string> result;
        for (int i = 0; i <= (int)s.length() - 10; ++i) {
            string sub = s.substr(i, 10);
            counts[sub]++;
            if (counts[sub] == 2) {
                result.push_back(sub);
            }
        }
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    string s1 = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT";
    vector<string> res1 = sol.findRepeatedDnaSequences(s1);
    cout << "Test 1: ";
    for (const string& str : res1) cout << str << " "; 
    cout << "(Expected: AAAAACCCCC CCCCCAAAAA)" << endl;

    // 测试用例 2
    string s2 = "AAAAAAAAAAA"; // 11个A，包含两个重复的 "AAAAAAAAAA"
    vector<string> res2 = sol.findRepeatedDnaSequences(s2);
    cout << "Test 2: ";
    for (const string& str : res2) cout << str << " "; 
    cout << "(Expected: AAAAAAAAAA)" << endl;
}
} // namespace Question187

// --- 第 188 题 ---
namespace Question188
{
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; ++i) {
                if (prices[i] > prices[i - 1]) {
                    profit += prices[i] - prices[i - 1];
                }
            }
            return profit;
        }
        vector<int> buy(k + 1, INT_MIN);
        vector<int> sell(k + 1, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j <= k; ++j) {
                buy[j] = max(buy[j], sell[j - 1] - prices[i]);
                sell[j] = max(sell[j], buy[j] + prices[i]);
            }
        }
        return sell[k];
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> prices1 = {3, 2, 6, 5, 0, 3};
    int k1 = 2;
    cout << "Test 1: " << sol.maxProfit(k1, prices1) << " (Expected: 7)" << endl; 
    // 解释：买2卖6(4)，买0卖3(3) -> 4+3=7

    // 测试用例 2
    vector<int> prices2 = {1, 2, 3, 4, 5};
    int k2 = 2;
    cout << "Test 2: " << sol.maxProfit(k2, prices2) << " (Expected: 4)" << endl; 
    // 解释：买1卖5 -> 4
}
} // namespace Question188

// --- 第 189 题 ---
namespace Question189
{
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0) return;
        k %= n;
        if (k == 0) return;
        reverse(nums.begin(), nums.end());
        reverse(nums.begin(), nums.begin() + k);
        reverse(nums.begin() + k, nums.end());
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question189

// --- 第 190 题 ---
namespace Question190
{
class Solution {
public:
    int reverseBits(int n) {
        int res = 0;
        for (int i = 0; i < 32; ++i) {
            res <<= 1;
            res |= (n & 1);
            n >>= 1;
        }
        return res;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question190

// --- 第 191 题 ---
namespace Question191
{
class Solution {
public:
    int hammingWeight(int n) {
        int count = 0;
        while (n != 0) {
            n &= (n - 1);
            count++;
        }
        return count;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    uint32_t n1 = 11; // 二进制: 1011
    cout << "Test 1 (11): " << sol.hammingWeight(n1) << " (Expected: 3)" << endl;

    // 测试用例 2
    uint32_t n2 = 128; // 二进制: 10000000
    cout << "Test 2 (128): " << sol.hammingWeight(n2) << " (Expected: 1)" << endl;

    // 测试用例 3
    uint32_t n3 = 0; // 二进制: 0
    cout << "Test 3 (0): " << sol.hammingWeight(n3) << " (Expected: 0)" << endl;

    // 测试用例 4
    uint32_t n4 = 0xFFFFFFFF; // 全 1
    cout << "Test 4 (MAX): " << sol.hammingWeight(n4) << " (Expected: 32)" << endl;
}
} // namespace Question191

// --- 第 198 题 ---
namespace Question198
{
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        int prev2 = 0;
        int prev1 = 0;
        for (int x : nums) {
            int current = max(prev1, x + prev2);
            prev2 = prev1;
            prev1 = current;
        }
        return prev1;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {1, 2, 3, 1};
    cout << "Test 1: " << sol.rob(nums1) << " (Expected: 4)" << endl; 
    // 解释: 偷 1号和3号 (1+3=4)

    // 测试用例 2
    vector<int> nums2 = {2, 7, 9, 3, 1};
    cout << "Test 2: " << sol.rob(nums2) << " (Expected: 12)" << endl; 
    // 解释: 偷 2, 9, 1 (2+9+1=12)
}
} // namespace Question198

// --- 第 199 题 ---
namespace Question199
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                if (i == levelSize - 1) {
                    res.push_back(node->val);
                }
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return res;
    }
};
void test() {
    Solution sol;
    //      1
    //     / \
    //    2   3
    //     \   \
    //      5   4
    //     / 
    //    6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(5);
    root->left->right->left = new TreeNode(6);
    root->right->right = new TreeNode(4);
    vector<int> res = sol.rightSideView(root);
    
    cout << "Right Side View: ";
    for (int val : res) cout << val << " ";
    cout << "(Expected: 1 3 4)" << endl; 

    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question199

// --- 第 200 题 ---
namespace Question200
{
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size();
        int n = grid[0].size();
        int islandCount = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '1') {
                    islandCount++;
                    dfs(grid, i, j);
                }
            }
        }
        return islandCount;
    }
private:
    void dfs(vector<vector<char>>& grid, int r, int c) {
        int m = grid.size();
        int n = grid[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] == '0') {
            return;
        }
        grid[r][c] = '0';
        dfs(grid, r - 1, c);
        dfs(grid, r + 1, c);
        dfs(grid, r, c - 1);
        dfs(grid, r, c + 1);
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<vector<char>> grid1 = {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0'}
    };
    cout << "Test 1: " << sol.numIslands(grid1) << " (Expected: 1)" << endl;

    // 测试用例 2
    vector<vector<char>> grid2 = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}
    };
    cout << "Test 2: " << sol.numIslands(grid2) << " (Expected: 3)" << endl;
}
} // namespace Question200

// --- 第 201 题 ---
namespace Question201
{
class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        int shift = 0;
        while (left < right) {
            left >>= 1;
            right >>= 1;
            shift++;
        }
        return left << shift;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    cout << "Test 1 (5, 7): " << sol.rangeBitwiseAnd(5, 7) << " (Expected: 4)" << endl;
    // 5: 101, 6: 110, 7: 111 -> 100 (4)

    // 测试用例 2
    cout << "Test 2 (0, 0): " << sol.rangeBitwiseAnd(0, 0) << " (Expected: 0)" << endl;

    // 测试用例 3
    cout << "Test 3 (1, 2147483647): " << sol.rangeBitwiseAnd(1, 2147483647) << " (Expected: 0)" << endl;
    // 跨度太大，最高位最终也会变为 0
}
} // namespace Question201

// --- 第 202 题 ---
namespace Question202
{
class Solution {
public:
    bool isHappy(int n) {
        int slow = n;
        int fast = getNext(n);
        while (fast != 1 && slow != fast) {
            slow = getNext(slow);
            fast = getNext(getNext(fast));
        }
        return fast == 1;
    }
private:
    int getNext(int n) {
        int totalSum = 0;
        while (n > 0) {
            int d = n % 10;
            n = n / 10;
            totalSum += d * d;
        }
        return totalSum;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 19 是快乐数
    // 1^2 + 9^2 = 82 -> 8^2 + 2^2 = 68 -> 6^2 + 8^2 = 100 -> 1^2 + 0^2 + 0^2 = 1
    cout << "Test 1 (19): " << (sol.isHappy(19) ? "True" : "False") << " (Expected: True)" << endl;

    // 测试用例 2: 2 不是快乐数
    cout << "Test 2 (2): " << (sol.isHappy(2) ? "True" : "False") << " (Expected: False)" << endl;

    // 测试用例 3: 7 是快乐数
    cout << "Test 3 (7): " << (sol.isHappy(7) ? "True" : "False") << " (Expected: True)" << endl;
}
} // namespace Question202

// --- 第 203 题 ---
namespace Question203
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* dummy = new ListNode(0, head);
        ListNode* curr = dummy;
        while (curr->next != nullptr) {
            if (curr->next->val == val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question203

// --- 第 204 题 ---
namespace Question204
{
class Solution {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;
        vector<char> isPrime(n, true);
        isPrime[0] = isPrime[1] = false;
        for (int p = 2; p * p < n; ++p) {
            if (isPrime[p]) {
                for (int i = p * p; i < n; i += p) {
                    isPrime[i] = false;
                }
            }
        }
        int count = 0;
        for (int i = 2; i < n; ++i) {
            if (isPrime[i]) count++;
        }
        return count;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: n = 10
    // 质数有: 2, 3, 5, 7
    cout << "Test 1 (10): " << sol.countPrimes(10) << " (Expected: 4)" << endl;

    // 测试用例 2: n = 0
    cout << "Test 2 (0): " << sol.countPrimes(0) << " (Expected: 0)" << endl;

    // 测试用例 3: n = 1
    cout << "Test 3 (1): " << sol.countPrimes(1) << " (Expected: 0)" << endl;

    // 测试用例 4: n = 2
    cout << "Test 4 (2): " << sol.countPrimes(2) << " (Expected: 0)" << endl;
}
} // namespace Question204

// --- 第 205 题 ---
namespace Question205
{
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if (s.length() != t.length()) return false;
        unordered_map<char, char> s2t;
        unordered_map<char, char> t2s;
        for (int i = 0; i < s.length(); ++i) {
            char charS = s[i];
            char charT = t[i];
            if (s2t.count(charS) && s2t[charS] != charT) {
                return false;
            }
            if (t2s.count(charT) && t2s[charT] != charS) {
                return false;
            }
            s2t[charS] = charT;
            t2s[charT] = charS;
        }
        return true;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: "egg", "add" -> true
    cout << "Test 1 (egg, add): " << (sol.isIsomorphic("egg", "add") ? "True" : "False") 
         << " (Expected: True)" << endl;

    // 测试用例 2: "foo", "bar" -> false
    cout << "Test 2 (foo, bar): " << (sol.isIsomorphic("foo", "bar") ? "True" : "False") 
         << " (Expected: False)" << endl;

    // 测试用例 3: "paper", "title" -> true
    cout << "Test 3 (paper, title): " << (sol.isIsomorphic("paper", "title") ? "True" : "False") 
         << " (Expected: True)" << endl;

    // 测试用例 4: "ab", "aa" -> false
    cout << "Test 4 (ab, aa): " << (sol.isIsomorphic("ab", "aa") ? "True" : "False") 
         << " (Expected: False)" << endl;
}
} // namespace Question205

// --- 第 206 题 ---
namespace Question206
{
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* reverseListItera(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }
    ListNode* reverseListRecursive(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode* newHead = reverseListRecursive(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    } 
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question206

// --- 第 207 题 ---
namespace Question207
{
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> inDegree(numCourses, 0);
        for (auto& pre : prerequisites) {
            int course = pre[0];
            int prerequisite = pre[1];
            adj[prerequisite].push_back(course);
            inDegree[course]++;
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
        int count = 0;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            count++;
            for (int next : adj[curr]) {
                inDegree[next]--;
                if (inDegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        return count == numCourses;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: [[1, 0]] -> 0 -> 1 (可完成)
    vector<vector<int>> pre1 = {{1, 0}};
    cout << "Test 1: " << (sol.canFinish(2, pre1) ? "True" : "False") 
         << " (Expected: True)" << endl;

    // 测试用例 2: [[1, 0], [0, 1]] -> 0 <-> 1 (有环，不可完成)
    vector<vector<int>> pre2 = {{1, 0}, {0, 1}};
    cout << "Test 2: " << (sol.canFinish(2, pre2) ? "True" : "False") 
         << " (Expected: False)" << endl;

    // 测试用例 3: 复杂依赖
    vector<vector<int>> pre3 = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    cout << "Test 3: " << (sol.canFinish(4, pre3) ? "True" : "False") 
         << " (Expected: True)" << endl;
}
} // namespace Question207

// --- 第 208 题 ---
namespace Question208
{
struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};
class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        clear(root);
    }
    
    void insert(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int index = c - 'a';
            if (curr->children[index] == nullptr) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
    }
    
    bool search(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int index = c - 'a';
            if (curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return curr->isEndOfWord;
    }
    
    bool startsWith(string prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return true;
    }
private:
    TrieNode* root;
    void clear(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                clear(node->children[i]);
            }
        }
        delete node;
    }
};
void test() {
    Trie* trie = new Trie();
    
    trie->insert("apple");
    cout << "search(\"apple\"): " << (trie->search("apple") ? "true" : "false") << endl;    // true
    cout << "search(\"app\"): " << (trie->search("app") ? "true" : "false") << endl;        // false
    cout << "startsWith(\"app\"): " << (trie->startsWith("app") ? "true" : "false") << endl; // true
    
    trie->insert("app");
    cout << "search(\"app\"): " << (trie->search("app") ? "true" : "false") << endl;        // true
    
    delete trie;
}
} // namespace Question208

// --- 第 209 题 ---
namespace Question209
{
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int minLen = INT_MAX;
        int left = 0;
        int sum = 0;
        for (int right = 0; right < n; ++right) {
            sum += nums[right];
            while (sum >= target) {
                minLen = min(minLen, right - left + 1);
                sum -= nums[left];
                left++;
            }
        }
        return (minLen == INT_MAX) ? 0 : minLen;
    }
};
void test() {
    Solution sol;

    // 测试用例 1
    vector<int> nums1 = {2, 3, 1, 2, 4, 3};
    int target1 = 7;
    cout << "Test 1: target=7, nums=[2,3,1,2,4,3] -> Expected: 2, Result: " 
         << sol.minSubArrayLen(target1, nums1) << endl;

    // 测试用例 2
    vector<int> nums2 = {1, 1, 1, 1, 1, 1};
    int target2 = 11;
    cout << "Test 2: target=11, nums=[1,1,1,1,1,1] -> Expected: 0, Result: " 
         << sol.minSubArrayLen(target2, nums2) << endl;

    // 测试用例 3
    vector<int> nums3 = {1, 2, 3, 4, 5};
    int target3 = 11;
    cout << "Test 3: target=11, nums=[1,2,3,4,5] -> Expected: 3, Result: " 
         << sol.minSubArrayLen(target3, nums3) << endl;
}
} // namespace Question209

// --- 第 210 题 ---
namespace Question210
{
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> inDegree(numCourses, 0);
        for (auto& pre : prerequisites) {
            int course = pre[0];
            int prerequisites = pre[1];
            adj[prerequisites].push_back(course);
            inDegree[course]++;
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
        vector<int> order;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            order.push_back(curr);
            for (int next : adj[curr]) {
                inDegree[next]--;
                if (inDegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        if (order.size() != numCourses) {
            return {};
        }
        return order;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 正常情况
    // 0 -> 1, 1 -> 2 (学习顺序应该是 0, 1, 2)
    int num1 = 3;
    vector<vector<int>> pre1 = {{1, 0}, {2, 1}};
    vector<int> res1 = sol.findOrder(num1, pre1);
    cout << "Test 1: ";
    for (int x : res1) cout << x << " "; // Expected: 0 1 2
    cout << (res1.empty() ? "Impossible" : "") << endl;

    // 测试用例 2: 存在环
    // 0 -> 1, 1 -> 0
    int num2 = 2;
    vector<vector<int>> pre2 = {{1, 0}, {0, 1}};
    vector<int> res2 = sol.findOrder(num2, pre2);
    cout << "Test 2: ";
    if (res2.empty()) cout << "Empty (Cycle detected)"; // Expected: Empty
    else for (int x : res2) cout << x << " ";
    cout << endl;

    // 测试用例 3: 多个可能的顺序
    int num3 = 4;
    vector<vector<int>> pre3 = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    vector<int> res3 = sol.findOrder(num3, pre3);
    cout << "Test 3: ";
    for (int x : res3) cout << x << " "; // Expected: 0 1 2 3 或 0 2 1 3
    cout << endl;
}
} // namespace Question210

// --- 第 211 题 ---
namespace Question211
{
struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};
class WordDictionary {
public:
    WordDictionary() {
        root = new TrieNode();
    }

    ~WordDictionary() {
        clear(root);
    }
    
    void addWord(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int index = c - 'a';
            if (curr->children[index] == nullptr) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
    }
    
    bool search(string word) {
        return searchInNode(word, 0, root);
    }
private:
    TrieNode* root;
    bool searchInNode(const string& word, int index, TrieNode* node) {
        if (index == word.length()) {
            return node->isEndOfWord;
        }
        char c = word[index];
        if (c != '.') {
            int childIdx = c - 'a';
            if (node->children[childIdx] == nullptr) {
                return false;
            }
            return searchInNode(word, index + 1, node->children[childIdx]);
        } else {
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    if (searchInNode(word, index + 1, node->children[i])) {
                        return true;
                    }
                }
            }
            return false;
        }
    }
    void clear(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) clear(node->children[i]);
        }
        delete node;
    }
};
void test() {
    WordDictionary* wd = new WordDictionary();
    
    wd->addWord("bad");
    wd->addWord("dad");
    wd->addWord("mad");
    
    cout << "search(\"pad\"): " << (wd->search("pad") ? "true" : "false") << endl;   // false
    cout << "search(\"bad\"): " << (wd->search("bad") ? "true" : "false") << endl;   // true
    cout << "search(\".ad\"): " << (wd->search(".ad") ? "true" : "false") << endl;   // true
    cout << "search(\"b..\"): " << (wd->search("b..") ? "true" : "false") << endl;   // true
    cout << "search(\".\"): " << (wd->search(".") ? "true" : "false") << endl;       // false
    
    delete wd;
}
} // namespace Question211

// --- 第 212 题 ---
namespace Question212
{
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    string word = "";
    ~TrieNode() {
        for (int i = 0; i < 26; ++i) {
            if (children[i]) delete children[i];
        }
    }
};
class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        if (board.empty() || board[0].empty()) return {};
        rows = board.size();
        cols = board[0].size();
        TrieNode* root = new TrieNode();
        for (const string& w : words) {
            TrieNode* curr = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!curr->children[idx]) curr->children[idx] = new TrieNode();
                curr = curr->children[idx];
            }
            curr->word = w;
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dfs(board, i, j, root);
            }
        }
        delete root;
        return result;
    }
private:
    int rows, cols;
    vector<string> result;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node) {
        char ch = board[r][c];
        int idx = ch - 'a';
        if (ch == '#' || !node->children[idx]) {
            return;
        }
        TrieNode* nextNode = node->children[idx];
        if (nextNode->word != "") {
            result.push_back(nextNode->word);
            nextNode->word = "";
        }
        board[r][c] = '#';
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                dfs(board, nr, nc, nextNode);
            }
        }
        board[r][c] = ch;
    }
};
void test() {
    Solution sol;
    
    vector<vector<char>> board = {
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    vector<string> words = {"oath","pea","eat","rain"};
    
    vector<string> res = sol.findWords(board, words);
    
    cout << "Result: ";
    for (const string& s : res) cout << s << " "; 
    // Expected: eat oath (顺序可能不同)
    cout << endl;
}
} // namespace Question212

// --- 第 213 题 ---
namespace Question213
{
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
        int res1 = robLinear(nums, 0, n - 2);
        int res2 = robLinear(nums, 1, n - 1);
        return max(res1, res2);
    }
private:
    int robLinear(const vector<int>& nums, int start, int end) {
        int prev2 = 0;
        int prev1 = 0;
        for (int i = start; i <= end; ++i) {
            int temp = prev1;
            prev1 = max(prev1, prev2 + nums[i]);
            prev2 = temp;
        }
        return prev1;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question213

// --- 第 214 题 ---
namespace Question214
{
class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;
        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());
        string t = s + "#" + rev_s;
        int n = t.length();
        vector<int> next(n, 0);
        for (int i = 1, j = 0; i < n; ++i) {
            while (j > 0 && t[i] != t[j]) {
                j = next[j - 1];
            }
            if (t[i] == t[j]) {
                j++;
            }
            next[i] = j;
        }
        int longestPalinePrefixLen = next[n - 1];
        string suffix = s.substr(longestPalinePrefixLen);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: aacecaaa -> aaacecaaa
    string s1 = "aacecaaa";
    cout << "Test 1: s=\"aacecaaa\" -> Expected: aaacecaaa, Result: " << sol.shortestPalindrome(s1) << endl;

    // 测试用例 2: abcd -> dcbabcd
    string s2 = "abcd";
    cout << "Test 2: s=\"abcd\" -> Expected: dcbabcd, Result: " << sol.shortestPalindrome(s2) << endl;

    // 测试用例 3: a -> a
    string s3 = "a";
    cout << "Test 3: s=\"a\" -> Expected: a, Result: " << sol.shortestPalindrome(s3) << endl;
}
} // namespace Question214

// --- 第 215 题 ---
namespace Question215
{
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int, vector<int>, greater<int>> minHeap;
        for (int num : nums) {
            minHeap.push(num);
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }
        return minHeap.top();
    }
    int findKthLargestSTL(vector<int>& nums, int k) {
        nth_element(nums.begin(), nums.begin() + (nums.size() - k), nums.end());
        return nums[nums.size() - k];
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<int> nums1 = {3, 2, 1, 5, 6, 4};
    int k1 = 2;
    cout << "Test 1: nums=[3,2,1,5,6,4], k=2 -> Expected: 5, Result: " 
         << sol.findKthLargest(nums1, k1) << endl;

    // 测试用例 2
    vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int k2 = 4;
    cout << "Test 2: nums=[3,2,3,1,2,4,5,5,6], k=4 -> Expected: 4, Result: " 
         << sol.findKthLargest(nums2, k2) << endl;
}
} // namespace Question215

// --- 第 216 题 ---
namespace Question216
{
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        result.clear();
        path.clear();
        backtrack(1, n, k);
        return result;
    }
private:
    vector<vector<int>> result;
    vector<int> path;
    void backtrack(int start, int target, int k) {
        if (path.size() == k) {
            if (target == 0) {
                result.push_back(path);
            }
            return;
        }
        for (int i = start; i <= 9; ++i) {
            if (i > target) break;
            if ((9 - i + 1) < (k - path.size())) break;
            path.push_back(i);
            backtrack(i + 1, target - i, k);
            path.pop_back();
        }
    }
};
void test() {
    Solution sol;

    // 测试用例 1: k=3, n=7 -> [[1,2,4]]
    int k1 = 3, n1 = 7;
    vector<vector<int>> res1 = sol.combinationSum3(k1, n1);
    cout << "Test 1 (k=3, n=7): ";
    for (auto& vec : res1) {
        cout << "[";
        for (int i = 0; i < vec.size(); ++i) cout << vec[i] << (i == vec.size()-1 ? "" : ",");
        cout << "] ";
    }
    cout << endl;

    // 测试用例 2: k=3, n=9 -> [[1,2,6],[1,3,5],[2,3,4]]
    int k2 = 3, n2 = 9;
    vector<vector<int>> res2 = sol.combinationSum3(k2, n2);
    cout << "Test 2 (k=3, n=9): ";
    for (auto& vec : res2) {
        cout << "[";
        for (int i = 0; i < vec.size(); ++i) cout << vec[i] << (i == vec.size()-1 ? "" : ",");
        cout << "] ";
    }
    cout << endl;
}
} // namespace Question216

// --- 第 217 题 ---
namespace Question217
{
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> seen;
        for (int num : nums) {
            if (seen.find(num) != seen.end()) {
                return true;
            }
            seen.insert(num);
        }
        return false;
    }
    bool containsDuplicateConcise(vector<int>& nums) {
        return unordered_set<int>(nums.begin(), nums.end()).size() < nums.size();
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 有重复
    vector<int> nums1 = {1, 2, 3, 1};
    cout << "Test 1: nums=[1,2,3,1] -> Expected: true, Result: " 
         << (sol.containsDuplicate(nums1) ? "true" : "false") << endl;

    // 测试用例 2: 无重复
    vector<int> nums2 = {1, 2, 3, 4};
    cout << "Test 2: nums=[1,2,3,4] -> Expected: false, Result: " 
         << (sol.containsDuplicate(nums2) ? "true" : "false") << endl;

    // 测试用例 3: 全部重复
    vector<int> nums3 = {1, 1, 1, 1};
    cout << "Test 3: nums=[1,1,1,1] -> Expected: true, Result: " 
         << (sol.containsDuplicate(nums3) ? "true" : "false") << endl;
}
} // namespace Question217

// --- 第 218 题 ---
namespace Question218
{
class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int, int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]});
            events.push_back({b[1], b[2]});
        }
        sort(events.begin(), events.end());
        vector<vector<int>> result;
        multiset<int> heights = {0};
        int prevMax = 0;
        for (auto& event : events) {
            int x = event.first;
            int h = event.second;
            if (h < 0) {
                heights.insert(-h);
            } else {
                heights.erase(heights.find(h));
            }
            int currMax = *heights.rbegin();
            if (currMax != prevMax) {
                result.push_back({x, currMax});
                prevMax = currMax;
            }
        }
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<vector<int>> buildings1 = {{2,9,10}, {3,7,15}, {5,12,12}, {15,20,10}, {19,24,8}};
    vector<vector<int>> res1 = sol.getSkyline(buildings1);
    cout << "Test 1: ";
    for (auto& p : res1) cout << "[" << p[0] << "," << p[1] << "] ";
    // Expected: [2,10] [3,15] [7,12] [12,0] [15,10] [20,8] [24,0]
    cout << endl;

    // 测试用例 2: 两个建筑无缝连接
    vector<vector<int>> buildings2 = {{1,2,1}, {2,3,1}};
    vector<vector<int>> res2 = sol.getSkyline(buildings2);
    cout << "Test 2: ";
    for (auto& p : res2) cout << "[" << p[0] << "," << p[1] << "] ";
    // Expected: [1,1] [3,0]
    cout << endl;
}
} // namespace Question218

// --- 第 219 题 ---
namespace Question219
{
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_set<int> window;
        for (int i = 0; i < nums.size(); ++i) {
            if (window.find(nums[i]) != window.end()) {
                return true;
            }
            window.insert(nums[i]);
            if (window.size() > k) {
                window.erase(nums[i - k]);
            }
        }
        return false;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: [1,2,3,1], k = 3 -> true (索引 0 和 3, 3-0 <= 3)
    vector<int> nums1 = {1, 2, 3, 1};
    int k1 = 3;
    cout << "Test 1: nums=[1,2,3,1], k=3 -> Expected: true, Result: " 
         << (sol.containsNearbyDuplicate(nums1, k1) ? "true" : "false") << endl;

    // 测试用例 2: [1,0,1,1], k = 1 -> true (索引 2 和 3, 3-2 <= 1)
    vector<int> nums2 = {1, 0, 1, 1};
    int k2 = 1;
    cout << "Test 2: nums=[1,0,1,1], k=1 -> Expected: true, Result: " 
         << (sol.containsNearbyDuplicate(nums2, k2) ? "true" : "false") << endl;

    // 测试用例 3: [1,2,3,1,2,3], k = 2 -> false (最近重复距离为 3)
    vector<int> nums3 = {1, 2, 3, 1, 2, 3};
    int k3 = 2;
    cout << "Test 3: nums=[1,2,3,1,2,3], k=2 -> Expected: false, Result: " 
         << (sol.containsNearbyDuplicate(nums3, k3) ? "true" : "false") << endl;
}
} // namespace Question219

// --- 第 220 题 ---
namespace Question220
{
class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        if (indexDiff <= 0 || valueDiff < 0) return false;
        set<long long> window;
        for (int i = 0; i < nums.size(); ++i) {
            long long x = nums[i];
            auto it = window.lower_bound(x - valueDiff);
            if (it != window.end() && *it <= x + valueDiff) {
                return true;
            }
            window.insert(x);
            if (window.size() > indexDiff) {
                window.erase((long long)nums[i - indexDiff]);
            }
        }
        return false;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: [1,2,3,1], indexDiff=3, valueDiff=0 -> true (1 and 1)
    vector<int> nums1 = {1, 2, 3, 1};
    cout << "Test 1: nums=[1,2,3,1], id=3, vd=0 -> Expected: true, Result: " 
         << (sol.containsNearbyAlmostDuplicate(nums1, 3, 0) ? "true" : "false") << endl;

    // 测试用例 2: [1,5,9,1,5,9], indexDiff=2, valueDiff=3 -> false
    vector<int> nums2 = {1, 5, 9, 1, 5, 9};
    cout << "Test 2: nums=[1,5,9,1,5,9], id=2, vd=3 -> Expected: false, Result: " 
         << (sol.containsNearbyAlmostDuplicate(nums2, 2, 3) ? "true" : "false") << endl;

    // 测试用例 3: [1,2,3,1], indexDiff=1, valueDiff=1 -> true (1,2 or 2,3 or 3,1)
    vector<int> nums3 = {1, 2, 3, 1};
    cout << "Test 3: nums=[1,2,3,1], id=1, vd=1 -> Expected: true, Result: " 
         << (sol.containsNearbyAlmostDuplicate(nums3, 1, 1) ? "true" : "false") << endl;
}
} // namespace Question220

// --- 第 221 题 ---
namespace Question221
{
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int rows = matrix.size();
        int cols = matrix[0].size();
        vector<vector<int>> dp(rows, vector<int>(cols, 0));
        int maxSide = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (matrix[i][j] == '1') {
                    if (i == 0 || j == 0) {
                        dp[i][j] = 1;
                    } else {
                        dp[i][j] = min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}) + 1;
                    }
                    maxSide = max(maxSide, dp[i][j]);
                }
            }
        }
        return maxSide * maxSide;
    }
};
void test() {
    Solution sol;
    // 测试用例 1
    vector<vector<char>> matrix1 = {
        {'1', '0', '1', '0', '0'},
        {'1', '0', '1', '1', '1'},
        {'1', '1', '1', '1', '1'},
        {'1', '0', '0', '1', '0'}
    };
    cout << "Test 1: " << sol.maximalSquare(matrix1) << " (Expected: 4)" << endl;

    // 测试用例 2
    vector<vector<char>> matrix2 = {
        {'0', '1'},
        {'1', '0'}
    };
    cout << "Test 2: " << sol.maximalSquare(matrix2) << " (Expected: 1)" << endl;

    // 测试用例 3
    vector<vector<char>> matrix3 = {
        {'1', '1'},
        {'1', '1'}
    };
    cout << "Test 3: " << sol.maximalSquare(matrix3) << " (Expected: 4)" << endl;
}
} // namespace Question221

// --- 第 222 题 ---
namespace Question222
{
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int leftHeight = 0, rightHeight = 0;
        TreeNode* leftNode = root, *rightNode = root;
        while (leftNode) {
            leftHeight++;
            leftNode = leftNode->left;
        }
        while (rightNode) {
            rightHeight++;
            rightNode = rightNode->right;
        }
        if (leftHeight == rightHeight) {
            return (1 << leftHeight) - 1;
        }
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
};
void test() {
    Solution sol;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    cout << "Test 1 (Expected 6): " << sol.countNodes(root) << endl;

    // 测试用例 2: 空树
    cout << "Test 2 (Expected 0): " << sol.countNodes(nullptr) << endl;

    // 测试用例 3: 单节点
    TreeNode* root3 = new TreeNode(1);
    cout << "Test 3 (Expected 1): " << sol.countNodes(root3) << endl;
}
} // namespace Question222

// --- 第 223 题 ---
namespace Question223
{
class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        int areaA = (ax2 - ax1) * (ay2 - ay1);
        int areaB = (bx2 - bx1) * (by2 - by1);
        int overlapX1 = max(ax1, bx1);
        int overlapX2 = min(ax2, bx2);
        int overlapY1 = max(ay1, by1);
        int overlapY2 = min(ay2, by2);
        int overlapWidth = overlapX2 - overlapX1;
        int overlapHeight = overlapY2 - overlapY1;
        int overlapArea = 0;
        if (overlapWidth > 0 && overlapHeight > 0) {
            overlapArea = overlapWidth * overlapHeight;
        }
        return areaA + areaB - overlapArea;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 部分重叠
    // A: (0,0) to (2,2), B: (1,0) to (3,1)
    // Area A = 4, Area B = 2, Overlap = (1,0) to (2,1) = 1
    // Total = 4 + 2 - 1 = 5
    cout << "Test 1 (Expected 5): " << sol.computeArea(0, 0, 2, 2, 1, 0, 3, 1) << endl;

    // 测试用例 2: 完全没有重叠
    cout << "Test 2 (Expected 4): " << sol.computeArea(0, 0, 1, 1, 2, 2, 3, 3) << endl;

    // 测试用例 3: 一个包含另一个
    // A: (0,0) to (4,4), B: (1,1) to (2,2)
    // Area A = 16, Area B = 1, Overlap = 1
    // Total = 16 + 1 - 1 = 16
    cout << "Test 3 (Expected 16): " << sol.computeArea(0, 0, 4, 4, 1, 1, 2, 2) << endl;

    // 测试用例 4: 恰好边界接触 (无重叠面积)
    cout << "Test 4 (Expected 2): " << sol.computeArea(0, 0, 1, 1, 1, 0, 2, 1) << endl;
}
} // namespace Question223

// --- 第 224 题 ---
namespace Question224
{
class Solution {
public:
    int calculate(string s) {
        stack<int> st;
        int res = 0;
        int num = 0;
        int sign = 1;
        for (int i = 0; i < s.length(); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            } else if (c == '+') {
                res += sign * num;
                num = 0;
                sign = 1;
            } else if (c == '-') {
                res += sign * num;
                num = 0;
                sign = -1;
            } else if (c == '(') {
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            } else if (c == ')') {
                res += sign * num;
                num = 0;
                int prevSign = st.top();
                st.pop();
                res *= prevSign;
                int prevRes = st.top();
                st.pop();
                res += prevRes;
            }
        }
        res += sign * num;
        return res;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 简单加减
    cout << "Test 1 (Expected 3): " << sol.calculate(" 2-1 + 2 ") << endl;

    // 测试用例 2: 嵌套括号
    // 1 + (4 + 5 + 2) - 3 = 1 + 11 - 3 = 9
    cout << "Test 2 (Expected 9): " << sol.calculate("(1+(4+5+2)-3)") << endl;

    // 测试用例 3: 负号在括号前
    // 1 - (5 - (2 + 1)) = 1 - (5 - 3) = 1 - 2 = -1
    cout << "Test 3 (Expected -1): " << sol.calculate("1 - (5 - (2 + 1))") << endl;

    // 测试用例 4: 复杂组合
    cout << "Test 4 (Expected 2): " << sol.calculate(" 1 + 1 ") << endl;
}
} // namespace Question224

// --- 第 225 题 ---
namespace Question225
{
class MyStack {
public:
    MyStack() {
        
    }
    
    void push(int x) {
        q.push(x);
        int size = q.size();
        for (int i = 0; i < size - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }
    
    int pop() {
        int topVal = q.front();
        q.pop();
        return topVal;
    }
    
    int top() {
        return q.front();
    }
    
    bool empty() {
        return q.empty();
    }
private:
    queue<int> q;
};
void test() {
    MyStack st;
    
    // 测试用例: push 1, push 2, top, pop, empty
    st.push(1);
    st.push(2);
    
    cout << "Test 1 (top should be 2): " << st.top() << endl;   // Expected: 2
    cout << "Test 2 (pop should be 2): " << st.pop() << endl;   // Expected: 2
    cout << "Test 3 (top should be 1): " << st.top() << endl;   // Expected: 1
    
    st.pop();
    cout << "Test 4 (empty should be 1): " << st.empty() << endl; // Expected: 1 (true)
}
} // namespace Question225

// --- 第 226 题 ---
namespace Question226
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) {
            return nullptr;
        }
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        root->left = right;
        root->right = left;
        return root;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question226

// --- 第 227 题 ---
namespace Question227
{
class Solution {
public:
    int calculate(string s) {
        if (s.empty()) return 0;
        stack<int> st;
        long long num = 0;
        char sign = '+';
        for (int i = 0; i < s.length(); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            }
            if ((!isdigit(c) && c != ' ') || i == s.length() - 1) {
                if (sign == '+') {
                    st.push(num);
                } else if (sign == '-') {
                    st.push(-num);
                } else if (sign == '*') {
                    int top = st.top();
                    st.pop();
                    st.push(top * num);
                } else if (sign == '/') {
                    int top = st.top();
                    st.pop();
                    st.push(top / num);
                }
                sign = c;
                num = 0;
            }
        }
        int result = 0;
        while (!st.empty()) {
            result += st.top();
            st.pop();
        }
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 基础优先级
    // 3 + 2 * 2 = 3 + 4 = 7
    cout << "Test 1 (Expected 7): " << sol.calculate("3+2*2") << endl;

    // 测试用例 2: 包含空格和除法
    // 3 / 2 = 1
    cout << "Test 2 (Expected 1): " << sol.calculate(" 3 / 2 ") << endl;

    // 测试用例 3: 混合运算
    // 3 + 5 / 2 = 3 + 2 = 5
    cout << "Test 3 (Expected 5): " << sol.calculate("3+5/2") << endl;

    // 测试用例 4: 连续乘除
    // 10 * 2 / 5 = 20 / 5 = 4
    cout << "Test 4 (Expected 4): " << sol.calculate("10*2/5") << endl;

    // 测试用例 5: 负数结果
    // 1 - 2 * 3 = 1 - 6 = -5
    cout << "Test 5 (Expected -5): " << sol.calculate("1-2*3") << endl;
}
} // namespace Question227

// --- 第 228 题 ---
namespace Question228
{
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> result;
        int n = nums.size();
        if (n == 0) return result;
        for (int i = 0; i < n; i++) {
            int start = nums[i];
            while (i + 1 < n && nums[i + 1] == nums[i] + 1) {
                i++;
            }
            if (start == nums[i]) {
                result.push_back(to_string(start));
            } else {
                result.push_back(to_string(start) + "->" + to_string(nums[i]));
            }
        }
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 混合区间
    vector<int> nums1 = {0, 1, 2, 4, 5, 7};
    vector<string> res1 = sol.summaryRanges(nums1);
    cout << "Test 1: ";
    for (const string& s : res1) cout << "[" << s << "] "; 
    cout << " (Expected: [0->2] [4->5] [7])" << endl;

    // 测试用例 2: 全部连续
    vector<int> nums2 = {1, 2, 3, 4, 5};
    vector<string> res2 = sol.summaryRanges(nums2);
    cout << "Test 2: ";
    for (const string& s : res2) cout << "[" << s << "] "; 
    cout << " (Expected: [1->5])" << endl;

    // 测试用例 3: 全部不连续
    vector<int> nums3 = {1, 3, 5, 7};
    vector<string> res3 = sol.summaryRanges(nums3);
    cout << "Test 3: ";
    for (const string& s : res3) cout << "[" << s << "] "; 
    cout << " (Expected: [1] [3] [5] [7])" << endl;

    // 测试用例 4: 空数组
    vector<int> nums4 = {};
    vector<string> res4 = sol.summaryRanges(nums4);
    cout << "Test 4: Size is " << res4.size() << " (Expected: 0)" << endl;
}
} // namespace Question228

// --- 第 229 题 ---
namespace Question229
{
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        if (nums.empty()) return {};
        int cand1 = 0, cand2 = 0;
        int count1 = 0, count2 = 0;
        for (int n : nums) {
            if (n == cand1) {
                count1++;
            } else if (n == cand2) {
                count2++;
            } else if (count1 == 0) {
                cand1 = n;
                count1 = 1;
            } else if (count2 == 0) {
                cand2 = n;
                count2 = 1;
            } else {
                count1--;
                count2--;
            }
        }
        vector<int> result;
        int actualCount1 = 0, actualCount2 = 0;
        for (int n : nums) {
            if (n == cand1) actualCount1++;
            else if (n == cand2) actualCount2++;
        }
        int nSize = nums.size();
        if (actualCount1 > nSize / 3) result.push_back(cand1);
        if (actualCount2 > nSize / 3) result.push_back(cand2);
        return result;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 两个元素达标
    vector<int> nums1 = {3, 2, 3};
    vector<int> res1 = sol.majorityElement(nums1);
    cout << "Test 1: "; 
    for(int x : res1) cout << x << " "; // Expected: 3
    cout << endl;

    // 测试用例 2: 两个元素达标
    vector<int> nums2 = {1, 1, 1, 3, 3, 2, 2, 2};
    vector<int> res2 = sol.majorityElement(nums2);
    cout << "Test 2: "; 
    for(int x : res2) cout << x << " "; // Expected: 1 2
    cout << endl;

    // 测试用例 3: 无元素达标
    vector<int> nums3 = {1, 2, 3};
    vector<int> res3 = sol.majorityElement(nums3);
    cout << "Test 3: Size is " << res3.size() << " (Expected: 0)" << endl;
}
} // namespace Question229

// --- 第 230 题 ---
namespace Question230
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        TreeNode* curr = root;
        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            k--;
            if (k == 0) {
                return curr->val;
            }
            curr = curr->right;
        }
        return -1;
    }
};
void test() {
    Solution sol;
    // 构建测试树: [3, 1, 4, null, 2]
    //       3
    //      / \
    //     1   4
    //      \
    //       2
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(1);
    root->right = new TreeNode(4);
    root->left->right = new TreeNode(2);

    cout << "Test 1 (k=1, Expected 1): " << sol.kthSmallest(root, 1) << endl;
    cout << "Test 2 (k=2, Expected 2): " << sol.kthSmallest(root, 2) << endl;
    cout << "Test 3 (k=3, Expected 3): " << sol.kthSmallest(root, 3) << endl;
}
} // namespace Question230

// --- 第 231 题 ---
namespace Question231
{
class Solution {
public:
    bool isPowerOfTwo(int n) {
        return n > 0 && (n & (long long)(n - 1)) == 0;
    }
};
void test() {
    Solution sol;
    // 测试用例 1: 是 2 的幂
    cout << "Test 1 (n=1, Expected 1): " << sol.isPowerOfTwo(1) << endl;
    cout << "Test 2 (n=16, Expected 1): " << sol.isPowerOfTwo(16) << endl;
    
    // 测试用例 2: 不是 2 的幂
    cout << "Test 3 (n=3, Expected 0): " << sol.isPowerOfTwo(3) << endl;
    cout << "Test 4 (n=18, Expected 0): " << sol.isPowerOfTwo(18) << endl;
    
    // 测试用例 3: 边界情况
    cout << "Test 5 (n=0, Expected 0): " << sol.isPowerOfTwo(0) << endl;
    cout << "Test 6 (n=-16, Expected 0): " << sol.isPowerOfTwo(-16) << endl;
}
} // namespace Question231

// --- 第 232 题 ---
namespace Question232
{
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
void test() {
    MyQueue mq;
    
    mq.push(1);
    mq.push(2);
    
    cout << "Test 1 (peek should be 1): " << mq.peek() << endl;   // Expected: 1
    cout << "Test 2 (pop should be 1): " << mq.pop() << endl;     // Expected: 1
    
    mq.push(3);
    cout << "Test 3 (pop should be 2): " << mq.pop() << endl;     // Expected: 2
    cout << "Test 4 (pop should be 3): " << mq.pop() << endl;     // Expected: 3
    cout << "Test 5 (empty should be 1): " << mq.empty() << endl; // Expected: 1 (true)
}
} // namespace Question232

// --- 第 233 题 ---
namespace Question233
{
class Solution {
public:
    int countDigitOne(int n) {
        if (n <= 0) return 0;
        long long count = 0;
        for (long long i = 1; i <= n; i *= 10) {
            long long higher = n / (i * 10);
            int curr = (n / i) % 10;
            long long lower = n % i;
            if (curr > 1) {
                count += (higher + 1) * i;
            } else if (curr == 1) {
                count += (higher * i) + (lower + 1);
            } else {
                count += higher * i;
            }
        }
        return (int)count;
    }
};
void test() {
    Solution sol;
    cout << "n=13, result=" << sol.countDigitOne(13) << " (Expected: 6)" << endl;
    cout << "n=0, result=" << sol.countDigitOne(0) << " (Expected: 0)" << endl;
    cout << "n=100, result=" << sol.countDigitOne(100) << " (Expected: 21)" << endl;
}
} // namespace Question233

// --- 第 234 题 ---
namespace Question234
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode *slow = head, *fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode* secondHalfHead = reverseList(slow->next);
        ListNode* p1 = head;
        ListNode* p2 = secondHalfHead;
        bool result = true;
        while (p2 != nullptr) {
            if (p1->val != p2->val) {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        slow->next = reverseList(secondHalfHead);
        return result;
    }
private:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question234

// --- 第 235 题 ---
namespace Question235
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (root != nullptr) {
            if (p->val > root->val && q->val > root->val) {
                root = root->right;
            } else if (p->val < root->val && q->val < root->val) {
                root = root->left;
            } else {
                return root;
            }
        }
        return nullptr;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question235

// --- 第 236 题 ---
namespace Question236
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == nullptr || root == p || root == q) {
            return root;
        }
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left != nullptr && right != nullptr) {
            return root; 
        }
        return (left != nullptr) ? left : right;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question236

// --- 第 237 题 ---
namespace Question237
{
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};
class Solution {
public:
    void deleteNode(ListNode* node) {
        ListNode* nextNode = node->next;
        node->val = nextNode->val;
        node->next = nextNode->next;
        delete nextNode;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question237

// --- 第 238 题 ---
namespace Question238
{
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, 1);
        int left = 1;
        for (int i = 0; i < n; ++i) {
            res[i] = left;
            left *= nums[i];
        }
        int right = 1;
        for (int i = n - 1; i >= 0; --i) {
            res[i] *= right;
            right *= nums[i];
        }
        return res;
    }
};
void test() {
    Solution sol;
    
    vector<int> case1 = {1, 2, 3, 4};
    vector<int> res1 = sol.productExceptSelf(case1);
    cout << "Test 1 [1,2,3,4]: ";
    for (int x : res1) cout << x << " "; // Expected: 24 12 8 6
    cout << endl;

    vector<int> case2 = {-1, 1, 0, -3, 3};
    vector<int> res2 = sol.productExceptSelf(case2);
    cout << "Test 2 [-1,1,0,-3,3]: ";
    for (int x : res2) cout << x << " "; // Expected: 0 0 9 0 0
    cout << endl;
}
} // namespace Question238

// --- 第 239 题 ---
namespace Question239
{
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0) return {};
        if (k == 1) return nums;
        vector<int> res;
        deque<int> dq;
        for (int i = 0; i < n; ++i) {
            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }
            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                dq.pop_back();
            }
            dq.push_back(i);
            if (i >= k - 1) {
                res.push_back(nums[dq.front()]);
            }
        }
        return res;
    }
};
void test() {
    Solution sol;
    
    vector<int> case1 = {1, 3, -1, -3, 5, 3, 6, 7};
    int k1 = 3;
    vector<int> res1 = sol.maxSlidingWindow(case1, k1);
    cout << "Test 1: ";
    for (int x : res1) cout << x << " "; // Expected: 3 3 5 5 6 7
    cout << endl;

    vector<int> case2 = {1};
    int k2 = 1;
    vector<int> res2 = sol.maxSlidingWindow(case2, k2);
    cout << "Test 2: ";
    for (int x : res2) cout << x << " "; // Expected: 1
    cout << endl;
}
} // namespace Question239

// --- 第 240 题 ---
namespace Question240
{
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int rows = matrix.size();
        int cols = matrix[0].size();
        int row = 0;
        int col = cols - 1;
        while (row < rows && col >= 0) {
            if (matrix[row][col] == target) {
                return true;
            } else if (matrix[row][col] > target) {
                col--;
            } else {
                row++;
            }
        }
        return false;
    }
};
void test() {
    Solution sol;
    
    vector<vector<int>> matrix = {
        {1,   4,  7, 11, 15},
        {2,   5,  8, 12, 19},
        {3,   6,  9, 16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}
    };

    cout << "Test 1 (target=5): " << (sol.searchMatrix(matrix, 5) ? "True" : "False") << " (Expected: True)" << endl;
    cout << "Test 2 (target=20): " << (sol.searchMatrix(matrix, 20) ? "True" : "False") << " (Expected: False)" << endl;
    cout << "Test 3 (target=1): " << (sol.searchMatrix(matrix, 1) ? "True" : "False") << " (Expected: True)" << endl;
    cout << "Test 4 (target=30): " << (sol.searchMatrix(matrix, 30) ? "True" : "False") << " (Expected: True)" << endl;
}
} // namespace Question240

// --- 第 241 题 ---
namespace Question241
{
class Solution {
    unordered_map<string, vector<int>> memo;
public:
    vector<int> diffWaysToCompute(string expression) {
        if (memo.count(expression)) {
            return memo[expression];
        }
        vector<int> res;
        for (int i = 0; i < expression.size(); ++i) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                string leftStr = expression.substr(0, i);
                string rightStr = expression.substr(i + 1);
                vector<int> leftResults = diffWaysToCompute(leftStr);
                vector<int> rightResults = diffWaysToCompute(rightStr);
                for (int l : leftResults) {
                    for (int r : rightResults) {
                        if (c == '+') res.push_back(l + r);
                        else if (c == '-') res.push_back(l - r);
                        else if (c == '*') res.push_back(l * r);
                    }
                }
            }
        }
        if (res.empty()) {
            res.push_back(stoi(expression));
        }
        return memo[expression] = res;
    }
};
void test() {
    Solution sol;
    
    string exp1 = "2-1-1";
    vector<int> res1 = sol.diffWaysToCompute(exp1);
    cout << "Test 1 (2-1-1): ";
    for (int x : res1) cout << x << " "; // Expected: 0 2 ( (2-1)-1=0, 2-(1-1)=2 )
    cout << endl;

    string exp2 = "2*3-4*5";
    vector<int> res2 = sol.diffWaysToCompute(exp2);
    cout << "Test 2 (2*3-4*5): ";
    for (int x : res2) cout << x << " "; // Expected: -14 -10 -10 -14 (等)
    cout << endl;
}
} // namespace Question241

// --- 第 242 题 ---
namespace Question242
{
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length()) {
            return false;
        }
        int counts[26] = {0};
        for (int i = 0; i < s.length(); ++i) {
            counts[s[i] - 'a']++;
            counts[t[i] - 'a']--;
        }
        for (int i = 0; i < 26; ++i) {
            if (counts[i] != 0) {
                return false;
            }
        }
        return true;
    }
};
void test() {
    Solution sol;
    
    string s1 = "anagram", t1 = "nagaram";
    cout << "Test 1 (anagram, nagaram): " << (sol.isAnagram(s1, t1) ? "True" : "False") << " (Expected: True)" << endl;

    string s2 = "rat", t2 = "car";
    cout << "Test 2 (rat, car): " << (sol.isAnagram(s2, t2) ? "True" : "False") << " (Expected: False)" << endl;

    string s3 = "a", t3 = "a";
    cout << "Test 3 (a, a): " << (sol.isAnagram(s3, t3) ? "True" : "False") << " (Expected: True)" << endl;
    
    string s4 = "hello", t4 = "world";
    cout << "Test 4 (hello, world): " << (sol.isAnagram(s4, t4) ? "True" : "False") << " (Expected: False)" << endl;
}
} // namespace Question242

// --- 第 257 题 ---
namespace Question257
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> res;
        if (!root) return res;
        dfs(root, "", res);
        return res;
    }
private:
    void dfs(TreeNode* node, string path, vector<string>& res) {
        path += to_string(node->val);
        if (!node->left && !node->right) {
            res.push_back(path);
            return;
        }
        if (node->left) {
            dfs(node->left, path + "->", res);
        }
        if (node->right) {
            dfs(node->right, path + "->", res);
        }
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question257

// --- 第 258 题 ---
namespace Question258
{
class Solution {
public:
    int addDigits(int num) {
        if (num == 0) return 0;
        return (num % 9 == 0) ? 9 : (num % 9);
    }
    int addDigitsSimulation(int num) {
        while (num >= 10) {
            int sum = 0;
            while (num > 0) {
                sum += num % 10;
                num /= 10;
            }
            num = sum;
        }
        return num;
    }
};
void test() {
    Solution sol;
    
    int case1 = 38;
    cout << "Test 1 (38): " << sol.addDigits(case1) << " (Expected: 2)" << endl;

    int case2 = 0;
    cout << "Test 2 (0): " << sol.addDigits(case2) << " (Expected: 0)" << endl;

    int case3 = 9;
    cout << "Test 3 (9): " << sol.addDigits(case3) << " (Expected: 9)" << endl;

    int case4 = 18;
    cout << "Test 4 (18): " << sol.addDigits(case4) << " (Expected: 9)" << endl;
}
} // namespace Question258

// --- 第 260 题 ---
namespace Question260
{
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        long long xorSum = 0;
        for (int num : nums) {
            xorSum ^= num;
        }
        int diff = xorSum & -xorSum;
        int a = 0;
        int b = 0;
        for (int num : nums) {
            if ((num & diff) == 0) {
                a ^= num;
            } else {
                b ^= num;
            }
        }
        return {a, b};
    }
};
void test() {
    Solution sol;
    
    vector<int> case1 = {1, 2, 1, 3, 2, 5};
    vector<int> res1 = sol.singleNumber(case1);
    cout << "Test 1 [1,2,1,3,2,5]: " << res1[0] << ", " << res1[1] << " (Expected: 3, 5)" << endl;

    vector<int> case2 = {-1, 0};
    vector<int> res2 = sol.singleNumber(case2);
    cout << "Test 2 [-1,0]: " << res2[0] << ", " << res2[1] << " (Expected: -1, 0)" << endl;
}
} // namespace Question260

// --- 第 263 题 ---
namespace Question263
{
class Solution {
public:
    bool isUgly(int n) {
        if (n <= 0) return false;
        int factors[] = {2, 3, 5};
        for (int f : factors) {
            while (n % f == 0) {
                n /= f;
            }
        }
        return n == 1;
    }
};
void test() {
    Solution sol;
    // 测试用例
    cout << "Test 6: " << (sol.isUgly(6) == true ? "Pass" : "Fail") << endl;    // 6 = 2 * 3 (True)
    cout << "Test 1: " << (sol.isUgly(1) == true ? "Pass" : "Fail") << endl;    // 1 (True)
    cout << "Test 14: " << (sol.isUgly(14) == false ? "Pass" : "Fail") << endl; // 14 = 2 * 7 (False)
    cout << "Test 8: " << (sol.isUgly(8) == true ? "Pass" : "Fail") << endl;    // 8 = 2*2*2 (True)
    cout << "Test 0: " << (sol.isUgly(0) == false ? "Pass" : "Fail") << endl;   // 0 (False)
}
} // namespace Question263

// --- 第 264 题 ---
namespace Question264
{class Solution {
public:
    int nthUglyNumber(int n) {
        if (n <= 0) return 0;
        vector<int> dp(n);
        dp[0] = 1;
        int p2 = 0, p3 = 0, p5 = 0;
        for (int i = 1; i < n; ++i) {
            int next2 = dp[p2] * 2;
            int next3 = dp[p3] * 3;
            int next5 = dp[p5] * 5;
            int nextUgly = min({next2, next3, next5});
            dp[i] = nextUgly;
            if (nextUgly == next2) p2++;
            if (nextUgly == next3) p3++;
            if (nextUgly == next5) p5++;
        }
        return dp[n - 1];
    }
};
void test() {
    Solution sol;
    // 测试用例
    cout << "Test n=1: " << (sol.nthUglyNumber(1) == 1 ? "Pass" : "Fail") << " (Expected 1)" << endl;
    cout << "Test n=10: " << (sol.nthUglyNumber(10) == 12 ? "Pass" : "Fail") << " (Expected 12)" << endl;
    cout << "Test n=11: " << (sol.nthUglyNumber(11) == 15 ? "Pass" : "Fail") << " (Expected 15)" << endl;
}
} // namespace Question264

// --- 第 268 题 ---
namespace Question268
{
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        // 方法一：数学求和法 (Gauss's Formula)
        // 0 到 n 的等差数列之和公式为: Sum = n * (n + 1) / 2
        // 缺失的数字 = 理论总和 - 数组实际总和
        long long expectedSum = (long long)n * (n + 1) / 2;
        long long actualSum = 0;
        for (int num : nums) {
            actualSum += num;
        }
        return (int)(expectedSum - actualSum);
        /* 
        // 方法二：异或法 (XOR) - 更极客，且没有溢出风险
        // 利用性质：a ^ a = 0,  a ^ 0 = a
        // 将 [0, n] 的所有数与 nums 数组中的所有数全部异或在一起
        // 成对的数字会抵消为 0，最后剩下的就是那个缺失的数
        int res = n; // 初始化为 n，因为循环只到 n-1
        for (int i = 0; i < n; ++i) {
            res ^= i ^ nums[i];
        }
        return res;
        */
    }
};
void test() {
    Solution sol;
    
    // 用例 1: [3, 0, 1] -> n=3, 范围[0,3], 缺失 2
    vector<int> v1 = {3, 0, 1};
    cout << "Test 1: " << (sol.missingNumber(v1) == 2 ? "Pass" : "Fail") << " (Expected 2)" << endl;
    
    // 用例 2: [0, 1] -> n=2, 范围[0,2], 缺失 2
    vector<int> v2 = {0, 1};
    cout << "Test 2: " << (sol.missingNumber(v2) == 2 ? "Pass" : "Fail") << " (Expected 2)" << endl;
    
    // 用例 3: [9,6,4,2,3,5,7,0,1] -> n=9, 范围[0,9], 缺失 8
    vector<int> v3 = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    cout << "Test 3: " << (sol.missingNumber(v3) == 8 ? "Pass" : "Fail") << " (Expected 8)" << endl;
}
} // namespace Question268

// --- 第 273 题 ---
namespace Question273
{
class Solution {
public:
    string numberToWords(int num) {
        if (num == 0) return "Zero";

        string res = "";
        int i = 0; // 用于在 thousands 数组中索引 (Thousand, Million, Billion)

        while (num > 0) {
            // 每次处理 3 位数字
            if (num % 1000 != 0) {
                // 处理当前 3 位，并加上对应的单位（如 Thousand）
                string part = helper(num % 1000);
                string unit = thousands[i];
                
                // 构建当前 3 位组： [数字部分] + [单位]
                string currentGroup = part;
                if (unit != "") {
                    currentGroup += " " + unit;
                }

                // 将当前组拼接到最终结果的前面： [当前组] + [之前的结果]
                if (res == "") {
                    res = currentGroup;
                } else {
                    res = currentGroup + " " + res;
                }
            }
            num /= 1000;
            i++;
        }

        // 修剪末尾多余的空格
        while (res.back() == ' ') {
            res.pop_back();
        }
        
        return res;
    }

private:
    // 定义基础词库
    vector<string> below_20 = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", 
                               "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    vector<string> tens = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    vector<string> thousands = {"", "Thousand", "Million", "Billion"};
    // 辅助函数：处理 0-999 之间的数字
    string helper(int num) {
        if (num == 0) return "";
        if (num < 20) return below_20[num];
        if (num < 100) {
            string res = tens[num / 10];
            string rem = helper(num % 10);
            return rem == "" ? res : res + " " + rem;
        }
        
        // 百位处理：数字 + "Hundred" + 递归处理剩下的十位和个位
        string res = below_20[num / 100] + " Hundred";
        string rem = helper(num % 100);
        return rem == "" ? res : res + " " + rem;
    }
};
void test() {
    Solution sol;
    cout << "Test 123: " << sol.numberToWords(123) << endl;           // One Hundred Twenty Three
    cout << "Test 12345: " << sol.numberToWords(12345) << endl;       // Twelve Thousand Three Hundred Forty Five
    cout << "Test 1234567: " << sol.numberToWords(1234567) << endl;   // One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven
    cout << "Test 1000000: " << sol.numberToWords(1000000) << endl;   // One Million
    cout << "Test 0: " << sol.numberToWords(0) << endl;               // Zero
}
} // namespace Question273

// --- 第 274 题 ---
namespace Question274
{
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        vector<int> counter(n + 1, 0);
        for (int c : citations) {
            if (c >= n) {
                counter[n]++;
            } else {
                counter[c]++;
            }
        }
        int totalPapers = 0;
        for (int h = n; h >= 0; --h) {
            totalPapers += counter[h];
            if (totalPapers >= h) {
                return h;
            }
        }
        return 0;
    }
};
void test() {
    Solution sol;
    
    // 用例 1: [3, 0, 6, 1, 5] 
    // 排序后: [6, 5, 3, 1, 0]
    // 3 篇论文引用 >= 3, 所以 h=3
    vector<int> v1 = {3, 0, 6, 1, 5};
    cout << "Test 1: " << (sol.hIndex(v1) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
    
    // 用例 2: [6, 6, 6, 6, 6] 
    // 5 篇论文引用 >= 5, 所以 h=5
    vector<int> v2 = {6, 6, 6, 6, 6};
    cout << "Test 2: " << (sol.hIndex(v2) == 5 ? "Pass" : "Fail") << " (Expected 5)" << endl;
    
    // 用例 3: [0, 0, 0]
    // 0 篇论文引用 >= 1, 所以 h=0
    vector<int> v3 = {0, 0, 0};
    cout << "Test 3: " << (sol.hIndex(v3) == 0 ? "Pass" : "Fail") << " (Expected 0)" << endl;
}
} // namespace Question274

// --- 第 275 题 ---
namespace Question275
{
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        int left = 0, right = n - 1;
        int ans = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            // 从 mid 到 n-1 共有 (n - mid) 篇论文
            // 这些论文的引用次数都 >= citations[mid]
            int h_candidate = n - mid;

            if (citations[mid] >= h_candidate) {
                // 找到了一个符合条件的 h，记录下来
                ans = h_candidate;
                // 尝试寻找更大的 h，即尝试更小的 mid
                right = mid - 1;
            } else {
                // 当前引用次数太低，无法支持这么大的 h，需要向右移动
                left = mid + 1;
            }
        }
        return ans;
    }
};
void test() {
    Solution sol;
    
    // 用例 1: [0, 1, 3, 5, 6] -> n=5
    // mid=2, cit[2]=3, h=5-2=3. 3>=3 (True), ans=3, right=1
    // mid=0, cit[0]=0, h=5-0=5. 0>=5 (False), left=1
    // mid=1, cit[1]=1, h=5-1=4. 1>=4 (False), left=2
    // Result: 3
    vector<int> v1 = {0, 1, 3, 5, 6};
    cout << "Test 1: " << (sol.hIndex(v1) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
    
    // 用例 2: [10, 10, 10] -> n=3
    // mid=1, cit[1]=10, h=3-1=2. 10>=2 (True), ans=2, right=0
    // mid=0, cit[0]=10, h=3-0=3. 10>=3 (True), ans=3, right=-1
    // Result: 3
    vector<int> v2 = {10, 10, 10};
    cout << "Test 2: " << (sol.hIndex(v2) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
    
    // 用例 3: [0, 0, 0] -> n=3
    // Result: 0
    vector<int> v3 = {0, 0, 0};
    cout << "Test 3: " << (sol.hIndex(v3) == 0 ? "Pass" : "Fail") << " (Expected 0)" << endl;
}
} // namespace Question275

// --- 第 278 题 ---
namespace Question278
{
int first_bad_version_global = -1; 
bool isBadVersion(int version) {
    return version >= first_bad_version_global;
}
class Solution {
public:
    int firstBadVersion(int n) {
        int left = 1;
        int right = n;
        while (left < right) {
            // 重点 1：防止整数溢出
            // 不要写成 (left + right) / 2，因为 left + right 可能会超过 int 的最大值 (2^31 - 1)
            int mid = left + (right - left) / 2;
            if (isBadVersion(mid)) {
                // 如果 mid 是坏版本，说明第一个坏版本在 mid 或 mid 的左边
                // 所以我们将搜索范围缩小到 [left, mid]
                right = mid;
            } else {
                // 如果 mid 是好版本，说明第一个坏版本一定在 mid 的右边
                // 所以我们将搜索范围缩小到 [mid + 1, right]
                left = mid + 1;
            }
        }
        // 当 left == right 时，我们就找到了第一个坏版本
        return left;
    }
};
void test() {
    Solution sol;
    
    // 测试用例 1: n = 5, 第 4 个版本开始坏
    first_bad_version_global = 4;
    cout << "Test 1 (n=5, bad=4): " << (sol.firstBadVersion(5) == 4 ? "Pass" : "Fail") << endl;
    
    // 测试用例 2: n = 1, 第 1 个版本就是坏的
    first_bad_version_global = 1;
    cout << "Test 2 (n=1, bad=1): " << (sol.firstBadVersion(1) == 1 ? "Pass" : "Fail") << endl;
    
    // 测试用例 3: n = 10, 第 10 个版本才坏
    first_bad_version_global = 10;
    cout << "Test 3 (n=10, bad=10): " << (sol.firstBadVersion(10) == 10 ? "Pass" : "Fail") << endl;
}
} // namespace Question278

// --- 第 279 题 ---
namespace Question279
{
class Solution {
public:
    int numSquares(int n) {
        // dp[i] 表示组成整数 i 所需的最少完全平方数数量
        // 初始化为 n，因为最坏情况是全部由 1 组成 (1+1...+1)，数量为 n
        vector<int> dp(n + 1, n);
        
        // 基础情况
        dp[0] = 0;

        // 外层循环：计算从 1 到 n 的每一个 dp 值
        for (int i = 1; i <= n; ++i) {
            // 内层循环：尝试所有小于等于 i 的完全平方数 j*j
            for (int j = 1; j * j <= i; ++j) {
                // 状态转移：当前数量 = 减去一个平方数后的数量 + 1
                dp[i] = min(dp[i], dp[i - j * j] + 1);
            }
        }

        return dp[n];
    }
};
void test() {
    Solution sol;
    
    // 用例 1: n = 12 -> 4+4+4 (3)
    cout << "Test 12: " << (sol.numSquares(12) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
    
    // 用例 2: n = 13 -> 4+9 (2)
    cout << "Test 13: " << (sol.numSquares(13) == 2 ? "Pass" : "Fail") << " (Expected 2)" << endl;
    
    // 用例 3: n = 1 -> 1 (1)
    cout << "Test 1: " << (sol.numSquares(1) == 1 ? "Pass" : "Fail") << " (Expected 1)" << endl;
    
    // 用例 4: n = 4 -> 4 (1)
    cout << "Test 4: " << (sol.numSquares(4) == 1 ? "Pass" : "Fail") << " (Expected 1)" << endl;
}
} // namespace Question279

// --- 第 282 题 ---
namespace Question282
{
class Solution {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> result;
        if (num.empty()) return result;
        
        // path: 记录当前的表达式字符串
        // index: 当前处理到 num 的哪个位置
        // currentVal: 当前表达式计算出的值
        // prevOperand: 上一个操作数（用于处理乘法优先级）
        backtrack(result, num, target, "", 0, 0, 0);
        return result;
    }

private:
    void backtrack(vector<string>& result, const string& num, int target, 
                   string path, int index, long long currentVal, long long prevOperand) {
        
        // 递归终止条件：处理完所有数字
        if (index == num.length()) {
            if (currentVal == target) {
                result.push_back(path);
            }
            return;
        }

        // 尝试截取不同长度的数字
        for (int i = index; i < num.length(); ++i) {
            // 处理前导零：如果当前截取的数字是以 '0' 开头且长度大于 1，则非法
            if (i > index && num[index] == '0') break;

            // 将截取的字符串转为 long long
            string part = num.substr(index, i - index + 1);
            long long currNum = stoll(part);

            if (index == 0) {
                // 第一个数字，直接递归，不需要运算符
                backtrack(result, num, target, part, i + 1, currNum, currNum);
            } else {
                // 尝试三种运算符
                
                // 1. 加法
                backtrack(result, num, target, path + "+" + part, i + 1, 
                          currentVal + currNum, currNum);

                // 2. 减法
                backtrack(result, num, target, path + "-" + part, i + 1, 
                          currentVal - currNum, -currNum);

                // 3. 乘法 (核心：处理优先级)
                // 撤销上一步的操作，加上 (prev * curr)
                backtrack(result, num, target, path + "*" + part, i + 1, 
                          (currentVal - prevOperand) + (prevOperand * currNum), 
                          prevOperand * currNum);
            }
        }
    }
};
void test() {
    Solution sol;
    
    // 用例 1: num = "123", target = 6
    vector<string> res1 = sol.addOperators("123", 6);
    cout << "Test 1 (123, 6): ";
    for (auto s : res1) cout << "[" << s << "] "; 
    cout << endl; // 预期: [1+2+3] [1*2*3]

    // 用例 2: num = "232", target = 8
    vector<string> res2 = sol.addOperators("232", 8);
    cout << "Test 2 (232, 8): ";
    for (auto s : res2) cout << "[" << s << "] ";
    cout << endl; // 预期: [2*3+2]

    // 用例 3: num = "105", target = 5
    vector<string> res3 = sol.addOperators("105", 5);
    cout << "Test 3 (105, 5): ";
    for (auto s : res3) cout << "[" << s << "] ";
    cout << endl; // 预期: [1*0+5]
}
} // namespace Question282

// --- 第 283 题 ---
namespace Question283
{
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0; // 指向下一个非零元素应该存放的位置

        for (int fast = 0; fast < nums.size(); ++fast) {
            // 如果快指针找到了非零元素
            if (nums[fast] != 0) {
                // 将其与慢指针位置的元素交换
                // 如果 slow == fast，相当于自己跟自己交换，不影响结果
                swap(nums[slow], nums[fast]);
                // 慢指针后移，为下一个非零元素腾出位置
                slow++;
            }
        }
    }
};
void test() {
    Solution sol;

    // 用例 1: [0, 1, 0, 3, 12] -> [1, 3, 12, 0, 0]
    vector<int> v1 = {0, 1, 0, 3, 12};
    sol.moveZeroes(v1);
    cout << "Test 1: ";
    for (int x : v1) cout << x << " "; 
    cout << (v1[0] == 1 && v1[4] == 0 ? " (Pass)" : " (Fail)") << endl;

    // 用例 2: [0] -> [0]
    vector<int> v2 = {0};
    sol.moveZeroes(v2);
    cout << "Test 2: ";
    for (int x : v2) cout << x << " ";
    cout << (v2[0] == 0 ? " (Pass)" : " (Fail)") << endl;

    // 用例 3: [1, 2, 3] -> [1, 2, 3] (没有零的情况)
    vector<int> v3 = {1, 2, 3};
    sol.moveZeroes(v3);
    cout << "Test 3: ";
    for (int x : v3) cout << x << " ";
    cout << (v3[0] == 1 && v3[2] == 3 ? " (Pass)" : " (Fail)") << endl;
}
} // namespace Question283

// --- 第 284 题 ---
namespace Question284
{
class Iterator {
private:
    const vector<int>& nums;
    int cursor;
public:
    Iterator(const vector<int>& n) : nums(n), cursor(0) {}
    bool hasNext() const { return cursor < nums.size(); }
    int next() { return nums[cursor++]; }
};
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
void test() {
    vector<int> nums = {1, 2, 3, 4};
    PeekingIterator it(nums);

    cout << "Test peek: " << it.peek() << " (Expected 1)" << endl; 
    cout << "Test next: " << it.next() << " (Expected 1)" << endl;
    cout << "Test peek: " << it.peek() << " (Expected 2)" << endl;
    cout << "Test next: " << it.next() << " (Expected 2)" << endl;
    cout << "Test next: " << it.next() << " (Expected 3)" << endl;
    cout << "Test peek: " << it.peek() << " (Expected 4)" << endl;
    cout << "Test next: " << it.next() << " (Expected 4)" << endl;
    cout << "Test hasNext: " << (it.hasNext() ? "True" : "False") << " (Expected False)" << endl;
}
} // namespace Question284

// --- 第 287 题 ---
namespace Question287
{
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        // 1. 第一阶段：寻找快慢指针的相遇点
        int slow = nums[0];
        int fast = nums[0];
        
        // 必须先走一步，否则 while 条件 slow != fast 一开始就不成立
        do {
            slow = nums[slow];            // 慢指针走一步
            fast = nums[nums[fast]];     // 快指针走两步
        } while (slow != fast);
        
        // 2. 第二阶段：寻找环的入口点
        slow = nums[0];                  // 慢指针回到起点
        while (slow != fast) {
            slow = nums[slow];           // 两人同时走一步
            fast = nums[fast];
        }
        
        return slow; // 相遇点即为重复数字
    }
};
void test() {
    Solution sol;
    
    // 用例 1: [1, 3, 4, 2, 2] -> 重复 2
    vector<int> v1 = {1, 3, 4, 2, 2};
    cout << "Test 1: " << (sol.findDuplicate(v1) == 2 ? "Pass" : "Fail") << " (Expected 2)" << endl;
    
    // 用例 2: [3, 1, 3, 4, 2] -> 重复 3
    vector<int> v2 = {3, 1, 3, 4, 2};
    cout << "Test 2: " << (sol.findDuplicate(v2) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
    
    // 用例 3: [3, 3, 3, 3, 3] -> 重复 3
    vector<int> v3 = {3, 3, 3, 3, 3};
    cout << "Test 3: " << (sol.findDuplicate(v3) == 3 ? "Pass" : "Fail") << " (Expected 3)" << endl;
}
} // namespace Question287

// --- 第 289 题 ---
namespace Question289
{
class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int rows = board.size();
        int cols = board[0].size();

        // 8 个方向的偏移量，用于检查周围邻居
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // 第一遍遍历：计算状态并编码
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int liveNeighbors = 0;

                // 检查 8 个方向的邻居
                for (int k = 0; k < 8; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];

                    // 边界检查，且判断原状态是否为活 (1 或 2)
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && 
                        (board[ni][nj] == 1 || board[ni][nj] == 2)) {
                        liveNeighbors++;
                    }
                }

                // 根据规则更新状态 (编码)
                if (board[i][j] == 1) {
                    if (liveNeighbors < 2 || liveNeighbors > 3) {
                        board[i][j] = 2; // 活 -> 死
                    }
                } else if (board[i][j] == 0) {
                    if (liveNeighbors == 3) {
                        board[i][j] = 3; // 死 -> 活
                    }
                }
            }
        }

        // 第二遍遍历：将编码还原为最终状态 (0 或 1)
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (board[i][j] == 1 || board[i][j] == 3) {
                    board[i][j] = 1;
                } else {
                    board[i][j] = 0;
                }
            }
        }
    }
};
void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << "-----------------" << endl;
}
void test() {
    Solution sol;

    // 测试用例 1
    vector<vector<int>> board1 = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };
    sol.gameOfLife(board1);
    cout << "Test 1 Result:" << endl;
    printBoard(board1); 
    // 预期:
    // 0 1 0
    // 0 0 1
    // 0 1 1 (取决于具体规则应用)
}
} // namespace Question289

// --- 第 290 题 ---
namespace Question290
{
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        // 1. 将 s 分割成单词列表
        vector<string> words;
        stringstream ss(s);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }

        // 2. 数量不一致，直接不匹配
        if (pattern.length() != words.size()) {
            return false;
        }

        // 3. 使用两个哈希表实现双向映射（双射）
        unordered_map<char, string> charToWord;
        unordered_map<string, char> wordToChar;

        for (int i = 0; i < pattern.length(); ++i) {
            char c = pattern[i];
            string w = words[i];

            // 检查字符到单词的映射
            if (charToWord.count(c)) {
                if (charToWord[c] != w) return false;
            } else {
                charToWord[c] = w;
            }

            // 检查单词到字符的映射
            if (wordToChar.count(w)) {
                if (wordToChar[w] != c) return false;
            } else {
                wordToChar[w] = c;
            }
        }

        return true;
    }
};
void test() {
    Solution sol;
    
    // 用例 1: True
    cout << "Test 1 (abba, dog cat cat dog): " 
         << (sol.wordPattern("abba", "dog cat cat dog") ? "Pass" : "Fail") << endl;
    
    // 用例 2: False (a 映射到 dog, b 也映射到 dog)
    cout << "Test 2 (abba, dog dog dog dog): " 
         << (!sol.wordPattern("abba", "dog dog dog dog") ? "Pass" : "Fail") << endl;
    
    // 用例 3: False (a 映射到 dog, a 又映射到 cat)
    cout << "Test 3 (aaaa, dog cat cat dog): " 
         << (!sol.wordPattern("aaaa", "dog cat cat dog") ? "Pass" : "Fail") << endl;
    
    // 用例 4: False (长度不一致)
    cout << "Test 4 (abc, dog cat): " 
         << (!sol.wordPattern("abc", "dog cat") ? "Pass" : "Fail") << endl;
}
} // namespace Question290

// --- 第 292 题 ---
namespace Question292
{
class Solution {
public:
    bool canWinNim(int n) {
        return (n % 4 != 0);
    }
};
void test() {
    Solution sol;
    
    // 用例 1: n = 4 -> 输 (False)
    cout << "Test 4: " << (sol.canWinNim(4) == false ? "Pass" : "Fail") << " (Expected False)" << endl;
    
    // 用例 2: n = 1 -> 赢 (True)
    cout << "Test 1: " << (sol.canWinNim(1) == true ? "Pass" : "Fail") << " (Expected True)" << endl;
    
    // 用例 3: n = 2 -> 赢 (True)
    cout << "Test 2: " << (sol.canWinNim(2) == true ? "Pass" : "Fail") << " (Expected True)" << endl;
    
    // 用例 4: n = 8 -> 输 (False)
    cout << "Test 8: " << (sol.canWinNim(8) == false ? "Pass" : "Fail") << " (Expected False)" << endl;
}
} // namespace Question292

// --- 第 295 题 ---
namespace Question295
{
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
void test() {
    MedianFinder sol;
    sol.addNum(1);
    cout << "Add 1, Median: " << sol.findMedian() << endl; // 1.0
    
    sol.addNum(2);
    cout << "Add 2, Median: " << sol.findMedian() << endl; // 1.5
    
    sol.addNum(3);
    cout << "Add 3, Median: " << sol.findMedian() << endl; // 2.0
    
    sol.addNum(4);
    cout << "Add 4, Median: " << sol.findMedian() << endl; // 2.5
}
} // namespace Question295

// --- 第 297 题 ---
namespace Question297
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (!root) return "#"; // 用 # 表示空节点
        
        // 当前节点值 + 分隔符 + 左子树序列 + 分隔符 + 右子树序列
        return to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
        
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }
private:
    // 辅助函数：利用 stringstream 依次读取被逗号分隔的 token
    TreeNode* deserializeHelper(stringstream& ss) {
        string val;
        if (!getline(ss, val, ',')) {
            return nullptr;
        }

        if (val == "#") {
            return nullptr; // 遇到 # 说明是空节点
        }

        // 创建当前节点，并递归构造左子树和右子树
        TreeNode* root = new TreeNode(stoi(val));
        root->left = deserializeHelper(ss);
        root->right = deserializeHelper(ss);
        
        return root;
    }
};
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question297

// --- 第 299 题 ---
namespace Question299
{
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
void test() {
    Solution sol;
    
    // 测试用例 1: 1807, 7810
    // 8 是 Bull (1A), 1,0,7 是 Cow (3B)
    string s1 = "1807", g1 = "7810";
    cout << "Secret: 1807, Guess: 7810 -> Result: " << sol.getHint(s1, g1) << " (Expected: 1A3B)" << endl;
    
    // 测试用例 2: 1123, 0111
    // 一个 '1' 位置正确 (1A), 另一个 '1' 位置错误 (1B)
    string s2 = "1123", g2 = "0111";
    cout << "Secret: 1123, Guess: 0111 -> Result: " << sol.getHint(s2, g2) << " (Expected: 1A1B)" << endl;
}
} // namespace Question299

// --- 第 300 题 ---
namespace Question300
{
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;

        // tails 数组用于存储当前所有递增子序列的最小末尾元素
        // tails[i] 是长度为 i+1 的子序列的最小结尾
        vector<int> tails;

        for (int x : nums) {
            // 使用二分查找找到 tails 中第一个 >= x 的元素位置
            // lower_bound 返回的是指向该元素的迭代器
            auto it = lower_bound(tails.begin(), tails.end(), x);

            if (it == tails.end()) {
                // 如果 x 比 tails 所有的元素都大，直接添加到末尾
                tails.push_back(x);
            } else {
                // 否则，用 x 替换掉第一个 >= x 的元素
                // 这样可以使得长度为 (dist+1) 的子序列末尾更小
                *it = x;
            }
        }

        // tails 的最终长度即为最长递增子序列的长度
        return tails.size();
    }
};
void test() {
    Solution sol;
    
    // 用例 1: [10, 9, 2, 5, 3, 7, 101, 18] -> LIS: [2, 3, 7, 18] 或 [2, 5, 7, 18]
    vector<int> v1 = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Test 1: " << (sol.lengthOfLIS(v1) == 4 ? "Pass" : "Fail") << " (Expected 4)" << endl;
    
    // 用例 2: [0, 1, 0, 3, 2, 3] -> LIS: [0, 1, 2, 3]
    vector<int> v2 = {0, 1, 0, 3, 2, 3};
    cout << "Test 2: " << (sol.lengthOfLIS(v2) == 4 ? "Pass" : "Fail") << " (Expected 4)" << endl;
    
    // 用例 3: [7, 7, 7, 7] -> LIS: [7] (严格递增)
    vector<int> v3 = {7, 7, 7, 7};
    cout << "Test 3: " << (sol.lengthOfLIS(v3) == 1 ? "Pass" : "Fail") << " (Expected 1)" << endl;
    
    // 用例 4: [1, 2, 3, 4, 5] -> LIS: [1, 2, 3, 4, 5]
    vector<int> v4 = {1, 2, 3, 4, 5};
    cout << "Test 4: " << (sol.lengthOfLIS(v4) == 5 ? "Pass" : "Fail") << " (Expected 5)" << endl;
}
} // namespace Question300

// --- 第 301 题 ---
namespace Question301
{
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> result;
        if (s.empty()) return result;

        queue<string> q;
        unordered_set<string> visited;

        q.push(s);
        visited.insert(s);

        bool found = false; // 标记是否在当前层找到了有效字符串

        while (!q.empty()) {
            int levelSize = q.size();
            
            // 遍历当前层的所有可能状态
            for (int i = 0; i < levelSize; ++i) {
                string curr = q.front();
                q.pop();

                if (isValid(curr)) {
                    result.push_back(curr);
                    found = true; // 标记找到了，后续层级不再需要探索
                }

                // 如果本层已经找到了有效字符串，则不需要再生成下一层（删除更多字符）
                if (found) continue;

                // 生成下一层状态：尝试删除每一个括号
                for (int j = 0; j < curr.length(); ++j) {
                    if (curr[j] != '(' && curr[j] != ')') continue;

                    // 构建新字符串：删除索引 j 处的字符
                    string next = curr.substr(0, j) + curr.substr(j + 1);
                    if (visited.find(next) == visited.end()) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }

            // 如果在当前层找到了一个或多个有效字符串，直接退出循环
            if (found) break;
        }

        return result;
    }
    // 辅助函数：检查字符串是否为有效括号组合
    bool isValid(string s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') {
                count--;
                if (count < 0) return false; // 右括号比左括号多
            }
        }
        return count == 0;
    }
};
void test() {
    Solution sol;
    
    // 测试用例 1
    string s1 = "()())()";
    vector<string> res1 = sol.removeInvalidParentheses(s1);
    cout << "Input: \"()())()\" -> Output: ";
    for (auto& str : res1) cout << "\"" << str << "\" "; 
    cout << "\n(Expected: [\"(())()\", \"()()()\"])" << endl;

    // 测试用例 2
    string s2 = "(a)())()";
    vector<string> res2 = sol.removeInvalidParentheses(s2);
    cout << "Input: \"(a)())()\" -> Output: ";
    for (auto& str : res2) cout << "\"" << str << "\" "; 
    cout << "\n(Expected: [\"(a())()\", \"(a)()()\"])" << endl;

    // 测试用例 3
    string s3 = ")(";
    vector<string> res3 = sol.removeInvalidParentheses(s3);
    cout << "Input: \")(\" -> Output: ";
    for (auto& str : res3) cout << "\"" << str << "\" "; 
    cout << "\n(Expected: [\"\"])" << endl;
}
} // namespace Question301


int main() {
    Question301::test();
    return 0;
}

// --- 第 0 题 ---
namespace Question0
{
void test() {
    // Solution sol;
    // cout << sol.romanToInt("XXVILCDM") << endl;
}
} // namespace Question0

