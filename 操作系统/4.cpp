#define TRUE 1
#define FALSE 0
#define INVALID -1
#define NULL  0
#define  total_instruction 320     /*指令流长*/
#define  total_vp  32               /*虚页长*/
#define  clear_period  50           /*清0周期*/
 
typedef struct                      /*页面结构*/
{
	int pn;      //页号 logic number
	int pfn;     //页面框架号 physical frame number
	int counter; //计数器
	int time;    //时间
}pl_type;
 
pl_type pl[total_vp];                      /*页面线性结构---指令序列需要使用地址*/
 
typedef struct pfc_struct                  /*页面控制结构，调度算法的控制结构*/
{                          
    int pn;
	int pfn;
	struct pfc_struct *next;
}pfc_type;
 
 
pfc_type pfc[total_vp], *freepf_head, *busypf_head, *busypf_tail;
 
int diseffect,  a[total_instruction]; /* a[]为指令序列*/
 
int page[total_instruction],  offset[total_instruction];/*地址信息*/
 
int  initialize(int);
int  FIFO(int);
int  LRU(int);
int  LFU(int);
int  NUR(int); //not use recently
int  OPT(int);
 
int main( )
{
	int s,i,j;
 
	srand(10*getpid());                    /*由于每次运行时进程号不同，故可用来作为初始化随机数队列的“种子”*/
 
	s=(float)319*rand( )/32767/32767/2+1;  /*正态分布*/
 
	for(i=0;i<total_instruction;i+=4)        /*产生指令队列*/
	{
		if(s<0||s>319)
		{
			printf("When i==%d,Error,s==%d\n",i,s);
			exit(0);
		} 
		a[i]=s;                                   /*任选一指令访问点m*/
		a[i+1]=a[i]+1;                            /*顺序执行一条指令*/
		a[i+2]=(float)a[i]*rand( )/32767/32767/2; /*执行前地址指令m*/
		a[i+3]=a[i+2]+1;                          /*顺序执行一条指令*/
 
		s=(float)(318-a[i+2])*rand( )/32767/32767/2+a[i+2]+2;
		if((a[i+2]>318)||(s>319))
 
			printf("a[%d+2],a number which is :%d and s==%d\n",i,a[i+2],s);
 
	}
	for (i=0;i<total_instruction;i++) /*将指令序列变换成页地址流*/
	{
		page[i]=a[i]/10;
		offset[i]=a[i]%10;
	}
	for(i=4;i<=32;i++)   /*用户内存工作区从4个页面到32个页面*/
	{
		printf("---%2d page frames---\n",i);
		FIFO(i);
		LRU(i);
		LFU(i);
		NUR(i);
		OPT(i);
 
	}
	return 0;
}
 
/*初始化相关数据结构 total_pf表示内存的块数 */
 
int initialize(int total_pf)             
{
	int i;
	diseffect=0;
	for(i=0;i<total_vp;i++)
	{
 
		pl[i].pfn=INVALID;       /*置页面控制结构中的页号，页面为空*/
		pl[i].counter=0;         /*页面控制结构中的访问次数为0*/
		pl[i].time=-1;           /*访问的时间*/
	}
 
	for(i=0;i<total_pf-1;i++)	/*建立pfc[i-1]和pfc[i]之间的链接*/
	{	
		pfc[i].next=&pfc[i+1];
		pfc[i].pfn=i;
	}   
 
	pfc[total_pf-1].next=NULL;
	pfc[total_pf-1].pfn=total_pf-1;
	freepf_head=&pfc[0];         /*空页面队列的头指针为pfc[0]*/
	return 0;
}
 
