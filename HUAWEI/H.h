#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include <cmath>
#include <unordered_set>
using namespace std;

//#define TEST_PARSEINPUT
//#define REDIRECT

struct SoldServer {   
    string server_name;
    int cpu_cores;
    int memory_size;
    int hardware_cost;
    int daily_energy_cost;
};
struct SoldVm {
    //string VM_name;
    int cpu_cores;
    int memory_size;
    int deployment_way;
};
struct RequestData {
    string operation;
    string vm_name;
    int vm_id;
};
struct PurchasedServer {
    string server_name;
    int server_id;
    int total_core_num;
    int total_memory_size;
    int A_remain_core_num;
    int A_remain_memory_size;
    int B_remain_core_num;
    int B_remain_memory_size;
    bool can_deploy_A = false;
    bool can_deploy_B = false;
    unordered_set<int> A_vm_id;
    unordered_set<int> B_vm_id;
    unordered_set<int> AB_vm_id;
};
struct VmIdInfo {
    PurchasedServer* purchase_server;
    string node;  //A 代表A节点  B代表B节点  C代表C节点
    string vm_name;
    int cpu_cores;
    int memory_size;
};
struct AddData {
    AddData() = default;
    AddData(int _deployment_way, int _cpu_cores, int _memory_size, int _vm_id, string _vm_name) : 
            deployment_way(_deployment_way), cpu_cores(_cpu_cores), memory_size(_memory_size), vm_id(_vm_id), vm_name(_vm_name) {}
    int deployment_way;
    int cpu_cores;
    int memory_size;
    int vm_id;
    string vm_name;
};
class Evaluate {
private: 
    const double threshold1 = 4;

    const double threshold_abs1 = 10;
    const double threshold_abs2 = 3;
    bool check1(double ratio_a, double ratio_b) {
        if (ratio_a >= 1) {
            if (fabs(ratio_a - ratio_b) > threshold_abs1) {
                return false;
            }
        } else {
            if (fabs(1 / ratio_a - 1 / ratio_b) > threshold_abs1) {
                return false;
            }
        }
        return true;
    }
    bool check2(double ratio_a, double ratio_b) {
        if (ratio_a >= 1) {
            if (fabs(ratio_a - ratio_b) > threshold_abs1) {
                return false;
            }
        } else {
            if (fabs(1 / ratio_a - 1 / ratio_b) > threshold_abs1) {
                return false;
            }
        }
        return true;
    }
public:
    bool PurchasedServerAB(PurchasedServer* purchased_server, int cpu_cores, int memory_size) {  //评价要不要插到双节点
        if (purchased_server->A_remain_core_num >= cpu_cores && purchased_server->A_remain_memory_size >= memory_size
            && purchased_server->B_remain_core_num >= cpu_cores && purchased_server->B_remain_memory_size >= memory_size){
                return true;
            }
        return false;
    }
    bool PurchasedServerA(PurchasedServer* purchased_server, int cpu_cores, int memory_size) {  //评价要不要插到A节点
        if (purchased_server->A_remain_core_num >= cpu_cores && purchased_server->A_remain_memory_size >= memory_size) {
            /*double ratio = 1.0 * purchased_server->A_remain_core_num / purchased_server->A_remain_memory_size;
            if (purchased_server)
            if (ratio > 1 && ratio < 10) {
                if (1.0 * (purchased_server->A_remain_core_num - cpu_cores) / (purchased_server->A_remain_memory_size - memory_size) > 20) {
                    return false;
                }
                if (1.0 * (purchased_server->A_remain_core_num - cpu_cores) / (purchased_server->A_remain_memory_size - memory_size) < 1 / 20) {
                    return false;
                }
            }
            if (ratio <= 1 && 1.0 / ratio < 10) {
                if (1.0 / (purchased_server->A_remain_core_num - cpu_cores) * (purchased_server->A_remain_memory_size - memory_size) > 20) {
                    return false;
                }
                if (1.0 * (purchased_server->A_remain_core_num - cpu_cores) / (purchased_server->A_remain_memory_size - memory_size) < 1 /20) {
                    return false;
                }
            }*/
            return true;
        }
        return false;
    }
    bool PurchasedServerB(PurchasedServer* purchased_server, int cpu_cores, int memory_size) {  //评价要不要插到B节点
        if (purchased_server->B_remain_core_num >= cpu_cores && purchased_server->B_remain_memory_size >= memory_size) {
            /*double ratio = 1.0 * purchased_server->B_remain_core_num / purchased_server->B_remain_memory_size;
            if (ratio > 1 && ratio < 10) {
                if (1.0 * (purchased_server->A_remain_core_num - cpu_cores) / (purchased_server->A_remain_memory_size - memory_size) > 20) {
                    return false;
                }
            }
            if (ratio <= 1 && 1.0 / ratio < 10) {
                if (1.0 / (purchased_server->A_remain_core_num - cpu_cores) * (purchased_server->A_remain_memory_size - memory_size) > 20) {
                    return false;
                }
            }*/
            return true;
        }
        return false;
    }
};
class Cmp {
public:
    static bool SoldServers (SoldServer& a, SoldServer& b) {
        return a.hardware_cost < b.hardware_cost;
    }
    /*static bool SoldServers (SoldServer& a, SoldServer& b) {
        double x = a.hardware_cost
        return a.hardware_cost < b.hardware_cost;
    }*/
    static bool ContinuousADD (AddData& a, AddData& b) {
        /*if (a.deployment_way != b.deployment_way) {
            return a.deployment_way > b.deployment_way;
        } else {
            return (a.cpu_cores + a.memory_size) > (b.cpu_cores + b.memory_size);
        }*/
        return (a.cpu_cores + a.memory_size) * (a.deployment_way + 1) > (b.cpu_cores + b.memory_size) * (b.deployment_way + 1);
    }
    static bool CanDeploy (PurchasedServer* a, PurchasedServer* b) {
        int use_ratio_a = (a->A_remain_core_num + a->A_remain_memory_size + a->B_remain_core_num + a->B_remain_memory_size) * 1.0 / (a->total_core_num + a->total_memory_size) * 2;
        int use_ratio_b = (b->A_remain_core_num + b->A_remain_memory_size + b->B_remain_core_num + a->B_remain_memory_size) * 1.0 / (b->total_core_num + b->total_memory_size) * 2;
        return use_ratio_a > use_ratio_b;
    }
};