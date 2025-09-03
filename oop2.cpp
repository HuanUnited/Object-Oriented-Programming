#include <cstdlib>
#include <cstring>
#include <iostream>


class Cstr {
private:
  char *chararr;

public:
  // Default constructor
  Cstr() {
    int random = rand() % 20 + 1; // 1..20
    chararr = new char[random + 1];
    chararr[0] = '\0';
    std::cout << "Random constructor called\n";
  }

  // Length constructor
  Cstr(int length) {
    chararr = new char[length + 1];
    chararr[0] = '\0';
    std::cout << "Length constructor called\n";
  }

  // String constructor
  Cstr(const char *str) {
    chararr = new char[strlen(str) + 1];
    strcpy(chararr, str);
    std::cout << "String constructor called\n";
  }

  // Copy constructor (deep copy)
  Cstr(const Cstr &other) {
    chararr = new char[strlen(other.chararr) + 1];
    strcpy(chararr, other.chararr);
    std::cout << "Copy constructor called\n";
  }

  // Destructor
  ~Cstr() { delete[] chararr; }

  int getlength(){
    return strlen(chararr);
  }

  // Output operator
  friend std::ostream &operator<<(std::ostream &os, const Cstr &obj) {
    os << obj.chararr;
    return os;
  }
};

int main() {
  Cstr string0;
  Cstr string1(10);
  Cstr string2("chim to khong lo");
  Cstr string3(string2);

string1 = "234567890";


  std::cout << string0.getlength() << " " << string1 << " " << string2 << " " << string3
            << std::endl;
}
