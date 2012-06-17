// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


/// \file
/// \brief Funciones para obtener el tiempo

/// Inicializa las funciones de tiempo. 
/// Guarda el tiempo actual y en el futuro lo utiliza para restarlo del actual y decir cuanto tiempo ha pasado.
void time_init(void);

void time_new_ref(t_time t);

/// Obtiene el tiempo actual.
/// En segundos flotante. Restando el tiempo almacenado del actual.
float time_now(void);

/// Da un entero dependiente del tiempo para poder usar de semilla 
int time_get_time_seed(void);
