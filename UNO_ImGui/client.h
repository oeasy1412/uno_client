#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
// #include <ranges>
// const char* convert_u8(const std::u8string& u8_str) {
//     // auto view = u8_str | std::views::transform([](char8_t c) -> char { return static_cast<char>(c); });
//     // return std::string(view.begin(), view.end()).c_str();
//     std::string str;
//     str.resize(u8_str.length());
//     std::transform(u8_str.begin(), u8_str.end(), str.begin(), [](char8_t c) -> char { return static_cast<char>(c); });
//     return str.c_str();
// }
void to_hello(std::string& information);
void login_to_server(std::string& information, std::string& p_name, std::string& my_str);
void to_get_start(std::string& information, bool& isStart);
void to_get_players(std::string& information, std::string& players);
void to_get_first(std::string& information);
void to_show_card(std::string& information, std::string& myCard);
void to_query_turn(std::string& information, std::string& my_str, bool& you_turn);
void to_query_add(std::string& information, std::string& query_str, bool& you_turn, bool& iterAdd);
void to_add(std::string& information, std::string& myCard, bool& iterAdd, int& ret);
void to_query_change(std::string& information);
void to_updata(std::string& information, std::string& myCard, int& ret);
void to_computer_update(std::string& information, std::string& my_str);
void to_call(std::string& information, std::string& p_name, std::string& p_msg);

#endif