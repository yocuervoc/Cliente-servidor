#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

struct dogType
{
    char Nombre[32]; //Nombre. Cadena de máximo 32 caracteres.
    char tipo[32]; //Tipo. [perro, gato, tortuga, etc] Cadena de máximo 32 caracteres.
    int edad;  //Edad [años]. Entero de 32 bits.Edad [años]. Entero de 32 bits.
    char raza[16]; //Raza. Cadena de máximo 16 caracteres.
    int estatura; //Estatura [cm]. Entero de 32 bits.
    float peso;   //Peso [Kg]. Real de 32 bits.
    char sexo;  //Sexo [H/M]. 1 caracter.
    struct dogType * siguiente;  //siguiente mascota en la tabla hash

};
//ingresa datos a la estructura dogType
int recibir(void *ap)
{

    struct dogType *pet;
    pet = ap;
    printf("Nombre de la mascota: ");
    scanf("%s", pet->Nombre);
    printf("Tipo: ");
    scanf("%s", pet->tipo);
    printf("Edad: ");
    scanf("%i", &pet->edad);
    printf("Raza: ");
    scanf("%s", pet->raza);
    printf("Estatura: ");
    scanf("%i", &pet->estatura);
    printf("Peso: ");
    scanf("%f", &pet->peso);
    printf("Sexo: ");
    scanf(" %c", &pet->sexo);
    pet->siguiente=NULL;
}

//imprime los datos de la estructura dogType
int imprimir(void *ap)
{

    struct dogType *pet;
    pet = ap;
    printf("\n %s ", pet->Nombre);
    printf("\n %s ", pet->tipo);
    printf("\n %i ", pet->edad);
    printf("\n %s ", pet->raza);
    printf("\n %i ", pet->estatura);
    printf("\n %f ", pet->peso);
    printf("\n %c ", pet->sexo);
}
void continuar()
{
    char t;
    printf("\nPess any key to continue: ");
    scanf("%c", &t);
    getchar();
}
//calcula la cantidad de mascotas en el archivo
long numeroMascotas(char FielFd[32])
{
    FILE *dogTypeData;

    dogTypeData = fopen (FielFd, "r");
    fseek(dogTypeData, 0, SEEK_END); //Corre el puntero de lectura
    long numeroPets = ftell (dogTypeData) / (sizeof(struct dogType)); //divide el tamano del archivo en el tamano de la estructura
    fclose(dogTypeData);
    return numeroPets;
}
void writePet(char FielFd[32])
{

    FILE *dogTypeData;
    dogTypeData = fopen (FielFd, "a");
    //verificacion de error en apertura
    if(dogTypeData==NULL)
    {
        printf("Error en la apertura del archivo");
        return 0;
    }

    struct dogType *pet;
    pet =malloc(sizeof(struct dogType));
    if (pet == NULL) perror("Error en la aisgnacion de memoria");
    recibir(pet);

    fseek(dogTypeData, 0, SEEK_END);
    fwrite (pet, sizeof( *pet), 1, dogTypeData);
    fclose(dogTypeData);
    free(pet);

}

void readPet(char FielFd[32], int registro)
{

    FILE *dogTypeData;
    dogTypeData = fopen (FielFd, "r");
    //verificacion de error en apertura
    if(dogTypeData==NULL)
    {
        printf("Error en la paertura del archivo");
        return 0;
    }

    struct dogType *pet;
    pet =malloc(sizeof(struct dogType));
    if (pet == NULL) {
        perror("No se pudo asignar memoria"); //verificaion de eerrores
    }
    registro=registro-1;
    fseek(dogTypeData, sizeof(struct dogType) * registro, SEEK_SET);  //ubica el puntero en el comienso de la estructura a leer
    fread (pet, sizeof(struct dogType), 1, dogTypeData); // lee desde la ubicaion del puntero hasta el tamano de la estrucura

    imprimir(pet);
    fclose(dogTypeData);
    free(pet);

}


//menu de sellecion impementado con un switch case
int contador_total=0;
struct hash_table *tabla = NULL;
void menu(char FielFd[32])
{
    int opcion_menu;

    do
    {
        make("dogtype.data");
        printf("\nDigite el numero de la opcion que quiere realizar:\n");
        printf("Opcion 1: Ingresar Registro\n");
        printf("Opcion 2: ver Registro\n");
        printf("Opcion 3: Borrar Registro\n");
        printf("Opcion 4: Buscar Registro\n");
        printf("Opcion 5: Sair\n");
        fflush(stdin);
        scanf (" %i", &opcion_menu);
        fflush(stdin);

        switch(opcion_menu)
        {

        case 1:
            writePet(FielFd);
            continuar();
            break;

        case 2:
            printf("Opcion 2: ver Registro\n");
            //printf("\n la cantidad de mascotas en la base de datos es %i", numeroMascotas(FielFd));
            printf("\ningrese el numero de registro que desea ver");
            int registro = 0;
            scanf ("%i", &registro);
            readPet(FielFd, registro);
            continuar();
            break;

        case 3:
            printf("Opcion 3: Borrar Registro\n");
            //printf("\n la cantidad de mascotas en la base de datos es %i", numeroMascotas(FielFd));
            printf("\n Ingrese el reguistro a borrar");
            int registroB = 0;
            scanf ("%i", &registroB);
            borrar(registroB, FielFd);
            continuar();
            break;


        case 4:
            printf("Opcion 4: Buscar Registro\n");
            printf("Busqueda por nombre: \n");
            char nombre [32];
            scanf("  %[^\n]s", nombre);
            searc(nombre);
            continuar();
            break;

        case 5:
            printf("Opcion 5: Sair\n");
            continuar();
            break;

        default:
            printf("Opcion invalida.");
            continuar();
            break;
        }

    }
    while (opcion_menu != 5);

}

