#include "iostream"
#include "Windows.h"

using namespace std;

int main(){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));
    //
    if(!CreateProcess(NULL,//模块名
    "C:\\Program Files\\Microsoft Office\\root\\Office16\\WINWORD.EXE",//命令行
    NULL,//进程安全描述符
    NULL,//线程安全描述符
    FALSE,//继承句柄标志
    0,//创建标志
    NULL,//环境变量
    NULL,//当前目录
    &si,//启动信息
    &pi//进程信息
    )){
        cout<<"CreatProcess failed"<<endl;
    }

    //等待Word进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);
    //终止Word进程
    TerminateProcess(pi.hProcess, 0);

    //关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}