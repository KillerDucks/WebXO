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
// using std::regex;

namespace WebX
{
    class Directory
    {
    private:
        // Functions
        void ErrorHandler();

        // Data
        string basePath = "./";
        std::regex fileExts;
        vector<string> fVector;
    public:
        // Constructors
        Directory(int d);
        Directory(char* _baseDirectory = (char*)"./", std::regex _fileExtensions = (std::regex)"\\(.png|.html|.js|.css)\\W");
        ~Directory();

        // Directory Operations
        void ScanDir();
        vector<string> ScanDir(std::regex searchCriteria);
        void GetWebFiles();

    };    
}

#endif