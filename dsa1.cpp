#include <stdio.h>
#include <stdlib.h>

class Node {
public:
  int m_data{100}; //test value
  Node *m_next;
};

class LinkedListWrap {
private:
    Node *m_head = nullptr;
    int list_length{0};

public:
  void Insert(int x) { // insert at beginning
    Node *temp = new Node();
    temp->m_data = x;
    temp->m_next = m_head;
    m_head = temp;
    // Adds 1 length to the list.
    list_length++;
  }

  void Insert(int pos, int data) { // insert at nth pos
    Node *temp = new Node();
    temp->m_data = data;
    temp->m_next = nullptr;
    if (pos == 1) {
      temp->m_next = m_head;
      m_head = temp;
      // Adds 1 length to the list
      list_length++;
      return;
    }
    Node *temp1 = m_head;
    for (int i = 0; i < pos - 2 && temp1 != nullptr; i++) {
      temp1 = temp1->m_next;
    }
    if (temp1 == nullptr) {
      printf("Error: Position %d is out of range\n", pos);
      delete temp;
      return;
    }
    temp->m_next = temp1->m_next;
    temp1->m_next = temp;
    // Adds 1 length to the list.
    list_length++;
  }

  void Print() {
    Node *temp = m_head;
    while (temp != nullptr) {
      printf(" %d", temp->m_data);
      temp = temp->m_next;
    }
    printf("\n");
  }

  void Delete(int pos) {
    Node *temp1 = m_head;
    if (pos == 1) {
      m_head = temp1->m_next;
      delete temp1;
      // Removes 1 length from the list.
      list_length--;
      return;
    }

    for (int i = 0; i < pos - 2 && temp1 != nullptr; i++) {
      temp1 = temp1->m_next;
    }
    if (temp1 == nullptr) {
      printf("Error: Position %d is out of range\n", pos);
      return;
    }
    Node *temp2 = temp1->m_next;
    temp1->m_next = temp2->m_next;
    delete temp2;
    // Removes 1 length from the list.
    list_length--;
  }

  void Reverse() {
    Node *r_prev = nullptr;
    Node *r_current = m_head;
    Node *r_next;
    while (r_current != nullptr) {
      r_next = r_current->m_next;
      r_current->m_next = r_prev;
      r_prev = r_current;
      r_current = r_next;
    }
    m_head = r_prev;
  }

  int Length() {
    // Counts the length of the linkedlist through traversal.
    Node *c_temp = m_head;
    int length{0};
    while (c_temp != nullptr) {
      c_temp = c_temp->m_next;
      printf("lenght = %d\n", length);
      length++;
    }
    return length;
  }
  int getLL() { return list_length; }
};

int main() {

    LinkedListWrap List1;

  printf("How many numbers?\n");
  int n, x;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    printf("Enter number: \n");
    scanf("%d", &x);
    List1.Insert(x); // insert at beginning
    List1.Print();
  }

  // These are invalid unless the list is already long enough
  List1.Insert(1, 10);
  List1.Insert(2, 11);
  printf("===== added new =====\n");
  List1.Print();

    List1.Reverse();
  List1.Print();

  int lenght = List1.Length();

  if (lenght == List1.getLL()) {
    printf("The lenghts are in sync. \n");
  } else {
    printf("Something is wrong. \n");
    printf("LLC is %d, Length is %d\n", List1.getLL(), lenght);
  }
}
