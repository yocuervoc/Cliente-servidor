#include <stdio.h>
#include <stdlib.h>
#include<time.h>
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

char *razaR(){										//Selecciona una cadena de caracteres al azar (Listado en una cadena de cadenas)
	char raza[23][16]=
        {"Akita",
		"Afgano",
		"Labrador",
		"Bóxer",
		"Beagle",
		"Bull Terrier",
		"Cairn Terrier",
		"Rottweiler",
		"Collie Smooth",
		"American Curl",
		"Europeo común",
		"Khao Manee",
		"Siamés",
		"Siberiano",
		"Tonkinés",
		"Chadnoso",
		"Galapagos",
		"Carey",
		"Boba",
		"Mora",
		"Matamata",
		"laud",
		"Abisinio"};
	int i = rand () % 23;
	char *R = malloc (16);
	strcpy(R, raza[i]);
	return R;
}
char *tipoR(){										//Selecciona una cadena de caracteres al azar (Listado en una cadena de cadenas)
	char tipo[7][32]=
        {"Perro",
		"Gato",
		"Tortuga",
		"Hamster",
		"Beagle",
		"Conejo",
		"Pajaro"};
	int i = rand () % 7;
	char *R = malloc (32);
	strcpy(R, tipo[i]);
	return R;
}

// genero aleatorio
char genero(){
	char gender[2]={'H','M'};
	int i = rand() %2;
	return gender[i];
}
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
void crear(char FielFd[32], char* nombres[1729][32]){
        FILE* dogTypeData;
        dogTypeData = fopen(FielFd, "a");

        struct dogType *pet;
        pet = malloc(sizeof(struct dogType));
        if (pet == NULL) perror("Error en la aisgnacion de memoria");

        int al= rand() %1729;
        strcpy(pet->Nombre, nombres[al]);
        strcpy(pet->tipo, tipoR());
        pet->edad= (rand()%21);
        strcpy(pet->raza, razaR());
        pet->estatura= rand() %89;
        pet->peso = (float)(rand()%30);
        pet->sexo= genero();
        pet->siguiente=NULL;

        fseek(dogTypeData, 0, SEEK_END);
        fwrite (pet, sizeof( *pet), 1, dogTypeData);
        imprimir(pet);
        free(pet);
        fclose(dogTypeData);


}
int main()
{
    remove("dogtype.data");
    int hora = time(NULL);
    FILE* dogTypeData2;
    dogTypeData2 = fopen("nombresMascotas.txt", "r");

    char *nombres[1729][32];
    for(int i=0; i<1729; i++){
        fscanf(dogTypeData2, "%s",  nombres[i]);
    }
    fclose(dogTypeData2);

    srand(time(NULL));  //semilla para rand()
    for(int i=0; i<10000000; i++){
         crear("dogtype.data",  nombres);
    }


    return 0;
}

