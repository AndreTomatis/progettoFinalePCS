#include <iostream>
#include <vector>
#include <iomanip>

struct Point {
    double x, y, z;
    Point operator+(const Point& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Point operator*(double s) const { return {x * s, y * s, z * s}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y, z - other.z}; }
};

int main() {
    Point A, B, C;
    int b;

    std::cout << "Inserisci coordinate A (x y z): ";
    std::cin >> A.x >> A.y >> A.z;
    std::cout << "Inserisci coordinate B (x y z): ";
    std::cin >> B.x >> B.y >> B.z;
    std::cout << "Inserisci coordinate C (x y z): ";
    std::cin >> C.x >> C.y >> C.z;
    std::cout << "Numero di suddivisioni b: ";
    std::cin >> b;

    Point u = (B - A) * (1.0 / b); // Vettore da A verso B
    Point v = (C - A) * (1.0 / b); // Vettore da A verso C

    std::cout << "\nTriangoli:\n";
    int count = 1;
    for (int i = 0; i < b; ++i) {
        for (int j = 0; j < b - i; ++j) {
            Point p1 = A + u * i + v * j;
            Point p2 = p1 + u;
            Point p3 = p1 + v;
            std::cout << "T" << count++ << ": (" << p1.x << ", " << p1.y << ", " << p1.z << ")  ";
            std::cout << "(" << p2.x << ", " << p2.y << ", " << p2.z << ")  ";
            std::cout << "(" << p3.x << ", " << p3.y << ", " << p3.z << ")\n";

            if (i + j < b - 1) {
                Point p4 = p1 + u + v;
                std::cout << "T" << count++ << ": (" << p2.x << ", " << p2.y << ", " << p2.z << ")  ";
                std::cout << "(" << p4.x << ", " << p4.y << ", " << p4.z << ")  ";
                std::cout << "(" << p3.x << ", " << p3.y << ", " << p3.z << ")\n";
            }
        }
    }

    return 0;
}
