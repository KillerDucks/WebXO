#include "HTTP.hpp"

namespace WebXO
{
    HTTP::HTTP(std::string httpPath, InterceptSettings interceptSettings) : _Log("HTTP"), iDirectory(httpPath), vHosts("./vhosts.txt"), httpCode(WebXO::HTTPStatusCodes::OK), _interceptionSettings(interceptSettings), MIMETYPE(MimeType::HTML)
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

        // Detect any data left over (possible POST data)
        if(!t.empty())
        {
            std::string tmp;
            // Data still exists, parse the data
            for(char c : t)
            {
                if(c != '&')
                {
                    tmp += c;
                }
                else
                {
                    hRequest.form_data.insert(std::pair<std::string, std::string>(tmp.substr(0, tmp.find('=')), tmp.substr(tmp.find('=') + 1, tmp.size())));
                    tmp.clear();
                }
                
            }

            if(!tmp.empty())
            {
                hRequest.form_data.insert(std::pair<std::string, std::string>(tmp.substr(0, tmp.find('=')), tmp.substr(tmp.find('=') + 1, tmp.size())));
                tmp.clear();
            }

            // [DEBUG] Print
            // printf("\n\nFORM DATA CAPTURED !!!\n");
            // for(auto d : hRequest.form_data)
            // {
            //     printf("Key %s, Value: %s\n", d.first.c_str(), d.second.c_str());
            // }
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

    CompBuffer HTTP::GetRequestedFile(HTTPReq hReq)
    {
        // Scratch Variables
        std::string filePath;
        std::string urlData;                        // [NOTE] This should be put through a decoder to remove HTTP special chars

        // Interception Hooking [DEBUG] [NOTE] Might be moved to a different place
        std::tuple<HTTPReq, CompBuffer> test = _interception.HookSync(hReq, _interceptionSettings.callback);
        if(std::get<0>(test).requestType == std::string("-1"))
        {
            // This interception is a direct buffer
            if(this->AcceptDeflate(hReq.accept_Encoding))
            {
                // Compress The interception buffer
                printf("File to Compress [DEFLATE]: %s\n", filePath.c_str());

                return Compression::DeflateBuffer(std::get<1>(test).first, (size_t)std::get<1>(test).second); 
            }
            else
            {
                return std::get<1>(test);
            }            
        }
        // This will update the HTTP Request based off the Interception callback [TODO] Possible make this conditional
        hReq = std::get<0>(test);

        // printf("RETURNED INTERCEPTION VALUE [%s] AND SIZE [%d]\n", std::get<1>(test).first, std::get<1>(test).second);

        // Virtual Hosts Redirection
        std::string vQuery = this->vHosts.Query(hReq.host.substr(0, hReq.host.find(':')));
        // printf("\nQuerying %s: %s\n", hReq.host.substr(0, hReq.host.find(':')).c_str(), this->vHosts.Query(hReq.host.substr(0, hReq.host.find(':'))).c_str());   // [DEBUG] Print

        printf("Method: [%s]\tFile [%s]\n", hReq.method().c_str(), hReq.file().c_str());

        // Get the Method
        auto it = HTTPMethodTypesTable.find(hReq.method());
        if (it != HTTPMethodTypesTable.end()) {
            this->httpMethod = it->second;
        } else { printf("\nHTTP Method Deduction ERROR !!!\n"); }

        // [DEBUG] [NOTE] This should be removed after testing
        switch (this->httpMethod)
        {
        case HTTPMethodTypes::GET:
            break;
        case HTTPMethodTypes::POST:
            // Check if we have any form data to handle
            if(!hReq.form_data.empty())
            {
                // Handle the data
                printf("\n[Form Data]:\n");
                for(auto kVal : hReq.form_data)
                {
                    printf("\tKey [%s]\tValue [%s]\n", kVal.first.c_str(), kVal.second.c_str());
                }
                printf("\n");
            }
            break;
        case HTTPMethodTypes::HEAD:
            break;

        default:
            break;
        }

        // Get the web path request needed from the HTTP Request
        std::string file(hReq.file());

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
        std::string parentPath(iDirectory.GetBasePath());

        if(vQuery != "-1")
        {
            relativePath += vQuery;
            parentPath += vQuery;
            parentPath += file;            
        }
        else
        {     
            parentPath += file;
        }

        // [TEST] [DEBUG] Relative Path FIX
        // First see if there is a '.' and assume if there is then its requesting a direct file
        if(file.find('.') == std::string::npos)
        {
            // if so then assume this is a path and append on a '/' to allow the Directory Component to get the index.html
            // Check that the last character isn't a '/'
            // [DEBUG] [PRINT]
            // printf("\nLocation of last '/' and the string length [%ld] / [%ld]\n", file.rfind('/'), file.length());
            if(file.rfind('/') != file.length() - 1 && file.rfind('/') != std::string::npos)
            {
                relativePath += file;
                relativePath += "/";
            }
            if(file.length() && file.rfind('/') == file.length() - 1)
            {
                relativePath += file;
            }
        }
        else
        {
            relativePath += file;
        }
        

        {
        // Debug Logging [REMOVE]
        // printf("requested accept: %s\n", hReq.accept.c_str());
        // printf("requested accept encodings: %s\n", hReq.accept_Encoding.c_str());
        // this->AcceptDeflate(hReq.accept_Encoding);
        // std::terminate();
        // printf("\nrequested host: %s requested file: %s\n", hReq.host.c_str(), file.c_str());
        // printf("requested file: %s\n", file.c_str());
        // printf("parent path: %s\n", parentPath.c_str());
        // printf("relative path: %s\n", relativePath.c_str());
        }
        

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
                // Check if the folder is a FolderView folder
                FolderView fView;
                if(fView.isFolderViewer(relativePath.substr(0, relativePath.rfind('/'))))
                {
                    // This is a compatable folder
                    std::string buffer = fView.GeneratePage(fView.GetFiles(relativePath.substr(0, relativePath.rfind('/'))));

                    httpCode = WebXO::HTTPStatusCodes::OK;
                    this->MIMETYPE = MimeType::HTML;
                    
                    // Verbose Logging
                    _Log.Log("Returning the Buffer", Logarithm::NOTICE);

                    printf("File to Compress [DEFLATE]: FolderView Dynamic Buffer\n");

                    return Compression::DeflateBuffer((char*)buffer.c_str(), buffer.size()); 
                }
                // There is no '.' found, assume this is a root path of a sub/directory
                std::regex rgx("(index.html)");
                // Log for debugging
                _Log.Log("Root path detected", Logarithm::INFO);
                // Set the MIME Type
                this->MIMETYPE = MimeType::HTML;
                // Scan the directories for this file
                filePath = iDirectory.ScanDir(rgx, relativePath.substr(0, relativePath.rfind('/')) ).at(0); // [CHANGE] Assume this is the first given file
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
                filePath += ERROR_PAGE_DIR;
                filePath += "/404.html";
                break;
            }
        case WebXO::HTTPStatusCodes::INTERNAL_SERVER_ERROR:            
            {
                filePath.clear();
                filePath += ERROR_PAGE_DIR;
                filePath += "/500.html";
                break;
            }
        case WebXO::HTTPStatusCodes::NO_CONTENT:            
            {
                filePath.clear();
                filePath += ERROR_PAGE_DIR;
                filePath += "/204.html";
                break;
            }        
        default:
            break;
        }

