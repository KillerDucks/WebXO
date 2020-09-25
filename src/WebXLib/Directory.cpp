#include "Directory.hpp"

namespace WebXO
{
    Directory::Directory(std::string _baseDirectory) : _Log("Directory")
    {
        if(!_baseDirectory.empty())
        {
            this->basePath = _baseDirectory;
        }
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

    // @ This is useful when looking for files outside the root directory
    vector<string> Directory::ScanDir(std::regex searchCriteria, std::string directoryLevel)
    {
        std::smatch sm;
        vector<string> files;
        bool root = this->isRoot(directoryLevel);
        _Log.iLog("[%z] [%q] Directory Path is Root? : [%d]\n",Logarithm::INFO, root);
        _Log.iLog("[%z] [%q] Directory Level Path: [%s]\n\n",Logarithm::INFO, directoryLevel.c_str());

        for(auto file : fs::recursive_directory_iterator(directoryLevel))
        {
            // Convert into a std::string
            string fPath(file.path());
            // Run the file against the regex
            if(std::regex_search(fPath, sm, searchCriteria))
            {
                // Check if the file needs to be from the root
                if(directoryLevel == file.path().parent_path())
                {
                    // Log for debugging
                    // _Log.iLog("[%z] [%q] Directory Base Path: [%s]\n",Logarithm::INFO, this->basePath.c_str());
                    // _Log.iLog("[%z] [%q] Requested File Parent Path: [%s]\n",Logarithm::INFO, file.path().parent_path().c_str());
                    // _Log.iLog("[%z] [%q] Requested File Relative Path: [%s]\n",Logarithm::INFO, file.path().relative_path().c_str());
                    // _Log.iLog("[%z] [%q] Requested File Stem: [%s]\n",Logarithm::INFO, file.path().stem().c_str());
                    // _Log.iLog("[%z] [%q] Requested File Extension: [%s]\n",Logarithm::INFO, file.path().extension().c_str());
                    files.push_back(file.path());
                    _Log.iLog("[%z] [%q] Requested File was found @ [%s]\n\n",Logarithm::INFO, file.path().c_str());
                }
            }
        }

        // Check to see if there are no files (double error checking)
        if(files.size() == 0)
        {
            // There are no files
            files.push_back((std::string)"-1");
            _Log.Log("There are no files found, returning -1", Logarithm::CRITICAL);
        }

        return files;
    }
    
    // [NOTE] This is hardly used, possible to remove this method completely ???
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
                if(!fs::is_directory(file))
                {
                    // This file is a Web file
                    this->fVector.push_back(file.path());
                    printf("File found [%s]\n", file.path().c_str());
                }
            }
        }
    }

    // [NOTE] This function has no real place as we are using the `fs` namespace for ease of operation
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

    // [NOTE] This method is set for removal in v1.4.X and is superseced by `Directory::szFile`
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

    // [NOTE] This is possible to change into `fs::is_regular_file` to remove the extra bloat
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

    bool Directory::isDirectory(std::string filePath)
    {
        return fs::is_directory(filePath);
    }

    bool Directory::isRoot(std::string filePath)
    {
        if(filePath == this->basePath)
        {
            return true;
        }

        return false;
    }

    bool Directory::doesExist(std::string filePath)
    {
        if(fs::exists(filePath))
        {
            return true;
        }           
        return false;
    }

    size_t Directory::szFile(std::string filePath)
    {
        // [DEBUG] Print
        // _Log.iLog("[%z] [%q] szFile() filePath: [%s]\n", filePath.c_str());
        return fs::file_size(filePath);
    }

    // @ This function will assist when looking for files that are/not in the root directory [NOTE] This method is set for removal in v1.4.X
    std::string Directory::getRootPath(std::string filePath)
    {
        return "NYI";
    }
}