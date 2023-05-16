#pragma once
#include<iostream>


namespace SystemConfig {
    bool DisplayImage(const std::string& Inputimage) {
        int result = system(("start " + Inputimage).c_str());
        if (result == 0) {
            std::cout << "successfully displayed Images...";
            return true;
        }
        else if (result == 1) {
            std::cout << "Could not display images...";
            return false;
        }
        return true;
    }
}