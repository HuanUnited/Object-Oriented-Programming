#include <cstring>
#include <iostream>
const int MS = 255;

class String{
    private:
    char line[MS];
    int len = strlen(line);

    public:
    void stringFill(const char*);
    int getLen(){
        return len;
    }
    void print(){
        std::cout << line;
    }
    char & index(int i);
};

int main(){

return 0;
}