#include <bits/stdc++.h> 
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ostream>
#include <sstream>
#include <stdint.h>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

#define T float //se define el tipo de dato de acuerdo a la preferencia

class PuntoGenerico { 
    public:
         T x, y; 
    
        bool operator<(PuntoGenerico p) 
        { 
            return x < p.x || (x == p.x && y < p.y); 
        } 
}; 
  
/*
se usa un algoritmo ya existente de monotone chain para determinara si una lista de puntos es una envolvente convexa.
bajo ese concepto, si un conjunto de puntos dado tiene una envolvente convexa igual a el conjunto de puntos (es decir,
ningun punto queda en el interior del poligono convexo formado). De esta forma se asegura que no haya angulos interiores
mayores a 180°, ni que lineas se crucen etc.
*/
//producto vectorial de dos vectores OA y OB, retorna positivo o negativo de acuerdo a la direccion(reloj o antirreloj)
T productoVectorial(PuntoGenerico O, PuntoGenerico A, PuntoGenerico B){ 
    return (A.x - O.x) * (B.y - O.y) 
           - (A.y - O.y) * (B.x - O.x); 
} 

//retorna la lista de puntos de la envolvente convexa en sentido antirreloj
vector<PuntoGenerico> envolventeConvexa(vector<PuntoGenerico> A){ 
    int n = A.size(), k = 0; 
    if (n <= 3) 
        return A; 
  
    vector<PuntoGenerico> ans(2 * n); 
    sort(A.begin(), A.end()); //se ordenan los puntos
    
    //se hace la parte de abajo de la envolvente
    for (int i = 0; i < n; ++i) { 
        while (k >= 2 && productoVectorial(ans[k - 2], ans[k - 1], A[i]) <= 0) 
            k--; 
        ans[k++] = A[i]; 
    }  
    //se hace la parte de arriba de la envolvente
    for (size_t i = n - 1, t = k + 1; i > 0; --i) {  
        while (k >= t && productoVectorial(ans[k - 2], ans[k - 1], A[i - 1]) <= 0) 
            k--; 
        ans[k++] = A[i - 1]; 
    }   
    // se cambia el tamaño del vector a la cantidad que tiene realmente
    ans.resize(k - 1);   
    return ans; 
}

//se crea la clase que contiene la excepcion de puntos invalidos para un poligono
class PoligonoInvalido{
    virtual const char* what() const throw()
  {
    return "Los puntos no forman un poligono convexo";
  }
}p_invalido;

//funcion que calcula la distancia entre 2 puntos dados, se usa de auxiliar para getPerimetro
float distanciaEntreDosPuntos(PuntoGenerico A, PuntoGenerico B){
    float d;
    d=sqrt(pow((B.x-A.x),2)+pow((B.y-A.y),2));//aplicacion directa de la formula de distancia entre 2 puntos en el plano de coordenadas
    
    return d;
}

//funcion que ve si la recta que pasa por un punto P corta un determinado lado del poligono, dado por los puntos A y B
bool ray_intersects_segment(PuntoGenerico P, PuntoGenerico A, PuntoGenerico B){
    //agregar el caso especial de que el punto este en un lado del poligono
    PuntoGenerico aux; //como por convencion el punto A esta mas abajo que B, si esta arriba debemos hacer un intercambio para facilitar el algoritmo
    if(A.y>B.y){
        aux=A;
        A=B;
        B=A;
    }
    float m1,m2; //variables para guardar las pendientes que calculamos
    if (P.y<A.y || P.y>B.y){
        return false;
    }else if (P.x >= max(A.x, B.x)){
        return false;
    }else{
        if (P.x < min(A.x, B.x)){
            return true;
        }else{
            if (A.x != B.x){
                m1=(B.y - A.y)/(B.x - A.x); //se calcula la pendiente
            }else{
                m1=6516516; //un numero arbitrariamente grande
            }
            if (A.x != P.x){
                m2=(P.y - A.y)/(P.x - A.x);//se calcula la pendiente nuevamente
            }else{
               m2=6516516; //un numero arbitrariamente grande
            }
            if (m2 >= m1){
                return true;
            }else{
                return false;
            }
        }
    }
}

