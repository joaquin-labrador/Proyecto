#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "pila.h"

//DEFINO CONSTANTES PARA USAR
#define max_libros 100
#define max_letras 150
#define archivo_leido "librosLeidos.bin"
#define archivo_noleidos "librosALeer.bin"
#define archivo_leyendo "librosLeidos.bin"
#define ordenado_puntaje "ordenarPuntaje.bin"

typedef struct
{
    char usuario[max_letras];
    char pass[max_letras];
    int id;
    int activo; //1 si esta activo, 0 sino
} inicio;

typedef struct
{
    int id;                   //
    char titulo[max_letras];  //
    char autor[max_letras];   //  PARA TODOS
    char generoPrincipal[50]; //
    char borrado;             //
    char review[max_letras];  // PARA LIBROS LEIDOS
    float puntaje;            // PARA LIBROS LEIDOS
    int progreso;             // PARA LIBROS LEYENDO
} librosTotal;
/************************************************
             PORTOTIPADOS 
*************************************************/

int menuPrincipal();
int ejecutarMenu();
void opcionSeleccionadaPrincipal(int opcion);
int menuVer();
int menuCargarLibros();
int menuOrdenar();
int menuEliminarLibros();

void cargarLibro(int opcion);
void verTodosLosLibros(int opcion);
void verLibroLeyendo(librosTotal libro);
void verLibroLeidos(librosTotal libro);
void verLibroNoleidos(librosTotal libro);

int idElementoAEliminar(int opcion, char nombreArchivo[]);
void eliminarLibro(int id, char nombreArchivo[]);
int posID(char nombreArchivo[], int idABuscar);

int checkearSiYaExiste(char archivo[], char nombre[], int dim);
int cantRegistros(char nombre[]);
int posIDUsuario(char nombreArchivo[], int idABuscar);
void cambiarActivo(int id, char nombreArchivo[]);

int archivoArreglo(librosTotal mejores[]);
void mostrarArreglo(librosTotal arreglo[], int validos);
int mayorPuntaje(librosTotal mejores[], int comienzo, int validos);
void ordenarSeleccion(librosTotal arreglo[], int validos);

int generarNumero();
int buscarLibroPorISBN(int ISBN, char nombreArchivo[]);
int generarID();

int menuUsuarios();
int menuInciarsesion();
void crearCuenta();
void menuUsuariosHandler(int opcion);
int retornarUsuarioActivo();
int usuarioPorId(int id, char arr[]);
int usuarioExiste(char usuario[]);

int main()
{
    printf("BIENVENIDO!\n");
    int opcionUsuarios = menuUsuarios();
    menuUsuariosHandler(opcionUsuarios);

    int opcionPrincipal = ejecutarMenu();
    opcionSeleccionadaPrincipal(opcionPrincipal);

    return 0;
}
/*
    #########################################################
 
        LOGICA DE INICIO DE SESION
 
    #########################################################
*/

int menuUsuarios()
{
    int opcionElegida = 0;

    do
    {
        printf("1 Iniciar Sesion\n");
        printf("2 Crear cuenta\n");
        printf("3 Salir\n");
        scanf("%d", &opcionElegida);

        system("cls");
    } while (opcionElegida < 1 || opcionElegida > 3);

    return opcionElegida;
}

//LOGICA DE MENUS -----------------------------------------------------------
//---------------------------------------------------------------------------
int menuPrincipal()
{
    int opcion = 0;

    do
    {
        printf("1. Cargar libros\n");
        printf("2. Ver mis libros\n");
        printf("3. Eliminar libros\n");
        printf("4. Cerrar sesion\n");
        printf("Ingrese su eleccion: ");
        scanf("%d", &opcion);
        printf("\n");

        system("cls");

    } while (opcion > 4 || opcion < 1);

    return opcion;
}
int menuVer()
{
    int opcionMenuVer = 0;

    do
    {
        printf("1. Libros leidos\n");
        printf("2. Libros que quiero leer\n");
        printf("3. Libros que estoy leyendo\n");
        printf("4. Volver\n");
        printf("Ingrese su eleccion: ");
        scanf("%d", &opcionMenuVer);
        printf("\n");

        system("cls");

    } while (opcionMenuVer < 1 || opcionMenuVer > 4);

    return opcionMenuVer;
}

