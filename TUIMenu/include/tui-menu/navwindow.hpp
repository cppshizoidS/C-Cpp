#pragma once

#include <functional>
#include <string>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

#include "types.hpp"
#include "windowbase.hpp"


namespace tuim {

using namespace ftxui;


template <typename T>
class NavWindow : public WindowBase
{
private: // properties
    std::string _info;
    std::string _content;
    Component _button_prev;
    Component _button_next;
    Component _button_exit;
    typename T::iterator _pos;
    T* _container = nullptr;

public:
    std::function<void()> on_exit;

public: // ctors
    NavWindow(string_like auto&& title = "")
        : WindowBase(std::forward<decltype(title)>(title))
    {}

public: // methods
    const std::string&
    get_info() const
    { return _info; }

    void
    set_info(string_like auto&& text)
    { _info = std::forward<decltype(text)>(text); }

    const std::string&
    get_content() const
    { return _content; }

    void
    set_content(string_like auto&& text)
    { _content = std::forward<decltype(text)>(text); }

    void
    reset()
    {
        _info.clear();
        _content.clear();
    }

    void
    set_container(T* container)
    {
        _container = container;
        _pos       = _container->begin();
    }

private:
    Component
    renderer() override
    {
        _button_prev = Button("Назад", [this] {
            if (_pos != _container->begin())
                --_pos;
        });
        _button_next = Button("Вперёд", [this] {
            if (_pos != --_container->end())
                ++_pos;
        });
        _button_exit = Button("Закрыть", on_exit);

        Component container = Container::Vertical({
            Container::Horizontal({
                _button_prev,
                _button_next,
            }),
            _button_exit
        });
        Component renderer  = Renderer(container, [this] {
            Element b_prev = _button_prev->Render() | color(
                _pos == _container->begin()
                ? Color::Red
                : Color::Green
            );
            Element b_next = _button_next->Render() | color(
                _pos == --_container->end()
                ? Color::Red
                : Color::Green
            );

            return window(text(_title) | center,
                vbox({
                    text(_info),
                    separator(),
                    hbox({
                        b_prev,
                        text(std::to_string(*_pos))
                            | center
                            | border
                            | flex
                            | size(WIDTH, GREATER_THAN, 30),
                        b_next,
                    }),
                    _button_exit->Render() | flex,
                })
            );
        });

        return renderer;
    }
};


}

