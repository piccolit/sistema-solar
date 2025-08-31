#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define PI 3.14159265
#define G 6.67430e-11 // Constante de gravitación universal en m^3 kg^-1 s^-2

struct cuerpo{

    string id;
    unsigned int masa;      //kg
    unsigned int radio;     // km
    //vectores de 3 elementos: uno por cada coo. en el espacio (x,y,z)
    vector<float> posicion;      //
    vector<float> velocidad;     //
    vector<float> aceleracion;   //
};

//Se carga la base de datos de cuerpos celestes, por cada carga se agrega un cuerpo celeste mas
void cargarCuerpoCeleste(string id, unsigned int masa, unsigned int radio, float xInicial, float yInicial, float zInicial, float velocidadX, float velocidadY, float velocidadZ, vector<cuerpo> sistema){

    cuerpo nuevoCuerpo;

    nuevoCuerpo.id = id;
    nuevoCuerpo.masa = masa;
    nuevoCuerpo.radio = radio;
    
    nuevoCuerpo.posicion[0] = xInicial;
    nuevoCuerpo.posicion[1] = yInicial;
    nuevoCuerpo.posicion[2] = zInicial;

    nuevoCuerpo.velocidad[0] = velocidadX;
    nuevoCuerpo.velocidad[1] = velocidadY;
    nuevoCuerpo.velocidad[2] = velocidadZ;

    sistema.push_back(nuevoCuerpo);
}


bool mismaPosicion(cuerpo cuerpo1, cuerpo cuerpo2){
    // Post: devuelve true si los dos cuerpos estan en la misma posicion

    if(cuerpo1.posicion[0] == cuerpo2.posicion[0] && cuerpo1.posicion[1] == cuerpo2.posicion[1] && cuerpo1.posicion[2] == cuerpo2.posicion[2])
        return true;
    else
        return false;
}


bool sonDistintos(const cuerpo & cuerpo1, const cuerpo & cuerpo2){ 
    // Post: devuelve false si dos cuerpos tienen distinta masa, radio y posicion

    bool masa = cuerpo1.masa == cuerpo2.masa;
    bool radio = cuerpo1.radio == cuerpo2.radio;

    return !(mismaPosicion(cuerpo1, cuerpo2) && masa && radio);
}



vector<float> calcularFuerzaGravitatoria(cuerpo cuerpo1, cuerpo cuerpo2){

    //Recibe dos cuerpos, devuelve un vector<float> con la fuerza gravitatoria que sufren los dos en
    //cada una de sus coordenadas X,Y,Z (cooFuerzas[0],cooFuerzas[1] y cooFuerzas[2] respectivamente)

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



vector<float> sumatoriaFuerzas(cuerpo cuerpo1, vector<cuerpo> sistema){
    // Post: devuelve un vector con la aceleracion del cuerpo dado segun los cuerpos del sistema.
    vector<float> aceleracion;
    
    for(int i=0; i<sistema.size(); i++){
        if(sonDistintos(sistema[i], cuerpo1)){
            aceleracion[0] = aceleracion[0] + calcularFuerzaGravitatoria(sistema[i], cuerpo1)[0];
            aceleracion[1] = aceleracion[1] + calcularFuerzaGravitatoria(sistema[i], cuerpo1)[1];
            aceleracion[2] = aceleracion[2] + calcularFuerzaGravitatoria(sistema[i], cuerpo1)[2];
        }
            
    }

}




vector<float> siguienteVelocidad(cuerpo cuerpo){

    cuerpo.aceleracion = sumatoriaFuerzas(cuerpo, vectorCuerpos);


    return velocidad_siguiente_t;
}

//La funcion principal. Actualiza el estado de un cuerpo por cada frame, cada frame senializa un t++.
void actualizarEstado(cuerpo cuerpo){



}

int main(){

    vector<cuerpo> sistema = {};

    cargarCuerpoCeleste();

    return 0;
}
