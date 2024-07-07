#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <queue>
 
using namespace std;
const int N = 100010;
 
int n;//进程数量
int totaltime;//总时间
int weighttotaltime;//总带权周转时间
int stime;//时间片
int a;//算法选择
 
struct PCB{
string id; //进程号
int pri;//优先级
int arrivetime;//到达时间
int begintime;//开始时间
int waittime;//等待时间
int finishtime;//完成时间
int runtime;//运行时间
int resttime;//仍需运行时间
int runcount;//运行次数
int zhouzhuantime;//周转时间
double weightzhouzhuantime; //带权周转时间
PCB* next;
bool operator < (const PCB& p)
{
return arrivetime < p.arrivetime;
}
}process[N];

PCB* createPCB()//创造进程
{
    PCB* head, * rear;//队首、队尾
    head = rear = NULL;
    PCB* p;//临时指针变量
    cout << "******************************************" << endl;
    cout << "请输入进程数量：";
    cin >> n;
    for (int i = 0;i < n;i++){
        //初始化进程
        p = new PCB;
        cout << "请依次输入第" << i + 1 << "个进程的信息（进程名、优先级、到达时间、运行时间）：" << endl;
        cin >> p->id >> p->pri >> p->arrivetime >> p->runtime;
        p->runcount = 1;
        p->resttime = p->runtime;
        totaltime += p->runtime;
        p->begintime = 0;
        p->finishtime = 0;
        p->zhouzhuantime = 0;
        p->weightzhouzhuantime = 0;
        p->next = NULL;
        //存入链表
        if (rear == NULL){
            head = p;
            rear = p;
        }
        else{
            rear->next = p;
            rear = p;
        }
    }
    cout << "******************************************" << endl;
    cout << endl;
    cout << endl;
    return head;
}
//链表插入排序
PCB* insertSort(PCB* head){
    PCB* first;//为原链表剩下用于直接插入排序的节点头指针
    PCB* t;//要插入的节点
    PCB* p;//要插入的位置
    PCB* q;//指向原链表
    first = head->next;
    head->next = NULL;
    p = NULL;
    while (first != NULL)//遍历剩下的无序链表
    {
    //找到插入位置p
    for (t = first, q = head;(q != NULL) && (q->arrivetime <= t->arrivetime);p = q, q = q->next);
    first = first->next;
    //插入
    if (q == head)//插在头节点之前
    {
        head = t;
    }
    else{
        p->next = t;
    }
    t->next = q;
    }
return head;
}
 
//获取当前时间段里的进程数量
int getCurrentNumOfProcess(PCB* head, int time){
    int count = 0;
    PCB* t;
    t = head;
    while (t != NULL && t->arrivetime <= time){
        count++;
        t = t->next;
    }
    return count;
}
 
//删除当前节点
PCB* deletePCB(PCB* head, PCB* t){
    PCB* p, * q;
    p = head;
    q = p->next;
    if (t == head){//删除的是头节点
        head = head->next;
    }
    else{
        while (q != t){
            //跳出循环之后q为该节点，p为前一节点
        p = p->next;
        q = p->next;
        }
        if (t->next == NULL)//删除的是尾节点
            p->next = NULL;
        else p->next = q->next;
    }
free(t);
return head;
}
 
//在头节点后的count个节点中找出优先数最大的进程
PCB* findMaxPri(PCB* head, int count){
    int max;
    PCB* q, * f;
    q = head;
    max = q->pri;
    f = q;
    while (count > 0){
        if (q->pri > max){
            max = q->pri;
            f = q;
        }
        count--;
        q = q->next;
    }
return f;
}
 
