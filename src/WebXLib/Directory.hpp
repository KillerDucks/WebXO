#include <stdio.h>
#include <string.h>
#include <vector>
#include <regex>
#include <string>

#include <fstream>

#include <experimental/filesystem>
#include "Logarithm.hpp"

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

        // Logger
        Logarithm _Log;
    public:
        // Constructors
        Directory(int d);
        Directory(char* _baseDirectory = (char*)"./", std::regex _fileExtensions = (std::regex)"\\(.png|.html|.js|.css)\\W");
        ~Directory();

        // Directory Operations
        void ScanDir();
        vector<string> ScanDir(std::regex searchCriteria);
        void GetWebFiles();

        // File Operations
        char* ReadFile(std::string filePath);
        size_t  GetFileSize(std::string filePath);

        // Helpers
        bool isFile(std::string filePath);

    };    
}

#endif