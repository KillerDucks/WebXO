#include <gtest/gtest.h>

// Include in the Class to test
#include "../src/WebXLib/FolderView.hpp"

// This test will see if all the files are retuned
TEST(FolderView, GetFiles) {
    using namespace WebXO;
    FolderView fViewer;
    EXPECT_EQ(3, fViewer.GetFiles(std::string("./assets")).size());
}

// This test will check to see if the page is getting generated (sort of)
TEST(FolderView, GeneratePage) {
    using namespace WebXO;
    FolderView fViewer;
    EXPECT_EQ(13, fViewer.GeneratePage( fViewer.GetFiles( std::string("./assets") ) ).find_first_of('<') );
}
