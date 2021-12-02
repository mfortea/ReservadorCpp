// app.h
// Declaración de la clase Reservador
// Author: Mateo Fortea Dugo

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <mysqlx/xdevapi.h>

using namespace std;

#ifndef RESERVADOR_H
#define RESERVADOR_H

class Reservador
{
private:
    bool iniciado_;
    string correo_;
    //Usuario usuario_;
    // Funciones privadas
    void leerFichero(string credenciales[]);
    bool comprobarCorreo(string correo, mysqlx::Table usuarios);
    bool comprobarClave(string clave, mysqlx::Table usuarios);
    void iniciarSesion(Reservador r, mysqlx::Table tabla);
    void mostrarMenu();

public:
    Reservador()
    {
        iniciado_ = false;
        correo_ = "";
    }
    inline bool getIniciado() { return iniciado_; }
    inline void setIniciado(bool iniciado) { iniciado_ = iniciado; }
    inline string getCorreo() { return correo_; }
    inline void setCorreo(string correo) { correo_ = correo; }
};

#endif
