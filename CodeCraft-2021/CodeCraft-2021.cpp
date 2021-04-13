/*
 * @Author: your name
 * @Date: 2021-04-04 19:30:29
 * @LastEditTime: 2021-04-13 15:59:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \planC\CodeCraft-2021\CodeCraft-2021.cpp
 */
#include "data.h"
unordered_map<string, SoldServer> server_info;
unordered_map<string, SoldVm> VM_info;
queue<vector<RequestData>> request_datas;
queue<vector<Game_result>> game_result;
int main() {
    freopen("training-0.txt", "r", stdin);
    Parse parse;
    parse.parse_input(server_info, VM_info, request_datas);
    //parse.print(server_info, VM_info, request_datas);
    return 0;
}
