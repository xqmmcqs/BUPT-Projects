#include <fstream>
using namespace std;

int dataNum = 20;        //数据条数
int timeMax = 5;         //读写最大时长，以 100 毫秒为单位
char RW[2] = {'R', 'W'}; //命令类型，分为读、写命令

int main()
{
    ofstream data("data.txt"); //以覆盖方式打开文件
    if (data)                  //打开文件成功
    {
        srand((unsigned)time(NULL));
        for (int i = 0; i < dataNum; i++)
        {
            int index = rand() % 2;                 //决定读写方式
            int spendTime = (rand() % timeMax + 1); //读写花费时间
            data << RW[index] << " " << spendTime;  //输出到文件保存
            if (i < dataNum - 1)
                data << "\n";
        }
        data.close();
    }
}
