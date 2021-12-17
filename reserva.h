// reserva.h
// Declaración de la clase Reserva

#include <string>
using namespace std;

#ifndef RESERVA_H
#define RESERVA_H

class Reserva
{
private:
    int ID_;
    int usuario_id_;
    int maquina_id_;
    string fecha_inicio_;
    string fecha_fin_;
    int num_nucleos_;
    string motivo_reserva_;

public:
    Reserva()
    {
    }
    Reserva(int ID, int usuario_id, int maquina_id, string fecha_inicio, string fecha_fin, int num_nucleos, string motivo_reserva)
    {
        ID_ = ID;
        usuario_id_ = usuario_id;
        maquina_id_ = maquina_id;
        fecha_inicio_ = fecha_inicio;
        fecha_fin_ = fecha_fin;
        num_nucleos_ = num_nucleos;
        motivo_reserva_ = motivo_reserva;
    }

    // Getters
    inline int getID() { return ID_; }
    inline int getUsuarioID() { return usuario_id_; }
    inline int getMaquinaID() { return maquina_id_; }
    inline string getFechaInicio() { return fecha_inicio_; }
    inline string getFechaFin() { return fecha_fin_; }
    inline int getNumNucleos() { return num_nucleos_; }
    inline string getMotivoReserva() { return motivo_reserva_; }

    //Setters
    inline void setID(int ID) { ID_ = ID; }
    inline void setUsuarioID(int usuario_id) { usuario_id_ = usuario_id; }
    inline void setMaquinaID(int maquina_id) { maquina_id_ = maquina_id; }
    inline void setFechaInicio(string fecha_inicio) { fecha_inicio_ = fecha_inicio; }
    inline void setFechaFin(string fecha_fin) { fecha_fin_ = fecha_fin; }
    inline void setNumNucleos(int num_nucleos) { num_nucleos_ = num_nucleos; }
    inline void setMotivoReserva(string motivo_reserva) { motivo_reserva_ = motivo_reserva; }
};
#endif
