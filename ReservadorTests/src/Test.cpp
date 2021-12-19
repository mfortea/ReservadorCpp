#include "../cute/cute.h"
#include "../cute/ide_listener.h"
#include "../cute/xml_listener.h"
#include "../cute/cute_runner.h"
#include "../../reserva.h"
#include "../../maquina.h"
#include "../../usuario.h"
#include <iostream>
#include <string>
#include <regex>
#include <fstream>

using namespace std;
// Funciones a testar (usadas en Reservador)
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
        return false;
    }
}
bool comprobarCorreo(string correo)
{
    const regex patronEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (regex_match(correo, patronEmail))
    {
    	return true;
    }
    else
    {
        return false;
    }
}

// TESTS
void constructorReservaTest() {
	Reserva r = Reserva(1,1,1,"01/01/2021","01/02/2022",5,"Ejemplo");
	ASSERT_EQUAL(r.getID(), 1);
	ASSERT_EQUAL(r.getUsuarioID(), 1);
	ASSERT_EQUAL(r.getMaquinaID(), 1);
	ASSERT_EQUAL(r.getFechaInicio(), "01/01/2021");
	ASSERT_EQUAL(r.getFechaFin(), "01/02/2022");
	ASSERT_EQUAL(r.getNumNucleos(), 5);
	ASSERT_EQUAL(r.getMotivoReserva(), "Ejemplo");
}

void constructorMaquinaTest() {
	Maquina m = Maquina(1,"Ordenador 1", 10, 16);
	ASSERT_EQUAL(m.getID(), 1);
	ASSERT_EQUAL(m.getNombreMaquina(), "Ordenador 1");
	ASSERT_EQUAL(m.getNucleos(), 10);
	ASSERT_EQUAL(m.getRam(), 16);
}

void constructorUsuarioTest() {
	Usuario u = Usuario(1,"Nombre", "Apellidos", "correo@email.com","1234","Departamento",5,6,8);
	ASSERT_EQUAL(u.getID(),1);
	ASSERT_EQUAL(u.getNombre(),"Nombre");
	ASSERT_EQUAL(u.getApellidos(),"Apellidos");
	ASSERT_EQUAL(u.getCorreo(),"correo@email.com");
	ASSERT_EQUAL(u.getPass(),"1234");
	ASSERT_EQUAL(u.getDepartamento(),"Departamento");
	ASSERT_EQUAL(u.getLimiteMaquinas(),5);
	ASSERT_EQUAL(u.getLimiteCpus(),6);
}

void comprobarFormatoFechaTest() {
	string fecha1 = "01/02/2022";
	string fecha2 = "20/20/2020";
	string fecha3 = "qwerty";
	string fecha4 = "01/01/1999";
	ASSERT_EQUAL(comprobarFormatoFecha(fecha1),true);
	ASSERT_EQUAL(comprobarFormatoFecha(fecha2),false);
	ASSERT_EQUAL(comprobarFormatoFecha(fecha3),false);
	ASSERT_EQUAL(comprobarFormatoFecha(fecha4),false);
}

void comprobarCorreoTest() {
	string correo1 = "usuario";
	string correo2 = "usuario@";
	string correo3 = "usuario@email";
	string correo4 = "usuario@email.com";
	ASSERT_EQUAL(comprobarCorreo(correo1),false);
	ASSERT_EQUAL(comprobarCorreo(correo2),false);
	ASSERT_EQUAL(comprobarCorreo(correo3),false);
	ASSERT_EQUAL(comprobarCorreo(correo4),true);
}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	s.push_back(CUTE(constructorReservaTest));
	s.push_back(CUTE(constructorMaquinaTest));
	s.push_back(CUTE(constructorUsuarioTest));
	s.push_back(CUTE(comprobarFormatoFechaTest));
	s.push_back(CUTE(comprobarCorreoTest));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
