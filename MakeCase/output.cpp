#include <iostream>
#include <fstream>
using namespace std;

const int N = 7;

void open_file(ifstream &finput, ofstream &foutput, int t_case) {
    string dir_path = "../data/secret/";
    
    string input_name = to_string(t_case) + ".in";
    string input_path = dir_path + input_name;

    string output_name = to_string(t_case) + ".ans";
    string output_path = dir_path + output_name;

    finput.open(input_path, ios::in);
    foutput.open(output_path, ios::out | ios::trunc);
    if(!finput) {
        cerr << "cannot open input file " << t_case << endl;
        exit(1);
    }
    if(!foutput) {
        cerr << "cannot open output file " << t_case << endl;
        exit(1);
    }
}

void solve(ifstream &finput, ofstream &foutput, int t_case) {
    cerr << "start generating " << t_case << ".ans" << endl;
    int n;
    finput >> n;

    int mx_val = -1, mx_index = -1;
    for(int i = 1; i <= n; i++) {
        int num;
        finput >> num;
        if(mx_val < num) {
            mx_val = num;
            mx_index = i;
        }
    }

    foutput << mx_val << " " << mx_index << endl;
}
void close_file(ifstream &finput, ofstream &foutput, int t_case) {
    finput.close();
    foutput.close();
}
int main() {
    for(int i = 1; i <= N; i++) {
        ifstream finput;
        ofstream foutput;
        open_file(finput, foutput, i);
        solve(finput, foutput, i);
        close_file(finput, foutput, i);
    }
    return 0;
}
