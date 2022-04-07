#pragma once

// default
#include <string>
#include <string_view>

// conan
#include "tl/expected.hpp"

// local
#include "models.hpp"
#include "storage.hpp"


enum class account_error
{
    wrong_password,
    unknown_user
};

tl::expected<user_info, account_error>
get_user_info(int64_t user_id);

tl::expected<int64_t, account_error>
login(const std::string& username, const std::string& password);

tl::expected<user_info, account_error>
auth(const std::string& username, const std::string& password);

