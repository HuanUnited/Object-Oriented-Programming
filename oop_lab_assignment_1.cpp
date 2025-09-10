#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>
using std::cin, std::cout, std::endl, std::sqrt, std::atan;

class Triangle {
private:
  int m_coord_x;
  int m_coord_y;
  double m_height;
  double m_base;
  double m_side;
  double m_base_angle;
  double m_small_angle;

public:
  // Triangle I/O:
  void inputTriangle(int x, int y, int height, int base) {
    m_coord_x = x;
    m_coord_y = y;
    m_height = height;
    m_base = base;
  }
  void inputTriangle() { // Hand input
    cout << "Input x: ";
    cin >> m_coord_x;
    cout << endl;
    cout << "Input y: ";
    cin >> m_coord_y;
    cout << endl;
    cout << "Input height: ";
    cin >> m_height;
    cout << endl;
    cout << "Input base: ";
    cin >> m_base;
    cout << endl;
  }

  void outputTriangle() { // Print params.
    cout << "Triangle Parameters are:" << "Coordinates - x: " << m_coord_x
         << "y: " << m_coord_x << " Base: " << m_base << "Height: " << m_height
         << endl;
  }

  void times(int k) {
    m_base *= k;
    m_height *= k;
    updateSide();
  }

  void updateSide() {
    m_side = sqrt((m_base / 2) * (m_base / 2) + m_height * m_height);
  }
  void updateBaseAngle() { m_base_angle = atan((m_base / 2) / m_height); }
  void updateSmallAngle() { m_small_angle = 180 - m_base_angle * 2; }

  bool isEqualto(Triangle &other) {
    if ((m_base == other.m_base) && (m_height == other.m_height))
      return true;
    else
      return false;
  }

  // Setters&Getters;
  void setX(int x) { m_coord_x = x; }
  void setY(int y) { m_coord_y = y; }
  void setHeight(int height) { m_height = height; }
  void setBase(int base) { m_base = base; }
  int getX() const { return m_coord_x; }
  int getY() const { return m_coord_y; }
  double getHeight() const { return m_height; }
  double getBase() const { return m_base; }
  double getSide() const { return m_side; }
  double getSmallAngle() const { return }

  // Getters2:
  double getPerimeter() const { return m_side * 2 + m_base; }
  double getArea() const { return m_base * m_height; }
  double getCircumradius() const { return (m_side * m_side / 2 * m_height); }
  double getCircumcenter() const { return; }
  double getSemiPerimiter() const { return (m_base + m_side * 2) / 2; }
  double getSideHeight() const { return (2 * getArea() / m_side); }

  // Verifications
  bool isEquilateral() const { return m_base == m_side ? true : false; }

  // Constructors;
  Triangle() {
    m_coord_x = 0; // x
    m_coord_y = 0;
    m_height = 1;
    m_base = 1;
    updateSide();
    updateBaseAngle();
    updateSmallAngle();
  }
  Triangle(int x, int y, double height, double base) {
    m_coord_x = x;
    m_coord_y = y;
    m_height = height;
    m_base = base;
  }
};

int main() { return 0; }