int menuCargarLibros()
{
    int opcion = 0;

    do
    {
        printf("1.Cargar libro leido\n");
        printf("2.Cargar libro para leer\n");
        printf("3.Cargar libro que estoy leyendo\n");
        printf("4.Volver\n");
        printf("Ingrese su eleccion: ");
        scanf("%d", &opcion);
        printf("\n");
        system("cls");

    } while (opcion > 4 || opcion < 1);

    return opcion;
}

int menuOrdenar()
{
    int opcion = 0;
    do
    {
        printf("1. Ver mis mejores libros\n");
        printf("2. Ver historial de libros\n");
        printf("3. Volver\n");
        printf("Ingrese su eleccion: ");
        scanf("%d", &opcion);
        printf("\n");
        system("cls");

    } while (opcion > 3 || opcion < 1);

    return opcion;
}

int menuEliminarLibros()
{
    int opcion = 0;
    do
    {
        printf("1.Eliminar Leido\n");
        printf("2.Eliminar Para Leer\n");
        printf("3.Eliminar Leyendo\n");
        printf("4.Volver");
        printf("Ingrese su eleccion: ");
        scanf("%d", &opcion);
        printf("\n");
        system("cls");

    } while (opcion > 4 || opcion < 1);

    return opcion;
}
//SE ENCARGA DE LA LOGICA DEL MENU.
void menuUsuariosHandler(int opcion)
{
    if (opcion == 1)
    {
        menuInciarsesion();
    }
    else if (opcion == 2)
    {
        crearCuenta();
        menuUsuariosHandler(menuUsuarios()); //Para que vuelva a mostrar el munu de usuario despues de crear la cuenta
    }
    else
    {
        exit(0);
    }
}
//PROTOTIPO DE COMO VA A FUNCIONAR PARA BUSCAR LOS ARCHIVOS DE CADA USUARIO
int retornarUsuarioActivo() //GUARDA EN EL ARRAY EL NOMBRE DEL USUARIO ACTIVO
{
    inicio temp;
    FILE *archi;
    int id = 0;
    archi = fopen("usuarios.bin", "rb"); // abro el archivo de usuarios
    //PARA ABRIR EL ARCHIVO, BUSCO CUAL ES EL USUARIO QUE TIENE LA PROPIEDAD DE ACTIVO EN 1.
    if (archi != NULL)
    {
        while (fread(&temp, sizeof(inicio), 1, archi) > 0)
        {
            if (temp.activo == 1)
            {
                id = temp.id;
                fclose(archi);
                return id; // EXITO
            }
        }
    }
    else
    {
        printf("ERROR.\n");
    }

    fclose(archi);

    return -1; // NO EXITO
}

int ejecutarMenu()
{
    int opcionMenuPrincipal;
    //GUARDO LA OPCION DEL MENU PRINCIPAL Q SELECCIONE
    do
    {

        opcionMenuPrincipal = menuPrincipal();

    } while (opcionMenuPrincipal > 5 || opcionMenuPrincipal < 1);

    if (opcionMenuPrincipal == 4)
    {
        menuUsuariosHandler(menuUsuarios());

        int activo = retornarUsuarioActivo();

        if (activo == -1)
        {
            printf("no hay usuarios activos.\n");
        }
        else
        {
            cambiarActivo(activo, "usuarios.bin");
        }

        int opcionPrincipal = ejecutarMenu();
        opcionSeleccionadaPrincipal(opcionPrincipal);
    }

    return opcionMenuPrincipal;
}

