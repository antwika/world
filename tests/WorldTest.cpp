#include "gtest/gtest.h"
#include "World.h"

TEST(WorldTest, DefaultConstruction)
{
    World world;
    EXPECT_TRUE(world.isEven(2));
}
