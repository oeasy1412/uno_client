#include "../thirdparty/httplib.h"

#include <iostream>
#include <string>
#include "map.h"
#include "def.h"

// #include <jsoncpp/json/json.h>1
enum Stage { Waitting, Ready, Racing };
extern Stage stage;
httplib::Client* client = nullptr;
#if DEBUG == 0
std::string server_address;
#else
std::string server_address = "172.16.2.173:25565";
#endif
// std::string p_name;         // 存放玩家昵称
// std::string my_str;         // 存放用户玩家信息
// std::string query_str = ""; // 存放询问服务端结果
// bool isStart = false;       // 判断游戏是否开始
// bool you_turn = false;      // 判断是否为自己的回合
// bool iterAdd = false;       // 判断能否迭代加加

// int playerColor;

// Hello服务端
void to_hello(std::string& information) {
    #if DEBUG == 0
    std::ifstream file("config.cfg");
    if(!file.good()){
        information += "启动失败！\n";
        exit(-1);
    }
    std::stringstream str_string;
    str_string << file.rdbuf();
    server_address = str_string.str();

    file.close();
    #endif

    httplib::Client client(server_address);
    httplib::Result result = client.Post("/hello");
    if (!result || result->status != 200) {
        information += "Hello Failed!\n";
        return;
    }
    information += result->body + '\n';
}

// 登入服务端
void login_to_server(std::string& information,
                     std::string& p_name,
                     std::string& my_str) {
    client = new httplib::Client(server_address);
    client->set_keep_alive(true);
    std::cout << "请输入玩家昵称：\n";
    std:: cin >> p_name;
    // 向服务端传输玩家昵称
    httplib::Headers headers{{"Name", p_name}};
    httplib::Result result = client->Post("/login", headers);
    if (!result || result->status != 200) {
        information += "无法连接到服务器！\n";
        exit(-1);
    }
    information += result->body + '\n';
    // 读入玩家为Player几
    for (int i = 8; i < 15; i++) {
        my_str += result->body[i];
    }
}

// 询问游戏是否开始
void to_get_start(std::string& information, bool& isStart) {
    httplib::Result result = client->Post("/get_start");
    if (!result || result->status != 200) {
        information += "游戏开始失败！无法连接到服务器！\n";
        exit(-1);
    }
    if (result->body == "1") {
        isStart = true;
    }
}

// 获取玩家信息
void to_get_players(std::string& information, std::string& players){
    httplib::Result result = client->Post("/get_players");
    if (!result || result->status != 200) {
        information += "获取玩家信息失败！无法连接到服务器！\n";
        exit(-1);
    }
    players = result->body;
}


// 获取第一张牌
void to_get_first(std::string& information) {
    httplib::Result result = client->Post("/get_first");
    if (!result || result->status != 200) {
        information += "游戏初始化失败！无法连接到服务器！\n";
        exit(-1);
    }
    information += result->body + '\n';
}

// 显示自己的手牌
void to_show_card(std::string& information, std::string& myCard) {
    httplib::Result result = client->Post("/show_card");
    if (!result || result->status != 200) {
        information += "卡片显示失败！无法连接到服务器！\n";
        exit(-1);
    }
    if (result != nullptr) {
        myCard = result->body;
    }
}

// 询问并更新是否为自己的回合
void to_query_turn(std::string& information,
                   std::string& my_str,
                   bool& you_turn) {
    httplib::Headers headers{{"Player", my_str}};
    httplib::Result result = client->Post("/query_turn", headers);
    if (!result || result->status != 200) {
        information += "询问失败！无法连接到服务器！\n";
        exit(-1);
    }
    // 更新
    you_turn = (int)result->body[0];
}
// 询问被加的状况
void to_query_add(std::string& information,
                  std::string& query_str,
                  bool& you_turn,
                  bool& iterAdd) {
    httplib::Result result = client->Post("/query_add");
    if (!result || result->status != 200) {
        information += "游戏运行添加错误！无法连接到服务器！\n";
        exit(-1);
    }
    // 共三种
    query_str = result->body;
    if (query_str == "s") {
        information += "You are Skipped!\n";
        you_turn = false;
    } else if (query_str == "t") {
        information += "You can play AddTwo!\n";
        iterAdd = true;
    } else if (query_str == "f") {
        information += "You can play Black4!\n";
        iterAdd = true;
    }
}

