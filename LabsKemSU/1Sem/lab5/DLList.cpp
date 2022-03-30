#include <iostream>
#include "DLList.h"
using namespace std;
void DLList::Out()
{
  if(!F)
  {
    cout << "List is empty" << endl;
    return;
  }
  Node* temp = F;
  cout << "List: ";
  do
  {
      cout << temp->data << " ";
      temp = temp->next;
  }
  while(temp);
  cout << endl;
}
void DLList::Info()
{
  if(Count) cout << "List node count: " << Count << endl;
  else cout << "List is empty" << endl;
  if(C) cout << "Current node data = " << C->data << endl;
}
bool DLList::MoveFirst()
{
  if(!F) return false;
  C = F;
  return true;
}
bool DLList::MoveLast()
{
  if(!L) return false;
  C = L;
  return true;
}
bool DLList::MoveNext()
{
  if(!F) return false;
  if(!C)
  {
    C = F;
    return true;
  }
  if(!C->next) return false;
  C = C->next;
  return true;
}
bool DLList::MovePrev()
{
  if(!F) return false;
  if(!C)
  {
    C = F;
    return true;
  }
  if(!C->prev) return false;
  C = C->prev;
  return true;
}
bool DLList::Init(void* data)
{
  if(!F)
  {
    F = new Node;
    L = F;
    C = F;
    F->prev = NULL;
    F->next = NULL;
    F->data = data;
    Count = 1;
    return true;
  }
  else return false;
}
bool DLList::AddNext(void* data)
{
  if(!F) return Init(data);
  Node* temp = C->next;
  C->next = new Node;
  C->next->next = temp;
  C->next->prev = C;
  if(!temp) L = C->next;
  else temp->prev = C->next;
  C = C->next;
  C->data = data;
  Count++;
  return true;
}
bool DLList::AddPrev(void* data)
{
  if(!F) return Init(data);
  Node* temp = C->prev;
  C->prev = new Node;
  C->prev->next = C;
  C->prev->prev = temp;
  if(!temp) F = C->prev;
  else temp->next = C->prev;
  C = C->prev;
  C->data = data;
  Count++;
  return true;
}
bool DLList::AddFirst(void* data)
{
  if(MoveFirst()) return AddPrev(data);
  else return Init(data);
}
bool DLList::AddLast(void* data)
{
  if(MoveLast()) return AddNext(data);
  else return Init(data);
}
bool DLList::DelFirst(void*& data)
{
  if(!F) return false;
  if(C != F) MoveFirst();
  Node* temp = C->next;
  data = C->data;
  if(temp) temp->prev = NULL;
  delete C;
  C = temp;
  F = temp;
  Count--;
  if(!temp)
  {
    L = NULL;
  }
  return true;
}
bool DLList::DelLast(void*& data)
{
  if(!F) return false;
  if(C != L) MoveLast();
  Node* temp = C->prev;
  data = C->data;
  if(temp) temp->next = NULL;
  delete C;
  C = temp;
  L = temp;
  Count--;
  if(!temp)
  {
    F = NULL;
  }
  return true;
}
bool DLList::Del(void*& data)
{
  if(!F) return false;
  if(!C) return false;
  if(C == F) return DelFirst(data);
  if(C == L) return DelLast(data);
  Node* temp = C->next;
  data = C->data;
  C->prev->next = C->next;
  C->next->prev = C->prev;
  delete C;
  C = temp;
  Count--;
  return true;
}
bool DLList::DelNext(void*& data)
{
  if(!F) return false;
  if(!C) return false;
  if(MoveNext()) return Del(data);
  return false;
}
bool DLList::DelPrev(void*& data)
{
  if(!F) return false;
  if(!C) return false;
  if(MovePrev()) return Del(data);
  return false;
}
void DLList::Clear()
{
  if(!MoveFirst())
  {
    cout << "List is empty" << endl;
    return;
  }
  void* k;
  while(Del(k));
}
bool DLList::Move_k(int k)
{
  if(k >= 0 & k < Count)
  {
    if(k == 0)
    {
      return MoveFirst();
    }
    else if(k == (Count - 1))
    {
      return MoveLast();
    }
    else
    {
      if(k < (Count - k))
      {
        MoveFirst();
        for(int i = 0; i < k; i++)
        {
          MoveNext();
        }
      }
      else
      {
        MoveLast();
        for(int i = Count; i > k; i--)
        {
          MovePrev();
        }
      }
      return true;
    }
  }
  else return false;
}
