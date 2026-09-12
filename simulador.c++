#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

enum Estado { NUEVO, LISTO, EJECUCION, BLOQUEADO, TERMINADO };

string estadoStr(Estado e) {
    string nombres[] = {"NUEVO", "LISTO", "EJECUCION", "BLOQUEADO", "TERMINADO"};
    return nombres[e];
}

struct Proceso {
    int id;
    string nombre;
    int tIrrupcion, tRestante, prioridad, datos, tLlegada;
    Estado estado;
    int tEspera, tRetorno;

    Proceso(int _id, string _n, int _irr, int _prio, int _dat, int _lleg)
        : id(_id), nombre(_n), tIrrupcion(_irr), tRestante(_irr), 
          prioridad(_prio), datos(_dat), tLlegada(_lleg), estado(NUEVO), tEspera(0), tRetorno(0) {}
};

void mostrarProcesos(int t, const vector<Proceso>& ps) {
    cout << "\n[Tiempo " << t << " ut]\n";
    for (size_t i = 0; i < ps.size(); i++) {
        cout << "P" << ps[i].id << " (" << ps[i].nombre << ") | Estado: " << estadoStr(ps[i].estado)
             << " | Restante: " << ps[i].tRestante << " | Prio: " << ps[i].prioridad << "\n";
    }
}

void simularRR(vector<Proceso> ps, int quantum) {
    cout << "\n--- SIMULACION ROUND ROBIN (Q=" << quantum << ") ---\n";
    int t = 0, completados = 0, n = ps.size();
    queue<int> cola;
    vector<bool> enCola(n, false);

    while (completados < n) {
        for (int i = 0; i < n; i++) {
            if (ps[i].estado == NUEVO && ps[i].tLlegada <= t) {
                ps[i].estado = LISTO;
                cola.push(i);
                enCola[i] = true;
            }
        }

        if (cola.empty()) { t++; continue; }

        int idx = cola.front();
        cola.pop();
        enCola[idx] = false;

        ps[idx].estado = EJECUCION;
        mostrarProcesos(t, ps);

        int ejec = min(quantum, ps[idx].tRestante);
        ps[idx].tRestante -= ejec;
        t += ejec;

        for (int i = 0; i < n; i++) {
            if (ps[i].estado == NUEVO && ps[i].tLlegada <= t && !enCola[i]) {
                ps[i].estado = LISTO;
                cola.push(i);
                enCola[i] = true;
            }
        }

        if (ps[idx].tRestante == 0) {
            ps[idx].estado = TERMINADO;
            ps[idx].tRetorno = t - ps[idx].tLlegada;
            ps[idx].tEspera = ps[idx].tRetorno - ps[idx].tIrrupcion;
            completados++;
        } else {
            ps[idx].estado = LISTO;
            cola.push(idx);
            enCola[idx] = true;
        }
    }
    mostrarProcesos(t, ps);
}

void simularPrioridad(vector<Proceso> ps) {
    cout << "\n--- SIMULACION POR PRIORIDAD ---\n";
    int t = 0, completados = 0, n = ps.size();

    while (completados < n) {
        for (int i = 0; i < n; i++) {
            if (ps[i].estado == NUEVO && ps[i].tLlegada <= t) ps[i].estado = LISTO;
        }

        int idx = -1, mejorPrio = 999;
        for (int i = 0; i < n; i++) {
            if (ps[i].estado == LISTO || ps[i].estado == EJECUCION) {
                if (ps[i].prioridad < mejorPrio) {
                    mejorPrio = ps[i].prioridad;
                    idx = i;
                }
            }
        }

        if (idx == -1) { t++; continue; }

        for (int i = 0; i < n; i++) {
            if (i == idx) ps[i].estado = EJECUCION;
            else if (ps[i].estado == EJECUCION) ps[i].estado = LISTO;
        }

        ps[idx].tRestante--;
        t++;

        if (ps[idx].tRestante == 0) {
            ps[idx].estado = TERMINADO;
            ps[idx].tRetorno = t - ps[idx].tLlegada;
            ps[idx].tEspera = ps[idx].tRetorno - ps[idx].tIrrupcion;
            completados++;
        }
    }
    mostrarProcesos(t, ps);
}

int main() {
    vector<Proceso> procesos;
    procesos.push_back(Proceso(1, "Camaras_Central", 8, 3, 450, 0));
    procesos.push_back(Proceso(2, "Semaforo_Emergencia", 4, 1, 50, 1));
    procesos.push_back(Proceso(3, "Analitica_Flujo", 6, 2, 200, 2));

    simularRR(procesos, 2);
    simularPrioridad(procesos);

    return 0;
}