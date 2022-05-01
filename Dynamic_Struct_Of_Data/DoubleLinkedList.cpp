#include <iostream>
using namespace std;

struct Node {
   int data;
   struct Node* next;
   struct Node* prev;
};

void insert_front(struct Node** head, int new_data)
{
   struct Node* newNode = new Node;
   newNode->data = new_data;

   newNode->next = (*head);
   newNode->prev = NULL;

   if ((*head) != NULL)
   (*head)->prev = newNode;

   (*head) = newNode;
}

void insert_After(struct Node* prev_node, int new_data)
{
   if (prev_node == NULL) {
   cout<<"Previous node is required , it cannot be NULL";
   return;
}

   struct Node* newNode = new Node;

   newNode->data = new_data;

   newNode->next = prev_node->next;

   prev_node->next = newNode;

   newNode->prev = prev_node;

   //set prev of new node's next to newnode
   if (newNode->next != NULL)
   newNode->next->prev = newNode;
}

void insert_end(struct Node** head, int new_data)
{
   //allocate memory for node
   struct Node* newNode = new Node;
   struct Node* last = *head; 

   //set data for new node
   newNode->data = new_data;
   newNode->next = NULL;

   if (*head == NULL) {
   newNode->prev = NULL;
   *head = newNode;
    return;
}

while (last->next != NULL)
last = last->next;
last->next = newNode;
newNode->prev = last;
return;
}

void displayList(struct Node* node) {
   struct Node* last;

   while (node != NULL) {
      cout<<node->data<<"<==>";
      last = node;
      node = node->next;
   }
   if(node == NULL)
   cout<<"NULL";
   }

//main program
int main() {
   struct Node* head = NULL;

   insert_end(&head, 40);
   insert_front(&head, 20);
   insert_front(&head, 10);
   insert_end(&head, 50);
   insert_After(head->next, 30);

   cout<<"Doubly linked list is as follows: "<<endl;
   displayList(head);
   return 0;
}
