#include <cmath>
#include <iostream>
using std::cin, std::cout, std::endl, std::sqrt, std::atan;

class Triangle {
private:
    int m_coord_x;
    int m_coord_y;
    double m_height;
    double m_base;
    double m_side;
    double m_base_angle;   // angle at base
    double m_small_angle;  // top angle

public:
    // Triangle I/O
    void inputTriangle(int x, int y, double height, double base) {
        m_coord_x = x;
        m_coord_y = y;
        m_height = height;
        m_base = base;
        updateSide();
        updateAngles();
    }

    void inputTriangle() { // Hand input
        cout << "Input x: "; cin >> m_coord_x;
        cout << "Input y: "; cin >> m_coord_y;
        cout << "Input height: "; cin >> m_height;
        cout << "Input base: "; cin >> m_base;
        updateSide();
        updateAngles();
    }

    void outputTriangle() const {
        cout << "Triangle:\n";
        cout << "A: (" << m_coord_x << "," << m_coord_y << ")\n";
        cout << "Base: " << m_base << " Height: " << m_height << "\n";
        cout << "Side: " << m_side << " Perimeter: " << getPerimeter() << "\n";
    }

    // Update
    void updateSide() {
        m_side = sqrt((m_base / 2) * (m_base / 2) + m_height * m_height);
    }

    void updateAngles() {
        // angles in degrees
        m_base_angle = atan((m_base / 2) / m_height) * 180.0 / M_PI;
        m_small_angle = 180.0 - 2 * m_base_angle;
    }

    // Operators
    Triangle operator*(int k) const {
        Triangle scaled = *this;
        scaled.m_base *= k;
        scaled.m_height *= k;
        scaled.updateSide();
        scaled.updateAngles();
        return scaled;
    }
    /*This pointer is a pointer that points to the current instance of the class.*/

    bool operator==(const Triangle &other) const {
        return (fabs(m_base - other.m_base) < 1e-6 &&
                fabs(m_height - other.m_height) < 1e-6 &&
                m_coord_x == other.m_coord_x &&
                m_coord_y == other.m_coord_y);
    }

    // Getter&Setters
    void setX(int x) { m_coord_x = x; }
    void setY(int y) { m_coord_y = y; }
    void setHeight(double h) { m_height = h; updateSide(); updateAngles(); }
    void setBase(double b) { m_base = b; updateSide(); updateAngles(); }

    int getX() const { return m_coord_x; }
    int getY() const { return m_coord_y; }
    double getHeight() const { return m_height; }
    double getBase() const { return m_base; }
    double getSide() const { return m_side; }
    double getSmallAngle() const { return m_small_angle; }
    double getBaseAngle() const { return m_base_angle; }

    // GeoGebra
    double getPerimeter() const { return 2 * m_side + m_base; }
    double getArea() const { return 0.5 * m_base * m_height; }
    double getSemiPerimeter() const { return (m_base + 2 * m_side) / 2.0; }
    double getSideHeight() const { return (2 * getArea() / m_side); }
    double getCircumradius() const {
        double S = getArea();
        return (m_base * m_side * m_side) / (4 * S);
    }
    void getCircumcenter() const { // Coordinates
        double R = getCircumradius();
        double cx = m_coord_x + m_base / 2.0;
        double cy = m_coord_y - (R - m_height);
        cout << "Circumcenter: (" << cx << "," << cy << ")\n";
    }

    // Checker
    bool isEquilateral() const {
        return m_base == m_side? true : false;
    }
    /*fabs is a function that returns the absolute value*/

    // Constructors
    Triangle() {
        m_coord_x = 0;
        m_coord_y = 0;
        m_height = 1;
        m_base = 1;
        updateSide();
        updateAngles();
    }

    Triangle(int x, int y, double height, double base) {
        m_coord_x = x;
        m_coord_y = y;
        m_height = height;
        m_base = base;
        updateSide();
        updateAngles();
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
