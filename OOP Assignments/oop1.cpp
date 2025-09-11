#include <iostream>
#include <ostream>
using std::string;


class AbstractEmployee {
    virtual void AskForPromotion() = 0; //abstract function by making it virtual
};


class Employee:AbstractEmployee{
protected: // These are class attributes
    string Name;
    string Company;
    int Age;

    // How can we describe behaviors in a class, we use a method
public:
    void setName(string name){
        Name = name;
    }
    string getName(){
        return Name;
    }
    void setCompany(string company){
        Company = company;
    }
    string getCompany(){
        return Company;
    }    
    
    void setAge(int age){
        if (age>=18)
        Age = age;
    }
    int getAge(){
        return Age;
    }
    void Introduceyourself(){ //This is a class method!
        std::cout<<"Name - " << Name << std::endl;
        std::cout<<"Company - " << Company << std::endl;
        std::cout<<"Age - " << Age << std::endl;

    }

    void AskForPromotion() {
        if(Age>30){
        std::cout<<Name <<" got promoted." << std::endl;}
        else  {std::cout<<Name<< " sorry no promotion." << std::endl;}
    }


    Employee(string name, string company, int age) { // This is a constructor! it's job is to construct the object of employee.
        Name = name;
        Company = company;
        Age = age;
    }

    virtual void Work(){
        std::cout << Name << " is checking emails... " << std::endl;
    }
};

class Developer: public Employee {
    public:
    string FavProgLang;

    Developer(string name, string company, int age, string fav) : Employee(name,company,age) 
    {
        FavProgLang = fav;
    }

    void fixBug(){
        std::cout << getName() << " fixed bug using " << FavProgLang << std::endl;
    }
    
    void Work(){
        std::cout << Name << " is writing code " << std::endl;
    }
};

class Teacher: public Employee{
    public:
    string Subject;
    void prepareLesson(){
        std::cout<< Name << " is preparing " << Subject << " lesson." << std::endl;
    }
    
    void Work(){
        std::cout << Name << " is teaching " << std::endl;
    }

    Teacher(string name, string company, int age, string subject) : Employee(name,company,age){
        Subject = subject;
    }
};

int main() {
    
    Developer d = Developer("Malina","YT",39,"C++");

    Teacher t = Teacher("Jack","CoolSkool",29,"History");

    Employee *e1 = &d; //The most common use of polymorphism is when a parent class reference is used to refer to a child class object.
    Employee *e2 = &t;

    e1 -> Work();
    e2 -> Work();

    // Contructor is a method without a return value

    return 0;
}

// Access Modifiers: Private, Public, Protected -> Inheritance