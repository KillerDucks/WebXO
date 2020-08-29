#include "HTTP.hpp"

namespace WebXO
{
    HTTP::HTTP(std::string httpPath, InterceptSettings interceptSettings) : _Log("HTTP"), iDirectory(httpPath), httpCode(WebXO::HTTPStatusCodes::OK), _interceptionSettings(interceptSettings), MIMETYPE(MimeType::HTML)
    {}
    HTTP::~HTTP()
    {}

    HTTPReq HTTP::ParseRequest(char* request)
    {
        vector<std::pair<string, string>> reqOpts;
        string t;
        bool requestLine = true;
        HTTPReq hRequest;

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

    std::pair<char*, int> HTTP::GetRequestedFile(HTTPReq hReq)
    {
        // [DEBUG] [IMPL] [TEST] Will compress all files regardless of Client Accept Ranges
        // char* buffer;
        // int fLength = 0;


        string filePath;
        string urlData;
        fstream ssFileReader;
        std::vector<std::string> dirLookup;
        std::regex findFile;

        // Interception Hooking [DEBUG] [NOTE] Might be moved to a diffrent place
        // _interception.HookSync(hReq, _interceptionSettings.callback);
        // if(_interceptionSettings.isBlocking)
        // {
        //     _interception.HookSync();
        // }
        // else
        // {
        //     _interception.HookAsync();
        // }

        // Get the web path request needed from the HTTP Request
        string file(hReq.requestType.substr(hReq.requestType.find(' ') + 1, hReq.requestType.size()));

        // Decode any URL parameters if any
        if(file.find('?') != std::string::npos)
        {
            // Capture the URL Data (After the '?')
            urlData = file.substr(file.find('?'), file.size());
            // Change the file to prevent failed lookups
            file = file.substr(0, file.find('?'));
        }

        // Convert the requested web path into a meaningful file path

        // Determine the Path of the requested file    
        std::string relativePath(iDirectory.GetBasePath());
        relativePath += (file.rfind('/') == 0) ? file : file.substr(0, file.rfind('/'));
        std::string parentPath(iDirectory.GetBasePath());
        parentPath += file;

        // Debug Logging
        // _Log.iLog("[%z] [%q] Calculated Relative Path: [%s]\n",Logarithm::INFO, relativePath.c_str());

        // Check if are given a absolute path (eg: [GET /])
        if(file.find('.') == std::string::npos)
        {
            // Check to see if the directory exists
            if(!iDirectory.doesExist(parentPath))
            {
                // Set the HTTP Status code
                httpCode = WebXO::HTTPStatusCodes::NOT_FOUND;
            }
            else
            {
                // There is no '.' found, assume this is a root path of a sub/directory
                std::regex rgx("(index.html)");
                // Log for debugging
                _Log.Log("Root path detected", Logarithm::INFO);
                // Set the MIME Type
                this->MIMETYPE = MimeType::HTML;
                // Scan the directories for this file
                filePath = iDirectory.ScanDir(rgx, (file.size() == 1) ? iDirectory.GetBasePath() : relativePath).at(0); // [CHANGE] Assume this is the first given file
                // Set the HTTP Status code
                httpCode = WebXO::HTTPStatusCodes::OK;
            }            
        }
        else
        {
            // This is not a root path so handle as a normal absolute path file

            // Check if the file exists
            if(!iDirectory.doesExist(parentPath))
            {
                // Log for debugging
                _Log.iLog("[%z] [%q] File [%s] does not exist\n",Logarithm::CRITICAL, file.c_str());
                // Set the MIME Type
                this->MIMETYPE = MimeType::HTML;
                // Set the HTTP Status code
                httpCode = WebXO::HTTPStatusCodes::NOT_FOUND;
            }
            else
            {
                // Use the relative path 
                if(iDirectory.szFile(parentPath) == 0)
                {
                    // filePath = "-1";
                    // Log for debugging
                    _Log.iLog("[%z] [%q] File [%s] is empty\n",Logarithm::CRITICAL, file.c_str());
                    // Set the HTTP Status Code
                    httpCode = WebXO::HTTPStatusCodes::NO_CONTENT;
                }
                else
                {
                    filePath = parentPath;
                    // Set the HTTP Status Code
                    httpCode = WebXO::HTTPStatusCodes::OK;
                    // Set the MIME Type
                    this->MIMETYPE = this->GetMIMEType(filePath);          
                }                
            
                      
            }
            
        }
        
        // Check if any HTTP Error codes have been raised
        switch (httpCode)
        {
        case WebXO::HTTPStatusCodes::NOT_FOUND:            
            {
                filePath.clear();
                filePath += iDirectory.GetBasePath();
                filePath += "/404.html";
                break;
            }
        case WebXO::HTTPStatusCodes::INTERNAL_SERVER_ERROR:            
            {
                filePath.clear();
                filePath += iDirectory.GetBasePath();
                filePath += "/500.html";
                break;
            }
        case WebXO::HTTPStatusCodes::NO_CONTENT:            
            {
                filePath.clear();
                filePath += iDirectory.GetBasePath();
                filePath += "/204.html";
                break;
            }        
        default:
            break;
        }

        // Final Checks to see if we have a vaild file path
        if(filePath == "-1")
        {
            // Invalid file
            filePath.clear();
            filePath += iDirectory.GetBasePath();
            filePath += "/500.html";

            // Set the MIME Type
            this->MIMETYPE = MimeType::HTML;         
            // Set the HTTP status code
            httpCode = WebXO::HTTPStatusCodes::INTERNAL_SERVER_ERROR; 
        }
        
        // Open a stream and read the file into the buffer
        // ssFileReader.open(filePath, std::ios::binary | std::ios::in);

        // // Check if the stream is okay to work with
        // if(ssFileReader.good())
        // {
        //     // Get the file length
        //     ssFileReader.seekg(0, ssFileReader.end);
        //     fLength = ssFileReader.tellg();
        //     ssFileReader.seekg(0, ssFileReader.beg);

        //     _Log.iLog("[%z] [%q] File Size: [~%dKB]\n", Logarithm::NOTICE, fLength / 1024); // [DEBUG] Print

        //     buffer = new char[fLength + 1];
        //     ssFileReader.read(buffer, fLength);
        //     ssFileReader.close();
        // }
        // else
        // {
        //     // [TODO] Move the error checking to above the file read
        //     _Log.Log("Invalid File Stream", Logarithm::CRITICAL);        
        //     filePath.clear();
        //     filePath += iDirectory.GetBasePath();
        //     filePath += "/500.html";
        //     buffer = new char[iDirectory.GetFileSize(filePath)];
        //     memset(buffer, 0x00, iDirectory.GetFileSize(filePath));
        //     memcpy(buffer, iDirectory.ReadFile(filePath), iDirectory.szFile(filePath));
        //     // Set the MIME Type
        //     this->MIMETYPE = MimeType::HTML;
        //     // Set the HTTP status code
        //     httpCode = WebX::HTTPStatusCodes::INTERNAL_SERVER_ERROR;
        //     ssFileReader.close();
        // }

        Compression zippy;
        
        // Verbose Logging
        _Log.Log("Returning the Buffer", Logarithm::NOTICE);

        // return std::pair<char*, int>(buffer, fLength);
        return zippy.DeflateFile(filePath); 
        // return buffer;
    }

    HTTPRes HTTP::GenerateHTTPResponse()
    {
        // Structs !!!
        HTTPRes httpRes;
        hGeneral httpGeneral;
        hEntity httpEntity;

        httpGeneral.Date = httpGeneral.GetCurrentTime();
        httpEntity.contentLength += "Content-Length: ";
        httpGeneral.Connection = "Connection: close";

        // Set the Response Code + Phrase
        httpRes.statusCode = (int)httpCode;
        // [TODO] Use the Enum to set the phrase corrently
        httpRes.reasonPhrase = "OKAY";

        //  Set the HTTP Entity Header (ignore the cast issue, will be looked into later on)
        httpRes.httpEntityHeader = httpEntity;

        // Set the MIME Type correctly        
        // [TODO] Possibly break into a helper function that is smaller [NOTE] This is really dumb I should consolidate this with HTTP::GetMINEType
        switch (this->MIMETYPE)
        {
        case MimeType::HTML:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/html;";
            break;
        case MimeType::CSS:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/css;";
            break;
        case MimeType::JS:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/javascript;";
            break;
        case MimeType::IMAGE:
            httpRes.httpEntityHeader.contentType = "Content-Type: image/jpeg;";
            break;
        default:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/plain;";
            break;
        }

        httpRes.httpGeneralHeader = httpGeneral;

        httpRes.httpEntityHeader.contentEncoding = "Content-Encoding: deflate";

        httpRes.httpEntityHeader.contentLength = strlen(httpRes.ReturnHeader().c_str());

        // [DEBUG] Debug Printing
        printf("\n\n%s\n\n", httpRes.ReturnHeader().c_str());


        return httpRes;
    }

    // [TODO] I should probs change this :P
    enum HTTP::MimeType HTTP::GetMIMEType(std::string filePath)
    {
        // [NOTE] This is the most basic and most likely the worst way of doing this
        std::string fExt = filePath.substr(filePath.rfind('.') + 1, filePath.length());
        
        // Debug Logging
        // _Log.iLog("[%z] [%q] Detected a MIME Type using file extension [%s]\n", Logarithm::NOTICE, fExt.c_str());
        // printf("Detected a MIME Type using file extension [%s] for the file [%s]\n", fExt.c_str(), filePath.c_str());
        
        if(fExt == std::string("html"))
        {
            _Log.iLog("[%z] [%q] Detected a MIME Type of [%s]\n", Logarithm::NOTICE, "HTML");
            return MimeType::HTML;
        }
        if(fExt == std::string("css"))
        {
            _Log.iLog("[%z] [%q] Detected a MIME Type of [%s]\n", Logarithm::NOTICE, "CSS");
            return MimeType::CSS;
        }
        if(fExt == std::string("js"))
        {
            _Log.iLog("[%z] [%q] Detected a MIME Type of [%s]\n", Logarithm::NOTICE, "JS");
            return MimeType::JS;
        }
        if(fExt == std::string("png") || fExt == std::string("jpg") || fExt == std::string("gif") || fExt == std::string("jpeg") || fExt == std::string("svg"))
        {
            _Log.iLog("[%z] [%q] Detected a MIME Type of [%s]\n", Logarithm::NOTICE, "IMAGE");
            return MimeType::IMAGE;
        }
        // [TODO] Add favicon support

        _Log.iLog("[%z] [%q] Failed to Detected a MIME Type\n", Logarithm::CRITICAL);
        return MimeType::HTML;
    }
};