/*
 * @lc app=leetcode id=68 lang=cpp
 *
 * [68] Text Justification
 */

// @lc code=start
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
// @lc code=end

