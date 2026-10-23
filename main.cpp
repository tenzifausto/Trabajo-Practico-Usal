#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define str 50
#define max 999

// Estructura
typedef struct {
    char modelo[str];
    char categoria;
    int anio;
    float precio;
    bool esCero;
} Vehiculo;

void esperarCeroParaVolver() {
    int volver;
    do {
        printf("\n  [0] Volver al menu > ");
        if (scanf("%d", &volver) != 1) {
            volver = -1;
        }
        while (getchar() != '\n');
    } while (volver != 0);
}

void imprimirEncabezadoTabla() {
    printf("  +----+----------------------+-----+------+-------------+-----+\n");
    printf("  | #  | MODELO               | CAT | ANIO | PRECIO      | 0KM |\n");
    printf("  +----+----------------------+-----+------+-------------+-----+\n");
}

void imprimirFilaVehiculo(Vehiculo v, int indice) {
    printf("  | %-2d | %-20.20s |  %c  | %-4d | $%-10.2f | %-3s |\n",
           indice,
           v.modelo,
           v.categoria,
           v.anio,
           v.precio,
           v.esCero ? "Si" : "No");
}

void imprimirPieTabla() {
    printf("  +----+----------------------+-----+------+-------------+-----+\n");
}

// Carga de datos
int cargarVehiculos(Vehiculo lista[], int cantidad) {
    char continuar;
    FILE *archivo = fopen("autos.txt", "a");

    if (archivo == NULL) {
        printf("\n  [!] Error al abrir el archivo.\n");
        return cantidad;
    }

    do {
        if (cantidad >= max) {
            printf("\n  [!] Se alcanzo el limite maximo de vehiculos.\n");
            break;
        }

        printf("\n  +----------------------------------------+\n");
        printf("  | NUEVO VEHICULO #%-22d |\n", cantidad + 1);
        printf("  +----------------------------------------+\n");

        printf("  > Modelo: ");
        scanf(" %[^\n]", lista[cantidad].modelo);
        while (getchar() != '\n');

        printf("  > Categoria [S]edan [D]eportivo [C]amioneta: ");
        scanf(" %c", &lista[cantidad].categoria);
        while (getchar() != '\n');
        lista[cantidad].categoria = toupper(lista[cantidad].categoria);

        printf("  > Anio: ");
        scanf("%d", &lista[cantidad].anio);
        while (getchar() != '\n');

        printf("  > Precio: $");
        scanf("%f", &lista[cantidad].precio);
        while (getchar() != '\n');

        int auxBool;
        printf("  > Es 0km? [1] Si / [0] No: ");
        scanf("%d", &auxBool);
        while (getchar() != '\n');

        lista[cantidad].esCero = (auxBool == 1);

        fprintf(archivo, "%s|%c|%d|%.2f|%d\n",
                lista[cantidad].modelo,
                lista[cantidad].categoria,
                lista[cantidad].anio,
                lista[cantidad].precio,
                lista[cantidad].esCero ? 1 : 0);

        cantidad++;

        printf("\n  [OK] Guardado. Cargar otro? (s/n) > ");
        scanf(" %c", &continuar);
        while (getchar() != '\n');
    } while (continuar == 's' || continuar == 'S');
    
    fclose(archivo);
    return cantidad;
}

// Mostrar todos los datos
void mostrarVehiculos(Vehiculo lista[], int cantidad) {
    printf("\n  :: LISTADO DE VEHICULOS ::\n\n");

    if (cantidad == 0) {
        printf("  [!] No hay vehiculos cargados en memoria.\n");
        return;
    }

    imprimirEncabezadoTabla();
    for (int i = 0; i < cantidad; i++) {
        imprimirFilaVehiculo(lista[i], i + 1);
    }
    imprimirPieTabla();
}

// Mostrar solo 0km
void mostrar0km(Vehiculo lista[], int cantidad) {
    int aux = 0;
    printf("\n  :: VEHICULOS 0KM ::\n\n");

    for (int i = 0; i < cantidad; i++) {
        if (lista[i].esCero) {
            if (aux == 0) imprimirEncabezadoTabla();
            imprimirFilaVehiculo(lista[i], i + 1);
            aux++;
        }
    }

    if (aux == 0) {
        printf("  [!] No hay vehiculos 0km registrados.\n");
    } else {
        imprimirPieTabla();
    }
}

