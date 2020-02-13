#include "Directory.hpp"

namespace WebX
{
    Directory::Directory(char* _baseDirectory, regex _fileExtensions)
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
        for(auto file : fs::recursive_directory_iterator(this->basePath))
        {
            if(regex_search((const char*)file.path().c_str(), this->fileExts))
            {
                // This file is a Web file
                this->fVector.push_back(file.path());
                printf("File found [%s]\n", file.path().c_str());
            }
        }
    }
}