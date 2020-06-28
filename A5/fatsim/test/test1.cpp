#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// A linked list node
struct Node
{
    int data;
    struct Node *next;
};

//insert a new node in front of the list
void push(struct Node** head, int node_data)
{
    /* 1. create and allocate node */
    struct Node* newNode = new Node;

    /* 2. assign data to node */
    newNode->data = node_data;

    /* 3. set next of new node as head */
    newNode->next = (*head);

    /* 4. move the head to point to the new node */
    (*head) = newNode;
}

//insert new node after a given node
void insertAfter(struct Node* prev_node, int node_data)
{
    /*1. check if the given prev_node is NULL */
    if (prev_node == NULL)
    {
        cout<<"the given previous node is required,cannot be NULL"; return; }

    /* 2. create and allocate new node */
    struct Node* newNode =new Node;

    /* 3. assign data to the node */
    newNode->data = node_data;

    /* 4. Make next of new node as next of prev_node */
    newNode->next = prev_node->next;

    /* 5. move the next of prev_node as new_node */
    prev_node->next = newNode;
}

/* insert new node at the end of the linked list */
void append(struct Node** head, int node_data)
{
/* 1. create and allocate node */
    struct Node* newNode = new Node;

    struct Node *last = *head; /* used in step 5*/

/* 2. assign data to the node */
    newNode->data = node_data;

/* 3. set next pointer of new node to null as its the last node*/
    newNode->next = NULL;

/* 4. if list is empty, new node becomes first node */
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

/* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

/* 6. Change the next of last node */
    last->next = newNode;
    return;
}

// display linked list contents
void displayList(struct Node *node)
{
    //traverse the list to display each node
    while (node != NULL)
    {
        cout<<node->data<<"-->";
        node = node->next;
    }

    if(node== NULL)
        cout<<"null";
}

//delete first node in the linked list
Node* deleteFirstNode(struct Node* head)
{
    if (head == NULL)
        return NULL;

    // Move the head pointer to the next node
    Node* tempNode = head;
    head = head->next;
    delete tempNode;

    return head;
}
//delete last node from linked list
Node* removeLastNode(struct Node* head){
    if (head == NULL)
        return NULL;

    if (head->next == NULL) {
        delete head;
        return NULL;
    }

    // first find second last node
    Node* second_last = head;
    while (second_last->next->next != NULL)
        second_last = second_last->next;

     // Delete the last node
    delete (second_last->next);

    // set next of second_last to null
    second_last->next = NULL;

    return head;
}

/* Given a reference (pointer to pointer) to the head of a list
   and a key, deletes the first occurrence of key in linked list */
void deleteNode(struct Node **head_ref, int key)
{
    // Store head node
    struct Node* temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->data == key)
    {
        *head_ref = temp->next;   // Changed head
        free(temp);               // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);  // Free memory
}

/* main program for linked list*/
int main()
{
/* empty list */
    struct Node* head = NULL;

// Insert 10.
    append(&head, 10);

// Insert 20 at the beginning.
    push(&head, 20);

// Insert 30 at the beginning.
    push(&head, 30);

// Insert 40 at the end.
    append(&head, 40); //

    //Insert 50, after 20.
    insertAfter(head->next, 50);

    cout<<"Final linked list: \n"<<endl;
    displayList(head);

    return 0;
}
//vector removal
//删除最后一个的时候删除的是it2--
//删除vector[i] => it1 + i; 包括0
//增加到vector [i] => it 1 + i
//改变数据 vector[5] = ??
//改变数据 vecofvec[0][1] = 5;
 //   vector<int> myvector{ 1, 2, 3, 4, 5 };
//    vector<int>::iterator it1, it2;
//
//    it1 = myvector.begin();
//    it2 = myvector.end();
//    it2--;
//    it2--;
//    myvector.erase (it2);
//   for (auto it1 = myvector.begin(); it1 != myvector.end(); ++it1)
//        cout << ' ' << *it1;
//    return 0;

//insert
//int main()
//{
//    // initialising the vector
//    vector<int> vec = { 10, 20, 30, 40 };
//
//    // inserts 3 at front
//    auto it = vec.insert(vec.begin(), 3);
//    // inserts 2 at front
//    vec.insert(it, 2);
//
//    int i = 2;
//    // inserts 7 at i-th index
//    it = vec.insert(vec.begin() + i, 7);
//
//    cout << "The vector elements are: ";
//    for (auto it = vec.begin(); it != vec.end(); ++it)
//        cout << *it << " ";
//
//    return 0;
//}

//found element
/*int main()
{
    std::vector<int> vecOfNums = { 12, 45, 54, 33, 2, 7, 8, 22, 43, 19 };
    int index = 0;

    bool found = false;
// Iterate over all elements in Vector
    for (auto & elem : vecOfNums)
    {
        if (elem == 22)
        {
            found = true;
            std::cout << index << std::endl;
            break;
        }
        index ++ ;
    }
*/


//free = -1  else = tag
// if 找不到合适大的chunk 需求更多的空间在后面
// else 放在第一个找到的chunk里面

//search = true;
//       i =
//       it1 = vecOfNums.start() + i;
//       删除vector[i] => it1 + i; 包括0
//       vector[i][1]
//       vecOfNums.erase(it1);
//       vecOfNums.insert(it1, [-1, vector[i][1] - input[1]]);
//       vecOfNums.insert(it1, [[input[0], input[1]] );
//
//       ==
//       i =
//       it1 = vecOfNums.start() + i;
//       vecOfNums.erase(it1);
//       vecOfNums.insert(it1, [[input[0], input[1]] );
//
//[if search = false ]
//  if (vector[vector.size() - 1]][0] < 0]
//        int size = (input[1] / pagesize) + 1
//        long add_size = pagesize * size
//        push_back[input[0], input[1]];
//        push_back[-1, pagesize - input[1]];
//  else
//         long = input[1] - vector[vector.size() - 1]][0];  // 5900 - 999 = 4901
//         int size = (long / pagesize) + 1;  // 5
//         long add_size = pagesize * size; //5000
//         long kk = add_size + vector[vector.size() - 1]][0] // 5000 + 999
//         std::vector<???>::iterator it1
//         it1 = vecOfNums.end() - 1;
//         myvector.erase (it1);
//         push_back[input[0], input[1]];
//         push_back[-1, kk - input[0]]

//free
//  for
//   if(input[0] == vector[i][0])
//   if  i == 0
//       check right
//
//   if  i == vector.size() - 1
//       check left
//
//   if  (i  > 0 && i < vector.size() - 1)
//       it1
//       it1 + i

//       check left
//       check it + i - 1
//       if < 0
//       it2
//       it2 + ( i- 1)
//       long = [1]
//       erase (it2)
//       insert (it2,  )
//
//       check right
//
//
//





// push_back[-1, multiple * 1000]

//vector [-1, 1000],  ->  []
//vector [5, 100] [9,100]

