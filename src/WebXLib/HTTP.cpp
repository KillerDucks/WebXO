#include "HTTP.hpp"

namespace WebX
{
    HTTP::HTTP()
    {}
    HTTP::~HTTP()
    {}

    HTTP::HTTPReq HTTP::ParseRequest(char* request)
    {
        vector<string> reqOpts;
        string t;
        HTTP::HTTPReq hRequest;
        memset(&hRequest, 0x00, sizeof(hRequest));

        // Look through the request and fill in the struct
        for (size_t i = 0; i < strlen(request); i++)
        {
            // Convert the buffer into a managable sting vector
            if(request[i] == '\n')
            {
                t += '\0';
                reqOpts.push_back(t);
                t.clear();
            }
            else
            {
                t += request[i];
            }
            
        }

        // The first line will be the requestType
        // memcpy(hRequest.requestType, string(reqOpts.at(0), 0, reqOpts.at(0).find(' ')).c_str(), reqOpts.at(0).find(' '));

        bool x = true;
        vector<string> tVec;
        for(auto opt : reqOpts)
        {
            if(x)
            {
                x = false;
                continue;
            }

            if(opt.length() > 2)
            {
                string tmp;
                tmp = opt.substr(opt.find(':') + 2, opt.length());
                tVec.push_back(tmp);

                // reqOpts.pop_back();
                // printf("%s\n", tmp.c_str());
            }
        }

        // for(auto o : tVec)
        // {
        //     printf("%s\n", o.c_str());
        // }
        // hRequest = &tVec[0];
        // Fuck it hardcode the assignments
        string tmp;
        tmp = reqOpts.at(0).substr(0, reqOpts.at(0).find(' '));
        hRequest.requestType = new char[250];
        memcpy(hRequest.requestType,                   tmp.c_str(),  strlen(tmp.c_str()));
        memcpy(&hRequest.host,                          tVec.at(0).c_str(), strlen(tVec.at(0).c_str()));
        memcpy(hRequest.connection,                    tVec.at(1).c_str(), sizeof(tVec.at(1)));
        // memcpy(hRequest.dnt,                           tVec.at(2).c_str(), sizeof(tVec.at(2)));
        hRequest.dnt =                                 std::stoi(tVec.at(2));
        hRequest.upgrade_insecure_requests =           std::stoi(tVec.at(3));
        // memcpy(hRequest.upgrade_insecure_requests,     (int)stoi(tVec.at(3).data()), sizeof(tVec.at(3)));
        memcpy(hRequest.user_Agent,                    tVec.at(4).c_str(), sizeof(tVec.at(4)));
        memcpy(hRequest.sec_Fetch_User,                tVec.at(5).c_str(), sizeof(tVec.at(5)));
        memcpy(hRequest.accept,                        tVec.at(6).c_str(), sizeof(tVec.at(6)));
        memcpy(hRequest.sec_Fetch_Site,                tVec.at(7).c_str(), sizeof(tVec.at(7)));
        memcpy(hRequest.sec_Fetch_Mode,                tVec.at(8).c_str(), sizeof(tVec.at(8)));
        memcpy(hRequest.accept_Encoding,               tVec.at(9).c_str(), sizeof(tVec.at(9)));
        memcpy(hRequest.accept_Languages,              tVec.at(10).c_str(), sizeof(tVec.at(10)));

        printf("%s\n", hRequest.requestType);
        
        return hRequest;
    }
};