// app.cc
// Desarrollo de los métodos de la clase Reservador
// Author: Mateo Fortea Dugo
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <mysqlx/xdevapi.h>
#include "app.h"
#include "usuario.h"
#include "reserva.h"
#include "maquina.h"

using namespace std;
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define BOLDWHITE "\033[1m\033[37m"
#define MAGENTA "\033[35m"

void leerFichero(string credenciales[])
{
    string nombreArchivo = "bd.txt";
    ifstream fichero(nombreArchivo.c_str());
    string linea;
    int i = 0;
    while (getline(fichero, linea))
    {
        credenciales[i] = linea;
        i++;
    }
}

bool comprobarCorreo(string correo, mysqlx::Table usuarios)
{
    const regex patronEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (regex_match(correo, patronEmail))
    {
        mysqlx::RowResult res = usuarios.select("correo")
                                    .where("correo like :entrada")
                                    .bind("entrada", correo)
                                    .execute();
        if (res.count() == 1)
        {
            return true;
        }
        else
        {
            cout << RED << "⚠️  Error: El correo no existe\n"
                 << RESET;
            return false;
        }
    }
    else
    {
        cout << RED << "⚠️  Error: El formato de correo no es válido\n"
             << RESET;
        return false;
    }
}

bool comprobarClave(string clave, mysqlx::Table usuarios, string correo)
{
    mysqlx::RowResult res = usuarios.select("pass")
                                .where("correo like :entrada")
                                .bind("entrada", correo)
                                .execute();

    mysqlx::Row fila = res.fetchOne();

    if ((string)fila[0] == clave)
    {
        return true;
    }
    else
    {
        cout << RED << " ⚠️  Credenciales incorrectas\n"
             << RESET;
        return false;
    }
}

void iniciarSesion(Reservador &r, mysqlx::Table usuarios)
{
    while (r.getIniciado() != true)
    {
        cout << BOLDWHITE << "Introduce tu correo: " << RESET;
        string correo;
        cin >> correo;
        r.setCorreo(correo);
        if (comprobarCorreo(correo, usuarios))
        {
            cout << BOLDWHITE << "Contraseña: " << RESET;
            string clave;
            cin >> clave;
            if (comprobarClave(clave, usuarios, r.getCorreo()))
            {
                r.setIniciado(true);
            }
        }
    }
}

void cargarUsuario(Reservador &r, mysqlx::Table usuarios)
{
    mysqlx::RowResult res = usuarios.select("ID", "nombre", "apellidos", "correo", "pass", "departamento", "limite_maquinas", "limite_cpus", "limite_reservas")
                                .where("correo like :entrada")
                                .bind("entrada", r.getCorreo())
                                .execute();
    mysqlx::Row fila = res.fetchOne();

    Usuario u = Usuario((int)fila[0], (string)fila[1], (string)fila[2], (string)fila[3], (string)fila[4], (string)fila[5], (int)fila[6], (int)fila[7], (int)fila[8]);
    r.setUsuario(u);
}

Maquina obtenerMaquina(int id, mysqlx::Table maquinas)
{
    mysqlx::RowResult res = maquinas.select("ID", "nombre_maquina", "nucleos", "ram")
                                .where("ID like :entrada")
                                .bind("entrada", id)
                                .execute();
    mysqlx::Row fila = res.fetchOne();
    Maquina m = Maquina((int)fila[0], (string)fila[1], (int)fila[2], (int)fila[3]);
    return m;
}

void mostrarReservas(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas)
{
    int id = r.getUsuario().getID();
    mysqlx::RowResult res = reservas.select("ID", "usuario", "maquina", "fecha_inicio", "fecha_fin", "num_nucleos", "motivo_reserva")
                                .where("usuario like :entrada")
                                .bind("entrada", id)
                                .execute();
    int nReservas = 0;
    for (mysqlx::Row fila : res.fetchAll())
    {
        nReservas++;
        Maquina m = obtenerMaquina((int)fila[2], maquinas);
        cout << MAGENTA << fila[0] << ")" << RESET << GREEN << " Máquina: " << RESET << m.getNombreMaquina() << GREEN << "\n   Fecha: " << RESET << " Del " << fila[3] << " hasta " << fila[4] << GREEN << "\n   Núcleos: " << RESET << fila[5] << GREEN << "\n   Motivo: " << RESET << fila[6] << "\n\n";
    }
    if (nReservas == 0)
    {
        cout << "NO HAY RESERVAS\n\n";
    }
    cout << MAGENTA << "0)" << RESET << " Salir\n\n-> "
         << "Elige una opción: ";
    int opcion = -1;
    cin >> opcion;
}

void mostrarMenu(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas)
{
    int opcion = -1;
    do
    {
        system("clear");
        cout << CYAN << "========||  R E S E R V A D O R  ||======== \n\n"
             << RESET;
        cout << MAGENTA << " 1)" << RESET << " Hacer reserva\n"
             << MAGENTA << " 2)" << RESET << " Mis Reservas\n"
             << MAGENTA << " 0)" << RESET << " Salir\n\n-> " << r.getUsuario().getNombre() << ", elige una opción (0-2): ";
        cin >> opcion;
        switch (opcion)
        {
        case 0:
        {
            break;
        }
        case 1:
        {
            int opcion1 = -1;
            system("clear");
            cout << CYAN << "========||  1 - NUEVA RESERVA  ||======== \n\n"
                 << RESET;
            cin >> opcion1;
            break;
        }
        case 2:
        {
            int opcion2 = -1;
            system("clear");
            cout << CYAN << "========||  2 - MIS RESERVAS  ||======== \n\n"
                 << RESET;
            mostrarReservas(r, reservas, maquinas);
            cin >> opcion2;
            break;
        }
        default:
        {
            cout << RED << "\n\n ⚠️  ERROR: Introduce una opción entre 0 y 2\n\n"
                 << RESET;
        }
        }
    } while (opcion != 0);
}

// MAIN()
int main()
{
    system("clear");
    Reservador r = Reservador();
    string credenciales[4];
    leerFichero(credenciales);

    try
    {
        mysqlx::Session conexion(mysqlx::SessionOption::HOST, credenciales[0],
                                 mysqlx::SessionOption::PORT, 33060,
                                 mysqlx::SessionOption::USER, credenciales[1],
                                 mysqlx::SessionOption::PWD, credenciales[2]);

        try
        {
            cout << GREEN << "-> Conexión establecida [√]\n\n"
                 << RESET;
            mysqlx::Schema bd = conexion.getSchema(credenciales[3]);
            mysqlx::Table usuarios = bd.getTable("usuarios");
            mysqlx::Table reservas = bd.getTable("reservas");
            mysqlx::Table maquinas = bd.getTable("maquinas");
            cout << CYAN << "--------| Iniciar sesión |--------\n"
                 << RESET;

            iniciarSesion(r, usuarios);
            cargarUsuario(r, usuarios);
            mostrarMenu(r, reservas, maquinas);
            exit(0);
        }
        catch (const mysqlx::Error &err)
        {
            cout << RED << "⚠️  ERROR: Ha ocurrido un error con la base de datos:\n " << err
                 << RESET << endl;
            exit(1);
        }
    }
    catch (const mysqlx::Error &err)
    {
        cout << RED << "-> ⚠️  ERROR: No se ha podido conectar a la base de datos [X]\n\n"
             << RESET;
        exit(1);
    }
}
