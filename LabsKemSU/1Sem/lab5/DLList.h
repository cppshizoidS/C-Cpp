#include <iostream>
using namespace std;
#ifndef DLList_h
#define DLList_h
struct DLList
{
  struct Node
  {
    void* data;
    Node* next;
    Node* prev;
  };
  Node* F = NULL;
  Node* L = NULL;
  Node* C = NULL;
  int Count = 0;
  void Out();
  void Info();
  void Clear();
  bool MoveNext();
  bool MovePrev();
  bool MoveFirst();
  bool MoveLast();
  bool Init(void*);
  bool AddNext(void*);
  bool AddPrev(void*);
  bool AddFirst(void*);
  bool AddLast(void*);
  bool Del(void*&);
  bool DelNext(void*&);
  bool DelPrev(void*&);
  bool DelFirst(void*&);
  bool DelLast(void*&);
  bool Move_k(int);
};
#endif