//EJECUTA EL 2DO MENU SEGUN LO QUE HAYAS SELECCIONADO EN EL MENU PRINCIPAL.
void opcionSeleccionadaPrincipal(int opcion)
{
    if (opcion == 1)
    {
        int opcionMenu = menuCargarLibros();
        if (opcionMenu == 1)
        {
            cargarLibro(opcionMenu);
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 2)
        {
            cargarLibro(opcionMenu);
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 3)
        {
            cargarLibro(opcionMenu);
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 4)
        {
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
    }
    else if (opcion == 2)
    {
        int opcionMenu = menuVer();
        if (opcionMenu == 1)
        {
            int opcionMenuOrdenar = menuOrdenar();
            if (opcionMenuOrdenar == 1)
            {
                //LOGICA PARA ORDENAR POR PUNTAJE
                librosTotal mejores[max_libros];
                int validos = archivoArreglo(mejores);
                ordenarSeleccion(mejores, validos);

                mostrarArreglo(mejores, validos);

                opcionSeleccionadaPrincipal(ejecutarMenu());
            }
            else if (opcionMenuOrdenar == 2)
            {
                librosTotal mejores[max_libros];
                int validos = archivoArreglo(mejores);

                mostrarArreglo(mejores, validos);

                opcionSeleccionadaPrincipal(ejecutarMenu());
            }
            else
            {
                opcionSeleccionadaPrincipal(2);
                int opcionMenu = menuVer();
            }
        }
        else if (opcionMenu == 2)
        {
            verTodosLosLibros(opcionMenu);
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 3)
        {
            verTodosLosLibros(opcionMenu);
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 4)
        {
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
    }
    else if (opcion == 3)
    {
        int opcionMenu = menuEliminarLibros();
        if (opcionMenu == 1)
        {
            char usuario[90];
            usuarioPorId(retornarUsuarioActivo(), usuario);
            strcat(usuario, "librosLeidos.bin");

            int posisicion = idElementoAEliminar(1, usuario);
            eliminarLibro(posisicion, usuario);

            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
        else if (opcionMenu == 4)
        {
            opcionSeleccionadaPrincipal(ejecutarMenu());
        }
    }
}

/*
 
    #############################################################
 
    FUNCIONES CARGAR
 
    #############################################################
 
*/

void cargarLibro(int opcion)
{
    char mander = 's';
    char review;
    int i = 0;
    int existe = 0;
    librosTotal libros[max_libros];

    if (opcion == 1)
    {
        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, "librosLeidos.bin");

        FILE *archi;
        archi = fopen(usuario, "ab");

        while (tolower(mander) == 's')
        {
            fflush(stdin);

            printf("Titulo: ");
            gets(libros[i].titulo);
            printf("\n");

            existe = checkearSiYaExiste(usuario, libros[i].titulo, cantRegistros(usuario));
            if (existe == 1)
            {
                system("cls");
                printf("El libro ya existe.\n");
                system("pause");
                system("cls");
                opcionSeleccionadaPrincipal(ejecutarMenu());
            }

            printf("Autor: ");
            gets(libros[i].autor);
            printf("\n");

            printf("Genero principal: ");
            gets(libros[i].generoPrincipal);
            printf("\n");

            printf("Puntaje (formato decimal de 0 a 5): ");
            scanf("%f", &libros[i].puntaje);
            printf("\n");

            printf("Desea dejar una review de el libro? (150 caracteres maximo)\n");
            printf("S para SI: ");
            fflush(stdin);
            scanf("%c", &review);

            printf("\n");

            fflush(stdin);
            if (tolower(review) == 's')
            {
                printf("Review: ");
                gets(libros[i].review);
                printf("\n");
            }
            else
            {
                memcpy(libros[i].review, "No hay review", 13);
            }

            libros[i].borrado = 'n';

            do
            {
                libros[i].id = generarNumero();
            } while (buscarLibroPorISBN(libros[i].id, usuario) == 0);

            fwrite(&libros[i], sizeof(librosTotal), 1, archi);

            printf("Desea seguir cargando libros?\n");
            printf("S para SI: ");
            fflush(stdin);
            scanf("%c", &mander);

            printf("\n");

            i++;

            system("cls");
        }
        fclose(archi);
    }
    else if (opcion == 2)
    {
        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, archivo_noleidos);

        FILE *archi;

        archi = fopen(usuario, "ab");

        while (tolower(mander) == 's')
        {
            fflush(stdin);

            printf("Titulo: ");
            gets(libros[i].titulo);
            printf("\n");

            existe = checkearSiYaExiste(archivo_noleidos, libros[i].titulo, cantRegistros(archivo_noleidos));
            if (existe == 1)
            {
                system("cls");
                printf("El libro ya existe.\n");
                system("pause");
                system("cls");
                opcionSeleccionadaPrincipal(ejecutarMenu());
            }

            printf("Autor: ");
            gets(libros[i].autor);
            printf("\n");

            printf("Genero principal: ");
            gets(libros[i].generoPrincipal);
            printf("\n");

            libros[i].borrado = 'n';

            do
            {
                libros[i].id = generarNumero();
            } while (buscarLibroPorISBN(libros[i].id, usuario) == 0);

            fwrite(&libros[i], sizeof(librosTotal), 1, archi);

            printf("Desea seguir cargando libros?\n");
            printf("S para SI: ");
            fflush(stdin);
            scanf("%c", &mander);

            printf("\n");

            i++;

            system("cls");
        }
        fclose(archi);
    }
    else
    {
        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, archivo_leyendo);

        FILE *archi;

        archi = fopen(usuario, "ab");

        while (tolower(mander) == 's')
        {
            fflush(stdin);

            printf("Titulo: ");
            gets(libros[i].titulo);
            printf("\n");

            existe = checkearSiYaExiste(archivo_leyendo, libros[i].titulo, cantRegistros(archivo_leyendo));
            if (existe == 1)
            {
                system("cls");
                printf("El libro ya existe.\n");
                system("pause");
                system("cls");
                opcionSeleccionadaPrincipal(ejecutarMenu());
            }

            printf("Autor: ");
            gets(libros[i].autor);
            printf("\n");

            printf("Genero principal: ");
            gets(libros[i].generoPrincipal);
            printf("\n");

            printf("Porcentaje de el libro leido: ");
            scanf("%d", &libros[i].progreso);
            printf("\n");

            libros[i].borrado = 'n';

            do
            {
                libros[i].id = generarNumero();
            } while (buscarLibroPorISBN(libros[i].id, usuario) == 0);

            fwrite(&libros[i], sizeof(librosTotal), 1, archi);

            printf("Desea seguir cargando libros?\n");
            printf("S para SI: ");
            fflush(stdin);
            scanf("%c", &mander);

            printf("\n");

            i++;

            system("cls");
        }
        fclose(archi);
    }
}

