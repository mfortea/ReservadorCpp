// app.cc
// Desarrollo de los métodos de la clase Reservador
// Author: Mateo Fortea Dugo
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
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
            cout << '\a';
            cout << RED << "⚠️  Error: El correo no existe\n"
                 << RESET;
            return false;
        }
    }
    else
    {
        cout << "\a";
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
        cout << '\a';
        cout << RED << " ⚠️  Credenciales incorrectas\n"
             << RESET;
        return false;
    }
}

bool comprobarFormatoFecha(string fecha)
{
    const regex patronFecha("^[0-3]?[0-9]/[0-3]?[0-9]/(?:[0-9]{2})?[0-9]{2}$");
    string anio = fecha.substr(6, 4);
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    stringstream ss_anio;
    ss_anio << timePtr->tm_year + 1900;
    string anio_actual = ss_anio.str();

    if (regex_match(fecha, patronFecha) && anio >= anio_actual)
    {
        return true;
    }
    else
    {
        cout << '\a';
        cout << RED << "⚠️  Error: La fecha no es válida\n"
             << RESET;
        return false;
    }
}

bool comprobarFecha(string fecha, Reservador r, mysqlx::Table reservas)
{
    bool hueco = false;
    string anio = fecha.substr(6, 4);
    string mes = fecha.substr(3, 2);
    int dia = stoi(fecha.substr(0, 2));

    mysqlx::RowResult res = reservas.select("fecha_inicio", "fecha_fin")
                                .where("maquina like :entrada")
                                .bind("entrada", r.getReserva().getMaquinaID())
                                .execute();

    int nReservas = 0;
    for (mysqlx::Row fila : res.fetchAll())
    {
        nReservas++;
        string fecha_inicio = (string)fila[0];
        string fecha_fin = (string)fila[1];

        string anio_fecha_inicio = fecha_inicio.substr(6, 4);
        string mes_fecha_inicio = fecha_inicio.substr(3, 2);
        int dia_fecha_inicio = stoi(fecha_inicio.substr(0, 2));

        string anio_fecha_fin = fecha_fin.substr(6, 4);
        string mes_fecha_fin = fecha_fin.substr(3, 2);
        int dia_fecha_fin = stoi(fecha_fin.substr(0, 2));

        if (anio_fecha_inicio == anio && mes_fecha_inicio == mes)
        {
            if (dia >= dia_fecha_inicio && dia <= dia_fecha_fin)
            {
                hueco = false;
            }
            else
            {
                hueco = true;
            }
        }
        else
        {
            hueco = true;
        }
    }

    if (nReservas == 0)
    {
        hueco = true;
    }

    if (hueco == false)
    {
        cout << RED << "\nERROR: Esa fecha ya está ocupada\n"
             << RESET;
    }

    return hueco;
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
        cout << '\a';
        cout << "NO HAY RESERVAS\n\n";
    }
    cout << MAGENTA << "0)" << RESET << " Salir\n\n-> "
         << "Elige una opción: ";
    int opcion = -1;
    cin >> opcion;
}

void seleccionarMaquina(Reservador &r, mysqlx::Table maquinas)
{
    mysqlx::RowResult res = maquinas.select("ID", "nombre_maquina", "nucleos", "ram")
                                .execute();
    vector<Maquina> vMaquinas;
    for (mysqlx::Row fila : res.fetchAll())
    {
        Maquina m = Maquina((int)fila[0], (string)fila[1], (int)fila[2], (int)fila[3]);
        vMaquinas.push_back(m);
    }

    for (int i = 0; i < vMaquinas.size(); i++)
    {
        cout << MAGENTA << " " << vMaquinas[i].getID() << ")" << RESET << " Nombre: " << vMaquinas[i].getNombreMaquina() << "\n    Núcleos: " << vMaquinas[i].getNucleos() << "\n    RAM: " << vMaquinas[i].getRam() << "\n\n";
    }

    if (vMaquinas.size() == 0)
    {
        cout << "\a";
        cout << "NO HAY MÁQUINAS\n\n";
    }
    bool encontrado = false;
    int opcionMaquina = -1;
    do
    {
        cout << "\a";
        cout << "-> Elige una máquina para hacer la reserva: ";
        cin >> opcionMaquina;

        for (int i = 0; i < vMaquinas.size(); i++)
        {
            if (opcionMaquina == vMaquinas[i].getID())
            {
                Reserva reserva = Reserva();
                reserva.setMaquinaID(opcionMaquina);
                r.setReserva(reserva);
                encontrado = true;
            }
        }

    } while (encontrado != true);
}

void seleccionarFechas(Reservador &r, mysqlx::Table reservas)
{
    system("clear");
    mysqlx::RowResult res = reservas.select("ID", "usuario", "maquina", "fecha_inicio", "fecha_fin", "num_nucleos", "motivo_reserva")
                                .where("maquina like :entrada")
                                .bind("entrada", r.getReserva().getMaquinaID())
                                .execute();
    int nReservas = 0;
    cout << CYAN << "\nReservas de la máquina\n========================\n\n"
         << RESET;
    for (mysqlx::Row fila : res.fetchAll())
    {
        nReservas++;
        cout << RED << "* " << RESET "Máquina ocupada del " << fila[3] << " al " << fila[4] << "\n";
    }
    if (nReservas == 0)
    {
        cout << GREEN << "La máquina está libre de reservas\n"
             << RESET;
    }

    bool correcta = false;
    while (correcta != true)
    {
        cout << "\n-> Introduce una fecha de inicio (dd/mm/aaaa): ";
        string fechaInicio;
        cin >> fechaInicio;
        if (comprobarFormatoFecha(fechaInicio) && comprobarFecha(fechaInicio, r, reservas))
        {
            correcta = true;
            Reserva reserva = Reserva();
            reserva.setMaquinaID(r.getReserva().getMaquinaID());
            reserva.setFechaInicio(fechaInicio);
            r.setReserva(reserva);
        }
    }

    correcta = false;
    while (correcta != true)
    {
        cout << "\n-> Introduce una fecha de fin (dd/mm/aaaa): ";
        string fechaFin;
        cin >> fechaFin;
        if (comprobarFormatoFecha(fechaFin) && comprobarFecha(fechaFin, r, reservas))
        {
            correcta = true;
            Reserva reserva = Reserva();
            reserva.setMaquinaID(r.getReserva().getMaquinaID());
            reserva.setFechaInicio(r.getReserva().getFechaInicio());
            reserva.setFechaFin(fechaFin);
            r.setReserva(reserva);
        }
    }
}