int FIFO(int total_pf)              /*先进先出算法total_pf:用户进程的内存页面数*/
{
	int i,j;
	pfc_type *p;					/*中间变量*/
	initialize(total_pf);         /*初始化相关页面控制用数据结构*/
	busypf_head=busypf_tail=NULL; /*忙页面队列头，队列尾链接*/
	for(i=0;i<total_instruction;i++)
	{
		if(pl[page[i]].pfn==INVALID)   /*页面失效*/
		{
			diseffect+=1;                  /*失效次数*/
			if(freepf_head==NULL)         /*无空闲页面*/
			{
				p=busypf_head->next;       
				pl[busypf_head->pn].pfn=INVALID;
				freepf_head=busypf_head;  /*释放忙页面队列的第一个页面*/
				freepf_head->next=NULL;  /*表明还是缺页*/
				busypf_head=p;
			}
			p=freepf_head->next;        
			freepf_head->pn=page[i];
			pl[page[i]].pfn=freepf_head->pfn;
			freepf_head->next=NULL; /*使busy的尾为null*/
			if(busypf_tail==NULL)
			{
				busypf_tail=busypf_head=freepf_head;
			}
			else
			{
				busypf_tail->next=freepf_head;
				busypf_tail=freepf_head;
			}
			freepf_head=p;
		}
	}
	printf("FIFO:%6.4f ",1-(float)diseffect/320);
	return 0;
}
int LRU (int total_pf)       /*最近最久未使用算法least recently used*/
{
	int min,minj,i,j,present_time; /*minj为最小值下标*/
	initialize(total_pf);
	present_time=0;
	for(i=0;i<total_instruction;i++)
	{
		if(pl[page[i]].pfn==INVALID)             /*页面失效*/
		{
			diseffect++;
			if(freepf_head==NULL)              /*无空闲页面*/
			{
				min=32767;						/*设置最大值*/
				for(j=0;j<total_vp;j++)            /*找出time的最小值*/
				{ 
					if(min>pl[j].time&&pl[j].pfn!=INVALID)
					{
						min=pl[j].time;
						minj=j;
					}
				}
				freepf_head=&pfc[pl[minj].pfn];   //腾出一个单元
				pl[minj].pfn=INVALID;
				pl[minj].time=0;
				freepf_head->next=NULL;
			}
			pl[page[i]].pfn=freepf_head->pfn;   //有空闲页面,改为有效
			pl[page[i]].time=present_time;
			freepf_head=freepf_head->next;      //减少一个free 页面
		}
		else
		{
			pl[page[i]].time=present_time;        //命中则增加该单元的访问次数
			present_time++;
		}
	}
	printf("LRU:%6.4f ",1-(float)diseffect/320);
	return 0;
}
 
int NUR(int  total_pf )                  /*最近未使用算法Not Used recently count表示*/
{ 
int i,j,dp,cont_flag,old_dp;
pfc_type *t;
initialize(total_pf);
dp=0;
 
for(i=0;i<total_instruction;i++)
{ 
	if (pl[page[i]].pfn==INVALID)         /*页面失效*/
	{
		diseffect++;
		if(freepf_head==NULL)               /*无空闲页面*/
		{ 
			cont_flag=TRUE;
			old_dp=dp;
			
			while(cont_flag)
			{
				
			   if(pl[dp].counter==0&&pl[dp].pfn!=INVALID)
					cont_flag=FALSE;
				else
				{
					dp++;
					if(dp==total_vp)
						dp=0;
					if(dp==old_dp)
						for(j=0;j<total_vp;j++)
						 pl[j].counter=0;
				}
			}
			freepf_head=&pfc[pl[dp].pfn];
			pl[dp].pfn=INVALID;
			freepf_head->next=NULL;
		}
		
		pl[page[i]].pfn=freepf_head->pfn;
		
		freepf_head->pn=page[i];
		
		freepf_head=freepf_head->next;
	}
	else
		pl[page[i]].counter=1;
	if(i%clear_period==0)
		for(j=0;j<total_vp;j++)
			pl[j].counter=0;
}
printf("NUR:%6.4f ",1-(float)diseffect/320);
return 0;
}
 
