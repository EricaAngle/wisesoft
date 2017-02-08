#include <gtest/gtest.h>
#include "longsublen.h"

TEST(testssl , testssl_case_1){
    leetcode::SolutionSSL ssl;
    EXPECT_EQ(3, ssl.lengthOfLongestSubstring("abcabcbb"));
}