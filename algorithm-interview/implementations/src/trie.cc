// 题目：前缀树。练习约定：仅 a-z，允许空字符串；非法输入抛异常。
// 插入/查询 O(L)，空间 O(不同前缀数)；unique_ptr 负责释放节点。
// 运行：g++ -std=c++17 trie.cc -o trie && ./trie
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
class Trie {
    struct Node {
        std::array<std::unique_ptr<Node>,26> children{};
        bool terminal = false;
    };
    Node root_;
    static void validate(const std::string& word) {
        for (unsigned char c : word)
            if (c < 'a' || c > 'z') throw std::invalid_argument("expected a-z");
    }
    const Node* walk(const std::string& word) const {
        validate(word); const Node* node = &root_;
        for (char c : word) {
            node = node->children[c-'a'].get();
            if (!node) return nullptr;
        }
        return node;
    }
public:
    void insert(const std::string& word) {
        validate(word); Node* node = &root_;
        for (char c : word) {
            auto& child = node->children[c-'a'];
            if (!child) child = std::make_unique<Node>();
            node = child.get();
        }
        node->terminal = true;
    }
    bool search(const std::string& word) const {
        const Node* node = walk(word); return node && node->terminal;
    }
    bool startsWith(const std::string& prefix) const { return walk(prefix) != nullptr; }
};
int main() {
    Trie trie; trie.insert("apple"); trie.insert("app");
    std::cout << std::boolalpha << trie.search("app") << ' ' << trie.startsWith("ap") << '\n';
}
