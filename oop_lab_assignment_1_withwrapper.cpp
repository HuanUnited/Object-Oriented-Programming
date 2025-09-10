#include <cmath>
#include <iostream>
using std::cin, std::cout, std::endl, std::sqrt, std::atan;

class Triangle {
private:
  double m_coord_x;
  double m_coord_y;
  double m_height;
  double m_base;

public:
  // Getter&Setters
  void setX(double x) { m_coord_x = x; }
  void setY(double y) { m_coord_y = y; }
  void setHeight(double h) { m_height = h; }
  void setBase(double b) { m_base = b; }

  double getX() const { return m_coord_x; }
  double getY() const { return m_coord_y; }
  double getHeight() const { return m_height; }
  double getBase() const { return m_base; }

  // Constructors
  Triangle() {
    m_coord_x = 0;
    m_coord_y = 0;
    m_height = 1;
    m_base = 1;
  }

  Triangle(int x, int y, double height, double base) {
    m_coord_x = x;
    m_coord_y = y;
    m_height = height;
    m_base = base;
  }
};

class TriangleWrapper {
private:
  Triangle *triangle;

public:
  // Triangle I/O

  void makeTriangle(){
    triangle = new Triangle;
    inputTriangle();
  }

    void makeTriangle(int x, int y, double height, double base){
    triangle = new Triangle;
    inputTriangle(x,y,height,base);
  }

  void inputTriangle(int x, int y, double height, double base) {
    triangle->setX(x);
    triangle->setY(y);
    triangle->setHeight(height);
    triangle->setBase(base);
  }

  void inputTriangle() { // Hand input
    int x,y,height,base;
    cout << "Input x: ";
    cin >> x;
    cout << "Input y: ";
    cin >> y;
    cout << "Input height: ";
    cin >> height;
    cout << "Input base: ";
    cin >> base;
    inputTriangle(x,y,height,base);
  }

  void outputTriangle() const {
    cout << "Triangle:\n";
    cout << "A: (" << triangle->getX() << "," << triangle->getY() << ")\n";
    cout << "Base: " << triangle->getBase() << " Height: " << triangle->getHeight() << "\n";
    cout << "Side: " << getSide() << " Perimeter: " << getPerimeter() << "\n";
  }

  // Operators
  TriangleWrapper operator*(int k) const {
    TriangleWrapper scaled = *this;
    scaled.triangle->setBase(triangle->getBase()*k);
    scaled.triangle->setHeight(triangle->getHeight()*k);
    return scaled;
  }
  /*This pointer is a pointer that points to the current instance of the
   * class.*/

  bool operator==(const TriangleWrapper &other) const {
    return ((triangle->getBase() == other.triangle->getBase()) && (triangle->getHeight() == other.triangle->getBase())) ? true
                                                                      : false;
  }

  double getSide() const {
    return sqrt((triangle->getBase() / 2) * (triangle->getBase() / 2) + triangle->getHeight() * triangle->getHeight());
  }
  double getSmallAngle() const { return 180.0 - 2 * getBaseAngle(); }
  double getBaseAngle() const {
    return atan((triangle->getBase() / 2) / triangle->getHeight()) * 180.0 / M_PI;
  }

  // GeoGebra
  double getPerimeter() const { return 2 * getSide() + triangle->getBase(); }
  double getArea() const { return 0.5 * triangle->getBase() * triangle->getHeight(); }
  double getSemiPerimeter() const { return (triangle->getBase() + 2 * getSide()) / 2.0; }
  double getSideHeight() const { return (2 * getArea() / getSide()); }
  double getCircumradius() const {
    double S = getArea();
    return (triangle->getBase() * getSide() * getSide()) / (4 * S);
  }
  void getCircumcenter() const { // Coordinates
    double R = getCircumradius();
    double cx = triangle->getX() + triangle->getBase() / 2.0;
    double cy = triangle->getY() - (R - triangle->getHeight());
    cout << "Circumcenter: (" << cx << "," << cy << ")\n";
  }

  // Checker
  bool isEquilateral() const { return triangle->getBase() == getSide() ? true : false; }
  /*fabs is a function that returns the absolute value*/
};

int main() {
  TriangleWrapper t1;
  t1.makeTriangle(1,2,1,2);
  TriangleWrapper t2;
  t2.makeTriangle(0,0,1,2);

  t1.outputTriangle();
  cout << "Area: " << t1.getArea() << endl;
  cout << "Circumradius: " << t1.getCircumradius() << endl;
  t1.getCircumcenter();

  // Test operator==
  if (t1 == t2)
    cout << "Triangles are equal!\n";

  // Test operator*
  TriangleWrapper t3;
  t3 = t1 * 2;
  t3.outputTriangle();

  return 0;
}