int OPT(int total_pf)       /*最佳置换算法*/
{
	int i,j, max,maxpage,d,dist[total_vp];
	pfc_type *t;
	initialize(total_pf);
	for(i=0;i<total_instruction;i++)
	{ 
		if(pl[page[i]].pfn==INVALID)      /*页面失效*/
		{
			diseffect++;
			if(freepf_head==NULL)         /*无空闲页面*/
			{
				for(j=0;j<total_vp;j++)
				{
					if(pl[j].pfn!=INVALID)
						dist[j]=32767;
					else
						dist[j]=0;	 
				}
				for(j=0;j<total_vp;j++)	       
				{
					if((pl[j].pfn!=INVALID)&&(dist[j]==32767))
					{
						dist[j]=j;
					}
				}
				max=0;
				for(j=0;j<total_vp;j++)
					if(max<dist[j])
					{
						max=dist[j];
						maxpage=j;
					}
					freepf_head=&pfc[pl[maxpage].pfn];
					freepf_head->next=NULL;
					pl[maxpage].pfn=INVALID;
			}
			pl[page[i]].pfn=freepf_head->pfn;
			freepf_head=freepf_head->next;
		}
	}
	printf("OPT:%6.4f\n",1-(float)diseffect/320);
	return 0;
}
/*该算法时根据已知的预测未知的，least frequency  Used是最不经常使用置换法*/
int  LFU(int total_pf)        
{
	int i,j,min,minpage;
	pfc_type *t;
	initialize(total_pf);
	for(i=0;i<total_instruction;i++)
	{ 
		if(pl[page[i]].pfn==INVALID)      /*页面失效*/
		{ 
			diseffect++;
			if(freepf_head==NULL)          /*无空闲页面*/
			{ 
				min=32767;	
				/*获取counter的使用用频率最小的内存*/	
				for(j=0;j<total_vp;j++)
				{
					if(min>pl[j].counter&&pl[j].pfn!=INVALID)
					{
						min=pl[j].counter;
						minpage=j;
					}
				}
				freepf_head=&pfc[pl[minpage].pfn];
				pl[minpage].pfn=INVALID;
				pl[minpage].counter=0;
				freepf_head->next=NULL;
			}
			pl[page[i]].pfn=freepf_head->pfn;   //有空闲页面,改为有效
			pl[page[i]].counter++;
			freepf_head=freepf_head->next;      //减少一个free 页面
		}
		else
		{
			pl[page[i]].counter;
			pl[page[i]].counter=pl[page[i]].counter+1;
		}
	}
	printf("LFU:%6.4f ",1-(float)diseffect/320);
	return 0;
}	
 
 
 
//2.在可变分区管理方式下采用最先适应算法实现主存分配和实现主存回收。
#include<iostream>
#include<stdlib.h>
using namespace std;
 
#define Free 0 //空闲状态
#define Busy 1 //已用状态
#define OK 1    //完成
#define ERROR 0 //出错
#define MAX_length 640 //最大内存空间为640KB
typedef int Status;
int flag;
 
typedef struct freearea//定义一个空闲区说明表结构
{
    long size;   //分区大小
    long address; //分区地址
    int state;   //状态
}ElemType;
 
// 线性表的双向链表存储结构
typedef struct DuLNode
{
    ElemType data;
    struct DuLNode *prior; //前趋指针
    struct DuLNode *next;  //后继指针
}
 
DuLNode,*DuLinkList;
DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点
Status alloc(int);//内存分配
Status free(int); //内存回收
Status First_fit(int);//首次适应算法
Status Best_fit(int); //最佳适应算法
Status Worst_fit(int); //最差适应算法
void show();//查看分配
Status Initblock();//开创空间表
 
Status Initblock()//开创带头结点的内存空间链表
{
    block_first=(DuLinkList)malloc(sizeof(DuLNode));
    block_last=(DuLinkList)malloc(sizeof(DuLNode));
    block_first->prior=NULL;
    block_first->next=block_last;
    block_last->prior=block_first;
    block_last->next=NULL;
    block_last->data.address=0;
    block_last->data.size=MAX_length;
    block_last->data.state=Free;
    return OK;
}
 
//分配主存
Status alloc(int ch)
{
    int request = 0;
    cout<<"请输入需要分配的主存大小(单位:KB)：";
    cin>>request;
    if(request<0 ||request==0)
    {
        cout<<"分配大小不合适，请重试！"<<endl;
        return ERROR;
    }
 
    if(ch==2) //选择最佳适应算法
    {
        if(Best_fit(request)==OK) cout<<"分配成功！"<<endl;
        else cout<<"内存不足，分配失败！"<<endl;
        return OK;
    }
	if(ch==3) //选择最差适应算法
    {
        if(Worst_fit(request)==OK) cout<<"分配成功！"<<endl;
        else cout<<"内存不足，分配失败！"<<endl;
        return OK;
    }
    else //默认首次适应算法
    {
        if(First_fit(request)==OK) cout<<"分配成功！"<<endl;
        else cout<<"内存不足，分配失败！"<<endl;
        return OK;
    }
}
 
