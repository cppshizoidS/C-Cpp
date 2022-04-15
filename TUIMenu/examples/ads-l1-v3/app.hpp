#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "fmt/core.h"

#include "tui-menu/tui.hpp"
#include "tui-menu/inputwindow.hpp"
#include "tui-menu/mainwindow.hpp"
#include "tui-menu/navwindow.hpp"
#include "tui-menu/types.hpp"
#include "tui-menu/validators.hpp"

#include "container.hpp"


class App
{
private: // aliases
    using container_type = list_V3<int>;
    using ItemsList      = tuim::MainWindow<container_type>::ItemsList;

private: // fields
    tuim::TerminalUserInterface _tui;
    tuim::MainWindow<container_type> _main_window;
    tuim::NavWindow<container_type> _nav_window;
    tuim::InputWindow _input_window;
    lid_t _mwd, _iwd, _nwd;

public: // ctors
    App(tuim::string_like auto&& title)
        : _main_window(std::forward<decltype(title)>(title))
        , _nav_window("Навигация")
        , _input_window("Ввод") {}

public: // methods
    void
    run()
    {
        _mwd = _tui.add_layer(&_main_window, true);
        _iwd = _tui.add_layer(&_input_window);
        _nwd = _tui.add_layer(&_nav_window);

        _input_window.on_cancel = [this] { _tui.set_layer(_mwd); };
        _nav_window.on_exit     = [this] { _tui.set_layer(_mwd); };

        _tui.render();
    }

    void
    add_commands(ItemsList&& commands)
    { _main_window.add_commands(std::move(commands)); }

