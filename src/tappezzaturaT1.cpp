#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point {
    double x, y;
    Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    Point operator*(double s) const { return {x * s, y * s}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y}; }
};

// Interpolazione tra due punti
Point interpolate(const Point& a, const Point& b, double t) {
    return a + (b - a) * t;
}

int main() {
    Point A, B, C;
    int b;

    std::cout << "Inserisci coordinate A (x y): ";
    std::cin >> A.x >> A.y;
    std::cout << "Inserisci coordinate B (x y): ";
    std::cin >> B.x >> B.y;
    std::cout << "Inserisci coordinate C (x y): ";
    std::cin >> C.x >> C.y;
    std::cout << "Numero di suddivisioni b: ";
    std::cin >> b;

    // Vettori base
    Point u = (B - A) * (1.0 / b); // da A verso B
    Point v = (C - A) * (1.0 / b); // da A verso C

    std::cout << "\nTriangoli equilateri interni (vertici):\n";
    int count = 1;
    for (int i = 0; i < b; ++i) {
        for (int j = 0; j < b - i; ++j) {
            // Primo triangolo in ogni cella
            Point p1 = A + u * i + v * j;
            Point p2 = p1 + u;
            Point p3 = p1 + v;
            std::cout << "T" << count++ << ": (" << p1.x << ", " << p1.y << ")  ";
            std::cout << "(" << p2.x << ", " << p2.y << ")  ";
            std::cout << "(" << p3.x << ", " << p3.y << ")\n";

            // provo a fare il secondo
            if (i + j < b - 1) {
                Point p4 = p1 + u + v;
                std::cout << "T" << count++ << ": (" << p2.x << ", " << p2.y << ")  ";
                std::cout << "(" << p4.x << ", " << p4.y << ")  ";
                std::cout << "(" << p3.x << ", " << p3.y << ")\n";
            }
        }
    }

    return 0;
}