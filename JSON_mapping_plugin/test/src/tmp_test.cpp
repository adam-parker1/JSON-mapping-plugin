// #include "JSON_mapping_plugin.hpp"
#include "tmp.hpp"

#include <gtest/gtest.h>

TEST(TmpAddTest, CheckValues) {
    ASSERT_EQ(1+2, 3);
    EXPECT_TRUE(true);
}

TEST(TmpAdd2Test, CheckValues) {
    ASSERT_EQ(tmp::add(1,2), 3);
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
