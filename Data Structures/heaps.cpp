#include <cstddef>
#include <iostream>
#include <vector>

struct Node {

  int value{};
  Node *parent{nullptr};
  std::vector<Node> children;

  void print() {
    std::cout << value; // prints the value.

    // if a node has children, prints the child nodes.
    if(!children.empty()){
      std::cout << '\n';
      for(auto child : children){
        child.print();
        std::cout <<" "; 
    }
    std::cout << '\n';
  }
  }
};

/*
TODO:
+ if parent is smaller than child, switch position between parent and child.

TODO:
+ print().
*/

int main() {

  Node v1;
  v1.value = 10;
  Node v2;
  v2.value = 5;
  Node v3;
  v3.value = 2;

  v1.children.push_back(v2);
  v1.children.push_back(v3);

  // v1.print();

  //v2 children
  Node v4,v5,v6;
  v4.value = 1;
  v5.value = 4;
  v6.value = 3;

  v2.children.push_back(v4);
  v2.children.push_back(v5);
  v2.children.push_back(v6);
  
  v1.print();

  v2.print();

}