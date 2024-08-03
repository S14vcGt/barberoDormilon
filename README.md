# El problema del barbero dormilón (Dijkstra 1971).💈💇‍♂️

Una barbería se compone de una sala de espera con **n sillas** y la sala de barbería donde se encuentra la silla del barbero. Si **no hay clientes** a quienes atender, el barbero se pone a dormir. Si un cliente **entra y todas las sillas están ocupadas**, el cliente sale de la barbería. Si el barbero está **ocupado, pero hay sillas disponibles**, el cliente se sienta en una. Si **el barbero está dormido**, el cliente lo despierta. Escriba un programa para coordinar al barbero y sus clientes utilizando **semáforos**.

## Compilacion

En Ubuntu se debe descargar el compilador gcc y el paquete build-essential para tener disponible todas las librerias y poder compilar el programa de la siguiente forma:

`gcc barberoDormilon.c -o barberoDormilon -pthread`

# Uso

`./barberoDormilon <numero de clientes> <numero de sillas>`