    std::function<void()>
    new_unit_empty = [this]
    {
        _input_window.reset();
        _input_window.set_title("Новый контейнер");
        _input_window.set_placeholder("название");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok     = [this] {
            _main_window.add_unit(_input_window.content());
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    new_unit_reserved = [this]
    {
        using namespace tuim;

        _input_window.reset();
        _input_window.set_title("Новый контейнер. Объём");
        _input_window.set_placeholder("объём");
        _input_window.validator = is_integer() | positive() | not_null();
        _input_window.on_ok     = [this] {
            int capacity = std::stoi(_input_window.content());

            _input_window.reset();
            _input_window.set_title("Новый контейнер. Название");
            _input_window.set_info("Объём: " + std::to_string(capacity));
            _input_window.set_placeholder("имя контейнера");
            _input_window.validator = wild_text();

            _input_window.on_ok = [this, capacity] {
                _main_window.add_unit(_input_window.content(), capacity);
                _tui.set_layer(_mwd);
            };
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    new_unit_based = [this]
    {
        _input_window.reset();
        _input_window.set_title(_main_window.selected_command());
        _input_window.set_placeholder("название");
        _input_window.on_ok = [this] {
            auto unit = _main_window.get_unit(_input_window.content());

            if (!unit) {
                _input_window.set_info("Такого контейнера нет");
            } else {
                _input_window.reset();
                _input_window.set_title("Новый контейнер");
                _input_window.on_ok = [this, unit] {
                    _main_window.add_unit(_input_window.content(), *unit);
                    _tui.set_layer(_mwd);
                };
            }
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    read_by_index = [this]
    {
        try {
            by_index_setup("Чтение по индексу");
        } catch (const std::runtime_error& re) {
            return _main_window.set_info("Ошибка: " + std::string(re.what()));
        }

        _input_window.on_ok = [this] {
            int index = std::stoi(_input_window.content());
            int value = _main_window.current_unit()[index];

            _main_window.set_info(
                "[" + std::to_string(index) + "]: " + std::to_string(value)
            );
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    write_by_index = [this]
    {
        try {
            by_index_setup("Запись по индексу");
        } catch (const std::runtime_error& re) {
            return _main_window.set_info("Ошибка: " + std::string(re.what()));
        }

        _input_window.on_ok = [this] {
            int index = std::stoi(_input_window.content());
            
            _input_window.reset();
            _input_window.set_placeholder("число");
            _input_window.validator = tuim::is_integer();
            _input_window.on_ok     = [this, index] {
                int value = std::stoi(_input_window.content());

                _main_window.set_info(fmt::format(
                    "Индекс {} записан значением {}", index, value
                ));
                _main_window.current_unit()[index] = value;
                _tui.set_layer(_mwd);
            };
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    read_items = [this]
    {
        try {
            std::string str = "{ ";
            for (const auto& item : _main_window.current_unit())
                str += fmt::format("{}, ", item);
            str += "}";

            _main_window.set_info(str);
        } catch (const std::runtime_error& re) {
            _main_window.set_info("Ошибка: " + std::string(re.what()));
        }
    };

    std::function<void()>
    items_count = [this]
    {
        std::string message;

        try {
            size_t count = _main_window.current_unit().size();
            message      = "Количество элементов: " + std::to_string(count);
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
    };

    std::function<void()>
    unit_capacity = [this]
    {
        std::string message;

        try {
            size_t cap = _main_window.current_unit().size();
            message    = "Объём контейнера: " + std::to_string(cap);
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
    };

    std::function<void()>
    unit_is_empty = [this]
    {
        std::string message = "Контейнер ";

        try {
            message += (
                _main_window.current_unit().empty()
                ? "пуст"
                : "не пуст"
            );
        } catch (const std::runtime_error& re) {
            message = "Ошибка: " + std::string(re.what());
        }

        _main_window.set_info(std::move(message));
    };

    std::function<void()>
    unit_clear = [this]
    {
        try {
            _main_window.current_unit().clear();
            _main_window.set_info("Контейнер очищен");
        } catch (const std::runtime_error& re) {
            _main_window.set_info("Ошибка: " + std::string(re.what()));
        }
    };

    std::function<void()>
    push_back = [this]
    {
        _input_window.reset();
        _input_window.set_title("Вставка в конец");
        _input_window.set_placeholder("число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int num = std::stoi(_input_window.content());

            try {
                _main_window.current_unit().push_back(num);
                _main_window.set_info("Элемент вставлен");
            } catch (const std::runtime_error& re) {
                _main_window.set_info("Ошибка: " + std::string(re.what()));
            }

            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    /* std::function<void()> */
    /* push_front = [this] */
    /* { */
    /*     _input_window.reset(); */
    /*     _input_window.set_title("Вставка в конец"); */
    /*     _input_window.set_placeholder("число"); */
    /*     _input_window.validator = tuim::is_integer(); */
    /*     _input_window.on_ok     = [this] { */
    /*         int num = std::stoi(_input_window.content()); */

    /*         try { */
    /*             _main_window.current_unit().push_front(num); */
    /*             _main_window.set_info("Элемент вставлен"); */
    /*         } catch (const std::runtime_error& re) { */
    /*             _main_window.set_info("Ошибка: " + std::string(re.what())); */
    /*         } */

    /*         _tui.set_layer(_mwd); */
    /*     }; */

    /*     _tui.set_layer(_iwd); */
    /* }; */

    std::function<void()>
    contains = [this]
    {
        if (_main_window.units_empty())
            return _main_window.set_info("Ошибка: нет контейнера");

        _input_window.reset();
        _input_window.set_title("Поиск элемента");
        _input_window.set_placeholder("целое число");
        _input_window.validator = tuim::is_integer();
        _input_window.on_ok     = [this] {
            int item             = std::stoi(_input_window.content());
            std::string message  = "Элемент " + std::to_string(item);
            container_type& unit = _main_window.current_unit();

            try {
                message += (
                    std::find(unit.begin(), unit.end(), item) == unit.end()
                    /* unit.find(item) == unit.end() */
                    ? " не найден"
                    : " содержится в контейнере"
                );
            } catch (const std::runtime_error& re) {
                message = "Ошибка: " + std::string(re.what());
            }

            _main_window.set_info(std::move(message));
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    insert_before = [this]
    {
        try {
            by_index_setup("Вставка перед индексом");
        } catch (const std::runtime_error& re) {
            return _main_window.set_info("Ошибка: " + std::string(re.what()));
        }

        _input_window.on_ok = [this] {
            int index = std::stoi(_input_window.content());
            
            _input_window.reset();
            _input_window.set_placeholder("число");
            _input_window.validator = tuim::is_integer();
            _input_window.on_ok     = [this, index] {
                int value = std::stoi(_input_window.content());

                _main_window.set_info(fmt::format(
                    "Элемент {} вставлен перед индексом {}", value, index
                ));
                
                auto pos = _main_window.current_unit().begin();
                for (int i = 0; i < index; i++)
                    pos++;
                _main_window.current_unit().insert(pos, value);
                _tui.set_layer(_mwd); // ??
            };
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    erase = [this]
    {
        try {
            by_index_setup("Удаление по индексу");
        } catch (const std::runtime_error& re) {
            return _main_window.set_info("Ошибка: " + std::string(re.what()));
        }

        _input_window.on_ok = [this] {
            int index = std::stoi(_input_window.content());
            _main_window.set_info(fmt::format(
                "Удалён элемент по индексу {}", index
            ));
            
            auto pos = _main_window.current_unit().begin();
            for (int i = 0; i < index; i++)
                pos++;
            _main_window.current_unit().erase(pos);
            _tui.set_layer(_mwd); // ??
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    unit_remove = [this]
    {
        _input_window.reset();
        _input_window.set_title("Удаление контейнера");
        _input_window.set_placeholder("название");
        _input_window.validator = tuim::wild_text();
        _input_window.on_ok     = [this] {
            const std::string& name = _input_window.content();
            std::string message     = "Контейнер " + name;

            message += (
                _main_window.remove_unit(name)
                ? " удалён"
                : " не найден"
            );
            
            _main_window.set_info(std::move(message));
            _tui.set_layer(_mwd);
        };

        _tui.set_layer(_iwd);
    };

    std::function<void()>
    navigate = [this]
    {
        if (_main_window.current_unit().empty())
            return _main_window.set_info("Контейнер пуст");

        _nav_window.reset();
        _nav_window.set_container(&_main_window.current_unit());

        _tui.set_layer(_nwd);
    };

private:
    void
    by_index_setup(tuim::string_like auto&& title)
    {
        using namespace tuim;
        container_type* unit = &_main_window.current_unit();

        _input_window.reset();
        _input_window.set_title(std::forward<decltype(title)>(title));
        _input_window.set_placeholder(fmt::format(
            "индекс [0; {})", unit->size()
        ));
        _input_window.validator = is_integer() | in_range(0, unit->size() - 1);
    };

    void
    by_name_setup(
        tuim::string_like auto&& title,
        tuim::string_like auto&& placeholder
    ) {
        using namespace tuim;
        container_type* unit = &_main_window.current_unit();

        _input_window.hard_reset();
        _input_window.set_title(std::forward<decltype(title)>(title));
        _input_window.set_placeholder(
            std::forward<decltype(placeholder)>(placeholder)
        );
    };
};