/*
 
    #############################################################
 
    FUNCIONES VER
 
    #############################################################
 
*/

void verTodosLosLibros(int opcion)
{
    librosTotal libros;

    if (opcion == 1)
    {
        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, archivo_leido);

        FILE *archi;

        archi = fopen(usuario, "rb");

        if (archi != NULL)
        {
            while (fread(&libros, sizeof(librosTotal), 1, archi) > 0)
            {
                if (libros.borrado == 'n')
                {
                    verLibroLeidos(libros);
                }
            }
        }
        else
        {
            printf("No hay libros.\n");
        }
        fclose(archi);
    }
    else if (opcion == 2)
    {

        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, archivo_noleidos);

        FILE *archi;

        archi = fopen(usuario, "rb");

        if (archi != NULL)
        {
            while (fread(&libros, sizeof(librosTotal), 1, archi) > 0)
            {
                if (libros.borrado == 'n')
                {
                    verLibroNoleidos(libros);
                }
            }
        }
        else
        {
            printf("No hay libros.\n");
        }
        fclose(archi);
    }
    else
    {

        char usuario[90];
        usuarioPorId(retornarUsuarioActivo(), usuario);

        strcat(usuario, archivo_leyendo);

        FILE *archi;

        archi = fopen(usuario, "rb");

        if (archi != NULL)
        {
            while (fread(&libros, sizeof(librosTotal), 1, archi) > 0)
            {
                if (libros.borrado == 'n')
                {
                    verLibroLeyendo(libros);
                }
            }
            fclose(archi);
        }
        else
        {
            printf("No hay libros.\n");
        }
    }
    system("pause");
    system("cls");
}