//输出
void print(vector<PCB> vec_output){
    for (int i = 0;i < vec_output.size();i++){
        cout << "**********************************" << endl;
        cout << "进程号" << vec_output[i].id << endl;
        cout << "进程到达时间：" << vec_output[i].arrivetime << endl;
        cout << "开始运行时间: " << vec_output[i].begintime << endl;
        cout << "结束运行时间: " << vec_output[i].finishtime << endl;
        cout << "此次运行时间：" << vec_output[i].finishtime - vec_output[i].begintime << endl;
        cout << "**********************************" << endl;
        cout << endl;
        cout << endl;
    }
    //输出周转时间信息
    int i;
    for (i = 0;i < vec_output.size() - 1;i++){
        bool flag = true;
        for (int j = i + 1;j < vec_output.size();j++){
            if (vec_output[j].id == vec_output[i].id){
                flag = false;
                break;
            }
        }
        if (flag){
            cout << "**********************************" << endl;
            cout << "进程" << vec_output[i].id << "的周转时间为：" << vec_output[i].zhouzhuantime << endl;
            cout << "进程" << vec_output[i].id << "的带权周转时间为：" << vec_output[i].weightzhouzhuantime << endl;
            cout << "**********************************" << endl;
            cout << endl;
            cout << endl;
        }
    }
    //输出最后一个进程的周转时间信息
    cout << "**********************************" << endl;
    cout << "进程" << vec_output[i].id << "的周转时间为：" << vec_output[i].zhouzhuantime << endl;
    cout << "进程" << vec_output[i].id << "的带权周转时间为: " << vec_output[i].weightzhouzhuantime << endl;
    cout << "**********************************" << endl;
    cout << endl;
    cout << endl;
    //平均周转时间信息
    cout << "**********************************" << endl; 
    cout << "平均周转时间：" << totaltime / (double)n << endl;
    cout << "平均带权周转时间：" << weighttotaltime / (double)n << endl;
    cout << "**********************************" << endl;
    cout << endl;
    cout << endl;
}
 
void PCB_MAIN(PCB* head){
    cout << "最高优先数优先算法" << endl;
    head = insertSort(head);
    int time = 0;
    int count;
    PCB* q;
    vector<PCB>vec_out;
    PCB temp;
    while (head != NULL){
        count = getCurrentNumOfProcess(head, time);
        if (count == 0){
            time++;
        }
        else{
            //找出优先数最大的进程
            q = findMaxPri(head, count);
            if (q->runcount == 1){
                //该线程第一次运行
                q->begintime = time;
                //输出信息
                temp = *q;
                temp.finishtime = 0;
                temp.next = NULL;
                if (vec_out.size() != 0 && vec_out[vec_out.size() - 1].finishtime == 0){
                    vec_out[vec_out.size() - 1].finishtime = temp.begintime;
                }
                vec_out.push_back(temp);
                }
            ++time;
            ++q->runcount;
            --q->resttime;
            if (q->resttime == 0){//进程结束
                q->finishtime = time;//记录结束时间
                q->zhouzhuantime = time - q->arrivetime;//计算周转时间
                q->weightzhouzhuantime = q->zhouzhuantime / (double)q->runtime;//计算带权周转时间
                weighttotaltime += q->weightzhouzhuantime;
                //输出信息
                temp = *q;
                temp.begintime = 0;
                temp.next = NULL;
                if (vec_out[vec_out.size() - 1].id == temp.id){
                    vec_out[vec_out.size() - 1].finishtime = temp.finishtime;
                    vec_out[vec_out.size() - 1].zhouzhuantime = temp.zhouzhuantime;
                    vec_out[vec_out.size() - 1].weightzhouzhuantime = temp.weightzhouzhuantime;
                }
            else{
                temp.begintime = vec_out[vec_out.size() - 1].finishtime;
                vec_out.push_back(temp);
            }
                //删除该进程
                head = deletePCB(head, q);
            }
        }
    }
    print(vec_out);
}
 
void HPF(){
    PCB* head = NULL;
    head = createPCB();
    PCB_MAIN(head);
}
  
void Initfcfs(){
    cout << "******************************************************************" << endl;
    cout << "请输入进程数量：";
    cin >> n;
    for (int i = 0;i < n;i++) {
        cout << "请依次输入第" << i + 1 << "个进程的信息（进程号、到达时间、运行时间）：" << endl;
        cin >> process[i].id >> process[i].arrivetime >> process[i].runtime;
        process[i].resttime = process[i].runtime;
        totaltime += process[i].runtime;
    }
    cout << "******************************************************************" << endl;
    cout << endl;
    cout << endl;
}
 
