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
    Triangle operator*(const int k) const;
    bool operator==(const Triangle &other) const;

    // Getter&Setters
    void setX(const double x);
    void setY(const double y);
    void setHeight(const double h);
    void setBase(const double b);

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
Triangle Triangle::operator*(const int k) const {
    Triangle scaled = *this;
    scaled.m_base *= k;
    scaled.m_height *= k;
    return scaled;
}

bool Triangle::operator==(const Triangle &other) const { // If the other triangle can be shifted so that it sits right on top of the given triangle,
    // then they are translated <если одну прямуголнику можем двигать так, чтобы она сидела прямо над другой, и есть одинаковые размерности, то является параллелный перенос.
    // same dimensions
    if (m_base != other.m_base) return false; // 1e-6 = epsilon
    if (m_height != other.m_height) return false;

    // compute this triangle’s vertices (считать вершины)
    double Tx = m_coord_x + m_base / 2.0; // top vertex
    double Ty = m_coord_y;
    double Lx = m_coord_x; // left vertex
    double Ly = m_coord_y + m_height;
    double Rx = m_coord_x + m_base; // right vertex
    double Ry = m_coord_y + m_height;

    // compute other triangle’s vertices
    double Tx2 = other.m_coord_x + other.m_base / 2.0;
    double Ty2 = other.m_coord_y;
    double Lx2 = other.m_coord_x;
    double Ly2 = other.m_coord_y + other.m_height;
    double Rx2 = other.m_coord_x + other.m_base;
    double Ry2 = other.m_coord_y + other.m_height;

    // translation vector between top vertices // векторы переноса
    double dx = Tx2 - Tx;
    double dy = Ty2 - Ty;

    // check if all vertices align under the same shift
    return (Lx + dx - Lx2 <= 1e-6) &&
            (Ly + dy - Ly2 <= 1e-6) &&
            (Rx + dx - Rx2 <= 1e-6) &&
            (Ry + dy - Ry2 <= 1e-6);
}


// Setter
void Triangle::setX(const double x) { m_coord_x = x; }
void Triangle::setY(const double y) { m_coord_y = y; }
void Triangle::setHeight(const double h) { m_height = h; }
void Triangle::setBase(const double b) { m_base = b; }

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
    Triangle t2(6,8,4,6);

    t1.outputTriangle();
    cout << "Area: " << t1.getArea() << endl;
    cout << "Circumradius: " << t1.getCircumradius() << endl;
    t1.getCircumcenter();

    // Test operator==
    if (t1 == t2) cout << "Triangles are equal!\n";
    else cout <<"Triangles are not equal! \n";

    // Test operator*
    Triangle t3 = t1 * 2;
    t3.outputTriangle();

    return 0;
}