void verLibroLeyendo(librosTotal libro)
{
    printf("\n");
    printf("------------------------------------");
    printf("\nID %d:", libro.id);
    printf("\nTitulo: %s", libro.titulo);
    printf("\n");
    printf("Autor: %s", libro.autor);
    printf("\n");
    printf("Genero principal: %s", libro.generoPrincipal);
    printf("\n");
    printf("Progreso: %d de 100%%.", libro.progreso);
    printf("\n");
    printf("------------------------------------");
    printf("\n");
}

void verLibroLeidos(librosTotal libro)
{
    printf("\n");
    printf("------------------------------------");
    printf("\nID %d:", libro.id);
    printf("\nTitulo: %s", libro.titulo);
    printf("\n");
    printf("Autor: %s", libro.autor);
    printf("\n");
    printf("Genero principal: %s", libro.generoPrincipal);
    printf("\n");
    printf("Puntaje: %.1f", libro.puntaje);
    printf("\n");
    printf("Review: %s", libro.review);
    printf("\n");
    printf("------------------------------------");
    printf("\n");
}

void verLibroNoleidos(librosTotal libro)
{
    printf("\n");
    printf("------------------------------------");
    printf("\nID %d:", libro.id);
    printf("\nTitulo: %s", libro.titulo);
    printf("\n");
    printf("Autor: %s", libro.autor);
    printf("\n");
    printf("Genero principal: %s", libro.generoPrincipal);
    printf("\n");
    printf("------------------------------------");
    printf("\n");
}

/*
 
    #############################################################
 
    FUNCIONES ELIMINAR
 
    #############################################################
 
*/

int idElementoAEliminar(int opcion, char nombreArchivo[])
{
    FILE *archi;
    archi = fopen(nombreArchivo, "rb");
    int eliminar = 0;

    printf("Ingrese la ID de el libro que desea eliminar al final de la muestra");

    if (archi != NULL)
    {
        verTodosLosLibros(opcion);
        do
        {
            fflush(stdin);
            printf("ID del libro a eliminar: ");
            scanf("%d", &eliminar);
        } while (eliminar < 0);
    }
    else
    {
        printf("ERROR");
    }

    fclose(archi);

    return eliminar;
}

void eliminarLibro(int id, char nombreArchivo[])
{
    FILE *archi;
    librosTotal libros;
    archi = fopen(nombreArchivo, "rb+");

    int pos = posID(nombreArchivo, id);

    if (pos == -1)
    {
        printf("Esa ID no existe.\n");
        opcionSeleccionadaPrincipal(ejecutarMenu());
    }

    if (archi != NULL)
    {

        fseek(archi, sizeof(librosTotal) * (pos - 1), SEEK_SET);
        fread(&libros, sizeof(librosTotal), 1, archi);

        libros.borrado = 's';

        fseek(archi, sizeof(librosTotal) * (pos - 1), SEEK_SET);
        fwrite(&libros, sizeof(librosTotal), 1, archi); // despues guardo

        fclose(archi);
    }
    else
    {
        printf("ERROR\n");
    }

    system("cls");
}

int posID(char nombreArchivo[], int idABuscar)
{
    FILE *archi;
    librosTotal libros;
    archi = fopen(nombreArchivo, "rb");
    int i = 0;

    if (archi != NULL)
    {
        while (fread(&libros, sizeof(librosTotal), 1, archi) > 0)
        {
            i++;
            if (libros.id == idABuscar)
            {
                return i;
            }
        }
    }
    else
    {
        printf("error\n");
    }
    fclose(archi);
    return -1;
}

/*
 
    #############################################################
 
    FUNCIONES ADICIONALES
 
    #############################################################
 
*/

int checkearSiYaExiste(char archivo[], char nombre[], int dim)
{
    FILE *leidos;
    librosTotal libros;
    leidos = fopen(archivo, "rb");
    int i = 0;
    if (leidos != NULL)
    {
        while (fread(&libros, sizeof(librosTotal), 1, leidos) > 0 && i < dim - 1)
        {
            if (strcasecmp(libros.titulo, nombre) == 0)
            {
                fclose(leidos);
                return 1;
            }
            i++;
        }
        fclose(leidos);
    }
    else
    {
        printf("ERROR.\n");
    }

    return 0;
}

