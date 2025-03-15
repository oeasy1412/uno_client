#include "client.h"

#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>


std::unordered_map<char, std::string> ToString = {
    {'0', "红"},
    {'1', "绿"},
    {'2', "蓝"},
    {'3', "黄"},
    {'4', "Black"},
    {'5', "Skip"},
    {'6', "AddTwo"},
    {'7', "Reverse"},
    {'8', "BlackC"},
    {'9', "Black4"},
    {'n', "Number"},
};

int main() {
    system("chcp 65001");
    std::cout << "testing...\n";
    // 
    std::cout << "test_update Ok\n";
    return 0;
}