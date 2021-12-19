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
    // Variable booleana que indica si se ha iniciado sesión
    bool iniciado_;
    // Variable que contiene el correo actual del usuario
    string correo_;
    // Variable que contiene un objero Usuario con el usuario actual que ha iniciado sesión
    Usuario usuario_;
    // Variable que contiene un objeto Reserva con la reserva que se va a realizar
    Reserva reserva_;
    void
    // Función que lee el fichero con las credenciales de la base de datos
    leerFichero(string credenciales[]);
    // Función que comprueba el formato del correo y si existe en la base de datos
    bool comprobarCorreo(string correo, mysqlx::Table usuarios);
    // Función que comprueba la clave con la base de datos
    bool comprobarClave(string clave, mysqlx::Table usuarios);
    // Función encargada de iniciar sesión
    void iniciarSesion(Reservador &r, mysqlx::Table tabla);
    // Función que muestra el menú de opciones del sistema
    void mostrarMenu(Reservador r, mysqlx::Table reservas);
    // Función que copia la información del usuario actual a la variable usuario_
    void cargarUsuario(Reservador &r, mysqlx::Table usuarios);
    // Función que devuelve un objeto Máquina al buscarlo por su ID
    Maquina obtenerMaquina(int id, mysqlx::Table maquinas);
    // Función que muestra en pantalla las reservas del actual usuario
    void mostrarReservas(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas);
    // Función que permite seleccionar máquina para la reserva
    void seleccionarMaquina(Reservador r, mysqlx::Table maquinas);
    // Función que permite seleccionar fechas para la reserva
    void seleccionarFechas(Reservador r, mysqlx::Table reservas);
    // Función que comprueba que el formato de la fecha sea correcta
    bool comprobarFormatoFecha(string fecha);
    // Función que devuelve si hay hueco para la fecha seleccionada
    bool comprobarFecha(string fecha, Reservador r, mysqlx::Table reservas);
    // Función que devuelve los núcleos reales disponibles de la máquina
    int obtenerNucleosReales(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas);
    // Función que permite seleccionar los núcleos para la reserva
    void seleccionarNucleos(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas);
    // Función que permite seleccionar el motivo de la reserva
    void seleccionarMotivo(Reservador r);
    // Función que crea una nueva reserva en la base de datos
    bool subirReservaBD(Reservador r, mysqlx::Table reservas);

public:
    // Constructor
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