int cantRegistros(char nombre[])
{
    FILE *buffer;
    librosTotal libros;
    buffer = fopen(nombre, "rb");
    int cant = 0;
    if (buffer != NULL)
    {
        fseek(buffer, 0, SEEK_END);
        cant = ftell(buffer) / sizeof(libros);
    }
    fclose(buffer);

    return cant;
}

int posIDUsuario(char nombreArchivo[], int idABuscar)
{
    FILE *archi;
    inicio usu;
    archi = fopen(nombreArchivo, "rb");
    int i = 0;

    if (archi != NULL)
    {
        while (fread(&usu, sizeof(inicio), 1, archi) > 0)
        {
            i++;
            if (usu.id == idABuscar)
            {
                return i;
            }
        }
    }
    else
    {
        printf("error\n");
    }
    fclose(archi);
    return -1;
}

void cambiarActivo(int id, char nombreArchivo[])
{
    FILE *archi;
    inicio usu;
    archi = fopen(nombreArchivo, "rb+");

    int pos = posIDUsuario(nombreArchivo, id);

    if (pos == -1)
    {
        printf("Esa ID no existe.\n");
        opcionSeleccionadaPrincipal(ejecutarMenu());
    }

    if (archi != NULL)
    {

        fseek(archi, sizeof(inicio) * (pos - 1), SEEK_SET);
        fread(&usu, sizeof(inicio), 1, archi);

        if (usu.activo == 0)
        {
            usu.activo = 1;
        }
        else
        {
            usu.activo = 0;
        }

        fseek(archi, sizeof(inicio) * (pos - 1), SEEK_SET);
        fwrite(&usu, sizeof(inicio), 1, archi); // despues guardo

        fclose(archi);
    }
    else
    {
        printf("ERROR\n");
    }

    system("cls");
}

/*
    #########################################################
 
        LOGICA DE ARCHIVO A ARREGLO Y ORDENACION.
 
    #########################################################
*/

int archivoArreglo(librosTotal mejores[])
{
    char usuario[90];
    usuarioPorId(retornarUsuarioActivo(), usuario);

    strcat(usuario, "librosLeidos.bin");

    FILE *archi;
    archi = fopen(usuario, "rb");
    librosTotal temp;
    int i = 0;

    if (archi != NULL)
    {
        while (fread(&temp, sizeof(librosTotal), 1, archi) > 0)
        {
            if (temp.borrado == 'n')
            {
                mejores[i] = temp;
                i++;
            }
        }
    }

    return i;
}

void mostrarArreglo(librosTotal arreglo[], int validos)
{
    for (int i = 0; i < validos; i++)
    {
        verLibroLeidos(arreglo[i]);
    }

    system("pause");
    system("cls");
}

int mayorPuntaje(librosTotal mejores[], int comienzo, int validos)
{
    int mayor = mejores[comienzo].puntaje;
    int index = comienzo;

    for (int i = comienzo + 1; i < validos; i++)
    {
        if (mejores[i].puntaje > mayor)
        {
            mayor = mejores[i].puntaje;
            index = i;
        }
    }

    return index;
}

void ordenarSeleccion(librosTotal arreglo[], int validos)
{
    librosTotal arreglo2[1];
    for (int i = 0; i < validos - 1; i++)
    {
        int aux = mayorPuntaje(arreglo, i, validos);
        arreglo2[0] = arreglo[i];
        arreglo[i] = arreglo[aux];
        arreglo[aux] = arreglo2[0];
    }
}

int generarNumero()
{
    time_t t;
    srand((unsigned)time(&t));

    return rand() % 500 + 1;
}

int buscarLibroPorISBN(int ISBN, char nombreArchivo[])
{
    librosTotal temp;
    FILE *archi;
    archi = fopen(nombreArchivo, "rb");

    while (fread(&temp, sizeof(librosTotal), 1, archi) > 0)
    {
        if (temp.id == ISBN)
        {
            fclose(archi);
            return 0; //LO ENCONTRO
        }
    }
    fclose(archi);
    return 1; //NO LO ENCONTRO
}

int generarID()
{
    time_t t;
    srand((unsigned)time(&t));

    return rand() % 100 + 1;
}