        // Final Checks to see if we have a valid file path
        if(filePath == "-1")
        {
            // Invalid file
            filePath.clear();
            filePath += ERROR_PAGE_DIR;
            filePath += "/500.html";

            // Set the MIME Type
            this->MIMETYPE = MimeType::HTML;         
            // Set the HTTP status code
            httpCode = WebXO::HTTPStatusCodes::INTERNAL_SERVER_ERROR; 
        }
        
        // [HIGH] The file should not be openned here, abstract this out
        if(!this->AcceptDeflate(hReq.accept_Encoding))
        {
            return IO::ReadFile(filePath);
        }
        
        printf("File to Compress [DEFLATE]: %s\n", filePath.c_str());

        return Compression::DeflateFile(filePath); 
    }

    HTTPRes HTTP::GenerateHTTPResponse(int contentLength, HTTPReq hReq)
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
        switch (this->httpCode)
        {
        case HTTPStatusCodes::OK:
            httpRes.reasonPhrase = "OK";
            break;
        case HTTPStatusCodes::NOT_FOUND:
            httpRes.reasonPhrase = "Not Found";
            break;
        case HTTPStatusCodes::NO_CONTENT:
            httpRes.reasonPhrase = "No Content";
            break;
        case HTTPStatusCodes::INTERNAL_SERVER_ERROR:
            httpRes.reasonPhrase = "Internal Server Error";
            break;
        // Let all other Status Codes fall through
        default:
            break;
        }

        //  Set the HTTP Entity Header (ignore the cast issue, will be looked into later on)
        httpRes.httpEntityHeader = httpEntity;

        // Set the MIME Type correctly        
        // [TODO] Possibly break into a helper function that is smaller [NOTE] This is really dumb I should consolidate this with HTTP::GetMINEType
        switch (this->MIMETYPE)
        {
        case MimeType::HTML:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/html";
            break;
        case MimeType::CSS:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/css";
            break;
        case MimeType::JS:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/javascript";
            break;
        case MimeType::IMAGE:
            httpRes.httpEntityHeader.contentType = "Content-Type: image/jpeg";
            break;
        case MimeType::VIDEO:
            httpRes.httpEntityHeader.contentType = "Content-Type: video/mp4";
            break;
        default:
            httpRes.httpEntityHeader.contentType = "Content-Type: text/plain";
            break;
        }

        httpRes.httpGeneralHeader = httpGeneral;

        // [NOTE] This should not be hardcoded to deflate, this should work with the Client Ranges
        if(this->AcceptDeflate(hReq.accept_Encoding))
        {
            httpRes.httpEntityHeader.contentEncoding = "Content-Encoding: deflate";
        }
        else
        {
            httpRes.httpEntityHeader.contentEncoding = "";
        }

        // This is in octal form so the value will need to be converted
        httpRes.httpEntityHeader.contentLength += std::to_string(contentLength);

        // [DEBUG] Debug Printing
        // printf("!!\n\n%s\n\n!!", httpRes.ReturnHeader().c_str());

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
        if(fExt == std::string("mp4"))
        {
            _Log.iLog("[%z] [%q] Detected a MIME Type of [%s]\n", Logarithm::NOTICE, "MP4");
            return MimeType::VIDEO;
        }
        // [TODO] Add favicon support

        _Log.iLog("[%z] [%q] Failed to Detected a MIME Type\n", Logarithm::CRITICAL);
        return MimeType::HTML;
    }

    bool HTTP::AcceptDeflate(std::string accept_encodings)
    {
        // Check if deflate is present
        if(accept_encodings.find("deflate", 0) != std::string::npos)
        {               
            return true;            
        }
        return false;
    }
};