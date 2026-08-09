/*
 * @lc app=leetcode id=211 lang=cpp
 *
 * [211] Design Add and Search Words Data Structure
 */

// @lc code=start

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

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */
// @lc code=end

