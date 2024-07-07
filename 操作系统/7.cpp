#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
using namespace std;
 
#define MaxNumber 100
int TrackOrder[MaxNumber];//初始磁道序列
int MoveDistance[MaxNumber];//磁头移动距离（磁道数）
double AverageDistance;//磁头平均移动距离
int direction;//选择磁头方向 
int TrackNum;//磁道数
int StartTrack;//开始磁道号m 
int VisitOrder[MaxNumber];//访问磁道序列
bool isVisit[MaxNumber];//标记是否被访问过
int SumDistance;//磁头移动的总距离
int choose;

void input();//输入起始磁道号、磁道顺序
void initial();
void output();
void FCFS();//先来先服务，先进先出
void SSTF();//最短寻道时间优先
void SCAN();//扫描，从开始磁道沿选择方向扫描，直到没有要访问的磁道在沿反方向扫描
void CSCAN();//循环扫描，自开始磁道始终沿一个方向扫描，直到没有要访问的磁道再从最里圈或最外圈扫描
void chooseAlgorithm();
 
int main(){
    input();
    chooseAlgorithm();
    return 0;
}
 
void input(){
    ifstream readData;
	readData.open("data.txt");
	readData>>TrackNum;  //磁道个数
	for (int i=0;i<TrackNum;i++)
	{
		readData>>TrackOrder[i];   //磁道访问序列
	}
	readData>>StartTrack;  //开始磁道号
	cout<<"磁道个数 = "<<TrackNum<<endl;
	cout<<"磁道访问序列：";
	for (int i=0;i<TrackNum;i++)
	{
		cout<<TrackOrder[i]<<" ";
	}
	cout<<endl;
	cout<<"开始磁道号m= "<<StartTrack<<endl;
	cout<<"------------------------------------"<<endl;
}
void initial(){
    for (int i=0;i<TrackNum;i++){
        MoveDistance[i] = 0;
        VisitOrder[i] = TrackOrder[i];
        isVisit[i] = false;
    }
    SumDistance = 0;
    AverageDistance = 0;
}

void output(){
    cout<<"从"<<StartTrack<<"号磁道开始"<<endl;
    cout<<"被访问的下一个磁道号"<<"\t"<<"移动距离"<<"\t"<<endl;
    for (int i=0;i<TrackNum;i++){
        cout<<VisitOrder[i]<<"\t\t\t"<<MoveDistance[i]<<"\t"<<endl;
    }
    cout<<"平均寻道长度: "<<setprecision(4)<<AverageDistance<<endl;
    cout<<endl;
}
 
//先来先服务，先进先出
void FCFS(){
    cout<<endl;
    cout<<"FCFS"<<endl;
    initial();
 
    //按照输入顺序依次访问磁道
    MoveDistance[0] = abs(TrackOrder[0]-StartTrack);
    SumDistance = MoveDistance[0];
    VisitOrder[0] = TrackOrder[0];
 
    for (int i=1;i<TrackNum;i++){
        MoveDistance[i] = abs(TrackOrder[i]-TrackOrder[i-1]);
        SumDistance += MoveDistance[i];
        VisitOrder[i] = TrackOrder[i];
    }
 
    AverageDistance = SumDistance*1.0/TrackNum;
    output();
}
 
//最短寻道时间优先
void SSTF(){
    cout<<endl;
    cout<<"SSTF"<<endl;
    initial();
    int CurrentTrack = StartTrack;
    int i,j,pointMin;
    int distance[MaxNumber];
 
    for (i = 0;i<TrackNum;i++){
        for (j = 0;j<TrackNum;j++){
            if (!isVisit[j])
                distance[j] = abs(TrackOrder[j]-CurrentTrack);
            else
                distance[j] = 10000;  //表示无穷远，即访问过的磁道就不再访问
        }
        pointMin = 0;
        for (j = 0;j<TrackNum;j++){
            if (distance[pointMin] > distance[j])
                pointMin = j;   //指向最小的位置
        }
        VisitOrder[i] = TrackOrder[pointMin];  //给访问序列赋值
        MoveDistance[i] = abs(TrackOrder[pointMin]-CurrentTrack);  //计算每次的移动距离
        SumDistance += MoveDistance[i];   //累计移动距离
        CurrentTrack = TrackOrder[pointMin];   //改变当前的磁道号
        isVisit[pointMin] = true;  //将当前的磁道号设置为已访问
    }
    AverageDistance = SumDistance*1.0/(TrackNum);
    output();
}
 
