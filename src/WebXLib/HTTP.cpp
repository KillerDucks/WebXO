#include "HTTP.hpp"

namespace WebX
{
    HTTP::HTTP()
    {}
    HTTP::~HTTP()
    {}

    HTTP::HTTPReq HTTP::ParseRequest(char* request)
    {
        vector<std::pair<string, string>> reqOpts;
        string t;
        bool requestLine = true;
        HTTP::HTTPReq hRequest;
        memset(&hRequest, 0x00, sizeof(hRequest));

        // Look through the request and fill in the struct
        for (size_t i = 0; i < strlen(request); i++)
        {
            // Convert the buffer into a managable sting vector
            if(request[i] == '\n')
            {
                continue;
            }
            if(request[i] == '\r')
            {
                if(i + 2 >= strlen(request))
                {
                    continue;
                }
                t += '\0';
                string key, val;
                if(requestLine)
                {
                    requestLine = false;
                    key = "requestLine";
                    val = t.substr(0, t.find(' ', t.find(' ') + 1));
                    reqOpts.push_back(std::pair<string, string>(key, val));
                    t.clear();
                    continue;
                }
                key = t.substr(0, t.find(':'));
                val = t.substr(t.find(':') + 2, t.length());
                reqOpts.push_back(std::pair<string, string>(key, val));
                t.clear();
            }
            else
            {
                t += request[i];
            }
            
        }

        // [TODO] Debugging Print
        // for(auto o : reqOpts)
        // {
        //     printf("HTTP Request [%s] => [%s]\n", o.first.c_str(), o.second.c_str());
        // }

        for(auto opt : reqOpts)
        {
            switch (hash(opt.first.c_str()))
            {
            case hash("requestLine"):
                hRequest.requestType = opt.second;                
                break;

            case hash("Host"):
                hRequest.host = opt.second;                
                break;

            case hash("Connection"):
                hRequest.connection = opt.second;                
                break;

            case hash("DNT"):
                hRequest.dnt = std::stoi(opt.second);                
                break;

            case hash("Upgrade-Insecure-Requests"):
                hRequest.upgrade_insecure_requests = std::stoi(opt.second);                
                break;

            case hash("User-Agent"):
                hRequest.user_Agent = opt.second;                
                break;

            case hash("Sec-Fetch-User"):
                hRequest.sec_Fetch_User = opt.second;                
                break;

            case hash("Accept"):
                hRequest.accept = opt.second;                
                break;

            case hash("Sec-Fetch-Site"):
                hRequest.sec_Fetch_Site = opt.second;                
                break;

            case hash("Sec-Fetch-Mode"):
                hRequest.sec_Fetch_Mode = opt.second;                
                break;

            case hash("Accept-Encoding"):
                hRequest.accept_Encoding = opt.second;                
                break;

            case hash("Accept-Language"):
                hRequest.accept_Languages = opt.second;                
                break;

            default:
                break;
            }
        }

        // [TODO] Debugging Print
        // printf("%s\n", hRequest.requestType.c_str());
        
        return hRequest;
    }

    char* HTTP::GetRequestedFile(HTTPReq hReq)
    {
        char* buffer;
        int fLength = 0;
        string filePath;
        fstream ssFileReader;
        Directory iDirectory(54);

        // Get the web path request needed from the HTTP Request
        string file(hReq.requestType.substr(hReq.requestType.find(' ') + 1, hReq.requestType.size()));
        // Convert the requested web path into a meaningful file path
        // First check if the request wants the root '/'
        if(file == string("/"))
        {
            // Check for the index file of any extension
            std::regex findFile("(index.html)");
            // [TODO] Change this to find any index file not just a html one
            filePath = iDirectory.ScanDir(findFile).at(0);
        }
        else
        {
            // All other files
            std::regex findFile(file);
            if(iDirectory.ScanDir(findFile).size() <= 0)
            {
                // 404 Message
                std::regex findFile("(404.html)");
                filePath = iDirectory.ScanDir(findFile).at(0);
            }
            else
            {
                // Return the file path
                filePath = iDirectory.ScanDir(findFile).at(0);
            }
            
        }
        
        // Open a stream and read the file into the buffer
        ssFileReader.open(filePath, std::ios::binary | std::ios::in);

        // Check if the stream is okay to work with
        if(ssFileReader.good())
        {
            // Get the file length
            ssFileReader.seekg(0, ssFileReader.end);
            fLength = ssFileReader.tellg();
            ssFileReader.seekg(0, ssFileReader.beg);

            // Init the buffer with the file length
            buffer = new char[fLength];
            // Read the file into the buffer
            ssFileReader.read(buffer, fLength);

            ssFileReader.close();
        }

        return buffer;
    }

    HTTP::HTTPRes HTTP::GenerateHTTPResponse(char* message)
    {
        HTTPRes httpRes;
        // memset(&httpRes, 0x00, sizeof(httpRes));
        hGeneral httpGeneral;
        // memset(&httpGeneral, 0x00, sizeof(httpGeneral));
        hEntity httpEntity;
        // memset(&httpEntity, 0x00, sizeof(httpEntity));

        httpGeneral.Date = httpGeneral.GetCurrentTime();
        httpEntity.contentLength += "Content-Length: ";
        httpGeneral.Connection = "Connection: close";



        httpRes.statusCode = 200;
        httpRes.reasonPhrase = "OKAY";
        // memcpy(&httpRes.httpGeneralHeader, (char*)httpGeneral, sizeof(httpGeneral));
        httpRes.httpEntityHeader = httpEntity;
        httpRes.httpGeneralHeader = httpGeneral;

        httpRes.httpEntityHeader.contentLength = strlen(httpRes.ReturnHeader().c_str());

        // [TODO] Debug Printing
        // printf("\n\n%s\n\n", httpRes.ReturnHeader().c_str());

        return httpRes;
    }
};