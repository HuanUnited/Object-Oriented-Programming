#include <iostream>
#include <string>
int main() {

  std::string str;

  std::cin >> str;

  int a{0}, b{0};

  for (int i = 0; i < str.length(); i += 2) {
    // std::cout << "Debug :" << str[i] << std::endl;

    if (str[i] == 'A')
      a += str[i + 1] - '0';
    else
      b += str[i + 1] - '0';
  }

  // std::cout << "Score A :" << a << std::endl;
  // std::cout << "Score B :" << b << std::endl;

  if (a > b)
    std::cout << "A" << std::endl;
  else
    std::cout << "B" << std::endl;
}