void RR_MAIN() {
    cout << "时间片轮转调度算法" << endl;
    sort(process, process + n);
    int time = 0;
    int count;
    queue<PCB> q;
    for (int i = 0;i < n;i++){
        q.push(process[i]);
    }
    int times = stime;
    time = q.front().arrivetime;
    while (true){
        if (times == 0){//重置时间片
            times = stime;
        }
        if (q.front().arrivetime > time){
            q. push(q.front());
            q.pop();
            continue;
        }
        q.front().begintime = time;
        while (times != 0){
            --times;
            ++time;
            --q.front().resttime;
            if (q.front().resttime == 0) break;
        }
        q.front().finishtime = time;
        cout << "****************************************" << endl;
        cout << "进程号" << q.front().id << endl;
        cout << "到达时间：" << q.front().arrivetime << endl;
        cout << "开始运行时间: " << q.front().begintime << endl;
        cout << "结束运行时间: " << q.front().finishtime << endl;
        if (q.front().resttime != 0){
            cout << "此进程运行了一个时间片，切换到下一进程！" << endl;
            q.push(q.front());
            q.pop();
        }
        else{
            q.front().zhouzhuantime = time - q.front().arrivetime;
            q.front().weightzhouzhuantime = q.front().zhouzhuantime / (double)q.front().runtime;
            weighttotaltime += q.front().weightzhouzhuantime;
            cout << "此进程已完成！" << endl;
            cout << "进程" << q.front().id << "的周转时间为：" << q.front().zhouzhuantime << endl;
            cout << "进程" << q.front().id << "的带权周转时间为: " << q.front().weightzhouzhuantime << endl;
            q.pop();
            times = 0;
        }
        cout << "****************************************" << endl;
        cout << endl;
        cout << endl;
        if (q.empty())break;
    }
    cout << "**********************************" << endl;
    cout << "平均周转时间：" << totaltime / (double)n << endl;
    cout << "平均带权周转时间：" << weighttotaltime / (double)n << endl;
    cout << "**********************************" << endl;
    cout << endl;
    cout << endl;
}
 
void RR(){
    Initfcfs();
    cout << "请输入时间片大小：";
    cin >> stime;
    cout << endl;
    cout << endl;
    RR_MAIN();
}
 
bool FCFS(PCB a, PCB b) {
    if (a.arrivetime != b.arrivetime) return a.arrivetime < b.arrivetime;
    else return a.id < b.id;
}//先来先服务 比较到达时间
  
void fcfs(){
    Initfcfs();
    cout << "先来先服务算法" << endl;
    sort(process, process + n, FCFS);
    queue<PCB> q;
    int time = 0;
    for (int i = 0;i < n;i++){
        q.push(process[i]);
    }
    time = q.front().arrivetime;
    while (true){
        q.front().begintime = time;
        while (q.front().resttime != 0){
            ++time;
            --q.front().resttime;
        }
        q.front().finishtime = time;
        cout << "****************************************" << endl;
        cout << "进程号" << q.front().id << endl;
        cout << "到达时间：" << q.front().arrivetime << endl;
        cout << "开始运行时间: " << q.front().begintime << endl;
        cout << "结束运行时间: " << q.front().finishtime << endl;
        q.front().zhouzhuantime = time - q.front().arrivetime;
        q.front().weightzhouzhuantime = q.front().zhouzhuantime / (double)q.front().runtime;
        weighttotaltime += q.front().weightzhouzhuantime;
        cout << "此进程已完成！" << endl;
        cout << "进程" << q.front().id << "的周转时间为：" << q.front().zhouzhuantime << endl;
        cout << "进程" << q.front().id << "的带权周转时间为: " << q.front().weightzhouzhuantime << endl;
        cout << "****************************************" << endl;
        cout << endl;
        cout << endl;
        q.pop();    
        if (q.empty())break;
    }
    cout << "**********************************" << endl;
    cout << "平均周转时间：" << totaltime / (double)n << endl;
    cout << "平均带权周转时间：" << weighttotaltime / (double)n << endl;
    cout << "**********************************" << endl;
    cout << endl;
    cout << endl;
}
 
void menu(){
    cout << "******************************************" << endl;
    cout << "1、先来先服务算法" << endl << "2、时间片轮转调度算法" << endl << "3、最高优先数优先算法" << endl << "(输入序号即可)" << endl;
    cout << "******************************************" << endl;
    cin >> a;
    switch (a){
        case 1:fcfs();break;
        case 2:RR();break;
        case 3:HPF();break;
    }
}
 
int main(){
    while (true){
        menu(); 
    }
}