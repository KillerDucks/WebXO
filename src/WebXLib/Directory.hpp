#include <regex>

#include <experimental/filesystem>
#include "Logarithm.hpp"
#include "Common.hpp"

#ifndef WEBX_DIR_H
#define WEBX_DIR_H

// [NOTE] Change this when full support is released
namespace fs = std::experimental::filesystem;

// [NOTE] These should be removed (bad practise)
using std::string;
using std::vector;

namespace WebXO
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