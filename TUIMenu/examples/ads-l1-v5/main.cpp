#include "app.hpp"


int
main()
{
    App app("ЛР №1, В-5. Лёвкин В., Жамсаранова А.");

    app.add_commands({
        {"Создать контейнер",                   app.new_unit_empty   },
        {"Создать контейнер на основе другого", app.new_unit_based   },
        {"Вывод элементов контейнера",          app.read_items       },
        {"Количество элементов в контейнере",   app.items_count      },
        {"Проверка контейнера на пустоту",      app.unit_is_empty    },
        {"Очистка контейнера",                  app.unit_clear       },
        {"Добавление в конец контейнера",       app.push_back        },
        {"Проверка наличия элемента (поиск)",   app.contains         },
        {"Удаление контейнера",                 app.unit_remove      },
    });

    app.run();

    return 0;
}