void to_query_change(std::string& information); // useless declaration
// 加牌
void to_add(std::string& information, std::string& myCard, bool& iterAdd, int& ret) {
    int x = 1;
    if (iterAdd) {
        x = ret;
        iterAdd = false;
    }
    httplib::Headers headers{{"Card", std::to_string(x)}};
    httplib::Result result = client->Post("/add");
    if (!result || result->status != 200) {
        information += "添加失败！无法连接到服务器！\n";
        exit(-1);
    }
    ret = -1;
    to_show_card(information, myCard);
    to_query_change(information);
}

// 询问并更新转色
void to_query_change(std::string& information) {
    httplib::Result result = client->Post("/query_change");
    if (!result || result->status != 200) {
        information += "游戏运行转色失败！无法连接到服务器！\n";
        exit(-1);
    }
    if (result->body == "w") {
        information += "What color?(1:红 2:绿 3:蓝 4:黄)\n";
        // 返回转色的值
        int x;
        std::cin >> x;
        if (x > 4 || x < 1) {
            x = 4;
        }
        httplib::Headers headers{{"Color", std::to_string(x)}};
        result = client->Post("/change_color", headers);
        if (!result || result->status != 200) {
            information += "转色失败！无法连接到服务器！\n";
            exit(-1);
        }
        information += "You change color into " + ToString[(char)(x - 1)] + '\n';
    }
}

// 玩家返回打出的手牌
void to_updata(std::string& information, std::string& myCard, int& ret) {
    httplib::Headers headers{{"Card", std::to_string(ret)}};
    httplib::Result result = client->Post("/updata", headers);
    if (!result || result->status != 200) {
        information += "发送失败！无法连接到服务器！\n";
        exit(-1);
    }
    information += result->body + '\n';
    ret = -1;
    // 询问转色
    to_show_card(information, myCard);
    to_query_change(information);
}

// 获取人机的输出状态
void to_computer_update(std::string& information, std::string& my_str) {
    httplib::Headers headers{{"Bool", my_str}};
    httplib::Result result = client->Post("/computer_update", headers);
    if (!result || result->status != 200) {
        information += "游戏更新失败！无法连接到服务器！\n";
        exit(-1);
    }
    if (!result->body.empty()) {
        information += result->body + '\n';
    }
}

void to_call(std::string& information, std::string& p_name, std::string& p_msg) {
    httplib::Headers headers{{"Name", p_name}, {"Message", p_msg}};
    httplib::Result result = client->Post("/call", headers);
    if (!result || result->status != 200) {
        information += "访问失败！无法连接到服务器！\n";
        exit(-1);
    }
    // music_qiu();
    p_msg.clear();
}




/*
int main(int argc, char** argv) {
    // 第一次握手
    to_hello(information);
    // 登入服务端
    login_to_server(information);
    // 持续与服务端交互
    while (true) {
        if (stage == Stage::Waitting) {
            // 询问游戏是否开始
            to_get_start(information);
            if (!isStart) {
                continue;
            } else {
                stage = Stage::Ready;
                // 获取第一张牌
                to_get_first(information);
            }
        }

        // 询问并更新是否为自己的回合
        to_query_turn(information);
        // 获取人机的输出状态
        to_computer_update(information);
        if (you_turn) {
            to_query_add(information);
            if (!query_str.empty()) {
                to_add(information);
            } else {
                to_show_card(information);
                to_updata(information);
            }
            query_str.clear(); // 归零
            you_turn = false;  // 不是自己的回合
        }
    }

    return 0;
}
*/

// x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++
// -lpthread -Wl,-Bdynamic client.cpp -o uno.exe -lws2_32