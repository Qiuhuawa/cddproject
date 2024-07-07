#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef struct LNode
{
    int data;
    struct LNode *next;
}LNode, *LinkList;

bool InitList(LinkList &L){
    L = new LNode();
    L->next = NULL;
    return true;
}

int Length(LinkList L){
    int Len = 0;
    LNode *p = L;
    while(p->next != NULL){
        p = p->next;
        Len++;
    }
    return Len;
}

LNode *GetElem(LinkList L, int i){
    LNode *p = L;
    int j = 0;
    while(p != NULL && j < i){
        p = p->next;
        j ++;
    }
    return p;
}

LNode *LocateElem(LinkList L, int i){
    LNode *p = L->next;
    while(p != NULL && p->data != i){
        p = p->next;
    }
    return p;
}

bool ListInsert(LinkList &L, int i, int e){
    LNode *p = L;
    int j = 0;
    while(p != NULL && j < i-1){
        p = p->next;
        j++;
    }
    if(p == NULL){
        return false;
    }
    LNode *s = new LNode();
    s->data = e;
    s->next = p->next;
    p->next = s;
    return true;
}

bool ListDelet(LinkList &L, int i, int e){
    LNode *p = L;
    int j = 0;
    while(p != NULL && j < i-1){
        p = p->next;
        j++;
    }
    if(p == NULL || p->next == NULL){
        return false;
    }
    LNode *s = p->next;
    e = s->data;
    p->next = s->next;
    free(s);
    return true;
}

LinkList List_Start(LinkList &L){
    int x;
    L = new LNode();
    cin >> x;
    LNode *s, *r = L;
    while(x != 9999){
        s = new LNode();
        s->data = x;
        r->next = s;
        r = s;
        scanf("%d", &x);
    }
    r->next = NULL;
    return L;
}

int main(){
    LNode *L = new LNode();
    InitList(L);
    List_Start(L);
    printf("%d\n", L->data);
    printf("%d\n", L->next);
    printf("%d\n", *L->next);
    return 0;
}