//首次适应算法
Status First_fit(int request)
{
    //为申请作业开辟新空间且初始化
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.size=request;
    temp->data.state=Busy;
 
    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.state==Free && p->data.size==request)
        {//有大小恰好合适的空闲块
            p->data.state=Busy;
            return OK;
            break;
        }
        if(p->data.state==Free && p->data.size>request)
        {//有空闲块能满足需求且有剩余
            temp->prior=p->prior;
            temp->next=p;
            temp->data.address=p->data.address;
            p->prior->next=temp;
            p->prior=temp;
            p->data.address=temp->data.address+temp->data.size;
            p->data.size-=request;
            return OK;
            break;
        }
        p=p->next;
    }
    return ERROR;
}
 
 
 
//最佳适应算法
Status Best_fit(int request)
{
    int ch; //记录最小剩余空间
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.size=request;
    temp->data.state=Busy;
    DuLNode *p=block_first->next;
    DuLNode *q=NULL; //记录最佳插入位置
 
    while(p) //初始化最小空间和最佳位置
    {
        if(p->data.state==Free && (p->data.size>=request) )
        {
			if(q==NULL)
			{
				q=p;
				ch=p->data.size-request;
			}
			else if(q->data.size > p->data.size)
			{
				q=p;
				ch=p->data.size-request;
			}
        }
        p=p->next;
    }
 
    if(q==NULL) return ERROR;//没有找到空闲块
    else if(q->data.size==request)
    {
        q->data.state=Busy;
        return OK;
    }
	else
	{
        temp->prior=q->prior;
        temp->next=q;
        temp->data.address=q->data.address;
        q->prior->next=temp;
        q->prior=temp;
        q->data.address+=request;
        q->data.size=ch;
        return OK;
    }
	return OK;
}
 
//最差适应算法
Status Worst_fit(int request)
{
    int ch; //记录最大剩余空间
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.size=request;
    temp->data.state=Busy;
    DuLNode *p=block_first->next;
    DuLNode *q=NULL; //记录最佳插入位置
 
    while(p) //初始化最大空间和最佳位置
    {
        if(p->data.state==Free && (p->data.size>=request) )
        {
			if(q==NULL)
			{
				q=p;
				ch=p->data.size-request;
			}
			else if(q->data.size < p->data.size)
			{
				q=p;
				ch=p->data.size-request;
			}
        }
        p=p->next;
    }
 
    if(q==NULL) return ERROR;//没有找到空闲块
    else if(q->data.size==request)
    {
        q->data.state=Busy;
        return OK;
    }
	else
	{
        temp->prior=q->prior;
        temp->next=q;
        temp->data.address=q->data.address;
        q->prior->next=temp;
        q->prior=temp;
        q->data.address+=request;
        q->data.size=ch;
        return OK;
    }
	return OK;
}
 
//主存回收
Status free(int flag)
{
    DuLNode *p=block_first;
	for(int i= 0; i <= flag; i++)
		if(p!=NULL)
			p=p->next;
		else
			return ERROR;
 
	p->data.state=Free;
    if(p->prior!=block_first && p->prior->data.state==Free)//与前面的空闲块相连
    {
        p->prior->data.size+=p->data.size;
        p->prior->next=p->next;
        p->next->prior=p->prior;
		p=p->prior;
    }
    if(p->next!=block_last && p->next->data.state==Free)//与后面的空闲块相连
    {
        p->data.size+=p->next->data.size;
        p->next->next->prior=p;
        p->next=p->next->next;
    }
	if(p->next==block_last && p->next->data.state==Free)//与最后的空闲块相连
    {
		p->data.size+=p->next->data.size;
        p->next=NULL;
    }
 
    return OK;
}
 
//显示主存分配情况
void show()
{
	int flag = 0;
    cout<<"\n主存分配情况:\n";
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    DuLNode *p=block_first->next;
	cout<<"分区号\t起始地址\t分区大小\t状态\n\n";
    while(p)
    {
        cout<<"  "<<flag++<<"\t";
        cout<<"  "<<p->data.address<<"\t\t";
        cout<<" "<<p->data.size<<"KB\t\t";
        if(p->data.state==Free) cout<<"空闲\n\n";
        else cout<<"已分配\n\n";
        p=p->next;
    }
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}
 
