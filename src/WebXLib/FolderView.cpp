#include "FolderView.hpp"

namespace WebXO
{
    FolderView::FolderView()
    {}

    std::vector<string> FolderView::GetFiles(std::string folderPath)
    {
        Directory dir(folderPath);
        std::regex rgx("[^]*");
        auto files = dir.ScanDir(rgx, folderPath);

        this->currentFolder = folderPath.substr(folderPath.rfind('/'), folderPath.length());

        // // [DEBUG] Print
        // printf("\nPrinting all files in folder (%s)\n", folderPath.c_str());
        // for(auto file : files)
        // {
        //     printf("File Found: [%s]\n", file.c_str());
        // }

        return files;
    }

    std::string FolderView::GeneratePage(std::vector<string> files)
    {
        // Create a String Buffer to hold the generated html
        std::string page = this->HEADER;

        page += "<h1 style=\"text-align:center\">";
        page += this->currentFolder;
        page +="</h1>";

        // Inject Content
        for(auto file : files)
        {
            // [TODO] Add code here to remove the '.FolderViewer' file from the listing
            if(file.substr(file.rfind('/') + 1, file.length()) != std::string(".FolderViewer"))
            {
                // printf("File Raw %s, File HTTP Link %s\n", file.c_str(), file.substr(file.rfind('/', file.rfind('/') - 1), file.length()).c_str());
                page += "<li class=\"list-group-item\"><a href=\".";
                page += file.substr(file.rfind('/', file.rfind('/') - 1), file.length());
                // page +=  file.substr(file.rfind('/'), file.length());
                page += "\">";                
                page +=  file.substr(file.rfind('/') + 1, file.length());
                page +=  "</a>";
                page +=  "</li>";
            }
        }

        // Inject the footer
        page += this->FOOTER;

        // Return the buffer
        return page;
    }
};