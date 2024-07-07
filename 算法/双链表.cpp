#include<iostream>
using namespace std;
const int M=100010;

//与单链表相区分，双链表不设头结点head,设序号为0的节点为“虚拟头结点”，序号为1的结点为“虚拟尾结点”，
//即在数组中下标为0和1的两个位置不存储任何数据，0和1作为输出双链表的判定链头和链尾的依据
//由于双链表不设头尾结点，故双链表的操作为初始化、任意位置的插入/删除

int e[M],l[M],r[M],idx; //e[M]存储数据，l[M]/r[M]记录某个结点的左右指针，即分别记录某节点前驱和后继的下标，idx用来记录元素的位置

//初始化双链表
void init(){
//初始情况下只有0和1两个虚拟头尾结点
    r[0]=1;
    l[1]=0;
    idx=2;//初始状态下从2开始存储，因为0和1用来表示虚拟头尾结点，不存储任何数据 
}

//在序号为k的右边插入一个新节点
void add(int k,int x){ 
    e[idx]=x;
    r[idx]=r[k];
    l[r[k]]=idx;
    r[k]=idx;
    l[idx]=k;
    idx++;
}

//删除序号为k的节点 
void cut(int k){
    r[l[k]]=r[k];
    l[r[k]]=l[k];
}

int main(){
    int m;
    cin>>m;
    init();
    for(int i=0;i<m;i++){
        string op;  //注意：本题的输入输出格式，由于出现形如“IL/IR”的输入方式，因此不能继续用char型字符变量来表示具体操作，而改用string型字符串变量表示
        int k,x;
        cin>>op;
        if(op=="L"){    //注意！！！比较字符是否相等用单引号，比较字符串是否相等用双引号
            cin>>x;
            add(0,x);   //在双链表的最左边插入节点 等价于 在“头结点”（结点0）的右边插入节点
        }
        else if(op=="R"){
            cin>>x;
            add(l[1],x);//在双链表的最右边插入节点 等价于 在“尾结点”（结点1）的前驱的右边插入节点
        }
        else if(op=="D"){
            cin>>k;
            cut(k+1);   //在本双链表的模拟中，第k个插入元素的下标为k+1，下面同理
        }
        else if(op=="IL"){
            cin>>k>>x;
            add(l[k+1],x);  //在下标为k的左边插入节点 等价于  在下标为k的前驱结点的右边插入节点
        }
        else if(op=="IR"){
            cin>>k>>x;
            add(k+1,x);
        }
    }

    //输出双链表的时候注意双链表的边界判定方式
    for(int i=r[0];i!=1;i=r[i])cout<<e[i]<<' ';
    cout<<endl;
    return 0;
}
