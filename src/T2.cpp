#include <vector>

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


struct Edge{
    Point x1,x2;

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
        (A.x+B.x)*n/nsez,
        (A.y+B.y)*n/nsez,
        (A.z+B.z)*n/nsez
    };
}


int main() {
    Point A, B, C;
    int b;
     //---------CALCOLO--LE--DIREZIONI------
    Point mab=midpoints(A,B,2,1);
    Point mac=midpoints(A,C,2,1);
    Point mbc=midpoints(B,C,2,1);

    Point dirR0;
    Point dirR1;
    Point dirR2;
    //calcolo dir retta verso destra(denominata R1)
    dirR0 = {mbc.x - A.x, mbc.y - A.y, mbc.z - A.z};
    //calcolo dir retta verticale(denominata R2)
    dirR1 = {mab.x - C.x, mab.y - C.y, mab.z - C.z};
    //calcolo dir retta verso sinistra(denominata R3)
    dirR2 = {mbc.x - B.x, mbc.y - B.y, mbc.z - B.z};




    
    
    //ora calcolo in base a b i midpoints sui lati
    vector<Point> punti;
    //Lato AB
    punti.push_back(A);
    for(int j=1; j<b*2; ++j){
        punti.push_back(midpoints(A,B,b*2,j));
    }
    punti.push_back(B);

    //creazione nuovi punti tramite le direzioni sul lato AB
    //ciclo per verticali

    //il baricentro è a h/3
    //h è sqrt(3)/2 * lato
    //quindi sommo sempre alternando 2/3h ovvero 1/sqrt(3) di l
    //il lato è la distanza tra 2 punti dello stesso lato che hanno solo 1 punto in mezzo
    double latoTriPicc=distanza(punti(0),punti(2));
    double saltovert= (1/sqrt(3))*latoTriPicc;
    
    vector<Point> newpunti;
    //righe verticali
    for(int j=1; j<vector.size()/2; j=j+2){
        //prendo il punto
        bool gira=true;
        
        while(gira){
            Point dirNorm = normalize(dirR2);
            newpunti.push_back(punti[j]+ dirNorm * salto);
            

            //controllo x uscire
            if()//come fare a sapere quando fermarsi?
                gira=false;
        }
        
    }
    //righe diagonali dx --- distanza di salto è uguale all altezza (è solo un altezza ruotata)
    for(int j=0; j<vector.size()/2; j=j+2){
        //prendo il punto
        bool gira=true;
        
        while(gira){
            Point dirNorm = normalize(dirR1);
            newpunti.push_back(punti[j]+ dirNorm * salto);
            

            //controllo x uscire
            if()//come fare a sapere quando fermarsi?
                gira=false;
        }
        
    }
    //righe diagonali sx --- distanza di salto è uguale all altezza (è solo un altezza ruotata)
    for(int j=vector.size()/2; j>0; j=j-2){
        //prendo il punto
        bool gira=true;
        
        while(gira){
            Point dirNorm = normalize(dirR3);
            newpunti.push_back(punti[j]+ dirNorm * salto);
            

            //controllo x uscire
            if()//come fare a sapere quando fermarsi?
                gira=false;
        }
        
    }


    
}