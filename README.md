INFORME TÉCNICO: SIMULADOR DE PLANIFICACIÓN DE CPU

(SIGET)

Sistemas Operativos | Simulación de Procesos de Tráfico

1. OBJETIVOS Y DISEÑO DE LA SOLUCIÓN
El objetivo de este proyecto es implementar un simulador del planificador de procesos de CPU
enfocado en el motor de procesamiento del Sistema Integrado de Gestión de Tráfico (SIGET). La solución
busca comparar el comportamiento de tareas rutinarias frente a eventos de alta prioridad como
emergencias viales.
Para el diseño del simulador en C++ se definieron los siguientes componentes principales:
Estructura de Procesos: Cada proceso contiene atributos para identificar la tarea (ID, Nombre),
tiempo de ejecución (Tiempo Irrupción), prioridad (1 a 3), volumen de datos (MB) y tiempo de
llegada.
Gestión de Estados: Se simulan los 5 estados clásicos del ciclo de vida de un proceso: Nuevo, Listo,
Ejecución, Bloqueado y Terminado.
Simulación del Reloj: Un bucle central incrementa el tiempo por unidades discretas (ticks)
evaluando las transiciones de los procesos.
2. PROCESOS DE PRUEBA UTILIZADOS
Se configuraron tres procesos representativos de las operaciones de tráfico:
ID Proceso T. Irrupción Prioridad Datos T. Llegada
P1 Camaras_Central 8 ut 3 (Baja) 450 MB 0 ut
P2 Semaforo_Emergencia 4 ut 1 (Alta) 50 MB 1 ut
P3 Analitica_Flujo 6 ut 2 (Media) 200 MB 2 ut
3. ALGORITMOS EVALUADOS Y OBSERVACIONES
a) Round Robin (Quantum = 2 ut): Asigna a cada proceso una cuota de tiempo equitativa. Evita que
procesos grandes queden bloqueados indefinidamente, pero aumenta el tiempo de espera para eventos
críticos ya que deben esperar su turno en la cola circular.
b) Planificación por Prioridad Preventiva: Ejecuta el proceso con el nivel de alerta más prioritario
(menor valor numérico). Si llega una emergencia (P2) mientras se ejecuta una tarea de rutina (P1), la CPU
desaloja la tarea actual para atender la emergencia de inmediato.
4. RESULTADOS COMPARATIVOS
Algoritmo Tiempo de Espera Promedio Tiempo de Retorno Promedio
Round Robin (Q=2) 8.33 ut 14.33 ut
Prioridad Preventiva 3.66 ut 9.66 ut
•

•
•

5. CONCLUSIONES
El algoritmo por Prioridad Preventiva resulta óptimo para entornos como el SIGET, donde la respuesta
inmediata a semáforos de emergencia o colisiones es prioritaria frente al procesamiento masivo de datos
de rutina. Sin embargo, para evitar la inanición de tareas rutinarias en producción, se recomienda
complementar la lógica con colas multinivel.
