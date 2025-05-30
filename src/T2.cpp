#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Point {
    double x, y, z;
    Point operator+(const Point& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Point operator*(double s) const { return {x * s, y * s, z * s}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y, z - other.z}; }
};

struct Vector3D {
    double x, y, z;

    Vector3D operator*(double s) const {
        return {x * s, y * s, z * s};
    }
};

Point normalize(const Point& v) {
    double norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (norm == 0) return {0, 0, 0}; // direzione nulla
    return {v.x / norm, v.y / norm, v.z / norm};
}



double distanza(const Point& a, const Point& b) {
    return sqrt(
        (b.x - a.x) * (b.x - a.x) +
        (b.y - a.y) * (b.y - a.y) +
        (b.z - a.z) * (b.z - a.z)
    );
}
Point midpoints(const Point& A, const Point& B, int nsez, int n) {
    return {
        A.x + (B.x - A.x) * static_cast<double>(n) / nsez,
        A.y + (B.y - A.y) * static_cast<double>(n) / nsez,
        A.z + (B.z - A.z) * static_cast<double>(n) / nsez
    };
}


int main() {
    Point A, B, C;
    int b;

    std::cout << "Inserisci coordinate A (x y z): ";
    std::cin >> A.x >> A.y >> A.z;
    std::cout << "Inserisci coordinate B (x y z): ";
    std::cin >> B.x >> B.y >> B.z;
    std::cout << "Inserisci coordinate C (x y z): ";
    std::cin >> C.x >> C.y >> C.z;
    std::cout << "Numero di b: ";
    std::cin >> b;

     //---------CALCOLO--LE--DIREZIONI------
    Point mab=midpoints(A,B,2,1);
    Point mac=midpoints(A,C,2,1);
    Point mbc=midpoints(B,C,2,1);

 
    Point dirR1;


    //calcolo dir retta verticale(denominata R2)
    dirR1.x=mab.x - C.x;
    dirR1.y=mab.y - C.y;
    dirR1.z=mab.z - C.z;


    
    
    //ora calcolo in base a b i midpoints sui lati
    vector<Point> puntiAB;
    //Lato AB
    puntiAB.push_back(A);
    for(int j=1; j<b*2; ++j){
        puntiAB.push_back(midpoints(A,B,b*2,j));
    }
    puntiAB.push_back(B);

    //CALCOLO I PUNTI SUI SEGMENTI ACB (SENZA AB COME LATO)
    vector<Point> puntiACB;
    //Lato AB
    puntiACB.push_back(A);
    for(int j=1; j<b*2; ++j){
        puntiACB.push_back(midpoints(A,C,b*2,j));
    }
    puntiACB.push_back(C);

    //ora aggiungo i punti su CB 

    for(int j=1; j<b*2; ++j){
        puntiACB.push_back(midpoints(C,B,b*2,j));
    }
    puntiACB.push_back(B);



    //creazione nuovi punti tramite le direzioni sul lato AB
    //ciclo per verticali

    //il baricentro è a h/3
    //h è sqrt(3)/2 * lato
    //quindi sommo sempre alternando 2/3h ovvero 1/sqrt(3) di l
    //il lato è la distanza tra 2 punti dello stesso lato che hanno solo 1 punto in mezzo
    double latoTriPicc=distanza(puntiAB[0],puntiAB[2]);
    double salto= (1/sqrt(3))*latoTriPicc;
    
    vector<Point> newpunti;
    //righe verticali
    for(int j=2; j<puntiACB.size()-1; j=j+2){
        //prendo il punto
        bool gira=true;
        int c=1;
        while(gira){

            Point dirNorm = normalize(dirR1);
            newpunti.push_back(puntiACB[j]+ dirNorm * salto * c);
            

            //controllo x uscire
            if(distanza(puntiAB[j/2], newpunti[newpunti.size()-1])<salto)//come fare a sapere quando fermarsi?
                gira=false;
            c=c+1;
        }
        
    }

    
    cout << "punti contorno AB:" << endl;
    for (Point p: puntiAB)
        std::cout << p.x << ' '<< p.y << ' '<< p.z << endl;
    cout << "punti contorno ACB:" << endl;
    for (Point p: puntiACB)
        std::cout << p.x << ' '<< p.y << ' '<< p.z << endl;
    cout << "Punti interni" << endl;
    for (Point p: newpunti)
        std::cout << p.x << ' '<< p.y << ' '<< p.z << endl;
}