//主函数
void main()
{
    int ch;//算法选择标记
    cout<<"请输入所使用的内存分配算法：\n";
    cout<<"(1)首次适应算法\n(2)最佳适应算法\n(3)最差适应算法\n";
 
    cin>>ch;
	while(ch<1||ch>3)
	{
		cout<<"输入错误，请重新输入所使用的内存分配算法：\n";
		cin>>ch;
	}
    Initblock(); //开创空间表
    int choice;  //操作选择标记
    while(1)
    {
		show();
		cout<<"请输入您的操作：";
        cout<<"\n1: 分配内存\n2: 回收内存\n0: 退出\n";
 
        cin>>choice;
        if(choice==1) alloc(ch); // 分配内存
        else if(choice==2)  // 内存回收
        {
            int flag;
            cout<<"请输入您要释放的分区号：";
            cin>>flag;
            free(flag);
        }
        else if(choice==0) break; //退出
        else //输入操作有误
        {
            cout<<"输入有误，请重试！"<<endl;
            continue;
        }
    }
}
 
 
 
//3.在分页式管理方式下采用位示图来表示主存分配情况，实现主存空间的分配和回收。
//（1）链表方式：
#include <iostream>
#include <string>
using namespace std;
 
struct ListNode  //链表的节点
{
    int begin;
    int end;
    int size;
    int num;
    bool freeable;
    ListNode *next;
};
 
typedef ListNode *  ListPtr;
 
class Mem
{
public:
	Mem();
    void getNumStringrequest();
    void getNumStringfree();
    void getrequest();
    void getfreerequest();
    // int getNum();
    //void Insert();
    //void Delete();
    void Print();
    void freemem();
    void getmem();
private:
	ListPtr first;
    string request;
    int requestnum;
    int freenum;
    string freerequest;
	string str;
};
 
Mem::Mem()   //初始化，把first置为空
{
	first=new ListNode;
	first->next=NULL;
	str="";
}
void Mem::getrequest()
{
    cout<<"请输入内存申请请求："<<endl;
    cin>>request;
	str=str+request;
}
 
void Mem::getfreerequest()   //每次的请求都放入str中
{
    cout<<"请输入内存释放申请请求："<<endl;
    cin>>freerequest;
	str=str+freerequest;
}
 
void Mem::getNumStringrequest()
{
    int len=request.length();
    string numstring=request.substr(1,len-1);
    requestnum=atoi(numstring.c_str());
    cout<<"申请内存的大小是："<<requestnum<<endl;
 
}
void Mem::getNumStringfree()   //使用atoi函数将char *（string通过strin.c_str())可以得到char *类型
{
    int len=freerequest.length();
    string freestring=freerequest.substr(5,len-2);
    freenum=atoi(freestring.c_str());
    cout<<"释放内存的起始地址是："<<freenum<<endl;
}
 
void Mem::freemem()
{
    ListNode *p=first;
    while(p->next!=NULL)
    {
        if(p->next->begin==freenum&&p->next->freeable==false)
        {
            cout<<"释放内存！"<<p->next->num<<endl;
            p->next->freeable=true;
        }
        ListNode *q=p->next;
        if(q->freeable==true)   //采用向前合并的原则
        {
 
            if(q->next!=NULL&&q->next->freeable==true&&p->freeable==true)  //前后均为空
            {
                cout<<"释放内存的前后都为空，因此将三块内存合并！"<<endl;  //3块内存合并到最前面一块
                p->size=p->size+q->size+q->next->size;
                p->freeable=true;
                p->end=p->begin+p->size;
				ListNode *k=q->next;
				p->next=k->next;
                delete q;
				delete k;
            }
            else if(p->freeable==true&&q->next!=NULL&&q->next->freeable==false)  //前为空，后面不空
            {
                cout<<"释放内存的前一块是空的，合并"<<endl;
                p->size=p->size+q->size;
                p->freeable=true;
                p->end=p->begin+p->size;
				p->next=q->next;
                delete q;
            }
 
			else if(p->freeable==false&&q->freeable==true&&q->next->freeable==true)  //前面空后面不空
			{
			cout<<"释放内存的后一块是空的，合并！"<<endl;
            ListNode * k=q->next;
            q->size=q->size+k->size;
            q->freeable=true;
            q->next=k->next;
            q->end=q->begin+q->size;
            delete k;
			}
 
        }
 
 
		p=p->next;
    }
 
}
 
