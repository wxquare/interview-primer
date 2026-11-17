// 题目：抽象工厂模式。练习约定：工厂同时创建按钮、复选框，保持同一 UI 产品族。
// 区别于简单工厂：抽象接口提供一组相关产品的创建方法。创建 O(1)，空间 O(1)。
// 运行：g++ -std=c++17 factory_abstract.cc -o factory && ./factory
#include <iostream>
#include <memory>
struct Button { virtual ~Button()=default; virtual void draw() const=0; };
struct Checkbox { virtual ~Checkbox()=default; virtual void draw() const=0; };
struct WindowsButton:Button { void draw() const override { std::cout<<"Windows button\n"; } };
struct WindowsCheckbox:Checkbox { void draw() const override { std::cout<<"Windows checkbox\n"; } };
struct MacButton:Button { void draw() const override { std::cout<<"Mac button\n"; } };
struct MacCheckbox:Checkbox { void draw() const override { std::cout<<"Mac checkbox\n"; } };
struct UIFactory {
    virtual ~UIFactory()=default;
    virtual std::unique_ptr<Button> button() const=0;
    virtual std::unique_ptr<Checkbox> checkbox() const=0;
};
struct WindowsFactory:UIFactory {
    std::unique_ptr<Button> button() const override { return std::make_unique<WindowsButton>(); }
    std::unique_ptr<Checkbox> checkbox() const override { return std::make_unique<WindowsCheckbox>(); }
};
struct MacFactory:UIFactory {
    std::unique_ptr<Button> button() const override { return std::make_unique<MacButton>(); }
    std::unique_ptr<Checkbox> checkbox() const override { return std::make_unique<MacCheckbox>(); }
};
void render(const UIFactory& factory) {
    auto button=factory.button(); auto checkbox=factory.checkbox(); button->draw(); checkbox->draw();
}
int main() { WindowsFactory windows; MacFactory mac; render(windows); render(mac); }