//se crea la clase poligono con sus respectivos constructores
class Poligono{
    public:
        vector<PuntoGenerico> puntos;
        //constructor que recibe un vector como parametro, intenta construir el poligono y si no es posible da una excepcion
        Poligono(vector<PuntoGenerico> points){
            try{
                vector<PuntoGenerico> ans = envolventeConvexa(points);
                if(ans.size()==points.size()){//basta con verificar que tengan la misma cantidad de elementos
                    puntos=ans;
                }else{
                    throw p_invalido;
                }
            }
            catch(exception& e){
                cout << e.what() << '\n';
            }
        };
        //constructor que recibe como parametro un string,intenta construir el poligono y si no es posible da una excepcion
        Poligono(string cadenaPuntos){
            vector<PuntoGenerico> points;
            string aux_x, aux_y;
            T x,y;
            int j;
            try{
                for(int i=0;i<cadenaPuntos.length();i++){ //se recorre la cadena para poder separarla
                    if(cadenaPuntos[i]=='('){
                        for(j=i+1;cadenaPuntos[j]!=';';j++){
                            aux_x.push_back(cadenaPuntos[j]);
                        }
                        
                        for(j=j+1;cadenaPuntos[j]!=')';j++){
                            aux_y.push_back(cadenaPuntos[j]);
                        }
                        stringstream convx(aux_x),convy(aux_y); //se convierten los numeros de string al tipo de dato que contenga T
                        convx>>x;
                        convy>>y;
                        points.push_back({x,y});
                        i=j;
                        aux_x.clear();//se vacian ambos auxiliares para poder cargar los nuevos puntos
                        aux_y.clear();
                    }
                }
                vector<PuntoGenerico> ans = envolventeConvexa(points); //se verifica con la envolvente convexa
                if(ans.size()==points.size()){
                    puntos=ans;
                }else{
                    throw p_invalido;
                }
            }
            catch(exception& e){
                cout << e.what() << '\n';
            }
        };

        template<typename L>
        void graficar(vector<L> v1, vector<L> v2, vector<L> v3, vector<L> v4){
            
            vector<int> x,y,x2,y2,x3,y3,x4,y4;
            v1.push_back(v1[0]); //en v1 se encuentran los puntos del poligono, por lo que es necesario agregar el primer punto al final para graficar
            if(!v4.empty()){
                v4.push_back(v4[0]);
            }

            int i,j,k,l; 
            //se separan las x e y en vectores diferentes para poder utilizar las funciones de plt
            for(i=0;i<v1.size();i++){
                x.push_back(v1[i].x);
                y.push_back(v1[i].y);
            }
            for(j=0;j<v2.size();j++){
                x2.push_back(v2[j].x);
                y2.push_back(v2[j].y);
            }
            for(k=0;k<v3.size();k++){
                x3.push_back(v3[k].x);
                y3.push_back(v3[k].y);
            }
            for(l=0;l<v4.size();l++){
                x4.push_back(v4[l].x);
                y4.push_back(v4[l].y);
            }

            plt::plot(x,y,"bo-"); //plot es el que hace las lineas del poligono
            plt::plot(x4,y4, "ro-");

            plt::scatter(x2,y2, 8.0); //scatter solo marca los puntos
            plt::scatter(x3,y3, 7.0);
    
            plt::show();
        }   

        //metodo que agrega un punto nuevo ingresado por el usuario y se verifica que el nuevo conjunto de puntos sea tambien un poligono valido
        string addPunto(PuntoGenerico punto_ag){
            vector<PuntoGenerico> puntos_con_ag;
            puntos_con_ag=puntos;
            puntos_con_ag.push_back(punto_ag);
            vector<PuntoGenerico> resp;
            resp=envolventeConvexa(puntos_con_ag);
            if(puntos_con_ag.size()==resp.size()){
                puntos=envolventeConvexa(puntos_con_ag);
                return "El poligono con el punto agregado es valido."; //retorna distintas cadenas de acuerdo a si pudo formar un poligono o no
            }else{
                return "El punto agregado no es compatible con el poligono anterior.";
            }
        }

        //metodo que elimina un punto si es que este pertenece al poligono ya existente, y caso contrario dice que no es parte de el
        //retorna una cadena de acuerdo a la accion que realizo
        string deletePunto(PuntoGenerico punto_borrar){
            for (int i = 0; i < puntos.size(); i++){
                if((puntos[i].x==punto_borrar.x) && (puntos[i].y==punto_borrar.y)){
                    puntos.erase(puntos.begin()+i);
                    return "Se elimino el punto."; 
                }
            }
            return "El punto no es parte del poligono, por lo tanto no se pudo eliminar.";
        }

        bool sonPuntosInternos(vector<PuntoGenerico> puntos_ie){
            //para calcular los puntos internos se usa un algoritmo llamado "ray casting"
            //se agrega el primer punto al final para hacer el calculo de todos los lados
            bool punto_afuera=false;
            puntos.push_back(puntos[0]);
            vector<PuntoGenerico> puntos_adentro;
            vector<PuntoGenerico> puntos_afuera;
            vector<PuntoGenerico> vacio;
            for (int i = 0; i < puntos_ie.size()-1; i++){
                int contador=0;
                for(int j=1;j<puntos.size();j++){
                    if(ray_intersects_segment(puntos_ie[i], puntos[j-1], puntos[j])){
                        contador++;
                    }
                }
                if((contador%2)==0){
                    puntos_afuera.push_back(puntos_ie[i]);
                    punto_afuera=true;
                }else{
                    puntos_adentro.push_back(puntos_ie[i]);
                }
            }
            cout<<"Puntos afuera"<<endl;
            for(int k=0; k<puntos_afuera.size();k++){
                cout << "(" << puntos_afuera[k].x << ", " << puntos_afuera[k].y << ")" << endl; 
            }
            cout<<"Puntos adentro"<<endl;
            for(int k=0; k<puntos_adentro.size();k++){
                cout << "(" << puntos_adentro[k].x << ", " << puntos_adentro[k].y << ")" << endl; 
            }
            cout<<"intentando graficar"<<endl;
            graficar(puntos,puntos_adentro,puntos_afuera,vacio);
            cout<<"yasta graficar"<<endl;
            return punto_afuera;
        }

