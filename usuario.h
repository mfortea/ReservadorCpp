// usuario.h
// Declaración de la clase usuario

#include <string>
using namespace std;

#ifndef USUARIO_H
#define USUARIO_H

class Usuario
{
private:
    int ID_;
    string nombre_;
    string apellidos_;
    string correo_;
    string pass_;
    string departamento_;
    int limite_maquinas_;
    int limite_cpus_;
    int limite_reservas_;

public:
    Usuario()
    {
    }
        Usuario(int ID, string nombre, string apellidos, string correo, string pass, string departamento, int limite_maquinas, int limite_cpus, int limite_reservas)
    {
        ID_ = ID;
        nombre_=nombre;
        apellidos_=apellidos;
        correo_=correo;
        pass_=pass;
        departamento_=departamento;
        limite_maquinas_=limite_maquinas;
        limite_cpus_=limite_cpus;
        limite_reservas_=limite_reservas;
    }

    // Getters
    inline int getID() { return ID_; }
    inline string getNombre() { return nombre_; }
    inline string getApellidos() { return apellidos_; }
    inline string getCorreo() { return correo_; }
    inline string getPass() { return pass_; }
    inline string getDepartamento() { return departamento_; }
    inline int getLimiteMaquinas() { return limite_maquinas_; }
    inline int getLimiteCpus() { return limite_cpus_; }
    inline int getLimiteReservas() { return ID_; }

    //Setters
    inline void setID(int ID) { ID_ = ID; }
    inline void setNombre(string nombre) { nombre_ = nombre; }
    inline void setApellidos(string apellidos) { apellidos_ = apellidos; }
    inline void setCorreo(string correo) { correo_ = correo; }
    inline void setPass(string pass) { pass_ = pass; }
    inline void setDepartamento(string departamento) { departamento_ = departamento; }
    inline void setLimiteMaquinas(int limite_maquinas) { limite_maquinas_ = limite_maquinas; }
    inline void setLimiteCpus(int limite_cpus) { limite_cpus_ = limite_cpus; }
    inline void setLimiteReservas(int limite_reservas) { limite_reservas_ = limite_reservas; }
};

#endif
