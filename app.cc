// app.cc
// Desarrollo de los métodos de la clase Reservador
// Author: Mateo Fortea Dugo
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <mysqlx/xdevapi.h>
#include "app.h"
using namespace std;

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
            cout << "Error: El correo no existe\n";
            return false;
        }
    }
    else
    {
        cout << "Error: El formato de correo no es válido\n";
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
        system("clear");
        cout << "Credenciales incorrectas\n";
        return false;
    }
}

void iniciarSesion(Reservador r, mysqlx::Table usuarios)
{
    while (r.getIniciado() != true)
    {
        cout << "Introduce tu correo: ";
        string correo;
        cin >> correo;
        r.setCorreo(correo);
        if (comprobarCorreo(correo, usuarios))
        {
            cout << "Contraseña: ";
            string clave;
            cin >> clave;
            if (comprobarClave(clave, usuarios, r.getCorreo()))
            {
                r.setIniciado(true);
            }
        }
    }
}

// MAIN()
int main()
{
    Reservador r = Reservador();
    string credenciales[3];
    leerFichero(credenciales);

    try
    {
        mysqlx::Session conexion(33060, credenciales[0], credenciales[1]);

        try
        {
            cout << "-> Conexión establecida [√]\n\n";
            mysqlx::Schema bd = conexion.getSchema(credenciales[2]);
            mysqlx::Table usuarios = bd.getTable("usuarios");
            cout << "--------| Iniciar sesión |--------\n";

            iniciarSesion(r, usuarios);
            // Aqui se crea el objeto usuario a partir del correo guardado
            system("clear");
            cout << "========||  R E S E R V A D O R  ||======== \n\n";
            cout << "### [Nombre] elige una opción (0-2)\n";
            exit(0);
        }
        catch (const mysqlx::Error &err)
        {
            cout << "Ha ocurrido un error con la base de datos:\n " << err
                 << endl;
            exit(1);
        }
    }
    catch (const mysqlx::Error &err)
    {
        cout << "Error de conexión con la base de datos \n"
             << endl;
        exit(1);
    }
}

