#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define x first
#define y second
using namespace std;

void wa() {
    exit(43);
}
void accept() {
    exit(42);
}

// TODO:
int numTestcase = 20;
string msg;
void readInput(ifstream &inFile);
void readAns(ifstream &ansFile);
void readUser();
bool isInt(string &str);
int s2i(string str);
vector<int> ss2vector(string &s);
vector<string> ss2string(string &s);

// input variable
// #Switches #Links #FlowPairs #Labels
unsigned int numSwitches, numLinks, numFlowPairs, numLabels;
// map LinkID Node1 Node2 Capacity

struct flow_data {  // store the next node, link_num, fidelity
        // unsigned int linkID;
        int srcID, dstID;
        int flowSize;
        vector<int> labels;
};
map<pair<int, int>, int> input_links;   // input[{node1, node2}] = capacity
map<pair<int, int>, int> residual_cap;  // input[{node1, node2}] = capacity

map<pair<int, int>, int> tmp_cnt_cap;  // need to be later
map<int, flow_data> input_flows;
map<pair<int, int>, int> baseline_routingTable;  // stores baseline routing table

int checker_acceptedFlow = 0;

// nw user variable
map<int, flow_data> user_flows;
int user_acceptedFlow = 0;
int user_acceptedSDpair = 0;

// read input in .in
void readInput(ifstream &inFile) {
    inFile >> numSwitches >> numLinks >> numFlowPairs >> numLabels;
    for (int i = 0; i < numLinks; i++) {
        int linkID, node1, node2, capacity;
        inFile >> linkID >> node1 >> node2 >> capacity;
        input_links[make_pair(node1, node2)] = capacity;
        input_links[make_pair(node2, node1)] = capacity;
        residual_cap[make_pair(node1, node2)] = capacity;
        residual_cap[make_pair(node2, node1)] = capacity;
    }

    for (int i = 0; i < numFlowPairs; i++) {
        int flowID, src, dst, flowSize;
        inFile >> flowID >> src >> dst >> flowSize;
        input_flows[flowID].srcID = src;
        input_flows[flowID].dstID = dst;
        input_flows[flowID].flowSize = flowSize;
    }
    return;
}
// read baseline in .ans
void readAns(ifstream &ansFile) {
    // check this is first case or last case;
    // read baseline routing table
    // curID, DstID, NextID
    string s;
    int curID = 0;
    ansFile >> msg;
    cerr << "msg: " << msg << endl;
    getline(ansFile, s);
    int cnt = 0;
    // cerr << "#numSwitches" << numSwitches << endl;
    while (curID < numSwitches) {
        getline(ansFile, s);
        // cout << curID << " " << numSwitches << endl;
        // cerr << "ans output: " << s << endl;
        vector<int> ansOutput = ss2vector(s);
        if ((int)ansOutput.size() == 1) {
            curID++;
        }
        for (int i = 0; i < numSwitches - 1; i++) {
            getline(ansFile, s);
            vector<int> ansOutput1 = ss2vector(s);
            if (ansOutput1.size() != 2) {
                cerr << "ans output size: " << ansOutput1.size() << endl;
                wa();
            }
            baseline_routingTable[make_pair(curID - 1, ansOutput1[0])] = ansOutput1[1];
        }
    }
    return;
}