        //funcion que obtiene el perimetro de el poligono
        //se vale de una funcion definida anteriormente que calcula la distancia entre dos puntos
        string getPerimetro(){
            float perimetro=0;
            ostringstream var;
            for (int i = 1; i <= puntos.size(); i++){
                if(i==puntos.size()){
                    perimetro=perimetro+distanciaEntreDosPuntos(puntos[i-1],puntos[0]);
                }else{
                    perimetro=perimetro+distanciaEntreDosPuntos(puntos[i],puntos[i-1]);
                }
            }
            
            var<<"El perimetro del poligono es "<<perimetro<<" unidades";
            return var.str(); 
        }

        //funcion que calcula el area del poligono
        string getArea(){
            //calculamos el determinante para luego dividirlo por 2 y obtener el area del poligono
            T P=0;
            T Q=0;
            float area;
            ostringstream var;
            puntos.push_back(puntos[0]); //se agrega el primer punto al final para poder calcular el determinante
            for (int i = 0; i < puntos.size()-1; i++){
                P=P+puntos[i].x*puntos[i+1].y;
                Q=Q+puntos[i].y*puntos[i+1].x;
            }
            area=P-Q; //este es el doble del area, por lo que se debe dividir por dos
            area=area/2;
            puntos.pop_back(); //se elimina nuevamente el punto que acabamos de agregar al final
            var<<"El área del poligono es "<<area<<" unidades cuadradas";
            return var.str();
        }

        //para calcular el centroide basta con sacar un promedio de los puntos x e y, cada uno por su lado, y obtendremos las coordenadas
        //que estamos buscando
        PuntoGenerico getCentroide(){
            PuntoGenerico centroide;
            float promedio_x=0;
            float promedio_y=0;
            for(int i=0;i<puntos.size();i++){
                promedio_x=promedio_x+puntos[i].x;
                promedio_y=promedio_y+puntos[i].y;
            }
            centroide.x=promedio_x/puntos.size();
            centroide.y=promedio_y/puntos.size();

            return centroide;
        }
        //funcion que verifica si un poligono P esta contenido en este poligono
        string estaContenido(Poligono P){
            vector<PuntoGenerico> vacio;
            cout<<"intentando"<<endl;
            //se utiliza el metodo puntos internos para verificar que todos los puntos de P estan dentro del poligono
            if(sonPuntosInternos(P.puntos)){ //este metodo retorna true si tiene por lo menos un punto que esta fuera
                graficar(puntos,vacio,vacio,P.puntos);
                return"El poligono P no esta contenido, tiene por lo menos un punto afuera";
            }else{
                graficar(puntos,vacio,vacio,P.puntos);
                return"El poligono P esta contenido.";
            }
        }

};

int main() 
{ 
    vector<PuntoGenerico> points;
    vector<PuntoGenerico> points1; 

    //PRUEBA 1
    //puntos a utilizar para construir el poligono
    points.push_back({ 4, -2 }); 
    points.push_back({ -3, 2 }); 
    points.push_back({ 2, 3 }); 
    
    Poligono poli(points);
    
    //puntos a utilizar para probar si son internos o externos
    points1.push_back({ 2, 1 }); 
    points1.push_back({ 3, 0 });
    points1.push_back({ -3, 4 }); 
    points1.push_back({ 5, 7 });
    points1.push_back({ 1, 1 });
    points1.push_back({ 4, 2 }); 

    //PRUEBA 2, se debe cambiar la T a float
    /*string construc="(3.2;-2.5)_(0.5;7.3)_(6.6;9.8)_(-4.2;3.7)";
    Poligono poli(construc);

    points1.push_back({ 0.5, 1.3});
    points1.push_back({ 3.7, 7.3});
    points1.push_back({ 2.8, -5.5});
    points1.push_back({ 5.98, -3.3});*/

    vector<PuntoGenerico> vacio;
    poli.graficar(poli.puntos, vacio, vacio, vacio);
    cout<<poli.addPunto({ 0, 0})<<endl;
    for (int i = 0; i < poli.puntos.size(); i++) 
        cout << "(" << poli.puntos[i].x << ", " << poli.puntos[i].y << ")" << endl; 
    poli.graficar(poli.puntos, vacio, vacio, vacio);

    cout<<poli.deletePunto({ -3, 2})<<endl;
    for (int i = 0; i < poli.puntos.size(); i++) 
        cout << "(" << poli.puntos[i].x << ", " << poli.puntos[i].y << ")" << endl; 
    poli.graficar(poli.puntos, vacio, vacio, vacio);

    cout<<poli.getPerimetro()<<endl;
    cout<<poli.getArea()<<endl;
    cout<<"El centroide es "<<poli.getCentroide().x<<","<<poli.getCentroide().y<<endl;
    poli.sonPuntosInternos(points1);
    
    cout<<"kpaso"<<endl;
    Poligono P(points1); 
    cout<<poli.estaContenido(P)<<endl;
    
    return 0; 
}