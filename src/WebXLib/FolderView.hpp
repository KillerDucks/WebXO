#pragma once

#include <regex>

#include "Common.hpp"

#include "Directory.hpp"

#ifndef WEBXO_FOLDERVIEW_H
#define WEBXO_FOLDERVIEW_H

namespace WebXO
{
    class FolderView
    {
    private:
        const std::string HEADER = " \
            <!doctype html> \
                <html lang=\"en\"> \
                <head> \
                    <title>FolderView v1.0.0</title> \
                    <!-- Required meta tags --> \
                    <meta charset=\"utf-8\"> \
                    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"> \
                    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\"> \
                </head> \
                <body> \
                <div class=\"jumbotron\"><h1 class=\"display-3\">Folder Listing</h1></div> \
                <div class=\"container\"><ul class=\"list-group list-group-flush\"> \
        ";

        const std::string FOOTER = " \
                    </ul></div> \
                    <!-- Optional JavaScript --> \
                    <!-- jQuery first, then Popper.js, then Bootstrap JS --> \
                    <script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script> \
                    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script> \
                    <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script> \
                </body> \
            </html> \
        ";
    public:
        FolderView();
        ~FolderView() = default;

        // Get Files
        std::vector<string> GetFiles(std::string folderPath);

        // Generate Page
        std::string GeneratePage(std::vector<string> files);

        // inline detector
        static bool isFolderViewer(std::string filePath)
        {
            Directory dir(filePath);
            std::regex rgx(".FolderViewer");
            auto files = dir.ScanDir(rgx, filePath);
            if(files.at(0) == std::string("-1"))
            {
                return false;
            }
            return true;
        }
    };
};

#endif //WEBXO_FOLDERVIEW_H