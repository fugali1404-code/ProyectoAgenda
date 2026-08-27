#ifndef NOTIFICACION_HPP
#define NOTIFICACION_HPP

#include <string>

// ============================================================
// TIPO DE NOTIFICACIÓN
// ============================================================

enum class TipoNotificacion
{
    RECORDATORIO,
    NUEVA_TAREA,
    CAMBIO_FECHA,
    MENSAJE_PROFESOR,
    SISTEMA
};

// ============================================================
// TIPO DE REFERENCIA
// ============================================================

enum class TipoReferenciaNotificacion
{
    NINGUNA,
    TAREA,
    MATERIA,
    PROFESOR
};

// ============================================================
// NOTIFICACIÓN
// ============================================================

class Notificacion
{
private:

    // ID único de la notificación
    int id;

    // Usuario que recibe la notificación
    int idUsuario;

    // Tipo de notificación
    TipoNotificacion tipo;

    // ID del elemento relacionado
    //
    // Ejemplo:
    // NUEVA_TAREA -> id de la tarea
    // CAMBIO_FECHA -> id de la tarea
    // MENSAJE_PROFESOR -> id del profesor
    //
    // Si no existe referencia:
    // idReferencia = 0
    int idReferencia;

    // Tipo de elemento al que hace referencia
    TipoReferenciaNotificacion tipoReferencia;

    // Título
    std::string titulo;

    // Mensaje
    std::string mensaje;

    // Fecha de creación
    std::string fecha;

    // Estado de lectura
    bool leida;

public:

    // ========================================================
    // CONSTRUCTOR POR DEFECTO
    // ========================================================

    Notificacion();

    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    Notificacion(
        int id,
        int idUsuario,
        TipoNotificacion tipo,
        int idReferencia,
        TipoReferenciaNotificacion tipoReferencia,
        const std::string& titulo,
        const std::string& mensaje,
        const std::string& fecha
    );

    // ========================================================
    // GETTERS
    // ========================================================

    int getId() const;

    int getUsuarioId() const;

    TipoNotificacion getTipo() const;

    int getIdReferencia() const;

    TipoReferenciaNotificacion getTipoReferencia() const;

    std::string getTitulo() const;

    std::string getMensaje() const;

    std::string getFecha() const;

    bool estaLeida() const;

    // ========================================================
    // ACCIONES
    // ========================================================

    void marcarComoLeida();

    void marcarComoNoLeida();
};

#endif

