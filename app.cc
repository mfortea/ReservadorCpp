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
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"
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
            cout <<RED<< "Error: El correo no existe\n"<<RESET;
            return false;
        }
    }
    else
    {
        cout <<RED<< "Error: El formato de correo no es válido\n"<<RESET;
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
        cout <<RED<< "Credenciales incorrectas\n"<<RESET;
        return false;
    }
}

void iniciarSesion(Reservador r, mysqlx::Table usuarios)
{
    while (r.getIniciado() != true)
    {
        cout <<BOLDWHITE<< "Introduce tu correo: "<<RESET;
        string correo;
        cin >> correo;
        r.setCorreo(correo);
        if (comprobarCorreo(correo, usuarios))
        {
            cout <<BOLDWHITE<< "Contraseña: "<<RESET;
            string clave;
            cin >> clave;
            if (comprobarClave(clave, usuarios, r.getCorreo()))
            {
                r.setIniciado(true);
            }
        }
    }
}

void mostrarMenu(){
    int opcion = -1;
    system("clear");
    do {
        cout <<CYAN<< "========||  R E S E R V A D O R  ||======== \n\n"<<RESET;
    	cout<<MAGENTA<<" 1)"<<RESET<<" Hacer reserva\n"<<MAGENTA<<" 2)"<<RESET<<" Mis Reservas\n"<<MAGENTA<<" 0)"<<RESET<<" Salir\n\n-> Usuario, elige una opción (0-2): ";
        cin>>opcion;
        if (opcion < 0 | opcion > 2 ) {
        	cout<<RED<<"\n\nERROR: Introduce una opción entre 0 y 2\n\n"<<RESET;
        }
    } while (opcion != 0);

}

// MAIN()
int main()
{
	system("clear");
    Reservador r = Reservador();
    string credenciales[3];
    leerFichero(credenciales);

    try
    {
        mysqlx::Session conexion(33060, credenciales[0], credenciales[1]);

        try
        {
            cout <<GREEN<< "-> Conexión establecida [√]\n\n"<<RESET;
            mysqlx::Schema bd = conexion.getSchema(credenciales[2]);
            mysqlx::Table usuarios = bd.getTable("usuarios");
            cout <<CYAN<< "--------| Iniciar sesión |--------\n"<<RESET;

            iniciarSesion(r, usuarios);
            mostrarMenu();
            // Aqui se crea el objeto usuario a partir del correo guardado

            exit(0);
        }
        catch (const mysqlx::Error &err)
        {
            cout <<RED<<"Ha ocurrido un error con la base de datos:\n " << err
                 <<RESET<<endl;
            exit(1);
        }
    }
    catch (const mysqlx::Error &err)
    {
        cout <<RED<< "Error de conexión con la base de datos \n"
             <<RESET<<endl;
        exit(1);
    }
}
