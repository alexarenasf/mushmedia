#include <stdlib.h> 
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "conio2.h"

/*
La libreria dirent.h se utiliza para el manejo de directorios.
La Libreria conio.h incluye funciones para colorear sectores de la pantalla, ya sea primer o
segundo plano.
textbackground(char) = cambia el color del fondo hasta la siguiente vez que se llame esta 
                        funcion, o hasta el final del programa en caso de que solo se llame
                         una vez. Recibe como entrada una variable constante del tipo char
                          que representa un int en la libreria conio.h.
textcolor(char) = cambia el color del texto hasta la siguiente vez que se llame esta funcion,
                 o hasta el final del programa en caso de que solo se llame una vez. Recibe
                  como entrada una variable constante del tipo char que representa un int
                  en la libreria conio.h.
gotoxy(inx x,int y) = mueve el cursor a la posicion x,y de pa pantalla
window(int,int,int,int)= crea una ventana con los limites ingresados
getch() =recibe una tecla y devueve un int

cprintf = printf compatible con funciones de conio.h
Los colores disponibles son:
Constante     Valor 	Significado       De Fondo o de Texto
BLACK     	    0 	       Negro 	             Ambos
BLUE           	1 	       Azul 	             Ambos
GREEN           2 	       Verde 	             Ambos
CYAN            3 	       Cían 	             Ambos
RED             4 	       Rojo 	             Ambos
MAGENTA         5 	       Magenta 	             Ambos
BROWN           6 	       Marrón 	             Ambos
LIGHTGRAY 	    7 	       Gris Claro 	         Ambos
DARKGRAY       	8 	       Gris Oscuro      Sólo para texto
LIGHTBLUE 	    9 	       Azul Claro 	    Sólo para texto
LIGHTGREEN 	    10 	       Verde Claro 	    Sólo para texto
LIGHTCYAN 	    11 	       Cían Claro 	    Sólo para texto
LIGHTRED 	    12 	       Rojo Claro 	    Sólo para texto
LIGHTMAGENTA 	13 	       Magenta Claro    Sólo para texto
YELLOW 	        14 	       Amarillo 	    Sólo para texto
WHITE 	        15 	       Blanco 	        Sólo para texto
BLINK 	        128 	   Parpadeo 	    Sólo para texto
*/
//COLORES GLOBALES
#define COLOR_TITULO LIGHTRED
#define COLOR_SELECCIONADO LIGHTGRAY
#define COLOR_FONDO BLACK
#define COLOR_TEXTO WHITE
#define COLOR_TEXTO_SELECCIONADO BLACK
#define COLOR_FONDO_EXPLORADOR1 BLUE
#define COLOR_FONDO_EXPLORADOR2 GREEN
//COLORES ICONOS
/*-------MIS VIDEOS--------*/
#define COLOR_CINTA1 MAGENTA
#define COLOR_CINTA2 BLUE
/*-------MIS IMAGENES--------*/
#define COLOR_BORDE WHITE
#define COLOR_PASTO GREEN
#define COLOR_TRONCO BROWN
#define COLOR_ARBOL GREEN
#define COLOR_SOL YELLOW
#define COLOR_CIELO LIGHTBLUE
/*-------MI MUSICA--------*/
#define COLOR_NOTA CYAN
//ENTRADAS DEL TECLADO
/* La funcion getch() recibe como entrada una tecla del teclado y su salida es un numero que
   corresponde a dicha tecla   */
#define ARRIBA 72
#define ABAJO 80
#define DERECHA 77
#define IZQUIERDA 75
#define ENTER 13
#define ESCAPE 27
#define INICIO 71
#define FIN 79
#define TECLA_R 114
#define TECLA_E 101
#define TECLA_O 111
#define INSERTAR 82
#define SUPRIMIR 83
//TIPOS DE MULTIMEDIA
#define IMAGEN 2
#define MUSICA 3
#define VIDEO 4
//CANTIDAD DE REGISTROS
#define TODOS 1
//ESTRUCTURA DE DATOS
typedef struct{
    char nombre[32];
    char ruta[255];
    char album_autor[32];
    char fecha[10];
    int tipo;
} registro;
//FIRMAS DE LAS FUNCIONES
void mostrar_titulo();
void mostrar_menu();
char seleccionar(int,int,char);
char texto_seleccionar(int,int,char);
int contar_registros(int);
void pantalla_completa();
void explorar();
int buscar_unidades();
int contar_archivos(char ruta[]);
int contar_carpetas(char ruta[]);
int reconocer_ext(char ruta[]);
void delete_reg(registro *archer,int select,int total);
void ordenar(registro *reg, int tam);
void ordenar_ventana(registro *reg, int tam,int seccion);
void editar_ventana(registro *reg, int indice,int tam);
void salir();
//VARIABLES GLOBALES
int menu_seleccionado=1;
/* menu_seleccionado indica el elemento del menú que se encuentra selaccionado tal como se
   indica a continuación:
1. Mis Videos   2. Mis Imagenes  3. Mi Música 
4. Agregar Archivo  5. Ver Todos los Archivos  6. Salir  */

int dir_seleccionado; //dir_seleccionado indica cual directorio se encuntra seleccionado
int lista_seleccionado; //lista_seleccionado indica cual elemento de la biblioteca de 
                        //archivos se encuentra seleccionado
int opcion_salir=1; //Se usa para el cuadro de dialogo salir

int i,j,k,teclado; //i,j y k se utilizan para iteraciones definidas (for), en teclado se
                    // guarda el numero de una tecla presionada
//FIN VARIABLES GLOBALES

int main() {
    pantalla_completa();  //cambia a modo pantalla completa
    mostrar_menu();
    return 0;
    }
    
