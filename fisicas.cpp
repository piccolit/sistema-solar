#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define PI 3.14159265
#define G 6.67430e-11 // Constante de gravitación universal en m^3 kg^-1 s^-2

struct cuerpo{

    unsigned int masa;      //kg
    unsigned int radio;     // km
    //vectores de 3 elementos: uno por cada coo. en el espacio (x,y,z)
    vector<float> posicion;      //
    vector<float> velocidad;     //
    vector<float> aceleracion;   //
};


bool sonDistintos(const cuerpo & cuerpo1, const cuerpo & cuerpo2){ 
    // Post: devuelve false si dos cuerpos tienen distinta masa, radio y posicion

    bool masa = cuerpo1.masa == cuerpo2.masa;
    bool radio = cuerpo1.radio == cuerpo2.radio;
    
    bool posicionX = cuerpo1.posicion[0] == cuerpo2.posicion[0];
    bool posicionY = cuerpo1.posicion[1] == cuerpo2.posicion[1];
    bool posicionZ = cuerpo1.posicion[2] == cuerpo2.posicion[2];

    //true <-> es igual en todos las coordenadas
    bool posicion = posicionX && posicionY && posicionZ;

    return !(posicion || masa || radio);
}


//Recibe dos cuerpos, devuelve un vector<float> con la fuerza gravitatoria que sufren los dos en
// cada una de sus coordenadas X,Y,Z (cooFuerzas[0],cooFuerzas[1] y cooFuerzas[2] respectivamente)
vector<float> calcularFuerzaGravitatoria(cuerpo cuerpo1, cuerpo cuerpo2){

    vector<float> cooFuerzas(3);
    float fuerzaTotal, distanciaTotal, dx, dy, dz, fuerzaX, fuerzaY, fuerzaZ;

    dx = cuerpo1.posicion[0] - cuerpo2.posicion[0];
    dy = cuerpo1.posicion[1] - cuerpo2.posicion[1];
    dz = cuerpo1.posicion[2] - cuerpo2.posicion[2];
    distanciaTotal = sqrt(dx*dx + dy*dy + dz*dz);
    
    fuerzaTotal = -G * cuerpo1.masa * cuerpo2.masa / pow(distanciaTotal,3);
    
    cooFuerzas[0] = fuerzaTotal * dx;
    cooFuerzas[1] = fuerzaTotal * dy;
    cooFuerzas[2] = fuerzaTotal * dz;

    return cooFuerzas;
}



vector<float> sumatoriaFuerzas(vector<cuerpo> sistema, cuerpo cuerpo){
    // Post: devuelve un vector con la aceleracion del cuerpo dado segun los cuerpos del sistema.
    vector<float> aceleracion;
    
    for(int i=0; i<sistema.size(); i++){
        if(sonDistintos(sistema[i], cuerpo)){
            aceleracion[0] = aceleracion[0] + calcularFuerzaGravitatoria(sistema[i], cuerpo)[0];
            aceleracion[1] = aceleracion[1] + calcularFuerzaGravitatoria(sistema[i], cuerpo)[1];
            aceleracion[2] = aceleracion[2] + calcularFuerzaGravitatoria(sistema[i], cuerpo)[2];
        }
            
    }

}

