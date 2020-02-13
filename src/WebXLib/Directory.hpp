#include <stdio.h>
#include <string.h>
#include <vector>
#include <regex>
#include <string>

#include <experimental/filesystem>

#ifndef WEBX_DIR_H
#define WEBX_DIR_H

namespace fs = std::experimental::filesystem;
using std::string;
using std::vector;
using std::regex;

namespace WebX
{
    class Directory
    {
    private:
        // Functions
        void ErrorHandler();

        // Data
        string basePath = "./";
        regex fileExts;
        vector<string> fVector;
    public:
        // Constructors
        Directory(char* _baseDirectory = (char*)"./", regex _fileExtensions = regex("(.png|.html|.js|.css)"));
        ~Directory();

        // Directory Operations
        void ScanDir();
        void GetWebFiles();

    };    
}

#endif