//SAQUE LOS GOTOXY PORQUE NO FUNCIONAN EN CODEBLOCKS.
//HICE CAMBIOS PARA NO TENER UN ARCHIVO POR USUARIO ASI PUEDO ITERAR POR TODOS LOS USUARIOS.
int menuInciarsesion()
{
    inicio usuario;
    int flag = 1;
    char pass[30];
    char usuarioInicio[30];

    do
    {
        printf("Ingrese su nombre de usuario: ");
        fflush(stdin);
        gets(usuarioInicio);
    } while (strcmp(usuarioInicio, "") == 0);

    FILE *archi;
    archi = fopen("usuarios.bin", "a+b");

    if (archi != NULL)
    {
        while (fread(&usuario, sizeof(inicio), 1, archi) > 0)
        {
            if (strcmp(usuario.usuario, usuarioInicio) == 0)
            {
                flag = 0;
                printf("\n");

                printf("Ingrese contrasenia: ");
                fflush(stdin);
                gets(pass);

                int intento = 5;
                int compare = strcmp(usuario.pass, pass);
                if (compare == 0)
                {
                    cambiarActivo(usuario.id, "usuarios.bin");
                    fclose(archi);
                    system("cls");
                    return 0;
                }
                while (compare != 0)
                {
                    printf("\n");
                    printf("Contrasenia incorrecta\n");
                    printf("\n");
                    printf("Vuelva intertar le quedan %d intentos", intento);
                    fflush(stdin);
                    gets(pass);
                    intento--;
                    compare = strcmp(usuario.pass, pass);
                    system("cls");
                    if (intento == 0)
                    {
                        printf("Cuenta bloqueada\n");
                        fclose(archi);
                        sleep(2);
                        exit(0);
                    }
                }
            }
        }
        if (flag == 1)
        {
            printf("El usuario no existe.\n");
            system("pause");
            system("cls");
            fclose(archi);
            menuUsuariosHandler(menuUsuarios());
        }
        fclose(archi);
    }
    else
    {
        printf("\n");
        printf("No hay usuarios.\n");
        fclose(archi);
        exit(0);
    }
    system("cls");
    fclose(archi);
}

//FUNCION QUE SE ENCARGA DE CREAR LA CUENTA
void crearCuenta()
{
    FILE *archi;
    archi = fopen("usuarios.bin", "ab"); //voy a querer cargar todos los usuarios en un archivo para poder trabajar con ellos.

    inicio crear;

    do
    {
        printf("Ingrese su nombre de usuario: ");
        fflush(stdin);
        gets(crear.usuario);
    } while (strcmp(crear.usuario, "") == 0 || usuarioExiste(crear.usuario) == 1); //CHECKEA QUE NO INGRESE UN USUARIO VACIO;

    printf("\n");
    do
    {
        printf("Ingrese su contrasenia: ");
        fflush(stdin);
        gets(crear.pass);
    } while (strcmp(crear.usuario, "") == 0); // ...

    crear.activo = 0;

    do
    {
        crear.id = generarID();
    } while (buscarLibroPorISBN(crear.id, "usuarios.bin") == 0);

    fwrite(&crear, sizeof(inicio), 1, archi);
    system("cls");

    fclose(archi);
}

int usuarioPorId(int id, char arr[])
{
    inicio temp;
    FILE *archi;
    archi = fopen("usuarios.bin", "rb");
    if (archi != NULL)
    {
        while (fread(&temp, sizeof(inicio), 1, archi) > 0)
        {
            if (temp.id == id)
            {
                strcpy(arr, temp.usuario);
                fclose(archi);
                return 0; // EXITO
            }
        }
    }
    else
    {
        printf("ERROR.\n");
    }

    fclose(archi);

    return -1; // NO EXITO
}

int usuarioExiste(char usuario[])
{
    FILE *archi;
    archi = fopen("usuarios.bin", "rb");
    inicio usu;

    while (fread(&usu, sizeof(inicio), 1, archi) > 0)
    {
        if (strcmp(usu.usuario, usuario) == 0)
        {
            printf("\nUsuario ya existente.\n");
            system("pause");
            system("cls");
            return 1;
        }
    }

    fclose(archi);

    return 0;
}