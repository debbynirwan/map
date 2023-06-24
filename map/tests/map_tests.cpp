#include "map.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

TEST(MapTests, EmptyMap)
{
    Map<int, int> map;

    EXPECT_EQ(map.Size(), 0);
    EXPECT_EQ(map.MaxDepth(), 0);
}

TEST(MapTests, CopyMap)
{
    Map<int, int> map;

    EXPECT_EQ(map.Size(), 0);

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    Map<int, int> map2(map);

    EXPECT_EQ(map2.Size(), 11);
    EXPECT_EQ(map2.At(1), 1);
    EXPECT_EQ(map.Size(), 11);
}

TEST(MapTests, CopyAssignMap)
{
    Map<int, int> map;
    Map<int, int> map2;

    EXPECT_EQ(map.Size(), 0);
    EXPECT_EQ(map2.Size(), 0);

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map2 = map;

    EXPECT_EQ(map2.Size(), 11);
    EXPECT_EQ(map2.At(1), 1);
    EXPECT_EQ(map.Size(), 11);
}

TEST(MapTests, MoveMap)
{
    Map<int, int> map;

    EXPECT_EQ(map.Size(), 0);

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    Map<int, int> map2(std::move(map));

    EXPECT_EQ(map2.Size(), 11);
    EXPECT_EQ(map2.At(1), 1);
    EXPECT_EQ(map.Size(), 0);
}

TEST(MapTests, MoveAssignMap)
{
    Map<int, int> map;
    Map<int, int> map2;

    EXPECT_EQ(map.Size(), 0);
    EXPECT_EQ(map2.Size(), 0);

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map2 = std::move(map);

    EXPECT_EQ(map2.Size(), 11);
    EXPECT_EQ(map2.At(1), 1);
    EXPECT_EQ(map.Size(), 0);
}

TEST(MapTests, NonEmptyMap)
{
    Map<int, int> map;

    map.Insert(1, 1);
    EXPECT_NE(map.Size(), 0);
}

TEST(MapTests, InsertAndRetrieve)
{
    Map<int, int> map;

    map.Insert(1, 1);
    EXPECT_EQ(map.At(1), 1);
}

TEST(MapTests, RetrieveThrow)
{
    Map<int, int> map;

    EXPECT_THROW(map.At(1), std::out_of_range);
}

TEST(MapTests, Remove)
{
    Map<int, int> map;

    map.Insert(1, 1);
    EXPECT_EQ(map.Size(), 1);

    map.Remove(1);
    EXPECT_EQ(map.Size(), 0);
}

TEST(MapTests, InsertMulti)
{
    Map<int, int> map;

    map.Insert(100, 1);
    map.Insert(110, 1);
    map.Insert(90, 1);
    map.Insert(80, 1);
    map.Insert(70, 1);
    map.Insert(60, 1);
    map.Insert(50, 1);

    EXPECT_EQ(map.Size(), 7);
}

TEST(MapTests, InsertRemoveMulti)
{
    Map<int, int> map;

    map.Insert(100, 1);
    map.Insert(110, 1);
    map.Insert(90, 1);
    map.Insert(80, 1);
    map.Insert(70, 1);
    map.Insert(60, 1);
    map.Insert(50, 1);

    EXPECT_EQ(map.Size(), 7);

    map.Remove(100);
    map.Remove(110);
    map.Remove(90);
    map.Remove(80);
    map.Remove(70);
    map.Remove(60);
    map.Remove(50);

    EXPECT_EQ(map.Size(), 0);
}

TEST(MapTests, InsertExisting)
{
    Map<int, int> map;

    map.Insert(100, 1);
    map.Insert(100, 1);

    EXPECT_EQ(map.Size(), 1);
}

TEST(MapTests, Insert1)
{
    Map<int, int> map;

    map.Insert(1, 1);
    map.Insert(5, 1);
    map.Insert(2, 1);

    EXPECT_EQ(map.Size(), 3);
}

TEST(MapTests, Insert2)
{
    Map<int, int> map;

    map.Insert(10, 1);
    map.Insert(1, 1);
    map.Insert(5, 1);
    map.Insert(2, 1);

    EXPECT_EQ(map.Size(), 4);
}

TEST(MapTests, Insert3)
{
    Map<int, int> map;

    map.Insert(10, 1);
    map.Insert(1, 1);
    map.Insert(15, 1);
    map.Insert(12, 1);

    EXPECT_EQ(map.Size(), 4);
}

TEST(MapTests, RemoveScenario1)
{
    Map<int, int> map;

    map.Insert(10, 1);
    EXPECT_EQ(map.Size(), 1);

    map.Remove(1);
    EXPECT_EQ(map.Size(), 1);
}

TEST(MapTests, RemoveScenario2)
{
    Map<int, int> map;

    map.Insert(10, 1);
    map.Insert(15, 1);
    EXPECT_EQ(map.Size(), 2);

    map.Remove(10);
    EXPECT_EQ(map.Size(), 1);
}

TEST(MapTests, RemoveScenario3)
{
    Map<int, int> map;

    map.Insert(10, 1);
    map.Insert(15, 1);
    map.Insert(8, 1);
    map.Insert(13, 1);
    EXPECT_EQ(map.Size(), 4);

    map.Remove(10);
    EXPECT_EQ(map.Size(), 3);
}

TEST(MapTests, RemoveScenario4)
{
    Map<int, int> map;

    map.Insert(10, 1);
    map.Insert(15, 1);
    map.Insert(8, 1);
    map.Insert(13, 1);
    map.Insert(17, 1);
    map.Insert(19, 1);
    EXPECT_EQ(map.Size(), 6);

    map.Remove(15);
    EXPECT_EQ(map.Size(), 5);
}

TEST(MapTests, RemoveScenario5)
{
    Map<int, int> map;

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map.Remove(3);
    map.Remove(2);
    EXPECT_EQ(map.Size(), 9);
}

TEST(MapTests, RemoveScenario6)
{
    Map<int, int> map;

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map.Remove(1);
    map.Remove(2);
    EXPECT_EQ(map.Size(), 9);
}

TEST(MapTests, RemoveScenario7)
{
    Map<int, int> map;

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map.Remove(8);
    map.Remove(9);
    map.Remove(10);

    EXPECT_EQ(map.Size(), 8);
}

TEST(MapTests, RemoveScenario8)
{
    Map<int, int> map;

    for (int i = 1; i < 12; i++)
        map.Insert(i, 1);

    EXPECT_EQ(map.Size(), 11);

    map.Remove(5);

    EXPECT_EQ(map.Size(), 10);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}