//扫描，从开始磁道沿选择方向扫描，直到没有要访问的磁道在沿反方向扫描
void SCAN(){
    cout<<endl;
    cout<<"SCAN"<<endl;
    cout<<"选择磁头移动的方向,1-增加,2-减少: "<<endl;
    cin>>direction;
    initial();
 
    int i,j,temp;
    int SortTrackOrder[MaxNumber];
    for (i = 0;i<TrackNum;i++){//初始化
        SortTrackOrder[i] = TrackOrder[i];
    }
 
    //冒泡排序
    for (i = 0;i<TrackNum;i++){
        for (j = i;j<TrackNum;j++){
            if (SortTrackOrder[i]>=SortTrackOrder[j]){
                temp = SortTrackOrder[i];
                SortTrackOrder[i] = SortTrackOrder[j];
                SortTrackOrder[j] = temp;
            }
        }
    }
 
    //找到既在当前磁道之外，又是距离最近的磁道号
    int point = 0;
    while(StartTrack>=SortTrackOrder[point]){
        point++;
    }
 
    int count = 0;
    int currentTrack = StartTrack;
 
    if (direction == 1){  //向磁道增加的方向访问
        cout<<"向磁道增加的方向访问"<<endl;
        for (i = point;i<TrackNum;i++){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
        for (i = point - 1;i>=0;i--){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
    }
    else if (direction == 2){  //向磁道减少的方向访问
        cout<<"向磁道减少的方向访问"<<endl;
        for (i = point-1;i>=0;i--){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
        for (i = point;i<TrackNum;i++){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
    }
    for (i = 0;i<TrackNum;i++){
        SumDistance += MoveDistance[i];
    }
    AverageDistance = (SumDistance*1.0)/TrackNum;
    output();
}
 
//循环扫描，自开始磁道始终沿一个方向扫描，直到没有要访问的磁道再从最里圈或最外圈扫描
void CSCAN(){

	initial();
    cout<<"CSCAN"<<endl;
    cout<<"选择磁头移动的方向,1-增加,2-减少: "<<endl;
    cin>>direction;
 
    
    int SortTrackOrder[MaxNumber];
    int i,j,temp;
    for (i = 0;i<TrackNum;i++){
        SortTrackOrder[i] = TrackOrder[i];
    }
  
    //冒泡排序
    for (i = TrackNum - 1;i>0;i--){
        for (j = 0;j<i;j++){
            if (SortTrackOrder[j]>=SortTrackOrder[j+1]){
                temp = SortTrackOrder[j];
                SortTrackOrder[j] = SortTrackOrder[j+1];
                SortTrackOrder[j+1] = temp;
            }
        }
    }
 
    //找到既在当前磁道之外，又是距离最近的磁道号
    int point = 0;
    while(StartTrack>=SortTrackOrder[point]){
        point++;
    }
 
    int count = 0;
    int currentTrack = StartTrack;
    if (direction == 1){  
        cout<<"向磁道增加的方向访问"<<endl;
        for (i = point;i<TrackNum;i++){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
 
        for (i =0;i<point;i++){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
    }
    else if (direction == 2){//向磁道减少的方向访问
        cout<<"向磁道减少的方向访问"<<endl;
        for (i = point-1;i>=0;i--){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
        for (i = TrackNum-1;i>point-1;i--){
            VisitOrder[count] = SortTrackOrder[i];
            MoveDistance[count] = abs(VisitOrder[count]-currentTrack);
            currentTrack = VisitOrder[count];
            count++;
        }
    }
 
    for (i = 0;i<TrackNum;i++){
        SumDistance += MoveDistance[i];
    }
    AverageDistance = (SumDistance*1.0)/TrackNum;
    output();
}


void chooseAlgorithm()
{
	cout<<"请选择算法“1-FCFS，2-SSTF，3-SCAN，4-循环SCAN ,0-退出”"<<endl;
	cin>>choose;
	if (choose==1)
	{
		FCFS();
		chooseAlgorithm();
	}
		else if(choose==2)
		{
			SSTF();
			chooseAlgorithm();
		}
		else if(choose==3){
            SCAN();
        	chooseAlgorithm();
		}
		else if(choose==4){
            CSCAN();
            chooseAlgorithm();
		}
		else if(choose==0){
          exit(0);
		}
	else
	{
		cout<<"请输入正确的选择“1-FCFS，2-SSTF，3-SCAN，4-循环SCAN ,0-退出”"<<endl;
		chooseAlgorithm();  
	}
}
