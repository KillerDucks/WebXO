#include "VirtualHosts.hpp"

namespace WebXO
{
    // Essentially this will look at the "Host" request parameter and then serve the files out of the folder

    VirtualHosts::VirtualHosts(std::string filename)
    {
        this->vHosts = std::move(this->Parse(filename));
    }

    std::map<std::string, std::string> VirtualHosts::Parse(std::string filename)
    {
        std::fstream fileStream;
        std::map<std::string, std::string> vHosts;
        int fLength = 0;
        char* buffer;

        fileStream.open(filename, std::ios::in);
        if(fileStream.good())
        {
            fileStream.seekg(0, fileStream.end);
            fLength = fileStream.tellg();
            fileStream.seekg(0, fileStream.beg);

            buffer = new char[fLength + 1];


            fileStream.read(buffer, fLength);
        }
        else
        {
            fileStream.close();
            return vHosts;
        }
        

        fileStream.close();

        // Now parse the file
        std::string strBuffer = buffer;
        strBuffer = strBuffer.erase(fLength, strBuffer.length());
        int cPos = 0;

        if(strBuffer.find('\n') == std::string::npos)
        {
            // [DEBUG] Print Statements
            // printf("Buffer %s\n", strBuffer.c_str());
            // printf("Host %s | Path %s\n", strBuffer.substr(0, strBuffer.find(":")).c_str(), strBuffer.substr(strBuffer.find(":") + 1, strBuffer.length()).c_str());
            vHosts[strBuffer.substr(0, strBuffer.find(":"))] = strBuffer.substr(strBuffer.find(":") + 1, strBuffer.length());
            return vHosts;
            
        }

        while(cPos != (int)strBuffer.length())
        {
            std::string s = strBuffer.substr(cPos, strBuffer.find('\n', cPos));
            // [DEBUG] Print Statements
            // printf("Host %s | Path %s\n", s.substr(0, s.find(":")).c_str(), s.substr(s.find(":") + 1, s.length()).c_str());
            vHosts[s.substr(0, s.find(":"))] = s.substr(s.find(":") + 1, s.length());
            if(strBuffer.find('\n', cPos) == std::string::npos)
            {
                cPos = static_cast<int>(strBuffer.length());
                break;
            }
            cPos = (int)strBuffer.find('\n', cPos) + 1;
        }

        return vHosts;
    }    

    std::string VirtualHosts::Query(std::string host)
    {
        if(this->vHosts.empty())
        {
            return std::string("-1");
        }

        if(this->vHosts.find(host) == this->vHosts.end())
        {
            return std::string("-1");
        }

        return this->vHosts.find(host)->second;
    }
};