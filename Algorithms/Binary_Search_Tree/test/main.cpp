#include "gtest/gtest.h"

/* #define TLR */
/* #define LTR */
#define LRT
/* #define INDEX_OF */
/* #define RECURSIVE */
#define ITERATIVE
#define COUNT_MORE_THAN
#define BALANCE_FACTOR

#include "bst_impl.hpp"


TEST(ctor, copy)
{
    bst<int> t1;

    t1.insert(2);
    t1.insert(1);
    t1.insert(4);
    t1.insert(3);

    auto t2 = t1;
    ASSERT_EQ(t1.size(), t2.size());

    for (
        auto it1 = t1.begin(), it2 = t2.begin();
        it2 != t2.end();
        ++it1, ++it2
    ) ASSERT_EQ(*it1, *it2);
}

TEST(method, insert)
{
    bst<int> tree;

    ASSERT_EQ(*tree.insert(2), 2);
    ASSERT_EQ(*tree.insert(1), 1);
    ASSERT_EQ(*tree.insert(3), 3);

    ASSERT_EQ(tree.size(),  3);
#ifdef OPTIONAL
    ASSERT_EQ(tree.front(), 1);
    ASSERT_EQ(tree.back(),  3);
#endif
}

TEST(iterator, forward)
{
    bst<int> tree;

    ASSERT_EQ(*tree.insert(2), 2);
    ASSERT_EQ(*tree.insert(1), 1);
    ASSERT_EQ(*tree.insert(3), 3);

    int n = 0;
    for (const auto& item : tree)
        ASSERT_EQ(item, ++n);
    ASSERT_EQ(n, tree.size());

    for (auto iter = tree.rbegin(); iter != tree.rend(); ++iter)
        ASSERT_EQ(*iter, n--);
    ASSERT_EQ(n, 0);
}

TEST(method, find)
{
    bst<int> tree;
    for (int n = 0; n < 10; n++) {
        tree.insert(n);
        ASSERT_EQ(*tree.find(n), n);
    }
}

TEST(method, erase)
{
    {
        bst<int> tree;
        tree.insert(5);
        tree.insert(4);
        tree.insert(6);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(8);
        tree.insert(1);
        tree.insert(9);
        int old_size = tree.size();

        tree.erase(tree.find(3));
        ASSERT_EQ(old_size, tree.size() + 1);

        int n = 1;
        for (const auto& item : tree) {
            ASSERT_EQ(n++, item);
            if (n == 3)
                n++;
        }
    }
    {
        bst<int> tree;
    
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(4);
        tree.insert(6);
        tree.insert(8);
        tree.insert(1);
        tree.insert(9);
        int old_size = tree.size();

        tree.erase(tree.find(3));
        ASSERT_EQ(old_size, tree.size() + 1);

        int n = 1;
        for (const auto& item : tree) {
            ASSERT_EQ(n++, item);
            if (n == 3)
                n++;
        }
    }
}

TEST(method, output)
{
    bst<int> tree;

    tree.insert(3);
    tree.insert(1);
    tree.insert(2);
    tree.insert(4);

    std::vector<int> out = tree.output();

#ifdef LTR
    int n = 1;
    for (const auto& item : out)
        ASSERT_EQ(item, n++);
#endif

#ifdef TLR
    ASSERT_EQ(out[0], 3);
    ASSERT_EQ(out[1], 1);
    ASSERT_EQ(out[2], 2);
    ASSERT_EQ(out[3], 4);
#endif

#ifdef LRT
    ASSERT_EQ(out[0], 2);
    ASSERT_EQ(out[1], 1);
    ASSERT_EQ(out[2], 4);
    ASSERT_EQ(out[3], 3);
#endif
}

#ifdef INDEX_OF
TEST(method, index_of)
{
    bst<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);
    tree.insert(1);
    tree.insert(9);

    for (const auto& item : tree)
        ASSERT_EQ(item, tree.index_of(item) + 1);
}
#endif

#ifdef COUNT_MORE_THAN
TEST(method, count_more_than)
{
    bst<int> tree;

    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);

    ASSERT_EQ(tree.count_more_than(2), 2);
}
#endif

#ifdef BALANCE_FACTOR
TEST(method, balance_factor)
{
    bst<int> tree;
    
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    ASSERT_EQ(tree.balance_factor(), 2);
}
#endif


int
main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

