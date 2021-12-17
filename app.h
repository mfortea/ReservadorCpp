// app.h
// Declaración de la clase Reservador

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <mysqlx/xdevapi.h>
#include "usuario.h"
#include "maquina.h"
#include "reserva.h"

using namespace std;

#ifndef RESERVADOR_H
#define RESERVADOR_H

class Reservador
{
private:
    bool iniciado_;
    string correo_;
    Usuario usuario_;
    Reserva reserva_;
    // Funciones privadas
    void
    leerFichero(string credenciales[]);
    bool comprobarCorreo(string correo, mysqlx::Table usuarios);
    bool comprobarClave(string clave, mysqlx::Table usuarios);
    void iniciarSesion(Reservador &r, mysqlx::Table tabla);
    void mostrarMenu(Reservador r, mysqlx::Table reservas);
    void cargarUsuario(Reservador &r, mysqlx::Table usuarios);
    Maquina obtenerMaquina(int id, mysqlx::Table maquinas);
    void mostrarReservas(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas);
    void seleccionarMaquina(Reservador r, mysqlx::Table maquinas);
    void seleccionarFechas(Reservador r);
    void seleccionarNucleos(Reservador r);
    void seleccionarMotivo(Reservador r);
    bool subirReservaBD(Reservador r, mysqlx::Table reservas);

public:
    Reservador()
    {
        iniciado_ = false;
        correo_ = "";
        usuario_ = Usuario();
        reserva_ = Reserva();
    }
    inline bool getIniciado() { return iniciado_; }
    inline void setIniciado(bool iniciado) { iniciado_ = iniciado; }
    inline string getCorreo() { return correo_; }
    inline void setCorreo(string correo) { correo_ = correo; }
    inline Usuario getUsuario() { return usuario_; }
    inline void setUsuario(Usuario usuario) { usuario_ = usuario; }
    inline Reserva getReserva() { return reserva_; }
    inline void setReserva(Reserva reserva) { reserva_ = reserva; }
};

#endif
