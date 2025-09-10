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
    void inputTriangle(int x, int y, double height, double base);
    void inputTriangle(); // Hand input
    void outputTriangle() const;

    // Operators
    Triangle operator*(int k) const;
    bool operator==(const Triangle &other) const;

    // Getter&Setters
    void setX(double x);
    void setY(double y);
    void setHeight(double h);
    void setBase(double b);

    double getX() const;
    double getY() const;
    double getHeight() const;
    double getBase() const;
    double getSide() const;
    double getSmallAngle() const;
    double getBaseAngle() const;

    // GeoGebra
    double getPerimeter() const;
    double getArea() const;
    double getSemiPerimeter() const;
    double getSideHeight() const;
    double getCircumradius() const;
    void getCircumcenter() const;

    // Checker
    bool isEquilateral() const;

    // Constructors
    Triangle();
    Triangle(int x, int y, double height, double base);
};

// ---------------- Definitions ----------------

// I/O
void Triangle::inputTriangle(int x, int y, double height, double base) {
    m_coord_x = x;
    m_coord_y = y;
    m_height = height;
    m_base = base;
}

void Triangle::inputTriangle() {
    cout << "Input x: "; cin >> m_coord_x;
    cout << "Input y: "; cin >> m_coord_y;
    cout << "Input height: "; cin >> m_height;
    cout << "Input base: "; cin >> m_base;
}

void Triangle::outputTriangle() const {
    cout << "Triangle:\n";
    cout << "A: (" << m_coord_x << "," << m_coord_y << ")\n";
    cout << "Base: " << m_base << " Height: " << m_height << "\n";
    cout << "Side: " << getSide() << " Perimeter: " << getPerimeter() << "\n";
}

// Operators
Triangle Triangle::operator*(int k) const {
    Triangle scaled = *this;
    scaled.m_base *= k;
    scaled.m_height *= k;
    return scaled;
}

bool Triangle::operator==(const Triangle &other) const {
    return ((m_base == other.m_base) && (m_height == other.m_height));
}

// Setter
void Triangle::setX(double x) { m_coord_x = x; }
void Triangle::setY(double y) { m_coord_y = y; }
void Triangle::setHeight(double h) { m_height = h; }
void Triangle::setBase(double b) { m_base = b; }

// Getter
double Triangle::getX() const { return m_coord_x; }
double Triangle::getY() const { return m_coord_y; }
double Triangle::getHeight() const { return m_height; }
double Triangle::getBase() const { return m_base; }
double Triangle::getSide() const { return sqrt((m_base / 2) * (m_base / 2) + m_height * m_height); }
double Triangle::getSmallAngle() const { return 180.0 - 2 * getBaseAngle(); }
double Triangle::getBaseAngle() const { return atan((m_base / 2) / m_height) * 180.0 / M_PI; }

// GeoGebra
double Triangle::getPerimeter() const { return 2 * getSide() + m_base; }
double Triangle::getArea() const { return 0.5 * m_base * m_height; }
double Triangle::getSemiPerimeter() const { return (m_base + 2 * getSide()) / 2.0; }
double Triangle::getSideHeight() const { return (2 * getArea() / getSide()); }

double Triangle::getCircumradius() const {
    double S = getArea();
    return (m_base * getSide() * getSide()) / (4 * S);
}

void Triangle::getCircumcenter() const {
    double R = getCircumradius();
    double cx = m_coord_x + m_base / 2.0;
    double cy = m_coord_y - (R - m_height);
    cout << "Circumcenter: (" << cx << "," << cy << ")\n";
}

// Checker
bool Triangle::isEquilateral() const {
    return m_base == getSide();
}

// Constructors
Triangle::Triangle() {
    m_coord_x = 0;
    m_coord_y = 0;
    m_height = 1;
    m_base = 1;
}

Triangle::Triangle(int x, int y, double height, double base) {
    m_coord_x = x;
    m_coord_y = y;
    m_height = height;
    m_base = base;
}

// ---------------- Main ----------------
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
