#pragma once

// conan
#include "sqlite_orm/sqlite_orm.h"

// local
#include "models.hpp"

namespace orm = sqlite_orm;


static auto storage = orm::make_storage("/home/alionapermes/tmp.sqlite",
    orm::make_table("users",
        orm::make_column("id",       &user::id),
        orm::make_column("name",     &user::name),
        orm::make_column("password", &user::password)
    ),
    orm::make_table("user_info",
        orm::make_column("user_id",   &user_info::user_id),
        orm::make_column("created",   &user_info::created),
        orm::make_column("last_seen", &user_info::last_seen),
        orm::make_column("rating",    &user_info::rating)
    ),
    orm::make_table("games",
        orm::make_column("id",          &game::id),
        orm::make_column("min_members", &game::min_members),
        orm::make_column("max_members", &game::max_members),
        orm::make_column("name",        &game::name),
        orm::make_column("description", &game::desc)
    ),
    orm::make_table("rooms",
        orm::make_column("game_id",    &room::game_id),
        orm::make_column("owner_id",   &room::owner_id),
        orm::make_column("member_ids", &room::member_ids)
    ),
    orm::make_table("sessions",
        orm::make_column("game_id",    &session::game_id),
        orm::make_column("time_start", &session::time_start),
        orm::make_column("time_end",   &session::time_end)
    )
);

