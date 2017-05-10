#ifndef BASENAME_HPP
#define BASENAME_HPP

#include <vector>
#include <iostream>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
char delim = '/';
#else
typedef unsigned int uint;
char delim = '\\';
#endif
namespace Basename {
    std::vector<std::string> split_string(std::string str) {
        std::vector<std::string> vec;
        std::string temp = "";
        for (uint i = 0; i < str.length(); i++) {
            if (str[i] == delim){
                if (temp != ""){
                    vec.push_back(temp);
                    temp = "";
                }
                continue;
            }
            temp += str[i];
        }
        if (temp != ""){
            vec.push_back(temp);
            temp = "";
        }
        return vec;
    }

    std::string basename(std::string path) {
        auto vec = split_string(path);
        return vec.at(vec.size()-1);
    }
}

#endif // BASENAME_HPP