void Mem::getmem()
{
    ListNode * p=first->next;
    ListNode *q;
    if(p==NULL)  //第一次申请内存
    {
        cout<<"第一次申请内存！"<<endl;
        q=new ListNode;
        q->begin=0;
        q->freeable=false;
        q->size=requestnum;
        q->end=q->begin+q->size;
        q->num=1;
        first->next=q;
        cout<<"内存大小："<<q->size<<"  内存起始地址："<<q->begin<<"  结束地址:"<<q->end<<"  内存编号:"<<q->num<<endl;
        q->next=NULL;
    }
    else  //前面有释放的内存。不向后面才查找
    {
        bool find=false;
		p=first;
        while(p->next!=NULL&&find!=true)
        {
            if(p->next!=NULL&&p->next->size>requestnum&&p->next->freeable==true)
            {
                cout<<"找到空的内存："<<endl;
                cout<<"内存大小："<<p->next->size<<"  内存起始地址："<<p->next->begin<<"  结束地址:"<<p->next->end<<"  内存编号:"<<p->next->num<<"  内存状态："<<p->freeable<<endl;
                cout<<"将大内存分割："<<endl;
                ListNode *temp=p->next;
                ListNode *k=new ListNode;
               // k=p->next;
                k->begin=p->next->begin;
                k->freeable=false;
                k->size=requestnum;
                k->end=p->next->begin+k->size;
                k->num=temp->num;
			    p->next=k;
 
                ListNode *l=new ListNode;
 
                l->begin=k->end+1;
                l->size=temp->size-k->size;
                l->end=l->begin+l->size;
                l->freeable=true;
                l->num=k->num;
                l->next=temp->next;
 
				k->next=l;
                find=true;
 
                delete temp;
            }
			p=p->next;
        }
 
        if(false==find)  //前面没找到合适的内存，因此开辟一块新的内存
        {
			p=first;
            cout<<"开辟一块新的内存！"<<endl;
            ListNode *q=new ListNode;
            while(p->next!=NULL)
                p=p->next;
            q->begin=p->end+1;
            q->end=q->begin+requestnum;
            q->freeable=false;
            q->num=p->num+1;
            q->size=requestnum;
			p->next=q;
            q->next=NULL;
 
        }
    }
}
 
void Mem::Print()
{
	cout<<"整个内存分配状况："<<endl;
	ListNode *p=first->next;
	while(p!=NULL)
	{
 
		cout<<"内存大小："<<p->size<<"  内存起始地址："<<p->begin<<"  内存末地址"<<p->end<<"  内存标号"<<p->num<<" 内存状态"<<p->freeable<<endl;
		p=p->next;
	}
	cout<<"申请释放过程："<<str<<endl;
}
int main()
{
    Mem mem;
	string str="";
	char quit='n';
	while(quit!='Y'&&quit!='y')  //采用while一直循环实行模拟内存的申请与释放
	{
		int chose;
	cout<<" ============================================"<<endl;
	cout<<"    1.内存申请     "<<endl;
	cout<<"    2.内存释放     "<<endl;
	cout<<"    3.显示内存状态 (状态0表示正在使用，不可以被释放，状态1表示未被使用，可以释放"<<endl;
	cout<<"    4.退出          "<<endl;
	cout<<" ============================================"<<endl;
	cin>>chose;
	switch(chose)
	{
	case 1:
		mem.getrequest();
		mem.getNumStringrequest();
		mem.getmem();
		mem.Print();
		break;
	case 2:
		mem.getfreerequest();
		mem.getNumStringfree();
		mem.freemem();
		mem.Print();
		break;
	case 3:
		mem.Print();
	case 4:   //一旦用户选择退出，那么置quit为YES退出程序
		quit='y';
	default:
		break;
	}
	}
    /*mem.getrequest();
    mem.getNumStringrequest();
	mem.getmem();
	mem.Print();
	mem.freemem();
	mem.Print();
    mem.getfreerequest();
    mem.getNumStringfree();
	*/
    return 0;
 
}
 
//（2）位示图：
#include <stdlib.h>
#include <stdio.h>
typedef int datatype;
 
typedef struct node
{
  datatype pageNum,blockNum;
  struct node *next;
}linknode;
 
typedef linknode *linklist;
 
linklist creatlinklist(int n)/*尾插法创建带头结点的单链表*/
{
    linklist head,r,s;
	int x,y,i=0;
    head=r=(linklist)malloc(sizeof(linknode));
    printf("\n请分别输入页表的页号及块号（-1表示空）：\n");
	printf("\n页号 | 块号\n");
    while (i<n)
    {
		scanf("%d %d",&x,&y);
        s=(linklist)malloc(sizeof(linknode));
        s->pageNum=x;
		s->blockNum=y;
        r->next=s;
        r=s;
		i++;
    }
    r->next=NULL;
    return head;
}
 
