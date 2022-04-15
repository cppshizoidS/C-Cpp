#include "app.hpp"


int
main()
{
    App app("ЛР №1, В-3. Баранова В., Обидиентов А.");

    app.add_commands({
        /* {"New container",                   app.new_unit_empty   }, */
        /* {"New with capacity", app.new_unit_reserved}, */
        /* {"New based", app.new_unit_based   }, */
        /* {"Read by index",         app.read_by_index    }, */
        /* {"Write by index",         app.write_by_index   }, */
        /* {"Output",          app.read_items       }, */
        /* {"Size",   app.items_count      }, */
        /* {"Capacity",                    app.unit_capacity    }, */
        /* {"Is empty",      app.unit_is_empty    }, */
        /* {"Clear",                  app.unit_clear       }, */
        /* {"Push back",       app.push_back        }, */
        /* {"Find (exists)",   app.contains         }, */
        /* {"Insert before",              app.insert_before    }, */
        /* {"Erase by index",                 app.erase            }, */
        /* {"Navigate",             app.navigate         }, */
        /* {"Remove container",                 app.unit_remove      }, */
        {"Создать контейнер",                   app.new_unit_empty   },
        {"Создать контейнер с резервированием", app.new_unit_reserved},
        {"Создать контейнер на основе другого", app.new_unit_based   },
        {"Получить элемент по индексу",         app.read_by_index    },
        {"Записать элемент по индексу",         app.write_by_index   },
        {"Вывод элементов контейнера",          app.read_items       },
        {"Количество элементов в контейнере",   app.items_count      },
        {"Объём контейнера",                    app.unit_capacity    },
        {"Проверка контейнера на пустоту",      app.unit_is_empty    },
        {"Очистка контейнера",                  app.unit_clear       },
        {"Добавление в конец контейнера",       app.push_back        },
        {"Проверка наличия элемента (поиск)",   app.contains         },
        {"Вставка перед индексом",              app.insert_before    },
        {"Удаление по индексу",                 app.erase            },
        {"Навигация по контейнеру",             app.navigate         },
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

