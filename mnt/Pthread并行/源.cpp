#include <immintrin.h> // AVX指令集
#include <omp.h> // OpenMP
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono> // for timing

using namespace std;
using namespace std::chrono;

const int N = 2000;
const int Len = 255;
vector<vector<int>> a(5, vector<int>(90000, 0)); // 消元子
int c[90000];

int String_to_int(const string& a) {
    int res = 0;
    for (char ch : a) {
        res = res * 10 + (ch - '0');
    }
    return res;
}

string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}

vector<int> reca(5, 0);

void input(istringstream& s, vector<int>& q) {
    string st;
    while (s >> st) {
        q[String_to_int(st)] = 1;
    }
}

void inFile(const string& load, const vector<int>& s) {
    ofstream fil(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            if (!flag) c[i] = 1;
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }

    if (!flag && load == "D://mnt//data//res2.txt") {
        fil << endl;
    }
    else if (flag) {
        fil << endl;
    }
}

vector<int> xiaoyuan(const vector<int>& s, const vector<int>& q) {
    vector<int> result(s.size(), 0);
#pragma omp parallel for
    for (size_t i = 0; i < s.size(); i += 8) { // 每次处理8个32位整数
        __m256i s_vec = _mm256_loadu_si256((__m256i*) & s[i]); // 加载AVX向量
        __m256i q_vec = _mm256_loadu_si256((__m256i*) & q[i]);
        __m256i result_vec = _mm256_xor_si256(s_vec, q_vec); // 执行按位异或
        _mm256_storeu_si256((__m256i*) & result[i], result_vec); // 存储结果
    }
    return result;
}

int signal[2] = { 0, 0 };

void get_duijiaoxian(int s[]) {
    // Implementation not provided
}

void get_xyz() {
    // Implementation not provided
}

int main() {
    // 文件的初始化
    std::ofstream("D://mnt//data//res1.txt", std::ios::trunc).close();
    std::ofstream("D://mnt//data//res0.txt", std::ios::trunc).close();
    std::ofstream("D://mnt//data//res2.txt", std::ios::trunc).close();

    std::string sourceFile = "D://mnt//data//被消元行.txt";
    std::string targetFile = "D://mnt//data//res1.txt";
    ifstream source(sourceFile, ios::binary);
    ofstream target(targetFile, ios::binary);

    if (!source.is_open() || !target.is_open()) {
        cerr << "Error: Could not open files." << endl;
        return 1;
    }

    target << source.rdbuf();
    source.close();
    target.close();

    ifstream file("D://mnt//data//消元子 - 副本.txt");
    string line;
    int curfile = 1;
    ofstream fileoutres("D://mnt//data//res2.txt", ios::app);

    auto start = high_resolution_clock::now(); // Start the timer

    bool flagg = true;
    while (flagg) {
        ifstream fileout("D://mnt//data//res1.txt");
        ifstream fileout1("D://mnt//data//res0.txt");
        int num1 = 0;
        flagg = true;
        int needle = 0;
        a.resize(5, vector<int>(90000, 0)); // 重置消元子向量
        fill(reca.begin(), reca.end(), 0);

        while (needle < 5 && getline(file, line)) {
            string str;
            istringstream stream(line);
            bool flag = false;
            while (stream >> str) {
                int val = String_to_int(str);
                if (!flag) {
                    reca[needle] = val;
                    flag = true;
                }
                a[needle][val] = 1;
            }
            needle++;
        }

        int p = 0;
        while (p < signal[curfile]) {
            getline((curfile == 1 ? fileout : fileout1), line);
            p++;
        }

#pragma omp parallel
        {
            vector<int> b(90000, 0);
#pragma omp for schedule(dynamic)
            for (int i = 0; i < 5; i++) {
                if (getline((curfile == 1 ? fileout : fileout1), line)) {
                    signal[curfile]++;
                    flagg = false;
                    int start = 0;
                    string str;
                    istringstream stream(line);
                    bool flag = true;
                    while (stream >> str) {
                        int val = String_to_int(str);
                        if (flag) {
                            start = val;
                            flag = false;
                        }
                        b[val] = 1;  // 读取被消元素
                    }
                    flag = false;
                    for (int j = 0; j < a.size(); j++) {
                        if (start > reca[j]) {
                            flag = true;
#pragma omp critical
                            {
                                a.insert(a.begin() + j, b);
                                reca.insert(reca.begin() + j, start);
                            }
                            inFile("D://mnt//data//res2.txt", b);
                            break;
                        }
                        else if (start < reca[j]) {
                            continue;
                        }
                        else if (start == reca[j]) {
                            b = xiaoyuan(b, a[j]);
                            while (start >= 0 && !b[start]) {
                                start--;
                            }
                            continue;
                        }
                    }
                    if (!flag) {
                        num1++;
                        string curF = "res" + int_to_String(curfile ^ 1) + ".txt";
                        inFile("D://mnt//data//" + curF, b);
                    }
                }
            }
        }

        curfile ^= 1;
        if (flagg) {
            break;
        }
        fileout.close();
        fileout1.close();
        flagg = true;
    }

    auto stop = high_resolution_clock::now(); // Stop the timer
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " milliseconds" << endl;

    return 0;
}
