// 题目：单向链表。练习约定：int 节点，独占所有权，不支持复制。
// 头尾插入 O(1)，删除指定值/反转 O(n)，空间 O(n)。
// 运行：g++ -std=c++17 linked_list.cc -o list && ./list
#include <iostream>
#include <memory>
#include <utility>
class LinkedList {
    struct Node { int value; std::unique_ptr<Node> next; explicit Node(int v):value(v){} };
    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
public:
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;
    ~LinkedList() { clear(); }
    void push_front(int value) {
        auto node = std::make_unique<Node>(value);
        if (!head_) tail_ = node.get();
        node->next = std::move(head_); head_ = std::move(node); ++size_;
    }
    void push_back(int value) {
        auto node = std::make_unique<Node>(value); Node* raw = node.get();
        if (tail_) tail_->next = std::move(node); else head_ = std::move(node);
        tail_ = raw; ++size_;
    }
    bool erase_first(int value) {
        auto* link = &head_; Node* previous = nullptr;
        while (*link && (*link)->value != value) { previous = link->get(); link = &((*link)->next); }
        if (!*link) return false;
        auto removed = std::move(*link); *link = std::move(removed->next);
        if (!*link) tail_ = previous;
        --size_; return true;
    }
    void reverse() {
        tail_ = head_.get(); std::unique_ptr<Node> previous;
        while (head_) {
            auto next = std::move(head_->next); head_->next = std::move(previous);
            previous = std::move(head_); head_ = std::move(next);
        }
        head_ = std::move(previous);
    }
    void clear() {
        // 迭代释放，避免超长链表递归析构耗尽调用栈。
        while (head_) { auto node = std::move(head_); head_ = std::move(node->next); }
        tail_ = nullptr; size_ = 0;
    }
    std::size_t size() const { return size_; }
    void print() const {
        for (auto* p=head_.get(); p; p=p->next.get()) std::cout << p->value << ' ';
        std::cout << '\n';
    }
};
int main() {
    LinkedList list; list.push_back(1); list.push_back(2); list.push_front(0);
    list.erase_first(1); list.reverse(); list.print();
}