// read user output
void readUser() {
    try {
        int lastNodeID = 0;
        string s;

        // routing table first
        // WIP: can be prettier here
        // I think it's better to traverse all the item in baseline_rouingTable to check if all of them are the same
        int curID = 0;
        while (curID < numSwitches) {
            // cerr << "i in switch: " << i << endl;
            getline(cin, s);
            // cerr << "user output: " << s << endl;

            vector<int> userOutput = ss2vector(s);

            if ((int)userOutput.size() == 0) {
                cerr << "No output from user to show routing table !" << endl;
                wa();
            } else if ((int)userOutput.size() == 1) {
                curID++;
                // cerr << "i++" << endl;
                for (int i = 0; i < numSwitches - 1; i++) {
                    getline(cin, s);
                    // cout << s << endl;
                    vector<int> userOutput1 = ss2vector(s);
                    if (userOutput1.size() != 2) {
                        cerr << "user output format error!" << endl;
                        cerr << "user output size: " << userOutput1.size() << endl;
                        wa();
                    }
                    if (baseline_routingTable[make_pair(curID - 1, userOutput1[0])] != userOutput1[1]) {
                        cout << baseline_routingTable[make_pair(curID - 1, userOutput1[0])] << " " << userOutput1[1] << endl;
                        cerr << "user routing table is not same as baseline" << endl;
                        wa();
                    }
                }
            }
        }

        getline(cin, s);
        vector<int> userOutput0 = ss2vector(s);
        user_acceptedSDpair = userOutput0[0];
        // cout << user_acceptedSDpair << endl;

        // flow table
        int i = 0;

        while (i < user_acceptedSDpair) {
            // cerr << "i in flow pair: " << i << endl;
            getline(cin, s);
            vector<int> userOutput = ss2vector(s);
            // cerr << "user output: " << s << endl;
            if ((int)userOutput.size() == 0) {
                cerr << " No output from user to show their choosed flow !" << endl;
                wa();
            } else if ((int)userOutput.size() == 1) {
                i++;
                string s1;
                int flowID = userOutput[0];
                getline(cin, s1);
                userOutput = ss2vector(s1);
                int len = userOutput.size();
                if (len > numLabels + 1) {
                    cerr << "user print out labels more than #labelNum at flow ID" << i - 1 << endl;
                    wa();
                } else {
                    user_flows[flowID].srcID = userOutput[0];
                    user_flows[flowID].dstID = userOutput[len - 1];
                    user_flows[flowID].flowSize = input_flows[flowID].flowSize;  // user need not output
                    user_flows[flowID].labels = vector<int>(userOutput.begin() + 1, userOutput.end());
                    user_acceptedFlow += input_flows[flowID].flowSize;
                }
            } else {
                cerr << "user output size: " << userOutput.size() << endl;
                cerr << "user output format error!" << endl;
                wa();
            }
        }

        // total flow size
        getline(cin, s);
        vector<int> userOutput = ss2vector(s);
        if ((int)userOutput.size() != 1) {
            cerr << "user did not print out total flow size" << endl;
            wa();
        }

        // finish reading user output, now check if the flow is accepted
        // check if user's flow can traverse to the destination

        int acceptedFlows = 0;
        // traverse all the flows
        for (auto &[flowId, flow] : user_flows) {
            int cur = flow.srcID;
            int flowSize = flow.flowSize;
            // cout << "flowSize: " << flowSize << endl;
            vector<int> labels = flow.labels;
            // record all the links needed in this certain flow
            // after that, check how many flow in these links can afford
            // check if the flow is accepted

            for (auto curDst : labels) {
                while (cur != curDst) {
                    int next = baseline_routingTable[make_pair(cur, curDst)];

                    residual_cap[make_pair(cur, next)] -= flowSize;
                    residual_cap[make_pair(next, cur)] -= flowSize;

                    if (residual_cap[make_pair(cur, next)] < 0 || residual_cap[make_pair(next, cur)] < 0) {
                        cerr << "user flow exceed the capacity" << endl;
                        wa();
                    }
                    cur = next;
                }
            }
            acceptedFlows += flowSize;
        }
        // cout << user_acceptedFlow << endl;
        if (acceptedFlows != user_acceptedFlow) {
            // cout << acceptedFlows << " " << user_acceptedFlow << endl;
            cerr << "checker did not accept same flow amount as user" << endl;
            wa();
        }

    } catch (const runtime_error &e) {
        cerr << "run error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << "sys error: " << e.what() << endl;
    }
}
// user_output: stdin
// .ans: argv[2]
// .in: argv[1]

int main(int argc, char *argv[]) {
    ifstream ans_file, in_file;
    in_file.open(argv[1]);
    ans_file.open(argv[2]);

    readInput(in_file);

    readAns(ans_file);

    readUser();

    // record all testcase output
    if (msg == "firstCase") {
        // cerr << "first case pass" << endl;
        ofstream total;
        total.open("/tmp/judge_output", ios::out);  //
        total.close();

        total.open("/tmp/judge_checker_count", ios::out);
        total << 1 << endl;
        total.close();
    } else if (msg == "lastCase") {
        ifstream all_result, checker_file;
        // checker
        int checker_cnt = 0, tmp;
        all_result.open("/tmp/judge_checker_count");
        while (all_result >> tmp)
            checker_cnt += tmp;
        all_result.close();

        // sum all number of path
        int user_singal_case = 0, user_total_sum = 0;
        checker_file.open("/tmp/judge_output");
        while (checker_file >> user_singal_case)
            user_total_sum += user_singal_case;
        checker_file.close();

        if (checker_cnt == numTestcase) {
            cerr << "Checker PASS !!" << endl;
            cerr << " success probility: " << user_total_sum << endl;  // how to check this??????
        } else {
            cerr << "Checker Fail !! Please submit code again." << endl;
        }
    } else {
        cerr << "no msg!!" << endl;
        // wa();
        ofstream total;
        total.open("/tmp/judge_output", ios::app);
        total.close();

        total.open("/tmp/judge_checker_count", ios::app);
        total << 1 << endl;
        total.close();
    }

    accept();
    return 0;
}

bool isInt(string &str) {
    if (str.size() >= 10) return false;
    for (auto c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int s2i(string str) {
    if (!isInt(str)) {
        cerr << "node's id isn't number " << str << endl;
        wa();
    }
    return stoi(str);
}

vector<int> ss2vector(string &s) {
    vector<int> res;
    stringstream ss;
    ss << s;
    try {
        string tmp;
        while (ss >> tmp) {
            res.push_back(s2i(tmp));
        }
        return res;
    } catch (const runtime_error &e) {
        cerr << "(str2vector) run error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << "(str2vector) sys error: " << e.what() << endl;
    }
    return res;
}

vector<string> ss2string(string &s) {
    vector<string> res;
    stringstream ss;
    ss << s;
    try {
        string tmp;
        while (ss >> tmp) {
            res.push_back(tmp);
        }
        return res;
    } catch (const runtime_error &e) {
        cerr << "(str2vector) run error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << "(str2vector) sys error: " << e.what() << endl;
    }
    return res;
}
