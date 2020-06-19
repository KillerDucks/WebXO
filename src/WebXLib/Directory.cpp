#include "Directory.hpp"

namespace WebX
{
    Directory::Directory(int d) : _Log("Directory")
    {
    }

    Directory::Directory(char* _baseDirectory, std::regex _fileExtensions) : _Log("Directory")
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
            printf("[Scanner] File found [%s]\n", file.path().c_str());
        }
    }

    vector<string> Directory::ScanDir(std::regex searchCriteria)
    {
        std::smatch sm;
        vector<string> files;
        // _Log.Log("Scanning Directory for Queried File", Logarithm::INFO);
        for(auto file : fs::recursive_directory_iterator(this->basePath))
        {
            // Convert into a std::string
            string fPath(file.path());
            // Run the file against the regex
            if(std::regex_search(fPath, sm, searchCriteria))
            {
                // This file is a Web file
                files.push_back(file.path());
                // printf("File found [%s]\n", file.path().c_str());
                _Log.iLog("[%z] [%q] Requested File was found @ [%s]\n",Logarithm::INFO, file.path().c_str());
            }
        }

        return files;
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

    char* Directory::ReadFile(std::string filePath)
    {
        std::fstream ssFileReader;
        size_t fLength = 0;
        char* buffer;

        ssFileReader.open(filePath, std::ios::binary | std::ios::in);

        // Check if the stream is okay to work with
        if(ssFileReader.good())
        {
            // Get the file length
            ssFileReader.seekg(0, ssFileReader.end);
            fLength = ssFileReader.tellg();
            ssFileReader.seekg(0, ssFileReader.beg);

            // if the file size is zero 
            if(fLength == 0)
            {
                ssFileReader.close();
                return nullptr;
            }
            else
            {
                // _Log.iLog("[%z] [%q] File Size: [%d]\n", Logarithm::NOTICE, fLength); // [DEBUG] Print

                // Init the buffer with the file length
                buffer = new char[fLength + 1];
                // Read the file into the buffer
                ssFileReader.read(buffer, fLength);

                ssFileReader.close();

                // Add a null terminator
                buffer[fLength] = '\n';

                return buffer;
            }
        }
        else
        {
            _Log.Log("Invalid File Stream", Logarithm::CRITICAL);            
            ssFileReader.close();
            return nullptr;
        }

        return nullptr;
    }

    size_t  Directory::GetFileSize(std::string filePath)
    {
        size_t fileLength = 0;
        std::fstream ssFileReader;
        ssFileReader.open(filePath, std::ios::binary | std::ios::in);

        // Check if the stream is okay to work with
        if(ssFileReader.good())
        {
            // Get the file length
            ssFileReader.seekg(0, ssFileReader.end);
            fileLength = ssFileReader.tellg();
            ssFileReader.seekg(0, ssFileReader.beg);

        }
        else
        {
            _Log.Log("Invalid File Stream", Logarithm::CRITICAL);            
            ssFileReader.close();            
        }
        return fileLength;
    }

    bool Directory::isFile(std::string filePath)
    {
        // See if there is an extension via a '.' in the string
        if(filePath.find_last_of('/', 1) == filePath.length() || filePath.find('.', 1) == std::string::npos)
        {
            // This is not a file
            return false;
        }
        // This is a file return true
        return true;
    }
}