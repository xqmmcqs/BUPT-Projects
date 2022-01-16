#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#define PRINT_WIDTH 30
using namespace std;

int n;                                    // 状态数
int m;                                    // 字母数
vector<string> Q;                         // 状态集
vector<string> T;                         // 字母表
unordered_map<string, int> mp;            // 状态字符串对序号的映射
vector<vector<unsigned long long>> delta; // NFA转移
int q0;                                   // 初始状态的序号
unsigned long long Fn;                    // 终止状态集
vector<vector<unsigned long long>> ans;   // DFA转移表
bool *vis;                                // 标记状态是否访问过
bool is_epsilon;                          // 是否是epsilon-NFA

void input()
{
    string lineString, tempString;
    cout << "请输入状态集，以空格分隔：" << endl;
    istringstream ss;
    getline(cin, lineString);
    ss.str(lineString);
    while (ss >> tempString) // 输入状态集
    {
        Q.push_back(tempString);
        mp[tempString] = Q.size() - 1; // 字符串到序号的映射
    }
    n = Q.size();

    cout << "请输入字符集，以空格分隔（如果是NFA，则请额外输入[empty]）：" << endl;
    getline(cin, lineString);
    ss.clear();
    ss.str(lineString);
    while (ss >> tempString) //输入字母表
        T.push_back(tempString);
    if (T.back() == "[empty]")
        is_epsilon = true;
    m = T.size();

    for (int i = 0; i < n; ++i) // 输入NFA状态转移
    {
        delta.emplace_back(0);
        for (int j = 0; j < m; ++j)
        {
            delta[i].push_back(0);
            cout << "请输入状态 " << Q[i] << " 沿字符 " << T[j] << " 转移的状态集合，以空格分隔（如果是空集本行留空）：" << endl;
            getline(cin, lineString);
            ss.clear();
            ss.str(lineString);
            while (ss >> tempString)
                delta[i][j] |= (1 << mp[tempString]);
        }
    }

    cout << "请输入初始状态：" << endl;
    getline(cin, lineString); // 输入初始状态
    q0 = mp[lineString];

    cout << "请输入终止状态集，以空格分隔：" << endl;
    getline(cin, lineString);
    ss.clear();
    ss.str(lineString);
    while (ss >> tempString) // 输入终止状态集
        Fn |= (1 << mp[tempString]);

    vis = new bool[1 << n];
    vis[0] = true;
}

void to_nonepsilon()
{
    for (int i = 0; i < n; ++i)
    {
        unsigned long long closure = 1 << i;
        while (true) // 求epsilon-closure
        {
            unsigned long long last_closure = closure, temp = closure;
            for (int i = 0; i < n && temp; ++i, temp >>= 1)
            {
                if (temp & 1)
                    closure |= delta[i][m - 1];
            }
            if (closure == last_closure) // 如果发现通过空串转移，closure集合没有新的状态加入，则求出了闭包
                break;
        }
        delta[i][m - 1] = closure;
    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m - 1; ++j) // 求i通过符号j转移的集合
        {
            unsigned long long temp = delta[i][m - 1];
            for (int k = 0; k < n && temp; ++k, temp >>= 1)
            {
                if (temp & 1) // 沿i的epsilon-closure到达的状态k
                {
                    unsigned long long temp2 = delta[k][j];
                    for (int l = 0; l < n && temp2; ++l, temp2 >>= 1) // 状态k通过符号j转移到l
                    {
                        if (temp2 & 1)
                            delta[i][j] |= delta[l][m - 1]; // 再沿l的epsilon-closure转移
                    }
                }
            }
        }
    m--;
    T.pop_back(); // 删除[empty]
}

void dfs(unsigned long long status)
{
    // 标记当前状态为已访问
    vis[status] = true;
    ans.emplace_back(0);
    int cur = ans.size() - 1;   // 记录当前行号
    ans[cur].push_back(status); // 当前状态作为ans行的第一个元素
    for (int i = 0; i < m; ++i)
    {
        unsigned long long nextStatus = 0, temp = status;
        for (int j = 0; j < n && temp; ++j, temp >>= 1)
            if (temp & 1)                  // 如果序号为j的状态在当前状态集合中
                nextStatus |= delta[j][i]; // 从序号为j的状态，沿序号为i的符号转移

        ans[cur].push_back(nextStatus);
        if (!vis[nextStatus]) // 如果下一个状态没有访问过，则访问下一个状态
            dfs(nextStatus);
    }
}

void output()
{
    // 输出第一行：字符集
    cout << "|";
    cout << setw(PRINT_WIDTH) << "|";
    for (auto i : T)
        cout << setw(PRINT_WIDTH) << i + "|";
    cout << endl;

    // 输出分隔线
    for (int i = 0; i <= m; ++i)
        cout << '|' << string(PRINT_WIDTH - 1, '-');
    cout << '|' << endl;

    for (auto &i : ans)
    {
        ostringstream ss;
        cout << "|";
        if (i[0] == (1 << q0)) // 初始状态
            ss << "->";
        if (i[0] & Fn) // 终止状态
            ss << '*';
        for (auto j : i)
        {
            ss << "{";
            if (!j)
                ss << ","; // 如果为空集，之后seekp方法会抹掉左大括号，所以加一个逗号占位
            for (int k = 0; k < n && j; ++k, j >>= 1)
                if (j & 1)
                    ss << (Q[k] + ",");
            ss.seekp(-1, ostringstream::end); // 去掉最后一个逗号
            ss << "}|";
            cout << setw(PRINT_WIDTH) << ss.str();
            ss.clear();
            ss.str("");
        }
        cout << endl;
    }

    delete[] vis;
}

int main()
{
    input();
    if (is_epsilon)
        to_nonepsilon();
    dfs(1 << q0); // 从初始状态开始搜索
    output();
    return 0;
}
