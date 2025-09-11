
#include <iostream>
class Point3d{
    private:
    int m_x{},m_y{},m_z{};

    public:
    void setValues(int a, int b, int c){
        m_x = a;
        m_y = b;
        m_z = c;
    }
    void print() const {
        std::cout<<"<" << m_x <<" "<< m_y << " "<< m_z << ">";
    }

    bool isEqual(Point3d &a){
        return (m_x == a.m_x) && (m_y == a.m_y) && (m_z == a.m_z);
    }
};


int main()
{
	Point3d point1{};
	point1.setValues(1, 2, 3);

	Point3d point2{};
	point2.setValues(1, 2, 3);

	std::cout << "point 1 and point 2 are" << (point1.isEqual(point2) ? "" : " not") << " equal\n";

	Point3d point3{};
	point3.setValues(3, 4, 5);

	std::cout << "point 1 and point 3 are" << (point1.isEqual(point3) ? "" : " not") << " equal\n";

	return 0;
}