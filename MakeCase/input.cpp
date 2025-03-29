#include<fstream>
#include<iostream>
#include<random>
#include<ctime>
using namespace std;

const int N = 7;

void open_input(ofstream &fout, int t_case) {
    string dir_path = "../data/secret/";
    string file_name = to_string(t_case) + ".in";
    string input_path = dir_path + file_name;
    fout.open(input_path, ios::out | ios::trunc);
    if(!fout) {
        cerr << "cannot open file" << input_path << endl;
        exit(1);
    }
}

default_random_engine gen(time(NULL));
uniform_int_distribution<int> unif(0, 2147483647);

void make_input(ofstream &fout, int t_case) {
    cerr << "start generating testcase " << t_case << endl;
    if(t_case == 1) {
        // example
        fout << "5" << endl;
        fout << "1 2 5 4 5" << endl;
    } else {
        int n = unif(gen) % 100 + 1;
        fout << n << endl;
        for(int i = 0; i < n; i++) {
            if(i) fout << " ";
            fout << unif(gen) % 50 + 1;
        }
    }
}
void close_input(ofstream &fout, int t_case) {
    fout.close();
}
int main() {
    for(int i = 1; i <= N; i++) {
        ofstream fout;
        open_input(fout, i);
        make_input(fout, i);
        close_input(fout, i);
    }
    return 0;
}
