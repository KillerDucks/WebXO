#include "HTTP.hpp"

namespace WebX
{
    HTTP::HTTP() : _Log("HTTP"), httpCode(WebX::HTTPStatusCodes::OK), MIMETYPE(MimeType::HTML)
    {}
    HTTP::~HTTP()
    {}

    HTTP::HTTPReq HTTP::ParseRequest(char* request)
    {
        vector<std::pair<string, string>> reqOpts;
        string t;
        bool requestLine = true;
        HTTP::HTTPReq hRequest;
        // memset(&hRequest, 0x00, sizeof(hRequest));

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
        std::vector<std::string> dirLookup;
        std::regex findFile;
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
            _Log.Log("Root path detected", Logarithm::INFO);
            this->MIMETYPE = MimeType::HTML;
            for(auto file : iDirectory.ScanDir(findFile))
            {
                int tmp = 0;
                for(char c : file)
                {
                    if(c == '/')
                    {
                        tmp++;
                    }
                }
                if(tmp >= 3)
                {
                    continue;
                }
                else
                {
                    filePath = file;
                    break;
                }
                
            }
            // filePath = iDirectory.ScanDir(findFile).at(0);
        }
        else
        {
            // Check if the request is to a root of a directory
            if(!iDirectory.isFile(file))
            {
                // Check if there is a file but with an extension
                std::string altPath = "(";
                altPath.append(file);
                // findFile = file;
                
                // Assume the Root file of that directory                    
                if(file.back() != '/')
                {
                    altPath.append("/index.html)");
                }
                else
                {
                    altPath.append("index.html)");
                }
                
                findFile = altPath;
                // _Log.iLog("[%z] [%q] Scanning Directory for [%s]\n",Logarithm::INFO, altPath.c_str()); // [DEBUG]
                _Log.Log("Alternate Root Path Located", Logarithm::NOTICE);
                
            }
            else
            {
                // All other files
                findFile = file;
            }
                        
            // Scan the Directory Once !!!
            _Log.iLog("[%z] [%q] Scanning Directory for [%s]\n",Logarithm::INFO, file.c_str());
            dirLookup = iDirectory.ScanDir(findFile);

            if(dirLookup.size() <= 0)
            {
                // 404 Message
                _Log.iLog("[%z] [%q] File not found !!! Sending a 404 back to the Client\n", Logarithm::NOTICE);
                std::regex findFile("(404.html)");
                filePath = iDirectory.ScanDir(findFile).at(0);
                // Set the MIME Type
                this->MIMETYPE = MimeType::HTML;
                // Set the HTTP status code
                httpCode = WebX::HTTPStatusCodes::NOT_FOUND;
            }
            else
            {
                // Out of the files found check to see if any are directories
                for(size_t i = 0; i < dirLookup.size(); i++)
                {
                    if(iDirectory.isFile(dirLookup.at(i)))
                    {
                        // This is a file [CHANGE] This will only use the first item that is a file 
                        filePath = dirLookup.at(i);

                        // _Log.iLog("[%z] [%q] [%s] is a File, Responding with this file!!!\n", Logarithm::NOTICE, dirLookup.at(i).c_str()); // [DEBUG] Print

                        // Check if the file is not 0 in length
                        if(iDirectory.GetFileSize(filePath) == 0)
                        {
                            // 204
                            _Log.iLog("[%z] [%q] File is empty !!! Sending a 404 back to the Client\n", Logarithm::NOTICE);
                            std::regex findFile("(404.html)");
                            filePath = iDirectory.ScanDir(findFile).at(0);
                            // Set the MIME Type
                            this->MIMETYPE = MimeType::HTML;
                            // Set the HTTP status code
                            httpCode = WebX::HTTPStatusCodes::NOT_FOUND;
                        }
                        else
                        {
                            // Detect the MIME Type via file extension + set the MIME type
                            this->MIMETYPE = GetMIMEType(filePath);

                            // Set the HTTP status code
                            httpCode = WebX::HTTPStatusCodes::OK;
                        }
                        break;
                    }
                }                
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

            // _Log.iLog("[%z] [%q] File Size: [%d]\n", Logarithm::NOTICE, fLength); // [DEBUG] Print

            // Init the buffer with the file length
            buffer = new char[fLength];
            memset(buffer, 0x00, fLength);
            // Read the file into the buffer
            ssFileReader.read(buffer, fLength);

            ssFileReader.close();

            // Add a null terminator
            // buffer[fLength] = '\n';
        }
        else
        {
            _Log.Log("Invalid File Stream", Logarithm::CRITICAL);        
            std::regex findFile("(204.html)");
            filePath = iDirectory.ScanDir(findFile).at(0);
            buffer = new char[iDirectory.GetFileSize(filePath)];
            memset(buffer, 0x00, iDirectory.GetFileSize(filePath));
            memcpy(buffer, iDirectory.ReadFile(filePath), iDirectory.GetFileSize(filePath));
            // Set the MIME Type
            this->MIMETYPE = MimeType::HTML;
            // Set the HTTP status code
            httpCode = WebX::HTTPStatusCodes::INTERNAL_SERVER_ERROR;
            ssFileReader.close();
        }
        

        _Log.Log("Returning the Buffer", Logarithm::CRITICAL);
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



        httpRes.statusCode = (int)httpCode;
        // httpRes.statusCode = 200;
        httpRes.reasonPhrase = "OKAY";
        // memcpy(&httpRes.httpGeneralHeader, (char*)httpGeneral, sizeof(httpGeneral));
        httpRes.httpEntityHeader = httpEntity;

        // Set the MIME Type correctly        
        // [TODO] Possibly break into a helper function that is smaller
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
        default:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/plain;";
            break;
        }

        httpRes.httpGeneralHeader = httpGeneral;

        httpRes.httpEntityHeader.contentLength = strlen(httpRes.ReturnHeader().c_str());

        // [TODO] Debug Printing
        // printf("\n\n%s\n\n", httpRes.ReturnHeader().c_str());

        return httpRes;
    }

    enum HTTP::MimeType HTTP::GetMIMEType(std::string filePath)
    {
        // [NOTE] This is the most basic and most likely the worst way of doing this
        std::string fExt = filePath.substr(filePath.rfind('.') + 1, filePath.length());
        _Log.iLog("[%z] [%q] Detected a MIME Type using file extension [%s]\n", Logarithm::NOTICE, fExt.c_str());
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
        _Log.iLog("[%z] [%q] Failed to Detected a MIME Type\n", Logarithm::CRITICAL);
        return MimeType::HTML;
    }
};