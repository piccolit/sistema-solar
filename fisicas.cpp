#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define PI 3.14159265
#define G 6.67430e-11 // Constante de gravitación universal en m^3 kg^-1 s^-2

struct cuerpo{

    string id;
    double masa;      //kg
    double radio;     // km
    //vectores de 3 elementos: uno por cada coo. en el espacio (x,y,z)
    vector<float> posicion = {0.0, 0.0, 0.0};      //UA
    vector<float> velocidad = {0.0, 0.0, 0.0};     //UA/medio dia terrestre
    vector<float> aceleracion = {0.0, 0.0, 0.0};   //UA/medio dia terrestre^2
};

//Se carga la base de datos de cuerpos celestes, por cada carga se agrega un cuerpo celeste mas
void cargarCuerpoCeleste(string id, unsigned int masa, unsigned int radio, 
                        float xInicial, float yInicial, float zInicial, 
                        float velocidadX, float velocidadY, float velocidadZ, 
                        vector<cuerpo> & sistema){

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

    sistema.push_back(move(nuevoCuerpo));
}

//SOPORTE------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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


void calcularAceleracion(cuerpo & cuerpo1, vector<cuerpo> & sistema){
    // Post: devuelve un vector con la aceleracion del cuerpo dado segun la atraccion gravitatoria sufrida a partir de los demas cuerpos del sistema.
    // Las fuerzas que se le pasen son para el siguiente t.
    
        if(sistema.size() < 2){
        cout << "Error, no se puede calcular aceleracion si el cuerpo no esta en un sistema de al menos dos entidades." << endl;
        return;
    }
    
    vector<float> aceleracion;
    
    for(int i=0; i<sistema.size(); i++){
        if(sonDistintos(sistema[i], cuerpo1)){
            aceleracion[0] = (calcularFuerzaGravitatoria(sistema[i], cuerpo1)[0])/cuerpo1.masa;
            aceleracion[1] = (calcularFuerzaGravitatoria(sistema[i], cuerpo1)[1])/cuerpo1.masa;
            aceleracion[2] = (calcularFuerzaGravitatoria(sistema[i], cuerpo1)[2])/cuerpo1.masa;
        }
            
    }
    cuerpo1.aceleracion = aceleracion;
}

void siguientePosicion(cuerpo cuerpo){
    //Post: Recibe un cuerpo y modifica su posicion(t) a posicion(t+1)

    //Esto tiene en cuenta que la velocidad este medida en las mismas unidades que el tiempo.
    cuerpo.posicion[0] = cuerpo.posicion[0] + cuerpo.velocidad[0];
    cuerpo.posicion[1] = cuerpo.posicion[1] + cuerpo.velocidad[1];
    cuerpo.posicion[2] = cuerpo.posicion[2] + cuerpo.velocidad[2];

}


void siguienteVelocidad(cuerpo cuerpo){
    //Post: //Post: Recibe un cuerpo y modifica su velocidad(t) a velocidad(t+1)

    //Esto tiene en cuenta que la velocidad este medida en las mismas unidades que el tiempo.
    cuerpo.velocidad[0] = cuerpo.velocidad[0] + cuerpo.aceleracion[0];
    cuerpo.velocidad[1] = cuerpo.velocidad[1] + cuerpo.aceleracion[1];
    cuerpo.velocidad[2] = cuerpo.velocidad[2] + cuerpo.aceleracion[2];

}


void nuevoEstado(vector<cuerpo> sistemaSolar){
    
    for(int i=0; i<sistemaSolar.size(); i++)                
        calcularAceleracion(sistemaSolar[i],sistemaSolar); 
    for(int i=0; i<sistemaSolar.size(); i++)                    
        siguientePosicion(sistemaSolar[i]);                     //Actualiza la posicion de todos los sistemas a su inmediata posicion
      //Ahora con las nuevas posiciones, calculamos las aceleraciones provocadas por sus nuevas atracciones
    for(int i=0; i<sistemaSolar.size(); i++)
        siguienteVelocidad(sistemaSolar[i]);                    //Actualiza su velocidad actual segun su nueva aceleracion, la velocidad sera la que determine la proxima posicion
}



int main(){

    vector<cuerpo> sistemaSolar = {};
    vector<float> aceleracion;

//1. Se cargan cuerpos en el estado inicial del sistema con posiciones y velocidades iniciales, las aceleraciones NO ESTAN INICIALIZADAS

//  cargarCuerpoCeleste(id, masa, radio, x, y, z, vx, vy, vz)

    cargarCuerpoCeleste("Sol",1.989e30,700000,
                        0,0,0,
                        0,0,0,sistemaSolar);
    cargarCuerpoCeleste("Tierra", 5.97e24, 6357,
                        -0.94,0.33,0,
                         100,200,0, sistemaSolar); //puse 100,200,300 como ejemplo


//2. Se inicializan las aceleraciones iniciales de todos los planetas dado su posicion respecto al resto

    for(int i = 0; i< sistemaSolar.size(); i++)
        calcularAceleracion(sistemaSolar[i],sistemaSolar);
    

//3. Se van actualizando los planetas del sistema solar

    while(true)
        nuevoEstado(sistemaSolar);


}