void explorar(){
  
    FILE *leer, *archon;
    
    typedef struct{ 
        char nombre[32];
        char ruta[100];
        int tipo; // 0 = Archivo , 1 = Carpeta
    }directorio_actual; // En esta estructura se guardan los datos de cada elemento del
                        // directorio, llamese elemento a un Archivo o una Carpeta
          
    struct dirent * dirent; //Requerido por las funcion 'opendir' de la libreria 'dirent.h'
    DIR *directorio;  //Requerido por las funcion 'opendir' de la libreria 'dirent.h'
    
    char dir[MAX_PATH]; //indica la ruta del explirador, la constante MAX_PATH es el numero
    char aux_dir[MAX_PATH]; //maximo de caracteres soportados en una rura de un S.O. Windows
    
    strcpy(dir," ");     //se asigna "" como directorio de inicio del explorador
    strcpy(aux_dir,dir);  // se copia el directorio al string auxiliar

    dir_seleccionado=0;//indica que el directorio de la posicion 0 se encuentra seleccionado
    
    int n_aux;            // variable auxialiar
    int ciclo_explorador=1;   // determina un ciclo infinito

    registro archer[255];  // arreglo de los archivos que se agregaran a MUSHMEDIA
    
//Conteo de los elementos del directorio explorado y de las letras de unidad del Equipo en
// el que se ejecuta MUSHMEDIA
    int n_letras=buscar_unidades(); //Esta funcion tambien muetra una barra de progreso
                                    // mintras se ejecuta
    
    archon=fopen("C:\\mainlista.msh","a"); //se abre el archivo que sera la base de datos
    fclose(archon);                        // en modo añadir para crearlo si no existe y
                                           // luego se cierra
    int all=-1; //indica la cantidad de archivos que de agregaran, -1 = ningun archivo

    leer=fopen("expl_tmp.msh","r"); //este archivo es generado por buscar_unidades();
    rewind(leer);                   //y contiene las letras de unidad de este PC 
    char letras[n_letras];  
    for(i=0;i<=n_letras+1;i++)         
        letras[i]=fgetc(leer);
    fclose(leer);
    //En caso de que se cambie de directorio
    while(ciclo_explorador){
   
        int n_archivos=contar_archivos(dir);  //Se cuentan los archivos y carpetas del
        int n_carpetas=contar_carpetas(dir);  // directorio actual
        
        int total=n_letras+n_carpetas+n_archivos-1; //Se resta 1 porque el arreglo parte de 0
    
//Ahora se crea un arreglo del tipo 'directorio_actual'
        directorio_actual elemento[n_letras+n_archivos+n_carpetas]; //se crea un arreglo
        //del tipo directorio_seleccionado, el cual contendra los elementos del directorio
        //que se encuantra abierto
        
//Llenando elemento[]
//Primero se almacenan las letras de unidad, luego las carpetas del directorio actual y por
// ultimo los archivos del directorio actual.

        //Letras de unidad 
        for(i=0;i<n_letras;i++){
           elemento[i].nombre[0]=letras[i]; //Se asigna la letra i al nombre del elemento[i]
           elemento[i].ruta[0]=letras[i]; //Se asigna la letra i a la ruta del elemento[i]
           elemento[i].nombre[1]=':';
           elemento[i].ruta[1]=':';
           elemento[i].nombre[2]='\\';
           elemento[i].ruta[2]='\\';
           for(j=3;j<30;j++){
                elemento[i].nombre[j]='\0';
                elemento[i].ruta[j]='\0';
            }
           elemento[i].tipo=1; //Se define el elemento[i] como 'Carpeta'
        }
        
        //Carpetas del directorio actual
        directorio = opendir(dir);
        i=n_letras;
	    while ((dirent = readdir(directorio)) != NULL) {
            strcat(aux_dir,dirent->d_name);
            if(!chdir(aux_dir)){
	   	       strcpy(elemento[i].nombre,dirent->d_name);
	   	       strcpy(elemento[i].ruta,dirent->d_name);
 	   	       elemento[i].nombre[31]='\0';
	   	       elemento[i].tipo=1;
	   	       i++;
            }
	   	    strcpy(aux_dir,dir);
        }
        closedir(directorio);
        
        //Archivos del directorio actual
        directorio = opendir(dir);
	    while ((dirent = readdir(directorio)) != NULL) {
            strcat(aux_dir,dirent->d_name);
            if(chdir(aux_dir)){
	   	       strcpy(elemento[i].nombre,dirent->d_name);
	   	       strcpy(elemento[i].ruta,dirent->d_name);
 	   	       elemento[i].nombre[31]='\0';
	   	       elemento[i].tipo=0;
	   	       i++;
            }
	   	    strcpy(aux_dir,dir);
        }
        closedir(directorio);
               
//Mostrando por pantalla el arreglo elemento[]
        int ciclo_dir=1; //determina un segundo ciclo infinito
        
        int primer_elemento=0;
        int ultimo_elemento=29; //Se muestran como maximo 30 elementos del arreglo
        
        int first_element=0;
        int last_element=29;    //se muestran 30 archivos por agregar a la vez
        
        while(ciclo_dir){ //este ciclo se utiliza para el dezplazamiento del rectangulo que indica cual elemento esta seleccionado
            mostrar_titulo();
                      
            printf("Explorador de Archivos\n");
            printf("\nDireccion: %s\n",dir);
            
            
            window(1,13,78,44);
//Directorio Actual
            textbackground(COLOR_FONDO_EXPLORADOR1);
            cprintf("%c%c%c%c%c%c%c%c%c%c%cDirectorio Actual%c%c%c%c%c%c%c%c%c%c",
            201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
            k=1;
            for(i=1;i<30;i++){
                gotoxy(1,k+1);
                cprintf("%c                                    %c",186,186);
                k++;
            }
            gotoxy(1,31);
            cprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
            200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,
            205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
            
            if(ultimo_elemento>29){
                gotoxy(38,15);
                cprintf("%c",24);
                }
            if(primer_elemento<total-28){
                gotoxy(38,43);
                cprintf("%c",25);
                }

            j=1;
            if(total+1<ultimo_elemento)
                ultimo_elemento=total+1; //Si hay menos de 30 elementos se muestran 'total+1' elementos
            
            for(i=primer_elemento;i<ultimo_elemento;i++){
                gotoxy(2,j+1);
                textbackground(seleccionar(dir_seleccionado,i,COLOR_FONDO_EXPLORADOR1));                                                                          
                textcolor(texto_seleccionar(dir_seleccionado,i,COLOR_TEXTO));
                if(elemento[i].tipo){
                    printf("[%s]",elemento[i].nombre);
                    }
                else{
                    printf("%c%s ",16,elemento[i].nombre);
                    }
                for(k=0;k<(34-strlen(elemento[i].nombre));k++)
                    printf(" ");
                printf("\n");
                j++;
                textcolor(COLOR_TEXTO);
                textbackground(COLOR_FONDO_EXPLORADOR1);
                }
            textbackground(COLOR_FONDO);
            
//Archivos por Agregar
            gotoxy(41,1);
            textbackground(COLOR_FONDO_EXPLORADOR2);
            cprintf("%c%c%c%c%c%c%c%c%cArchivos por Agregar%c%c%c%c%c%c%c%c%c",
            201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
            k=1;
            for(i=1;i<30;i++){
                gotoxy(41,1+k);
                cprintf("%c                                    %c",186,186);
                k++;
            }
            gotoxy(41,31);                
            cprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,
            205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
            
            if(last_element>29){
                gotoxy(38,2);
                cprintf("%c",24);
                }
            if(first_element<all-28){
                gotoxy(38,30);
                cprintf("%c",25);
                }
                                  
            j=1;
            if(all+1<last_element)
                last_element=all+1; //Si hay menos de 30 elementos se muestran 'all+1' elementos
            
            
            for(i=first_element;i<last_element;i++){
                gotoxy(42,j+1);
                j++;
                textbackground(seleccionar(dir_seleccionado,-i-1,COLOR_FONDO_EXPLORADOR2));                                                                          
                textcolor(texto_seleccionar(dir_seleccionado,-i-1,COLOR_TEXTO));
                    switch(archer[i].tipo){
                        case 2:
                            cprintf("%c",4);
                            break;
                        case 3:
                            cprintf("%c",14);
                            break;
                        case 4:
                            cprintf("%c",15);
                            break;
                        }                    
                    cprintf("%s ",archer[i].nombre);
                for(k=0;k<(34-strlen(archer[i].nombre));k++)
                    cprintf(" ");
                textcolor(COLOR_TEXTO);
                textbackground(COLOR_FONDO_EXPLORADOR2);
                }
                
            textbackground(COLOR_FONDO);
            
//Ayuda para teclas
            window(1,45,78,52);
            cprintf("%c/%c : Desplazamiento",24,25);
            gotoxy(1,2);
            cprintf("%c/%c : Cambio De Lista",26,27);
            gotoxy(1,3);
            cprintf("Inicio : Ir al principio");
            gotoxy(1,4);
            cprintf("Fin : Ir al final");
            gotoxy(30,1);
            cprintf("Insert : Agregar Archivos");
            gotoxy(30,2);
            cprintf("Esc : Volver al Menu Principal");
            gotoxy(30,3);
            cprintf("Enter : ");
            if(dir_seleccionado>=0 && dir_seleccionado<n_letras+n_carpetas)
                cprintf("Abrir Carpeta");
            else if(dir_seleccionado>=0 && dir_seleccionado>=n_letras+n_carpetas)
                cprintf("Agregar a la lista");
            gotoxy(30,4);
            if(dir_seleccionado<0)
                cprintf("Supr : Eliminar de la lista");            
//Hasta aqui lo que se muestra por pantalla

//Acciones del teclado            
            teclado=getch();
            
            if(dir_seleccionado>=0){//menu izquierda
            if(dir_seleccionado==0){
                  switch (teclado) {
                     case ABAJO:
                         ++dir_seleccionado;
                         break;
                     }              
            }
            else if(dir_seleccionado==total){
                  switch (teclado) {
                     case ARRIBA:
                         --dir_seleccionado;
                         break;
                     }
            }
            else if(dir_seleccionado==primer_elemento){
                 switch (teclado) {
                  case ARRIBA:
                         --dir_seleccionado;
                         --primer_elemento;
                         --ultimo_elemento;
                         break;
                  case ABAJO:
                         ++dir_seleccionado;
                         break;
                  }
            }
            else if(dir_seleccionado==ultimo_elemento-1){
                  switch (teclado) {
                     case ABAJO:
                         ++dir_seleccionado;
                         ++ultimo_elemento;
                         ++primer_elemento;
                         break;
                     case ARRIBA:
                         --dir_seleccionado;
                         break;
                     }
            }
            else {
                  switch (teclado) {
                    case ARRIBA:
                         --dir_seleccionado;
                         break;
                    case ABAJO:
                         ++dir_seleccionado;
                         break;
                 }
            }
            
            }//end menu izquierda
            
            if(dir_seleccionado<0){//menu der
            if(dir_seleccionado==-1){
                  switch (teclado) {
                     case ABAJO:
                         --dir_seleccionado;
                         break;
                     }              
            }
            else if(dir_seleccionado==-all-1){
                  switch (teclado) {
                     case ARRIBA:
                         ++dir_seleccionado;
                         break;
                     }
            }
            else if(dir_seleccionado==-first_element-1){
                 switch (teclado) {
                  case ARRIBA:
                         ++dir_seleccionado;
                         --first_element;
                         --last_element;
                         break;
                  case ABAJO:
                         --dir_seleccionado;
                         break;
                  }
            }
            else if(dir_seleccionado==-last_element){
                  switch (teclado) {
                     case ABAJO:
                         --dir_seleccionado;
                         ++last_element;
                         ++first_element;
                         break;
                     case ARRIBA:
                         ++dir_seleccionado;
                         break;
                     }
            }
            else {
                  switch (teclado) {
                    case ARRIBA:
                         ++dir_seleccionado;
                         break;
                    case ABAJO:
                         --dir_seleccionado;
                         break;
                 }
            }
            
            }//end menu derecha

        //teclas globales
          switch (teclado) {
             case ENTER:
                  if(strlen(dir)<=3){
                        if(0<=dir_seleccionado && dir_seleccionado<n_letras){
                            strcpy(dir,elemento[dir_seleccionado].ruta);
                            ciclo_dir=0;
                            dir_seleccionado=0;
                        }
                        else if(n_letras<=dir_seleccionado && 
                        dir_seleccionado<n_carpetas+n_letras){
                            strcat(dir,elemento[dir_seleccionado].ruta);
                            strcat(dir,"\\");
                            ciclo_dir=0;
                            dir_seleccionado=0;
                        }
                        else if(n_carpetas+n_letras<=dir_seleccionado){
                            if(reconocer_ext(elemento[dir_seleccionado].ruta)!=9 && 
                            reconocer_ext(elemento[dir_seleccionado].ruta)!=0){
                                strcpy(archer[all+1].nombre,
                                elemento[dir_seleccionado].nombre);
                                strcpy(archer[all+1].ruta,"\42");
                                strcat(archer[all+1].ruta,dir);
                                strcat(archer[all+1].ruta,elemento[dir_seleccionado].ruta);
                                strcat(archer[all+1].ruta,"\42");
                                archer[all+1].tipo=reconocer_ext(elemento[dir_seleccionado].
                                ruta);
                                all++;
                                if(last_element<29)
                                    last_element++;
                            }
                        }
                  }
                  else {
                        if(0<=dir_seleccionado && +dir_seleccionado<n_letras){
                            strcpy(dir,elemento[dir_seleccionado].ruta);
                            ciclo_dir=0;
                            dir_seleccionado=0;
                        }
                        else if(dir_seleccionado==n_letras){
                            //Expolrar la misma Directorio
                        }
                        else if(dir_seleccionado==n_letras+1){
                            //Subir al directorio superior
                            for(i=0;i<=strlen(dir)-2;i++){
                                if(dir[i]=='\\'){
                                    n_aux=i;
                                }
                            }
                            for(i=n_aux+1;i<=strlen(dir);i++){
                                dir[i]='\0';
                            }
                        dir_seleccionado=0;
                        ciclo_dir=0;
                        }
                        else if(n_letras+2<=dir_seleccionado && 
                        dir_seleccionado<n_carpetas+n_letras){
                            strcat(dir,elemento[dir_seleccionado].ruta);
                            strcat(dir,"\\");
                            ciclo_dir=0;
                            dir_seleccionado=0;
                        }
                        else if(n_carpetas+n_letras<=dir_seleccionado){
                            if(reconocer_ext(elemento[dir_seleccionado].ruta)!=9 && 
                            reconocer_ext(elemento[dir_seleccionado].ruta)!=0){
                                strcpy(archer[all+1].nombre,
                                elemento[dir_seleccionado].nombre);
                                strcpy(archer[all+1].ruta,"\42");
                                strcat(archer[all+1].ruta,dir);
                                strcat(archer[all+1].ruta,elemento[dir_seleccionado].ruta);
                                strcat(archer[all+1].ruta,"\42");
                                archer[all+1].tipo=reconocer_ext(elemento[dir_seleccionado].
                                ruta);
                                all++;
                                if(last_element<29)
                                    last_element++;
                            }
                        }
                    }                        
                  break;
             case ESCAPE:
                  ciclo_dir=0;
                  ciclo_explorador=0;
                  break;
             case SUPRIMIR:
			 	  if(dir_seleccionado<0){
			         if(all!=-1){
				  		delete_reg(archer,-dir_seleccionado-1,255);
							if(dir_seleccionado==-all-1){
								dir_seleccionado++;
							}						  						
				  			all--;								   
				    }
				}
				  break;
             case INICIO:
                  if(dir_seleccionado>=0){
                  dir_seleccionado=0;
                  primer_elemento=0;
                  ultimo_elemento=29;
                  if(total+1<ultimo_elemento)
                      ultimo_elemento=total+1;
                  }
                  if(dir_seleccionado<0){
                  dir_seleccionado=-1;
                  first_element=0;
                  last_element=29;
                  if(all+1<last_element)
                      last_element=all+1;                                                          
                                         
                                         }
                  break;
             case FIN:
                  if(dir_seleccionado>=0){
                  dir_seleccionado=total;
                  primer_elemento=0;
                  ultimo_elemento=total+1;
                  if(total+1>29)  
                      primer_elemento=total-28;
                  }
                  if(dir_seleccionado<0){
                  dir_seleccionado=-all-1;
                  first_element=0;
                  last_element=all+1;
                  if(all+1>29)  
                      first_element=all-28;
                      }
                  break;
             case DERECHA:
                  if(all!=-1){
                    dir_seleccionado=-1;
                    first_element=0;
                    last_element=29;
                  }         
                  break;
             case IZQUIERDA:
                  primer_elemento=0;
                  ultimo_elemento=29;
                  dir_seleccionado=0;
                  break;
             case INSERTAR:
                  archon=fopen("C:\\mainlista.msh","ab");  
                  for(i=0;i<=all;i++){
                      fwrite(&archer[i],sizeof(registro),1,archon);
                  }
                  fclose(archon);
                  all=-1;                 
                  break;
             } 
 
        system("cls");
        }//while(ciclo_dir)
    }//while(ciclo_explorador)
}//explorar();

int buscar_unidades(){

/* Esta funcion revisa las letras de la C a la Z intentando entrar al directorio dado por
 una letra mas dos puntos mediante la funcion system */

    char comando[3]=" :", unidades_activas[23]="";
    int i, j=0;    
    FILE *archivo;
    
    archivo=fopen("expl_tmp.msh","w");

    for(i=67;i<=90;i++){
         textcolor(YELLOW);
         printf("\n\n\n\n\n\n\n\n\n\n\n                    Explorando la configuracion de este PC...\n\n                 ");
         textcolor(GREEN);
         for(k=0;k<i/2;k++)
            printf("%c",220);
         
         comando[0]=i;
         

         textcolor(COLOR_FONDO);
         printf("\n");
         if(!(system(comando))){
              unidades_activas[j]=i;
              j++;
              }
         system("cls");
         
         }
    textcolor(COLOR_TEXTO);
    fputs(unidades_activas,archivo);         
    fclose(archivo);
    return strlen(unidades_activas);
    }
    
    
int contar_archivos(char d[1000]) {  
	 struct dirent * dirent = NULL;
	 DIR *directorio = NULL;
	 int n_archivos=0;
	 char aux[1000];

	 strcpy(aux,d);
	 
   	 directorio = opendir(d);     
     while ((dirent = readdir(directorio)) != NULL) {
        strcat(aux,dirent->d_name);
         if(chdir(aux))     // la funcion chdir devuelve 0 si pudo cambiar de directorio de 
             n_archivos++;  // manera exitosa y 1 si tuvo un error por esto sirve para 
        strcpy(aux,d);      //encontrar archivos
     }
     closedir(directorio);

     return n_archivos;
}

int contar_carpetas(char d[1000]) {  
	 struct dirent * dirent = NULL;
	 DIR *directorio = NULL;
	 int n_carpetas=0;
	 char aux[1000];
	 
	 strcpy(aux,d);
	 
   	 directorio = opendir(d);     
     while ((dirent = readdir(directorio)) != NULL) {
        strcat(aux,dirent->d_name);
        if(!chdir(aux))     // la funcion chdir devuelve 0 si pudo cambiar de directorio de 
             n_carpetas++;  // manera exitosa y 1 si tuvo un error, la negacion sirve para 
        strcpy(aux,d);      // encontrar carpetas
     }
     closedir(directorio);

     return n_carpetas;
}

//Tipos de Archivo: 0 = desconocido, 2 = jpg,png; 3=mp3,wma,ogg,acc; 4=avi,mpg,wmv; 
//                  9 = error
int reconocer_ext(char ruta[100]){ 
    char rec[100];
    int i,j=0;
    for(i=strlen(ruta)-1;ruta[i]!='.' && i>=0 ;i--){ 
         rec[j]=ruta[i];
         j++;
    }
    rec[j]='\0';
    strlwr(rec);
    strrev(rec);
    if(strcmp(rec,ruta)==0)
        return 9;
    if(strcmp(rec,"jpg")==0 || strcmp(rec,"png")==0)
        return 2;
    if(strcmp(rec,"mp3")==0 || strcmp(rec,"wma")==0 
    || strcmp(rec,"ogg")==0 || strcmp(rec,"acc")==0)
        return 3;
    if(strcmp(rec,"avi")==0 || strcmp(rec,"mpg")==0 
    || strcmp(rec,"wmv")==0)
        return 4;  
    return 0;
}

void lista(int seccion){
// Esta funcion muestra una lista de los archivos que hay en la base de datos
// Recibe como parametro de entrada la seccion (Mis Videos, Imagenes, Musica, etc)
    FILE *base_de_datos;
    
    base_de_datos=fopen("C:\\mainlista.msh","rb");
    
    if(base_de_datos==NULL){  //Si no se puede crear, se termina el programa
        base_de_datos=fopen("C:\\mainlista.msh","wb");
        if(base_de_datos==NULL){
            printf("Error con la Base de datos...");
            getch();
            exit(1);
        }
    }    
    
    int n_elementos[4]; // Este arreglo almacena la cantidad de registros de la base de
    for(i=1;i<=4;i++)   // datos correspondiente a cada tipo de seccion
        n_elementos[i]=0;
      
    registro tmp; // se crea una variable temporal del tipo registro para contar
    
    while(fread(&tmp,sizeof(tmp),1,base_de_datos)){ //se cuenta y se almacenan los
        if(tmp.tipo==MUSICA)                        // resultados correspondientes
            n_elementos[MUSICA]++;                  // a cada tipo de seccion
        else if(tmp.tipo==IMAGEN)                   // segun las constantes definidas
            n_elementos[IMAGEN]++;                  // al principio
        else if(tmp.tipo==VIDEO)
            n_elementos[VIDEO]++;
        }
    
    rewind(base_de_datos);
    
    n_elementos[TODOS]=n_elementos[VIDEO]+n_elementos[MUSICA]+n_elementos[IMAGEN];
    
    registro archivo[n_elementos[TODOS]];  
    //se crea un arreglo del tipo registro para almacenar los registros de la base de datos
        
    if(!fread(&tmp,sizeof(tmp),1,base_de_datos))
        printf("No Hay Archivos\n"); // si la base de datos esta vacia se muestra esto
    else{   // en caso contrario se comienza con el llenado
        i=0;
        rewind(base_de_datos);
        while(fread(&tmp,sizeof(registro),1,base_de_datos)){
                strcpy(archivo[i].nombre,tmp.nombre);
                strcpy(archivo[i].ruta,tmp.ruta);
                strcpy(archivo[i].album_autor,tmp.album_autor);
                strcpy(archivo[i].fecha,tmp.fecha);
                archivo[i].tipo=tmp.tipo;
                i++;
        }
    }
    
    fclose(base_de_datos);
    
    ordenar(archivo,n_elementos[TODOS]); // se ordenan los registros por nombre
    
    int ciclo_lista=1; //determina un ciclo infinito
    
    int primer_elemento=0;
    int ultimo_elemento=30;// se muestran 31  elementos a la vez en la pantalla
    
    int ultimo_elemento_tipo, primer_elemento_tipo; // indica el primer y el ultimo elemento
    int pultimo_elemento;                           // correpondiente a la seccion
    int contador_aux=0;
    int aux;
    
    if(seccion==TODOS){ // si la seccion es TODOS se selecciona el elemento 0
        lista_seleccionado=0;
        
        if(n_elementos[TODOS]+1<ultimo_elemento)  //si hay menros de 31 registros
            ultimo_elemento=n_elementos[TODOS];  // se define el ultimo elemento
                                                  // que se muestra como el total +1
    }else { //si la seccion no es TODOS, se llenan las variables correspondientes a seccion
        for(i=0;i<n_elementos[TODOS];i++)
            if(archivo[i].tipo==seccion){
                lista_seleccionado=i;
                primer_elemento_tipo=i;
                break;
            }
        for(i=n_elementos[TODOS]-1;i>=0;i--)
            if(archivo[i].tipo==seccion){
                ultimo_elemento_tipo=i;
                break;
            }
        if(n_elementos[seccion]<ultimo_elemento)
            ultimo_elemento=ultimo_elemento_tipo+1;
        else{
            
            primer_elemento=primer_elemento_tipo;
            
            for(i=primer_elemento_tipo;i<=ultimo_elemento_tipo;i++){
                if(archivo[i].tipo==seccion)
                    contador_aux++;
                if(contador_aux==31){
                    ultimo_elemento=i;
                    for(j=i-1;i>=0;i--){
                        if(archivo[i].tipo==seccion){
                            pultimo_elemento=i;
                        break;
                        }
                    }

                    break;
                }
                    
            }
        }
    }

          
    while(ciclo_lista){
        
        system("cls");

        mostrar_titulo();
        printf("\n");
    //ENCABEZADO
        switch(seccion){
            case VIDEO:
                printf("%c MIS VIDEOS",15);
                printf("\n");        
                
                for(i=1;i<=80;i++)
                    printf("%c",205);
                printf("Nombre                             Album                            Fecha");
                break;
            case IMAGEN:
                printf("%c MIS IMAGENES",4);
                printf("\n");        
                for(i=1;i<=80;i++)
                    printf("%c",205);
                printf("Nombre                             Album                            Fecha");
                break;
            case MUSICA:
                printf("%c MI MUSICA",14);
                printf("\n");        
                for(i=1;i<=80;i++)
                    printf("%c",205);
                printf("Nombre                             Autor                            Fecha");
                break;
            default:
                printf("%c MIS ARCHIVOS MULTIMEDIA",2);
                printf("\n");        
                for(i=1;i<=80;i++)
                    printf("%c",205);
                printf("Nombre                             Tipo                             Fecha");
                break;
            }

        printf("\n"); 
        
        for(i=1;i<=80;i++)
            printf("%c",205);
       //LISTADO DE REGISTROS DE LA BASE DE DATOS         
        if(n_elementos[TODOS]==0)
            printf("No Hay Archivos");
        else{
            for(i=primer_elemento;i<ultimo_elemento;i++){
                if(seccion==TODOS){
                    textcolor(texto_seleccionar(lista_seleccionado,i,COLOR_TEXTO));
                    textbackground(seleccionar(lista_seleccionado,i,COLOR_FONDO));
                    switch(archivo[i].tipo){
                        case IMAGEN:
                            printf("%c",4);
                            break;
                        case MUSICA:
                            printf("%c",14);
                            break;
                        case VIDEO:
                            printf("%c",15);
                            break;
                        }   
                    printf(" %s",archivo[i].nombre);
                    
                    for(j=0;j<33-strlen(archivo[i].nombre);j++)
                        printf(" ");
                        
                    switch(archivo[i].tipo){
                        case IMAGEN:
                            printf("Archivo de Imagen");
                            break;
                        case MUSICA:
                            printf("Archivo de Musica");
                            break;
                        case VIDEO:
                            printf("Archivo de Video ");
                            break;
                        }

                    for(j=0;j<15;j++)
                        printf(" ");

                    if(strcmp(archivo[i].fecha,"\0")==0)
                        printf("Desconocida");
                    else
                        printf("%s",archivo[i].fecha);
                    printf("\n");
                }
                else{
                    if(seccion==archivo[i].tipo){
                        textcolor(texto_seleccionar(lista_seleccionado,i,COLOR_TEXTO));
                        textbackground(seleccionar(lista_seleccionado,i,COLOR_FONDO)); 
                        switch(archivo[i].tipo){
                            case IMAGEN:
                                printf("%c",4);
                                break;
                            case MUSICA:
                                printf("%c",14);
                                break;
                            case VIDEO:
                                printf("%c",15);
                                break;
                        }   
                        printf(" %s",archivo[i].nombre);
                    
                        for(j=0;j<33-strlen(archivo[i].nombre);j++)
                            printf(" ");
                        
                        printf("%s",archivo[i].album_autor);
                    
                        for(j=0;j<33-strlen(archivo[i].album_autor);j++)
                            printf(" ");

                        if(strcmp(archivo[i].fecha,"\0")==0)
                            printf("Desconocida");
                        else
                            printf("%s",archivo[i].fecha);
                        printf("\n");
                    }
                }           
            }
        }
                
        textcolor(COLOR_TEXTO);
        textbackground(COLOR_FONDO);
        //PIE DE LA LISTA
        window(1,44,80,55);
        for(i=1;i<=80;i++)
            printf("%c",205);
        
        printf("%d Archivo(s)\n",n_elementos[seccion]);
          
        for(i=1;i<=80;i++)
            printf("%c",205); 



//Ayuda para teclas
                gotoxy(1,4);
                cprintf("%c/%c : Desplazamiento",24,25);
                gotoxy(1,5);
                cprintf("Inicio : Ir al principio");
                gotoxy(1,6);
                cprintf("Fin : Ir al final");

                gotoxy(26,4);
                cprintf("E : Editar Seleccionado");
                gotoxy(26,5);
                cprintf("O : Ordenar por...");
        
                gotoxy(50,4);
                cprintf("Enter : Reproducir Seleccionado");        
                gotoxy(50,5);   
                cprintf("Supr : Borrar Seleccionado");
                gotoxy(50,6);
                cprintf("Esc : Ir al Menu Principal");


//Acciones del Teclado

            teclado=getch();
            
            if(seccion==TODOS){
                if(lista_seleccionado==0){
                    switch (teclado) {
                        case ABAJO:
                            ++lista_seleccionado;
                            break;
                        }              
                }
                else if(lista_seleccionado==n_elementos[TODOS]-1){
                    switch (teclado) {
                        case ARRIBA:
                            --lista_seleccionado;
                            break;
                        }
                }
                else if(lista_seleccionado==primer_elemento){
                    switch (teclado) {
                    case ARRIBA:
                            --lista_seleccionado;
                            --primer_elemento;
                            --ultimo_elemento;
                            break;
                    case ABAJO:
                            ++lista_seleccionado;
                            break;
                    }
                }
                else if(lista_seleccionado==ultimo_elemento-1){
                    switch (teclado) {
                        case ABAJO:
                            ++lista_seleccionado;
                            ++ultimo_elemento;
                            ++primer_elemento;
                            break;
                        case ARRIBA:
                            --lista_seleccionado;
                            break;
                        }
            }
            else {
                  switch (teclado) {
                    case ARRIBA:
                         --lista_seleccionado;
                         break;
                    case ABAJO:
                         ++lista_seleccionado;
                         break;
                 }
            }
        }
        else{
            
            
            
                if(lista_seleccionado==primer_elemento_tipo){
                    switch (teclado) {
                        case ABAJO:
                            for(i=lista_seleccionado+1;i<=ultimo_elemento;i++){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                            break;
                        }              
                }
                else if(lista_seleccionado==ultimo_elemento_tipo){
                    switch (teclado) {
                        case ARRIBA:
                            for(i=lista_seleccionado-1;i>=primer_elemento;i--){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                            break;
                        }
                }
                else if(lista_seleccionado==primer_elemento){
                    switch (teclado) {
                    case ARRIBA:
                            aux=lista_seleccionado;
                            for(i=lista_seleccionado-1;i>=primer_elemento_tipo;i--){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    primer_elemento-=aux-i;
                                    break;
                                }
                            }
                            for(i=ultimo_elemento-1;i>=primer_elemento_tipo;i--){
                                if(archivo[i].tipo==seccion){
                                    ultimo_elemento=i;
                                    break;
                                }
                            }
                            break;
                    case ABAJO:
                            for(i=lista_seleccionado+1;i<=ultimo_elemento;i++){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                            break;
                    }
                }
                else if(lista_seleccionado==pultimo_elemento){
                    switch (teclado) {
                        case ABAJO:
                            aux=lista_seleccionado;
                            for(i=lista_seleccionado+1;i<=ultimo_elemento_tipo;i++){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    ultimo_elemento+=i-aux;
                                    break;
                                }
                            }
                            for(i=primer_elemento+1;i<=ultimo_elemento_tipo;i++){
                                if(archivo[i].tipo==seccion){
                                    primer_elemento=i;
                                    break;
                                }
                            }
                            break;
                        case ARRIBA:
                            for(i=lista_seleccionado-1;i>=primer_elemento;i--){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                            break;
                        }
            }
            else {
                  switch (teclado) {
                    case ARRIBA:
                            for(i=lista_seleccionado-1;i>=primer_elemento_tipo;i--){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                         break;
                    case ABAJO:
                            for(i=lista_seleccionado+1;i<=ultimo_elemento_tipo;i++){
                                if(archivo[i].tipo==seccion){
                                    lista_seleccionado=i;
                                    break;
                                }
                            }
                         break;
                 }
            }
        }
      
                
            switch(teclado){
                case ESCAPE:
                    ciclo_lista=0;
                    break;
                case INICIO:
                    break;
                case FIN:
                    break;
                }
                
            if(n_elementos[TODOS]!=0)    
            switch(teclado){
                case TECLA_E:
                    editar_ventana(archivo,lista_seleccionado,n_elementos[TODOS]);
                    break;
                case TECLA_O:
                    ordenar_ventana(archivo,n_elementos[TODOS],seccion);
                    break;
                case SUPRIMIR:
                    delete_reg(archivo,lista_seleccionado,n_elementos[TODOS]);
                    break;
                case ENTER:
                    system(archivo[lista_seleccionado].ruta);
                    break;
                }
            
        }//while(ciclo_lista)
    }//lista(int);


int contar_registros(int categoria){
//cuenta el numero de registros segun el parametro categoria
    FILE *a;
    int cont_musica=0,cont_imagen=0,cont_video=0;
    int total;
    registro tmp;
    if((a=fopen("C:\\mainlista.msh","rb"))==NULL){
        return 0;
    }else{
        a=fopen("C:\\mainlista.msh","rb");
        while(fread(&tmp,sizeof(tmp),1,a)){
            if(tmp.tipo==MUSICA)
                cont_musica++;
            else if(tmp.tipo==IMAGEN)
                cont_imagen++;
            else if(tmp.tipo==VIDEO)
                cont_video++;
        }
    fclose(a);
    total=cont_musica+cont_imagen+cont_video;
    }
    switch(categoria){
        case TODOS:
            return total;
            break;
        case MUSICA:
            return cont_musica;
            break;
        case IMAGEN:
            return cont_imagen;
            break;
        case VIDEO:
            return cont_video;
            break;
        }
    return 0;
}

void ordenar(registro *reg, int tam){
   //ordena regitros por el campo "nombre"   
    int i,j;
    char aux_nombre1[32];
    char aux_nombre2[32];
    char auxn[32];
    char auxr[255];
    char auxa[32];
    char auxf[10];
    int auxy;
    
    for(j=0;j<tam-1;j++){
    for(i=0;i<tam-1;i++){
        strcpy(aux_nombre1,reg[i].nombre);
        strcpy(aux_nombre2,reg[i+1].nombre);
        strlwr(aux_nombre1);
        strlwr(aux_nombre2);
        if(strcmp(aux_nombre1,aux_nombre2)==1){
                                               
            strcpy(auxn,reg[i].nombre);
            strcpy(auxr,reg[i].ruta);
            strcpy(auxa,reg[i].album_autor);            
            strcpy(auxf,reg[i].fecha);
          	auxy=reg[i].tipo;            
        
            strcpy(reg[i].nombre,reg[i+1].nombre);
            strcpy(reg[i].ruta,reg[i+1].ruta);
            reg[i].tipo=reg[i+1].tipo;
            strcpy(reg[i].album_autor,reg[i+1].album_autor);            
            strcpy(reg[i].fecha,reg[i+1].fecha);                        
            
            strcpy(reg[i+1].nombre,auxn);
            strcpy(reg[i+1].ruta,auxr);
            strcpy(reg[i+1].album_autor,auxa);            
            strcpy(reg[i+1].fecha,auxf);
          	reg[i+1].tipo=auxy;            
                                          
        }

    }
    }

}//ordenar()

void ordenar_ventana(registro *reg, int tam,int seccion){
//    ordena  regitros segun eleccion del usiario
    int i,j;
    char aux_nombre1[32];
    char aux_nombre2[32];
    char auxn[32];
    char auxr[255];
    char auxa[32];
    char auxf[10];
    int auxy;
    
    int opcion=0;
    int ciclo=1;
          
     while(ciclo){
        window(30,19,50,29);
        textbackground(RED);
        cprintf("%c",201);
        for(i=0;i<19;i++)
            cprintf("%c",205);
        cprintf("%c",187);
        for(i=1;i<5;i++)
            cprintf("%c                   %c",186,186);
        cprintf("%c",200);
        for(i=0;i<19;i++)
            cprintf("%c",205);
        cprintf("%c",188);
                
        gotoxy(2,2);
        cprintf("Ordenar por...");
        gotoxy(2,3);
        textbackground(seleccionar(opcion,0,RED));
        textcolor(texto_seleccionar(opcion,0,COLOR_TEXTO));
        cprintf(" Nombre ");
        gotoxy(2,4);
        textbackground(seleccionar(opcion,1,RED));
        textcolor(texto_seleccionar(opcion,1,COLOR_TEXTO));
        if(seccion==IMAGEN || seccion==VIDEO)
            cprintf(" Album  ");
        if(seccion==MUSICA)
            cprintf(" Autor  ");
        if(seccion==TODOS)    
            cprintf(" Tipo  ");
        gotoxy(2,5);
        textbackground(seleccionar(opcion,2,RED));
        textcolor(texto_seleccionar(opcion,2,COLOR_TEXTO));
        cprintf(" Fecha  ");
        textbackground(COLOR_FONDO);
        textcolor(COLOR_TEXTO);
        
        teclado=getch();
        
        switch(opcion){
            case 0:
                switch(teclado){
                    case ABAJO:
                        opcion=1;
                        break;
                    case ENTER:
                        for(j=0;j<tam-1;j++){
                        for(i=0;i<tam-1;i++){
                            strcpy(aux_nombre1,reg[i].nombre);
                            strcpy(aux_nombre2,reg[i+1].nombre);
                            strlwr(aux_nombre1);
                            strlwr(aux_nombre2);
                            if(strcmp(aux_nombre1,aux_nombre2)==1){
                                               
                                strcpy(auxn,reg[i].nombre);
                                strcpy(auxr,reg[i].ruta);
                                strcpy(auxa,reg[i].album_autor);            
                                strcpy(auxf,reg[i].fecha);
          	                    auxy=reg[i].tipo;            
        
                                strcpy(reg[i].nombre,reg[i+1].nombre);
                                strcpy(reg[i].ruta,reg[i+1].ruta);
                                reg[i].tipo=reg[i+1].tipo;
                                strcpy(reg[i].album_autor,reg[i+1].album_autor);            
                                strcpy(reg[i].fecha,reg[i+1].fecha);                        
            
                                strcpy(reg[i+1].nombre,auxn);
                                strcpy(reg[i+1].ruta,auxr);
                                strcpy(reg[i+1].album_autor,auxa);            
                                strcpy(reg[i+1].fecha,auxf);
          	                    reg[i+1].tipo=auxy;            
                                          
                            }

                        }
                        }
                        ciclo=0;
                        break;
                    }
                break;
            case 1:
                switch(teclado){
                    case ARRIBA:
                        opcion=0;
                        break;
                    case ABAJO:
                        opcion=2;
                        break;
                    case ENTER:
                        for(j=0;j<tam-1;j++){
                        for(i=0;i<tam-1;i++){
                            if(seccion==TODOS){
                                aux_nombre1[0]=reg[i].tipo;
                                aux_nombre2[0]=reg[i+1].tipo;
                            }
                            else{
                                strcpy(aux_nombre1,reg[i].album_autor);
                                strcpy(aux_nombre2,reg[i+1].album_autor);
                            }
                            strlwr(aux_nombre1);
                            strlwr(aux_nombre2);
                            if(strcmp(aux_nombre1,aux_nombre2)==1){
                                               
                                strcpy(auxn,reg[i].nombre);
                                strcpy(auxr,reg[i].ruta);
                                strcpy(auxa,reg[i].album_autor);            
                                strcpy(auxf,reg[i].fecha);
          	                    auxy=reg[i].tipo;            
        
                                strcpy(reg[i].nombre,reg[i+1].nombre);
                                strcpy(reg[i].ruta,reg[i+1].ruta);
                                reg[i].tipo=reg[i+1].tipo;
                                strcpy(reg[i].album_autor,reg[i+1].album_autor);            
                                strcpy(reg[i].fecha,reg[i+1].fecha);                        
            
                                strcpy(reg[i+1].nombre,auxn);
                                strcpy(reg[i+1].ruta,auxr);
                                strcpy(reg[i+1].album_autor,auxa);            
                                strcpy(reg[i+1].fecha,auxf);
          	                    reg[i+1].tipo=auxy;            
                                          
                            }

                        }
                        }

                        ciclo=0;
                        break;
                    }
                break;
            case 2:
                switch(teclado){
                    case ARRIBA:
                        opcion=1;
                        break;
                    case ENTER:
                        for(j=0;j<tam-1;j++){
                        for(i=0;i<tam-1;i++){
                            strcpy(aux_nombre1,reg[i].fecha);
                            strcpy(aux_nombre2,reg[i+1].fecha);
                            strlwr(aux_nombre1);
                            strlwr(aux_nombre2);
                            if(strcmp(aux_nombre1,aux_nombre2)==1){
                                               
                                strcpy(auxn,reg[i].nombre);
                                strcpy(auxr,reg[i].ruta);
                                strcpy(auxa,reg[i].album_autor);            
                                strcpy(auxf,reg[i].fecha);
          	                    auxy=reg[i].tipo;            
        
                                strcpy(reg[i].nombre,reg[i+1].nombre);
                                strcpy(reg[i].ruta,reg[i+1].ruta);
                                reg[i].tipo=reg[i+1].tipo;
                                strcpy(reg[i].album_autor,reg[i+1].album_autor);            
                                strcpy(reg[i].fecha,reg[i+1].fecha);                        
            
                                strcpy(reg[i+1].nombre,auxn);
                                strcpy(reg[i+1].ruta,auxr);
                                strcpy(reg[i+1].album_autor,auxa);            
                                strcpy(reg[i+1].fecha,auxf);
          	                    reg[i+1].tipo=auxy;            
                                          
                            }

                        }
                        }
                        ciclo=0;
                        break;
                    }
                break;
            }//switch(opcion)
            
            switch(teclado){
                case ESCAPE:
                    return;
                    break;
                }
             }//while(ciclo)
            
}//ordenar_ventana()

void editar_ventana(registro *reg, int indice, int tam){
//edita datos de un regitro y luego actuasliza la base de datos
        FILE *guardar;
        
        teclado=getch();
        switch(teclado){
            case ESCAPE:
                return;
                break;
            }   
            
            
        window(20,19,60,29);
        textbackground(RED);
        cprintf("%c",201);
        for(i=0;i<39;i++)
            cprintf("%c",205);
        cprintf("%c",187);
        for(i=1;i<9;i++)
            cprintf("%c                                       %c",186,186);
        cprintf("%c",200);
        for(i=0;i<39;i++)
            cprintf("%c",205);
        cprintf("%c",188);
                
        gotoxy(2,2);
        cprintf("Editar");
        gotoxy(2,3);
        cprintf("Ingrese Nombre:");
        gotoxy(2,4);
        gets(reg[indice].nombre);
        setbuf(stdin,NULL);
        gotoxy(2,5);
        if(reg[indice].tipo==MUSICA)
            cprintf("Ingrese Autor:");
        else
            cprintf("Ingrese Album:");
        gotoxy(2,6);
        gets(reg[indice].album_autor);
        setbuf(stdin,NULL);
        gotoxy(2,7);
        cprintf("Ingrese Fecha(aaaa/mm/dd):");
        gotoxy(2,8);
        gets(reg[indice].fecha);
        setbuf(stdin,NULL);
        textcolor(COLOR_TEXTO);
        textbackground(COLOR_FONDO);
        
        guardar=fopen("C:\\mainlista.msh","wb");
        for(i=0;i<=tam;i++)
            fwrite(&reg[i],sizeof(registro),1,guardar);
        fclose(guardar);
        
     }//editar_ventana();
     
void delete_reg(registro *archer,int select,int total){
    //borra  un regitro y luego actuasliza la base de datos
	 int i;
	 FILE *guardar;
	 for(i=select;i<total;i++){
        strcpy(archer[i].nombre,archer[i+1].nombre);
        strcpy(archer[i].ruta,archer[i+1].ruta);
        strcpy(archer[i].album_autor,archer[i+1].album_autor);
        strcpy(archer[i].fecha,archer[i+1].fecha); 
        archer[i].tipo=archer[i+1].tipo;								  
	}
	    guardar=fopen("C:\\mainlista.msh","wb");
        for(i=0;i<=total;i++)
            fwrite(&archer[i],sizeof(registro),1,guardar);
        fclose(guardar);
	
	
	 return;
	 }//delete_reg()

char seleccionar(int opcion,int seleccionado,char bg){
//selecciona el fondo de una area de la pantalla segun la comparacion sigueite
     if(opcion==seleccionado)
          return COLOR_SELECCIONADO;
     else
          return bg;
     }//seleccionar();
     
char texto_seleccionar(int opcion,int texto_seleccionado,char text){
//selecciona el texto de una area de la pantalla segun la comparacion sigueite
     if(opcion==texto_seleccionado)
          return COLOR_TEXTO_SELECCIONADO;
     else
          return text;
     }//texto_seleccionar();
     
void pantalla_completa(){
//simula la combinacion de teclas ALT+ENTER
     keybd_event(VK_MENU,0x38,0,0);
     keybd_event(VK_RETURN,0x1c,0,0);
     keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
     keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
     }//pantalla_completa();


void salir(){
//muetra una ventana de confirmacion para salir del programa
     int ciclo_salir=1;
          
     while(ciclo_salir){
        window(20,19,60,29);
        textbackground(RED);
        cprintf("%c",201);
        for(i=0;i<39;i++)
            cprintf("%c",205);
        cprintf("%c",187);
        for(i=1;i<9;i++)
            cprintf("%c                                       %c",186,186);
        cprintf("%c",200);
        for(i=0;i<39;i++)
            cprintf("%c",205);
        cprintf("%c",188);
                
        gotoxy(6,4);
        cprintf("Esta seguro de que quiere salir?");
        gotoxy(12,7);
        textbackground(seleccionar(opcion_salir,0,RED));
        textcolor(texto_seleccionar(opcion_salir,0,COLOR_TEXTO));
        cprintf("SI");
        gotoxy(30,7);
        textbackground(seleccionar(opcion_salir,1,RED));
        textcolor(texto_seleccionar(opcion_salir,1,COLOR_TEXTO));
        cprintf("NO");
        textbackground(COLOR_FONDO);
        textcolor(COLOR_TEXTO);
        
        teclado=getch();
        
        switch(opcion_salir){
            case 0:
                switch(teclado){
                    case DERECHA:
                        opcion_salir=1;
                        break;
                    case ENTER:
                        exit(0);
                        break;
                    }
                break;
            case 1:
                switch(teclado){
                    case IZQUIERDA:
                        opcion_salir=0;
                        break;
                    case ENTER:
                        ciclo_salir=0;
                        break;
                    }
                break;
            }//switch(opcion_salir)
        }//while(ciclo_salir)
     }//salir();
     
     
void mostrar_titulo(){
/*----AQUI EMPIEZA EL TITULO----*/
        textcolor(COLOR_TITULO);
        printf("\n\n\t  %c%c   %c%c %c%c  %c%c  %c%c%c%c %c%c  %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c  %c%c  %c%c%c%c\n",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        printf("\t  %c%c%c %c%c%c %c%c  %c%c %c%c    %c%c  %c%c %c%c%c %c%c%c %c%c    %c%c  %c %c%c %c%c%c%c%c%c\n",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        printf("\t  %c%c %c %c%c %c%c  %c%c  %c%c%c  %c%c%c%c%c%c %c%c %c %c%c %c%c%c%c  %c%c  %c %c%c %c%c  %c%c\n",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        printf("\t  %c%c   %c%c %c%c  %c%c    %c%c %c%c  %c%c %c%c   %c%c %c%c    %c%c  %c %c%c %c%c%c%c%c%c\n",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        printf("\t  %c%c   %c%c  %c%c%c%c  %c%c%c%c  %c%c  %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c  %c%c %c%c  %c%c v1.0\n\n",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_TEXTO);
/*----AQUI TERMINA EL TITULO----*/
}

void mostrar_menu(){
    while(1){   
        setbuf(stdin,NULL);
        mostrar_titulo();
        printf("\n\n\n\n");

/*----AQUI EMPIEZAN LOS ICONOS----*/
//Linea 1 
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        printf("                      \n");
//Linea 2
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c%c%c",219,219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c%c%c    ",219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("        %c%c%c%c%c%c%c%c%c%c%c%c  \n",219,219,219,219,219,219,219,219,219,219,219,219);
//Linea 3
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c %c%c%c%c%c%c%c%c%c%c %c    ",219,219,219,219,219,219,219,219,219,219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("        %c%c%c%c%c%c%c%c%c%c%c%c  \n",219,219,219,219,219,219,219,219,219,219,219,219);
//Linea 4
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c%c%c",219,219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c%c%c    ",219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c",219,219,219);
        textcolor(COLOR_SOL);
        printf("%c%c",219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c%c",219,219,219,219,219);
        textcolor(COLOR_ARBOL);
        printf("%c%c",219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("        %c%c%c%c%c%c%c%c%c%c%c%c  \n",219,219,219,219,219,219,219,219,219,219,219,219);
//Liena 5
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c %c",219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c %c    ",219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c",219,219,219);
        textcolor(COLOR_SOL);
        printf("%c%c",219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_ARBOL);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c",219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("       %c%c%c       %c%c%c  \n",219,219,219,219,219,219);
//Linea 6
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_ARBOL);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c",219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("       %c%c%c       %c%c%c  \n",219,219,219,219,219,219);
//Linea 7
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c %c",219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c %c    ",219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_TRONCO);
        printf("%c%c",219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("      %c%c%c       %c%c%c   \n",219,219,219,219,219,219);
//Linea 8
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c%c%c",219,219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c%c%c    ",219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_TRONCO);
        printf("%c%c",219,219);
        textcolor(COLOR_CIELO);
        printf("%c%c%c%c",219,219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("    %c%c%c%c%c     %c%c%c%c%c   \n",219,219,219,219,219,219,219,219,219,219);
//Linea 9
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c %c%c%c%c%c%c%c%c%c%c %c    ",219,219,219,219,219,219,219,219,219,219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c",219);
        textcolor(COLOR_PASTO);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textcolor(COLOR_BORDE);
        printf("%c  ",219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("  %c%c%c%c%c%c    %c%c%c%c%c%c    \n",219,219,219,219,219,219,219,219,219,219,219,219);
//Linea 10
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(COLOR_CINTA1);
        printf("    %c%c%c",219,219,219);
        textcolor(COLOR_CINTA2);
        printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
        textcolor(COLOR_CINTA1);
        printf("%c%c%c    ",219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(COLOR_BORDE);
        printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(COLOR_NOTA);
        printf("  %c%c%c%c%c     %c%c%c%c%c     \n",219,219,219,219,219,219,219,219,219,219);
//Linea 11
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        printf("                      \n");
//Linea 12
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,1,COLOR_TEXTO));
        printf("      MIS VIDEOS      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,2,COLOR_TEXTO));
        printf("     MIS IMAGENES     ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,3,COLOR_TEXTO));
        printf("      MI MUSICA       \n");
//linea 13
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,1,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,2,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,3,COLOR_FONDO));
        printf("                      \n\n\n\n\n");
        textbackground(COLOR_FONDO);
/*----AQUI TERMINAN LOS ICONOS---*/

/*----AQUI EMPIEZAN LOS ELEMENTOS DEL MENU SIN ICONO---*/
//Linea 1
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,4,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,5,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,6,COLOR_FONDO));
        printf("                      \n");
//Linea 2
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,4,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,4,COLOR_TEXTO));
        printf("   AGREGAR ARCHIVO    ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,5,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,5,COLOR_TEXTO));
        printf("VER TODOS LOS ARCHIVOS");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,6,COLOR_FONDO));
        textcolor(texto_seleccionar(menu_seleccionado,6,COLOR_TEXTO));
        printf("        SALIR         \n");
//Linea 3
        textbackground(COLOR_FONDO);
        printf("   ");
        textbackground(seleccionar(menu_seleccionado,4,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,5,COLOR_FONDO));
        printf("                      ");
        textbackground(COLOR_FONDO);
        printf("    ");
        textbackground(seleccionar(menu_seleccionado,6,COLOR_FONDO));
        printf("                      \n\n\n");
        
/*----AQUI TERMINAN LOS ELEMENTOS DEL MENU SIN ICONO---*/

        textbackground(COLOR_FONDO);
        textcolor(COLOR_TEXTO);
        
        teclado=getch();
        
        switch(menu_seleccionado){
        //MIS VIDEOS
              case 1:
                   switch (teclado) {
                         case ABAJO:
                              menu_seleccionado=4;
                              break;
                         case DERECHA:
                              menu_seleccionado=2;
                              break;
                         case ENTER:
///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER EN MIS VIDEOS
                              lista(VIDEO);
                              break;
                         }
                   break;
        //MIS IMAGENES
              case 2:
                   switch (teclado) {
                         case ABAJO:
                              menu_seleccionado=5;
                              break;
                         case DERECHA:
                              menu_seleccionado=3;
                              break;
                         case IZQUIERDA:
                              menu_seleccionado=1;
                              break;
                         case ENTER:
///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER EN MIS IMAGENES
                              lista(IMAGEN);
                              break;
                         }
                   break;
        //MI MUSICA
              case 3:
                   switch (teclado) {
                         case ABAJO:
                              menu_seleccionado=6;
                              break;
                         case IZQUIERDA:
                              menu_seleccionado=2;
                              break;
                         case ENTER:
///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER EN MI MUSICA
                              lista(MUSICA);
                              break;
                         }
                   break;
        //AGREGAR ARCHIVO
              case 4:
                   switch (teclado) {
                         case ARRIBA:
                              menu_seleccionado=1;
                              break;
                         case DERECHA:
                              menu_seleccionado=5;
                              break;
                         case ENTER:
///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER EN AGREGAR ARCHIVO
                              explorar();
                              break;
                         }
                   break;
        //VER TODOS LOS ARCHIVOS
              case 5:
                   switch (teclado) {
                         case ARRIBA:
                              menu_seleccionado=2;
                              break;
                         case DERECHA:
                              menu_seleccionado=6;
                              break;
                         case IZQUIERDA:
                              menu_seleccionado=4;
                              break;
                         case ENTER:
///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER EN VER TODOS LOS
/////////////// ARCHIVOS
                              lista(TODOS);
                              break;
                         }
                   break;
        //SALIR
              case 6:
                   switch (teclado) {
                         case ARRIBA:
                              menu_seleccionado=3;
                              break;
                         case IZQUIERDA:
                              menu_seleccionado=5;
                              break;
                         case ENTER:

///////////////// AQUI VA LA FUNCION QUE SE EJECUTA CUANDO SE APRETA ENTER SALIR
                              salir();
                              break;
                         }
                   break;
              }//switch(opcion)
            
        system("cls");  
    }//while
}//mostrar_menu();