void init(int g[100][100],int N)/*初始化位示图，将值全置为零，0表示空闲状态*/
{
	int i,j;
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
		{
			g[i][j]=0; //全置为零
		}
	}
	g[N+1][0]=N*N; //在数组最后一个数的后面设置一个空间用来存放剩余空闲块数
}
 
linklist Init(linklist head,int g[100][100],int n,int N)
{
  linklist p;
	int i,j;
	p=head->next;
	if(n<=g[N+1][0]) //首先判断作业的页数是否小于等于位示图剩余空闲块的个数
	{
		while(p)
		{
			i=p->blockNum/N;
		    j=p->blockNum%N;
			g[i][j]=1;
			g[N+1][0]--;
			p=p->next;
		}
	}
  return head;
 
 
}
void printStr(int g[100][100],int N)/*打印位示图*/
{
	int i,j;
	printf("\n此时位示图为：\n");
	printf("\n ");
	for(i=0;i<N;i++)
	{
		printf(" ");
		printf("%d",i);
	}
	printf("\n");
	for(i=0;i<N;i++)
	{
		printf("%d",i);
		for(j=0;j<N;j++)
		{
			printf(" ");
			printf("%d",g[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void print(linklist head)/*输出带头结点的单链表*/
{
    linklist p;
    p=head->next;
    printf("\n该页表为:\n");
	printf("\n");
	printf("\n         页号 | 块号\n");
    while(p)
    {
        printf("%11d%7d\n",p->pageNum,p->blockNum);
        p=p->next;
    }
 
 
linklist Dis(linklist head,int g[100][100],int n,int N){
	linklist p;
	int i,j;
	p=head->next;
	if(n<=g[N+1][0]){//首先判断作业的页数是否小于等于位示图剩余空闲块的个数
		while(p){
			for(i=0;i<N;i++){
				for(j=0;j<N;j++){
					if(g[i][j]==0){
						p->blockNum=N*i+j; //将对应块号记录到页表
						g[i][j]=1; //将块置1，表示已被占用
						g[N+1][0]--; //剩余空闲块减1
					    break; //跳出循环，进行下一个页的分配
					}
				}
				break; //跳出循环
			}
		    p=p->next; //下一个页进行分配
		}
	    return head;
	}
}

linklist Recy(linklist head,int g[100][100],int n,int N)/*回收已经完成的页*/
{
	int i,j;
	linklist p;
	p=head->next;
	while(p&&p->pageNum!=n) //找出要回收的页号
	{
		p=p->next;
	}
 
 
if(p) //找到
	{
		i=p->blockNum/N;
		j=p->blockNum%N;
		g[i][j]=0; //将该块置0，表空闲状态
		g[N+1][0]++;
		p->blockNum=-1; //页表中对应的块号为空，置成-1
	}
	return head;
}
void main()
{
	int m,n,N;
	int x,y,a,b,t;
	int graph[100][100];
	linklist head,Head;
   printf("\n*****分页式存储管理分配及回收算法*****\n");
	printf("\n请输入位示图字长：");
	scanf("%d",&N);
    printf("\n请输入已占用内存作业的页数：");
	scanf("%d",&m);
    head=creatlinklist(m);
    init(graph,N);
	head=Init(head,graph,m,N);
    printStr(graph,N);
	printf("\n当前空闲块数为：%d",graph[N+1][0]);
    printf("\n\n现在进行作业分配：\n");
    printf("\n请输入需要分配的作业的页数：");
	scanf("%d",&n);
    Head=creatlinklist(n);
    Head=Dis(Head,graph,n,N);
	print(Head);
    printStr(graph,N);
    printf("\n当前空闲块数为：%d",graph[N+1][0]);
    printf("\n\n您是否想回收已完成的页，“是”请按1，“否”请按0：");
	scanf("%d",&x);
if(x) //判断是否要回收
	{
		printf("\n请输入您要回收的页号：");
		scanf("%d %d %d %d",&y,&a,&b,&t);
        head=Recy(head,graph,y,N);
        head=Recy(head,graph,a,N);
        head=Recy(head,graph,b,N);
        head=Recy(head,graph,t,N);
        printStr(graph,N);
	}
    printf("\n当前空闲块数为：%d",graph[N+1][0]);
    printf("\n");
}