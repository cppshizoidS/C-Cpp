// conan
#include "gtest/gtest.h"
#include "tl/expected.hpp"

// local
#include "test_data.hpp"
#include "src/storage.hpp"
#include "src/models.hpp"
#include "src/account.hpp"


TEST(login, account)
{
    auto res = login(me.name, me.password);
    ASSERT_TRUE(res);

    int64_t user_id = *res;
    ASSERT_EQ(user_id, me.id);
}

TEST(get_user_info, account)
{
    auto res = get_user_info(me.id);
    ASSERT_TRUE(res);

    user_info info = *res;
    ASSERT_EQ(info.user_id, me_info.user_id);
    ASSERT_EQ(info.rating,  me_info.rating);
}

TEST(auth, account)
{
    auto res = auth(me.name, me.password);
    ASSERT_TRUE(res);

    user_info info = *res;
    ASSERT_EQ(info.user_id, me_info.user_id);
    ASSERT_EQ(info.rating,  me_info.rating);
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

