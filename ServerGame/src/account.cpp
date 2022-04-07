#include "account.hpp"


tl::expected<user_info, account_error>
get_user_info(int64_t user_id)
{
    using namespace sqlite_orm;

    auto users_info = storage.get_all<user_info>(
        where(c(&user_info::user_id) == user_id)
    );
    if (users_info.empty()) {
        return tl::unexpected(account_error::unknown_user);
    }

    user_info& info = users_info[0];

    return info;
}

tl::expected<int64_t, account_error>
login(const std::string& username, const std::string& password)
{
    using namespace sqlite_orm;

    auto users = storage.get_all<user>(where(c(&user::name) == username));
    if (users.empty()) {
        return tl::unexpected(account_error::unknown_user);
    }

    user& user = users[0];
    if (user.password != password) {
        return tl::unexpected(account_error::wrong_password);
    }

    return user.id;
}

tl::expected<user_info, account_error>
auth(const std::string& username, const std::string& password)
{
    return login(username, password)
        .and_then(get_user_info);
}

