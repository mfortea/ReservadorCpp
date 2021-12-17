// maquina.h
// Declaración de la clase maquina

#include <string>
using namespace std;

#ifndef MAQUINA_H
#define MAQUINA_H

class Maquina
{
private:
    int ID_;
    string nombreMaquina_;
    int nucleos_;
    int ram_;

public:
    Maquina(int ID, string nombreMaquina, int nucleos, int ram)
    {
        ID_ = ID;
        nombreMaquina_ = nombreMaquina;
        nucleos_ = nucleos;
        ram_ = ram;
    }

    // Getters
    inline int getID() { return ID_; }
    inline string getNombreMaquina() { return nombreMaquina_; }
    inline int getNucleos() { return nucleos_; }
    inline int getRam() { return ram_; }

    //Setters
    inline void setID(int ID) { ID_ = ID; }
    inline void setNombreMaquina(string nombreMaquina) { nombreMaquina_ = nombreMaquina; }
    inline void setNucleos(int nucleos) { nucleos_ = nucleos; }
    inline void setRam(int ram) { ram_ = ram; }
};

#endif