int obtenerNucleosReales(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas)
{
    int nucleosMaquina = 0;
    mysqlx::RowResult res = maquinas.select("nucleos")
                                .where("ID like :entrada")
                                .bind("entrada", r.getReserva().getMaquinaID())
                                .execute();
    mysqlx::Row fila = res.fetchOne();
    nucleosMaquina = (int)fila[0];

    mysqlx::RowResult res2 = reservas.select("num_nucleos")
                                 .where("maquina like :entrada")
                                 .bind("entrada", r.getReserva().getMaquinaID())
                                 .execute();

    int nucleosReservados = 0;
    for (mysqlx::Row fila : res2.fetchAll())
    {
        nucleosReservados += (int)fila[0];
    }

    return (nucleosMaquina - nucleosReservados);
}

void seleccionarNucleos(Reservador &r, mysqlx::Table reservas, mysqlx::Table maquinas)
{
    int nucleos_reales = obtenerNucleosReales(r, reservas, maquinas);
    if (nucleos_reales == 0)
    {
        cout << RED << " La máquina NO tiene núcleos libres" << RESET << endl;
    }

    cout << "\n La máquina tiene libre " << GREEN << nucleos_reales << " núcleos" << RESET << "\n\n";
    bool correcto = false;
    while (correcto != true)
    {
        cout << "-> Introduce la cantidad de núcleos a reservar: ";
        int nucleos;
        cin >> nucleos;
        if (nucleos > 1 && nucleos <= nucleos_reales)
        {
            Reserva reserva = Reserva();
            reserva.setMaquinaID(r.getReserva().getMaquinaID());
            reserva.setFechaInicio(r.getReserva().getFechaInicio());
            reserva.setFechaFin(r.getReserva().getFechaFin());
            reserva.setNumNucleos(nucleos);
            r.setReserva(reserva);
            correcto = true;
        }
        else
        {
            cout << "\a";
            cout << RED << "\n\n ⚠️  ERROR: Cantidad de núcleos inválida\n\n"
                 << RESET;
        }
    }
}

void seleccionarMotivo(Reservador &r)
{
    cout << "\n-> Escriba el motivo de su reserva: ";
    string motivo;
    cin >> motivo;
    Reserva reserva = Reserva();
    reserva.setMaquinaID(r.getReserva().getMaquinaID());
    reserva.setFechaInicio(r.getReserva().getFechaInicio());
    reserva.setFechaFin(r.getReserva().getFechaFin());
    reserva.setNumNucleos(r.getReserva().getNumNucleos());
    reserva.setMotivoReserva(motivo);
    r.setReserva(reserva);
}

bool subirReservaBD(Reservador r, mysqlx::Table reservas)
{

    reservas.insert("usuario", "maquina", "fecha_inicio", "fecha_fin", "num_nucleos", "motivo_reserva")
        .values(r.getUsuario().getID(), r.getReserva().getMaquinaID(), r.getReserva().getFechaInicio(), r.getReserva().getFechaFin(), r.getReserva().getNumNucleos(), r.getReserva().getMotivoReserva())
        .execute();
    return true;
}

void hacerReservas(Reservador r, mysqlx::Table reservas, mysqlx::Table maquinas)
{
    seleccionarMaquina(r, maquinas);
    seleccionarFechas(r, reservas);
    seleccionarNucleos(r, reservas, maquinas);
    seleccionarMotivo(r);
    if (subirReservaBD(r, reservas))
    {
        cout << '\a';
        cout << '\a';
        system("clear");
        cout << GREEN << "\n\n====|| RESERVA REALIZADA CON ÉXITO ✅ ||====" << RESET << "\n\n";
        cout << CYAN << "Ha reservado la máquina " << r.getReserva().getMaquinaID() << " entre el día " << r.getReserva().getFechaInicio() << " y el " << r.getReserva().getFechaFin() << RESET << "\n\n";
        cout << "Volviendo al menú ...\n\n";
        sleep(5);
    }
    else
    {
        cout << "\a";
        cout << RED << "\n\nHA OCURRIDO UN ERROR ❌ " << RESET << "\n\n";
    }
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
            system("clear");
            cout << CYAN << "========||  1 - NUEVA RESERVA  ||======== \n\n"
                 << RESET;
            hacerReservas(r, reservas, maquinas);
            break;
        }
        case 2:
        {
            system("clear");
            cout << CYAN << "========||  2 - MIS RESERVAS  ||======== \n\n"
                 << RESET;
            mostrarReservas(r, reservas, maquinas);
            break;
        }
        default:
        {
            cout << '\a';
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
            cout << "\a";
            cout << RED << "⚠️  ERROR: Ha ocurrido un error con la base de datos:\n " << err
                 << RESET << endl;
            exit(1);
        }
    }
    catch (const mysqlx::Error &err)
    {
        cout << "\a";
        cout << RED << "-> ⚠️  ERROR: No se ha podido conectar a la base de datos [X]\n\n"
             << RESET;
        exit(1);
    }
}
