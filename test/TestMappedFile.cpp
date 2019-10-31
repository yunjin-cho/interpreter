/// Yunjin Cho

#include "interpreter/MappedFile.h"
#include <fstream>
#include <gtest/gtest.h>

// Tests that the constructor throws when the file is not found.
TEST(MappedFile, NotFoundCtorThrows) {
    try {
        MappedFile doesntExist("afjsafka");
    } catch (...) {
        return;
    }

    FAIL() << "Mapped file constructor didn't throw for non existant file";
}

TEST(MappedFile, Basic) {
    std::ofstream outfile("test.txt");
    outfile << "test\n";
    outfile.close();

    MappedFile test("test.txt");

    ASSERT_STREQ(test, "test\n");

    remove("test.txt");
}
