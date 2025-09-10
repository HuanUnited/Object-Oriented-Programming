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
    // Triangle I/O
    void inputTriangle(int x, int y, double height, double base) {
        m_coord_x = x;
        m_coord_y = y;
        m_height = height;
        m_base = base;
    }

    void inputTriangle() { // Hand input
        cout << "Input x: "; cin >> m_coord_x;
        cout << "Input y: "; cin >> m_coord_y;
        cout << "Input height: "; cin >> m_height;
        cout << "Input base: "; cin >> m_base;
    }

    void outputTriangle() const {
        cout << "Triangle:\n";
        cout << "A: (" << m_coord_x << "," << m_coord_y << ")\n";
        cout << "Base: " << m_base << " Height: " << m_height << "\n";
        cout << "Side: " << getSide() << " Perimeter: " << getPerimeter() << "\n";
    }

    // Operators
    Triangle operator*(int k) const {
        Triangle scaled = *this;
        scaled.m_base *= k;
        scaled.m_height *= k;
        return scaled;
    }
    /*This pointer is a pointer that points to the current instance of the class.*/

    bool operator==(const Triangle &other) const {
        return ((m_base == other.m_base) && (m_height == other.m_height)) ? true : false;
    }

    // Getter&Setters
    void setX(double x) { m_coord_x = x; }
    void setY(double y) { m_coord_y = y; }
    void setHeight(double h) { m_height = h; }
    void setBase(double b) { m_base = b; }

    double getX() const { return m_coord_x; }
    double getY() const { return m_coord_y; }
    double getHeight() const { return m_height; }
    double getBase() const { return m_base; }
    double getSide() const { return sqrt((m_base / 2) * (m_base / 2) + m_height * m_height); }
    double getSmallAngle() const { return 180.0 - 2 * getBaseAngle(); }
    double getBaseAngle() const { return atan((m_base / 2) / m_height) * 180.0 / M_PI; }

    // GeoGebra
    double getPerimeter() const { return 2 * getSide() + m_base; }
    double getArea() const { return 0.5 * m_base * m_height; }
    double getSemiPerimeter() const { return (m_base + 2 * getSide()) / 2.0; }
    double getSideHeight() const { return (2 * getArea() / getSide()); }
    double getCircumradius() const {
        double S = getArea();
        return (m_base * getSide() * getSide()) / (4 * S);
    }
    void getCircumcenter() const { // Coordinates
        double R = getCircumradius();
        double cx = m_coord_x + m_base / 2.0;
        double cy = m_coord_y - (R - m_height);
        cout << "Circumcenter: (" << cx << "," << cy << ")\n";
    }

    // Checker
    bool isEquilateral() const {
        return m_base == getSide()? true : false;
    }
    /*fabs is a function that returns the absolute value*/

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

int main() {
    Triangle t1(0,0,5,6);
    Triangle t2(0,0,5,6);

    t1.outputTriangle();
    cout << "Area: " << t1.getArea() << endl;
    cout << "Circumradius: " << t1.getCircumradius() << endl;
    t1.getCircumcenter();

    // Test operator==
    if (t1 == t2) cout << "Triangles are equal!\n";

    // Test operator*
    Triangle t3 = t1 * 2;
    t3.outputTriangle();

    return 0;
}
