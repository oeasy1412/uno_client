#include "client.h"

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<char, string> ToString = {
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
    cout << "testing...\n";
    string information;
    to_hello(information);
    assert(information == "Hello From Server!\n" && "Error in test_hello");

    string name, myIdx;
    login_to_server(information, name, myIdx);
    assert(!name.empty() && myIdx[6] == '1');

    bool isStart = false;
    to_get_start(information, isStart);
    assert(isStart == true && "check whether the max_player equal to 1 or not");

    string players;
    to_get_players(information, players);
    cout << players << '\n';

    to_get_first(information);
    cout << information << '\n';

    string myCard;
    to_show_card(information, myCard);
    assert(!myCard.empty());

    cout << "\ntest_hello Ok\n";
    return 0;
}