////haseo
long hasheo(char nombre[32])
{

    long h=0;
    int i=0, j=0;
    int len = strlen(nombre);
    char nombre2[len+1];
    for ( i=0; nombre[i]; i++)
    {
        nombre2[i] = toupper(nombre[i]); //paso el nombre a minusculass
    }
    nombre2[i]= '\0';  //marca el fin de la cadena nombre
    for (j=0; j<32; j++)
    {
        if (nombre2[j]=='\0')
        {
            break;
        }
        h= h+ (long)(nombre2[j])*i;
    }
    return h%9973;  //establesco el tamano del hass con un numero primo

}

//tabla hash
struct hash_table
{
    struct dogType *Npet; // estructura de una estructura
    int key;
};
///////////////////////////////////7creacion de la tabla hash en memoria


void make (char FielFd[32])
{
    int index=0, i=0, j=0;
    struct hash_table *tabla_temp = NULL;
    // Adjudica espacio para un array de 9973 pos y tamano de la estructura
    tabla_temp = (struct hash_table *) calloc(9973, sizeof(struct hash_table));
    FILE *dogTypeData;
    dogTypeData = fopen (FielFd, "r");
    long Npets = numeroMascotas(FielFd);  //se obtiene el numero de mascotas

    contador_total = 0;
    struct dogType *pet;
    pet = malloc(sizeof(struct dogType));  // reserva de memoria para la mascota
    if (pet == NULL) perror("Error en la asignacion de memoria");

    for (i=0; i<(int)Npets; i++) // se leen todos las mascotas
    {
        struct dogType *pet;
        pet = malloc(sizeof(struct dogType));  //reserva de memoria para la estructura pet

        fseek(dogTypeData, sizeof(struct dogType) * i, SEEK_SET); // semueve el puntero
        fread (pet, sizeof(struct dogType), 1, dogTypeData);  // se lee el archivo desde el puntero

        index = hasheo(pet->Nombre);

        if (!tabla_temp[index].Npet) // se la posicion esta vacia agrego una mascota
        {
            tabla_temp[index].Npet = pet;
            tabla_temp[index].key = 1;
            contador_total++;
        }
        else
        {
            pet->siguiente = (tabla_temp[index].Npet);
            tabla_temp[index].Npet = pet;
            tabla_temp[index].key = (tabla_temp[index].key)+1;
            contador_total++;
        }

    }

    tabla = tabla_temp;
    fclose(dogTypeData);
    free(pet);
}

void searc(char nombre[32])
{
    int counts =0;
    int index = hasheo(nombre);  //se realiza el hash del nombre de la mascota
    struct dogType * searchP;
    searchP = tabla[index].Npet;  //se busca la mascota segun la key retornada por hasheo

    if (!searchP)
    {
        printf("Mascota no encontrada\n");
        return;
    }

    while (searchP != NULL)
    {
        if (hasheo(searchP->Nombre) == hasheo(nombre)
                && strcasecmp(searchP->Nombre, nombre) == 0)
        {
            counts++;
            imprimir(searchP);
        }
        searchP = searchP->siguiente;
    }

    printf("Hay %d registros en el hash\n", tabla[index].key);
    printf("hay %d registros con el mismo numero\n", counts);
}
void borrar (int registroBorrar, char FielFd[32])
{  
int i = 0, j = 0,tam= 0;
    struct dogType *borrarPet;
    borrarPet = malloc(sizeof(struct dogType));
    if (borrarPet == NULL)perror("Error en la asignacion de memoria");

    FILE *dogTypeData;
    dogTypeData = fopen (FielFd, "r");

    FILE *dogTypeData2;
    dogTypeData2 = fopen ("dogtype2.data", "a");

    long npet = numeroMascotas(FielFd);
    registroBorrar = registroBorrar - 1;

    tam = (int)npet;

    for (i = 0; i <tam; i++)
    {
        if (i == registroBorrar) continue;  // evito copiar el registro indeseado
        if (i <= tam)
        {
            fseek(dogTypeData, sizeof(struct dogType) * i, SEEK_SET); // muevo el puntero al registro i en el archivo original
            fread (borrarPet, sizeof(struct dogType), 1,dogTypeData);// leeo la amscota en el archovo original
            fseek(dogTypeData2, 0, SEEK_END);                       // muevo el puntero al final en el archvo copia
            fwrite (borrarPet, sizeof(struct dogType), 1, dogTypeData2); // escrivo la mascota en el archov copia
        }
    }
    // closed ficheros
    fclose(dogTypeData);
    fclose(dogTypeData2);
    free(borrarPet);
    system("mv dogtype2.data dogtype.data");  //remplazo el archvo original con el copia
    make(FielFd);
    printf("se ha borrado el registro %i\n",registroBorrar+1);
}
#endif // FUNCIONES_H_INCLUDED
