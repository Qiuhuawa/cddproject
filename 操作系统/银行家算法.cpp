#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 12,M = 12;	//最大进程数，最大资源种类 
int n,m;		//n个进程，m种资源 
int mks[N][M];	//Max最大需求矩阵,进程i需要j类资源的数目 
int alcs[N][M];	//Allocation分配矩阵，进程i的j类资源已分配的数目 
int need[N][M];	//Need需求矩阵，进程i，j类资源还需要的数目 
int avlb[M];	//Available可用资源向量，j类资源可用的数目 
int sod[N];		//SafeOrder安全序列 
int bkup_alcs1[N][M],bkup_need1[N][M],bkup_avlb1[M];	//BackUp备份
int bkup_alcs2[N][M],bkup_need2[N][M],bkup_avlb2[M];	//BackUp备份

void myinput() {
	//输入
	cout << "请输入每个进程的Max Allocation" << endl;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) cin >> mks[i][j];
		for(int j = 0; j < m; j++) cin >> alcs[i][j];
		for(int j = 0; j < m; j++) need[i][j] = mks[i][j] - alcs[i][j];
	}
	cout << "请输入Avaliable" << endl;
	for(int j = 0; j < m; j++) cin >> avlb[j];
}

void myoutput() {
	cout << "\n资源分配表\n\n";
	cout << "进程\\资源\tMax\t\tAllocation" << endl;
	for(int i = 0; i < n; i++) {
		cout << "P" << i << "\t\t";
		for(int j = 0; j < m; j++) cout << mks[i][j] << " ";
		cout << "\t\t";
		for(int j = 0; j < m; j++) cout << alcs[i][j] << " ";
		cout << "\t\t";
		for(int j = 0; j < m; j++) cout << need[i][j] << " ";
        cout << endl;
	}
	cout << "Available" << endl;
	for(int j = 0; j < m; j++) cout << avlb[j] << " ";
	cout << endl;
}

//备份alcs,need,avlb
void backup(int bkup_alcs[N][M],int bkup_need[N][M],int bkup_avlb[M]) {
	memcpy(bkup_alcs,alcs,sizeof alcs);
	memcpy(bkup_need,need,sizeof need);
	memcpy(bkup_avlb,avlb,sizeof avlb);
}

//恢复alcs,need,avlb
void recovery(int bkup_alcs[N][M],int bkup_need[N][M],int bkup_avlb[M]) {
	memcpy(alcs,bkup_alcs,sizeof alcs);
	memcpy(need,bkup_need,sizeof need);
	memcpy(avlb,bkup_avlb,sizeof avlb);
}

//安全性检查
bool check_safe() {
	backup(bkup_alcs1,bkup_need1,bkup_avlb1);	//备份alcs,need,avlb
	bool flag[N];	//已完成的进程的标志
	int cnt = 0;	//完成的进程数 
	//初始化falg
	memset(flag,0,sizeof flag);
	//run 
	while(1) {
		//遍历，找到可以满足的资源 
		int i;
		for(i = 0; i < n; i++) {
			if(flag[i]) continue;	//已完成的进程跳过
			int j;
			for(j = 0; j < m; j++)
				if(need[i][j] > avlb[j]) break;	//有资源不能满足，判断下个进程
			if(j==m) break;	//找到了一个可以满足的进程，跳出 
		}
		//如果找不到可以满足的进程，那么不是安全状态 
		if(i==n) {
			recovery(bkup_alcs1,bkup_need1,bkup_avlb1);	//恢复alcs,need,avlb
			return false;
		}
		int id = i;	//取得进程编号 
		//回收该进程的资源
		for(int j = 0; j < m; j++) avlb[j] += alcs[id][j];
		flag[id] = true;	//当前进程设完成
		sod[cnt] = id;	//放安全序列 
		cnt++;	//已完成++
		if(cnt==n) {
			recovery(bkup_alcs1,bkup_need1,bkup_avlb1);	//恢复alcs,need,avlb
			return true;	//全部完成，返回是安全状态
		}
	}
}

//请求资源
//返回值，0:正确，1:超需求，2：超可分配资源，3：假定分配无法通过安全性检查 
int request_resource(int id,int req[M]) {
	//超需求判断 
	for(int j = 0; j < m; j++) if(req[j] > need[id][j]) return 1;
	//超可分配资源判断 
	for(int j = 0; j < m; j++) if(req[j] > avlb[j]) return 2;
	//假定分配
	backup(bkup_alcs2,bkup_need2,bkup_avlb2);	//备份alcs,need,avlb
	for(int j = 0; j < m; j++) avlb[j] -= req[j];
	for(int j = 0; j < m; j++) alcs[id][j] += req[j];
	for(int j = 0; j < m; j++) need[id][j] -= req[j];
	//安全性检查
	if(!check_safe()) {
		recovery(bkup_alcs2,bkup_need2,bkup_avlb2);	//恢复 
		return 3; 
	}
	//检查通过，正式分配资源，也就不需要恢复备份了 
	return 0;
}

int main() {
	cout << "请输入进程数 资源数" << endl;
	cin >> n >> m;
	myinput();	//输入参数
	myoutput();	//输出表格
	if(check_safe()) {
		cout << "是安全状态，安全序列为" << endl;
		for(int i = 0; i < n; i++) {
			cout << sod[i];
			if(i!=n-1) cout << "->";
		}
		cout << endl;
	}
	else cout << "不是安全状态" << endl;
	//输入请求资源
	while(1) {
		int id,req[M];
		cout << "\n请输入要分配的进程 要分配的资源数量" << endl;
		cin >> id;
		if(id==9999)break;
		for(int j = 0; j < m; j++) cin >> req[j];
		//做银行家算法分配资源的四个步骤 
		int res = request_resource(id,req);
		switch(res) {
			case 0: cout << "分配成功" << endl; myoutput(); break;
			case 1: cout << "分配失败！请求的资源数超过最大值！" << endl; break; 
			case 2: cout << "分配失败！系统中尚无足够的资源满足P" << id 
			<< "的申请！" << endl;break;
			case 3: cout << "分配失败！假定分配后，无法通过安全性检查！" 
			<< endl;break;
		}
	}
	return 0;
}

