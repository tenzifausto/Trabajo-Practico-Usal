#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define str 50
#define max 999

//Estructura

typedef struct {
    char modelo[str]; //modelo del auto
    char categoria; //sedan, deportivo o camioneta
    int anio; //año de fabricacion
    float precio; //precio del auto
    bool esCero; //si es 0km o no
} Vehiculo;

//Carga de datos

int cargarVehiculos(Vehiculo lista[], int cantidad) {
    char continuar;
    FILE *archivo;
    archivo = fopen("autos.txt", "a");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    }

    do {
        printf("\n--- CARGA DE VEHICULO #%d ---\n", cantidad + 1);

        printf("Ingrese modelo: ");
        scanf(" %[^\n]", lista[cantidad].modelo);
        while (getchar() != '\n');

        printf("Ingrese categoria\nS(Sedan)\nD(Deportivo)\nC(Camioneta)\n ");
        scanf(" %c", &lista[cantidad].categoria);
        while (getchar() != '\n');

        printf("Ingrese año: ");
        scanf("%d", &lista[cantidad].anio);
        while (getchar() != '\n');

        printf("Ingrese precio: ");
        scanf("%f", &lista[cantidad].precio);
        while (getchar() != '\n');

        int auxBool;
        printf("Es 0km? (1 = Si, 0 = No): ");
        scanf("%d", &auxBool);
        while (getchar() != '\n');

        lista[cantidad].esCero = (auxBool == 1);
        cantidad++; // suma un auto a la lista

        printf("\nVehiculo guardado correctamente! Desea ingresar otro? (s/n): ");
        scanf(" %c", &continuar);
        while (getchar() != '\n');

    } while (continuar == 's' || continuar == 'S');

    return cantidad;
}

// mostrar los datos
void mostrarVehiculos(Vehiculo lista[], int cantidad) {
    printf("\n================ VEHICULOS CARGADOS ================\n");

    for (int i = 0; i < cantidad; i++) {
        printf("\n--- Vehiculo #%d ---\n", i + 1);
        printf("Modelo:    %s\n", lista[i].modelo);
        printf("Categoria: %c\n", lista[i].categoria);
        printf("Año:      %d\n", lista[i].anio);
        printf("Precio:    $%.2f\n", lista[i].precio);
        printf("Es 0km?:   %s\n", lista[i].esCero ? "Si" : "No");
    }

    printf("\n=============================================================\n");
}

void mostrar0km(Vehiculo lista[], int cantidad) {
    int aux=0;
    for (int i = 0; i < cantidad; i++) {
        if (lista[i].esCero) {
            printf("\n================ VEHICULOS 0KM ================\n");
            printf("\n--- Vehiculo #%d ---\n", i + 1);
            printf("Modelo:    %s\n", lista[i].modelo);
            printf("Categoria: %c\n", lista[i].categoria);
            printf("Año:      %d\n", lista[i].anio);
            printf("Precio:    $%.2f\n", lista[i].precio);
            printf("Es 0km?:   %s\n", lista[i].esCero ? "Si" : "No");
            aux++;
        }
    }
    if (aux == 0) {
        printf("No hay vehiculos 0km.\n");
    }
    printf("\n=============================================================\n");
}

void mostrarPorCategoria(Vehiculo lista[], int cantidad) {
    char categoria;
    int aux=0;

    printf("\n================ CATEGORIA ================\n");
    printf("S. Sedan\n");
    printf("D. Deportivo\n");
    printf("C. Camioneta\n");
    printf("Ingrese categoria:");
    scanf("%c", &categoria);
    while (getchar() != '\n');

    categoria = toupper(categoria);
    if (categoria != 'S' && categoria != 'D' && categoria != 'C') {
        printf("Categoria invalida\n");
    }else {
        for (int i = 0; i < cantidad; i++) {
            if (lista[i].categoria == categoria) {
                printf("\n--- Vehiculo #%d ---\n", i + 1);
                printf("Modelo:    %s\n", lista[i].modelo);
                printf("Categoria: %c\n", lista[i].categoria);
                printf("Año:      %d\n", lista[i].anio);
                printf("Precio:    $%.2f\n", lista[i].precio);
                printf("Es 0km?:   %s\n", lista[i].esCero ? "Si" : "No");
                aux++;
            }
        }
        if (aux == 0) {
            printf("No hay vehiculos de esa categoria\n");
        }
    }
    printf("\n=============================================================\n");
}

void dividirArchivo() {
    FILE *origen, *ceroKm, *usados;
    Vehiculo vehiculo;
    int valorBool;

    origen = fopen("autos.txt", "r");

    if(origen == NULL) {
        printf("Error al abrir autos.txt\n");
        return;
    }
    ceroKm = fopen("autos.txt", "w");
    usados = fopen("usados.txt", "w");

    if (ceroKm == NULL || usados == NULL) {
        printf("Error al abrir archivos\n");
        if (ceroKm != NULL) {
            fclose(ceroKm);
        }
        if (usados != NULL) {
            fclose(usados);
        }
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

    printf("Archivos separados correctamente.\n");
}

int main() {
    Vehiculo lista[max];
    int cantidad = 0;
    int opcion;

    do {
        printf("\n================ MENU PRINCIPAL ================\n");
        printf("1. Cargar datos\n");
        printf("2. Mostrar datos\n");
        printf("3. Mostrar unicamente autos 0km\n");
        printf("4. Mostrar autos por categoria\n");
        printf("5. Dividir archivos de autos\n");

        // espacio para agregar otras opciones...

        printf("0. Salir del programa\n");
        printf("================================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        while (getchar() != '\n');

        switch (opcion) {
            case 1:
                cantidad = cargarVehiculos(lista, cantidad); // carga de datos
                break;

            case 2:
                mostrarVehiculos(lista, cantidad); // mostrar los datos
                break;

            case 3:
                mostrar0km(lista, cantidad);
                break;

            case 4:
                mostrarPorCategoria(lista, cantidad);
                break;

            case 5:
                dividirArchivo();
                break;


                // espacio para agregar otras opciones..
                // ejemplo case 3: ... break;

            case 0:
                printf("\nSaliendo del programa...\n");
                break;

            default:
                printf("\n[!] Opcion invalida. Intente nuevamente.\n");
                break;
        }

    } while (opcion != 0);

    return 0;
}

