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
        Directory(std::string _baseDirectory);
        Directory(char* _baseDirectory = (char*)"./", std::regex _fileExtensions = (std::regex)"\\(.png|.html|.js|.css)\\W");
        ~Directory();

        // Directory Operations
        void ScanDir();
        vector<string> ScanDir(std::regex searchCriteria);
        vector<string> ScanDir(std::regex searchCriteria, std::string directoryLevel);
        void GetWebFiles();

        // File Operations
        char* ReadFile(std::string filePath);
        size_t  GetFileSize(std::string filePath);

        // Helpers
        bool isFile(std::string filePath);        // [TODO] Made redundant via fs std
        bool isDirectory(std::string filePath);
        bool isRoot(std::string filePath);
        bool doesExist(std::string filePath);
        size_t szFile(std::string filePath);

        std::string getRootPath(std::string filePath);

        // inlines/statics
        inline std::string GetBasePath()
        {
            return this->basePath;
        }

    };    
}

#endif