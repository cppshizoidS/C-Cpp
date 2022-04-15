#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <memory>
#include <utility>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "types.hpp"
#include "validators.hpp"
#include "windowbase.hpp"


namespace tuim {

using namespace ftxui;


class InputWindow : public WindowBase
{
private: // fields
    bool _correct = true;
    std::string _info;
    std::string _content;
    std::string _placeholder;
    Component _input_field;

public: // handlers
    Validator validator;
    std::function<void()> on_ok;
    std::function<void()> on_cancel;

public: // ctors
    InputWindow(string_like auto&& title = "")
        : WindowBase(std::forward<decltype(title)>(title))
    { validator = wild_text(); }

public: // methods
    bool
    correct() const
    { return _correct && !_content.empty(); }

    const std::string&
    content() const
    { return _content; }

    const std::string&
    get_info() const
    { return _info; }

    void
    set_info(string_like auto&& text)
    { _info = std::forward<decltype(text)>(text); }

    const std::string&
    get_placeholder() const
    { return _placeholder; }

    void
    set_placeholder(string_like auto&& text)
    { _placeholder = std::forward<decltype(text)>(text); }

    void
    clear_info()
    { _info.clear(); }

    void
    clear_content()
    { _content.clear(); }

    void
    clear_placeholder()
    { _placeholder.clear(); }

    void
    reset()
    {
        clear_info();
        clear_content();
        focus_input();
    }

    void
    hard_reset()
    {
        validator = wild_text();
        _title.clear();
        clear_placeholder();
        reset();
    }

    void
    focus_input()
    { _input_field->TakeFocus(); }

private: // methods
    std::function<void()> check_content = [&] {
        _correct = validator(_content);
    };

    Component
    renderer() override
    {
        std::function<void()> ok_handler = [this] {
            if (_correct)
                on_ok();
        };

        auto input_opt      = InputOption();
        input_opt.on_enter  = ok_handler;
        input_opt.on_change = check_content;

        Component button_ok     = Button("OK", ok_handler);
        Component button_cancel = Button("Отмена", on_cancel);
        _input_field            = Input(
            &_content,
            &_placeholder,
            std::move(input_opt)
        );

        Component container = Container::Vertical({
            _input_field,
            Container::Horizontal({
                button_ok,
                button_cancel,
            }),
        });
        Component renderer  = Renderer(container,
            [this, button_ok, button_cancel] {
                Element input_element = _input_field->Render()
                    | border
                    | color(correct() ? Color::Green : Color::Red);

                return window(text(_title) | center,
                    vbox({
                        text(_info),
                        input_element,
                        hbox({
                            button_ok->Render() | flex,
                            button_cancel->Render() | flex,
                        }),
                    })
                );
            }
        );

        return renderer;
    }
};


}