// Mostrar por categoria
void mostrarPorCategoria(Vehiculo lista[], int cantidad) {
    char categoria;
    int aux = 0;

    printf("\n  :: FILTRAR POR CATEGORIA ::\n");
    printf("  [S] Sedan  |  [D] Deportivo  |  [C] Camioneta\n");
    printf("  > Seleccione: ");
    scanf(" %c", &categoria);
    while (getchar() != '\n');

    categoria = toupper(categoria);
    if (categoria != 'S' && categoria != 'D' && categoria != 'C') {
        printf("\n  [!] Categoria invalida.\n");
        return;
    }

    printf("\n");
    for (int i = 0; i < cantidad; i++) {
        if (toupper(lista[i].categoria) == categoria) {
            if (aux == 0) imprimirEncabezadoTabla();
            imprimirFilaVehiculo(lista[i], i + 1);
            aux++;
        }
    }

    if (aux == 0) {
        printf("  [!] No hay vehiculos en la categoria '%c'.\n", categoria);
    } else {
        imprimirPieTabla();
    }
}

// Dividir usado/cero
void dividirArchivo() {
    FILE *origen, *ceroKm, *usados;
    Vehiculo vehiculo;
    int valorBool;

    origen = fopen("autos.txt", "r");

    if (origen == NULL) {
        printf("\n  [!] Error al abrir 'autos.txt' (Cargue algun auto primero).\n");
        return;
    }

    ceroKm = fopen("cerokm.txt", "w");
    usados = fopen("usados.txt", "w");

    if (ceroKm == NULL || usados == NULL) {
        printf("\n  [!] Error al crear los archivos de destino.\n");
        if (ceroKm != NULL) fclose(ceroKm);
        if (usados != NULL) fclose(usados);
        fclose(origen);
        return;
    }

    while (fscanf(origen, "%49[^|]|%c|%d|%f|%d\n",
                  vehiculo.modelo,
                  &vehiculo.categoria,
                  &vehiculo.anio,
                  &vehiculo.precio,
                  &valorBool) == 5) {
        vehiculo.esCero = (valorBool == 1);

        if (vehiculo.esCero) {
            fprintf(ceroKm, "%s|%c|%d|%.2f|1\n",
                    vehiculo.modelo,
                    vehiculo.categoria,
                    vehiculo.anio,
                    vehiculo.precio);
        } else {
            fprintf(usados, "%s|%c|%d|%.2f|0\n",
                    vehiculo.modelo,
                    vehiculo.categoria,
                    vehiculo.anio,
                    vehiculo.precio);
        }
    }

    fclose(origen);
    fclose(ceroKm);
    fclose(usados);

    printf("\n  [OK] Archivos Divididos: 'cerokm.txt' y 'usados.txt'.\n");
}

int main() {
    Vehiculo lista[max];
    int cantidad = 0;
    int opcion;

    do {
        printf("\n  +----------------------------------------+\n");
        printf("  |          GESTION DE VEHICULOS          |\n");
        printf("  +----------------------------------------+\n");
        printf("  |  [1] Cargar vehiculo                   |\n");
        printf("  |  [2] Ver todos los vehiculos           |\n");
        printf("  |  [3] Ver exclusivamente 0km            |\n");
        printf("  |  [4] Filtrar por categoria             |\n");
        printf("  |  [5] Dividir archivos (0km / Usados)  |\n");
        printf("  |                                        |\n");
        printf("  |  [0] Salir                             |\n");
        printf("  +----------------------------------------+\n");
        printf("  > Opcion: ");

        if (scanf("%d", &opcion) != 1) {
            opcion = -1; // Evita errores si ingresan una letra en el menu
        }
        while (getchar() != '\n');

        switch (opcion) {
            case 1:
                cantidad = cargarVehiculos(lista, cantidad);
                break;
            case 2:
                mostrarVehiculos(lista, cantidad);
                esperarCeroParaVolver();
                break;
            case 3:
                mostrar0km(lista, cantidad);
                esperarCeroParaVolver();
                break;
            case 4:
                mostrarPorCategoria(lista, cantidad);
                esperarCeroParaVolver();
                break;
            case 5:
                dividirArchivo();
                break;
            case 0:
                printf("\n  Saliendo del programa...\n");
                break;
            default:
                printf("\n  [!] Opcion invalida.\n");
                break;
        }

    } while (opcion != 0);

    return 0;
}
