#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/FolderView.hpp"

TEST(FolderView, GetFiles) {
    using namespace WebXO;
    FolderView fViewer;
    EXPECT_EQ(3, fViewer.GetFiles(std::string("./assets")).size());
}

TEST(FolderView, GeneratePage) {
    using namespace WebXO;
    FolderView fViewer;
    EXPECT_EQ(3, fViewer.GetFiles(std::string("./assets")).size());
}
