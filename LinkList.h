#include<iostream>
using namespace std;
//这里就是一个链表类，完全参照数据结构的，没什么技术含量，不解释了。
#ifndef LINKLIST_H_
#define LINKLIST_H_
template<typename T>
class Node
{
public:
	T data;
	Node<T>* next;
};
template<typename T>
class LinkList
{
public:
	Node<T>* first;
	LinkList();
	~LinkList();
	int Length();
	T Get(int i);
	int Locate(T x);
	void Insert(int i, T x);
	void Delete(int i);
	void PrintList();
};
template<typename T>
LinkList<T>::LinkList()
{
	first = new Node<T>;
	first->next = NULL;
}
template<typename T>
T LinkList<T>::Get(int i)
{
	Node<T>* p = first->next;
	int j = 1;
	while(p && j < i)
	{
		p = p->next;
		j++;
	}
	if(!p)
	{
		throw "位置";
	}
	return p->data;
}
template<typename T>
void LinkList<T>::Insert(int i, T x)
{
	Node<T>* p = first;
	Node<T>* s = new Node<T>; 
	int j = 0;
	while(p && j < i - 1)
	{
		p = p->next;
		j++;
	}
	if(!p)
		cout<<"位置"<<endl;
	else
	{
		s->data = x;
		s->next = p->next;
		p->next = s;
 	}
}
template<typename T>
void LinkList<T>::Delete(int i)
{
	Node<T>* p = first;
	Node<T>* q;
	int j = 0;
	while(p && j < i - 1)
	{
		p = p->next;
		j++;
	}
	if(!p || !p->next)
		throw "位置";
	else
	{
		q = p->next;
		p->next = q->next;
		delete q;
	}
}
template<typename T>
LinkList<T>::~LinkList()
{
	Node<T>* p = first;
	Node<T>* q;
	while(p)
	{
		q = p;
		p = p->next;
		delete q;
	}
}
template<typename T>
int LinkList<T>::Length()
{
	int i = 0;
	Node<T>* p = first;
	while(p->next)
	{
		i++;
		p = p->next;
	}
	return i;
}
template<typename T>
void LinkList<T>::PrintList()
{
	Node<T>* p = first;
	while(p->next)
	{
		p = p->next;
		cout<<p->data<<' ';
	}
	cout<<endl;
}

#endif
