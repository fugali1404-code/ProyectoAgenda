#include "PlannerSemana.hpp"

// ============================================================
// CONSTRUCTOR
// ============================================================

PlannerSemana::PlannerSemana()
    : fechaInicio("")
{
}

// ============================================================
// FECHA DE INICIO
// ============================================================

void PlannerSemana::setFechaInicio(
    const std::string& fecha
)
{
    fechaInicio = fecha;
}

const std::string&
PlannerSemana::getFechaInicio() const
{
    return fechaInicio;
}

// ============================================================
// OBTENER DIA
// ============================================================

PlannerDia&
PlannerSemana::getDia(
    int indice
)
{
    return dias.at(indice);
}

// ============================================================
// OBTENER DIA CONST
// ============================================================

const PlannerDia&
PlannerSemana::getDia(
    int indice
) const
{
    return dias.at(indice);
}