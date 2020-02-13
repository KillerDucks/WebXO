#include "Directory.hpp"

namespace WebX
{
    Directory::Directory(char* _baseDirectory, std::regex _fileExtensions)
    {
        if(strlen(_baseDirectory) != 0)
        {
            // [TODO]
            // Assume a valid path has been passed
            this->basePath = _baseDirectory;
            this->fileExts = _fileExtensions;
        }
    }

    Directory::~Directory()
    {}

    void Directory::ScanDir()
    {
        for(auto file : fs::recursive_directory_iterator(this->basePath))
        {
            printf("File found [%s]\n", file.path().c_str());
        }
    }
    
    void Directory::GetWebFiles()
    {
        std::smatch sm;

        for(auto file : fs::recursive_directory_iterator(this->basePath))
        {
            // Convert into a std::string
            string fPath(file.path());
            // Run the file against the regex
            if(std::regex_search(fPath, sm, this->fileExts))
            {
                // This file is a Web file
                this->fVector.push_back(file.path());
                printf("File found [%s]\n", file.path().c_str());
            }
        }
    }
}