#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stddef.h>
#include <malloc.h>
#include <fstream>
#include <iostream>
#include <string.h>

#define BIENVENIDA          "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;93m BIENVENIDO \033[1;0m \033[1;97m"
#define CREADOR             "\033[1;92mGeremiasLS \033[1;97m/:  CREADO POR \033[1;46m GeremiasLS \033[1;0m \033[1;46m 200313184 \033[1;0m \n\033[1;97m"
#define SISTEMA             "\033[1;92mGeremiasLS \033[1;97m/:  "
#define ERROR               "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;101mERROR:      \033[1;0m \033[1;97m"
#define INFORMACION         "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;42mINFORMACION: \033[1;0m \033[1;97m"
#define EXITO               "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;46mSUCCESS:     \033[1;0m \033[1;97m"
#define BIENVENIDA          "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;93mBIENVENIDO   \033[1;0m \033[1;97m"
#define MONTADO             "\033[1;92mGeremiasLS \033[1;97m/:  \033[1;105mDISCO:      \033[1;0m \033[1;97m"




/*DESDE AQUI INICIAN LOS STRUCTS*/

/*STRUCT DE LA O LAS PARTICIONES QUE FUERON MONTADAS*/


typedef struct{
    char partition_status;
    char partition_type;
    char partition_fit;
    int partition_start;
    int partition_size;
    char partition_name[16];
} MBRPARTITION;

typedef struct{
    char mount_status;
    char mount_type;
    char mount_fit;
    int mount_start;
    int mount_size;
    char mount_id[5];
    char mount_name[16];
} PMOUNT;



typedef  struct{
    char name[999];
    char group[999];
    char pass[999];
    int numGrupo;
    int position;
} userstats;

/*STRUCT DISCO MONTADO
ESTE TIENE LA INFORMACION DE LA DIRECCION DEL DISCO Y DE SUS PARTICIONES MONTADAS
HAY QUE DESTACAR QUE TOOOODO STRUCT QUE SEA REFERENTE A FECHA SE LE PUSO UN VECTOR CHAR
DE TAMAÑO 16 PARA QUE ESTE PUEDA
*/



typedef struct{
    PMOUNT particiones_disco[10];
    char path[300];
    int letra;
} MONTURADISCO;


typedef struct{
    char nombre[999];
    char contenido[280320];
    int estado;
    int idGrupo;
    int iduser;
    int permArch;
    int position;
} archivo;

typedef struct{
    char name[16];
    char group[16];
    int permGroup;
    int tipouser;
    char namePartition[16];
    char pathDisco[300];
    int startiNodes;
    int startBlocks;
    int sizePartition;
    int id_user;
    int id_grupo;
    PMOUNT partUser;
    char bm_Block[270000];
    char bm_iNode[270000];
} user;







/*
DESDE AQUI APARECEN LOS STRUCTS DE LAS DIFERENTES FASES

AQUI EMPIEZAN LOS STRUCTS QUE CORRESPONDEN AL MBR Y EBR
*/
typedef struct{
    int mbr_size;
    char mbr_creation_date[16];
    int mbr_id;
    char mbr_fit;
    MBRPARTITION mbr_partitions[4];
} MBR;

typedef struct{
    char ebr_status;
    char ebr_fit;
    int ebr_start;
    int ebr_size;
    int ebr_next;
    char ebr_name[16];
} EBR;



/*
AQUI YA APARECEN LOS STRUCTS DE LA SEGUNDA FASE
*/

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime[16];
    char i_ctime[16];
    char i_mtime[16];
    int i_block[15];
    char i_type;
    int i_perm;
} inode;


typedef struct{
    int sb_filesystem_type;
    int sb_inodes_count;
    int sb_blocks_count;
    int sb_free_inodes_count;
    int sb_free_blocks_count;
    char sb_mtime[16];
    char sb_umtime[16];
    int mount_count;
    int sb_magic;
    int sb_inode_size;
    int sb_block_size;
    int sb_first_inode;
    int sb_first_block;
    int sb_bm_inode;
    int sb_bm_Block;
    int sb_t_inode;
    int sb_t_block;
} SuperBloque;

typedef struct{
    char j_op_type;
    char j_type;
    char j_name[499];
    char j_content[499];
    char j_date[16];
    int j_uid;
    int j_perm;
} journal;








typedef struct{
    char f_name[12];
    int f_inode;
} content;

/*
BLOQUE APUNTADORES
*/
typedef struct{
    content contenapunta[4];
} contentBlock;

typedef struct{
    char fcontenapunta[64];
} fileBlock;

typedef struct{
    int pb_pointers[16]; //Bloque de apuntadores, posiciones en bytes hacia otros bloques de carpeta o archivo
} pointerBlock;

typedef struct{
    int n;
    fileBlock contenido[4500];
} archivoPieza;

typedef struct{
    int n;
    char path[999];
} archivoN;

MONTURADISCO discosMontados[26];
user usuariologueadop;
userstats globusers[200];
userstats globGrupos[200];
int globNusers;
int globNGrupos;
bool exitoArchivo = false;
bool  noRecovery = true;
//----------------------------------------------

/*
    AQUI VAN LAS INICIALIZACIONES DE LOS METODOS
*/
void ToLowerC(char entrada[]);
int initDM();
int initUsr();
SuperBloque initSP(SuperBloque SP);
journal initJournal(journal J);
inode initiNode(inode N);
contentBlock initContentBlock(contentBlock CB);
int existsFile(char *fileName);
int verificacion();
int verificarExtension(char entrada[],char extension[]);
int verificarExtension2(char entrada[]);
int CrearPath(char entrada[]);
int getiNodeStart_2(int n, int start);
int getBlockStart_2(int n, int start);
SuperBloque getSuperBlock(PMOUNT particion, FILE *archivoDisco);
int nextFree( char bitMap[], int position);
int nextUsed(char bitMap[] , int position);
int lastUsed( char bitMap[] , int position);
int nextJournal(FILE *archivoDisco, int bm_start);
int checkPermisos(int uid, int gid, int permisosActuales, char operationType);
char *getPermisos(int permisosActuales);
archivoPieza contentSpliter(char texto[]);
archivo getArchivo(char ruta[], PMOUNT particion, int start, int startinodo, FILE* archivoDisco);
int findCarpeta(char pathVirtual[], contentBlock bloqueContenido, int operationType);
int findArchivoSub(char pathVirtual[], pointerBlock pb, int subPosicion, int operationType, FILE*archivoDisco);
int findArchivo(char pathVirtual[], inode inodoActual, int operationType, FILE*archivoDisco);
int positionArchivo(char pathVirtual[] , inode inodoActual , int niNodo, int niNodoPos, SuperBloque sp_aux, FILE*archivoDisco);
int modifyiNodeCarpet(int position,  int start, FILE *archivoDisco , int end, int genPadres);
int modifyBlockCarpet(int position, int start, FILE *archivoDisco , int end, int nap);
int findUser(FILE *archivoDisco);
int findUser2(PMOUNT particionTrabajo, FILE* archivoDisco);
int writeJournal(journal entrada, FILE *archivoDisco, int position);
int cleanArchivo(int start, char bm_Block_new[], int init_inode, int init_bm_Blocks, FILE* archivoDisco);
int fillArchivo(int start, char nuevo_texto[], char bm_Block_new[], int init_inode, int init_bm_Blocks, char creacion[], int permisos, FILE *archivoDisco);
int modificarArchivo(int start, char nuevoText[], char viejoTexto[], FILE *archivoDisco);
int escribirBitMap(FILE *archivoDisco, SuperBloque sp_aux);
archivo obtenerContent(char pathArchivo[]);
int crearCarpetaSub(int iNodeNumber,  int iNodeFather, FILE * archivoDisco);
char *auxFindCarpeta(int inodo, char expresion[], int tabulaciones, FILE *archivoDisco);
int actualizarSP(PMOUNT particion, int esMount, FILE* archivoDisco);
int ejecutarLOGIN(char usr[], char pwd[], char pathDisco[], PMOUNT particion, FILE *archivoDisco);
int ejecutarLOGOUT();
int ejecutarMKGRP(char groupName[], FILE* archivoDisco);
int ejecutarRMGRP(char groupName[], FILE* archivoDisco);
int ejecutarMKUSR(char user[], char password[], char groupName[], FILE* archivoDisco);
int ejecutarRMUSR(char userName[], FILE* archivoDisco);
int ejecutarCHGRP(char userV[], char grupoN[], FILE* archivoDisco);
int ejecutarARCHIVO(char pathVirtual[], int sizeFile, char contentFile[], int contentFileExists, int genPadres, int operationType, char rename[], FILE *archivoDisco);
int ejecutarMKFS(char pathDisco[], PMOUNT particion, char type, int fs);
int ejecutarCONVERT(PMOUNT particionTrabajo, char pathDisco[]);
int ejecutarRECOVERY(char pathDisco[], PMOUNT particion);
int ejecutarSYSLOSS(char pathDisco[], PMOUNT particion);
int ejecutarMKDisk(char path[], int sizeDisk, char fitDisk);
int ejecutarRMDisk(char path[]);
int ejecutarFDisk( char path[], char name[], int size, char type, char fit, int del, int add, int tipoFDisk);
int ejecutarMount(char path[], char name[]);
int ejecutarUnmount(char id[]);
int generarMBR(char pathSalida[], char pathDisco[]);
int generarDISK(char pathSalida[], char pathDisco[]);
int generarINODE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarBLOCK(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarJOURNALING(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarBM_INODO(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarBM_BLOQUE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarTREEiNode(FILE *archivoReporte, FILE*archivoDisco, char pathDisco[], int padre, int actual, char tipo, SuperBloque sp_aux, PMOUNT particionTrabajo);
int generarTREE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarSUPERBLOQUE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo);
int generarFILE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo, char rutaArchivo[]);
int generarLS(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo, char rutaArchivo[]);
int ejecutarRep(char pathSalida[], char pathDisco[],int tipoRep,PMOUNT particionTrabajo, char rutaArchivo[]);
void ejecutarShowMount();
int cargaComando(char entrada[]);
int ejecutarExec(char path[]);
//----------------------------------------------
int main(){
    int resolucion = 0;
    usuariologueadop.id_grupo = 3;
    usuariologueadop.id_user = 3;
    checkPermisos(4,2,664,'r');
    initDM();
    initUsr();


   printf("%s\n",BIENVENIDA);
    printf("%s","Bienvenidos al Sistema de Archivos \n",SISTEMA);
    char* comando = new char;

    while(true)
    {

        printf("%s","Ingrese el comando que desea realizar \n");
        printf("%s \033[1;96mComando: ",SISTEMA);
        printf("\033[1;97m");
        std::cin.getline(comando,500);

        if(strcasecmp(comando,"exit")==0)
        {
            printf("%s",CREADOR);
            break;
        }
        else
        {
          cargaComando(comando);
        }

    }






    return 0;
}


void ToLowerC(char entrada[]){

    for(int i = 0; i < strlen(entrada) ; i++){
        if(65 <= entrada[i] && entrada[i] <= 90){
            entrada[i] = entrada[i] + 32;
        }
    }
}
int initDM(){
    for (int i = 0 ;i < 26; i++) {
        discosMontados[i].letra = -1;
        memset(&discosMontados[i].path,0,300);
        for ( int j = 0 ; j < 10 ; j ++) {
            discosMontados[i].particiones_disco[j].mount_start = -1;
            discosMontados[i].particiones_disco[j].mount_size = -1;

            discosMontados[i].particiones_disco[j].mount_status = 'n';
            discosMontados[i].particiones_disco[j].mount_type = 'p';
            discosMontados[i].particiones_disco[j].mount_fit = 'f';

            memset(&discosMontados[i].particiones_disco[j].mount_id,0,5);
            memset(&discosMontados[i].particiones_disco[j].mount_name,0,16);
        }
    }
    return 0;
}
int initUsr(){
    memset(&usuariologueadop.name,0,16);
    memset(&usuariologueadop.group,0,16);
    memset(&usuariologueadop.namePartition,0,16);
    memset(&usuariologueadop.pathDisco,0,300);

    usuariologueadop.permGroup = -1;
    usuariologueadop.tipouser = -1;

    usuariologueadop.startiNodes = -1;
    usuariologueadop.sizePartition = -1;

    usuariologueadop.id_user = -1;
    usuariologueadop.id_grupo = -1;

    memset(&usuariologueadop.bm_Block,0,999);
    memset(&usuariologueadop.bm_iNode,0,999);

    return 0;
}
SuperBloque initSP(SuperBloque SP){
    SP.sb_filesystem_type = -1; //Tipo de sistema de archivos: 1 = EXT2 | 2 = EXT3
    SP.sb_inodes_count = -1; //Cantidad de inodos usados
    SP.sb_blocks_count = -1; //Cantidad de bloques usados
    SP.sb_free_inodes_count = -1; //Cantidad de inodos libres
    SP.sb_free_blocks_count = -1; //Cantidad de bloques libres

    memset(&SP.sb_mtime,0,16); //Ultima fecha de montado
    memset(&SP.sb_umtime,0,16); //Ultima fecha de desmontado

    SP.mount_count = -1; //Cantidad de veces que ha sido montado
    SP.sb_magic = -1; //Este valor debe ser 0xEF53 = 61267
    SP.sb_inode_size = -1; //Size de un inodo individual
    SP.sb_block_size = -1; //Size de un bloque individual
    SP.sb_first_inode = -1; //Posicion en bytes del primer inodo libre
    SP.sb_first_block = -1; //Posicion en bytes del primer bloque libre

    SP.sb_bm_inode = -1; //Posicion en bytes del bitmap de inodos
    SP.sb_bm_Block = -1; //Posicion en bytes del bitmap de bloques

    SP.sb_t_inode = -1; //Posicion en bytes de la tabla de inodos
    SP.sb_t_block = -1; //Posicion en bytes de la tabla de bloques
    return SP;
}
journal initJournal(journal J){
    J.j_op_type = 'x';
    J.j_type = 'x';

    memset(&J.j_name,0,999);
    memset(&J.j_date,0,16);
    memset(&J.j_content,0,999);

    J.j_uid = -1;
    J.j_perm = -1;
    return J;
}
inode initiNode(inode N){
    N.i_uid = -1; //ID del propietario
    N.i_gid = -1; //ID del grupo al que pertenece
    N.i_size = -1; //Tamano total del archivo en bytes (se debe actualizar cada vez que se genera una carpeta o archivo, la suma total de todo el contenido?)
    memset(&N.i_atime[16],0,16); //ultima fecha y hora cuando se lee sin modificar contenido
    memset(&N.i_ctime[16],0,16); //fecha y hora en la que se creo el inodo
    memset(&N.i_mtime[16],0,16); //Ultima fecha y hora en la que se modifico el inodo (Contenido dentro del inodo fue modificado)

    for(int i = 0; i < 15 ; i ++){
        N.i_block[i] = -1;
    } //posicion en bytes de los bloques contenidos dentro del inodo 'contentBlock'
    N.i_type = 'x'; //Tipo de inodo: 1 = archivo | 0 = carpeta (Utiliza char por el peso en bytes)
    N.i_perm = -1; //Permisos del archivo o carpeta al cual el inodo apunta (ABC -> UGO) "9 bits"

    return N;
}
contentBlock initContentBlock(contentBlock CB){
    for(int i = 0; i < 4 ; i++){
        memset(&CB.contenapunta[i].f_name,0,12);
        CB.contenapunta[i].f_inode = -1;
    }
    return CB;
}
fileBlock initFileBlock(fileBlock FB){
    memset(&FB.fcontenapunta,0,64);
    return FB;
}
int existsFile(char *fileName){
   /*
    //Open es "funcion" de UNIX
    int fileFlag=open(fileName, 0); //Apertura del archivo deseado
    if (fileFlag<0){
        return 0; //No se pudo abrir el archivo = no existe
    }
    //Close es "funcion" de UNIX
    close(fileFlag);
    return 1; //Se abrio el archivo, y cerro exitosamente = existe

    */

    char direccion[2000];
    strcpy(direccion,fileName);
    std::fstream disco(direccion, std::ios::in);
    if(disco.fail()) return 0;
    return 1;





}

int verificacion(){
    printf("\033[93m ¿ACEPTA LO QUE PASARA? \033[0m (s/n) \n");
    char aux[999]= {0};
    fgets(aux, 999, stdin);
    ToLowerC(aux);
    if( strcmp(aux , "s\n" ) == 0 )
        return 1;
    return -1;
}
int verificarExtension(char entrada[],char extension[]){
    int sizeExtension = strlen(extension);
    char aux[sizeExtension+1]; //Cadena que almacena la 'extension'
    int auxPos = 0;
    for (int i = strlen(entrada)-sizeExtension ; i < strlen(entrada) ; i++ ){
        //Recorrer desde el final menos el size de la extension
        aux[auxPos] = entrada[i];
        auxPos++;
    }
    aux[auxPos] = '\0';
    //Compara los caracteres restantes con la extension buscada
    if( strcmp( aux , extension ) == 0 ){
        return 0; //Comparacion exitosa
    }
    return -1; //Comparacion no exitosa
}
int verificarExtension2(char entrada[]){
    int length1 = strlen(entrada);
    if(entrada[length1-1] == '.'){
        return -1;
    }
    if(strcmp(entrada,"") == 0){
        return -1;
    }
    char aux2[length1+1];
    memset(&aux2,0,length1+1);
    int auxExtension = -1;
    for(int i = 0 ; i < length1 ; i++){
        sprintf(aux2,"%s%c",aux2,entrada[i]);
        if(entrada[i] == '/'){
            memset(&aux2,0,length1+1);
        }
    }
    char archivoExt[999] = {0};
    char extensionExt[999] = {0};
    int previo = -1;
    length1 = strlen(aux2);
    for(int i = 0 ; i < length1 ; i++ ){
        if(previo == 1 && aux2[i] == '.'){
            return -1;
        }
        if(aux2[i] == '.'){
            previo = 1;
            continue;
        }
        if(previo == 1){
            sprintf(extensionExt,"%s%c",extensionExt,aux2[i]);
        }else{
            sprintf(archivoExt,"%s%c",archivoExt,aux2[i]);
        }
    }
    if(strlen(archivoExt) > 0 && strlen(extensionExt) > 0){
        auxExtension = 0;
    }
    return auxExtension;
}
int CrearPath(char entrada[]){
    int i = 0;
    int estadoRuta = 0;
    char aux[999] = {0};
    if(entrada[0] != 47){
        printf("\033[101m ERROR:\033[0m -path~:~ Incorrecto");
        return 0;
    }
    while(entrada[i] != NULL){
        //Mientras exista una ruta
        char aux_char[]= {entrada[i],'\0'};
        strcat(aux, aux_char);
        if(entrada[i] == 47){
            //Si se encuentra un '/'
            estadoRuta = 0;
            if(existsFile(aux) == 0){
                //Si no existe ruta, generar
                estadoRuta = mkdir(aux, S_IRWXU | S_IRWXG | S_IRWXO );
            }
            if(estadoRuta == -1 ){
                printf(" \033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Se ha encontrado un error al crear %s \n",aux);
            }
        }
        i++;
    }
    return 1;
}
int getiNodeStart_2(int n, int start){
    //Si no se especifica inicio, tomar inicio (Este inicio es de la tabla de inodos)
    if(start == 0){
        start = usuariologueadop.startiNodes;
    }
    //Valor no iniciado
    if(n == -1){
        return  -1;
    }
    //Valor de la posicion en bytes
    return start + (n * sizeof(inode));
}
int getBlockStart_2(int n, int start){
    //Si no se especifica inicio, tomar inicio (Este inicio es de la tabla de inodos)
    if(start == 0){
        start = usuariologueadop.startBlocks;
    }
    //Si el valor no ha sido iniciado
    if(n == -1){
        return -1;
    }
    //Valor de la posicion en bytes
    return start +( n * sizeof (fileBlock));
}
SuperBloque getSuperBlock(PMOUNT particion, FILE *archivoDisco){
    SuperBloque sp_aux;
    sp_aux = initSP(sp_aux);
    //Si existe disco y es .disk
    if(archivoDisco != NULL){
        //Buscar SuperBloque de la particion
        fseek(archivoDisco,particion.mount_start,SEEK_SET);
        fread(&sp_aux,sizeof(sp_aux),1,archivoDisco);
        if(sp_aux.sb_magic != 61267 && sp_aux.sb_magic != -1){
            //Lectura mal hecha, no se escribio bien
            sp_aux = initSP(sp_aux);
        }
    }
    return sp_aux;
}
int nextFree( char bitMap[], int position){
    //Siguiente bit libre (Mapa de bits)
    int length = strlen(bitMap);
    for( int i = position ; i < length ; i++){
        if(bitMap[i] == '0'){
            return i;
        }
    }
    return  -1;
}
int nextUsed(char bitMap[] , int position){
    //Siguiente bit no libre (Mapa de bits)
    int length = strlen(bitMap);
    for( int i = position ; i < length ; i++ ){
        if( bitMap[i] == '1' || bitMap[i] == '2' || bitMap[i] == '3' ){
            return i;
        }
    }
    return -1;
}
int lastUsed( char bitMap[] , int position){
    //Ultimo bit no libre (Mapa de bits)
    int length = strlen(bitMap);
    for( int i = length ; i >= position ; i-- ){
        if( bitMap[i] == '1' || bitMap[i] == '2' || bitMap[i] == '3' ){
            return i;
        }
    }
    return -1;
}
int nextJournal(FILE *archivoDisco, int bm_start){
    int start = usuariologueadop.partUser.mount_start + sizeof (SuperBloque);
    journal journalaux;
    fseek(archivoDisco, start, SEEK_SET);
    int i = 0;
    while (bm_start > ftell(archivoDisco)) {
        fread(&journalaux,sizeof (journal),1,archivoDisco);
        if(journalaux.j_uid == -1){
            return i;
        }
        i++;
    }
    return -1;
}
int checkPermisos(int uid, int gid, int permisosActuales, char operationType){
    int acceso = -1;
    if(usuariologueadop.id_user == 1){
        //Es user root o user pertenece a grupo root
        return 1;
    }
    //Convertir los permisosn a char[] para revisar uno por uno
    char permits[3] = {0};
    sprintf(permits,"%i",permisosActuales);
    if(usuariologueadop.id_user == uid){
        //Es propietario
        int tempPermit = permits[0]-48;
        int auxBit = 0;
        char bitsPermit[3] = {0};
        for(int i = 0 ; i < 3 ; i++){
            auxBit = tempPermit%2;
            tempPermit = tempPermit/2;
            bitsPermit[2-i] = (char)(auxBit+48);
        }
        if(operationType == 'r'){
            if(bitsPermit[0] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'w'){
            if(bitsPermit[1] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'x'){
            if(bitsPermit[2] == '1'){
                acceso = 1;
            }
        }else{
            //Handler de error (¿?)
        }
    }else if(usuariologueadop.id_grupo == gid){
        //Es parte del grupo propietario
        int tempPermit = permits[1]-48;
        int auxBit = 0;
        char bitsPermit[3] = {0};
        for(int i = 0 ; i < 3 ; i++){
            auxBit = tempPermit%2;
            tempPermit = tempPermit/2;
            bitsPermit[2-i] = (char)(auxBit+48);
        }
        if(operationType == 'r'){
            if(bitsPermit[0] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'w'){
            if(bitsPermit[1] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'x'){
            if(bitsPermit[2] == '1'){
                acceso = 1;
            }
        }else{
            //Handler de error (¿?)
        }
    }else{
        //Es parte de 'otros'
        int tempPermit = permits[2]-48;
        int auxBit = 0;
        char bitsPermit[3] = {0};
        for(int i = 0 ; i < 3 ; i++){
            auxBit = tempPermit%2;
            tempPermit = tempPermit/2;
            bitsPermit[2-i] = (char)(auxBit+48);
        }
        if(operationType == 'r'){
            if(bitsPermit[0] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'w'){
            if(bitsPermit[1] == '1'){
                acceso = 1;
            }
        }else if(operationType == 'x'){
            if(bitsPermit[2] == '1'){
                acceso = 1;
            }
        }
    }
    return acceso;
}
char *getPermisos(int permisosActuales){
    char tempPerm[11] = {0};
    char permits[3] = {0};
    sprintf(permits,"%i",permisosActuales);
    for(int i = 0 ; i < 3 ; i++){
        int tempPermit = permits[i]-48;
        int auxBit = 0;
        char bitsPermit[3] = {0};
        for(int j = 0 ; j < 3 ; j++){
            auxBit = tempPermit%2;
            tempPermit = tempPermit/2;
            bitsPermit[2-j] = (char)(auxBit+48);
        }
        if(bitsPermit[0] == '1'){
            sprintf(tempPerm,"%s%c",tempPerm,'r');
        }else{
            sprintf(tempPerm,"%s%c",tempPerm,'-');
        }
        if(bitsPermit[1] == '1'){
            sprintf(tempPerm,"%s%c",tempPerm,'w');
        }else{
            sprintf(tempPerm,"%s%c",tempPerm,'-');
        }
        if(bitsPermit[2] == '1'){
            sprintf(tempPerm,"%s%c",tempPerm,'x');
        }else{
            sprintf(tempPerm,"%s%c",tempPerm,'-');
        }
        if( i < 2){
            sprintf(tempPerm,"%s%c",tempPerm,'|');
        }
    }
    char *salida = strdup(tempPerm);
    return salida;
}
archivoPieza contentSpliter(char texto[]){
    //Archivo de contenido repartido
    archivoPieza salida;
    salida.n = 0;
    int aux = 0;
    int n = 0;
    memset(&salida.contenido[0].fcontenapunta,0,64);
    for(int i = 0 ; i < strlen(texto) ; i++){
        //Recorrer todo el contenido del archivo
        char aux_char[] = {texto[i],'\0'};
        if(aux == 63){
            //Separar si es 64, osea tope de bloque
            aux = 0;
            n++;
            //Limpiar siquiente bloque
            memset(&salida.contenido[n].fcontenapunta,0,64);
        }
        //llenar bloque actual
        strcat(salida.contenido[n].fcontenapunta, aux_char);
        aux++;
    }
    salida.n = n+1;
    return salida;
}
archivo getArchivo(char ruta[], PMOUNT particion, int start, int startinodo, FILE* archivoDisco){
    archivo tempArch; //Archivo final
    memset(&tempArch.contenido,0,280320);
    tempArch.estado = -1;
    if(archivoDisco != NULL){
        SuperBloque sp_aux; //Super bloque de la particion
        sp_aux = getSuperBlock(particion,archivoDisco);
        if(sp_aux.sb_magic == 61267){
            pointerBlock pb1; //Indirectos
            pointerBlock pb2; //Doble indirecto
            pointerBlock pb3; //Triple indirecto
            fileBlock contenidoArchivo; //Bloque de contenido aux
            contentBlock bloqueContenido; //Bloque de carpetas y archivos
            inode inodoM; //iNodo del archivo
            fseek(archivoDisco,startinodo,SEEK_SET);
            fread(&inodoM, sizeof(inode),1,archivoDisco);
            char auxRuta[999]={0};
            strcpy(auxRuta,ruta);
            bool encontrado = false;
            int posision = -1;
            //Recorrer buscando iNodos
            for( int j = 0; j < 15; j++){
                if(encontrado == true){
                    break;
                }
                if( inodoM.i_block[j] == -1){
                    //Saltar siguientes acciones
                    continue;
                }
                if( j < 12 ){
                    //Bloques directos
                    fseek(archivoDisco, getBlockStart_2(inodoM.i_block[j], sp_aux.sb_t_block),SEEK_SET);
                    fread(&bloqueContenido, sizeof (bloqueContenido), 1 , archivoDisco);
                    for( int a= 0; a < 4 ; a++){
                        if(encontrado == true){
                            break;
                        }
                        if(strcmp(bloqueContenido.contenapunta[a].f_name, auxRuta) == 0){
                            tempArch.position = bloqueContenido.contenapunta[a].f_inode;
                            fseek(archivoDisco, getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode,sp_aux.sb_t_inode),SEEK_SET);
                            fread(&inodoM, sizeof (inode),1,archivoDisco);
                            posision = getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode, sp_aux.sb_t_inode);
                            encontrado = true;
                        }
                    }
                }else{
                    //Bloques indirectos
                    fseek(archivoDisco, getBlockStart_2(inodoM.i_block[j], sp_aux.sb_t_block),SEEK_SET);
                    fread(&pb1,sizeof (pointerBlock),1,archivoDisco);
                    for( int z = 0; z < 16 ; z++){
                        if(encontrado == true){
                            break;
                        }
                        if(pb1.pb_pointers[z] == -1){
                            //Saltar siguientes acciones
                            continue;
                        }
                        if(j == 12){
                            //indirecto
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[z], sp_aux.sb_t_block),SEEK_SET);
                            fread(&bloqueContenido, sizeof (bloqueContenido), 1 , archivoDisco);
                            for( int a= 0; a < 4 ; a++){
                                if(encontrado == true){
                                    break;
                                }
                                if(strcmp(bloqueContenido.contenapunta[a].f_name, auxRuta) == 0){
                                    tempArch.position = bloqueContenido.contenapunta[a].f_inode;
                                    fseek(archivoDisco,getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode,sp_aux.sb_t_inode),SEEK_SET);
                                    fread(&inodoM, sizeof (inode),1,archivoDisco);
                                    posision = getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode,sp_aux.sb_t_inode);
                                    encontrado = true;
                                }
                            }
                        }else{
                            //doble indirecto
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[z], sp_aux.sb_t_block),SEEK_SET);
                            fread(&pb2, sizeof (pointerBlock),1,archivoDisco);
                            for( int w = 0; w < 16 ; w++){
                                if(encontrado == true){
                                    break;
                                }
                                if(pb2.pb_pointers[w] == -1){
                                    //Saltar siguientes acciones
                                    continue;
                                }
                                if(j == 13){
                                    //doble indirecto
                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[w], sp_aux.sb_t_block),SEEK_SET);
                                    fread(&bloqueContenido, sizeof (bloqueContenido), 1 , archivoDisco);
                                    for( int a= 0; a < 4 ; a++){
                                        if(encontrado == true){
                                            break;
                                        }
                                        if(strcmp(bloqueContenido.contenapunta[a].f_name, auxRuta) == 0){
                                            tempArch.position = bloqueContenido.contenapunta[a].f_inode;
                                            fseek(archivoDisco, getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode,sp_aux.sb_t_inode),SEEK_SET);
                                            fread(&inodoM, sizeof (inode),1,archivoDisco);
                                            posision = getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode,sp_aux.sb_t_inode);
                                            encontrado = true;
                                        }
                                    }
                                }else{
                                    //Triple indirecto
                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[w], sp_aux.sb_t_block),SEEK_SET);
                                    fread(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                    for( int v = 0; v < 16 ; v ++){
                                        if(encontrado == true){
                                            break;
                                        }
                                        if( pb3.pb_pointers[v] == -1){
                                            //Saltar siguientes acciones
                                            continue;
                                        }
                                        fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[v], sp_aux.sb_t_block),SEEK_SET);
                                        fread(&bloqueContenido, sizeof (bloqueContenido), 1 , archivoDisco);
                                        for( int a= 0; a < 4 ; a++){
                                            if(encontrado == true){
                                                break;
                                            }
                                            if(strcmp(bloqueContenido.contenapunta[a].f_name, auxRuta) == 0){
                                                tempArch.position = bloqueContenido.contenapunta[a].f_inode;
                                                fseek(archivoDisco, getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode, sp_aux.sb_t_inode),SEEK_SET);
                                                fread(&inodoM, sizeof (inode),1,archivoDisco);
                                                posision = getiNodeStart_2(bloqueContenido.contenapunta[a].f_inode, sp_aux.sb_t_inode);
                                                encontrado = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(encontrado == true && inodoM.i_type == '1'){
                tempArch.estado = 1;
                tempArch.iduser = inodoM.i_uid;
                tempArch.idGrupo = inodoM.i_gid;
                tempArch.permArch = inodoM.i_perm;

                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(inodoM.i_atime, sizeof(inodoM.i_atime)-1, "%d/%m/%y %H:%M", fecha);

                for( int j = 0 ; j < 15 ; j++){
                    if(inodoM.i_block[j] == -1){
                        //Saltar siguientes acciones
                        continue;
                    }
                    if( j < 12){
                        fseek(archivoDisco, getBlockStart_2(inodoM.i_block[j],sp_aux.sb_t_block),SEEK_SET);
                        fread(&contenidoArchivo,sizeof (fileBlock),1,archivoDisco);
                        sprintf(tempArch.contenido,"%s%s",tempArch.contenido,contenidoArchivo.fcontenapunta);
                    }else{
                        //indirecto
                        fseek(archivoDisco, getBlockStart_2(inodoM.i_block[j], sp_aux.sb_t_block),SEEK_SET);
                        fread(&pb1,sizeof (pointerBlock),1,archivoDisco);
                        for( int z = 0; z < 16 ; z++){
                            if(pb1.pb_pointers[z] == -1){
                                //Saltar siguientes acciones
                                continue;
                            }
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[z],sp_aux.sb_t_block),SEEK_SET);
                            if(j == 12){
                                //indirecto
                                fread(&contenidoArchivo, sizeof (fileBlock), 1, archivoDisco);
                                sprintf(tempArch.contenido,"%s%s",tempArch.contenido,contenidoArchivo.fcontenapunta);
                            }else{
                                //doble indirecto
                                fread(&pb2, sizeof (pointerBlock),1,archivoDisco);
                                for( int w = 0; w < 16 ; w++){
                                    if(pb2.pb_pointers[w] == -1){
                                        //Saltar siguientes acciones
                                        continue;
                                    }
                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[w], sp_aux.sb_t_block), SEEK_SET);
                                    if(j == 13){
                                        //doble indirecto
                                        fread(&contenidoArchivo, sizeof (fileBlock), 1 , archivoDisco);
                                        sprintf(tempArch.contenido,"%s%s",tempArch.contenido,contenidoArchivo.fcontenapunta);
                                    }else{
                                        //Triple indirecto
                                        fread(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                        for( int v = 0; v < 16 ; v ++){
                                            if( pb3.pb_pointers[v] == -1){
                                                //Saltar siguientes acciones
                                                continue;
                                            }
                                            fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[v],sp_aux.sb_t_block),SEEK_SET);
                                            fread(&contenidoArchivo, sizeof (fileBlock), 1, archivoDisco);
                                            sprintf(tempArch.contenido,"%s%s",tempArch.contenido,contenidoArchivo.fcontenapunta);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                fseek(archivoDisco, posision, SEEK_SET);
                fwrite(&inodoM, sizeof (inode),1,archivoDisco);
                sprintf(tempArch.nombre,"%s", auxRuta);
            }else{
                //Algo salio mal
            }
        }else{
            //Algo salio mal
        }
    }else{
        //Algo salio mal
    }
    return tempArch;
}
int findCarpeta(char pathVirtual[], contentBlock bloqueContenido, int operationType){
    for (int i = 0 ; i < 4 ; i++ ) {
        if(strcmp(pathVirtual, bloqueContenido.contenapunta[i].f_name) == 0){
            return bloqueContenido.contenapunta[i].f_inode;
        }
    }
    return -1;
}
int findArchivoSub(char pathVirtual[], pointerBlock pb, int subPosicion, int operationType, FILE*archivoDisco){
    if(archivoDisco != NULL){
        pointerBlock pb_aux;
        contentBlock bloqueContenido;
        for(int i = 0; i< 16; i++){
            if(pb.pb_pointers[i] == -1){
                //Saltar siguientes acciones
                continue;
            }
            fseek(archivoDisco, getBlockStart_2(pb.pb_pointers[i], usuariologueadop.startBlocks) , SEEK_SET);
            if( subPosicion  == 0){
                fread(&bloqueContenido, sizeof(contentBlock), 1 , archivoDisco);
                int aux_carpeta = findCarpeta(pathVirtual,bloqueContenido, i);
                if(aux_carpeta != -1){
                    if(operationType == 1){
                        for(int j = 0; j < 4; j++){
                            if(bloqueContenido.contenapunta[j].f_inode == aux_carpeta){
                                bloqueContenido.contenapunta[j].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                            }
                        }
                        fseek(archivoDisco, getBlockStart_2(pb.pb_pointers[i] , usuariologueadop.startBlocks) , SEEK_SET);
                        fwrite(&bloqueContenido , sizeof(contentBlock) , 1 , archivoDisco);
                    }
                    return aux_carpeta;
                }
            }else{
                fread(&pb_aux , sizeof(pointerBlock) , 1 , archivoDisco);
                int aux_archivo = findArchivoSub(pathVirtual, pb_aux, subPosicion -1, operationType, archivoDisco);
                if(aux_archivo != -1){
                    return aux_archivo;
                }
            }
        }
        return -1;
    }
    return 0;
}
int findArchivo(char pathVirtual[], inode inodoActual, int operationType, FILE*archivoDisco){
    if(archivoDisco != NULL){
        pointerBlock pb;
        contentBlock bloqueContenido;
        for( int i = 0 ; i < 15 ; i++ ){
            if(inodoActual.i_block[i] == -1){
                //Saltar las siguientes acciones
                continue;
            }
            fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i], usuariologueadop.startBlocks), SEEK_SET);
            if( i < 12 ){
                //Mientras no sea bloque indirecto
                fread(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                int aux_carpeta = findCarpeta(pathVirtual,bloqueContenido, i);
                if(aux_carpeta != -1){
                    if(operationType == 1){
                        for(int j = 0; j < 4; j++){
                            if(bloqueContenido.contenapunta[j].f_inode == aux_carpeta){
                                bloqueContenido.contenapunta[j].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                            }
                        }
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks) , SEEK_SET);
                        fwrite(&bloqueContenido , sizeof(contentBlock) , 1 , archivoDisco);
                    }
                    return aux_carpeta;
                }
            }else{
                //Si es bloque indirecto
                fread(&pb,sizeof(pointerBlock),1,archivoDisco);
                int aux_archivo = findArchivoSub(pathVirtual, pb, i - 12, operationType,archivoDisco);
                if(aux_archivo != -1){
                    return aux_archivo;
                }
            }
        }
        return -1;
    }
    return 0;
}
int positionArchivo(char pathVirtual[] , inode inodoActual , int niNodo, int niNodoPos, SuperBloque sp_aux, FILE*archivoDisco){
    if(archivoDisco != NULL){
        pointerBlock pb1;
        pointerBlock pb2;
        pointerBlock pb3;
        contentBlock bloqueContenido;
        for(int i = 0; i < 15 ; i++){
            if(inodoActual.i_block[i] == -1){
                //Se debe generar bloque
                int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                if(niNodoPos_2 == -1){
                    printf("Bloques llenos.\n ");
                    return -1;
                }
                fseek(archivoDisco, getBlockStart_2(niNodoPos_2 , usuariologueadop.startBlocks) , SEEK_SET);
                if(i< 12){
                    //Bloque Directo
                    usuariologueadop.bm_Block[niNodoPos_2] = '1';
                    for(int j = 1; j < 4; j++){
                        bloqueContenido.contenapunta[j].f_inode = -1;
                        memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                    }
                    //iNodo de archivo/carpeta
                    bloqueContenido.contenapunta[0].f_inode = niNodo;
                    //Nombre de archivo/carpeta
                    strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                    //Escribirle posicion de bloque a inodo
                    inodoActual.i_block[i] = niNodoPos_2;
                    //Escribir bloque
                    fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                    //Reescribir iNodo
                    fseek(archivoDisco , getiNodeStart_2(niNodoPos, usuariologueadop.startiNodes) , SEEK_SET);
                    fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                    //Reescribir BitMap
                    escribirBitMap(archivoDisco , sp_aux);
                    return 1;
                }else{
                    if(i == 12){
                        //Indirecto
                        //No existe Indirecto
                        usuariologueadop.bm_Block[niNodoPos_2] = '3';
                        for(int a = 0 ; a < 16 ; a++ ){
                            pb1.pb_pointers[a] = -1;
                        }
                        //Escribirle posicion de apuntadores a inodo
                        inodoActual.i_block[i] = niNodoPos_2;
                        //llenar bloque
                        int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);
                        usuariologueadop.bm_Block[niNodoPos_3] = '1';
                        for(int a = 1; a < 4; a++){
                            bloqueContenido.contenapunta[a].f_inode = -1;
                            memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                        }
                        //iNodo de archivo/carpeta
                        bloqueContenido.contenapunta[0].f_inode = niNodo;
                        //Nombre de archivo/carpeta
                        strcpy(bloqueContenido.contenapunta[0].f_name,pathVirtual);
                        //Escribirle posicion de bloque a indirecto
                        pb1.pb_pointers[0] = niNodoPos_3;
                        //Escribirle posicion de indirectos a iNodo
                        inodoActual.i_block[i] = niNodoPos_2;
                        //Escribir bloque
                        fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                        //Escribir bloque indirecto
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                        fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                        //Reescribir iNodo
                        fseek(archivoDisco , getiNodeStart_2(niNodoPos, usuariologueadop.startiNodes) , SEEK_SET);
                        fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                        escribirBitMap(archivoDisco , sp_aux);
                        return 1;
                    }else{
                        if(i == 13){
                            //Doble Indirecto
                            //No existe Indirecto
                            usuariologueadop.bm_Block[niNodoPos_2] = '3';
                            for(int a = 0 ; a < 16 ; a++ ){
                                pb1.pb_pointers[a] = -1;
                            }
                            //Segundo bloque de apuntadores
                            int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);;
                            usuariologueadop.bm_Block[niNodoPos_3] = '3';
                            for(int a = 0 ; a < 16 ; a++ ){
                                pb2.pb_pointers[a] = -1;
                            }
                            //llenar bloque
                            int niNodoPos_4 = nextFree(usuariologueadop.bm_Block , 0);
                            usuariologueadop.bm_Block[niNodoPos_4] = '1';
                            for(int a = 1; a < 4; a++){
                                bloqueContenido.contenapunta[a].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                            }
                            //iNodo de archivo/carpeta
                            bloqueContenido.contenapunta[0].f_inode = niNodo;
                            //Nombre de archivo/carpeta
                            strcpy(bloqueContenido.contenapunta[0].f_name,pathVirtual);
                            //Escribirle posicion de bloque a indirecto
                            pb2.pb_pointers[0] = niNodoPos_4;
                            //Escribirle posicion de bloque a indirecto
                            pb1.pb_pointers[0] = niNodoPos_3;
                            //Escribirle posicion de indirectos a iNodo
                            inodoActual.i_block[i] = niNodoPos_2;
                            //Escribir bloque
                            fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                            //Escribir bloque indirecto 2
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                            //Escribir bloque indirecto 1
                            fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                            //Reescribir iNodo
                            fseek(archivoDisco , getiNodeStart_2(niNodoPos, usuariologueadop.startiNodes) , SEEK_SET);
                            fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                            escribirBitMap(archivoDisco , sp_aux);
                            return 1;
                        }else{
                            if(i == 14){
                                //Triple Indirecto
                                //No existe Indirecto
                                usuariologueadop.bm_Block[niNodoPos_2] = '3';
                                for(int a = 0 ; a < 16 ; a++ ){
                                    pb1.pb_pointers[a] = -1;
                                }
                                //Segundo bloque de apuntadores
                                int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);;
                                usuariologueadop.bm_Block[niNodoPos_3] = '3';
                                for(int a = 0 ; a < 16 ; a++ ){
                                    pb2.pb_pointers[a] = -1;
                                }
                                //tercer bloque de apuntadores
                                int niNodoPos_4 = nextFree(usuariologueadop.bm_Block , 0);;
                                usuariologueadop.bm_Block[niNodoPos_4] = '3';
                                for(int a = 0 ; a < 16 ; a++ ){
                                    pb2.pb_pointers[a] = -1;
                                }
                                //llenar bloque
                                int niNodoPos_5 = nextFree(usuariologueadop.bm_Block , 0);
                                usuariologueadop.bm_Block[niNodoPos_5] = '1';
                                for(int a = 1; a < 4; a++){
                                    bloqueContenido.contenapunta[a].f_inode = -1;
                                    memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                }
                                //iNodo de archivo/carpeta
                                bloqueContenido.contenapunta[0].f_inode = niNodo;
                                //Nombre de archivo/carpeta
                                strcpy(bloqueContenido.contenapunta[0].f_name,pathVirtual);
                                //Escribirle posicion de bloque a indirecto
                                pb3.pb_pointers[0] = niNodoPos_5;
                                //Escribirle posicion de bloque a indirecto
                                pb2.pb_pointers[0] = niNodoPos_4;
                                //Escribirle posicion de bloque a indirecto
                                pb1.pb_pointers[0] = niNodoPos_3;
                                //Escribirle posicion de indirectos a iNodo
                                inodoActual.i_block[i] = niNodoPos_2;
                                //Escribir bloque
                                fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                //Escribir bloque indirecto 2
                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&pb3, sizeof(pointerBlock) , 1 , archivoDisco);
                                //Escribir bloque indirecto 2
                                fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                                //Escribir bloque indirecto 1
                                fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                                //Reescribir iNodo
                                fseek(archivoDisco , getiNodeStart_2(niNodoPos, usuariologueadop.startiNodes) , SEEK_SET);
                                fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                                escribirBitMap(archivoDisco , sp_aux);
                                return 1;
                            }else{
                            }
                        }
                    }
                }
            }
            //Ya existe bloque
            if(i < 12){
                //Ya existe bloque
                fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                fread(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                for(int j = 0; j < 4; j++){
                    if(bloqueContenido.contenapunta[j].f_inode == -1){
                        bloqueContenido.contenapunta[j].f_inode = niNodo;
                        strcpy(bloqueContenido.contenapunta[j].f_name , pathVirtual);
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                        niNodoPos = niNodo;
                        return 1;
                    }
                }
            }else{
                if(i == 12){
                    //Indirecto
                    fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                    fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                    for (int j = 0 ; j < 16 ; j++ ) {
                        if(pb1.pb_pointers[j] == -1){
                            //Crear bloque
                            int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                            if(niNodoPos_2 == -1){
                                printf("Bloques llenos.\n ");
                                return -1;
                            }
                            fseek(archivoDisco, getBlockStart_2(niNodoPos_2 , usuariologueadop.startBlocks) , SEEK_SET);
                            //Bloque Directo
                            usuariologueadop.bm_Block[niNodoPos_2] = '1';
                            for(int k = 1; k < 4; k++){
                                bloqueContenido.contenapunta[k].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[k].f_name,0,12);
                            }
                            //iNodo de archivo/carpeta
                            bloqueContenido.contenapunta[0].f_inode = niNodo;
                            //Nombre de archivo/carpeta
                            strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                            //Escribirle posicion de bloque a inodo
                            pb1.pb_pointers[j] = niNodoPos_2;
                            //Escribir bloque
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                            //Reescribir bloque de punteros
                            fseek(archivoDisco , getBlockStart_2(inodoActual.i_block[i], usuariologueadop.startBlocks) , SEEK_SET);
                            fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                            //Reescribir BitMap
                            escribirBitMap(archivoDisco , sp_aux);
                            return 1;
                        }else{
                            //Ya existe bloque
                            fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                            fread(&bloqueContenido,sizeof (contentBlock),1,archivoDisco);
                            for(int a = 0 ; a < 4 ; a++){
                                if(bloqueContenido.contenapunta[a].f_inode == -1){
                                    bloqueContenido.contenapunta[a].f_inode = niNodo;
                                    strcpy(bloqueContenido.contenapunta[a].f_name,pathVirtual);
                                    fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                    niNodoPos = niNodo;
                                    return 1;
                                }
                            }
                        }
                    }
                }else{
                    if(i == 13){
                        //Doble Indirecto
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                        fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                        for (int j = 0 ; j < 16 ; j++ ) {
                            if(pb1.pb_pointers[j] == -1){
                                //Crear apuntador indirecto
                                int niNodoPos_2 = nextFree(usuariologueadop.bm_Block,0);
                                if(niNodoPos_2 == -1){
                                    printf("Bloques llenos.\n ");
                                    return -1;
                                }
                                usuariologueadop.bm_Block[niNodoPos_2] = '3';
                                int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);
                                if(niNodoPos_3 == -1){
                                    printf("Bloques llenos.\n ");
                                    return -1;
                                }
                                //Bloque Directo
                                usuariologueadop.bm_Block[niNodoPos_3] = '1';
                                for (int a = 0; a < 16; a++){
                                    pb2.pb_pointers[a] = -1;
                                }
                                for(int a = 1; a < 4; a++){
                                    bloqueContenido.contenapunta[a].f_inode = -1;
                                    memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                }
                                //iNodo de archivo/carpeta
                                bloqueContenido.contenapunta[0].f_inode = niNodo;
                                //Nombre de archivo/carpeta
                                strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                //Escribirle posicion de bloque a inodo
                                pb2.pb_pointers[0] = niNodoPos_3;
                                //Escribirle posicion de bloque a inodo
                                pb1.pb_pointers[j] = niNodoPos_2;
                                //Escribir bloque
                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                //Escribir indirecto
                                fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                                //Reescribir bloque de punteros
                                fseek(archivoDisco , getBlockStart_2(inodoActual.i_block[i], usuariologueadop.startBlocks) , SEEK_SET);
                                fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                                //Reescribir BitMap
                                escribirBitMap(archivoDisco , sp_aux);
                                return 1;
                            }else{
                                //Ya existe apuntador indirecto
                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                                fread(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                for (int b = 0 ; b < 16 ; b++ ){
                                    if(pb2.pb_pointers[b] == -1){
                                        //Hay que crear el bloque

                                        //Crear bloque
                                        int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                                        if(niNodoPos_2 == -1){
                                            printf("Bloques llenos.\n ");
                                            return -1;
                                        }
                                        //Bloque Directo
                                        usuariologueadop.bm_Block[niNodoPos_2] = '1';
                                        for(int a = 1; a < 4; a++){
                                            bloqueContenido.contenapunta[a].f_inode = -1;
                                            memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                        }
                                        //iNodo de archivo/carpeta
                                        bloqueContenido.contenapunta[0].f_inode = niNodo;
                                        //Nombre de archivo/carpeta
                                        strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                        //Escribirle posicion de bloque a inodo
                                        pb2.pb_pointers[b] = niNodoPos_2;
                                        //Escribir bloque
                                        fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[b] , usuariologueadop.startBlocks), SEEK_SET);
                                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                        //Reescribir bloque de punteros
                                        fseek(archivoDisco , getBlockStart_2(pb1.pb_pointers[j], usuariologueadop.startBlocks) , SEEK_SET);
                                        fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                                        //Reescribir BitMap
                                        escribirBitMap(archivoDisco , sp_aux);
                                        return 1;
                                    }else{
                                        //Ya hay bloque
                                        fseek(archivoDisco,getBlockStart_2(pb2.pb_pointers[b],usuariologueadop.startBlocks),SEEK_SET);
                                        fread(&bloqueContenido,sizeof (contentBlock),1,archivoDisco);
                                        for(int a = 0 ; a < 4 ; a++){
                                            if(bloqueContenido.contenapunta[a].f_inode == -1){
                                                bloqueContenido.contenapunta[a].f_inode = niNodo;
                                                strcpy(bloqueContenido.contenapunta[a].f_name,pathVirtual);
                                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[b] , usuariologueadop.startBlocks), SEEK_SET);
                                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                                niNodoPos = niNodo;
                                                return 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if(i == 14){
                            //Triple Indirecto
                            fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i], usuariologueadop.startBlocks),SEEK_SET);
                            fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                            for(int j = 0 ; j < 16 ; j ++){
                                if(pb1.pb_pointers[j] == -1){
                                    //Crear Apuntador doble Indirecto
                                        //Hay que crear el bloque
                                        //Crear doble indirecto
                                        int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                                        if(niNodoPos_2 == -1){
                                            printf("Bloques llenos.\n ");
                                            return -1;
                                        }
                                        usuariologueadop.bm_Block[niNodoPos_2] = '3';
                                        //Crear indirecto
                                        int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);
                                        if(niNodoPos_3 == -1){
                                            printf("Bloques llenos.\n ");
                                            return -1;
                                        }
                                        usuariologueadop.bm_Block[niNodoPos_3] = '3';
                                        //Crear bloque
                                        int niNodoPos_4 = nextFree(usuariologueadop.bm_Block , 0);
                                        if(niNodoPos_4 == -1){
                                            printf("Bloques llenos.\n ");
                                            return -1;
                                        }
                                        usuariologueadop.bm_Block[niNodoPos_4] = '1';
                                        //Bloque de apuntadores
                                        for (int a = 0; a < 16; a++){
                                            pb2.pb_pointers[a] = -1;
                                        }
                                        //Bloque de apuntadores
                                        for (int a = 0; a < 16; a++){
                                            pb3.pb_pointers[a] = -1;
                                        }
                                        //Bloque Directo
                                        for(int a = 1; a < 4; a++){
                                            bloqueContenido.contenapunta[a].f_inode = -1;
                                            memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                        }
                                        //iNodo de archivo/carpeta
                                        bloqueContenido.contenapunta[0].f_inode = niNodo;
                                        //Nombre de archivo/carpeta
                                        strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                        //Escribirle posicion de bloque a inodo
                                        pb3.pb_pointers[0] = niNodoPos_4;
                                        //Escribirle posicion de bloque a inodo
                                        pb2.pb_pointers[0] = niNodoPos_3;
                                        //Escribirle posicion de bloque a inodo
                                        pb1.pb_pointers[j] = niNodoPos_2;
                                        //Escribir bloque
                                        fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                        //Reescribir bloque de punteros
                                        fseek(archivoDisco , getBlockStart_2(pb2.pb_pointers[0], usuariologueadop.startBlocks) , SEEK_SET);
                                        fwrite(&pb3, sizeof(pointerBlock) , 1 , archivoDisco);
                                        //Reescribir bloque de punteros
                                        fseek(archivoDisco , getBlockStart_2(pb1.pb_pointers[j], usuariologueadop.startBlocks) , SEEK_SET);
                                        fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                                        //Reescribir bloque de punteros
                                        fseek(archivoDisco , getBlockStart_2(inodoActual.i_block[i], usuariologueadop.startBlocks) , SEEK_SET);
                                        fwrite(&pb1, sizeof(pointerBlock) , 1 , archivoDisco);
                                        //Reescribir BitMap
                                        escribirBitMap(archivoDisco , sp_aux);
                                        return 1;
                                }else{
                                    //Ya existe apuntador doble indirecto
                                    fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j], usuariologueadop.startBlocks),SEEK_SET);
                                    fread(&pb2, sizeof(pointerBlock),1,archivoDisco);
                                    for(int b = 0 ; b < 16 ; b++){
                                        if(pb2.pb_pointers[b] == -1){
                                            //Crear apuntador indirecto
                                            //Hay que crear el bloque
                                            //Crear indirecto
                                            int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                                            if(niNodoPos_2 == -1){
                                                printf("Bloques llenos.\n ");
                                                return -1;
                                            }
                                            usuariologueadop.bm_Block[niNodoPos_2] = '3';
                                            //Crear bloque
                                            int niNodoPos_3 = nextFree(usuariologueadop.bm_Block , 0);
                                            if(niNodoPos_3 == -1){
                                                printf("Bloques llenos.\n ");
                                                return -1;
                                            }
                                            usuariologueadop.bm_Block[niNodoPos_3] = '1';
                                            //Bloque de apuntadores
                                            for (int a = 0; a < 16; a++){
                                                pb3.pb_pointers[a] = -1;
                                            }
                                            //Bloque Directo
                                            for(int a = 1; a < 4; a++){
                                                bloqueContenido.contenapunta[a].f_inode = -1;
                                                memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                            }
                                            //iNodo de archivo/carpeta
                                            bloqueContenido.contenapunta[0].f_inode = niNodo;
                                            //Nombre de archivo/carpeta
                                            strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                            //Escribirle posicion de bloque a inodo
                                            pb3.pb_pointers[0] = niNodoPos_3;
                                            //Escribirle posicion de bloque a inodo
                                            pb2.pb_pointers[b] = niNodoPos_2;
                                            //Escribir bloque
                                            fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                            //Reescribir bloque de punteros
                                            fseek(archivoDisco , getBlockStart_2(pb2.pb_pointers[b], usuariologueadop.startBlocks) , SEEK_SET);
                                            fwrite(&pb3, sizeof(pointerBlock) , 1 , archivoDisco);
                                            //Reescribir bloque de punteros
                                            fseek(archivoDisco , getBlockStart_2(pb1.pb_pointers[j], usuariologueadop.startBlocks) , SEEK_SET);
                                            fwrite(&pb2, sizeof(pointerBlock) , 1 , archivoDisco);
                                            //Reescribir BitMap
                                            escribirBitMap(archivoDisco , sp_aux);
                                            return 1;
                                        }else{
                                            //YA existe apuntador indirecto
                                            fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[b], usuariologueadop.startBlocks),SEEK_SET);
                                            fread(&pb3, sizeof(pointerBlock),1,archivoDisco);
                                            for(int c = 0; c < 16 ; c++){
                                                if(pb3.pb_pointers[c] == -1){
                                                    //Hay que crear el bloque
                                                    //Crear bloque
                                                    int niNodoPos_2 = nextFree(usuariologueadop.bm_Block , 0);
                                                    if(niNodoPos_2 == -1){
                                                        printf("Bloques llenos.\n ");
                                                        return -1;
                                                    }
                                                    //Bloque Directo
                                                    usuariologueadop.bm_Block[niNodoPos_2] = '1';
                                                    for(int a = 1; a < 4; a++){
                                                        bloqueContenido.contenapunta[a].f_inode = -1;
                                                        memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                                    }
                                                    //iNodo de archivo/carpeta
                                                    bloqueContenido.contenapunta[0].f_inode = niNodo;
                                                    //Nombre de archivo/carpeta
                                                    strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                                    //Escribirle posicion de bloque a inodo
                                                    pb3.pb_pointers[c] = niNodoPos_2;
                                                    //Escribir bloque
                                                    fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[c] , usuariologueadop.startBlocks), SEEK_SET);
                                                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                                    //Reescribir bloque de punteros
                                                    fseek(archivoDisco , getBlockStart_2(pb2.pb_pointers[b], usuariologueadop.startBlocks) , SEEK_SET);
                                                    fwrite(&pb3, sizeof(pointerBlock) , 1 , archivoDisco);
                                                    //Reescribir BitMap
                                                    escribirBitMap(archivoDisco , sp_aux);
                                                    return 1;
                                                }else{
                                                    //Ya existe bloque
                                                    fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[c],usuariologueadop.startBlocks), SEEK_SET);
                                                    fread(&bloqueContenido, sizeof(contentBlock),1,archivoDisco);
                                                    for(int a = 0; a < 16 ; a++){
                                                        if(bloqueContenido.contenapunta[a].f_inode == -1){
                                                            bloqueContenido.contenapunta[a].f_inode = niNodo;
                                                            strcpy(bloqueContenido.contenapunta[a].f_name, pathVirtual);
                                                            fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[c], usuariologueadop.startBlocks),SEEK_SET);
                                                            fwrite(&bloqueContenido,sizeof(contentBlock),1,archivoDisco);
                                                            niNodoPos = niNodo;
                                                            return 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                        }
                    }
                }
            }
        }
    }
    return -1;
}
int modifyiNodeCarpet(int position,  int start, FILE *archivoDisco , int end, int genPadres){
    //Start = Inicio de tabla de inodos en este contexto
    int avance= 0;
    inode inodoM;
    fseek(archivoDisco , getiNodeStart_2(position , start), SEEK_SET);
    fread(&inodoM, sizeof(inode) , 1,  archivoDisco);
    if(end == -1){
        //Verificar permisos
        if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'w') == -1){
            printf("\033[101m ERROR \033[0m: No tiene permisos de escritura para realizar esta accion\n");
            printf("\033[101m ERROR \033[0m: No tiene permisos de escritura para realizar esta accion\n");

            exitoArchivo = false;
            return 1;
        }
        //return 1; -> No tiene permisos
        usuariologueadop.bm_iNode[position] = '0';
    }
    if(end != 1){
        if( !(inodoM.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1)){
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos para manipular esta carpeta\n");
            exitoArchivo = false;
            return -1;
        }
    }
    for(int a = 0; a < 15 ; a++){
        if(inodoM.i_block[a] == -1){
            continue;
        }if(genPadres == 1){
            avance += modifyBlockCarpet(inodoM.i_block[a] , usuariologueadop.startBlocks , archivoDisco , end, a);
        }
    }
    fseek(archivoDisco, getiNodeStart_2(position , start), SEEK_SET);
    if(end == -1 && avance == 0){
        char auximp = '\0';
        for(int a = 0; a < sizeof(inode) ; a++){
            fwrite(&auximp , 1 ,1 , archivoDisco);
        }
    }else{
        inodoM.i_perm = end;
        fwrite(&inodoM, sizeof(inode) , 1 , archivoDisco);
    }
    return avance;
}
int modifyBlockCarpet(int position, int start, FILE *archivoDisco , int end, int nap){
    //Start = inicio de bloques en este contexto
    int avance = 0;
    fseek(archivoDisco , getBlockStart_2(position, start), SEEK_SET);
    if(usuariologueadop.bm_Block[position] == '1'){
        contentBlock bloqueContenido;
        fread(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
        int infor = 0;
        if(nap == 0){
            infor = 2;
        }for(int a = infor; a < 4; a++){
            if(bloqueContenido.contenapunta[a].f_inode == -1){
                continue;
            }
            avance += modifyiNodeCarpet(bloqueContenido.contenapunta[a].f_inode ,usuariologueadop.startiNodes , archivoDisco, end, 1);
        }
    }
    else if(usuariologueadop.bm_Block[position] == '3'){
        pointerBlock pb;
        fread(&pb, sizeof(pointerBlock), 1 , archivoDisco);
        for(int a = 0; a < 16; a++){
            if(pb.pb_pointers[a] == -1){
                continue;
            }
            avance  += modifyBlockCarpet(pb.pb_pointers[a] , start , archivoDisco , end, 1);
        }
    }
    if(end == -1 && avance == 0){
        usuariologueadop.bm_Block[position] = '0';
        char auximp = '\0';
        fseek(archivoDisco , getBlockStart_2(position , start) , SEEK_SET);
        for(int a = 0; a < sizeof(fileBlock) ; a++){
            fwrite(&auximp , 1 ,1 , archivoDisco);
        }
    }
    return avance;
}
int findUser(FILE *archivoDisco){
    userstats grupos[200];
    userstats users[200];
    int nGrupos = 0;
    int nusers = 0;
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
    if(sp_aux.sb_magic == 61267){
        archivo archusers;
        archusers = getArchivo("users.txt",usuariologueadop.partUser,sp_aux.sb_t_inode, sp_aux.sb_t_inode,archivoDisco);
        if(archusers.estado != -1){
            if(archivoDisco != NULL){
                int blockStart = sp_aux.sb_bm_Block;
                int blockCount = sp_aux.sb_blocks_count;

                char bitmapBlock[270000] = {0};
                char bitAux;
                //Cargar mapa de bits de bloques
                fseek(archivoDisco, blockStart, SEEK_SET);
                for( int i = 0; i < blockCount ; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapBlock[i] = bitAux;
                    bitmapBlock[i+1] = '\0';
                }
                blockStart = sp_aux.sb_bm_inode;
                blockCount = sp_aux.sb_inodes_count;
                fseek(archivoDisco,blockStart,SEEK_SET);
                char bitmapiNode[270000] = {0};
                for( int i = 0; i < blockCount; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapiNode[i] = bitAux;
                    bitmapiNode[i+1] = '\0';
                }

                char aux[9999] = {0};
                char aux2[9999] = {0};
                char texto_archivo[999];
                char grupo[999] = {0};
                char com[4][1000];

                strcpy(texto_archivo,archusers.contenido);
                int i = 0;
                int length = 0;
                int comas = 0;
                int comillas = 0;
                int status = 0;
                int id_user;
                while (texto_archivo[i] != NULL) {
                    char aux_char[] = {texto_archivo[i],'\0'};
                    if(texto_archivo[i] == '\n'){
                        strcpy(aux2,"");
                        comillas = 0;
                        for(int j = 0; j < length; j++){
                            char aux_char_2[] = {aux[j],'\0'};
                            if(aux[j] == ','){
                                strcpy(com[comillas],aux2);
                                comillas++;
                                strcpy(aux2,"");
                            }else{
                                strcat(aux2, aux_char_2);
                            }
                        }
                        if(comas == 2){
                            grupos[nGrupos].numGrupo = atoi(com[0]);
                            strcpy(grupos[nGrupos].name,aux2);
                            nGrupos++;
                        }else if(comas == 4){
                            id_user = atoi(com[0]);
                            users[nusers].numGrupo = atoi(com[0]);
                            strcpy(users[nusers].group, com[2]);
                            strcpy(users[nusers].name, com[3]);
                            nusers++;
                            strcpy(grupo, com[2]);
                        }
                        strcpy(aux,"");
                        comas = 0;
                        length = 0;
                    }else{
                        strcat(aux,aux_char);
                        length++;
                        if(texto_archivo[i] == ','){
                            comas++;
                        }
                    }
                    i++;
                }
            }else{

            }
        }else{
            //Algo salio mal
        }
        globNusers = 0;
        for(int i = 0; i < nusers ; i++){
            globusers[i] = users[i];
            globNusers++;
        }
        globNGrupos = 0;
        for(int i = 0 ; i < nGrupos ; i++){
            globGrupos[i] = grupos[i];
            globNGrupos++;
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int findUser2(PMOUNT particionTrabajo, FILE* archivoDisco){
    userstats grupos[200];
    userstats users[200];
    int nGrupos = 0;
    int nusers = 0;
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
    if(sp_aux.sb_magic == 61267){
        archivo archusers;
        archusers = getArchivo("users.txt",particionTrabajo,sp_aux.sb_t_inode, sp_aux.sb_t_inode,archivoDisco);
        if(archusers.estado != -1){
            if(archivoDisco != NULL){
                int blockStart = sp_aux.sb_bm_Block;
                int blockCount = sp_aux.sb_blocks_count;

                char bitmapBlock[270000] = {0};
                char bitAux;
                //Cargar mapa de bits de bloques
                fseek(archivoDisco, blockStart, SEEK_SET);
                for( int i = 0; i < blockCount ; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapBlock[i] = bitAux;
                    bitmapBlock[i+1] = '\0';
                }
                blockStart = sp_aux.sb_bm_inode;
                blockCount = sp_aux.sb_inodes_count;
                fseek(archivoDisco,blockStart,SEEK_SET);
                char bitmapiNode[270000] = {0};
                for( int i = 0; i < blockCount; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapiNode[i] = bitAux;
                    bitmapiNode[i+1] = '\0';
                }

                char aux[9999] = {0};
                char aux2[9999] = {0};
                char texto_archivo[999];
                char grupo[999] = {0};
                char com[4][1000];

                strcpy(texto_archivo,archusers.contenido);
                int i = 0;
                int length = 0;
                int comas = 0;
                int comillas = 0;
                int status = 0;
                int id_user;
                while (texto_archivo[i] != NULL) {
                    char aux_char[] = {texto_archivo[i],'\0'};
                    if(texto_archivo[i] == '\n'){
                        strcpy(aux2,"");
                        comillas = 0;
                        for(int j = 0; j < length; j++){
                            char aux_char_2[] = {aux[j],'\0'};
                            if(aux[j] == ','){
                                strcpy(com[comillas],aux2);
                                comillas++;
                                strcpy(aux2,"");
                            }else{
                                strcat(aux2, aux_char_2);
                            }
                        }
                        if(comas == 2){
                            grupos[nGrupos].numGrupo = atoi(com[0]);
                            strcpy(grupos[nGrupos].name,aux2);
                            nGrupos++;
                        }else if(comas == 4){
                            id_user = atoi(com[0]);
                            users[nusers].numGrupo = atoi(com[0]);
                            strcpy(users[nusers].group, com[2]);
                            strcpy(users[nusers].name, com[3]);
                            nusers++;
                            strcpy(grupo, com[2]);
                        }
                        strcpy(aux,"");
                        comas = 0;
                        length = 0;
                    }else{
                        strcat(aux,aux_char);
                        length++;
                        if(texto_archivo[i] == ','){
                            comas++;
                        }
                    }
                    i++;
                }
            }else{

            }
        }else{
            //Algo salio mal
        }
        globNusers = 0;
        for(int i = 0; i < nusers ; i++){
            globusers[i] = users[i];
            globNusers++;
        }
        globNGrupos = 0;
        for(int i = 0 ; i < nGrupos ; i++){
            globGrupos[i] = grupos[i];
            globNGrupos++;
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int writeJournal(journal entrada, FILE *archivoDisco, int position)
{
    int start = usuariologueadop.partUser.mount_start + sizeof(SuperBloque) + position * sizeof(journal);
    fseek(archivoDisco, start, SEEK_SET);
    fwrite(&entrada, sizeof(journal) , 1 , archivoDisco);
    return 0;
}
int cleanArchivo(int start, char bm_Block_new[], int init_inode, int init_bm_Blocks, FILE* archivoDisco){
    //vacia inodos
    int start_2 = start;
    usuariologueadop.bm_iNode[start_2] = '0';
    start = getiNodeStart_2(start, usuariologueadop.startiNodes);
    char clean[64] = {0};
    SuperBloque sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic == 61267){
        if(archivoDisco != NULL){
            inode inodoM;
            fseek(archivoDisco, start, SEEK_SET);
            fread(&inodoM, sizeof (inode), 1, archivoDisco);

            char timeAux[16] = {0};
            strcpy(timeAux,inodoM.i_ctime);
            pointerBlock pb1;
            pointerBlock pb2;
            pointerBlock pb3;
            for ( int i = 0 ; i < 15 ; i++ ) {
                if(inodoM.i_block[i] == -1){
                    break;
                }
                if( i >= 12 ){
                    fseek(archivoDisco, getBlockStart_2(inodoM.i_block[i], usuariologueadop.startBlocks),SEEK_SET);
                    fread(&pb1,sizeof (pointerBlock),1,archivoDisco);
                    for( int j = 0 ; j < 16 ; j++ ){
                        if(pb1.pb_pointers[j] == -1 || pb1.pb_pointers[j] == 0){
                            break;
                        }
                        fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                        if(i == 12){
                            bm_Block_new[pb1.pb_pointers[j]] = '0';
                            fwrite(&clean,sizeof(clean),1,archivoDisco);
                        }else{
                            fread(&pb2,sizeof (pointerBlock),1,archivoDisco);
                            for ( int k = 0 ; k < 16 ; k++ ){
                                if(pb2.pb_pointers[k] == -1 || pb1.pb_pointers[j] == 0){
                                    break;
                                }
                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k],usuariologueadop.startBlocks),SEEK_SET);
                                if(i==13){
                                    bm_Block_new[pb2.pb_pointers[k]] = '0';
                                    fwrite(&clean,sizeof (clean),1,archivoDisco);
                                }else{
                                    //i == 14
                                    fread(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                    for( int l = 0 ; l < 16 ; l++ ){
                                        if(pb3.pb_pointers[l] == -1 || pb1.pb_pointers[j] == 0){
                                            break;
                                        }
                                        bm_Block_new[pb3.pb_pointers[l]] = '0';
                                        fwrite(&clean,sizeof (clean),1,archivoDisco);
                                    }
                                    bm_Block_new[pb2.pb_pointers[k]] = '0';
                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k], usuariologueadop.startBlocks),SEEK_SET);
                                    fwrite(&clean,sizeof(clean),1,archivoDisco);
                                }
                            }
                            bm_Block_new[pb1.pb_pointers[j]] = '0';
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j], usuariologueadop.startBlocks),SEEK_SET);
                            fwrite(&clean,sizeof(clean),1,archivoDisco);
                        }
                    }
                    bm_Block_new[inodoM.i_block[i]] = '0';
                    fseek(archivoDisco, getBlockStart_2(inodoM.i_block[i], usuariologueadop.startBlocks),SEEK_SET);
                    fwrite(&clean,sizeof(clean),1,archivoDisco);
                }else{
                    bm_Block_new[inodoM.i_block[i]] = '0';
                    fseek(archivoDisco, getBlockStart_2(inodoM.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                    fwrite(&clean, sizeof (clean),1,archivoDisco);
                }
                inodoM.i_block[i] = -1;
            }
            char auxImpresion = '\0';
            fseek(archivoDisco, start, SEEK_SET);
            for( int i = 0 ; i < sizeof (inode) ; i ++){
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
            fseek(archivoDisco, sp_aux.sb_bm_inode, SEEK_SET);
            for (int i = 0 ; i < strlen(usuariologueadop.bm_iNode) ; i++ ) {
                auxImpresion = usuariologueadop.bm_iNode[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
            fseek(archivoDisco, init_bm_Blocks, SEEK_SET);
            for ( int i = 0 ; i < strlen(bm_Block_new) ; i++ ) {
                auxImpresion = bm_Block_new[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
            strcpy(usuariologueadop.bm_Block, bm_Block_new);
        }else {
            //Algo salio mal
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int fillArchivo(int start, char nuevo_texto[], char bm_Block_new[], int init_inode, int init_bm_Blocks, char creacion[], int permisos, FILE *archivoDisco){
    //llena inodos
    //Start es la posicion donde esta el inodo
    usuariologueadop.bm_iNode[start] = '1';
    archivoPieza separados = contentSpliter(nuevo_texto);
    SuperBloque sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic == 61267){
        if(archivoDisco != NULL){
            inode inodoM;
            fseek(archivoDisco, getiNodeStart_2(start, usuariologueadop.startiNodes) , SEEK_SET);
            fread(&inodoM, sizeof(inode), 1, archivoDisco);

            inodoM.i_uid = usuariologueadop.id_user;
            inodoM.i_gid = usuariologueadop.id_grupo;
            inodoM.i_size = strlen(nuevo_texto);
            time_t tiempoActual = time(NULL);
            struct tm *fecha = localtime(&tiempoActual);
            strcpy(inodoM.i_ctime,creacion);
            strftime(inodoM.i_atime, sizeof(inodoM.i_atime)-1, "%d/%m/%y %H:%M", fecha);
            strftime(inodoM.i_mtime, sizeof(inodoM.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
            inodoM.i_type = '1';
            inodoM.i_perm = permisos;
            for(int i = 0; i<15;i++){
                inodoM.i_block[i] = -1;
            }

            int nEscritos = 0;
            int va0 = 0;
            fileBlock archivo1;
            fileBlock archivo2;
            pointerBlock pb1;
            pointerBlock pb2;
            pointerBlock pb3;
            int va1;
            int va2;
            int va3;
            int preFree = nextFree(bm_Block_new, 0);
            int preUsed = nextUsed(bm_Block_new, preFree);
            int preLast = lastUsed(bm_Block_new, 0);
            va0 = preLast;
            while(preUsed < preLast && preUsed != -1){
                if( ((preUsed-preFree) >= separados.n) ){
                    //llenar desde adelante (Primer espacio donde entra)
                    va0 = preFree -1;
                    break;
                }else{
                    //llenar desde atras
                    va0 = lastUsed(bm_Block_new,0);
                }
                preFree = nextFree(bm_Block_new, preUsed);
                preUsed = nextUsed(bm_Block_new, preFree);
            }
            for(int j = 0 ; j < 15 ; j ++){
                if(!(nEscritos < separados.n)){
                    break;
                }
                va0 = nextFree(bm_Block_new, va0);
                if(va0 == -1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Todos los bloques estan ocupados \n");
                    fclose(archivoDisco);
                    return 0;
                }
                inodoM.i_block[j] = va0;
                fseek(archivoDisco , getBlockStart_2(va0 , usuariologueadop.startBlocks) , SEEK_SET);
                if(j < 12){
                    bm_Block_new[va0] = '2';
                    fwrite(&separados.contenido[nEscritos].fcontenapunta, sizeof(fileBlock) , 1 , archivoDisco);
                    nEscritos++;
                }
                else{
                    for(int k = 0; k < 16; k++) {
                        pb1.pb_pointers[k] = -1;
                    }
                    bm_Block_new[va0] = '3';
                    va1 = getBlockStart_2(va0 , usuariologueadop.startBlocks);
                    for(int c = 0; c < 16; c++){
                        if(!(nEscritos < separados.n)){
                            break;
                        }
                        va0 = nextFree(bm_Block_new, va0);
                        if( va0 == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Todos los bloques estan ocupados \n");
                            return 0;
                        }
                        pb1.pb_pointers[c] = va0;
                        fseek(archivoDisco , getBlockStart_2(va0  , usuariologueadop.startBlocks), SEEK_SET);
                        if(j == 12){
                            bm_Block_new[va0] = '2';
                            fwrite(&separados.contenido[nEscritos].fcontenapunta, sizeof(fileBlock) , 1 , archivoDisco);
                            nEscritos++;
                        }
                        else{
                            for(int k = 0; k < 16; k++){
                                pb2.pb_pointers[k] = -1;
                            }
                            bm_Block_new[va0] = '3';
                            va2 = getBlockStart_2(va0 , usuariologueadop.startBlocks);
                            for(int d = 0; d < 16; d++){
                                if(!(nEscritos < separados.n)){
                                    break;
                                }
                                va0 = nextFree(bm_Block_new, va0);
                                if( va0 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Todos los bloques estan ocupados \n");
                                    return 0;
                                }
                                pb2.pb_pointers[d] = va0;
                                fseek(archivoDisco , getBlockStart_2(va0  , usuariologueadop.startBlocks), SEEK_SET);
                                if(j == 13){
                                    bm_Block_new[va0] = '2';
                                    fwrite(&separados.contenido[nEscritos].fcontenapunta, sizeof(fileBlock) , 1 , archivoDisco);
                                    nEscritos++;
                                }
                                else{
                                    for(int k =0; k < 16; k++){
                                        pb3.pb_pointers[k] = -1;
                                    }
                                    bm_Block_new[va0] = '3';
                                    va3 = getBlockStart_2(va0, usuariologueadop.startBlocks);
                                    for(int e = 0; e < 16; e++){
                                        if(!(nEscritos < separados.n)){
                                            break;
                                        }
                                        va0 = nextFree(bm_Block_new, va0);
                                        if( va0 == -1) {
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Todos los bloques estan ocupados \n");
                                            return 0;
                                        }
                                        pb3.pb_pointers[e] = va0;
                                        fseek(archivoDisco , getBlockStart_2(va0  , usuariologueadop.startBlocks), SEEK_SET);
                                        bm_Block_new[va0] = '2';
                                        fwrite(&separados.contenido[nEscritos].fcontenapunta, sizeof(fileBlock) , 1 , archivoDisco);
                                        nEscritos++;
                                    }
                                    fseek(archivoDisco, va3 , SEEK_SET);
                                    fwrite(&pb3, sizeof(pointerBlock), 1 , archivoDisco);
                                }
                            }
                            fseek(archivoDisco, va2 , SEEK_SET);
                            fwrite(&pb2, sizeof(pointerBlock), 1 , archivoDisco);
                        }
                    }
                    fseek(archivoDisco, va1 , SEEK_SET);
                    fwrite(&pb1, sizeof(pointerBlock), 1 , archivoDisco);
                }
            }

            strcpy(usuariologueadop.bm_Block, bm_Block_new);
            fseek(archivoDisco, getiNodeStart_2(start, usuariologueadop.startiNodes), SEEK_SET);
            fwrite(&inodoM, sizeof (inode),1,archivoDisco);

            char auxImpresion = '\0';
            fseek(archivoDisco, sp_aux.sb_bm_Block, SEEK_SET);
            for ( int i = 0 ; i < strlen(usuariologueadop.bm_Block) ; i++ ) {
                auxImpresion = usuariologueadop.bm_Block[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
            fseek(archivoDisco, sp_aux.sb_bm_inode, SEEK_SET);
            for ( int i = 0 ; i < strlen(usuariologueadop.bm_iNode) ; i++ ) {
                auxImpresion = usuariologueadop.bm_iNode[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
        }else{
            //Algo salio mal
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int modificarArchivo(int start, char nuevoText[], char viejoTexto[], FILE *archivoDisco){
    int starter = 0;
    int cantidad = 0;
    SuperBloque sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic == 61267){
        if(archivoDisco != NULL){
            inode inodoM;

            starter = sp_aux.sb_bm_Block;
            cantidad = sp_aux.sb_blocks_count;

            char bm_Block_new[cantidad];
            char aux_c;
            //Cargar mapa de bits de bloque
            fseek(archivoDisco, starter,SEEK_SET);
            for( int i = 0 ; i < cantidad ; i++){
                fread(&aux_c,1,1,archivoDisco);
                bm_Block_new[i] = aux_c;
            }
            fseek(archivoDisco, getiNodeStart_2(start, sp_aux.sb_t_inode),SEEK_SET);
            fread(&inodoM, sizeof (inode),1,archivoDisco);
            //Vaciar contenido del archivo deseado
            cleanArchivo(start, bm_Block_new, sp_aux.sb_t_inode, sp_aux.sb_bm_Block, archivoDisco);
            //Cargar nuevo mapa de bits de bloque
            fseek(archivoDisco, starter,SEEK_SET);
            for( int i = 0 ; i < cantidad ; i++){
                fread(&aux_c,1,1,archivoDisco);
                bm_Block_new[i] = aux_c;
            }
            fseek(archivoDisco, getiNodeStart_2(start, usuariologueadop.startiNodes), SEEK_SET);
            //Volver a llenar archivo
            fillArchivo(start, nuevoText, bm_Block_new, sp_aux.sb_t_inode, sp_aux.sb_bm_Block, inodoM.i_ctime, inodoM.i_perm, archivoDisco);
            return 1;
        }else{
            //Algo salio mal
        }
    }else {
        //Algo salio mal
    }
    return 0;
}
int escribirBitMap(FILE *archivoDisco, SuperBloque sp_aux){
    char aux_impresion;
    //Escribir Bit maps en archivo disco
    //BitMap de Bloques
    fseek(archivoDisco , sp_aux.sb_bm_Block, SEEK_SET);
    for(int i = 0; i < strlen(usuariologueadop.bm_Block); i++)
    {
        aux_impresion = usuariologueadop.bm_Block[i];
        fwrite(&aux_impresion, 1, 1 , archivoDisco);
    }
    //BitMap de iNodos
    fseek(archivoDisco, sp_aux.sb_bm_inode , SEEK_SET);
    for(int i = 0; i < strlen(usuariologueadop.bm_iNode); i++)
    {
        aux_impresion = usuariologueadop.bm_iNode[i];
        fwrite(&aux_impresion, 1, 1 , archivoDisco);
    }
    return 1;
}
archivo obtenerContent(char pathArchivo[]){
    //Archivo final
    archivo finalFile;
    finalFile.estado = -1;
    memset(&finalFile.contenido,0,280320);
    //Archivo de donde se va a extraer la informacion
    FILE *archivoFisico;
    archivoFisico = fopen(pathArchivo,"rb");
    if(archivoFisico != NULL){
        //Existe el archivo
        char aux_c;
        int length = -1;
        fseek(archivoFisico,0,SEEK_END);
        length = ftell(archivoFisico);
        fseek(archivoFisico,0,SEEK_SET);
        for(int i = 0; i < length ; i++){
            //Leer contenido bajo length
            fread(&aux_c,1,1,archivoFisico);
            sprintf(finalFile.contenido,"%s%c",finalFile.contenido,aux_c);
        }
        fclose(archivoFisico);
    }else{  //\033[1;93m
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[43m ADVERTENCIA \033[0m: Solo es contenido del archivo '%s'\n",pathArchivo);

    }
    return finalFile;
}
int crearCarpetaSub(int iNodeNumber,  int iNodeFather, FILE * archivoDisco){
    if(iNodeNumber != -1){
        if(archivoDisco != NULL){
            //llenar bloque de iNodo carpeta
            contentBlock bloqueContenido;
            bloqueContenido.contenapunta[0].f_inode = iNodeNumber;
            memset(&bloqueContenido.contenapunta[0].f_name,0,12);
            strcpy(bloqueContenido.contenapunta[0].f_name , ".");
            bloqueContenido.contenapunta[1].f_inode = iNodeFather;
            memset(&bloqueContenido.contenapunta[1].f_name,0,12);
            strcpy(bloqueContenido.contenapunta[1].f_name , "..");
            //Vaciar resto de bloque carpeta
            for(int i = 2; i < 4; i++){
                bloqueContenido.contenapunta[i].f_inode = -1;
                memset(&bloqueContenido.contenapunta[i].f_name,0,12);
            }
            //Generar iNodo para bloque carpeta
            inode inodeM;
            int siguienteBlockLibre = nextFree(usuariologueadop.bm_Block , 0);
            if(siguienteBlockLibre == -1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                return -1;
            }
            //Asignar a bitmap bloque Carpeta e iNodo
            usuariologueadop.bm_Block[siguienteBlockLibre] = '1';
            usuariologueadop.bm_iNode[iNodeNumber] = '1';
            //Datos para iNodo carpeta
            inodeM.i_uid = usuariologueadop.id_user;
            inodeM.i_gid = usuariologueadop.id_grupo;
            inodeM.i_size = 0;
            time_t tiempoActual = time(NULL);
            struct tm *fecha = localtime(&tiempoActual);
            strftime(inodeM.i_ctime, sizeof(inodeM.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
            strftime(inodeM.i_atime, sizeof(inodeM.i_atime)-1, "%d/%m/%y %H:%M", fecha);
            strftime(inodeM.i_mtime, sizeof(inodeM.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
            inodeM.i_block[0] = siguienteBlockLibre;
            for(int i = 1 ; i < 15; i++){
                inodeM.i_block[i] = -1;
            }
            inodeM.i_type = '0';
            inodeM.i_perm = 664;
            //Buscar Super Bloque para escritura
            SuperBloque sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);

            if(sp_aux.sb_magic == 61267){
                //Ingresar iNodo
                fseek(archivoDisco, getiNodeStart_2(iNodeNumber, usuariologueadop.startiNodes) , SEEK_SET);
                fwrite(&inodeM , sizeof(inode) , 1 , archivoDisco);
                //Ingresar Bloque Contenido
                fseek(archivoDisco, getBlockStart_2(siguienteBlockLibre , usuariologueadop.startBlocks) , SEEK_SET);
                fwrite(&bloqueContenido , sizeof(contentBlock) , 1, archivoDisco);
                //llenar bitmap de archivo disco
                char aux_impresion;
                //llenar iNodos
                fseek(archivoDisco, sp_aux.sb_bm_inode , SEEK_SET);
                for(int i = 0; i < strlen(usuariologueadop.bm_iNode); i++){
                    aux_impresion = usuariologueadop.bm_iNode[i];
                    fwrite(&aux_impresion, 1, 1 , archivoDisco);
                }
                //llenar Bloques
                fseek(archivoDisco, sp_aux.sb_bm_Block , SEEK_SET);
                for(int i = 0; i < strlen(usuariologueadop.bm_Block)-1; i++){
                    aux_impresion = usuariologueadop.bm_Block[i];
                    fwrite(&aux_impresion, 1, 1 , archivoDisco);
                }
                //Termino proceso
                return 1;
            }
        }
    }
    return -1;
}
int crearCarpeta(char pathVirtual[] , inode inodoActual, int inodeNumber, FILE *archivoDisco)
{
    if(archivoDisco != NULL){
        //Bloques indiretos
        pointerBlock pb1;
        pointerBlock pb2;
        pointerBlock pb3;
        //Bloque de contenido
        contentBlock bloqueContenido;
        for(int i = 0; i < 15 ; i++){
            if(inodoActual.i_block[i] == -1){
                //Siguiente bloque libre
                int siguienteLibre = nextFree(usuariologueadop.bm_Block , 0);
                if(siguienteLibre == -1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                    return -1;
                }
                //Ir a siguiente bloque para lectura
                fseek(archivoDisco, getBlockStart_2(siguienteLibre , usuariologueadop.startBlocks) , SEEK_SET);
                if(i < 12 ){
                    //Hay que crear bloque
                    //Asignar a bitmap nuevo bloque
                    usuariologueadop.bm_Block[siguienteLibre] = '1';
                    //Obtener iNodo para bloque carpeta
                    int siguienteLibre_2 = nextFree(usuariologueadop.bm_iNode,0);
                    if(siguienteLibre_2 == -1){
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                        usuariologueadop.bm_Block[siguienteLibre] = '0';
                        return -1;
                    }
                    if(crearCarpetaSub(siguienteLibre_2 , inodeNumber, archivoDisco) == -1){
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                        usuariologueadop.bm_Block[siguienteLibre] = '0';
                        return -1;
                    }
                    //Vaciar bloque carpeta
                    for(int j = 1; j < 4; j++){
                        bloqueContenido.contenapunta[j].f_inode = -1;
                        strcpy(bloqueContenido.contenapunta[j].f_name, "");
                    }
                    //Asignar a iNodo nuevo bloque carpeta
                    inodoActual.i_block[i] = siguienteLibre;
                    //Asignar iNodo a bloque carpeta
                    strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                    bloqueContenido.contenapunta[0].f_inode = siguienteLibre_2;
                    //Escribir bloque contenido
                    fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                    //Reescribir iNodo
                    fseek(archivoDisco , getiNodeStart_2(inodeNumber, usuariologueadop.startiNodes) , SEEK_SET);
                    fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                    //Termino el proceso
                    printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                    return siguienteLibre_2; //devuelve iNodo
                }else{
                    if(i == 12){
                        //Indirecto
                        //Hay que crear indirecto
                        //Asignar a bitmap nuevo bloque indirecto
                        usuariologueadop.bm_Block[siguienteLibre] = '3';
                        //Obtener Bloque para siguiente bloque carpeta
                        int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0);
                        if(siguienteLibre_2 == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                            return -1;
                        }
                        //Asignar a bitmap nuevo bloque carpeta
                        usuariologueadop.bm_Block[siguienteLibre_2] = '1';
                        //Obtener iNodo para siguiente bloque carpeta
                        int siguienteLibre_3 = nextFree(usuariologueadop.bm_iNode,0);
                        if(siguienteLibre_3 == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                            return -1;
                        }
                        if(crearCarpetaSub(siguienteLibre_3 , inodeNumber, archivoDisco) == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                            return -1;
                        }
                        //Vaciar bloque indirecto
                        for( int a = 0 ; a < 16 ; a++){
                            pb1.pb_pointers[a] = -1;
                        }
                        //Vaciar bloque carpeta
                        for(int j = 1; j < 4; j++){
                            bloqueContenido.contenapunta[j].f_inode = -1;
                            memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                        }
                        //Asignar a iNodo, bloque de punteros
                        inodoActual.i_block[i] = siguienteLibre;
                        //Asignar a bloque de punteros, bloque carpeta
                        pb1.pb_pointers[0] = siguienteLibre_2;
                        //Asignar iNodo a bloque carpeta
                        strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                        bloqueContenido.contenapunta[0].f_inode = siguienteLibre_3;
                        //Escribir bloque contenido
                        fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                        //Escribir bloque de apuntadores
                        fseek(archivoDisco,getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                        fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                        //Reescribir iNodo
                        fseek(archivoDisco , getiNodeStart_2(inodeNumber, usuariologueadop.startiNodes) , SEEK_SET);
                        fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                        //Termino el proceso
                        printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                        return siguienteLibre_3; //devuelve iNodo
                    }else{
                        if(i == 13){
                            //Doble Indirecto
                            //Hay que crear indirecto
                            usuariologueadop.bm_Block[siguienteLibre] = '3';
                            //Obtener Bloque para siguiente bloque carpeta
                            int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0);
                            if(siguienteLibre_2 == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                                return -1;
                            }
                            //Asignar a bitmap nuevo bloque indirecto
                            usuariologueadop.bm_Block[siguienteLibre_2] = '3';
                            //Obtener Bloque para siguiente bloque carpeta
                            int siguienteLibre_3 = nextFree(usuariologueadop.bm_Block,0);
                            if(siguienteLibre_3 == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                                return -1;
                            }
                            //Asignar a bitmap nuevo bloque carpeta
                            usuariologueadop.bm_Block[siguienteLibre_3] = '1';
                            //Obtener iNodo para siguiente bloque carpeta
                            int siguienteLibre_4 = nextFree(usuariologueadop.bm_iNode,0);
                            if(siguienteLibre_4 == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                return -1;
                            }
                            if(crearCarpetaSub(siguienteLibre_4 , inodeNumber, archivoDisco) == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                return -1;
                            }
                            //Vaciar bloque indirecto
                            for( int a = 0 ; a < 16 ; a++){
                                pb2.pb_pointers[a] = -1;
                            }
                            //Vaciar bloque indirecto
                            for( int a = 0 ; a < 16 ; a++){
                                pb1.pb_pointers[a] = -1;
                            }
                            //Vaciar bloque carpeta
                            for(int j = 1; j < 4; j++){
                                bloqueContenido.contenapunta[j].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                            }
                            //Asignar a iNodo, bloque de punteros
                            inodoActual.i_block[i] = siguienteLibre;
                            //Asignar a bloque de punteros, bloque carpeta
                            pb1.pb_pointers[0] = siguienteLibre_2;
                            //Asignar a bloque de punteros, bloque carpeta
                            pb2.pb_pointers[0] = siguienteLibre_3;
                            //Asignar iNodo a bloque carpeta
                            strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                            bloqueContenido.contenapunta[0].f_inode = siguienteLibre_4;
                            //Escribir bloque contenido
                            fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                            //Escribir bloque de apuntadores
                            fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[0],usuariologueadop.startBlocks),SEEK_SET);
                            fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                            //Escribir bloque de apuntadores
                            fseek(archivoDisco,getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                            fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                            //Reescribir iNodo
                            fseek(archivoDisco , getiNodeStart_2(inodeNumber, usuariologueadop.startiNodes) , SEEK_SET);
                            fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                            //Termino el proceso
                            printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                            return siguienteLibre_4; //devuelve iNodo
                        }else{
                            if(i == 14){
                                //Triple Indirecto
                                //Hay que crear indirecto
                                usuariologueadop.bm_Block[siguienteLibre] = '3';
                                //Obtener Bloque para siguiente bloque carpeta
                                int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0);
                                if(siguienteLibre_2 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                                    return -1;
                                }
                                //Asignar a bitmap nuevo bloque indirecto
                                usuariologueadop.bm_Block[siguienteLibre_2] = '3';
                                //Obtener Bloque para siguiente bloque carpeta
                                int siguienteLibre_3 = nextFree(usuariologueadop.bm_Block,0);
                                if(siguienteLibre_3 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                                    return -1;
                                }
                                //Asignar a bitmap nuevo bloque indirecto
                                usuariologueadop.bm_Block[siguienteLibre_3] = '3';
                                //Obtener Bloque para siguiente bloque carpeta
                                int siguienteLibre_4 = nextFree(usuariologueadop.bm_Block,0);
                                if(siguienteLibre_4 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin bloques para escritura\n");
                                    return -1;
                                }
                                //Asignar a bitmap nuevo bloque carpeta
                                usuariologueadop.bm_Block[siguienteLibre_4] = '1';
                                //Obtener iNodo para siguiente bloque carpeta
                                int siguienteLibre_5 = nextFree(usuariologueadop.bm_iNode,0);
                                if(siguienteLibre_5 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                    return -1;
                                }
                                if(crearCarpetaSub(siguienteLibre_5 , inodeNumber, archivoDisco) == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                    return -1;
                                }
                                //Vaciar bloque indirecto
                                for( int a = 0 ; a < 16 ; a++){
                                    pb3.pb_pointers[a] = -1;
                                }
                                //Vaciar bloque indirecto
                                for( int a = 0 ; a < 16 ; a++){
                                    pb2.pb_pointers[a] = -1;
                                }
                                //Vaciar bloque indirecto
                                for( int a = 0 ; a < 16 ; a++){
                                    pb1.pb_pointers[a] = -1;
                                }
                                //Vaciar bloque carpeta
                                for(int j = 1; j < 4; j++){
                                    bloqueContenido.contenapunta[j].f_inode = -1;
                                    memset(&bloqueContenido.contenapunta[j].f_name,0,12);
                                }
                                //Asignar a iNodo, bloque de punteros
                                inodoActual.i_block[i] = siguienteLibre;
                                //Asignar a bloque de punteros, bloque carpeta
                                pb1.pb_pointers[0] = siguienteLibre_2;
                                //Asignar a bloque de punteros, bloque carpeta
                                pb2.pb_pointers[0] = siguienteLibre_3;
                                //Asignar a bloque de punteros, bloque carpeta
                                pb3.pb_pointers[0] = siguienteLibre_4;
                                //Asignar iNodo a bloque carpeta
                                strcpy(bloqueContenido.contenapunta[0].f_name, pathVirtual);
                                bloqueContenido.contenapunta[0].f_inode = siguienteLibre_5;
                                //Escribir bloque contenido
                                fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                //Escribir bloque de apuntadores
                                fseek(archivoDisco,getBlockStart_2(pb2.pb_pointers[0],usuariologueadop.startBlocks),SEEK_SET);
                                fwrite(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                //Escribir bloque de apuntadores
                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[0],usuariologueadop.startBlocks),SEEK_SET);
                                fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                //Escribir bloque de apuntadores
                                fseek(archivoDisco,getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                                fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                                //Reescribir iNodo
                                fseek(archivoDisco , getiNodeStart_2(inodeNumber, usuariologueadop.startiNodes) , SEEK_SET);
                                fwrite(&inodoActual, sizeof(inode) , 1 , archivoDisco);
                                //Termino el proceso
                                printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                return siguienteLibre_5; //devuelve iNodo
                            }
                        }
                    }
                }
            }
            if(i < 12 ){
                //Bloque ya existe
                fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                fread(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                for(int j = 0; j < 4; j++){
                    //Buscar Vacio
                    if(bloqueContenido.contenapunta[j].f_inode == -1){
                        //Obtener iNodo para siguiente bloque carpeta
                        int siguienteLibre_2 = nextFree(usuariologueadop.bm_iNode,0);
                        if(siguienteLibre_2 == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                            return -1;
                        }
                        if(crearCarpetaSub(siguienteLibre_2 , inodeNumber, archivoDisco) == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                            return -1;
                        }
                        //Asignar iNodo a bloque
                        bloqueContenido.contenapunta[j].f_inode = siguienteLibre_2;
                        strcpy(bloqueContenido.contenapunta[j].f_name , pathVirtual);
                        //Escribir el bloque nuevo
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                        //Termino el proceso
                        printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                        inodeNumber = siguienteLibre_2;
                        return siguienteLibre_2;
                    }
                }
            }else{
                if(i == 12){
                    //Indirecto
                    //Ya existe indirecto
                    fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                    fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                    for ( int j = 0 ; j < 16 ; j++ ) {
                        if(pb1.pb_pointers[j] == -1){
                            //Se Debe Crear Bloque
                            //Obtener Bloque para siguiente bloque carpeta
                            int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                            if(siguienteLibre_2 == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                return -1;
                            }
                            //Asignar a bitmap nuevo bloque carpeta
                            usuariologueadop.bm_Block[siguienteLibre_2] = '1';
                            //Obtener iNodo para siguiente bloque carpeta
                            int siguienteLibre_3 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                            if(siguienteLibre_3 == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                usuariologueadop.bm_Block[siguienteLibre_2] = '0';
                                return -1;
                            }
                            if(crearCarpetaSub(siguienteLibre_3 , inodeNumber, archivoDisco) == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                usuariologueadop.bm_Block[siguienteLibre_2] = '0';
                                return -1;
                            }
                            //Vaciar bloque nuevo
                            for(int a = 1; a < 4; a++){
                                bloqueContenido.contenapunta[a].f_inode = -1;
                                memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                            }
                            //Asignar a bloque de punteros, el nuevo bloque carpeta
                            pb1.pb_pointers[j] = siguienteLibre_2;
                            //Asignar iNodo a bloque
                            bloqueContenido.contenapunta[0].f_inode = siguienteLibre_3;
                            strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                            //Escribir el bloque nuevo
                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                            //Reescribir bloque de apuntadores
                            fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                            fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                            //Termino el proceso
                            printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                            inodeNumber = siguienteLibre_2;
                            return siguienteLibre_2;
                        }else{
                            //Bloque ya existe
                            fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                            fread(&bloqueContenido,sizeof (contentBlock),1,archivoDisco);
                            for(int a = 0 ; a < 4 ; a++){
                                //Buscar vacio
                                if(bloqueContenido.contenapunta[a].f_inode == -1){
                                    //Obtener iNodo para siguiente bloque carpeta
                                    int siguienteLibre_2 = nextFree(usuariologueadop.bm_iNode,0);
                                    if(siguienteLibre_2 == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                        return -1;
                                    }
                                    if(crearCarpetaSub(siguienteLibre_2 , inodeNumber, archivoDisco) == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                        return -1;
                                    }
                                    //Asignar iNodo a bloque
                                    bloqueContenido.contenapunta[a].f_inode = siguienteLibre_2;
                                    strcpy(bloqueContenido.contenapunta[a].f_name , pathVirtual);
                                    //Reescribir Bloque
                                    fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                    //Termino el proceso
                                    printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                    inodeNumber = siguienteLibre_2;
                                    return siguienteLibre_2;
                                }
                            }
                        }
                    }
                }else{
                    if(i == 13){
                        //Doble Indirecto
                        //Ya existe indirecto
                        fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                        fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                        for(int j = 0 ; j < 16 ; j ++){
                            if(pb1.pb_pointers[j] == -1){
                                //No existe apuntador indirecto
                                //Obtener Bloque para siguiente bloque indirecto
                                int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                if(siguienteLibre_2 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                    return -1;
                                }
                                //Asignar a bitmap nuevo bloque indirecto
                                usuariologueadop.bm_Block[siguienteLibre_2] = '3';
                                //Obtener Bloque para siguiente bloque carpeta
                                int siguienteLibre_3 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                if(siguienteLibre_3 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                    return -1;
                                }
                                //Asignar a bitmap nuevo bloque carpeta
                                usuariologueadop.bm_Block[siguienteLibre_3] = '1';
                                //Obtener iNodo para siguiente bloque carpeta
                                int siguienteLibre_4 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                                if(siguienteLibre_4 == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                    return -1;
                                }
                                if(crearCarpetaSub(siguienteLibre_4 , inodeNumber, archivoDisco) == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                    return -1;
                                }
                                //Vaciar indirecto
                                for (int a = 0; a < 16 ;a++) {
                                    pb2.pb_pointers[a] = -1;
                                }
                                //Vaciar bloque nuevo
                                for(int a = 1; a < 4; a++){
                                    bloqueContenido.contenapunta[a].f_inode = -1;
                                    memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                }
                                //Asignar a bloque de punteros, el nuevo bloque carpeta
                                pb1.pb_pointers[j] = siguienteLibre_2;
                                //Asignar a bloque de punteros, el nuevo bloque carpeta
                                pb2.pb_pointers[0] = siguienteLibre_3;
                                //Asignar iNodo a bloque
                                bloqueContenido.contenapunta[0].f_inode = siguienteLibre_4;
                                strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                                //Escribir el bloque nuevo
                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                //Reescribir bloque de apuntadores
                                fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                                fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                //Reescribir bloque de apuntadores
                                fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                                fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                                //Termino el proceso
                                printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                inodeNumber = siguienteLibre_4;
                                return siguienteLibre_4;
                            }else{
                                //YA existe apuntador indirecto
                                fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                                fread(&pb2,sizeof(pointerBlock),1,archivoDisco);
                                for(int k = 0 ; k < 16 ; k++ ){
                                    if(pb2.pb_pointers[k] == -1){
                                        //No existe bloque
                                        //Obtener Bloque para siguiente bloque carpeta
                                        int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                        if(siguienteLibre_2 == -1){
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                            return -1;
                                        }
                                        //Asignar a bitmap nuevo bloque carpeta
                                        usuariologueadop.bm_Block[siguienteLibre_2] = '1';
                                        //Obtener iNodo para siguiente bloque carpeta
                                        int siguienteLibre_3 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                                        if(siguienteLibre_3 == -1){
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                            return -1;
                                        }
                                        if(crearCarpetaSub(siguienteLibre_3 , inodeNumber, archivoDisco) == -1){
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                            return -1;
                                        }
                                        //Vaciar bloque nuevo
                                        for(int a = 1; a < 4; a++){
                                            bloqueContenido.contenapunta[a].f_inode = -1;
                                            memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                        }
                                        //Asignar a bloque de punteros, el nuevo bloque carpeta
                                        pb2.pb_pointers[k] = siguienteLibre_2;
                                        //Asignar iNodo a bloque
                                        bloqueContenido.contenapunta[0].f_inode = siguienteLibre_3;
                                        strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                                        //Escribir el bloque nuevo
                                        fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k] , usuariologueadop.startBlocks), SEEK_SET);
                                        fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                        //Reescribir bloque de apuntadores
                                        fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                                        fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                        //Termino el proceso
                                        printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                        inodeNumber = siguienteLibre_2;
                                        return siguienteLibre_2;
                                    }else{
                                        //Ya existe bloque
                                        fseek(archivoDisco,getBlockStart_2(pb2.pb_pointers[k],usuariologueadop.startBlocks),SEEK_SET);
                                        fread(&bloqueContenido,sizeof (contentBlock),1,archivoDisco);
                                        for(int a = 0 ; a < 4 ; a++){
                                            //Buscar vacio
                                            if(bloqueContenido.contenapunta[a].f_inode == -1){
                                                //Obtener iNodo para siguiente bloque carpeta
                                                int siguienteLibre_2 = nextFree(usuariologueadop.bm_iNode,0);
                                                if(siguienteLibre_2 == -1){
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                                    return -1;
                                                }
                                                if(crearCarpetaSub(siguienteLibre_2 , inodeNumber,archivoDisco) == -1){
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                                    return -1;
                                                }
                                                //Asignar iNodo a bloque
                                                bloqueContenido.contenapunta[a].f_inode = siguienteLibre_2;
                                                strcpy(bloqueContenido.contenapunta[a].f_name , pathVirtual);
                                                //Reescribir Bloque
                                                fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k] , usuariologueadop.startBlocks), SEEK_SET);
                                                fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                                //Termino el proceso
                                                printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                                inodeNumber = siguienteLibre_2;
                                                return siguienteLibre_2;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if(i == 14){
                            //Triple Indirecto
                            //Ya existe indirecto
                            fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i] , usuariologueadop.startBlocks), SEEK_SET);
                            fread(&pb1,sizeof(pointerBlock),1,archivoDisco);
                            for(int j = 0 ; j < 16 ; j ++){
                                if(pb1.pb_pointers[j] == -1){
                                    //No existe apuntador doble indirecto
                                    //Obtener Bloque para siguiente bloque indirecto
                                    int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                    if(siguienteLibre_2 == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                        return -1;
                                    }
                                    //Asignar a bitmap nuevo bloque indirecto
                                    usuariologueadop.bm_Block[siguienteLibre_2] = '3';
                                    //Obtener Bloque para siguiente bloque indirecto
                                    int siguienteLibre_3 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                    if(siguienteLibre_3 == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                        return -1;
                                    }
                                    //Asignar a bitmap nuevo bloque indirecto
                                    usuariologueadop.bm_Block[siguienteLibre_3] = '3';
                                    //Obtener Bloque para siguiente bloque carpeta
                                    int siguienteLibre_4 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                    if(siguienteLibre_4 == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                        return -1;
                                    }
                                    //Asignar a bitmap nuevo bloque carpeta
                                    usuariologueadop.bm_Block[siguienteLibre_4] = '1';
                                    //Obtener iNodo para siguiente bloque carpeta
                                    int siguienteLibre_5 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                                    if(siguienteLibre_5 == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                        return -1;
                                    }
                                    if(crearCarpetaSub(siguienteLibre_5 , inodeNumber, archivoDisco) == -1){
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                        return -1;
                                    }
                                    //Vaciar indirecto
                                    for (int a = 0; a < 16 ;a++) {
                                        pb2.pb_pointers[a] = -1;
                                    }
                                    //Vaciar bloque nuevo
                                    for(int a = 1; a < 4; a++){
                                        bloqueContenido.contenapunta[a].f_inode = -1;
                                        memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                    }
                                    //Asignar a bloque de punteros, el nuevo bloque carpeta
                                    pb1.pb_pointers[j] = siguienteLibre_2;
                                    //Asignar a bloque de punteros, el nuevo bloque carpeta
                                    pb2.pb_pointers[0] = siguienteLibre_3;
                                    //Asignar a bloque de punteros, el nuevo bloque carpeta
                                    pb3.pb_pointers[0] = siguienteLibre_4;
                                    //Asignar iNodo a bloque
                                    bloqueContenido.contenapunta[0].f_inode = siguienteLibre_5;
                                    strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                                    //Escribir el bloque nuevo
                                    fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                    //Reescribir bloque de apuntadores
                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[0],usuariologueadop.startBlocks),SEEK_SET);
                                    fwrite(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                    //Reescribir bloque de apuntadores
                                    fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                                    fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                    //Reescribir bloque de apuntadores
                                    fseek(archivoDisco, getBlockStart_2(inodoActual.i_block[i],usuariologueadop.startBlocks),SEEK_SET);
                                    fwrite(&pb1,sizeof (pointerBlock),1,archivoDisco);
                                    //Termino el proceso
                                    printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                    inodeNumber = siguienteLibre_5;
                                    return siguienteLibre_5;
                                }else{
                                    //YA existe apuntador doble indirecto
                                    fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j] , usuariologueadop.startBlocks), SEEK_SET);
                                    fread(&pb2,sizeof(pointerBlock),1,archivoDisco);
                                    for(int k = 0 ; k < 16 ; k++ ){
                                        if(pb2.pb_pointers[k] == -1){
                                            //No existe apuntador indirecto
                                            //Obtener Bloque para siguiente bloque indirecto
                                            int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                            if(siguienteLibre_2 == -1){
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                                return -1;
                                            }
                                            //Asignar a bitmap nuevo bloque indirecto
                                            usuariologueadop.bm_Block[siguienteLibre_2] = '3';
                                            //Obtener Bloque para siguiente bloque carpeta
                                            int siguienteLibre_3 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                            if(siguienteLibre_3 == -1){
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                                return -1;
                                            }
                                            //Asignar a bitmap nuevo bloque carpeta
                                            usuariologueadop.bm_Block[siguienteLibre_3] = '1';
                                            //Obtener iNodo para siguiente bloque carpeta
                                            int siguienteLibre_4 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                                            if(siguienteLibre_4 == -1){
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                                return -1;
                                            }
                                            if(crearCarpetaSub(siguienteLibre_4 , inodeNumber, archivoDisco) == -1){
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                                return -1;
                                            }
                                            //Vaciar indirecto
                                            for (int a = 0; a < 16 ;a++) {
                                                pb2.pb_pointers[a] = -1;
                                            }
                                            //Vaciar bloque nuevo
                                            for(int a = 1; a < 4; a++){
                                                bloqueContenido.contenapunta[a].f_inode = -1;
                                                memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                            }
                                            //Asignar a bloque de punteros, el nuevo bloque carpeta
                                            pb2.pb_pointers[k] = siguienteLibre_2;
                                            //Asignar a bloque de punteros, el nuevo bloque carpeta
                                            pb3.pb_pointers[0] = siguienteLibre_3;
                                            //Asignar iNodo a bloque
                                            bloqueContenido.contenapunta[0].f_inode = siguienteLibre_4;
                                            strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                                            //Escribir el bloque nuevo
                                            fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[0] , usuariologueadop.startBlocks), SEEK_SET);
                                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                            //Reescribir bloque de apuntadores
                                            fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k],usuariologueadop.startBlocks),SEEK_SET);
                                            fwrite(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                            //Reescribir bloque de apuntadores
                                            fseek(archivoDisco, getBlockStart_2(pb1.pb_pointers[j],usuariologueadop.startBlocks),SEEK_SET);
                                            fwrite(&pb2,sizeof (pointerBlock),1,archivoDisco);
                                            //Termino el proceso
                                            printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                            inodeNumber = siguienteLibre_4;
                                            return siguienteLibre_4;
                                        }else{
                                            //Ya existe apuntador indirecto
                                            fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k] , usuariologueadop.startBlocks), SEEK_SET);
                                            fread(&pb3,sizeof(pointerBlock),1,archivoDisco);
                                            for( int h = 0 ; h < 16 ; h++ ){
                                                if(pb3.pb_pointers[h] == -1){
                                                    //No existe bloque
                                                    //Obtener Bloque para siguiente bloque carpeta
                                                    int siguienteLibre_2 = nextFree(usuariologueadop.bm_Block,0); //posicion para el bloque
                                                    if(siguienteLibre_2 == -1){
                                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Bloques llenos\n");
                                                        return -1;
                                                    }
                                                    //Asignar a bitmap nuevo bloque carpeta
                                                    usuariologueadop.bm_Block[siguienteLibre_2] = '1';
                                                    //Obtener iNodo para siguiente bloque carpeta
                                                    int siguienteLibre_3 = nextFree(usuariologueadop.bm_iNode,0); //posicion para el iNodo
                                                    if(siguienteLibre_3 == -1){
                                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                                        return -1;
                                                    }
                                                    if(crearCarpetaSub(siguienteLibre_3 , inodeNumber, archivoDisco) == -1){
                                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                                        return -1;
                                                    }
                                                    //Vaciar bloque nuevo
                                                    for(int a = 1; a < 4; a++){
                                                        bloqueContenido.contenapunta[a].f_inode = -1;
                                                        memset(&bloqueContenido.contenapunta[a].f_name,0,12);
                                                    }
                                                    //Asignar a bloque de punteros, el nuevo bloque carpeta
                                                    pb3.pb_pointers[h] = siguienteLibre_2;
                                                    //Asignar iNodo a bloque
                                                    bloqueContenido.contenapunta[0].f_inode = siguienteLibre_3;
                                                    strcpy(bloqueContenido.contenapunta[0].f_name , pathVirtual);
                                                    //Escribir el bloque nuevo
                                                    fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[h] , usuariologueadop.startBlocks), SEEK_SET);
                                                    fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                                    //Reescribir bloque de apuntadores
                                                    fseek(archivoDisco, getBlockStart_2(pb2.pb_pointers[k],usuariologueadop.startBlocks),SEEK_SET);
                                                    fwrite(&pb3,sizeof (pointerBlock),1,archivoDisco);
                                                    //Termino el proceso
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                                    inodeNumber = siguienteLibre_2;
                                                    return siguienteLibre_2;
                                                }else{
                                                    //YA existe bloque
                                                    fseek(archivoDisco,getBlockStart_2(pb3.pb_pointers[h],usuariologueadop.startBlocks),SEEK_SET);
                                                    fread(&bloqueContenido,sizeof (contentBlock),1,archivoDisco);
                                                    for(int a = 0 ; a < 4 ; a++){
                                                        //Buscar vacio
                                                        if(bloqueContenido.contenapunta[a].f_inode == -1){
                                                            //Obtener iNodo para siguiente bloque carpeta
                                                            int siguienteLibre_2 = nextFree(usuariologueadop.bm_iNode,0);
                                                            if(siguienteLibre_2 == -1){
                                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                                                                return -1;
                                                            }
                                                            if(crearCarpetaSub(siguienteLibre_2 , inodeNumber, archivoDisco) == -1){
                                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio mal\n");
                                                                return -1;
                                                            }
                                                            //Asignar iNodo a bloque
                                                            bloqueContenido.contenapunta[a].f_inode = siguienteLibre_2;
                                                            strcpy(bloqueContenido.contenapunta[a].f_name , pathVirtual);
                                                            //Reescribir Bloque
                                                            fseek(archivoDisco, getBlockStart_2(pb3.pb_pointers[h] , usuariologueadop.startBlocks), SEEK_SET);
                                                            fwrite(&bloqueContenido, sizeof(contentBlock) , 1 , archivoDisco);
                                                            //Termino el proceso
                                                            printf("\033[1;92mGeremiasLS \033[1;97m/: \033[1;45m COMANDO REALIZADO \033[0m se genero '%s'\n", pathVirtual);
                                                            inodeNumber = siguienteLibre_2;
                                                            return siguienteLibre_2;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return -1;
    }
    return 0;
}
char *auxFindCarpeta(int inodo, char expresion[], int tabulaciones, FILE *archivoDisco){
    char aux[9999] = {0};
    if(archivoDisco != NULL){
        inode inodoM;
        //Ir a iNodo de carpeta
        fseek(archivoDisco,getiNodeStart_2(inodo,usuariologueadop.startiNodes),SEEK_SET);
        fread(&inodoM,sizeof(inodoM),1,archivoDisco);
        //Generar expresion regular (Ya fue validada)
        regex_t expresionFind;
        int estadoExpresionFind = -1;
        estadoExpresionFind = regcomp(&expresionFind,expresion,REG_EXTENDED);
        if(estadoExpresionFind){
            //Algo salio mal
          // return nullptr;  // return -1;
           return "-1";
        }
        for(int y = 0 ; y < 15 ; y++ ){
            if(inodoM.i_block[y] == -1){
                continue;
            }
            if(y < 12){
                contentBlock archivosiNodo;
                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                //Leer contenido de bloque
                fseek(archivoDisco, offset, SEEK_SET);
                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                for( int x = 0 ; x < 4 ; x++ ){
                    if(archivosiNodo.contenapunta[x].f_inode != -1){
                        if( verificarExtension2(archivosiNodo.contenapunta[x].f_name) == 0 ){
                            //Es un archivo
                            if(!regexec(&expresionFind,archivosiNodo.contenapunta[x].f_name,0,NULL,0)){
                                //Cumple el regex
                                for(int b = 0 ; b < tabulaciones ; b++ ){
                                    sprintf(aux,"%s     ",aux);
                                }
                                sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[x].f_name);
                                sprintf(aux,"%s\n",aux);
                            }
                        }else if( strcmp(archivosiNodo.contenapunta[x].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[x].f_name,"..") != 0 ){
                            //Es una carpeta
                            if(strcmp(archivosiNodo.contenapunta[x].f_name,"") != 0){
                                for(int b = 0 ; b < tabulaciones ; b++ ){
                                    sprintf(aux,"%s     ",aux);
                                }
                                sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[x].f_name);
                                sprintf(aux,"%s\n",aux);
                                tabulaciones++;
                                char *temp = auxFindCarpeta(archivosiNodo.contenapunta[x].f_inode, expresion, tabulaciones, archivoDisco);
                                char temp2[9999] = {0};
                                strcpy(temp2,temp);
                                sprintf( aux,"%s%s",aux, temp2 );
                                sprintf(aux,"%s\n",aux);
                                tabulaciones--;
                                free(temp);
                            }
                        }
                    }
                }
            }else{
                //Indirectos
                if(y == 12){
                    //Indirecto
                    pointerBlock pb1;
                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                    fseek(archivoDisco, offset, SEEK_SET);
                    fread(&pb1,sizeof(pb1),1,archivoDisco);
                    for( int z = 0 ; z < 16 ; z++ ){
                        if(pb1.pb_pointers[z] == -1){
                            continue;
                        }
                        contentBlock archivosiNodo;
                        int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                        fseek(archivoDisco, offset_2, SEEK_SET);
                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                        for( int a = 0 ; a < 4 ; a++ ){
                            if(archivosiNodo.contenapunta[a].f_inode != -1){
                                if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                    //Es un archivo
                                    if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                        //Cumple el regex
                                        for(int b = 0 ; b < tabulaciones ; b++ ){
                                            sprintf(aux,"%s     ",aux);
                                        }
                                        sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                        sprintf(aux,"%s\n",aux);
                                    }
                                }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                    //Es una carpeta
                                    if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                        for(int b = 0 ; b < tabulaciones ; b++ ){
                                            sprintf(aux,"%s     ",aux);
                                        }
                                        sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                        sprintf(aux,"%s\n",aux);
                                        tabulaciones++;
                                        char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, expresion, tabulaciones, archivoDisco);
                                        char temp2[9999] = {0};
                                        strcpy(temp2,temp);
                                        sprintf( aux,"%s%s",aux, temp2 );
                                        sprintf(aux,"%s\n",aux);
                                        tabulaciones--;
                                        free(temp);
                                    }
                                }
                            }
                        }
                    }
                }else{
                    if(y == 13){
                        //Doble indirecto
                        pointerBlock pb2;
                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                        fseek(archivoDisco, offset, SEEK_SET);
                        fread(&pb2,sizeof(pb2),1,archivoDisco);
                        for( int z = 0 ; z < 16 ; z++ ){
                            if(pb2.pb_pointers[z] == -1){
                                continue;
                            }
                            pointerBlock pb2_2;
                            int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                            fseek(archivoDisco, offset_2, SEEK_SET);
                            fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                            for (int w = 0 ; w < 16 ; w++ ){
                                if(pb2_2.pb_pointers[w] == -1){
                                    continue;
                                }
                                contentBlock archivosiNodo;
                                int offset_3 = getBlockStart_2(pb2_2.pb_pointers[w] , usuariologueadop.startBlocks);
                                fseek(archivoDisco, offset_3, SEEK_SET);
                                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                for( int a = 0 ; a < 4 ; a++ ){
                                    if(archivosiNodo.contenapunta[a].f_inode != -1){
                                        if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                            //Es un archivo
                                            if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                                //Cumple el regex
                                                for(int b = 0 ; b < tabulaciones ; b++ ){
                                                    sprintf(aux,"%s     ",aux);
                                                }
                                                sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                                sprintf(aux,"%s\n",aux);
                                            }
                                        }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                            //Es una carpeta
                                            if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                                for(int b = 0 ; b < tabulaciones ; b++ ){
                                                    sprintf(aux,"%s     ",aux);
                                                }
                                                sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                                sprintf(aux,"%s\n",aux);
                                                tabulaciones++;
                                                char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, expresion, tabulaciones, archivoDisco);
                                                char temp2[9999] = {0};
                                                strcpy(temp2,temp);
                                                sprintf( aux,"%s%s",aux, temp2 );
                                                sprintf(aux,"%s\n",aux);
                                                tabulaciones--;
                                                free(temp);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if(y == 14){
                            //Triple indirecto
                            pointerBlock pb3;
                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                            fseek(archivoDisco, offset, SEEK_SET);
                            fread(&pb3,sizeof(pb3),1,archivoDisco);
                            for( int z = 0 ; z < 16 ; z++ ){
                                if(pb3.pb_pointers[z] == -1){
                                    continue;
                                }
                                pointerBlock pb3_2;
                                int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                fseek(archivoDisco, offset_2, SEEK_SET);
                                fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                for (int w = 0 ; w < 16 ; w++ ){
                                    if(pb3_2.pb_pointers[w] == -1){
                                        continue;
                                    }
                                    pointerBlock pb3_2_1;
                                    int offset_3 = getBlockStart_2(pb3_2.pb_pointers[w], usuariologueadop.startBlocks);
                                    fseek(archivoDisco, offset_3, SEEK_SET);
                                    fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                    for (int v = 0 ; v < 16 ; v++ ){
                                        if(pb3_2_1.pb_pointers[v] == -1){
                                            continue;
                                        }
                                        contentBlock archivosiNodo;
                                        int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[v] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset_4, SEEK_SET);
                                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                        for( int a = 0 ; a < 4 ; a++ ){
                                            if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                    //Es un archivo
                                                    if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                                        //Cumple el regex
                                                        for(int b = 0 ; b < tabulaciones ; b++ ){
                                                            sprintf(aux,"%s     ",aux);
                                                        }
                                                        sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                                        sprintf(aux,"%s\n",aux);
                                                    }
                                                }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                    //Es una carpeta
                                                    if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                                        for(int b = 0 ; b < tabulaciones ; b++ ){
                                                            sprintf(aux,"%s     ",aux);
                                                        }
                                                        sprintf(aux,"%s|__%s",aux,archivosiNodo.contenapunta[a].f_name);
                                                        sprintf(aux,"%s\n",aux);
                                                        tabulaciones++;
                                                        char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, expresion, tabulaciones, archivoDisco);
                                                        char temp2[9999] = {0};
                                                        strcpy(temp2,temp);
                                                        sprintf( aux,"%s%s",aux, temp2 );
                                                        sprintf(aux,"%s\n",aux);
                                                        tabulaciones--;
                                                        free(temp);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    char *salidaEx;
    salidaEx = strdup(aux);
    return salidaEx;
}
int actualizarSP(PMOUNT particion, int esMount, FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = initSP(sp_aux);
    sp_aux = getSuperBlock(particion, archivoDisco);
    if(sp_aux.sb_magic == 61267){
        if(archivoDisco != NULL){
            char bm_inode[270000] = {0};
            char bm_block[270000] = {0};

            int read_iNode = sp_aux.sb_inodes_count;
            int last_iNode = -1;
            int read_Block = sp_aux.sb_blocks_count;
            int last_Block = -1;

            char aux_recorrido = 0;
            //Obtener todos los iNodos
            fseek(archivoDisco,sp_aux.sb_bm_inode,SEEK_SET);
            for(int i = 0 ; i < read_iNode; i++){
                fread(&aux_recorrido,1,1,archivoDisco);
                bm_inode[i] = aux_recorrido;
                if(aux_recorrido == '1' || aux_recorrido == '2' || aux_recorrido == '3'){
                    last_iNode = i;
                }
            }
            //Obtener todos los Bloques
            fseek(archivoDisco,sp_aux.sb_bm_Block,SEEK_SET);
            for(int i = 0 ; i < read_Block; i++){
                fread(&aux_recorrido,1,1,archivoDisco);
                bm_block[i] = aux_recorrido;
                if(aux_recorrido == '1' || aux_recorrido == '2' || aux_recorrido == '3'){
                    last_Block = i;
                }
            }
            //Despues de los bitmap
            sp_aux.sb_first_inode = last_iNode;
            sp_aux.sb_first_block = last_Block;

            sp_aux.sb_free_inodes_count = sp_aux.sb_inodes_count - last_iNode;
            sp_aux.sb_free_blocks_count = sp_aux.sb_blocks_count - last_Block;

            if(esMount == 1){
                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(sp_aux.sb_mtime, sizeof(sp_aux.sb_mtime)-1, "%d/%m/%y %H:%M", fecha);

                sp_aux.mount_count++;
            }
            if(esMount == 2){
                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(sp_aux.sb_umtime, sizeof(sp_aux.sb_umtime)-1, "%d/%m/%y %H:%M", fecha);
            }

            if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                strcpy(usuariologueadop.bm_iNode,bm_inode);
                strcpy(usuariologueadop.bm_Block,bm_block);
            }

            fseek(archivoDisco,particion.mount_start,SEEK_SET);
            fwrite(&sp_aux,sizeof (SuperBloque),1,archivoDisco);
        }
        return 1;
    }
    return 0;
}
int ejecutarLOGIN(char usr[], char pwd[], char pathDisco[], PMOUNT particion, FILE *archivoDisco){
    userstats grupos[200];
    userstats users[200];
    int nGrupos = 0;
    int nusers = 0;
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(particion, archivoDisco);
    if(sp_aux.sb_magic == 61267){
        archivo archusers;
        archusers = getArchivo("users.txt",particion,sp_aux.sb_t_inode, sp_aux.sb_t_inode,archivoDisco);
        if(archusers.estado != -1){
            if(archivoDisco != NULL){
                int blockStart = sp_aux.sb_bm_Block;
                int blockCount = sp_aux.sb_blocks_count;

                char bitmapBlock[270000] = {0};
                char bitAux;
                //Cargar mapa de bits de bloques
                fseek(archivoDisco, blockStart, SEEK_SET);
                for( int i = 0; i < blockCount ; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapBlock[i] = bitAux;
                    bitmapBlock[i+1] = '\0';
                }
                blockStart = sp_aux.sb_bm_inode;
                blockCount = sp_aux.sb_inodes_count;
                fseek(archivoDisco,blockStart,SEEK_SET);
                char bitmapiNode[270000] = {0};
                for( int i = 0; i < blockCount; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapiNode[i] = bitAux;
                    bitmapiNode[i+1] = '\0';
                }
                usuariologueadop.startiNodes = sp_aux.sb_t_inode;
                usuariologueadop.startBlocks = sp_aux.sb_t_block;

                strcpy(usuariologueadop.bm_Block , bitmapBlock);
                strcpy(usuariologueadop.bm_iNode , bitmapiNode);

                char aux[9999] = {0};
                char aux2[9999] = {0};
                char texto_archivo[999];
                char grupo[999] = {0};
                char com[4][1000];

                strcpy(texto_archivo,archusers.contenido);
                int i = 0;
                int length = 0;
                int comas = 0;
                int comillas = 0;
                int status = 0;
                int id_user;
                while (texto_archivo[i] != NULL) {
                    char aux_char[] = {texto_archivo[i],'\0'};
                    if(texto_archivo[i] == '\n'){
                        strcpy(aux2,"");
                        comillas = 0;
                        for(int j = 0; j < length; j++){
                            char aux_char_2[] = {aux[j],'\0'};
                            if(aux[j] == ','){
                                strcpy(com[comillas],aux2);
                                comillas++;
                                strcpy(aux2,"");
                            }else{
                                strcat(aux2, aux_char_2);
                            }
                        }
                        if(comas == 2){
                            grupos[nGrupos].numGrupo = atoi(com[0]);
                            strcpy(grupos[nGrupos].name,aux2);
                            if(status == 1){
                                if(strcmp(grupo,aux2) == 0){
                                    usuariologueadop.id_grupo = grupos[nGrupos].numGrupo;
                                    strcpy(usuariologueadop.namePartition,particion.mount_name);
                                    strcpy(usuariologueadop.pathDisco,pathDisco);
                                    strcpy(usuariologueadop.name,usr);
                                    usuariologueadop.partUser = particion;
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Bienvenido %s\n", usr);
                                    strcpy(usuariologueadop.bm_Block,bitmapBlock);
                                    strcpy(usuariologueadop.bm_iNode,bitmapiNode);
                                    return 1;
                                }
                            }
                            nGrupos++;
                        }else if(status == 0){
                            id_user = atoi(com[0]);
                            users[nusers].numGrupo = atoi(com[0]);
                            strcpy(users[nusers].group, com[2]);
                            strcpy(users[nusers].name, com[3]);
                            nusers++;
                            strcpy(grupo, com[2]);
                            if(strcmp(usr,com[3]) == 0 && strcmp(pwd,aux2) == 0){
                                status = 1;
                                usuariologueadop.id_user = id_user;
                                for( int j = 0; j < nGrupos; j++){
                                    if(strcmp(grupo,grupos[j].name) == 0){
                                        usuariologueadop.id_grupo = grupos[j].numGrupo;
                                        strcpy(usuariologueadop.namePartition,particion.mount_name);
                                        strcpy(usuariologueadop.pathDisco,pathDisco);
                                        strcpy(usuariologueadop.name,usr);
                                        usuariologueadop.partUser = particion;
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Bienvenido %s\n",usr);
                                        return 1;
                                    }
                                }
                            }else{
                                printf("\033[101m ERROR PAPU \033[0m Usuario o contraseña no validos \n");
                            }
                        }
                        strcpy(aux,"");
                        comas = 0;
                        length = 0;
                    }else{
                        strcat(aux,aux_char);
                        length++;
                        if(texto_archivo[i] == ','){
                            comas++;
                        }
                    }
                    i++;
                }
            }else{

            }
        }else{
            //Algo salio mal
        }
        for(int i = 0; i < nusers ; i++){
            globusers[i] = users[i];
        }
        globNusers = nusers;
    }else{
        //Algo salio mal
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
        return -1;
    }
    return 0;
}
int ejecutarLOGOUT(){
    initUsr();
    return 0;
}
int ejecutarMKGRP(char groupName[], FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic != 61267){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de arhcivos corrompido\n");
        return -1;
    }
    userstats grupos[200];
    for(int a = 0 ; a < 200 ; a ++){
        memset(grupos[a].name,0,999);
        memset(grupos[a].pass,0,999);
        memset(grupos[a].group,0,999);
        grupos[a].numGrupo = 0;
        grupos[a].position = 0;
    }
    archivo userFile = getArchivo("users.txt",usuariologueadop.partUser,usuariologueadop.startiNodes,usuariologueadop.startiNodes,archivoDisco);
    int i = 0;
    int length = 0;
    int comas = 0;
    int comillas = 0;
    int nGrupos = 0;
    int estado = 1;
    int starter = 0;
    if(userFile.estado != -1){
        char aux[9999] = {0};
        char aux2[9999] = {0};
        char texto_archivo[999];
        char com[4][1000];

        strcpy(texto_archivo,userFile.contenido);

        while(texto_archivo[i]!=NULL){
            char aux_char[] = {texto_archivo[i],'\0'};
            if(texto_archivo[i] == '\n'){
                if(comas == 2){
                    memset(&aux2,0,9999);
                    comillas = 0;
                    for ( int j = 0 ; j < length ; j++) {
                        char aux_char_2[] = {aux[j],'\0'};
                        if(aux[j] == ','){
                            strcpy(com[comillas],aux2);
                            comillas++;
                            memset(&aux2,0,9999);
                        }else{
                            strcat(aux2, aux_char_2);
                        }
                    }
                    grupos[nGrupos].numGrupo = atoi(com[0]);
                    grupos[nGrupos].position = starter;
                    strcpy(grupos[nGrupos].name,aux2);
                    nGrupos++;
                }
                memset(&aux,0,9999);
                comas = 0;
                length = 0;
                starter = i + 1;
            }else{
                strcat(aux, aux_char);
                length++;
                if(texto_archivo[i] ==','){
                    comas++;
                }
            }
            i++;
        }
        for(int j = 0; j < nGrupos ; j++){
            if(strcmp(grupos[j].name,groupName) == 0 && grupos[j].numGrupo > 0){
                estado = -1;
                break;
            }
        }
        if(estado == 1){
            sprintf(texto_archivo,"%s%i,G,%s\n",texto_archivo,nGrupos+1,groupName);
            modificarArchivo(userFile.position, texto_archivo, userFile.contenido, archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero grupo %s \n",groupName);
            //JOURNAL CREAR GRUPO
            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                journal entrada;
                entrada = initJournal(entrada);
                entrada.j_uid = usuariologueadop.id_user;

                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                strcpy(entrada.j_name,groupName);
                strcpy(entrada.j_content,"");

                entrada.j_perm = 664;
                entrada.j_type = '1';
                entrada.j_op_type = '0';

                int nextJor = nextJournal(archivoDisco, sp_aux.sb_bm_inode);
                writeJournal(entrada,archivoDisco,nextJor);
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Grupo %s ya existe \n",groupName);
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarRMGRP(char groupName[], FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic != 61267){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de arhcivos corrompido\n");
        return -1;
    }
    userstats grupos[200];
    for(int a = 0 ; a < 200 ; a ++){
        memset(grupos[a].name,0,999);
        memset(grupos[a].pass,0,999);
        memset(grupos[a].group,0,999);
        grupos[a].numGrupo = 0;
        grupos[a].position = 0;
    }
    archivo userFile = getArchivo("users.txt",usuariologueadop.partUser,usuariologueadop.startiNodes,usuariologueadop.startiNodes,archivoDisco);
    int i = 0;
    int length = 0;
    int comas = 0;
    int comillas = 0;
    int nGrupos = 0;
    int estado = -1;
    int starter = 0;
    if(userFile.estado != -1){
        char aux[9999] = {0};
        char aux2[9999] = {0};
        char texto_archivo[999];
        char com[4][1000];

        strcpy(texto_archivo,userFile.contenido);

        while(texto_archivo[i]!=NULL){
            char aux_char[] = {texto_archivo[i],'\0'};
            if(texto_archivo[i] == '\n'){
                if(comas == 2){
                    memset(&aux2,0,9999);
                    comillas = 0;
                    for ( int j = 0 ; j < length ; j++) {
                        char aux_char_2[] = {aux[j],'\0'};
                        if(aux[j] == ','){
                            strcpy(com[comillas],aux2);
                            comillas++;
                            memset(&aux2,0,9999);
                        }else{
                            strcat(aux2, aux_char_2);
                        }
                    }
                    grupos[nGrupos].numGrupo = atoi(com[0]);
                    grupos[nGrupos].position = starter;
                    strcpy(grupos[nGrupos].name,aux2);
                    nGrupos++;
                }
                memset(&aux,0,9999);
                comas = 0;
                length = 0;
                starter = i + 1;
            }else{
                strcat(aux, aux_char);
                length++;
                if(texto_archivo[i] ==','){
                    comas++;
                }
            }
            i++;
        }
        for(int j = 0; j < nGrupos ; j++){
            if(strcmp(grupos[j].name,groupName) == 0 && grupos[j].numGrupo > 0){
                if(grupos[j].numGrupo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede borrar este grupo \n");
                    break;
                }
                char aux_numG[7];
                sprintf(aux_numG,"%i",grupos[j].numGrupo);
                estado = 1;
                for ( int k = grupos[j].position ; k < grupos[j].position + strlen(aux_numG) ; k++ ) {
                    texto_archivo[k] = '0';
                }
                break;
            }
        }
        if(estado == 1){
            modificarArchivo(userFile.position, texto_archivo, userFile.contenido, archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino grupo %s \n",groupName);
            //JOURNAL ELIMINAR GRUPO
            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                journal entrada;
                entrada = initJournal(entrada);
                entrada.j_uid = usuariologueadop.id_user;

                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                strcpy(entrada.j_name,groupName);
                strcpy(entrada.j_content,"");

                entrada.j_perm = 664;
                entrada.j_type = '1';
                entrada.j_op_type = '1';

                int nextJor = nextJournal(archivoDisco, sp_aux.sb_bm_inode);
                writeJournal(entrada,archivoDisco,nextJor);
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Grupo %s no existe \n",groupName);
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarMKUSR(char user[], char password[], char groupName[], FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic != 61267){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de arhcivos corrompido\n");
        return -1;
    }
    userstats grupos[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(grupos[a].name,0,999);
        memset(grupos[a].pass,0,999);
        memset(grupos[a].group,0,999);
        grupos[a].numGrupo = 0;
        grupos[a].position = 0;
    }
    userstats users[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(users[a].name,0,999);
        memset(users[a].pass,0,999);
        memset(users[a].group,0,999);
        users[a].numGrupo = 0;
        users[a].position = 0;
    }
    archivo userFile = getArchivo("users.txt",usuariologueadop.partUser,usuariologueadop.startiNodes,usuariologueadop.startiNodes, archivoDisco);
    int i = 0;
    int length = 0;
    int comas = 0;
    int comillas = 0;
    int nGrupos = 0;
    int nUsers = 0;
    int estado = 1;
    int starter=0;
    if(userFile.estado != -1){
        char aux[9999]= {0};
        char texto_archivo[999] = {0};
        char aux2[9999] = {0};
        char com[4][1000];
        strcpy(texto_archivo, userFile.contenido);
        while(texto_archivo[i] != NULL){
            char aux_char[]= {texto_archivo[i],'\0'};
            if(texto_archivo[i] == '\n'){
                memset(&aux2,0,9999);
                comillas = 0;
                for(int j = 0; j < length; j++){
                    char aux_char_2[] = {aux[j], '\0'};
                    if(aux[j] == ','){
                        strcpy(com[comillas] , aux2);
                        comillas++;
                        memset(&aux2,0,9999);
                    }else{
                        strcat(aux2 , aux_char_2);
                    }
                }
                if(comas == 2){
                    grupos[nGrupos].numGrupo = atoi(com[0]);
                    grupos[nGrupos].position = starter;
                    strcpy(grupos[nGrupos].name , aux2);
                    nGrupos++;
                }else if(comas == 4){
                    users[nUsers].numGrupo = atoi(com[0]);
                    users[nUsers].position = starter;
                    strcpy(users[nUsers].name , com[3]);
                    nUsers++;
                }
                memset(&aux,0,9999);
                comas=0;
                length=0;
                starter = i+1;
            }else{
                strcat(aux, aux_char);
                length++;
                if(texto_archivo[i] == ',')
                {
                    comas++;
                }
            }
            i++;
        }
        int eGrupo = -1;
        for(int j =0 ; j < nGrupos; j++){
            if(strcmp(grupos[j].name , groupName) == 0 && grupos[j].numGrupo > 0 ){
                eGrupo = 1;
                break;
            }
        }
        if(eGrupo != -1){
            for(int j =0 ; j < nUsers; j++){
                if(strcmp(users[j].name , user) == 0 && users[j].numGrupo > 0){
                    estado = -1;
                    break;
                }
            }
            if(estado == 1){
                sprintf(texto_archivo, "%s%i,U,%s,%s,%s\n",texto_archivo,  nUsers+1 ,groupName,  user , password);
                modificarArchivo(userFile.position, texto_archivo, userFile.contenido, archivoDisco);
                printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero user %s \n" , user);
                //JOURNAL CREAR user
                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                    journal entrada;
                    entrada = initJournal(entrada);
                    entrada.j_uid = usuariologueadop.id_user;

                    time_t tiempoActual = time(NULL);
                    struct tm *fecha = localtime(&tiempoActual);
                    strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                    sprintf(entrada.j_name,"%s@%s@%s",user,password,groupName);
                    strcpy(entrada.j_content,"");

                    entrada.j_perm = 664;
                    entrada.j_type = '1';
                    entrada.j_op_type = '2';

                    int nextJor = nextJournal(archivoDisco, sp_aux.sb_bm_inode);
                    writeJournal(entrada,archivoDisco,nextJor);
                }
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: user %s ya existe \n",user);
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Grupo %s no existe en particion \n",groupName);
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarRMUSR(char userName[], FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic != 61267){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de arhcivos corrompido\n");
        return -1;
    }
    userstats grupos[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(grupos[a].name,0,999);
        memset(grupos[a].pass,0,999);
        memset(grupos[a].group,0,999);
        grupos[a].numGrupo = 0;
        grupos[a].position = 0;
    }
    userstats users[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(users[a].name,0,999);
        memset(users[a].pass,0,999);
        memset(users[a].group,0,999);
        users[a].numGrupo = 0;
        users[a].position = 0;
    }
    archivo userFile = getArchivo("users.txt",usuariologueadop.partUser,usuariologueadop.startiNodes,usuariologueadop.startiNodes, archivoDisco);
    int i = 0;
    int length = 0;
    int comas = 0;
    int comillas = 0;
    int nGrupos = 0;
    int nUsers = 0;
    int estado = -1;
    int starter=0;
    if(userFile.estado != -1){
        char aux[9999]= {0};
        char texto_archivo[999] = {0};
        char aux2[9999] = {0};
        char com[4][1000];
        strcpy(texto_archivo, userFile.contenido);
        while(texto_archivo[i] != NULL){
            char aux_char[]= {texto_archivo[i],'\0'};
            if(texto_archivo[i] == '\n'){
                memset(&aux2,0,9999);
                comillas = 0;
                for(int j = 0; j < length; j++){
                    char aux_char_2[] = {aux[j], '\0'};
                    if(aux[j] == ','){
                        strcpy(com[comillas] , aux2);
                        comillas++;
                        memset(&aux2,0,9999);
                    }else{
                        strcat(aux2 , aux_char_2);
                    }
                }
                if(comas == 2){
                    grupos[nGrupos].numGrupo = atoi(com[0]);
                    grupos[nGrupos].position = starter;
                    strcpy(grupos[nGrupos].name , aux2);
                    nGrupos++;
                }else{
                    users[nUsers].numGrupo = atoi(com[0]);
                    users[nUsers].position = starter;
                    strcpy(users[nUsers].name , com[3]);
                    nUsers++;
                }
                memset(&aux,0,9999);
                comas=0;
                length=0;
                starter = i+1;
            }else{
                strcat(aux, aux_char);
                length++;
                if(texto_archivo[i] == ',')
                {
                    comas++;
                }
            }
            i++;
        }
        for(int j =0 ; j < nUsers; j++){
            if(strcmp(users[j].name , userName) == 0 && users[j].numGrupo > 0){
                if(users[j].numGrupo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede eliminar este user\n");
                    break;
                }
                char aux_numU[7];
                sprintf(aux_numU,"%i",users[j].numGrupo);
                estado = 1;
                for ( int k = users[j].position ; k < users[j].position + strlen(aux_numU) ; k++ ) {
                    texto_archivo[k] = '0';
                }
                break;
            }
        }
        if(estado == 1){
            modificarArchivo(userFile.position, texto_archivo, userFile.contenido, archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino user %s \n" , userName);
            //JOURNAL ELIMINAR user
            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                journal entrada;
                entrada = initJournal(entrada);
                entrada.j_uid = usuariologueadop.id_user;

                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                strcpy(entrada.j_name,userName);
                strcpy(entrada.j_content,"");

                entrada.j_perm = 664;
                entrada.j_type = '1';
                entrada.j_op_type = '3';

                int nextJor = nextJournal(archivoDisco, sp_aux.sb_bm_inode);
                writeJournal(entrada,archivoDisco,nextJor);
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: user %s no existe \n",userName);
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarCHGRP(char userV[], char grupoN[], FILE* archivoDisco){
    SuperBloque sp_aux;
    sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
    if(sp_aux.sb_magic != 61267){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de arhcivos corrompido\n");
        return -1;
    }
    userstats grupos[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(grupos[a].name,0,999);
        memset(grupos[a].pass,0,999);
        memset(grupos[a].group,0,999);
        grupos[a].numGrupo = 0;
        grupos[a].position = 0;
    }
    userstats users[100];
    for(int a = 0 ; a < 100 ; a ++){
        memset(users[a].name,0,999);
        memset(users[a].pass,0,999);
        memset(users[a].group,0,999);
        users[a].numGrupo = 0;
        users[a].position = 0;
    }
    archivo userFile = getArchivo("users.txt",usuariologueadop.partUser,usuariologueadop.startiNodes,usuariologueadop.startiNodes, archivoDisco);
    int i = 0;
    int length = 0;
    int comas = 0;
    int comillas = 0;
    int nGrupos = 0;
    int nUsers = 0;
    int estado = -1;
    int starter=0;
    if(userFile.estado != -1){
        char aux[9999]= {0};
        char texto_archivo[999] = {0};
        char aux2[9999] = {0};
        char com[4][1000];
        strcpy(texto_archivo, userFile.contenido);
        while(texto_archivo[i] != NULL){
            char aux_char[]= {texto_archivo[i],'\0'};
            if(texto_archivo[i] == '\n'){
                memset(&aux2,0,9999);
                comillas = 0;
                for(int j = 0; j < length; j++){
                    char aux_char_2[] = {aux[j], '\0'};
                    if(aux[j] == ','){
                        strcpy(com[comillas] , aux2);
                        comillas++;
                        memset(&aux2,0,9999);
                    }else{
                        strcat(aux2 , aux_char_2);
                    }
                }
                if(comas == 2){
                    grupos[nGrupos].numGrupo = atoi(com[0]);
                    grupos[nGrupos].position = starter;
                    strcpy(grupos[nGrupos].name , aux2);
                    nGrupos++;
                }else{
                    users[nUsers].numGrupo = atoi(com[0]);
                    users[nUsers].position = starter;
                    strcpy(users[nUsers].group, com[2]);
                    strcpy(users[nUsers].name , com[3]);
                    strcpy(users[nUsers].pass,aux2);
                    nUsers++;
                }
                memset(&aux,0,9999);
                comas=0;
                length=0;
                starter = i+1;
            }else{
                strcat(aux, aux_char);
                length++;
                if(texto_archivo[i] == ',')
                {
                    comas++;
                }
            }
            i++;
        }
        for(int j =0 ; j < nUsers; j++){
            if(strcmp(users[j].name , userV) == 0 && users[j].numGrupo > 0){
                if(users[j].numGrupo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede cambair el grupo a este user\n");
                    break;
                }
                for(int k = 0 ; k < nGrupos ; k++){
                    if(strcmp(grupos[k].name,grupoN) == 0 && grupos[k].numGrupo > 0){
                        estado = 1;
                        memset(&users[j].group,0,999);
                        strcpy(users[j].group, grupoN);
                        break;
                    }
                }
                if(estado == 1){
                    break;
                }
            }
        }
        if(estado == 1){
            char textoNuevo[9999] = {0};
            int posU = 0;
            int posG = 0;
            while(users[posU].position != -1 || grupos[posG].position != -1){
                if((grupos[posG].position < users[posU].position) && strlen(grupos[posG].name) > 0){
                    sprintf(textoNuevo,"%s%i,G,%s\n",textoNuevo,grupos[posG].numGrupo,grupos[posG].name); //Ingresar Grupo
                    posG++;
                }
                if((grupos[posG].position > users[posU].position) && strlen(users[posU].name) > 0){
                    sprintf(textoNuevo,"%s%i,U,%s,%s,%s\n",textoNuevo,users[posU].numGrupo,users[posU].group,users[posU].name,users[posU].pass); //Ingresar user
                    posU++;
                }
                if(grupos[posG].position != -1 && users[posU].position == -1 && strlen(grupos[posG].name) > 0){
                    sprintf(textoNuevo,"%s%i,G,%s\n",textoNuevo,grupos[posG].numGrupo,grupos[posG].name); //Ingresar Grupo
                    posG++;
                }
                if(grupos[posG].position == -1 && users[posU].position != -1 && strlen(users[posU].name) > 0){
                    sprintf(textoNuevo,"%s%i,U,%s,%s,%s\n",textoNuevo,users[posU].numGrupo,users[posU].group,users[posU].name,users[posU].pass); //Ingresar user
                    posU++;
                }
            }
            modificarArchivo(userFile.position, textoNuevo, userFile.contenido, archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio de grupo a user %s \n" , userV);
            //JOURNAL CAMBIAR GRUPO user
            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                journal entrada;
                entrada = initJournal(entrada);
                entrada.j_uid = usuariologueadop.id_user;

                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                strcpy(entrada.j_name,userV);
                strcpy(entrada.j_content,grupoN);

                entrada.j_perm = 664;
                entrada.j_type = '1';
                entrada.j_op_type = '4';

                int nextJor = nextJournal(archivoDisco, sp_aux.sb_bm_inode);
                writeJournal(entrada,archivoDisco,nextJor);
            }
            return 1;
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No es posible hacer el cambio de grupo \n");
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarARCHIVO(char pathVirtual[], int sizeFile, char contentFile[], int contentFileExists, int genPadres, int operationType, char rename[], FILE *archivoDisco){
    //Obtecion de cantidad de '/'
    int n = 0;
    for(int i = 0; i < strlen(pathVirtual) ; i++){
        if(pathVirtual[i] == '/'){
            n++;
        }
    }
    //Archivo logico
    archivo finalFile;
    memset(&finalFile.contenido,0,280320);
    //Verificar si es ruta valida
    if(pathVirtual[0] != '/'){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ruta no valida\n");
        exitoArchivo = false;
        return -1;
    }
    if(rename[0] == '/' && operationType != 7 && operationType != 8 && operationType != 9 && operationType != 10 && operationType != 11){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Nombre nuevo de archivo no debe empezar con '/'\n");
        exitoArchivo = false;
        return -1;
    }
    //Eliminar ultimo '/' si es carpeta
    if(strlen(pathVirtual) > 1){
        if(pathVirtual[strlen(pathVirtual)-1] == '/'){
            pathVirtual[strlen(pathVirtual)-1] = '\0';
        }
    }
    //Verificacion de tipo de archivo
    int esArchivo;
    esArchivo = verificarExtension2(pathVirtual);
    if(esArchivo == -1 && operationType != 3 && operationType != 4 && operationType != 6 && operationType != 7 && operationType != 8 && operationType != 9 && operationType != 10 && operationType != 11){
        //Si es carpeta y no se quiere eliminar o modificar permisos 0 renombrar
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Tipo de archivo y Tipo de accion no validos\n");
        exitoArchivo = false;
        return -1;
    }
    //Revision de acciones de contenido (Existe size o existe contentFile)
    if(operationType == 0 || operationType == 5){
        if(contentFileExists == 1){
            //Existe archivo que contiene la informacion
             finalFile = obtenerContent(contentFile);
             if(strcmp(finalFile.contenido,"")==0) {
                 strcpy(finalFile.contenido,contentFile);
             }
                sizeFile = strlen(finalFile.contenido);

         }else{
            //No existe archivo con informacion
            for(int i = 0; i < sizeFile ; i++){
                //Operador % para size mayores a 10, convierte el valor final a un rango de 0 a 9
                sprintf(finalFile.contenido,"%s%i",finalFile.contenido,i%10);
            }
        }
        //Error de size de archivo, debe estar aqui
        if(sizeFile < 0){
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede generar archivo\n");
            exitoArchivo = false;
            return -1;
        }
    }
    //Ejecucion de comando
    if( existsFile(usuariologueadop.pathDisco) == 1){
        int start=0;
        int cantidad=0;
        SuperBloque sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
        if(sp_aux.sb_magic == 61267){
            if(archivoDisco != NULL){
                inode inodoM;

                start = sp_aux.sb_bm_inode;
                cantidad = sp_aux.sb_inodes_count;
                fseek(archivoDisco , start, SEEK_SET);
                char bm_iNode[cantidad];
                char aux_c;
                for(int i =0 ; i < cantidad; i++){
                    fread(&aux_c, 1 , 1 , archivoDisco);
                    bm_iNode[i] = aux_c;
                }
                start = sp_aux.sb_bm_Block;
                cantidad = sp_aux.sb_blocks_count;
                fseek(archivoDisco , start, SEEK_SET);
                char bm_Block[cantidad];
                for(int i =0 ; i < cantidad; i++){
                    fread(&aux_c, 1 , 1 , archivoDisco);
                    bm_Block[i] = aux_c;
                }
                start = sp_aux.sb_t_inode;
                fseek(archivoDisco,start, SEEK_SET);
                fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                int posInodo = 0;
                int i = 1;
                char aux[9999]= {0};
                while(pathVirtual[i] != NULL){
                    char aux_char[]= {pathVirtual[i],'\0'};
                    if(pathVirtual[i] == '/'){
                        int auxfind = findArchivo(aux , inodoM, 0, archivoDisco);
                        if(auxfind  == -1){
                            if(genPadres == 0){
                                if( operationType == 0 ){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta %s no existe, no se puede crear %s\n" , aux, pathVirtual);
                                    exitoArchivo = false;
                                    return -1;
                                }else if(operationType == 1){
                                    int sePuedeGenerar = 1;
                                    for(int x = i + 1; pathVirtual[x] != NULL ; x++ ){
                                        if(pathVirtual[x] == '/'){
                                            sePuedeGenerar = -1;
                                            for(int h = x; h < strlen(pathVirtual) ; h++){
                                                pathVirtual[h] = '\0';
                                            }
                                            break;
                                        }
                                    }
                                    if(sePuedeGenerar == 1){
                                        if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'w') == -1){
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de escritura en la carpeta\n");
                                            exitoArchivo = false;
                                            return -1;
                                        }
                                        int nd = crearCarpeta(aux, inodoM , posInodo, archivoDisco);
                                        fseek(archivoDisco, getiNodeStart_2(nd, usuariologueadop.startiNodes), SEEK_SET);
                                        fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                                        posInodo = nd;
                                        escribirBitMap(archivoDisco , sp_aux);

                                        return 1;
                                    }else{
                                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta %s no existe, no se puede crear %s\n" , aux, pathVirtual);
                                        exitoArchivo = false;
                                        return -1;
                                    }
                                }else if(operationType == 2){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta %s no existe, no se puede acceder a %s\n" , aux, pathVirtual);
                                    exitoArchivo = false;
                                    return -1;
                                }else if(operationType == 3){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta %s no existe, no se pude eliminar %s\n" , aux, pathVirtual);
                                    exitoArchivo = false;
                                    return -1;
                                }else if(operationType == 4){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta %s no existe, no se puede cambiar permisos %s\n" , aux , pathVirtual);
                                    exitoArchivo = false;
                                    return -1;
                                }
                            }
                            if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'w') == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de escritura en la carpeta\n");
                                exitoArchivo = false;
                                return -1;
                            }
                            int nd = crearCarpeta(aux, inodoM , posInodo, archivoDisco);
                            fseek(archivoDisco, getiNodeStart_2(nd, usuariologueadop.startiNodes), SEEK_SET);
                            fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                            posInodo = nd;
                            escribirBitMap(archivoDisco , sp_aux);
                        }else{
                            fseek(archivoDisco, getiNodeStart_2(auxfind, usuariologueadop.startiNodes), SEEK_SET);
                            fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                            posInodo = auxfind;
                        }
                        memset(&aux,0,9999);
                    }
                    else{
                        strcat(aux, aux_char);
                    }
                    i++;
                }
                if(operationType == 1) {
                    //la generacion de carpetas esta antes de esto
                    //Si llega aqui es por que solo se deseaban generar carpetas
                    //JOURNAL arriba

                    return 1;
                }
                if(operationType != 3 && operationType != 4 && operationType != 5 && operationType != 6 && operationType != 7 && operationType != 8 && operationType != 9 && operationType != 10 && operationType != 11){
                    //Escritura de archivo
                    archivo finalFile_2 = getArchivo(aux,usuariologueadop.partUser, usuariologueadop.startiNodes ,getiNodeStart_2(posInodo, usuariologueadop.startiNodes),archivoDisco);
                    if(operationType == 0 && finalFile_2.estado == -1){
                        if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'w') == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se tienen permisos de escritura dentro de esta carpeta, no puede generar archivo\n");
                            exitoArchivo = false;
                            return -1;
                        }
                        time_t tiempoActual = time(NULL);
                        struct tm *fecha = localtime(&tiempoActual);
                        char tiempo[16];
                        strftime(tiempo, sizeof(tiempo)-1, "%d/%m/%y %H:%M", fecha);
                        int niNode = nextFree(usuariologueadop.bm_iNode, 0);
                        if(niNode == -1 ){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sin iNodos para escritura\n");
                            exitoArchivo = false;
                            return -1;
                        }
                        if(strlen(aux) > 11){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: nombre de archivo muy extenso\n");
                            exitoArchivo = false;
                            return -1;
                        }
                        positionArchivo(aux, inodoM, niNode, posInodo, sp_aux, archivoDisco);
                        fillArchivo(niNode, finalFile.contenido , usuariologueadop.bm_Block , usuariologueadop.startiNodes , sp_aux.sb_bm_Block , tiempo , 664, archivoDisco);
                    }
                    finalFile_2 = getArchivo(aux,usuariologueadop.partUser, usuariologueadop.startiNodes ,getiNodeStart_2(posInodo, usuariologueadop.startiNodes), archivoDisco);
                    int test_1 = strlen(finalFile_2.contenido);
                    int test_2 = strlen(finalFile.contenido);
                    bool test = (strlen(finalFile_2.contenido) != strlen(finalFile.contenido));
                    if(finalFile_2.estado == 1 && (operationType == 2 || (operationType == 0 && test == 0) )){
                        if(operationType == 2){
                            if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'r') == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No tiene permisos de lectura sobre este archivo\n");
                                exitoArchivo = false;
                                return -1;
                            }
                            printf("\033[1;105m COMANDO REALIZADO \033[0m %s:\n|_____%s\n",pathVirtual,  finalFile_2.contenido);
                            return 1;
                        }
                        else if(operationType == 0){
                            //Escritura de archivo (No duplicados)
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero archivo '%s'\n",pathVirtual);

                            return 1;
                        }else{
                            printf("\033[101m BAIA BAIA ERROR\033[0m No se pudo generar el archivo '%s'\n",pathVirtual);
                            exitoArchivo = false;
                        }
                    }else{
                        exitoArchivo = false;
                        if(operationType == 0 && test == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo ya existe '%s'\n",pathVirtual);
                            exitoArchivo = false;
                            return -1;
                        }
                        if(operationType == 2){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo no existe '%s'\n",pathVirtual);
                            exitoArchivo = false;
                            return -1;
                        }
                    }
                }else{
                    if(esArchivo == -1){
                        //Trabajo de eliminacion o permisos en carpetas o renombrar
                        int auxfind = findArchivo(aux , inodoM, 0, archivoDisco);
                        if(auxfind == -1 && strcmp(pathVirtual,"/") != 0){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Carpeta no se ha encontrado\n");
                            exitoArchivo = false;
                            return -1;
                        }
                        if(strcmp(aux,"")==0){
                            auxfind = 0;
                        }
                        if(operationType == 3){
                            //Eliminar carpeta
                            if(modifyiNodeCarpet(auxfind , start, archivoDisco , -1, 1) == 0){
                                findArchivo(aux , inodoM, 1, archivoDisco);
                                printf("\033[1;105m COMANDO REALIZADO \033[0m Carpeta eliminada (Recursivo)\n");
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se tiene permisos para eliminar la carpeta\n");
                                exitoArchivo = false;
                                return -1;
                            }
                            escribirBitMap(archivoDisco , sp_aux);
                            return 1;
                        }else if(operationType == 4){
                            //Permisos carpeta
                            if(auxfind == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede modificar permisos desde carpeta raiz");
                                return -1;
                            }
                            if(modifyiNodeCarpet(auxfind , start, archivoDisco , sizeFile, genPadres) == 0){
                                if(genPadres == 1){
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambiaron los permisos de '%s' (recursivo a los posibles)\n",pathVirtual);
                                    return 1;
                                }else{
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambiaron los permisos de '%s'\n",pathVirtual);
                                    return 1;
                                }
                            }
                        }else if(operationType == 6){
                            //Seguro de existencia de carpeta 'rename'
                            int auxfindDirectory = findArchivo(rename , inodoM, 0, archivoDisco);
                            if(auxfindDirectory != -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Carpeta con nombre '%s' ya existe en directorio\n",rename);
                                exitoArchivo = false;
                                return -1;
                            }
                            inode inodoAux;
                            fseek(archivoDisco,getiNodeStart_2(auxfind,usuariologueadop.startiNodes),SEEK_SET);
                            fread(&inodoAux,sizeof(inode),1,archivoDisco);
                            if(checkPermisos(inodoAux.i_uid,inodoAux.i_gid,inodoAux.i_perm,'w') == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se poseen permisos de escritura sobre carpeta\n");
                                exitoArchivo = false;
                                return -1;
                            }
                            if(verificarExtension2(rename) != 0){
                                int puede_cambiar_nombre = 1;
                                for(int y = 0 ; y < 15 ; y++ ){
                                    if(inodoM.i_block[y] == -1){
                                        continue;
                                    }
                                    if(y < 12){
                                        contentBlock archivosiNodo;
                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                        for( int x = 0 ; x < 4 ; x++ ){
                                            if( strcmp(rename,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                puede_cambiar_nombre = -1;
                                                exitoArchivo = false;
                                            }
                                        }
                                        if(puede_cambiar_nombre == 1){
                                            for( int x = 0 ; x < 4 ; x++ ){
                                                if( strcmp(aux,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                    memset(&archivosiNodo.contenapunta[x].f_name,0,12);
                                                    strcpy(archivosiNodo.contenapunta[x].f_name,rename);

                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                    return 1;
                                                }
                                            }
                                        }
                                    }else{
                                        //Indirectos
                                        if(y == 12){
                                            //Indirecto
                                            pointerBlock pb1;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb1,sizeof(pb1),1,archivoDisco);
                                            for( int z = 0 ; z < 16 ; z++ ){
                                                if(pb1.pb_pointers[z] == -1){
                                                    continue;
                                                }
                                                contentBlock archivosiNodo;
                                                int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                for( int a = 0 ; a < 4 ; a++ ){
                                                    if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                        puede_cambiar_nombre = -1;
                                                    }
                                                }
                                                if(puede_cambiar_nombre == 1){
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                            memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                            strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                            return 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }else{
                                            if(y == 13){
                                                //Doble indirecto
                                                pointerBlock pb2;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb2,sizeof(pb2),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb2.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    pointerBlock pb2_2;
                                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                    for (int w = 0 ; w < 16 ; w++ ){
                                                        if(pb2_2.pb_pointers[w] == -1){
                                                            continue;
                                                        }
                                                        contentBlock archivosiNodo;
                                                        int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                        for( int a = 0 ; a < 4 ; a++ ){
                                                            if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                puede_cambiar_nombre = -1;
                                                            }
                                                        }
                                                        if(puede_cambiar_nombre == 1){
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                    memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                    strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                    fseek(archivoDisco, offset_3, SEEK_SET);
                                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                                    return 1;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }else{
                                                if(y == 14){
                                                    //Triple indirecto
                                                    pointerBlock pb3;
                                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                    for( int z = 0 ; z < 16 ; z++ ){
                                                        if(pb3.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb3_2;
                                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_2, SEEK_SET);
                                                        fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                        for (int w = 0 ; w < 16 ; w++ ){
                                                            if(pb3_2.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            pointerBlock pb3_2_1;
                                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_3, SEEK_SET);
                                                            fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                            for (int v = 0 ; v < 16 ; v++ ){
                                                                if(pb3_2_1.pb_pointers[z] == -1){
                                                                    continue;
                                                                }
                                                                contentBlock archivosiNodo;
                                                                int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                                fseek(archivoDisco, offset_4, SEEK_SET);
                                                                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                for( int a = 0 ; a < 4 ; a++ ){
                                                                    if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                        puede_cambiar_nombre = -1;
                                                                    }
                                                                }
                                                                if(puede_cambiar_nombre == 1){
                                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                                        if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                            memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                            strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                            fseek(archivoDisco, offset_4, SEEK_SET);
                                                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                                            return 1;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' no existe \n",pathVirtual);
                                                exitoArchivo = false;
                                                return -1;
                                            }
                                        }
                                    }
                                }
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: carpeta no puede tener tipo de extension\n");
                                exitoArchivo = false;
                                return -1;
                            }
                        }else if(operationType == 7){
                            //Copiar carpeta
                            contentBlock tempCopia;
                            int auxCola = 0;
                            int auxX = 0;
                            userstats ColaArchivos[100];  // Introducir los iNodos aqui
                            int aux_carpeta_real = findArchivo(aux,inodoM,0,archivoDisco);
                            if(aux_carpeta_real == -1){
                                //Algo salio mal
                                exitoArchivo = false;
                                return -1;
                            }
                            fseek(archivoDisco,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),SEEK_SET);
                            fread(&inodoM,sizeof (inode),1,archivoDisco);
                            if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'r') == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de lectura sobre carpeta\n");
                                return -1;
                            }
                            for(int x = 0 ; x < 100 ; x++ ){
                                ColaArchivos[x].numGrupo = -1;
                                memset(&ColaArchivos[x].name,0,999 );
                            }
                            //Empezar analisis de contenido de carpeta
                            do{
                                if(auxX < 12){
                                    //Directos
                                    fseek(archivoDisco,getBlockStart_2(inodoM.i_block[auxX],usuariologueadop.startBlocks),SEEK_SET);
                                    fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                    for(int k = 0 ; k < 4 ; k++ ){
                                        if(tempCopia.contenapunta[k].f_inode != -1){
                                            if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                //Es un archivo dentro de la carpeta
                                                ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                auxCola++;
                                            }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                //Si es una carpeta
                                                if(strcmp(tempCopia.contenapunta[k].f_name,"") !=0){
                                                    ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                    strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                    ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                    auxCola++;
                                                }
                                            }
                                        }
                                    }
                                }else{
                                    if(auxX == 12){
                                        //Indirecto
                                        pointerBlock pb1;
                                        int offset = getBlockStart_2(inodoM.i_block[auxX] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb1.pb_pointers[a] != -1){
                                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],usuariologueadop.startiNodes),SEEK_SET);
                                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                for(int k = 0 ; k < 4 ; k++ ){
                                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                            //Es un archivo dentro de la carpeta
                                                            ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                            strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                            ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                            auxCola++;
                                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                            //Si es una carpeta
                                                            if(strcmp(tempCopia.contenapunta[k].f_name,"") !=0){
                                                                ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                                auxCola++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }else{
                                        if(auxX == 13){
                                            //Doble Indirecto
                                            pointerBlock pb2;
                                            int offset = getBlockStart_2(inodoM.i_block[auxX] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                                            for( int a = 0 ; a < 16 ; a ++){
                                                if(pb2.pb_pointers[a] != -1){
                                                    pointerBlock pb2_1;
                                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                                    for(int b = 0; b < 16 ; b++){
                                                        fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[a],usuariologueadop.startBlocks),SEEK_SET);
                                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                        for(int k = 0 ; k < 4 ; k++ ){
                                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                                    //Es un archivo dentro de la carpeta
                                                                    ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                    strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                    ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                                    auxCola++;
                                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                                    //Si es una carpeta
                                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"") !=0){
                                                                        ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                        strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                        ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                                        auxCola++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }else{
                                            if(auxX == 14){
                                                //Triple indirecto
                                                pointerBlock pb3;
                                                int offset = getBlockStart_2(inodoM.i_block[auxX] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                for( int a = 0 ; a < 16 ; a ++){
                                                    if(pb3.pb_pointers[a] != -1){
                                                        pointerBlock pb3_2;
                                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                        for(int b = 0; b < 16 ; b++){
                                                            pointerBlock pb3_2_1;
                                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],usuariologueadop.startBlocks);
                                                            fseek(archivoDisco,offset_3,SEEK_SET);
                                                            fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                            for(int c = 0 ; c < 16 ; c++ ){
                                                                fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],usuariologueadop.startBlocks),SEEK_SET);
                                                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                                for(int k = 0 ; k < 4 ; k++ ){
                                                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                                            //Es un archivo dentro de la carpeta
                                                                            ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                            strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                            ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                                            auxCola++;
                                                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                                            //Si es una carpeta
                                                                            if(strcmp(tempCopia.contenapunta[k].f_name,"") !=0){
                                                                                ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                                strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                                ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                                                auxCola++;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                //revisar por mas elementos para copiar
                                auxX++;
                            }while(inodoM.i_block[auxX] != -1 && auxX < 15);
                            //Termino analisis
                            if(auxCola > 0){
                                //Hay contenido para copiar
                                for( int l = 0 ; l < auxCola ; l++ ){
                                    if(ColaArchivos[l].numGrupo == 0){
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se intento copiar carpeta de '%s' en '%s'\n",pathVirtual,rename);
                                        //Generar carpeta y sus contenidos
                                        char tempCopiaCarpeta[999] = {0};

                                        sprintf(tempCopiaCarpeta,"%s/%s/%s",rename,aux,"x.txt");
                                        //Copiar carpeta original (si no existe)
                                        if(ejecutarARCHIVO(tempCopiaCarpeta,1,"",0,0,1,"",archivoDisco) != -1){
                                            sprintf(tempCopiaCarpeta,"%s/%s/%s/%s",rename,aux,ColaArchivos[l].name,"x.txt");

                                            //Copiar carpeta actual
                                            if(ejecutarARCHIVO(tempCopiaCarpeta,1,"",0,0,1,"",archivoDisco) != -1){
                                                memset(&tempCopiaCarpeta,0,999);
                                                sprintf(tempCopiaCarpeta,"%s/%s",pathVirtual,ColaArchivos[l].name);
                                                char rename_nuevo[999];
                                                memset(&rename_nuevo,0,999);
                                                sprintf(rename_nuevo,"%s/%s",rename,aux);
                                                ejecutarARCHIVO(tempCopiaCarpeta,1,"",0,0,7,rename_nuevo,archivoDisco);
                                            }
                                        }
                                    }
                                    if(ColaArchivos[l].numGrupo == 1){
                                        //Generar archivo
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se intento copiar archivo de '%s' en '%s'\n",pathVirtual,rename);
                                        char tempCopiaArchivo[999] = {0};

                                        sprintf(tempCopiaArchivo,"%s/%s/%s",rename,aux,"x.txt");
                                        //Copiar carpeta original (si no existe)
                                        if(ejecutarARCHIVO(tempCopiaArchivo,1,"",0,0,1,"",archivoDisco) != -1){
                                            memset(&tempCopiaArchivo,0,999);
                                            sprintf(tempCopiaArchivo,"%s/%s/%s",rename,aux,ColaArchivos[l].name);

                                            archivo tempCopiaCarpeta;
                                            tempCopiaCarpeta = getArchivo(ColaArchivos[l].name,usuariologueadop.partUser,usuariologueadop.startiNodes,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),archivoDisco);
                                            if(tempCopiaCarpeta.estado != -1){
                                                if(checkPermisos(tempCopiaCarpeta.iduser,tempCopiaCarpeta.idGrupo,tempCopiaCarpeta.permArch,'r') == -1){
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de lectura del archivo\n");
                                                    return -1;
                                                }else{
                                                    char pathAuxFile2[] = "/home/edi/auxCopy.txt";
                                                    FILE *secundario;
                                                    secundario = fopen(pathAuxFile2,"w");
                                                    if(secundario != NULL){
                                                        fprintf(secundario,"%s",tempCopiaCarpeta.contenido);
                                                        ejecutarARCHIVO(tempCopiaArchivo,0,pathAuxFile2,1,0,0,"",archivoDisco);
                                                    }else{
                                                        /*SI ALGO FALLA*/
                                                        exitoArchivo = false;
                                                    }
                                                }
                                            }else{
                                                /*SI ALGO FALLA*/
                                                exitoArchivo = false;
                                            }
                                        }
                                    }
                                }
                                return 1;
                            }else{
                                //Solo se copia una carpeta
                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se intento copiar carpeta '%s' en '%s'\n",pathVirtual,rename);
                                char tempCarpeta[999] = {0};
                                sprintf(tempCarpeta,"%s/%s/%s",rename,aux,"x.txt");
                                ejecutarARCHIVO(tempCarpeta,1,"",0,0,1,"",archivoDisco);
                                return 1;
                            }
                        }else if(operationType == 8){
                            //Mover Carpeta de aqui
                            if(verificarExtension2(rename) != 0){
                                inode inodoAux;
                                int auxfind = findArchivo(aux , inodoM, 0,archivoDisco);
                                fseek(archivoDisco,getiNodeStart_2(auxfind,usuariologueadop.startiNodes),SEEK_SET);
                                fread(&inodoAux,sizeof (inode),1,archivoDisco);
                                if(checkPermisos(inodoAux.i_uid,inodoAux.i_gid,inodoAux.i_perm,'r') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: no posee permisos de lectura sobre la carpeta\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                for(int y = 0 ; y < 15 ; y++ ){
                                    if(inodoM.i_block[y] == -1){
                                        continue;
                                    }
                                    if(y < 12){
                                        contentBlock archivosiNodo;
                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                        for( int x = 0 ; x < 4 ; x++ ){
                                            if( strcmp(aux,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                char rename2[999] = {0};
                                                sprintf(rename2,"%s",archivosiNodo.contenapunta[x].f_name);
                                                sizeFile = archivosiNodo.contenapunta[x].f_inode;

                                                archivosiNodo.contenapunta[x].f_inode = -1;
                                                memset(&archivosiNodo.contenapunta[x].f_name,0,12);

                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                return 1;
                                            }
                                        }
                                    }else{
                                        //Indirectos
                                        if(y == 12){
                                            //Indirecto
                                            pointerBlock pb1;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb1,sizeof(pb1),1,archivoDisco);
                                            for( int z = 0 ; z < 16 ; z++ ){
                                                if(pb1.pb_pointers[z] == -1){
                                                    continue;
                                                }
                                                contentBlock archivosiNodo;
                                                int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                for( int a = 0 ; a < 4 ; a++ ){
                                                    if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                        char rename2[999] = {0};
                                                        sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                        sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                        archivosiNodo.contenapunta[a].f_inode = -1;
                                                        memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                        fseek(archivoDisco, offset, SEEK_SET);
                                                        fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                        ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                        return 1;
                                                    }
                                                }
                                            }
                                        }else{
                                            if(y == 13){
                                                //Doble indirecto
                                                pointerBlock pb2;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb2,sizeof(pb2),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb2.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    pointerBlock pb2_2;
                                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                    for (int w = 0 ; w < 16 ; w++ ){
                                                        if(pb2_2.pb_pointers[w] == -1){
                                                            continue;
                                                        }
                                                        contentBlock archivosiNodo;
                                                        int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                        fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                        for( int a = 0 ; a < 4 ; a++ ){
                                                            if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                char rename2[999] = {0};
                                                                sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                                sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                                archivosiNodo.contenapunta[a].f_inode = -1;
                                                                memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                                fseek(archivoDisco, offset, SEEK_SET);
                                                                fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                                return 1;
                                                            }
                                                        }
                                                    }
                                                }
                                            }else{
                                                if(y == 14){
                                                    //Triple indirecto
                                                    pointerBlock pb3;
                                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                    for( int z = 0 ; z < 16 ; z++ ){
                                                        if(pb3.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb3_2;
                                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_2, SEEK_SET);
                                                        fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                        for (int w = 0 ; w < 16 ; w++ ){
                                                            if(pb3_2.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            pointerBlock pb3_2_1;
                                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_3, SEEK_SET);
                                                            fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                            for (int v = 0 ; v < 16 ; v++ ){
                                                                if(pb3_2_1.pb_pointers[z] == -1){
                                                                    continue;
                                                                }
                                                                contentBlock archivosiNodo;
                                                                int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                                fseek(archivoDisco, offset_4, SEEK_SET);
                                                                fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                for( int a = 0 ; a < 4 ; a++ ){
                                                                    if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                        char rename2[999] = {0};
                                                                        sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                                        sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                                        archivosiNodo.contenapunta[a].f_inode = -1;
                                                                        memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                                        fseek(archivoDisco, offset, SEEK_SET);
                                                                        fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                        ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                                        return 1;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' no existe \n",pathVirtual);
                                                exitoArchivo = false;
                                                return -1;
                                            }
                                        }
                                    }
                                }
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ubicacion final  no puede ser de tipo de extension (.txt)\n");
                                exitoArchivo = false;
                                return -1;
                            }
                        }else if(operationType == 9){
                            //Mover Archivo o Carpeta aqui
                            int aux_carpeta_real = findArchivo(aux,inodoM,0,archivoDisco);
                            if(aux_carpeta_real == -1){
                                //Algo salio mal
                                exitoArchivo = false;
                                return -1;
                            }
                            fseek(archivoDisco,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),SEEK_SET);
                            fread(&inodoM,sizeof (inode),1,archivoDisco);
                            if(checkPermisos(inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,'w') == -1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se tienen permisos de escritura sobre carpeta\n");
                                exitoArchivo = false;
                                return -1;
                            }
                            for(int y = 0 ; y < 15 ; y++ ){
                                if(inodoM.i_block[y] == -1){
                                    continue;
                                }
                                if(y < 12){
                                    contentBlock archivosiNodo;
                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                    fseek(archivoDisco, offset, SEEK_SET);
                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                    for( int x = 0 ; x < 4 ; x++ ){
                                        if( archivosiNodo.contenapunta[x].f_inode == -1 ){
                                            archivosiNodo.contenapunta[x].f_inode = sizeFile;
                                            memset(&archivosiNodo.contenapunta[x].f_name,0,12);
                                            strcpy(archivosiNodo.contenapunta[x].f_name,rename);

                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);

                                            inode inodoA;
                                            fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[x].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                            fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                            contentBlock auxiNodoA;
                                            fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                            fread(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);
                                            auxiNodoA.contenapunta[0].f_inode = aux_carpeta_real;
                                            fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                            fwrite(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);

                                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se movio archivo/carpeta '%s' a '%s'\n",rename,pathVirtual);

                                            return 1;
                                        }
                                    }
                                }else{
                                    //Indirectos
                                    if(y == 12){
                                        //Indirecto
                                        pointerBlock pb1;
                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                                        for( int z = 0 ; z < 16 ; z++ ){
                                            if(pb1.pb_pointers[z] == -1){
                                                continue;
                                            }
                                            contentBlock archivosiNodo;
                                            int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                            for( int a = 0 ; a < 4 ; a++ ){
                                                if( archivosiNodo.contenapunta[a].f_inode == -1 ){
                                                    archivosiNodo.contenapunta[a].f_inode = sizeFile;
                                                    memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                    strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);

                                                    inode inodoA;
                                                    fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                    fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                    contentBlock auxiNodoA;
                                                    fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                    fread(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);
                                                    auxiNodoA.contenapunta[0].f_inode = aux_carpeta_real;
                                                    fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                    fwrite(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);

                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se movio archivo/carpeta '%s' a '%s'\n",rename,pathVirtual);

                                                    return 1;
                                                }
                                            }
                                        }
                                    }else{
                                        if(y == 13){
                                            //Doble indirecto
                                            pointerBlock pb2;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                                            for( int z = 0 ; z < 16 ; z++ ){
                                                if(pb2.pb_pointers[z] == -1){
                                                    continue;
                                                }
                                                pointerBlock pb2_2;
                                                int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                for (int w = 0 ; w < 16 ; w++ ){
                                                    if(pb2_2.pb_pointers[w] == -1){
                                                        continue;
                                                    }
                                                    contentBlock archivosiNodo;
                                                    int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_3, SEEK_SET);
                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if( archivosiNodo.contenapunta[a].f_inode == -1 ){
                                                            archivosiNodo.contenapunta[a].f_inode = sizeFile;
                                                            memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                            strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                            fseek(archivoDisco, offset, SEEK_SET);
                                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);

                                                            inode inodoA;
                                                            fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                            fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                            contentBlock auxiNodoA;
                                                            fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                            fread(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);
                                                            auxiNodoA.contenapunta[0].f_inode = aux_carpeta_real;
                                                            fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                            fwrite(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);

                                                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se movio archivo/carpeta '%s' a '%s'\n",rename,pathVirtual);

                                                            return 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }else{
                                            if(y == 14){
                                                //Triple indirecto
                                                pointerBlock pb3;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb3.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    pointerBlock pb3_2;
                                                    int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                    for (int w = 0 ; w < 16 ; w++ ){
                                                        if(pb3_2.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb3_2_1;
                                                        int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                        fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                        for (int v = 0 ; v < 16 ; v++ ){
                                                            if(pb3_2_1.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            contentBlock archivosiNodo;
                                                            int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_4, SEEK_SET);
                                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if( archivosiNodo.contenapunta[a].f_inode == -1 ){
                                                                    archivosiNodo.contenapunta[a].f_inode = sizeFile;
                                                                    memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                    strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);

                                                                    inode inodoA;
                                                                    fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                                    fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                                    contentBlock auxiNodoA;
                                                                    fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                                    fread(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);
                                                                    auxiNodoA.contenapunta[0].f_inode = aux_carpeta_real;
                                                                    fseek(archivoDisco,getBlockStart_2(inodoA.i_block[0],usuariologueadop.startBlocks),SEEK_SET);
                                                                    fwrite(&auxiNodoA,sizeof(auxiNodoA),1,archivoDisco);

                                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se movio archivo/carpeta '%s' a '%s'\n",rename,pathVirtual);

                                                                    return 1;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' no existe \n",pathVirtual);
                                            exitoArchivo = false;
                                            return -1;
                                        }
                                    }
                                }
                            }
                        }else if(operationType == 10){
                            //Find desde carpeta, inodo de carpeta
                            char salidaRecorrido[99999] = {0};
                            memset(&salidaRecorrido,0,99999);
                            char auxRecorrido[999] = {0};
                            memset(&salidaRecorrido,0,999);
                            int tab = 0;
                            int aux_carpeta_real = findArchivo(aux,inodoM,0,archivoDisco); //Inodo De la Carpeta Deseada
                            if(aux_carpeta_real == -1 && strcmp(pathVirtual,"/") != 0){
                                //Algo salio mal
                                return -1;
                            }
                            //Recorrer a raiz(?)
                            sprintf(salidaRecorrido,"/\n");
                            for(int a = 1 ; a < strlen(pathVirtual) ; a++ ){
                                sprintf(auxRecorrido,"%s%c",auxRecorrido,pathVirtual[a]);
                                if(pathVirtual[a] == '/'){
                                    for(int b = 0 ; b < tab ; b++ ){
                                        sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                    }
                                    sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,auxRecorrido);
                                    memset(&auxRecorrido,0,999);
                                    tab++;
                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                }
                            }
                            if(strlen(auxRecorrido) > 0 ){
                                sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,auxRecorrido);
                                memset(&auxRecorrido,0,999);
                                tab++;
                                sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                            }
                            //Avanzar a dicha carpeta
                            if(aux_carpeta_real > -1){
                                fseek(archivoDisco,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),SEEK_SET);
                                fread(&inodoM,sizeof (inode),1,archivoDisco);
                            }
                            //Generar expresion regular (Ya fue validada)
                            regex_t expresionFind;
                            int estadoExpresionFind = -1;
                            estadoExpresionFind = regcomp(&expresionFind,rename,REG_EXTENDED);
                            if(estadoExpresionFind){
                                //Algo salio mal
                                return -1;
                            }
                            //Revisar Todos los Archivos
                            for(int y = 0 ; y < 15 ; y++ ){
                                if(inodoM.i_block[y] == -1){
                                    continue;
                                }
                                if(y < 12){
                                    contentBlock archivosiNodo;
                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                    fseek(archivoDisco, offset, SEEK_SET);
                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                    for( int x = 0 ; x < 4 ; x++ ){
                                        if(archivosiNodo.contenapunta[x].f_inode != -1){
                                            if( verificarExtension2(archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                //Es un archivo
                                                if(!regexec(&expresionFind,archivosiNodo.contenapunta[x].f_name,0,NULL,0)){
                                                    //Cumple el regex
                                                    for(int b = 0 ; b < tab ; b++ ){
                                                        sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                    }
                                                    sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[x].f_name);
                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                }
                                            }else if( strcmp(archivosiNodo.contenapunta[x].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[x].f_name,"..") != 0 ){
                                                //Es una carpeta
                                                if(strcmp(archivosiNodo.contenapunta[x].f_name,"") != 0){
                                                    for(int b = 0 ; b < tab ; b++ ){
                                                        sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                    }
                                                    sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[x].f_name);
                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                    tab++;
                                                    char *temp = auxFindCarpeta(archivosiNodo.contenapunta[x].f_inode, rename, tab, archivoDisco);
                                                    char temp2[9999] = {0};
                                                    strcpy(temp2,temp);
                                                    sprintf( salidaRecorrido,"%s%s",salidaRecorrido, temp2 );
                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                    tab--;
                                                    free(temp);
                                                }
                                            }
                                        }
                                    }
                                }else{
                                    //Indirectos
                                    if(y == 12){
                                        //Indirecto
                                        pointerBlock pb1;
                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                                        for( int z = 0 ; z < 16 ; z++ ){
                                            if(pb1.pb_pointers[z] == -1){
                                                continue;
                                            }
                                            contentBlock archivosiNodo;
                                            int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                            for( int a = 0 ; a < 4 ; a++ ){
                                                if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                    if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                        //Es un archivo
                                                        if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                                            //Cumple el regex
                                                            for(int b = 0 ; b < tab ; b++ ){
                                                                sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                            }
                                                            sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                        }
                                                    }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                        //Es una carpeta
                                                        if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                                            for(int b = 0 ; b < tab ; b++ ){
                                                                sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                            }
                                                            sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                            tab++;
                                                            char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, rename, tab,archivoDisco);
                                                            char temp2[9999] = {0};
                                                            strcpy(temp2,temp);
                                                            sprintf( salidaRecorrido,"%s%s",salidaRecorrido, temp2 );
                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                            tab--;
                                                            free(temp);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }else{
                                        if(y == 13){
                                            //Doble indirecto
                                            pointerBlock pb2;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                                            for( int z = 0 ; z < 16 ; z++ ){
                                                if(pb2.pb_pointers[z] == -1){
                                                    continue;
                                                }
                                                pointerBlock pb2_2;
                                                int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                for (int w = 0 ; w < 16 ; w++ ){
                                                    if(pb2_2.pb_pointers[w] == -1){
                                                        continue;
                                                    }
                                                    contentBlock archivosiNodo;
                                                    int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_3, SEEK_SET);
                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                            if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                //Es un archivo
                                                                if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                                                    //Cumple el regex
                                                                    for(int b = 0 ; b < tab ; b++ ){
                                                                        sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                                    }
                                                                    sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                }
                                                            }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                                //Es una carpeta
                                                                if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                                                    for(int b = 0 ; b < tab ; b++ ){
                                                                        sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                                    }
                                                                    sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                    tab++;
                                                                    char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, rename, tab,archivoDisco);
                                                                    char temp2[9999] = {0};
                                                                    strcpy(temp2,temp);
                                                                    sprintf( salidaRecorrido,"%s%s",salidaRecorrido, temp2 );
                                                                    sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                    tab--;
                                                                    free(temp);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }else{
                                            if(y == 14){
                                                //Triple indirecto
                                                pointerBlock pb3;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb3.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    pointerBlock pb3_2;
                                                    int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                    for (int w = 0 ; w < 16 ; w++ ){
                                                        if(pb3_2.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb3_2_1;
                                                        int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                        fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                        for (int v = 0 ; v < 16 ; v++ ){
                                                            if(pb3_2_1.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            contentBlock archivosiNodo;
                                                            int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_4, SEEK_SET);
                                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                                    if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                        //Es un archivo
                                                                        if(!regexec(&expresionFind,archivosiNodo.contenapunta[a].f_name,0,NULL,0)){
                                                                            //Cumple el regex
                                                                            for(int b = 0 ; b < tab ; b++ ){
                                                                                sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                                            }
                                                                            sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                        }
                                                                    }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                                        //Es una carpeta
                                                                        if(strcmp(archivosiNodo.contenapunta[a].f_name,"") != 0){
                                                                            for(int b = 0 ; b < tab ; b++ ){
                                                                                sprintf(salidaRecorrido,"%s     ",salidaRecorrido);
                                                                            }
                                                                            sprintf(salidaRecorrido,"%s|__%s",salidaRecorrido,archivosiNodo.contenapunta[a].f_name);
                                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                            tab++;
                                                                            char *temp = auxFindCarpeta(archivosiNodo.contenapunta[a].f_inode, rename, tab,archivoDisco);
                                                                            char temp2[9999] = {0};
                                                                            strcpy(temp2,temp);
                                                                            sprintf( salidaRecorrido,"%s%s",salidaRecorrido, temp2 );
                                                                            sprintf(salidaRecorrido,"%s\n",salidaRecorrido);
                                                                            tab--;
                                                                            free(temp);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Mapeo->");
                            printf("\n%s\n",salidaRecorrido);
                        }else if(operationType == 11){
                            //Cambiar dueño desde carpeta
                            int aux_carpeta_real = findArchivo(aux,inodoM,0,archivoDisco); //Inodo De la Carpeta Deseada
                            if(aux_carpeta_real == -1 && strcmp(pathVirtual,"/") != 0){
                                //Algo salio mal
                                exitoArchivo = false;
                                return -1;
                            }
                            //Avanzar a dicha carpeta
                            if(aux_carpeta_real > -1){
                                fseek(archivoDisco,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),SEEK_SET);
                                fread(&inodoM,sizeof (inode),1,archivoDisco);
                                if(inodoM.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                    inodoM.i_uid = sizeFile;
                                    fseek(archivoDisco,getiNodeStart_2(aux_carpeta_real,usuariologueadop.startiNodes),SEEK_SET);
                                    fwrite(&inodoM,sizeof (inodoM),1,archivoDisco);
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de carpeta '%s'\n",pathVirtual);
                                }
                            }
                            if(genPadres != 1){
                                //Aqui termino si no es recursivo
                                return 1;
                            }
                            //Revisar Todos los Archivos
                            for(int y = 0 ; y < 15 ; y++ ){
                                if(inodoM.i_block[y] == -1){
                                    continue;
                                }
                                if(y < 12){
                                    contentBlock archivosiNodo;
                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                    fseek(archivoDisco, offset, SEEK_SET);
                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                    for( int x = 0 ; x < 4 ; x++ ){
                                        if(archivosiNodo.contenapunta[x].f_inode != -1){
                                            if( verificarExtension2(archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                //Es un archivo
                                                //Cambiar dueño
                                                inode inodoA;
                                                fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[x].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                if(inodoA.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                                    fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[x].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                    inodoA.i_uid = sizeFile;
                                                    fwrite(&inodoA,sizeof (inodoA),1,archivoDisco);
                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de archivo '%s/%s'\n",pathVirtual,archivosiNodo.contenapunta[x].f_name);
                                                }
                                            }else if( strcmp(archivosiNodo.contenapunta[x].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[x].f_name,"..") != 0 ){
                                                //Es una carpeta
                                                //Cambiar dueño
                                                //Ejecutar para siguiente carpeta
                                                if(strcmp(archivosiNodo.contenapunta[x].f_name,"") !=0){
                                                    char tempCarpeta[999] = {0};
                                                    sprintf(tempCarpeta,"%s/%s",pathVirtual,archivosiNodo.contenapunta[x].f_name);
                                                    ejecutarARCHIVO(tempCarpeta,sizeFile,"",0,genPadres,11,"",archivoDisco);
                                                }
                                            }
                                        }
                                    }
                                }else{
                                    //Indirectos
                                    if(y == 12){
                                        //Indirecto
                                        pointerBlock pb1;
                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                                        for( int z = 0 ; z < 16 ; z++ ){
                                            if(pb1.pb_pointers[z] == -1){
                                                continue;
                                            }
                                            contentBlock archivosiNodo;
                                            int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                            for( int a = 0 ; a < 4 ; a++ ){
                                                if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                    if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                        //Es un archivo
                                                        //Cambiar dueño
                                                        inode inodoA;
                                                        fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                        fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                        if(inodoA.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                                            fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                            inodoA.i_uid = sizeFile;
                                                            fwrite(&inodoA,sizeof (inodoA),1,archivoDisco);
                                                            printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de archivo '%s/%s'\n",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                        }
                                                    }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                        //Es una carpeta
                                                        //Cambiar dueño
                                                        //Ejecutar para siguiente carpeta
                                                        if(strcmp(archivosiNodo.contenapunta[a].f_name,"") !=0){
                                                            char tempCarpeta[999] = {0};
                                                            sprintf(tempCarpeta,"%s/%s",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                            ejecutarARCHIVO(tempCarpeta,sizeFile,"",0,genPadres,11,"",archivoDisco);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }else{
                                        if(y == 13){
                                            //Doble indirecto
                                            pointerBlock pb2;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                                            for( int z = 0 ; z < 16 ; z++ ){
                                                if(pb2.pb_pointers[z] == -1){
                                                    continue;
                                                }
                                                pointerBlock pb2_2;
                                                int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                for (int w = 0 ; w < 16 ; w++ ){
                                                    if(pb2_2.pb_pointers[w] == -1){
                                                        continue;
                                                    }
                                                    contentBlock archivosiNodo;
                                                    int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_3, SEEK_SET);
                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                            if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                //Es un archivo
                                                                //Cambiar dueño
                                                                inode inodoA;
                                                                fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                                fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                                if(inodoA.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                                                    fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                                    inodoA.i_uid = sizeFile;
                                                                    fwrite(&inodoA,sizeof (inodoA),1,archivoDisco);
                                                                    printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de archivo '%s/%s'\n",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                                }
                                                            }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                                //Es una carpeta
                                                                //Cambiar dueño
                                                                //Ejecutar para siguiente carpeta
                                                                if(strcmp(archivosiNodo.contenapunta[a].f_name,"") !=0){
                                                                    char tempCarpeta[999] = {0};
                                                                    sprintf(tempCarpeta,"%s/%s",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                                    ejecutarARCHIVO(tempCarpeta,sizeFile,"",0,genPadres,11,"",archivoDisco);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }else{
                                            if(y == 14){
                                                //Triple indirecto
                                                pointerBlock pb3;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb3.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    pointerBlock pb3_2;
                                                    int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                    for (int w = 0 ; w < 16 ; w++ ){
                                                        if(pb3_2.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb3_2_1;
                                                        int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                        fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                        for (int v = 0 ; v < 16 ; v++ ){
                                                            if(pb3_2_1.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            contentBlock archivosiNodo;
                                                            int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_4, SEEK_SET);
                                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if(archivosiNodo.contenapunta[a].f_inode != -1){
                                                                    if( verificarExtension2(archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                        //Es un archivo
                                                                        //Cambiar dueño
                                                                        inode inodoA;
                                                                        fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                                        fread(&inodoA,sizeof(inodoA),1,archivoDisco);
                                                                        if(inodoA.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                                                            fseek(archivoDisco,getiNodeStart_2(archivosiNodo.contenapunta[a].f_inode,usuariologueadop.startiNodes),SEEK_SET);
                                                                            inodoA.i_uid = sizeFile;
                                                                            fwrite(&inodoA,sizeof (inodoA),1,archivoDisco);
                                                                            printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de archivo '%s/%s'\n",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                                        }
                                                                    }else if( strcmp(archivosiNodo.contenapunta[a].f_name,".") != 0 && strcmp(archivosiNodo.contenapunta[a].f_name,"..") != 0 ){
                                                                        //Es una carpeta
                                                                        //Cambiar dueño
                                                                        //Ejecutar para siguiente carpeta
                                                                        if(strcmp(archivosiNodo.contenapunta[a].f_name,"") !=0){
                                                                            char tempCarpeta[999] = {0};
                                                                            sprintf(tempCarpeta,"%s/%s",pathVirtual,archivosiNodo.contenapunta[a].f_name);
                                                                            ejecutarARCHIVO(tempCarpeta,sizeFile,"",0,genPadres,11,"",archivoDisco);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        //Trabajo de eliminacion o permisos de archivos o editar archivo o renombrar archivo
                        archivo finalFile_2 = getArchivo(aux,usuariologueadop.partUser,usuariologueadop.startiNodes ,getiNodeStart_2(posInodo, usuariologueadop.startiNodes),archivoDisco);
                        if(finalFile_2.estado == 1){
                            if(operationType == 3){
                                //Eliminar Archivo
                                if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'w') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se poseen permisos de escritura sobre este archivo\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                cleanArchivo(finalFile_2.position, usuariologueadop.bm_Block, sp_aux.sb_t_inode, sp_aux.sb_bm_Block,archivoDisco);
                                findArchivo(aux , inodoM, 1,archivoDisco);
                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino archivo '%s'\n",pathVirtual);
                                return 1;
                            }else if(operationType == 4){
                                //Permisos Archivo
                                if(usuariologueadop.id_user == finalFile_2.iduser || usuariologueadop.id_user == 1){
                                    fseek(archivoDisco , getiNodeStart_2(finalFile_2.position , usuariologueadop.startiNodes) , SEEK_SET);
                                    fread(&inodoM , sizeof(inode) , 1 , archivoDisco);
                                    inodoM.i_perm = sizeFile;
                                    time_t tiempoActual = time(NULL);
                                    struct tm *fecha = localtime(&tiempoActual);
                                    strftime(inodoM.i_mtime, sizeof(inodoM.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                                    fseek(archivoDisco , getiNodeStart_2(finalFile_2.position , usuariologueadop.startiNodes) , SEEK_SET);
                                    fwrite(&inodoM , sizeof(inode) , 1 , archivoDisco);
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambiaron los permisos de '%s'\n",pathVirtual);
                                    return 1;
                                }
                                exitoArchivo = false;
                                return -1;
                            }else if(operationType == 5){
                                //Editar Archivo
                                if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'r') == -1 || checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'w') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de lectura y escritura sobre este archivo\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                char inodoM_cFecha[16] = {0};

                                strcpy(inodoM_cFecha,inodoM.i_ctime);

                                modificarArchivo(finalFile_2.position, finalFile.contenido, "", archivoDisco);

                                //cleanArchivo(finalFile_2.position, usuariologueadop.bm_Block, sp_aux.sb_t_inode, sp_aux.sb_bm_Block,archivoDisco);
                                //findArchivo(aux , inodoM, 1,archivoDisco);
                                //ejecutarARCHIVO(pathVirtual,sizeFile,contentFile,contentFileExists,0,0,"",archivoDisco);

                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se actualizo archivo '%s'\n",pathVirtual);

                                archivo finalFile_3 = getArchivo(aux,usuariologueadop.partUser,usuariologueadop.startiNodes ,getiNodeStart_2(posInodo, usuariologueadop.startiNodes),archivoDisco);

                                fseek(archivoDisco , getiNodeStart_2(finalFile_3.position , usuariologueadop.startiNodes) , SEEK_SET);
                                fread(&inodoM , sizeof(inode) , 1 , archivoDisco);
                                strcpy(inodoM.i_ctime,inodoM_cFecha);
                                fseek(archivoDisco , getiNodeStart_2(finalFile_3.position , usuariologueadop.startiNodes) , SEEK_SET);
                                fwrite(&inodoM , sizeof(inode) , 1 , archivoDisco);
                                return 1;
                            }else if( operationType == 6 ){
                                //Seguro de existencia de archivo 'rename'
                                archivo tempRename = getArchivo(rename,usuariologueadop.partUser,usuariologueadop.startiNodes ,getiNodeStart_2(posInodo, usuariologueadop.startiNodes),archivoDisco);
                                if(tempRename.estado == 1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' ya existe en directorio\n",rename);
                                    exitoArchivo = false;
                                    return -1;
                                }
                                if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'w') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de escritura sobre el archivo\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                if(verificarExtension2(rename) == 0){
                                    int puede_cambiar_nombre = 1;
                                    for(int y = 0 ; y < 15 ; y++ ){
                                        if(inodoM.i_block[y] == -1){
                                            continue;
                                        }
                                        if(y < 12){
                                            contentBlock archivosiNodo;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                            for( int x = 0 ; x < 4 ; x++ ){
                                                if( strcmp(rename,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                    puede_cambiar_nombre = -1;
                                                }
                                            }
                                            if(puede_cambiar_nombre == 1){
                                                for( int x = 0 ; x < 4 ; x++ ){
                                                    if( strcmp(aux,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                        memset(&archivosiNodo.contenapunta[x].f_name,0,12);
                                                        strcpy(archivosiNodo.contenapunta[x].f_name,rename);

                                                        fseek(archivoDisco, offset, SEEK_SET);
                                                        fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                        return 1;
                                                    }
                                                }
                                            }
                                        }else{
                                            //Indirectos
                                            if(y == 12){
                                                //Indirecto
                                                pointerBlock pb1;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb1,sizeof(pb1),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb1.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    contentBlock archivosiNodo;
                                                    int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                            puede_cambiar_nombre = -1;
                                                        }
                                                    }
                                                    if(puede_cambiar_nombre == 1){
                                                        for( int a = 0 ; a < 4 ; a++ ){
                                                            if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                fseek(archivoDisco, offset_2, SEEK_SET);
                                                                fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                                return 1;
                                                            }
                                                        }
                                                    }
                                                }
                                            }else{
                                                if(y == 13){
                                                    //Doble indirecto
                                                    pointerBlock pb2;
                                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fread(&pb2,sizeof(pb2),1,archivoDisco);
                                                    for( int z = 0 ; z < 16 ; z++ ){
                                                        if(pb2.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb2_2;
                                                        int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_2, SEEK_SET);
                                                        fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                        for (int w = 0 ; w < 16 ; w++ ){
                                                            if(pb2_2.pb_pointers[w] == -1){
                                                                continue;
                                                            }
                                                            contentBlock archivosiNodo;
                                                            int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_3, SEEK_SET);
                                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                    puede_cambiar_nombre = -1;
                                                                }
                                                            }
                                                            if(puede_cambiar_nombre == 1){
                                                                for( int a = 0 ; a < 4 ; a++ ){
                                                                    if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                        memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                        strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                        fseek(archivoDisco, offset_3, SEEK_SET);
                                                                        fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                                        return 1;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }else{
                                                    if(y == 14){
                                                        //Triple indirecto
                                                        pointerBlock pb3;
                                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset, SEEK_SET);
                                                        fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                        for( int z = 0 ; z < 16 ; z++ ){
                                                            if(pb3.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            pointerBlock pb3_2;
                                                            int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                                            fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                            for (int w = 0 ; w < 16 ; w++ ){
                                                                if(pb3_2.pb_pointers[z] == -1){
                                                                    continue;
                                                                }
                                                                pointerBlock pb3_2_1;
                                                                int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                                fseek(archivoDisco, offset_3, SEEK_SET);
                                                                fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                                for (int v = 0 ; v < 16 ; v++ ){
                                                                    if(pb3_2_1.pb_pointers[z] == -1){
                                                                        continue;
                                                                    }
                                                                    contentBlock archivosiNodo;
                                                                    int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                                    fseek(archivoDisco, offset_4, SEEK_SET);
                                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                                        if( strcmp(rename,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                            puede_cambiar_nombre = -1;
                                                                        }
                                                                    }
                                                                    if(puede_cambiar_nombre == 1){
                                                                        for( int a = 0 ; a < 4 ; a++ ){
                                                                            if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                                memset(&archivosiNodo.contenapunta[a].f_name,0,12);
                                                                                strcpy(archivosiNodo.contenapunta[a].f_name,rename);

                                                                                fseek(archivoDisco, offset_4, SEEK_SET);
                                                                                fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio nombre de '%s' a '%s'\n",pathVirtual,rename);
                                                                                return 1;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' no existe \n",pathVirtual);
                                                    exitoArchivo = false;
                                                    return -1;
                                                }
                                            }
                                        }
                                    }
                                    exitoArchivo = false;
                                    return -1;
                                }else{
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo no puede ser de otro tipo de extension (.*)\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                            }else if(operationType == 7){
                                //Copiar archivo
                                if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'r') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de lectura sobre el archivo\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                char pathAuxFile2[] = "/home/edi/auxCopy.txt";
                                FILE *secundario;
                                secundario = fopen(pathAuxFile2,"w");
                                if(secundario != NULL){
                                    fprintf(secundario,"%s",finalFile_2.contenido);
                                    ejecutarARCHIVO(rename,0,pathAuxFile2,1,0,0,"",archivoDisco);
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se copio archivo '%s' en '%s'\n",pathVirtual,rename);
                                }else{
                                    //Algo salio mal
                                    exitoArchivo = false;
                                    return -1;
                                }
                            }else if(operationType == 8){
                                //Mover Archivo de aqui
                                if(checkPermisos(finalFile_2.iduser,finalFile_2.idGrupo,finalFile_2.permArch,'w') == -1){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No posee permisos de escritura sobre el archivo\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                                if(verificarExtension2(rename) != 0){
                                    for(int y = 0 ; y < 15 ; y++ ){
                                        if(inodoM.i_block[y] == -1){
                                            continue;
                                        }
                                        if(y < 12){
                                            contentBlock archivosiNodo;
                                            int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                            fseek(archivoDisco, offset, SEEK_SET);
                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                            for( int x = 0 ; x < 4 ; x++ ){
                                                if( strcmp(aux,archivosiNodo.contenapunta[x].f_name) == 0 ){
                                                    char rename2[999] = {0};
                                                    sprintf(rename2,"%s",archivosiNodo.contenapunta[x].f_name);
                                                    sizeFile = archivosiNodo.contenapunta[x].f_inode;

                                                    archivosiNodo.contenapunta[x].f_inode = -1;
                                                    memset(&archivosiNodo.contenapunta[x].f_name,0,12);

                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                    return 1;
                                                }
                                            }
                                        }else{
                                            //Indirectos
                                            if(y == 12){
                                                //Indirecto
                                                pointerBlock pb1;
                                                int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                fseek(archivoDisco, offset, SEEK_SET);
                                                fread(&pb1,sizeof(pb1),1,archivoDisco);
                                                for( int z = 0 ; z < 16 ; z++ ){
                                                    if(pb1.pb_pointers[z] == -1){
                                                        continue;
                                                    }
                                                    contentBlock archivosiNodo;
                                                    int offset_2 = getBlockStart_2(pb1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset_2, SEEK_SET);
                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                        if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                            char rename2[999] = {0};
                                                            sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                            sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                            archivosiNodo.contenapunta[a].f_inode = -1;
                                                            memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                            fseek(archivoDisco, offset, SEEK_SET);
                                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                            return 1;
                                                        }
                                                    }
                                                }
                                            }else{
                                                if(y == 13){
                                                    //Doble indirecto
                                                    pointerBlock pb2;
                                                    int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                    fread(&pb2,sizeof(pb2),1,archivoDisco);
                                                    for( int z = 0 ; z < 16 ; z++ ){
                                                        if(pb2.pb_pointers[z] == -1){
                                                            continue;
                                                        }
                                                        pointerBlock pb2_2;
                                                        int offset_2 = getBlockStart_2(pb2.pb_pointers[z], usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset_2, SEEK_SET);
                                                        fread(&pb2_2, sizeof (pb2_2),1,archivoDisco);
                                                        for (int w = 0 ; w < 16 ; w++ ){
                                                            if(pb2_2.pb_pointers[w] == -1){
                                                                continue;
                                                            }
                                                            contentBlock archivosiNodo;
                                                            int offset_3 = getBlockStart_2(pb2_2.pb_pointers[z] , usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_3, SEEK_SET);
                                                            fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                            for( int a = 0 ; a < 4 ; a++ ){
                                                                if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                    char rename2[999] = {0};
                                                                    sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                                    sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                                    archivosiNodo.contenapunta[a].f_inode = -1;
                                                                    memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                                    fseek(archivoDisco, offset, SEEK_SET);
                                                                    fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                    ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                                    return 1;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }else{
                                                    if(y == 14){
                                                        //Triple indirecto
                                                        pointerBlock pb3;
                                                        int offset = getBlockStart_2(inodoM.i_block[y] , usuariologueadop.startBlocks);
                                                        fseek(archivoDisco, offset, SEEK_SET);
                                                        fread(&pb3,sizeof(pb3),1,archivoDisco);
                                                        for( int z = 0 ; z < 16 ; z++ ){
                                                            if(pb3.pb_pointers[z] == -1){
                                                                continue;
                                                            }
                                                            pointerBlock pb3_2;
                                                            int offset_2 = getBlockStart_2(pb3.pb_pointers[z], usuariologueadop.startBlocks);
                                                            fseek(archivoDisco, offset_2, SEEK_SET);
                                                            fread(&pb3_2, sizeof (pb3_2),1,archivoDisco);
                                                            for (int w = 0 ; w < 16 ; w++ ){
                                                                if(pb3_2.pb_pointers[z] == -1){
                                                                    continue;
                                                                }
                                                                pointerBlock pb3_2_1;
                                                                int offset_3 = getBlockStart_2(pb3_2.pb_pointers[z], usuariologueadop.startBlocks);
                                                                fseek(archivoDisco, offset_3, SEEK_SET);
                                                                fread(&pb3_2_1, sizeof (pb3_2_1),1,archivoDisco);
                                                                for (int v = 0 ; v < 16 ; v++ ){
                                                                    if(pb3_2_1.pb_pointers[z] == -1){
                                                                        continue;
                                                                    }
                                                                    contentBlock archivosiNodo;
                                                                    int offset_4 = getBlockStart_2(pb3_2_1.pb_pointers[z] , usuariologueadop.startBlocks);
                                                                    fseek(archivoDisco, offset_4, SEEK_SET);
                                                                    fread(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                    for( int a = 0 ; a < 4 ; a++ ){
                                                                        if( strcmp(aux,archivosiNodo.contenapunta[a].f_name) == 0 ){
                                                                            char rename2[999] = {0};
                                                                            sprintf(rename2,"%s",archivosiNodo.contenapunta[a].f_name);
                                                                            sizeFile = archivosiNodo.contenapunta[a].f_inode;

                                                                            archivosiNodo.contenapunta[a].f_inode = -1;
                                                                            memset(&archivosiNodo.contenapunta[a].f_name,0,12);

                                                                            fseek(archivoDisco, offset, SEEK_SET);
                                                                            fwrite(&archivosiNodo,sizeof(archivosiNodo),1,archivoDisco);
                                                                            ejecutarARCHIVO(rename,sizeFile,"",0,0,9,rename2,archivoDisco);
                                                                            return 1;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo con nombre '%s' no existe \n",pathVirtual);
                                                    exitoArchivo = false;
                                                    return -1;
                                                }
                                            }
                                        }
                                    }
                                    exitoArchivo = false;
                                    return -1;
                                }else{
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ubicacion final  no puede ser de tipo de extension (.*)\n");
                                    exitoArchivo = false;
                                    return -1;
                                }
                            }else if(operationType == 11){
                                //Cambiar dueño de archivo
                                inode inodoA;
                                if(archivoDisco != NULL){
                                    fseek(archivoDisco,getiNodeStart_2(finalFile_2.position,usuariologueadop.startiNodes),SEEK_SET);
                                    fread(&inodoA,sizeof (inode),1,archivoDisco);
                                    if(inodoA.i_uid == usuariologueadop.id_user || usuariologueadop.id_user == 1){
                                        fseek(archivoDisco,getiNodeStart_2(finalFile_2.position,usuariologueadop.startiNodes),SEEK_SET);
                                        inodoA.i_uid = sizeFile;
                                        fwrite(&inodoA,sizeof (inodoA),1,archivoDisco);
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m se cambio dueño de archivo '%s'\n",pathVirtual);
                                        return 1;
                                    }
                                    exitoArchivo = false;
                                    return 0;
                                }
                                exitoArchivo = false;
                                return -1;
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo no existe\n");
                            exitoArchivo = false;
                            return -1;
                        }
                    }
                }
            }else{

            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
        }
    }
    return 0;
}
int ejecutarMKFS(char pathDisco[], PMOUNT particion, char type, int fs){
    if(verificarExtension(pathDisco,".disk") == 0){
        if(existsFile(pathDisco) == 1){
            //Existe disco y es posible trabajar
            MBR aux_mbr;
            EBR aux_ebr;
            FILE *archivoDisco;
            int inicio_trabajo = -1;
            int size_particion = -1;
            archivoDisco = fopen(pathDisco,"rb+");
            if(archivoDisco != NULL){
                //Existe archivo de disco
                if(particion.mount_type == 'p' || particion.mount_type == 'P' || particion.mount_type == 'l' || particion.mount_type == 'L'){
                    //Lectura de particion
                    fseek(archivoDisco,particion.mount_start,SEEK_SET); //posicionar puntero de lectura a inicio de particion
                    inicio_trabajo = particion.mount_start; //inicio de la particion
                    size_particion = particion.mount_size; //size de la particion
                    if(type == 'c'){
                        int sizeReal = particion.mount_size;
                        int sizeMaximo = 1024*1024;
                        if(sizeReal > sizeMaximo){
                            char insert[1024*1024] = {0};
                            while (sizeReal > sizeMaximo) {
                                sizeReal = sizeReal - sizeMaximo;
                                fwrite(insert,sizeof(char),sizeMaximo,archivoDisco);
                            }
                        }
                        char ultimoInsert[sizeReal];
                        for (int i = 0; i < sizeReal; i++){
                            ultimoInsert[i] = '\0';
                        }
                        fwrite(ultimoInsert,sizeof(char),sizeReal,archivoDisco);
                        fseek(archivoDisco,particion.mount_start,SEEK_SET);
                        int n = -1;
                        if(fs == 3){
                            n = 1 + ((size_particion - sizeof(SuperBloque) - 1)/(sizeof(journal) + sizeof(inode) + 3 * sizeof (contentBlock) + 4));
                        }else if(fs == 2){
                            n = 1 + ((size_particion - sizeof(SuperBloque) - 1)/(sizeof(inode) + 3 * sizeof (contentBlock) + 4));
                        }
                        SuperBloque sp_nuevo;
                        sp_nuevo = initSP(sp_nuevo);

                        sp_nuevo.sb_filesystem_type = fs ;
                        sp_nuevo.sb_inodes_count = n;
                        sp_nuevo.sb_blocks_count = 3 * n;
                        sp_nuevo.sb_free_inodes_count = n;
                        sp_nuevo.sb_free_blocks_count = 3 * n;
                        time_t tiempoActual = time(NULL);
                        struct tm *fecha = localtime(&tiempoActual);
                        strftime(sp_nuevo.sb_mtime, sizeof(sp_nuevo.sb_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        memset(&sp_nuevo.sb_umtime,0,16);
                        sp_nuevo.mount_count = 1;
                        sp_nuevo.sb_magic = 61267;
                        sp_nuevo.sb_inode_size = sizeof(inode);
                        sp_nuevo.sb_block_size = sizeof(contentBlock);
                        if(fs == 3){
                            //EXT3 - tomar en cuenta el journaling
                            inicio_trabajo = inicio_trabajo + sizeof (SuperBloque) + n * sizeof(journal);
                        }else if(fs == 2){
                            //EXT2 - no existe journaling
                            inicio_trabajo = inicio_trabajo + sizeof (SuperBloque);
                        }else{
                            //Algo salio mal
                        }
                        //inicio_trabajo -> justo despues de SuperBloque +
                        //Bitmap de inodos y bloques
                        sp_nuevo.sb_bm_inode = inicio_trabajo;
                        sp_nuevo.sb_bm_Block = inicio_trabajo + n;
                        //Inicio de inodos y bloques
                        sp_nuevo.sb_t_inode = inicio_trabajo + n + 3*n;
                        sp_nuevo.sb_t_block = inicio_trabajo + n + 3*n + n * sizeof(inode);

                        sp_nuevo.sb_first_inode = 2;
                        sp_nuevo.sb_first_block = 2;
                        //Inicio de la particion
                        fwrite(&sp_nuevo,sizeof(sp_nuevo),1,archivoDisco);
                        //Despues del Superbloque
                        if(fs == 3){
                            //EXT 3 - generar journals
                            journal nueva_entrada;
                            nueva_entrada = initJournal(nueva_entrada);
                            for(int i = 0; i < n; i++){
                                fwrite(&nueva_entrada,sizeof(journal),1,archivoDisco);
                            }
                        }
                        //Despues de los journal si existen o despues del superbloque
                        char vacio = '0';
                        char lleno = '1';
                        for(int i = 0; i < 4* n; i++){
                            //llenado de bitmaps
                            //0 - vacio
                            //1 - inodo
                            //2 - bloque de informacion
                            if(i == n + 1){
                                lleno = '2';
                                fwrite(&lleno,1,1,archivoDisco);
                            }else if(i == 0 || i == 1 || i == n){
                                fwrite(&lleno,1,1,archivoDisco);
                            }else{
                                fwrite(&vacio,1,1,archivoDisco);
                            }
                        }
                        //Depsues de mapas de bit de iNodos y Bloques
                        //inodo raiz
                        inode nuevo_inodo;
                        inode nuevo_inodo_2;
                        nuevo_inodo = initiNode(nuevo_inodo);
                        nuevo_inodo.i_uid = 1;
                        nuevo_inodo.i_gid = 1;
                        nuevo_inodo.i_size = 0;
                        strftime(nuevo_inodo.i_ctime, sizeof(nuevo_inodo.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo.i_atime, sizeof(nuevo_inodo.i_atime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo.i_mtime, sizeof(nuevo_inodo.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        nuevo_inodo.i_block[0] = 0;
                        for (int i = 1; i < 15; i++) {
                            nuevo_inodo.i_block[i] = -1;
                        }
                        nuevo_inodo.i_type ='0';
                        nuevo_inodo.i_perm = 664;
                        fseek(archivoDisco,getiNodeStart_2(0,sp_nuevo.sb_t_inode),SEEK_SET);
                        fwrite(&nuevo_inodo,sizeof (nuevo_inodo),1,archivoDisco);
                        //Creacion de contenido
                        contentBlock nuevo_bloque;
                        fileBlock nuevo_archivo;
                        nuevo_bloque = initContentBlock(nuevo_bloque);
                        nuevo_bloque.contenapunta[0].f_inode = 0;
                        strcpy(nuevo_bloque.contenapunta[0].f_name,".");
                        nuevo_bloque.contenapunta[1].f_inode = 0;
                        strcpy(nuevo_bloque.contenapunta[1].f_name,"..");
                        nuevo_bloque.contenapunta[2].f_inode = 1;
                        strcpy(nuevo_bloque.contenapunta[2].f_name,"users.txt");
                        nuevo_bloque.contenapunta[3].f_inode = -1;
                        fseek(archivoDisco,getBlockStart_2(0,sp_nuevo.sb_t_block),SEEK_SET);
                        fwrite(&nuevo_bloque,sizeof (nuevo_bloque),1,archivoDisco);
                        //Creacion de users.txt
                        fseek(archivoDisco,getiNodeStart_2(1,sp_nuevo.sb_t_inode), SEEK_SET);
                        char primer_users[] = "1,G,root\n1,U,root,root,123\n";
                        nuevo_inodo_2.i_uid = 1;
                        nuevo_inodo_2.i_gid = 1;
                        nuevo_inodo_2.i_size = strlen(primer_users);
                        strftime(nuevo_inodo_2.i_ctime, sizeof(nuevo_inodo_2.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo_2.i_atime, sizeof(nuevo_inodo_2.i_atime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo_2.i_mtime, sizeof(nuevo_inodo_2.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        nuevo_inodo_2.i_block[0] = 1;
                        for(int i = 1; i < 15 ; i++){
                            nuevo_inodo_2.i_block[i] = -1;
                        }
                        nuevo_inodo_2.i_type = '1';
                        nuevo_inodo_2.i_perm = 664;
                        fwrite(&nuevo_inodo_2,sizeof (inode),1,archivoDisco);
                        fseek(archivoDisco, getBlockStart_2(1,sp_nuevo.sb_t_block),SEEK_SET);
                        nuevo_archivo = initFileBlock(nuevo_archivo);
                        strcpy(nuevo_archivo.fcontenapunta,primer_users);
                        fwrite(&nuevo_archivo,sizeof (fileBlock),1,archivoDisco);
                        fclose(archivoDisco);
                        if(fs == 3){
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se formateo particion: (n '%i'): inodos '%i' , bloques '%i', journal '%i' \n", n, n, 3*n, n);

                            archivoDisco = fopen(pathDisco,"rb+");
                            journal entrada;
                            entrada.j_uid = 1;
                            strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);
                            strcpy(entrada.j_name,"/users.txt");
                            entrada.j_perm = 664;
                            entrada.j_type = '1';

                           // char pathAuxFile2[] = "0@/home/edi/firstUsers.pdf@0";
                            char pathAuxFile2[] = "0@/home/edi/firstUsers.pdf@0";


                            strcpy(entrada.j_content,pathAuxFile2);
                            entrada.j_op_type = 'a';

                            int start = particion.mount_start + sizeof(SuperBloque);
                            fseek(archivoDisco, start, SEEK_SET);
                            fwrite(&entrada, sizeof(journal) , 1 , archivoDisco);
                            fclose(archivoDisco);

                            return 0;
                        }else if(fs == 2){
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se formateo particion: (n '%i'): inodos '%i' , bloques '%i' \n", n, n, 3*n);
                            return 0;
                        }
                    }else{
                        //Formateo rapido
                        fseek(archivoDisco,particion.mount_start,SEEK_SET);
                        int n = -1;
                        if(fs == 3){
                            n = 1 + ((size_particion - sizeof(SuperBloque) - 1)/(sizeof(journal) + sizeof(inode) + 3 * sizeof (contentBlock) + 4));
                        }else if(fs == 2){
                            n = 1 + ((size_particion - sizeof(SuperBloque) - 1)/(sizeof(inode) + 3 * sizeof (contentBlock) + 4));
                        }
                        SuperBloque sp_nuevo;
                        sp_nuevo = initSP(sp_nuevo);

                        sp_nuevo.sb_filesystem_type = fs ;
                        sp_nuevo.sb_inodes_count = n;
                        sp_nuevo.sb_blocks_count = 3 * n;
                        sp_nuevo.sb_free_inodes_count = n;
                        sp_nuevo.sb_free_blocks_count = 3 * n;
                        time_t tiempoActual = time(NULL);
                        struct tm *fecha = localtime(&tiempoActual);
                        strftime(sp_nuevo.sb_mtime, sizeof(sp_nuevo.sb_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        memset(&sp_nuevo.sb_umtime,0,16);
                        sp_nuevo.mount_count = 1;
                        sp_nuevo.sb_magic = 61267;
                        sp_nuevo.sb_inode_size = sizeof(inode);
                        sp_nuevo.sb_block_size = sizeof(contentBlock);
                        if(fs == 3){
                            inicio_trabajo = inicio_trabajo + sizeof (SuperBloque) + n * sizeof(journal);
                        }else if(fs == 2){
                            inicio_trabajo = inicio_trabajo + sizeof (SuperBloque);
                        }else{
                            //Algo salio mal
                        }
                        //inicio_trabajo -> justo despues de SuperBloque + Journal
                        //Bitmap de inodos y bloques
                        sp_nuevo.sb_bm_inode = inicio_trabajo;
                        sp_nuevo.sb_bm_Block = inicio_trabajo + n;
                        //Inicio de inodos y bloques
                        sp_nuevo.sb_t_inode = inicio_trabajo + n + 3*n;
                        sp_nuevo.sb_t_block = inicio_trabajo + n + 3*n + n * sizeof(inode);

                        sp_nuevo.sb_first_inode = 2;
                        sp_nuevo.sb_first_block = 2;
                        //Inicio de la particion
                        fwrite(&sp_nuevo,sizeof(sp_nuevo),1,archivoDisco);
                        //Despues del Superbloque
                        if(fs == 3){
                            //EXT 3 - generar journals
                            journal nueva_entrada;
                            nueva_entrada = initJournal(nueva_entrada);
                            for(int i = 0; i < n; i++){
                                fwrite(&nueva_entrada,sizeof(journal),1,archivoDisco);
                            }
                        }
                        //Despues de los journal si existen o despues del superbloque
                        char vacio = '0';
                        char lleno = '1';
                        for(int i = 0; i < 4* n; i++){
                            //llenado de bitmaps
                            //0 - vacio
                            //1 - lleno
                            //2 - bloques
                            if(i == n + 1){
                                lleno = '2';
                                fwrite(&lleno,1,1,archivoDisco);
                            }else if(i == 0 || i == 1 || i == n){
                                fwrite(&lleno,1,1,archivoDisco);
                            }else{
                                fwrite(&vacio,1,1,archivoDisco);
                            }
                        }
                        //Depsues de mapas de bit de iNodos y Bloques
                        //inodo raiz
                        //inodo raiz
                        inode nuevo_inodo;
                        inode nuevo_inodo_2;
                        nuevo_inodo = initiNode(nuevo_inodo);
                        nuevo_inodo.i_uid = 1;
                        nuevo_inodo.i_gid = 1;
                        nuevo_inodo.i_size = 0;
                        strftime(nuevo_inodo.i_ctime, sizeof(nuevo_inodo.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo.i_atime, sizeof(nuevo_inodo.i_atime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo.i_mtime, sizeof(nuevo_inodo.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        nuevo_inodo.i_block[0] = 0;
                        for (int i = 1; i < 15; i++) {
                            nuevo_inodo.i_block[i] = -1;
                        }
                        nuevo_inodo.i_type ='0';
                        nuevo_inodo.i_perm = 664;
                        fseek(archivoDisco,getiNodeStart_2(0,sp_nuevo.sb_t_inode),SEEK_SET);
                        fwrite(&nuevo_inodo,sizeof (nuevo_inodo),1,archivoDisco);
                        //Creacion de contenido
                        contentBlock nuevo_bloque;
                        fileBlock nuevo_archivo;
                        nuevo_bloque = initContentBlock(nuevo_bloque);
                        nuevo_bloque.contenapunta[0].f_inode = 0;
                        strcpy(nuevo_bloque.contenapunta[0].f_name,".");
                        nuevo_bloque.contenapunta[1].f_inode = 0;
                        strcpy(nuevo_bloque.contenapunta[1].f_name,"..");
                        nuevo_bloque.contenapunta[2].f_inode = 1;
                        strcpy(nuevo_bloque.contenapunta[2].f_name,"users.txt");
                        nuevo_bloque.contenapunta[3].f_inode = -1;
                        fseek(archivoDisco,getBlockStart_2(0,sp_nuevo.sb_t_block),SEEK_SET);
                        fwrite(&nuevo_bloque,sizeof (nuevo_bloque),1,archivoDisco);
                        //Creacion de users.txt
                        fseek(archivoDisco,getiNodeStart_2(1,sp_nuevo.sb_t_inode), SEEK_SET);
                        char primer_users[] = "1,G,root\n1,U,root,root,123\n";
                        nuevo_inodo_2.i_uid = 1;
                        nuevo_inodo_2.i_gid = 1;
                        nuevo_inodo_2.i_size = strlen(primer_users);
                        strftime(nuevo_inodo_2.i_ctime, sizeof(nuevo_inodo_2.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo_2.i_atime, sizeof(nuevo_inodo_2.i_atime)-1, "%d/%m/%y %H:%M", fecha);
                        strftime(nuevo_inodo_2.i_mtime, sizeof(nuevo_inodo_2.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                        nuevo_inodo_2.i_block[0] = 1;
                        for(int i = 1; i < 15 ; i++){
                            nuevo_inodo_2.i_block[i] = -1;
                        }
                        nuevo_inodo_2.i_type = '1';
                        nuevo_inodo_2.i_perm = 664;
                        fwrite(&nuevo_inodo_2,sizeof (inode),1,archivoDisco);
                        fseek(archivoDisco, getBlockStart_2(1,sp_nuevo.sb_t_block),SEEK_SET);
                        nuevo_archivo = initFileBlock(nuevo_archivo);
                        strcpy(nuevo_archivo.fcontenapunta,primer_users);
                        fwrite(&nuevo_archivo,sizeof (fileBlock),1,archivoDisco);
                        fclose(archivoDisco);
                        if(fs == 3){
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se formateo particion: (n '%i'): inodos '%i' , bloques '%i', journal '%i' \n", n, n, 3*n, n);

                            archivoDisco = fopen(pathDisco,"rb+");
                            journal entrada;
                            entrada.j_uid = 1;
                            strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);
                            strcpy(entrada.j_name,"/users.txt");
                            entrada.j_perm = 664;
                            entrada.j_type = '1';

                            char pathAuxFile2[] = "0@/home/edi/firstUsers.pdf@0";
                           // char pathAuxFile2[] = "/home/edi/firstUsers.txt";

                            strcpy(entrada.j_content,pathAuxFile2);
                            entrada.j_op_type = 'a';

                            int start = particion.mount_start + sizeof(SuperBloque);
                            fseek(archivoDisco, start, SEEK_SET);
                            fwrite(&entrada, sizeof(journal) , 1 , archivoDisco);
                            fclose(archivoDisco);

                            return 0;
                        }else if(fs == 2){
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se formateo particion: (n '%i'): inodos '%i' , bloques '%i' \n", n, n, 3*n);
                            return 0;
                        }
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Tipo de particion no puede contener sistema de archivos\n");
                }
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: verificar valores de Mkfs \n");
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: verificar valores de Mkfs \n");
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: verificar valores de Mkfs \n");
    }
    return 0;
}
int ejecutarCONVERT(PMOUNT particionTrabajo, char pathDisco[]){
    if(existsFile(pathDisco) == 1 && verificarExtension(pathDisco,".disk") == 0 ){
        FILE *archivoDiscoTemporal;
        archivoDiscoTemporal = fopen(pathDisco,"rb+");
        SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDiscoTemporal);
        if(sp_aux.sb_magic == 61267){
            if(sp_aux.sb_filesystem_type == 3){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion ya posee sistema de archivos EXT3\n");
                return -1;
            }
            archivo tempusers = getArchivo("users.txt", particionTrabajo, sp_aux.sb_t_inode, sp_aux.sb_t_inode,archivoDiscoTemporal);
            fclose(archivoDiscoTemporal);

            int sizeUsado = 0;
            sizeUsado = sizeof (SuperBloque);
            sizeUsado += sp_aux.sb_blocks_count * sizeof (fileBlock);
            sizeUsado += sp_aux.sb_inodes_count * sizeof (inode);
            sizeUsado += sp_aux.sb_blocks_count + sp_aux.sb_inodes_count;

            int lastInodo = 0;
            int lastBlock = 0;

            for (int i = 0 ; i < strlen(usuariologueadop.bm_Block) ; i++ ) {
                if(usuariologueadop.bm_Block[i] == '1' || usuariologueadop.bm_Block[i] == '2' || usuariologueadop.bm_Block[i] == '3'){
                    lastBlock = i;
                }
            }
            for (int i = 0 ; i < strlen(usuariologueadop.bm_iNode) ; i++ ) {
                if(usuariologueadop.bm_iNode[i] == '1'){
                    lastInodo = i;
                }
            }
            lastBlock++;
            lastInodo++;

            int n;
            int numInodo = 0;
            int numBloque = 0;
            sp_aux.sb_filesystem_type = 3;

            n = 1 + ((sizeUsado - sizeof(SuperBloque) - 1)/(sizeof(journal) + sizeof(inode) + 3 * sizeof (contentBlock) + 4));
            numInodo = n;
            numBloque = 3 * n;
            if(lastInodo > numInodo){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: La cantidad de inodos utilizados no caben en nuevo formato\n");
                return -1;
            }
            if(lastBlock > numBloque){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: La cantidad de bloques utilizados no caben en nuevo formato\n");
                return -1;
            }

            sp_aux.sb_inodes_count = numInodo;
            sp_aux.sb_blocks_count = numBloque;

            sp_aux.sb_free_inodes_count = numInodo-lastInodo;
            sp_aux.sb_free_blocks_count = numBloque-lastBlock;

            time_t tiempoActual = time(NULL);
            struct tm *fecha = localtime(&tiempoActual);
            strftime(sp_aux.sb_mtime, sizeof(sp_aux.sb_mtime)-1, "%d/%m/%y %H:%M", fecha);

            int startTrabajo = particionTrabajo.mount_start + sizeof (SuperBloque) + n * sizeof (journal);
            if(particionTrabajo.mount_type == 'e' || particionTrabajo.mount_type == 'E'){
                startTrabajo += sizeof (EBR);
            }
            sp_aux.sb_bm_inode = startTrabajo;
            sp_aux.sb_bm_Block = startTrabajo + n;

            sp_aux.sb_t_inode = startTrabajo + n + 3 * n;
            sp_aux.sb_t_block = startTrabajo + n + 3 * n + n * sizeof (inode);

            sp_aux.sb_first_inode = lastInodo;
            sp_aux.sb_first_block = lastBlock;

            char auxImp[9999] = {0};
            CrearPath("/home/edis/Escritorio/temporalpapu/tempConvert.disk");
            sprintf(auxImp,"cp %s /home/edis/Escritorio/temporalpapu/tempConvert.disk", pathDisco);

            FILE *archivoDisco;
            FILE *archivoTemp;

            archivoTemp = fopen("/home/edis/Escritorio/temporalpapu/tempConvert.disk","wb");
            fclose(archivoTemp);
            system(auxImp);
            archivoDisco = fopen(pathDisco,"rb+");
            archivoTemp = fopen("/home/edis/Escritorio/temporalpapu/tempConvert.disk","rb+");

            fseek(archivoDisco, particionTrabajo.mount_start,SEEK_SET);
            fwrite(&sp_aux,sizeof(SuperBloque),1,archivoDisco);
            journal entrada;
            entrada = initJournal(entrada);

            for( int i = 0 ; i < n ; i++ ){
                fwrite(&entrada,sizeof (journal),1,archivoDisco);
            }

            char auxImpresion;
            for(int i = 0 ; i < numInodo ; i ++){
                auxImpresion = usuariologueadop.bm_iNode[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }
            for(int i = 0 ; i < numBloque ; i ++){
                auxImpresion = usuariologueadop.bm_Block[i];
                fwrite(&auxImpresion,1,1,archivoDisco);
            }

            usuariologueadop.bm_iNode[numInodo] = '0';
            usuariologueadop.bm_Block[numBloque] = '0';

            inode iNodoM;
            fileBlock bloqueArchivo;
            contentBlock bloqueContenido;
            pointerBlock bloqueApuntadores;

            startTrabajo = sp_aux.sb_t_block;
            for(int i = 0; i < numBloque ; i++ ){
                if(usuariologueadop.bm_Block[i] == '0'){
                    continue;
                }
                fseek(archivoDisco,getBlockStart_2(i, startTrabajo), SEEK_SET);
                fseek(archivoTemp, getBlockStart_2(i, usuariologueadop.startBlocks), SEEK_SET);
                if(usuariologueadop.bm_Block[i] == '1'){
                    fread(&bloqueContenido, sizeof (bloqueContenido),1,archivoTemp);
                    fwrite(&bloqueContenido,sizeof (bloqueContenido),1,archivoDisco);
                }else if(usuariologueadop.bm_Block[i] == '2'){
                    fread(&bloqueArchivo, sizeof(bloqueArchivo),1,archivoTemp);
                    fwrite(&bloqueArchivo,sizeof (bloqueArchivo),1,archivoDisco);
                }else if(usuariologueadop.bm_Block[i] == '3'){
                    fread(&bloqueApuntadores, sizeof(bloqueApuntadores),1,archivoTemp);
                    fwrite(&bloqueApuntadores,sizeof (bloqueApuntadores),1,archivoDisco);
                }
            }

            tiempoActual = time(NULL);
            fecha = localtime(&tiempoActual);
            strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

            entrada.j_uid = 1;
            strcpy(entrada.j_name,"/users.txt");
            entrada.j_type = '1';
            entrada.j_perm = 664;
            strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

            char pathAuxFile2[] = "0@/home/edi/firstUsers.pdf@0";

            strcpy(entrada.j_content,pathAuxFile2);
            entrada.j_op_type = 'a';

            writeJournal(entrada,archivoDisco,0);
            startTrabajo = sp_aux.sb_t_inode;
            for( int i = 0; i < n ; i++){
                if(usuariologueadop.bm_iNode[i] == '0'){
                    continue;
                }
                fseek(archivoDisco, getiNodeStart_2(i,startTrabajo),SEEK_SET);
                fseek(archivoTemp, getiNodeStart_2(i, usuariologueadop.startiNodes), SEEK_SET);

                fread(&iNodoM,sizeof (inode),1,archivoTemp);
                fwrite(&iNodoM,sizeof (inode),1,archivoDisco);
            }

            initUsr();
            fclose(archivoTemp);
            fclose(archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se cambio tipo de sistema de archivos a EXT3 en '%s'\n",particionTrabajo.mount_id);
            return 1;

        }else{
            //Algo salio mal
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
            return -1;
        }
    }else{
        //Algo salio mal
    }
    return 0;
}
int ejecutarRECOVERY(char pathDisco[], PMOUNT particion){
    if(particion.mount_type == 'l' || particion.mount_type == 'L' || particion.mount_type == 'p' || particion.mount_type == 'P'){
        FILE *archivoDiscoTemporal;
        archivoDiscoTemporal = fopen(pathDisco,"rb+");
        SuperBloque sp_aux = getSuperBlock(particion, archivoDiscoTemporal);
        if(sp_aux.sb_magic == -1 && sp_aux.sb_filesystem_type == 3){
            fclose(archivoDiscoTemporal);
            FILE *archivoDisco;
            archivoDisco = fopen(pathDisco,"rb+");
            if(archivoDisco != NULL){
                //Reestructurar Sistema de archivos
                int n = 1 + ((particion.mount_size - sizeof(SuperBloque) - 1)/(sizeof(journal) + sizeof(inode) + 3 * sizeof (contentBlock) + 4));
                int start_reestructura = particion.mount_start + sizeof (SuperBloque) + n * sizeof (journal);
                fseek(archivoDisco,start_reestructura,SEEK_SET);
                //Reescribir los iNodos y Bloques
                char vacio = '0';
                char lleno = '1';
                for(int i = 0; i < 4* n; i++){
                    if(i == 0 || i == n){
                        fwrite(&lleno,1,1,archivoDisco);
                    }else{
                        fwrite(&vacio,1,1,archivoDisco);
                    }
                }
                //Nodo Raiz y Bloque Raiz
                inode nuevo_inodo;
                nuevo_inodo = initiNode(nuevo_inodo);
                nuevo_inodo.i_uid = 1;
                nuevo_inodo.i_gid = 1;
                nuevo_inodo.i_size = 0;
                time_t tiempoActual = time(NULL);
                struct tm *fecha = localtime(&tiempoActual);
                strftime(nuevo_inodo.i_ctime, sizeof(nuevo_inodo.i_ctime)-1, "%d/%m/%y %H:%M", fecha);
                strftime(nuevo_inodo.i_atime, sizeof(nuevo_inodo.i_atime)-1, "%d/%m/%y %H:%M", fecha);
                strftime(nuevo_inodo.i_mtime, sizeof(nuevo_inodo.i_mtime)-1, "%d/%m/%y %H:%M", fecha);
                nuevo_inodo.i_block[0] = 0;
                for (int i = 1; i < 15; i++) {
                    nuevo_inodo.i_block[i] = -1;
                }
                nuevo_inodo.i_type ='0';
                nuevo_inodo.i_perm = 664;
                fwrite(&nuevo_inodo,sizeof (nuevo_inodo),1,archivoDisco);
                contentBlock nuevo_bloque;
                nuevo_bloque = initContentBlock(nuevo_bloque);
                nuevo_bloque.contenapunta[0].f_inode = 0;
                strcpy(nuevo_bloque.contenapunta[0].f_name,".");
                nuevo_bloque.contenapunta[1].f_inode = 0;
                strcpy(nuevo_bloque.contenapunta[1].f_name,"..");
                nuevo_bloque.contenapunta[2].f_inode = -1;
                nuevo_bloque.contenapunta[3].f_inode = -1;
                fwrite(&nuevo_bloque,sizeof (nuevo_bloque),1,archivoDisco);
                //Recargar BitMap
                int blockStart = sp_aux.sb_bm_Block;
                int blockCount = sp_aux.sb_blocks_count;

                char bitmapBlock[270000] = {0};
                char bitAux;
                //Cargar mapa de bits de bloques
                fseek(archivoDisco, blockStart, SEEK_SET);
                for( int i = 0; i < blockCount ; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapBlock[i] = bitAux;
                    bitmapBlock[i+1] = '\0';
                }
                blockStart = sp_aux.sb_bm_inode;
                blockCount = sp_aux.sb_inodes_count;
                fseek(archivoDisco,blockStart,SEEK_SET);
                char bitmapiNode[270000] = {0};
                for( int i = 0; i < blockCount; i++){
                    fread(&bitAux,1,1,archivoDisco);
                    bitmapiNode[i] = bitAux;
                    bitmapiNode[i+1] = '\0';
                }
                usuariologueadop.startiNodes = sp_aux.sb_t_inode;

                strcpy(usuariologueadop.bm_Block , bitmapBlock);
                strcpy(usuariologueadop.bm_iNode , bitmapiNode);
                //Reescritura por Journal
                int userReal = usuariologueadop.id_user;
                int grupoReal = usuariologueadop.id_grupo;
                int start = particion.mount_start + sizeof (SuperBloque); //Posicionarse al principio de los journal
                findUser2(particion,archivoDisco);
                journal entrada;
                entrada = initJournal(entrada);
                //Recuperar SP
                sp_aux.sb_magic = 61267;
                fseek(archivoDisco,particion.mount_start,SEEK_SET);
                fwrite(&sp_aux,sizeof (SuperBloque),1,archivoDisco);
                do{
                    noRecovery = false;
                    fseek(archivoDisco, start,SEEK_SET);
                    fread(&entrada,sizeof (journal),1,archivoDisco);
                    start = ftell(archivoDisco);
                    usuariologueadop.id_user = entrada.j_uid;
                    //Obtener el grupo al que pertenecen
                    for(int let = 0 ; let < globNusers ; let++){
                        if(globusers[let].numGrupo == entrada.j_uid){
                            for(int lot = 0 ; lot < globNGrupos ; lot++ ){
                                if(strcmp(globusers[let].group, globGrupos[lot].name)==0){
                                    usuariologueadop.id_grupo = globGrupos[lot].numGrupo;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    if(entrada.j_uid == -1){
                        //Salida por ultimo journal
                        break;
                    }
                    if(entrada.j_op_type == '0'){
                        //Crear Grupo
                        ejecutarMKGRP("root",archivoDisco);
                        ejecutarMKGRP(entrada.j_name,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == '1'){
                        //Eliminar Grupo
                        ejecutarRMGRP(entrada.j_name,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == '2'){
                        //Crear user
                        char user[999] = {0};
                        char passwd[999] = {0};
                        char group[999] = {0};
                        int auxPos = 0;
                        char sep[999] = {0};
                        for(int i = 0; i < strlen(entrada.j_name) ;i++){
                            if(entrada.j_name[i] == '@'){
                                if(auxPos == 0){
                                    strcpy(user,sep);
                                }
                                if(auxPos == 1){
                                    strcpy(passwd,sep);
                                }
                                auxPos++;
                                memset(&sep,0,999);
                                continue;
                            }
                            sprintf(sep,"%s%c",sep,entrada.j_name[i]);
                        }
                        strcpy(group,sep);
                        ejecutarMKUSR("root","123","root",archivoDisco);
                        ejecutarMKUSR(user,passwd,group,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == '3'){
                        //Eliminar user
                        ejecutarRMUSR(entrada.j_name,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == '4'){
                        //Cambiar user a grupo
                        ejecutarCHGRP(entrada.j_name,entrada.j_content,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'a'){
                        //Crear archivo
                        int auxPos = 0;
                        int size = 0;
                        char content[999] = {0};
                        int bp = 0;
                        int bcontent = 0;
                        //Obtener datos
                        char sep[999] = {0};
                        for (int i = 0; i < strlen(entrada.j_content); i++) {
                            if(entrada.j_content[i] == '@'){
                                if(auxPos == 0){
                                    size = atoi(sep);
                                }
                                if( auxPos == 1){
                                    strcpy(content,sep);
                                }
                                auxPos++;
                                memset(&sep,0,999);
                                continue;
                            }
                            sprintf(sep,"%s%c",sep,entrada.j_content[i]);
                        }
                        bp = atoi(sep);
                        if(strlen(content) > 0){
                            bcontent = 1;
                        }
                        ejecutarARCHIVO(entrada.j_name,size,content,bcontent,bp,0,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'b'){
                        //Eliminar Archivo/Carpeta
                        ejecutarARCHIVO(entrada.j_name,0,"",0,0,3,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'c'){
                        //Editar Archivo
                        int auxPos = 0;
                        int size = 0;
                        char content[999] = {0};
                        int bp = 0;
                        int bcontent = 0;
                        //Obtener datos
                        char sep[999] = {0};
                        for (int i = 0; i < strlen(entrada.j_content); i++) {
                            if(entrada.j_content[i] == '@'){
                                if(auxPos == 0){
                                    size = atoi(sep);
                                }
                                if( auxPos == 1){
                                    strcpy(content,sep);
                                }
                                auxPos++;
                                memset(&sep,0,999);
                                continue;
                            }
                            sprintf(sep,"%s%c",sep,entrada.j_content[i]);
                        }
                        bp = atoi(sep);
                        if(strlen(content) > 0){
                            bcontent = 1;
                        }
                        ejecutarARCHIVO(entrada.j_name,size,content,bcontent,bp,5,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'd'){
                        //Renombrar Archivo/Carpeta
                        ejecutarARCHIVO(entrada.j_name,0,"",0,0,6,entrada.j_content,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'e'){
                        //Crear Carpeta
                        ejecutarARCHIVO(entrada.j_name,1,"",0,atoi(entrada.j_content),1,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'f'){
                        //Copia Archivo/Carpeta
                        ejecutarARCHIVO(entrada.j_name,1,"",0,0,7,entrada.j_content,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'g'){
                        //Mover Archivo/Carpeta
                        ejecutarARCHIVO(entrada.j_name,0,"",0,0,8,entrada.j_content,archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'h'){
                        //Cambiar Dueño Archivo/Carpeta
                        int auxPos = 0;
                        char sep[999] = {0};
                        char uid_nuevo[999] = {0};
                        int uidEjecutar = -1;
                        int br = 0;
                        for(int i = 0 ; i < strlen(entrada.j_content); i++){
                            if(entrada.j_content[i] == '@'){
                                if(auxPos == 0){
                                    strcpy(uid_nuevo,sep);
                                }
                                memset(sep,0,999);
                                auxPos++;
                                continue;
                            }
                            sprintf(sep,"%s%c",sep,entrada.j_content[i]);
                        }
                        br = atoi(sep);
                        //Encontrar user
                        findUser2(particion,archivoDisco);
                        for(int let = 0 ; let < globNusers ; let++){
                            if(strcmp(globusers[let].name,uid_nuevo) == 0){
                                uidEjecutar = globusers[let].numGrupo;
                                break;
                            }
                        }
                        //Ejecutar
                        ejecutarARCHIVO(entrada.j_name,uidEjecutar,"",0,br,11,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    if(entrada.j_op_type == 'z'){
                        //Cambiar Permisos Archivo/Carpeta
                        int auxPos = 0;
                        char sep[999] = {0};
                        int ugo = 0;
                        int br = 0;
                        for(int i = 0 ; i < strlen(entrada.j_content); i++){
                            if(entrada.j_content[i] == '@'){
                                if(auxPos == 0){
                                    ugo = atoi(sep);
                                }
                                memset(sep,0,999);
                                auxPos++;
                                continue;
                            }
                            sprintf(sep,"%s%c",sep,entrada.j_content[i]);
                        }
                        br = atoi(sep);
                        ejecutarARCHIVO(entrada.j_name,ugo,"",0,br,4,"",archivoDisco);
                        fclose(archivoDisco);
                        archivoDisco = fopen(pathDisco,"rb+");
                    }
                    noRecovery = true;
                }while (entrada.j_uid != -1);
                usuariologueadop.id_user = userReal;
                usuariologueadop.id_grupo = grupoReal;
                printf("\033[1;105m COMANDO REALIZADO \033[0m Se recuperaron unos archivos\n");
                fclose(archivoDisco);
                return 1;
            }
        }else{
            if(sp_aux.sb_magic != -1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion estable, no hay que recuperar nada\n");
            }
            if(sp_aux.sb_filesystem_type != 3 && sp_aux.sb_magic == -1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion no es EXT3, no hay Journaling\n");
            }
            if(sp_aux.sb_filesystem_type != 3 && sp_aux.sb_magic != -1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion estable, no hay que recuperar nada\n");
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion no es EXT3, no hay Journaling\n");
            }
        }
    }
    return 0;
}
int ejecutarSYSLOSS(char pathDisco[], PMOUNT particion){
    if(particion.mount_type == 'l' || particion.mount_type == 'L' || particion.mount_type == 'p' || particion.mount_type == 'P'){
        FILE *archivoDiscoTemporal;
        archivoDiscoTemporal = fopen(pathDisco,"rb+");
        SuperBloque sp_aux = getSuperBlock(particion,archivoDiscoTemporal);
        if(sp_aux.sb_magic == 61267){
            fclose(archivoDiscoTemporal);
            sp_aux.sb_magic = -1;
            FILE *archivoDisco;
            archivoDisco = fopen(pathDisco,"rb+");
            if(archivoDisco != NULL){
                int start = sp_aux.sb_bm_inode;
                fseek(archivoDisco,start,SEEK_SET);
                int actual = ftell(archivoDisco);
                int final = particion.mount_start+particion.mount_size;
                int maximo = 1024 * 1024;
                char insert[1024*1024] = {0};
                while (actual < final) {
                    if((final - actual) > maximo){
                        fwrite(&insert,sizeof (char),maximo,archivoDisco);
                        actual = ftell(archivoDisco);
                    }else{
                        int diferencia = final - actual;
                        char insertSegundo[diferencia];
                        for(int i = 0 ; i < (final-actual) ; i++){
                            insertSegundo[i] = '\0';
                        }
                        fwrite(&insertSegundo,sizeof (char),diferencia,archivoDisco);
                        actual = ftell(archivoDisco);
                    }
                }
                fseek(archivoDisco,particion.mount_start,SEEK_SET);
                fwrite(&sp_aux,sizeof (SuperBloque),1,archivoDisco);

                memset(&usuariologueadop.bm_Block,0,270000);
                memset(&usuariologueadop.bm_iNode,0,270000);

                fclose(archivoDisco);
                printf("\033[1;105m COMANDO REALIZADO \033[0m Se replico perdida de datos en '%s'\n",particion.mount_id);
                return 1;
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
            return -1;
        }
    }
    return 0;
}
int ejecutarMKDisk(char path[], int sizeDisk, char fitDisk){
    if(existsFile(path)==1){
        //Ya existe archivo de disco
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco '%s' ya existe \n",path);
        return -1;
    }
    int sizeFisico = sizeDisk -140;
    char auxMKDisk[999] = {0};
    strcpy(auxMKDisk,path);
/*
    char comando []="sudo mkdir -p \'";
    strcat(comando,path);
    strcat(comando,"\'");
    system(comando);
    char comando2 []="sudo chmod -R 777 \'";
    strcat(comando2,path);
    strcat(comando2,"\'");
    system(comando2);
*/




    CrearPath(path);
    if( verificarExtension(path,".disk") == 0 ){
        FILE *archivoDisco;
        archivoDisco=fopen(auxMKDisk,"wb");
        if(archivoDisco == NULL)
        {
            return -1;
        }
        else
        {
            MBR aux_mbr;
            aux_mbr.mbr_id = 200313184;
            aux_mbr.mbr_size = sizeDisk;
            aux_mbr.mbr_fit = fitDisk;
            for(int i = 0; i < 4; i++)
            {
                aux_mbr.mbr_partitions[i].partition_start = -1 ;
                aux_mbr.mbr_partitions[i].partition_status = 'n' ;
            }
            time_t tiempoActual = time(NULL);
            struct tm *fecha = localtime(&tiempoActual);
            strftime(aux_mbr.mbr_creation_date, sizeof(aux_mbr.mbr_creation_date)-1, "%d/%m/%y %H:%M", fecha);
            fwrite(&aux_mbr,sizeof(aux_mbr),1,archivoDisco);
            int sizeMax = 1024 *1024;
            if(sizeFisico > sizeMax){ //Insercion de datos si sizeFisico > 1MByte
                char insert[1024*1024] = {0};
                while(sizeFisico > sizeMax){
                    sizeFisico = sizeFisico - sizeMax;
                    fwrite(insert , sizeof(char),sizeMax,archivoDisco);
                }
            }
            //Insercion de datos si sizeFisico < 1MByte
            char ultimoInsert[sizeFisico];
            for(int i =0 ; i < sizeFisico; i++){
                ultimoInsert[i] = '\0';
            }
            fwrite(&ultimoInsert , sizeof(char),sizeFisico,archivoDisco);
            //Fin de creacion de disco - exito
            fclose(archivoDisco);
            printf("\033[1;105m COMANDO REALIZADO \033[0m Se ha generado disco de '%i' en ruta '%s' \n",sizeDisk ,auxMKDisk);
            return 1;
        }
    }
    else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco no puede ser generado \n");
        return -1;
    }
    return 0;
}
int ejecutarRMDisk(char path[]){
    if(existsFile(path)==1){
        if(verificarExtension(path,".disk") == 0){
            if( verificacion() == 1 ){
                int beliminado = remove(path);
                if( beliminado == 0 ){
                    printf("\033[1;105m COMANDO REALIZADO \033[0m archivo de disco eliminado \n");
                    return 1;
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo de disco no se pudo eliminar \n");
                    return -1;
                }
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo de disco no reconocido \n");
            return -1;
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo no existe \n");
        return -1;
    }
    return 0;
}
int ejecutarFDisk( char path[], char name[], int size, char type, char fit, int del, int add, int tipoFDisk){
    if(existsFile(path) == 1 && verificarExtension(path,".disk") == 0){
        MBR aux_mbr;
        int found = 0;
        int inicio_disco = 140;
        int tope_particion = 0;
        FILE *archivoDisco;
        archivoDisco = fopen(path,"rb+");
        if(archivoDisco != NULL){
            int auxPos = -1;
            fread(&aux_mbr, sizeof(aux_mbr), 1 , archivoDisco);
            tope_particion = aux_mbr.mbr_size;
            MBRPARTITION aux_partition;
            int partition_found = -1;
            int extended_found = -1;
            int cantidad_primarias = 0;
            //Ciclo de busqueda de particiones
            for(int i = 0; i < 4; i++){
                if( aux_mbr.mbr_partitions[i].partition_start != -1){
                    if(strcmp(name,aux_mbr.mbr_partitions[i].partition_name)==0){
                        //Ya existe una particion con ese name
                        auxPos = i;
                        partition_found = 1;
                    }
                }
                if(aux_mbr.mbr_partitions[i].partition_type == 'e' ){
                    //Existe particion extendida
                    extended_found = 1;
                    aux_partition = aux_mbr.mbr_partitions[i];
                }
                if(aux_mbr.mbr_partitions[i].partition_type == 'p' && aux_mbr.mbr_partitions[i].partition_start != -1 ){
                    cantidad_primarias++;
                }
            }
            if(partition_found == -1 && extended_found == 1){
                //REVISAR CODIGO
                //buscar particion con ese 'Name' en particion 'Extend'
                //No existe en particiones del disco
                int ebr_encontrado = 0;
                EBR aux_ebr;
                EBR aux_ebr_2;
                fseek(archivoDisco, aux_partition.partition_start, SEEK_SET);
                fread(&aux_ebr, sizeof(aux_ebr),1, archivoDisco);
                aux_ebr_2 = aux_ebr;
                if( aux_ebr.ebr_start != -1 || aux_ebr.ebr_next != -1){
                    if(strcmp(aux_ebr.ebr_name, name)==0){
                        partition_found = 2;
                        ebr_encontrado = 1;
                    }else{
                        while(aux_ebr.ebr_next != -1){
                            fseek(archivoDisco, aux_ebr.ebr_next, SEEK_SET);
                            aux_ebr_2 = aux_ebr;
                            fread(&aux_ebr, sizeof(aux_ebr),1,archivoDisco);
                            if(strcmp(aux_ebr.ebr_name,name)==0){
                                partition_found = 2;
                                ebr_encontrado = 1;
                                break;
                            }
                        }
                    }
                }
                if(type == 'l' || partition_found == 2){
                    if(partition_found != -1){
                        //Add o Delete particion logica
                        if(tipoFDisk == 0){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion logica existe con nombre '%s' \n",name);
                        }else if(tipoFDisk == 1){
                            //Add particion logica
                            int tope_EBR = aux_ebr.ebr_next;
                            if(aux_ebr.ebr_next == -1){
                                //No existe siguiente EBR
                                tope_EBR = aux_partition.partition_start + aux_partition.partition_size;
                            }
                            int final_ebr = aux_ebr.ebr_size + add;
                            if(aux_ebr.ebr_start + final_ebr < tope_EBR && final_ebr > 32){
                                aux_ebr.ebr_size = final_ebr;
                                fseek(archivoDisco, aux_ebr.ebr_start, SEEK_SET);
                                fwrite(&aux_ebr, sizeof(aux_ebr),1,archivoDisco);
                                printf("\033[1;105m COMANDO REALIZADO \033[0m -ADD~:~ en particion '%s' bajo '%d' nuevo size '%d' \n", name, add, final_ebr);
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Imposible utilizar -ADD~:~ en particion '%s' bajo '%d' \n", name, add);
                            }
                        }else if(tipoFDisk == 2){
                            //Delete particion logica
                            if( verificacion() == 1){
                                int principio = aux_ebr.ebr_start;
                                int sizeReal = aux_ebr.ebr_size;
                                if(del == 1){
                                    //Eliminacion completa
                                    if(aux_ebr.ebr_start == aux_ebr_2.ebr_start){
                                        //Borrar primera EBR
                                        principio = principio + 32;
                                        sizeReal = sizeReal - 32;
                                        fseek(archivoDisco, aux_ebr_2.ebr_start,SEEK_SET);
                                        aux_ebr_2.ebr_status = 'n';
                                        aux_ebr_2.ebr_start = -1;
                                        aux_ebr_2.ebr_size = -1;
                                        memset(&aux_ebr_2.ebr_name,0,sizeof(aux_ebr_2.ebr_name));
                                        fwrite(&aux_ebr_2,sizeof(aux_ebr_2),1,archivoDisco);
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion logica '%s' en '%s' \n",name,path);
                                    }else{
                                        //Borrar otra EBR
                                        aux_ebr_2.ebr_next = aux_ebr.ebr_next;
                                        fseek(archivoDisco, aux_ebr_2.ebr_start, SEEK_SET);
                                        fwrite(&aux_ebr_2, sizeof(aux_ebr_2),1,archivoDisco);
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion logica '%s' en '%s' \n",name,path);
                                    }
                                    fseek(archivoDisco,principio,SEEK_SET);
                                    int sizeMax = 1024 * 1024;
                                    if(sizeReal > sizeMax){
                                        char insert[1024 * 1024] = {0};
                                        while(sizeReal > sizeMax){
                                            sizeReal = sizeReal - sizeMax;
                                            fwrite(insert, sizeof(char),sizeMax,archivoDisco);
                                        }
                                    }
                                    char ultimoInsert[sizeReal];
                                    for(int i = 0; i < sizeReal ; i++){
                                        ultimoInsert[i] = '\0';
                                    }
                                    fwrite(&ultimoInsert,sizeof(char),sizeReal,archivoDisco);
                                }else if(del == 2){
                                    //Eliminacion rapida
                                    if(aux_ebr.ebr_start == aux_ebr_2.ebr_start){
                                        //Borrar primera EBR
                                        fseek(archivoDisco, aux_ebr_2.ebr_start,SEEK_SET);
                                        aux_ebr_2.ebr_status = 'n';
                                        aux_ebr_2.ebr_start = -1;
                                        aux_ebr_2.ebr_size = -1;
                                        memset(&aux_ebr_2.ebr_name,0,sizeof(aux_ebr_2.ebr_name));
                                        fwrite(&aux_ebr_2,sizeof(aux_ebr_2),1,archivoDisco);
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion logica '%s' en '%s' \n",name,path);
                                    }else{
                                        //Borrar otra EBR
                                        aux_ebr_2.ebr_next = aux_ebr.ebr_next;
                                        fseek(archivoDisco, aux_ebr_2.ebr_start, SEEK_SET);
                                        fwrite(&aux_ebr_2, sizeof(aux_ebr_2),1,archivoDisco);
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion logica '%s' en '%s' \n",name,path);
                                    }
                                }else{
                                    //Algo salio mal
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametros para FDisk incorrectos \n");
                                }
                            }
                        }else{
                            //Algo salio mal
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametros para FDisk incorrectos \n");
                        }
                    }else{
                        //Insertar particion logica
                        EBR insercion_primero;
                        EBR insercion_aux;
                        fseek(archivoDisco, aux_partition.partition_start, SEEK_SET);
                        fread(&insercion_primero, sizeof(insercion_primero),1, archivoDisco);
                        if(insercion_primero.ebr_next != -1 || insercion_primero.ebr_start != -1){
                            //Insertar la n-esima particion logica (n>1) o Reemplazo de primer EBR
                            char fit_Extendida = aux_partition.partition_fit;
                            int exito_logica = -1;
                            if(fit_Extendida == 'f'){
                                //Primer ajuste
                                while(insercion_primero.ebr_next != -1){
                                    if(insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) > 0){
                                        //Hay espacio entre particiones
                                        if(size <= insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) ){
                                            //Es posible asignar entre las particiones logicas
                                            //Cumple condicion de primer EBR eliminado
                                            EBR nuevo_ebr;

                                            if(insercion_primero.ebr_start == -1 && insercion_primero.ebr_size == -1){
                                                nuevo_ebr.ebr_start = aux_partition.partition_start;
                                            }else{
                                                nuevo_ebr.ebr_start = insercion_primero.ebr_size+insercion_primero.ebr_start;
                                            }

                                            nuevo_ebr.ebr_fit = fit;
                                            nuevo_ebr.ebr_next = insercion_primero.ebr_next;
                                            nuevo_ebr.ebr_size = size;
                                            nuevo_ebr.ebr_status = 'a';

                                            memset(&nuevo_ebr.ebr_name,0,sizeof (nuevo_ebr.ebr_name));
                                            strcpy(nuevo_ebr.ebr_name,name);

                                            if(insercion_primero.ebr_start != -1){
                                                //Insertar con primer EBR
                                                //Reemplazo entre EBR
                                                insercion_primero.ebr_next = nuevo_ebr.ebr_start;
                                                fseek(archivoDisco,insercion_primero.ebr_start,SEEK_SET);
                                                fwrite(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);

                                                fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                                fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);

                                                exito_logica = 1;
                                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                                break;
                                            }else{
                                                //Insertar con primer EBR eliminado
                                                //Reemplazo en primer EBR
                                                fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                                fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);

                                                exito_logica = 1;
                                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                                break;
                                            }
                                        }
                                    }
                                    if(insercion_primero.ebr_next == -1){
                                        //Insertar al final
                                        //Sigue recorriendo los EBR
                                        EBR nuevo_ebr;

                                        nuevo_ebr.ebr_start = insercion_primero.ebr_size+insercion_primero.ebr_start;
                                        nuevo_ebr.ebr_fit = fit;
                                        nuevo_ebr.ebr_next = -1;
                                        nuevo_ebr.ebr_size = size;
                                        nuevo_ebr.ebr_status = 'a';

                                        memset(&nuevo_ebr.ebr_name,0,sizeof (nuevo_ebr.ebr_name));
                                        strcpy(nuevo_ebr.ebr_name,name);

                                        insercion_primero.ebr_next = nuevo_ebr.ebr_start;
                                        fseek(archivoDisco,insercion_primero.ebr_start,SEEK_SET);
                                        fwrite(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);

                                        fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                        fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);

                                        exito_logica = 1;
                                        printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                        break;
                                    }
                                    insercion_aux = insercion_primero;
                                    fseek(archivoDisco, insercion_primero.ebr_next, SEEK_SET);
                                    fread(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);
                                }
                                if(insercion_primero.ebr_next == -1 && exito_logica == -1){
                                    //Insertar al final
                                    //No recorrio los EBR
                                    EBR nuevo_ebr;

                                    nuevo_ebr.ebr_start = insercion_primero.ebr_size+insercion_primero.ebr_start;
                                    nuevo_ebr.ebr_fit = fit;
                                    nuevo_ebr.ebr_next = -1;
                                    nuevo_ebr.ebr_size = size;
                                    nuevo_ebr.ebr_status = 'a';

                                    memset(&nuevo_ebr.ebr_name,0,sizeof (nuevo_ebr.ebr_name));
                                    strcpy(nuevo_ebr.ebr_name,name);

                                    insercion_primero.ebr_next = nuevo_ebr.ebr_start;
                                    fseek(archivoDisco,insercion_primero.ebr_start,SEEK_SET);
                                    fwrite(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);

                                    fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                    fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);

                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                }else{
                                    //Algo salio mal
                                    //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Algo salio muy mal \n");
                                }
                            }else if(fit_Extendida == 'b'){
                                //Mejor ajuste
                                EBR mejor_fit;
                                mejor_fit.ebr_next = 999999999;
                                mejor_fit.ebr_size = 0;
                                mejor_fit.ebr_start = 0;
                                //Recorrer por mejor fit
                                while(insercion_primero.ebr_next != -1){
                                    if(insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) > 0){
                                        //Hay espacio entre particiones
                                        if(size <= insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) ){
                                            //Particion entre logicas
                                            if( (insercion_primero.ebr_next-insercion_primero.ebr_start-insercion_primero.ebr_size) < (mejor_fit.ebr_next-mejor_fit.ebr_start-mejor_fit.ebr_size) ){
                                                mejor_fit = insercion_primero;
                                                exito_logica = 1;
                                            }
                                        }
                                    }
                                    if(insercion_primero.ebr_next == -1){
                                        //Insertar al final
                                        if(size <= aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size){
                                            //Espacio libre entre la ultima particion y el final de la extendida
                                            if( (aux_partition.partition_start+aux_partition.partition_size-insercion_primero.ebr_start-insercion_primero.ebr_size) < (mejor_fit.ebr_next-mejor_fit.ebr_start-mejor_fit.ebr_size) ){
                                                //Ultima particion logica
                                                mejor_fit = insercion_primero;
                                                exito_logica = 1;
                                            }
                                        }
                                    }
                                    insercion_aux = insercion_primero;
                                    fseek(archivoDisco, insercion_primero.ebr_next, SEEK_SET);
                                    fread(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);
                                }
                                if(insercion_primero.ebr_next == -1){
                                    //Insertar al final
                                    //No recorrio los EBR
                                    if(size <= aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size){
                                        if( (aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size) < (mejor_fit.ebr_next-mejor_fit.ebr_start-mejor_fit.ebr_size) ){
                                            //Ultima particion logica
                                            mejor_fit = insercion_primero;
                                            exito_logica = 1;
                                        }
                                    }
                                }
                                if(exito_logica == 1){
                                    //Encontro mejor fit
                                    EBR nuevo_ebr;

                                    nuevo_ebr.ebr_start = mejor_fit.ebr_start+mejor_fit.ebr_size;
                                    nuevo_ebr.ebr_fit = fit;
                                    nuevo_ebr.ebr_status = 'a';
                                    nuevo_ebr.ebr_size = size;
                                    nuevo_ebr.ebr_next = mejor_fit.ebr_next;

                                    memset(&nuevo_ebr.ebr_name,0,sizeof(nuevo_ebr.ebr_name));
                                    strcpy(nuevo_ebr.ebr_name,name);

                                    mejor_fit.ebr_next = nuevo_ebr.ebr_start;
                                    fseek(archivoDisco,mejor_fit.ebr_start,SEEK_SET);
                                    fwrite(&mejor_fit,sizeof(mejor_fit),1,archivoDisco);

                                    fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                    fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                }else{
                                    //No se encontro mejor fit
                                    //Error
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: error de mejor fit \n");
                                }
                            }else if(fit_Extendida == 'w'){
                                //Peor ajuste
                                EBR peor_fit;
                                peor_fit.ebr_next = 0;
                                peor_fit.ebr_size = 0;
                                peor_fit.ebr_start = 0;
                                //Recorrer por mejor fit
                                while(insercion_primero.ebr_next != -1){
                                    if(insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) > 0){
                                        //Hay espacio entre particiones
                                        if(size <= insercion_primero.ebr_next-(insercion_primero.ebr_start+insercion_primero.ebr_size) ){
                                            //Particion entre logicas
                                            if( (insercion_primero.ebr_next-insercion_primero.ebr_start-insercion_primero.ebr_size) > (peor_fit.ebr_next-peor_fit.ebr_start-peor_fit.ebr_size) ){
                                                peor_fit = insercion_primero;
                                                exito_logica = 1;
                                            }
                                        }
                                    }
                                    if(insercion_primero.ebr_next == -1){
                                        //Insertar al final
                                        if(size <= aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size){
                                            //Espacio libre entre la ultima particion y el final de la extendida
                                            if( (aux_partition.partition_start+aux_partition.partition_size-insercion_primero.ebr_start-insercion_primero.ebr_size) > (peor_fit.ebr_next-peor_fit.ebr_start-peor_fit.ebr_size) ){
                                                //Ultima particion logica
                                                peor_fit = insercion_primero;
                                                exito_logica = 1;
                                            }
                                        }
                                    }
                                    insercion_aux = insercion_primero;
                                    fseek(archivoDisco, insercion_primero.ebr_next, SEEK_SET);
                                    fread(&insercion_primero,sizeof(insercion_primero),1,archivoDisco);
                                }
                                if(insercion_primero.ebr_next == -1){
                                    //Insertar al final
                                    //No recorrio los EBR
                                    if(size <= aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size){
                                        if( (aux_partition.partition_start+aux_partition.partition_size - insercion_primero.ebr_start - insercion_primero.ebr_size) > (peor_fit.ebr_next-peor_fit.ebr_start-peor_fit.ebr_size) ){
                                            //Ultima particion logica
                                            peor_fit = insercion_primero;
                                            exito_logica = 1;
                                        }
                                    }
                                }
                                if(exito_logica == 1){
                                    //Encontro Peor fit
                                    EBR nuevo_ebr;

                                    nuevo_ebr.ebr_start = peor_fit.ebr_start+peor_fit.ebr_size;
                                    nuevo_ebr.ebr_fit = fit;
                                    nuevo_ebr.ebr_status = 'a';
                                    nuevo_ebr.ebr_size = size;
                                    nuevo_ebr.ebr_next = peor_fit.ebr_next;

                                    memset(&nuevo_ebr.ebr_name,0,sizeof(nuevo_ebr.ebr_name));
                                    strcpy(nuevo_ebr.ebr_name,name);

                                    peor_fit.ebr_next = nuevo_ebr.ebr_start;
                                    fseek(archivoDisco,peor_fit.ebr_start,SEEK_SET);
                                    fwrite(&peor_fit,sizeof(peor_fit),1,archivoDisco);

                                    fseek(archivoDisco,nuevo_ebr.ebr_start,SEEK_SET);
                                    fwrite(&nuevo_ebr,sizeof(nuevo_ebr),1,archivoDisco);
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                                }else{
                                    //No se encontro peor fit
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: error de peor fit \n");
                                }
                            }else{
                                //Algo salio mal
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: lectura de Fit de particion Extendida fallida \n");
                            }
                        }else{
                            //Insertar primer EBR
                            int tope_EBR = aux_partition.partition_size;
                            if(insercion_primero.ebr_next != -1){
                                //Precaucion por siguiente EBR
                                tope_EBR = insercion_primero.ebr_next;
                            }
                            if( size > tope_EBR){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: insuficientes bytes para insertar particion logica \n");
                            }else if( size <= 32 ){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: -SIZE~:~ para particiones logicas deben tener un minimo de 32 bytes\n");
                            }else{
                                //Efectivo sobreescribir primer EBR
                                EBR nuevo_ebr;

                                nuevo_ebr.ebr_start = aux_partition.partition_start;
                                nuevo_ebr.ebr_fit = fit;
                                nuevo_ebr.ebr_next = -1;
                                nuevo_ebr.ebr_size = size;
                                nuevo_ebr.ebr_status = 'a';
                                memset(&nuevo_ebr.ebr_name,0,sizeof(nuevo_ebr.ebr_name));
                                strcpy(nuevo_ebr.ebr_name, name);

                                fseek(archivoDisco, aux_partition.partition_start, SEEK_SET);
                                fwrite(&nuevo_ebr, sizeof(nuevo_ebr),1,archivoDisco);
                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion logica '%s' de '%i' en '%s' \n",name,size,path);
                            }
                        }
                    }
                }
            }
            if(type == 'l' && extended_found != 1){
                //Se desea generar particion logica, pero no existe extend
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se puede generar particion logica, genere primero una particion extendida \n");
            }else if( partition_found == -1 && type != 'l'){
                //Se quiere generar particion Primaria o Extendida
                if(size > 0){
                    //Revisar fit del disco
                    char fit_disco = aux_mbr.mbr_fit;
                    int auxPos2 = -1;
                    //Seguir a metodo de insercion
                    if(fit_disco == 'f'){
                        //Primer ajuste
                        for(int i = 0; i<4 ; i++){
                            auxPos2 = i;
                            if(aux_mbr.mbr_partitions[i].partition_start == -1){
                                //Encontro particion vacia
                                break;
                            }
                            if( i == 0 ){
                                int disponible = aux_mbr.mbr_partitions[i].partition_start -140;
                                if(disponible >= size){
                                    inicio_disco = 140;
                                    found = 1;
                                }
                            }else{
                                int disponible = aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i-1].partition_start - aux_mbr.mbr_partitions[i-1].partition_size;
                                if(disponible >= size){
                                    inicio_disco = aux_mbr.mbr_partitions[i-1].partition_start + aux_mbr.mbr_partitions[i-1].partition_size;
                                    found = 1;
                                }
                            }
                            if(found==0){
                                inicio_disco = aux_mbr.mbr_partitions[i].partition_start + aux_mbr.mbr_partitions[i].partition_size;
                            }
                        }
                    }else if(fit_disco == 'b'){
                        //Mejor ajuste
                        int posiciones[5] = {0};
                        int espacios[5] = {0};
                        for(int i = 0; i<4 ; i++){
                            auxPos2 = i;
                            if(aux_mbr.mbr_partitions[i].partition_start == -1){
                                //Encontro particion vacia
                                break;
                            }
                            if( i == 0 ){
                                int disponible = aux_mbr.mbr_partitions[i].partition_start -140;
                                espacios[i] = disponible;
                                posiciones[i] = i;
                                if( aux_mbr.mbr_partitions[i+1].partition_start == -1 ){
                                    espacios[i+1] = tope_particion - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }else{
                                    espacios[i+1] = aux_mbr.mbr_partitions[i+1].partition_start - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }
                            }else{
                                int disponible = aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i-1].partition_start - aux_mbr.mbr_partitions[i-1].partition_size;
                                espacios[i] = disponible;
                                posiciones[i] = i;
                                if( aux_mbr.mbr_partitions[i+1].partition_start == -1 || i == 3){
                                    espacios[i+1] = tope_particion - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }else{
                                    espacios[i+1] = aux_mbr.mbr_partitions[i+1].partition_start - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }
                            }
                        }
                        int posAjuste = -1;
                        int mejorAjuste = 999999999;
                        for(int i = 0; i < 5 ; i++){
                            if(espacios[i] < mejorAjuste && espacios[i] >= size){
                                mejorAjuste = espacios[i];
                                posAjuste = i;
                            }
                        }
                        if( posAjuste != -1){
                            if(posAjuste == 0){
                                //0 antes de la primera particion
                                inicio_disco = 140;
                            }else if(posAjuste == 1){
                                //Despues de la primera particion
                                inicio_disco = aux_mbr.mbr_partitions[0].partition_start + aux_mbr.mbr_partitions[0].partition_size;
                            }else if(posAjuste == 2){
                                //Despues de la segunda particion
                                inicio_disco = aux_mbr.mbr_partitions[1].partition_start + aux_mbr.mbr_partitions[1].partition_size;
                            }else if(posAjuste == 3){
                                //Despues de la tercera particion
                                inicio_disco = aux_mbr.mbr_partitions[2].partition_start + aux_mbr.mbr_partitions[2].partition_size;
                            }else{ //posAjuste == 4
                                //Despues de la cuarta particion
                                inicio_disco = aux_mbr.mbr_partitions[3].partition_start + aux_mbr.mbr_partitions[3].partition_size;
                            }
                        }else{
                            if( auxPos2 == 0 && aux_mbr.mbr_partitions[auxPos2].partition_start == -1 ){
                                inicio_disco = 140;
                            }else{
                                if(aux_mbr.mbr_partitions[auxPos2].partition_start == -1){
                                    inicio_disco = aux_mbr.mbr_partitions[auxPos2-1].partition_start + aux_mbr.mbr_partitions[auxPos2-1].partition_size;
                                }else{
                                    inicio_disco = aux_mbr.mbr_partitions[auxPos2].partition_start + aux_mbr.mbr_partitions[auxPos2].partition_size;
                                }
                            }
                        }
                    }else if(fit_disco == 'w'){
                        //Peor ajuste
                        int posiciones[5] = {0};
                        int espacios[5] = {0};
                        for(int i = 0; i<4 ; i++){
                            auxPos2 = i;
                            if(aux_mbr.mbr_partitions[i].partition_start == -1){
                                //Encontro particion vacia
                                break;
                            }
                            if( i == 0 ){
                                int disponible = aux_mbr.mbr_partitions[i].partition_start -140;
                                espacios[i] = disponible;
                                posiciones[i] = i;
                                if( aux_mbr.mbr_partitions[i+1].partition_start == -1 ){
                                    espacios[i+1] = tope_particion - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }else{
                                    espacios[i+1] = aux_mbr.mbr_partitions[i+1].partition_start - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }
                            }else{
                                int disponible = aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i-1].partition_start - aux_mbr.mbr_partitions[i-1].partition_size;
                                espacios[i] = disponible;
                                posiciones[i] = i;
                                if( aux_mbr.mbr_partitions[i+1].partition_start == -1 || i == 3){
                                    espacios[i+1] = tope_particion - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }else{
                                    espacios[i+1] = aux_mbr.mbr_partitions[i+1].partition_start - aux_mbr.mbr_partitions[i].partition_start - aux_mbr.mbr_partitions[i].partition_size;
                                    posiciones[i+1] = i+1;
                                }
                            }
                        }
                        int posAjuste = -1;
                        int peorAjuste = 0;
                        for(int i = 0; i < 5 ; i++){
                            if(espacios[i] > peorAjuste && espacios[i] >= size){
                                peorAjuste = espacios[i];
                                posAjuste = i;
                            }
                        }
                        if( posAjuste != -1){
                            if(posAjuste == 0){
                                //0 antes de la primera particion
                                inicio_disco = 140;
                            }else if(posAjuste == 1){
                                //Despues de la primera particion
                                inicio_disco = aux_mbr.mbr_partitions[0].partition_start + aux_mbr.mbr_partitions[0].partition_size;
                            }else if(posAjuste == 2){
                                //Despues de la segunda particion
                                inicio_disco = aux_mbr.mbr_partitions[1].partition_start + aux_mbr.mbr_partitions[1].partition_size;
                            }else if(posAjuste == 3){
                                //Despues de la tercera particion
                                inicio_disco = aux_mbr.mbr_partitions[2].partition_start + aux_mbr.mbr_partitions[2].partition_size;
                            }else{ //posAjuste == 4
                                //Despues de la cuarta particion
                                inicio_disco = aux_mbr.mbr_partitions[3].partition_start + aux_mbr.mbr_partitions[3].partition_size;
                            }
                        }else{
                            if( auxPos2 == 0 && aux_mbr.mbr_partitions[auxPos2].partition_start == -1 ){
                                inicio_disco = 140;
                            }else{
                                if(aux_mbr.mbr_partitions[auxPos2].partition_start == -1){
                                    inicio_disco = aux_mbr.mbr_partitions[auxPos2-1].partition_start + aux_mbr.mbr_partitions[auxPos2-1].partition_size;
                                }else{
                                    inicio_disco = aux_mbr.mbr_partitions[auxPos2].partition_start + aux_mbr.mbr_partitions[auxPos2].partition_size;
                                }
                            }
                        }
                    }
                    if(auxPos2 == 4){
                        //Particiones llenas
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco con particiones llenas \n");
                    }else{
                        if( tope_particion >= inicio_disco+size ){
                            if( cantidad_primarias == 3 && type == 'p' ){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco ya tiene 3 particiones primarias \n");
                            }else{
                                if(extended_found == 1 && type == 'e' ){
                                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco ya tiene particion extendida \n");
                                }else{
                                    aux_mbr.mbr_partitions[auxPos2].partition_fit = fit;
                                    aux_mbr.mbr_partitions[auxPos2].partition_size = size;
                                    memset(&aux_mbr.mbr_partitions[auxPos2].partition_name,0,sizeof(aux_mbr.mbr_partitions[auxPos2].partition_name));
                                    strcpy(aux_mbr.mbr_partitions[auxPos2].partition_name,name);
                                    aux_mbr.mbr_partitions[auxPos2].partition_start = inicio_disco;
                                    aux_mbr.mbr_partitions[auxPos2].partition_status = 'a';
                                    aux_mbr.mbr_partitions[auxPos2].partition_type = type;
                                    if(type == 'e'){
                                        EBR aux_ebr;
                                        aux_ebr.ebr_start = -1;
                                        aux_ebr.ebr_next = -1;
                                        aux_ebr.ebr_status = 'n';
                                        fseek(archivoDisco, inicio_disco, SEEK_SET);
                                        fwrite(&aux_ebr, sizeof(aux_ebr),1,archivoDisco);
                                    }
                                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se genero particion '%s' en '%s' con size '%i' de tipo '%c' \n", name, path, size, type );
                                }
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Size muy grande en %s se recomienda como maximo: %i \n", name, tope_particion);
                        }
                    }
                }else if(tipoFDisk == 0){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -size~:~ debe ser mayor a cero \n");
                }else{
                     printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Valores para ejecutar FDisk no coinciden \n");
                }
            }else if( partition_found == 1){
                //Se encontro disco, trabajara add o delete
                if(tipoFDisk == 0){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion ya existe \n");
                }else if(tipoFDisk == 1){
                    //Proceder a hacer Add
                    int posicion_final = aux_mbr.mbr_partitions[auxPos].partition_start + aux_mbr.mbr_partitions[auxPos].partition_size + add;
                    tope_particion = aux_mbr.mbr_size;
                    if(auxPos < 4){
                        if(aux_mbr.mbr_partitions[auxPos+1].partition_start != -1){
                            tope_particion = aux_mbr.mbr_partitions[auxPos+1].partition_start;
                        }
                    }
                    if( posicion_final < tope_particion && posicion_final > aux_mbr.mbr_partitions[auxPos].partition_start){
                        aux_mbr.mbr_partitions[auxPos].partition_size = aux_mbr.mbr_partitions[auxPos].partition_size + add;
                        printf("\033[1;105m COMANDO REALIZADO \033[0m -ADD~:~ en particion '%s' bajo '%d' nuevo size '%d' \n", name, add, aux_mbr.mbr_partitions[auxPos].partition_size);
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Imposible utilizar -ADD~:~ en particion '%s' bajo '%d' \n", name, add);
                    }
                }else{
                    //Proceder a hacer Delete
                    if( verificacion() == 1){
                        //Se procede a eliminar
                        if(del == 1){
                            //Eliminacion completa
                            fseek( archivoDisco, aux_mbr.mbr_partitions[auxPos].partition_start, SEEK_SET);
                            int sizeMax = 1024 * 1024;
                            int sizeExtendida = aux_mbr.mbr_partitions[auxPos].partition_size;
                            if(sizeExtendida > sizeMax){
                                char insert[1024*1024] = {0};
                                while( sizeExtendida > sizeMax){
                                    sizeExtendida = sizeExtendida - sizeMax;
                                    fwrite(insert,sizeof(char),sizeMax,archivoDisco);
                                }
                            }
                            char ultimoInsert[sizeExtendida];
                            for(int i =0 ; i < sizeExtendida; i++){
                                ultimoInsert[i] = '\0';
                            }
                            fwrite(&ultimoInsert , sizeof(char),sizeExtendida,archivoDisco);
                            aux_mbr.mbr_partitions[auxPos].partition_start = -1;
                            aux_mbr.mbr_partitions[auxPos].partition_status = 'n';
                            memset(&aux_mbr.mbr_partitions[auxPos].partition_name,0,sizeof(aux_mbr.mbr_partitions[auxPos].partition_name));
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion '%s' \n", name);
                        }else if(del == 2){
                            //Eliminacion rapida
                            aux_mbr.mbr_partitions[auxPos].partition_start = -1;
                            aux_mbr.mbr_partitions[auxPos].partition_status = 'n';
                            memset(&aux_mbr.mbr_partitions[auxPos].partition_name,0,sizeof(aux_mbr.mbr_partitions[auxPos].partition_name));
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se elimino particion '%s' \n", name);
                        }else{
                            //Algo esta mal
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Un valor equivoco fue introducido \n");
                        }
                    }
                }
            }
            //Ciclo para ordenar las particiones
            MBRPARTITION aux_orden;
            for(int i = 1; i<4; i++){
                for(int j =0; j< 4-i; j++){
                    if( ((aux_mbr.mbr_partitions[j].partition_start > aux_mbr.mbr_partitions[j+1].partition_start) && aux_mbr.mbr_partitions[j+1].partition_start != -1) || aux_mbr.mbr_partitions[j].partition_start == -1){
                        aux_orden = aux_mbr.mbr_partitions[j+1];
                        aux_mbr.mbr_partitions[j+1] = aux_mbr.mbr_partitions[j];
                        aux_mbr.mbr_partitions[j] = aux_orden;
                    }
                }
            }
            //Cerrar y Reimprimir el archivo
            fseek(archivoDisco, 0, SEEK_SET);
            fwrite(&aux_mbr, sizeof(aux_mbr), 1, archivoDisco);
            fclose(archivoDisco);
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo de disco imposible de abrir \n");
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Disco no existe, no se puede montar particion \n" );
    }
    return 0;
}
int ejecutarMount(char path[], char name[]){
    if(existsFile(path) == 1 && verificarExtension(path,".disk") == 0){
        //Archivo existe y es .disk
        int posicion_disco = -1;
        int existe_disco = -1;
        //Recorrer discos por path
        for ( int i = 0 ; i < 26 ; i++ ) {
            if(strcmp(discosMontados[i].path,path)==0){
                //Encontro disco
                existe_disco = 1;
                posicion_disco = i;
                break;
            }
            if(discosMontados[i].letra == -1){
                //No existe disco en posicion i
                posicion_disco = i;
                break;
            }
        }
        //Verificar existencia de disco
        if(existe_disco == 1){
            //disco con 'path' ya existe
            FILE *archivoDisco;
            archivoDisco=fopen(path,"rb+");
            if(archivoDisco!=NULL){
                //Exito al abrir archivo de disco
                //Buscar espacio de mount vacio
                int posicion_mount = -1;
                int existe_mount = -1;
                for (int i = 0 ; i < 10 ; i++ ) {
                    if(strcmp(discosMontados[posicion_disco].particiones_disco[i].mount_name,name)==0){
                        //Existe particion montada
                        existe_mount = 1;
                        break;
                    }
                    if( discosMontados[posicion_disco].particiones_disco[i].mount_start == -1 ){
                        //Espacio disponible para hacer mount
                        posicion_mount = i;
                        break;
                    }
                }
                //Abrir MBR de disco
                MBR aux_mbr;
                fread(&aux_mbr, sizeof(aux_mbr), 1 , archivoDisco);
                MBRPARTITION aux_partition;
                //Buscar particion entre las primarias
                int encontro_particion = -1;
                int encontro_extendida = -1;
                for ( int i = 0 ; i < 4 ; i++ ) {
                    if(aux_mbr.mbr_partitions[i].partition_start != -1){
                        //Particion no vacia
                        if(strcmp(aux_mbr.mbr_partitions[i].partition_name, name)==0){
                            aux_partition = aux_mbr.mbr_partitions[i];
                            encontro_particion = i;
                            break;
                        }
                        if(aux_mbr.mbr_partitions[i].partition_type == 'e'){
                            encontro_extendida = i;
                        }
                    }
                }
                if(existe_mount != 1){
                    //No existe mount
                    if(encontro_particion != -1){
                        //Encontro particion para montar
                        if(aux_partition.partition_type == 'p'){
                            //Elegible para montar
                            if(posicion_mount < 10){
                                //Es posible montar particion
                                //Crear particion montada
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_start = aux_partition.partition_start;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_size = aux_partition.partition_size;

                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_status = aux_partition.partition_status;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_type = aux_partition.partition_type;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_fit = aux_partition.partition_fit;

                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[0]='v';
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[1]='d';
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[2]=(char)discosMontados[posicion_disco].letra;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[3]=(char)48+posicion_mount;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[4]='\0';

                                strcat(discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_name,aux_partition.partition_name);

                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se monto particion '%s' de disco '%s' \n",name,path);

                                actualizarSP(discosMontados[posicion_disco].particiones_disco[posicion_mount],1,archivoDisco);
                            }else{
                                //esta opcion es inalcanzable, pero es un seguro por si algo sale mal
                                //No es posible montar particion
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: limite de particion 'vd%c' alcanzada, particion '%s' imposible de montar \n",(char)(97+posicion_disco),name);
                            }
                        }else{
                            //Particion extendida
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' es extendida en disco '%s' \n",name,path);
                        }
                    }else{
                        //Error de existencia de particion como primaria
                        if(encontro_extendida != -1){
                            //Existe extendida
                            //Busqueda entre las logicas
                            EBR aux_ebr;
                            int encontro_logica = -1;
                            fseek(archivoDisco,aux_mbr.mbr_partitions[encontro_extendida].partition_start,SEEK_SET);
                            fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                            if(strcmp(aux_ebr.ebr_name,name)==0){
                                encontro_logica = 1;
                            }
                            while(aux_ebr.ebr_next != -1){
                                if(strcmp(aux_ebr.ebr_name,name)==0){
                                    encontro_logica = 1;
                                    break;
                                }
                                fseek(archivoDisco,aux_ebr.ebr_next,SEEK_SET);
                                fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                            }
                            if(encontro_logica == 1){
                                //Se encontro particion como logica
                                //Crear particion montada
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_start = aux_ebr.ebr_start + sizeof (EBR);
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_size = aux_ebr.ebr_size - sizeof (EBR);

                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_status = aux_ebr.ebr_status;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_type = 'l';
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_fit = aux_ebr.ebr_fit;

                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[0]='v';
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[1]='d';
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[2]=(char)discosMontados[posicion_disco].letra;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[3]=(char)48+posicion_mount;
                                discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[4]='\0';

                                strcat(discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_name,aux_ebr.ebr_name);

                                printf("\033[1;105m COMANDO REALIZADO \033[0m Se monto particion '%s' de disco '%s' \n",name,path);
                                actualizarSP(discosMontados[posicion_disco].particiones_disco[posicion_mount],1,archivoDisco);
                            }else{
                                //No se encontro particion
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' no existe en disco '%s' \n",name,path);
                            }
                        }else{
                            //no existe extendida
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' no existe en disco '%s' \n",name,path);
                        }
                    }
                }else{
                    //Ya existe mount
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion '%s' ya montada en sistema \n ",name);
                }
                fclose(archivoDisco);
            }else{
                //Error al abrir disco
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo de disco '%s' no se pudo abrir \n",path);
            }
        }else{
            //disco con 'path' no existe
            FILE *archivoDisco;
            archivoDisco=fopen(path,"rb+");
            if(archivoDisco!=NULL){
                //Exito al abrir archivo de disco
                //Buscar espacio de mount vacio
                int posicion_mount = -1;
                for (int i = 0 ; i < 10 ; i++ ) {
                    if( discosMontados[posicion_disco].particiones_disco[i].mount_start == -1 ){
                        posicion_mount = i;
                        break;
                    }
                }
                //Abrir MBR de disco
                MBR aux_mbr;
                fread(&aux_mbr, sizeof(aux_mbr), 1 , archivoDisco);
                MBRPARTITION aux_partition;
                //Buscar particion entre las primarias
                int encontro_particion = -1;
                int encontro_extendida = -1;
                for ( int i = 0 ; i < 4 ; i++ ) {
                    if(aux_mbr.mbr_partitions[i].partition_start != -1){
                        //Particion no vacia
                        if(strcmp(aux_mbr.mbr_partitions[i].partition_name, name)==0){
                            aux_partition = aux_mbr.mbr_partitions[i];
                            encontro_particion = i;
                            break;
                        }
                        if(aux_mbr.mbr_partitions[i].partition_type == 'e'){
                            encontro_extendida = i;
                        }
                    }
                }
                if(encontro_particion != -1){
                    //Encontro particion para montar
                    if(aux_partition.partition_type == 'p'){
                        //Elegible para montar
                        if(posicion_mount < 10){
                            //Es posible montar particion
                            //Crear disco
                            discosMontados[posicion_disco].letra = 97 + posicion_disco;
                            strcpy(discosMontados[posicion_disco].path,path);
                            //Crear particion montada
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_start = aux_partition.partition_start;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_size = aux_partition.partition_size;

                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_status = aux_partition.partition_status;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_type = aux_partition.partition_type;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_fit = aux_partition.partition_fit;

                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[0]='v';
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[1]='d';
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[2]=(char)discosMontados[posicion_disco].letra;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[3]=(char)48+posicion_mount;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[4]='\0';

                            strcat(discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_name,aux_partition.partition_name);

                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se monto particion '%s' de disco '%s' \n",name,path);
                            actualizarSP(discosMontados[posicion_disco].particiones_disco[posicion_mount],1,archivoDisco);
                        }else{
                            //esta opcion es inalcanzable, pero es un seguro por si algo sale mal
                            //No es posible montar particion
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: limite de particion 'vd%c' alcanzada, particion '%s' imposible de montar \n",(char)(97+posicion_disco),name);
                        }
                    }else{
                        //Particion extendida
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' es extendida en disco '%s' \n",name,path);
                    }
                }else{
                    //Error de existencia de particion como primaria
                    if(encontro_extendida != -1){
                        //Existe extendida
                        //Busqueda entre las logicas
                        EBR aux_ebr;
                        int encontro_logica = -1;
                        fseek(archivoDisco,aux_mbr.mbr_partitions[encontro_extendida].partition_start,SEEK_SET);
                        fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                        if(strcmp(aux_ebr.ebr_name,name)==0){
                            encontro_logica = 1;
                        }
                        while(aux_ebr.ebr_next != -1){
                            if(strcmp(aux_ebr.ebr_name,name)==0){
                                encontro_logica = 1;
                                break;
                            }
                            fseek(archivoDisco,aux_ebr.ebr_next,SEEK_SET);
                            fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                        }
                        if(encontro_logica == 1){
                            //Se encontro particion como logica
                            //Crear disco
                            discosMontados[posicion_disco].letra = 97 + posicion_disco;
                            strcpy(discosMontados[posicion_disco].path,path);
                            //Crear particion montada
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_start = aux_ebr.ebr_start + sizeof (EBR);
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_size = aux_ebr.ebr_size - sizeof (EBR);

                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_status = aux_ebr.ebr_status;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_type = 'l';
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_fit = aux_ebr.ebr_fit;

                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[0]='v';
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[1]='d';
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[2]=(char)discosMontados[posicion_disco].letra;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[3]=(char)48+posicion_mount;
                            discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_id[4]='\0';

                            strcat(discosMontados[posicion_disco].particiones_disco[posicion_mount].mount_name,aux_ebr.ebr_name);
                            //48 = 0 57 = 9
                            printf("\033[1;105m COMANDO REALIZADO \033[0m Se monto particion '%s' de disco '%s' \n",name,path);
                            actualizarSP(discosMontados[posicion_disco].particiones_disco[posicion_mount],1,archivoDisco);
                        }else{
                            //No se encontro particion
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' no existe en disco '%s' \n",name,path);
                        }
                    }else{
                        //no existe extendida
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion a montar '%s' no existe en disco '%s' \n",name,path);
                    }
                }
                fclose(archivoDisco);
            }else{
                //Error al abrir disco
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Archivo de disco '%s' no se pudo abrir \n",path);
            }
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo de disco no existe \n");
    }
    return 0;
}
int ejecutarUnmount(char id[]){
    //buscar particion montada
    int elimino_mount = -1;
    int existe_mount = -1;
    for ( int i = 0 ; i < 26 ; i++ ) {
        //Recorrer cada disco montado
        if(discosMontados[i].letra != -1){
            //Existe disco en esa posicion
            int cantidad_mounts = 0;
            for ( int j = 0 ; j < 10 ; j++ ) {
                //Recorrer particiones del disco
                if(strcmp(discosMontados[i].particiones_disco[j].mount_id,id)==0){
                    existe_mount = 1;
                    FILE *archivoDiscoTemporal;
                    archivoDiscoTemporal = fopen(discosMontados[i].path,"rb+");
                    actualizarSP(discosMontados[i].particiones_disco[j],2,archivoDiscoTemporal);
                    fclose(archivoDiscoTemporal);

                    memset(&discosMontados[i].particiones_disco[j].mount_name,0,16);
                    memset(&discosMontados[i].particiones_disco[j].mount_id,0,5);

                    discosMontados[i].particiones_disco[j].mount_status = 'n';
                    discosMontados[i].particiones_disco[j].mount_type = 'p';
                    discosMontados[i].particiones_disco[j].mount_fit = 'f';

                    discosMontados[i].particiones_disco[j].mount_start = -1;
                    discosMontados[i].particiones_disco[j].mount_size = -1;

                    elimino_mount = 1;
                    printf("\033[1;105m COMANDO REALIZADO \033[0m Se desmonto particion con id '%s' \n",id);
                    break;
                }
            }
            for ( int j = 0 ; j < 10 ; j++ ) {
                if(discosMontados[i].particiones_disco[j].mount_start != -1){
                    cantidad_mounts++;
                }
            }
            if(cantidad_mounts == 0){
                //Ya no tiene particiones montadas
                memset(&discosMontados[i].path,0,300);
                discosMontados[i].letra = -1;
            }
            if(elimino_mount == 1){
                break;
            }
        }
    }
    if(existe_mount == -1 && elimino_mount == -1){
        //No se desmonto particion
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: particion con id '%s' no esta montada \n",id);
    }
    //Ordenar discos restantes
    MONTURADISCO aux_orden;
    for (int i = 1;i<26;i++) {
        for ( int j = 0; j < 26 - i; j++) {
            if( ((discosMontados[j].letra > discosMontados[j+1].letra) && discosMontados[j+1].letra != -1) || (discosMontados[j].letra == -1) ){
                aux_orden = discosMontados[j+1];
                discosMontados[j+1] = discosMontados[j];
                discosMontados[j] = aux_orden;
            }
        }
    }
    //reestablecer letras
    for (int i = 0;i<26;i++) {
        //Recorrer cada disco montado
        if(discosMontados[i].letra != -1){
            //existe disco montado
            discosMontados[i].letra = 97 + i;
            for (int j = 0;j<10;j++) {
                if(discosMontados[i].particiones_disco[j].mount_start != -1){
                    //existe particion montada
                    memset(&discosMontados[i].particiones_disco[j].mount_id,0,5);

                    discosMontados[i].particiones_disco[j].mount_id[0]='v';
                    discosMontados[i].particiones_disco[j].mount_id[1]='d';
                    discosMontados[i].particiones_disco[j].mount_id[2]=(char)discosMontados[i].letra;
                    discosMontados[i].particiones_disco[j].mount_id[3]=(char)48+j;
                    discosMontados[i].particiones_disco[j].mount_id[4]='\0';
                }
            }
        }
    }
    //Ordenar mounts restantes
    PMOUNT aux_orden_mount;
    for (int i = 0;i<26;i++) {
        //Recorrer cada disco montado
        if(discosMontados[i].letra != -1){
            //existe disco
            for ( int x = 1; x < 10 ;x++) {
                for( int y = 0; y < 10 - x; y++){
                    if( discosMontados[i].particiones_disco[y].mount_start == -1 ){
                        aux_orden_mount = discosMontados[i].particiones_disco[y+1];
                        discosMontados[i].particiones_disco[y+1] = discosMontados[i].particiones_disco[y];
                        discosMontados[i].particiones_disco[y] = aux_orden_mount;
                    }
                }
            }
        }
    }
    //reestablecer numerales
    for (int i = 0 ;i < 26 ;i++ ) {
        //recorrer cada disco montado
        if(discosMontados[i].letra != -1){
            for (int j=0;j < 10;j++) {
                //recorrer cada particion montada
                if(discosMontados[i].particiones_disco[j].mount_start != -1){
                    memset(&discosMontados[i].particiones_disco[j].mount_id,0,5);

                    discosMontados[i].particiones_disco[j].mount_id[0]='v';
                    discosMontados[i].particiones_disco[j].mount_id[1]='d';
                    discosMontados[i].particiones_disco[j].mount_id[2]=(char)discosMontados[i].letra;
                    discosMontados[i].particiones_disco[j].mount_id[3]=(char)48+j;
                    discosMontados[i].particiones_disco[j].mount_id[4]='\0';
                }
            }
        }
    }
    return 0;
}
int generarMBR(char pathSalida[], char pathDisco[]){
    if(existsFile(pathSalida)==1){
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            char insert[999] = {0};
            int cantidadEBR = 0;
            MBR aux_mbr;
            fread(&aux_mbr,sizeof(aux_mbr),1,archivoDisco);
            sprintf(insert,
             "digraph D {\nnode[shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table color=\'skyblue1\' border=\'0\' cellborder=\'1\' cellspacing=\'0\'  width=\'300\' height=\'160\'  ><tr><td colspan=\"3\">\"%s\"</td></tr>\n",pathDisco);
            fputs(insert,archivoReporte);
            sprintf(insert,"<tr><td colspan=\"2\">ID Disco</td><td> %i </td></tr>\n",aux_mbr.mbr_id);
            fputs(insert, archivoReporte);
            sprintf(insert,"<tr><td colspan=\"2\">Fecha Creacion</td><td> %s </td></tr>\n",aux_mbr.mbr_creation_date);
            fputs(insert, archivoReporte);
            sprintf(insert, "<tr><td colspan=\"2\">Size Disco (Bytes)</td><td> %i </td></tr>\n",aux_mbr.mbr_size);
            fputs(insert, archivoReporte);
            sprintf(insert, "<tr><td colspan=\"2\">Fit Disco </td><td> %cf </td></tr>\n",aux_mbr.mbr_fit);
            fputs(insert, archivoReporte);
            //Recorrido de particiones
            for (int i = 0;i<4;i++) {
                if(aux_mbr.mbr_partitions[i].partition_start == -1){

                    break;
                }
                sprintf(insert, "<tr><td colspan=\"3\">Particion  %i</td></tr>\n", i+1);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Nombre Particion</td><td> %s </td></tr>\n",aux_mbr.mbr_partitions[i].partition_name);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Inicio (Bytes En Disco)</td><td> %i </td></tr>\n",aux_mbr.mbr_partitions[i].partition_start);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Size (Bytes En Disco)</td><td> %i </td></tr>\n",aux_mbr.mbr_partitions[i].partition_size);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Estado</td><td> %c </td></tr>\n",aux_mbr.mbr_partitions[i].partition_status);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Tipo</td><td> %c </td></tr>\n",aux_mbr.mbr_partitions[i].partition_type);
                fputs(insert, archivoReporte);
                sprintf(insert,"<tr><td colspan=\"2\">Fit</td><td> %c </td></tr>\n",aux_mbr.mbr_partitions[i].partition_fit);
                fputs(insert, archivoReporte);
                if(aux_mbr.mbr_partitions[i].partition_type == 'e' && aux_mbr.mbr_partitions[i].partition_start != -1 ){
                    //Existe particion Extendida
                    EBR aux_EBR;
                    fseek(archivoDisco, aux_mbr.mbr_partitions[i].partition_start, SEEK_SET);
                    fread(&aux_EBR,sizeof(aux_EBR),1,archivoDisco);
                    if(aux_EBR.ebr_start != -1 || aux_EBR.ebr_next != -1){
                        sprintf(insert,"<tr><td></td><td colspan=\"2\">EBR #%i</td></tr>\n", cantidadEBR+1);
                        cantidadEBR++;
                        fputs(insert, archivoReporte);
                        sprintf(insert,"<tr><td width=\"20%\"></td><td>Nombre Particion</td><td> %s</td></tr>\n" , aux_EBR.ebr_name);
                        fputs(insert, archivoReporte);
                        if( !(aux_EBR.ebr_status == 'n' || aux_EBR.ebr_status == 'a') ){
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Estado</td><td> %c</td></tr>\n", 'X');
                        }else{
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Estado</td><td> %c</td></tr>\n", aux_EBR.ebr_status);
                        }
                        fputs(insert, archivoReporte);
                        if( !(aux_EBR.ebr_fit == 'f' || aux_EBR.ebr_fit == 'b' || aux_EBR.ebr_fit == 'w') ){
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Fit</td><td> %c </td></tr>\n", 'X');
                        }else{
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Fit</td><td> %c </td></tr>\n", aux_EBR.ebr_fit);
                        }
                        fputs(insert, archivoReporte);
                        sprintf(insert,"<tr><td width=\"20%\"></td><td>Inicio (Bytes En Disco)</td><td> %i</td></tr>\n" , aux_EBR.ebr_start);
                        fputs(insert, archivoReporte);
                        sprintf(insert,"<tr><td width=\"20%\"></td><td>Size (Bytes En Disco)</td><td> %i</td></tr>\n" , aux_EBR.ebr_size);
                        fputs(insert, archivoReporte);
                        sprintf(insert,"<tr><td width=\"20%\"></td><td>Siguiente (Pisicion en Bytes)</td><td> %i</td></tr>\n" , aux_EBR.ebr_next);
                        fputs(insert, archivoReporte);
                        while(aux_EBR.ebr_next != -1 && aux_EBR.ebr_start != 0)
                        {
                            //Recorrer todos los EBR
                            fseek(archivoDisco, aux_EBR.ebr_next, SEEK_SET);
                            fread(&aux_EBR,sizeof(aux_EBR),1,archivoDisco);
                            sprintf(insert,"<tr><td></td><td colspan=\"2\">EBR #%i</td></tr>\n", cantidadEBR+1);
                            cantidadEBR++;
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Nombre Particion</td><td> %s</td></tr>\n" , aux_EBR.ebr_name);
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Estado</td><td> %c</td></tr>\n", aux_EBR.ebr_status);
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Fit</td><td> %c </td></tr>\n", aux_EBR.ebr_fit);
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Inicio (Bytes En Disco)</td><td> %i</td></tr>\n" , aux_EBR.ebr_start);
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Size (Bytes En Disco)</td><td> %i</td></tr>\n" , aux_EBR.ebr_size);
                            fputs(insert, archivoReporte);
                            sprintf(insert,"<tr><td width=\"20%\"></td><td>Siguiente (Pisicion en Bytes)</td><td> %i</td></tr>\n" , aux_EBR.ebr_next);
                            fputs(insert, archivoReporte);
                        }
                    }
                }
            }
            //Si llega hast aqui, todo salio bien
            fclose(archivoDisco);
            exito_reporte = 1;
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo abrir '%s' revise disco \n",pathDisco);
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo abrir archivo de reporte \n");
        return -1;
    }
    if(exito_reporte == 1){
        //Fin de archivo
        fputs("</table> >  ];}", archivoReporte);
    }
    fclose(archivoReporte);
    return exito_reporte;
}
int generarDISK(char pathSalida[], char pathDisco[]){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    int cantidadEBR = 0;
    char ultimoInsert[999] = {0};
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        char insert[1024] = {0};
        MBR aux_mbr;
        FILE *archivoDisco;
        archivoDisco=fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Iniciar lectura de archivo de disco
            fread (&aux_mbr, sizeof (aux_mbr), 1, archivoDisco);
            fseek(archivoDisco , 0 , SEEK_SET);
            //Variables de apoyo para reporte
            float sizeMbr = aux_mbr.mbr_size / 100;
            int inicioDisco = 140;
            int auxPos = 0;
            sprintf(insert, "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table><tr><td rowspan=\"2\" BGCOLOR=\'Pink\'>MBR<br/>Nombre: %s<br/>Creacion: %s<br/>Size: %i</td>\n" ,pathDisco,aux_mbr.mbr_creation_date,aux_mbr.mbr_size);
            fputs(insert, archivoReporte);
            for(int i = 0; i < 4; i++){
                auxPos = i;
                if(aux_mbr.mbr_partitions[i].partition_start == -1){
                    //Particion en posicion i no existe
                    break;
                }
                if(aux_mbr.mbr_partitions[i].partition_start -  inicioDisco > 0){
                    //Revisar por espacio libre entre MBR y Primera particion
                    sprintf(insert,"<td rowspan=\"2\" BGCOLOR=\'cyan\'>Libre<br/>%4.2f%</td>\n",(aux_mbr.mbr_partitions[i].partition_start -  inicioDisco)/ sizeMbr);
                    fputs(insert, archivoReporte);
                }
                int sumaTotal = 0;
                if(aux_mbr.mbr_partitions[i].partition_type == 'e' && aux_mbr.mbr_partitions[i].partition_start != -1 ){
                    //Recorrido si particion es extendida
                    EBR aux_ebr;
                    fseek(archivoDisco, aux_mbr.mbr_partitions[i].partition_start, SEEK_SET);
                    fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                    if(aux_ebr.ebr_start == -1){
                        //Espacio libre al inicio
                        sprintf(ultimoInsert,"%s<td BGCOLOR=\'blue\' >EBR</td><td BGCOLOR=\'cyan\'>Libre<br/>%4.2f%</td>\n",ultimoInsert,(aux_ebr.ebr_size / sizeMbr));
                    }else{
                        //Primera particion
                        sprintf(ultimoInsert,"%s<td BGCOLOR=\'blue\' >EBR</td><td BGCOLOR=\'orange\' >Logica<br/>Nombre: %s<br/>%4.2f%</td>\n",ultimoInsert,aux_ebr.ebr_name,(aux_ebr.ebr_size / sizeMbr));
                    }
                    if(aux_ebr.ebr_next - aux_ebr.ebr_start - aux_ebr.ebr_size > 0){
                        //Espacio libre entre primera logica y segunda
                        sumaTotal++;
                        sprintf(ultimoInsert,"%s<td BGCOLOR=\'cyan\' >Libre<br/>%4.2f%</td>\n",ultimoInsert, ((aux_ebr.ebr_next - aux_ebr.ebr_start - aux_ebr.ebr_size)/ sizeMbr));
                    }
                    cantidadEBR++;
                    while(aux_ebr.ebr_next != -1 && aux_ebr.ebr_start != 0){
                        //Recorrer todas las logicas
                        fseek(archivoDisco, aux_ebr.ebr_next, SEEK_SET);
                        fread(&aux_ebr,sizeof(aux_ebr),1,archivoDisco);
                        sprintf(ultimoInsert,"%s<td BGCOLOR=\'blue\' >EBR</td><td BGCOLOR=\'orange\' >Logica<br/>Nombre: %s<br/>%4.2f%</td>\n",ultimoInsert,aux_ebr.ebr_name,(aux_ebr.ebr_size / sizeMbr));
                        cantidadEBR++;
                        if(aux_ebr.ebr_next - aux_ebr.ebr_start - aux_ebr.ebr_size > 0){
                            //Espacio libre entre dos particiones logicas
                            sumaTotal++;
                            sprintf(ultimoInsert,"%s<td BGCOLOR=\'blue\' >Libre<br/>%4.2f%</td>\n",ultimoInsert, ((aux_ebr.ebr_next - aux_ebr.ebr_start - aux_ebr.ebr_size)/ sizeMbr));
                        }
                    }
                    float posfinal = aux_mbr.mbr_partitions[i].partition_start + aux_mbr.mbr_partitions[i].partition_size - aux_ebr.ebr_start - aux_ebr.ebr_size;
                    if(posfinal > 0){
                        //Termina de imprimir particion extendida (con espacio libre)
                        sprintf(ultimoInsert,"%s<td BGCOLOR=\'blue\'  >Libre<br/>%4.2f%</td>  \n",ultimoInsert,(posfinal/sizeMbr));
                        sprintf(insert,"<td colspan=\"%d\">Extendida<br/>Nombre: %s<br/>%4.2f%</td> \n",cantidadEBR*2+1 + sumaTotal,aux_mbr.mbr_partitions[i].partition_name,aux_mbr.mbr_partitions[i].partition_size/sizeMbr);
                    }else{
                        //Termina de imprimir particion extendida (sin espacio libre)
                        sprintf(insert,"<td colspan=\"%d\" BGCOLOR=\'yellow\' >Extendida<br/>Nombre: %s<br/>%4.2f%</td> \n",cantidadEBR*2 + sumaTotal,aux_mbr.mbr_partitions[i].partition_name,aux_mbr.mbr_partitions[i].partition_size/sizeMbr);
                    }
                    fputs(insert, archivoReporte);
                }
                else{
                    //Particion primaria
                    sprintf(insert,"<td rowspan=\"2\" BGCOLOR=\'green\' >Primaria<br/>Nombre: %s<br/>%4.2f%</td>  \n" ,aux_mbr.mbr_partitions[i].partition_name,(aux_mbr.mbr_partitions[i].partition_size/ sizeMbr));
                    fputs(insert, archivoReporte);
                }
                inicioDisco = aux_mbr.mbr_partitions[i].partition_start + aux_mbr.mbr_partitions[i].partition_size;
            }
            if(auxPos > 0){
                //Espacio libre al final (al menos una particion)
                if(auxPos == 3){
                    //Recorrio hasta el final (max particiones)
                           float tamanolibre = aux_mbr.mbr_size- (
                            aux_mbr.mbr_partitions[0].partition_size+
                            aux_mbr.mbr_partitions[1].partition_size+
                            aux_mbr.mbr_partitions[2].partition_size);
                            if(tamanolibre > 140){
                            sprintf(insert,"<td rowspan=\"2\" BGCOLOR=\'cyan\' >Libre<br/>%4.2f%</td>\n",(tamanolibre/sizeMbr));
                        fputs(insert, archivoReporte);
                        }
                }else{
                    //Recorrido parcial (No estaba lleno)
                    float posfinal = aux_mbr.mbr_size - aux_mbr.mbr_partitions[auxPos-1].partition_start - aux_mbr.mbr_partitions[auxPos-1].partition_size;
                    if(posfinal > 140){
                        sprintf(insert,"<td rowspan=\"2\" BGCOLOR=\'cyan\'>Libre<br/>%4.2f%</td>\n",(posfinal/sizeMbr));
                        fputs(insert, archivoReporte);
                    }
                }
            }else{
                //Espacio libre al final (sin particiones)
                sprintf(insert,"<td rowspan=\"2\" BGCOLOR=\'cyan\'>Libre<br/>%4.2f%</td>\n",((aux_mbr.mbr_size - 140 ) / sizeMbr));
            }
            //Si llego hasta aqui, todo salio bien
            fclose(archivoDisco);
            exito_reporte = 1;
        }
        else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo abrir '%s' revise disco \n",pathDisco);
            return false;
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo abrir archivo de reporte \n");
        return -1;
    }
    if(cantidadEBR != 0 && exito_reporte == 1){
        //Fin de tablas EBR
        fputs("</tr><tr>",archivoReporte);
        fputs(ultimoInsert, archivoReporte);
    }
    if(exito_reporte == 1){
        //Fin de archivo
        fputs("</tr></table> >  ];}", archivoReporte);
    }
    fclose(archivoReporte);
    return exito_reporte;
}
int generarINODE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                inode inodoM;
                inodoM = initiNode(inodoM);
                char bmBlock[270000] = {0};
                char auxRecorrido = '\0';
                char insert[2048] = {0};
                int maxBlock = sp_aux.sb_inodes_count;
/*
                sprintf(insert, "digraph D {\n rankdir=LR;\n");
                sprintf(insert, "Journal%i[shape=record,color=red,style=filled, fillcolor=Cyan, label=\"{{ Title:Journal_%i|Usuario: %s|Fecha_Creacion: %s|Operacion: %s|Contenido 1: %s| Contenido 2: %s}}\"];\n",
                        i,i,user,entrada.j_date,operacion,entrada.j_name,entrada.j_content);
                */


                fputs("Digraph D{\n rankdir=LR;\n",archivoReporte);
                fseek(archivoDisco,sp_aux.sb_bm_inode,SEEK_SET);
                for(int i = 0 ; i < maxBlock ; i ++){
                    fread(&auxRecorrido,1,1,archivoDisco);
                    bmBlock[i] = auxRecorrido;
                }
                for(int i = 0 ; i < strlen(bmBlock) ; i++){
                    if(bmBlock[i] == '0'){
                        break;
                    }
                    fseek(archivoDisco,getiNodeStart_2(i,sp_aux.sb_t_inode),SEEK_SET);
                    fread(&inodoM,sizeof(inode),1,archivoDisco);

                    char tipo[12] = {0};
                    if(inodoM.i_type == '1'){
                        //Archivo
                        strcpy(tipo,"Archivo");
                    }else{
                        //Carpeta
                        strcpy(tipo,"Carpeta");
                    }
                    sprintf(insert,"iNode%i[shape=record,color=cyan,style=filled,fillcolor=Orange,   label=\"{{INode_%i|Owner: %i|Group: %i|Permisos: %i|Size: %i|Tipo: %s|Creacion: %s|Lectura: %s|Modificacion: %s"
                            ,i,i,inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,inodoM.i_size,tipo,inodoM.i_ctime,inodoM.i_atime,inodoM.i_mtime);
                    for (int j = 0 ; j < 15; j++) {
                        if(inodoM.i_block[j] != -1){
                            sprintf(insert,"%s|iBlock%i: %i",insert,j,inodoM.i_block[j]);
                        }else{
                            sprintf(insert,"%s|iBlock%i: -1",insert,j);
                        }
                    }
                    sprintf(insert,"%s}}\"];\n",insert);
                    fputs(insert,archivoReporte);
                    memset(&insert,0,2048);
                }
                fputs("\n}",archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);

            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarBLOCK(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo, archivoDisco);
            if(sp_aux.sb_magic == 61267){
                char bmBlock[270000] = {0};
                char auxRecorrido = '\0';
                char insert[2048] = {0};
                int maxBlock = sp_aux.sb_blocks_count;

                fputs("Digraph D{\nrankdir=LR;\n",archivoReporte);
                fseek(archivoDisco,sp_aux.sb_bm_Block,SEEK_SET);
                for(int i = 0 ; i < maxBlock ; i ++){
                    fread(&auxRecorrido,1,1,archivoDisco);
                    bmBlock[i] = auxRecorrido;
                }
                for(int i = 0 ; i < strlen(bmBlock) ; i++){
                    if(bmBlock[i] == '0'){
                        break;
                    }
                    if(bmBlock[i] == '1'){
                        //Bloque Archivo/Carpeta
                        contentBlock tempContent;
                        fseek(archivoDisco,getBlockStart_2(i,sp_aux.sb_t_block),SEEK_SET);
                        fread(&tempContent,sizeof (contentBlock),1,archivoDisco);
                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",i,i);
                        for(int a = 0 ; a < 4 ; a++){
                            if(tempContent.contenapunta[a].f_inode != -1){
                                sprintf(insert,"%s|%s: %i",insert,tempContent.contenapunta[a].f_name,tempContent.contenapunta[a].f_inode);
                            }else{
                                sprintf(insert,"%s|NULL",insert);
                            }
                        }
                        sprintf(insert,"%s}}\"];\n",insert);
                        fputs(insert,archivoReporte);
                    }
                    if(bmBlock[i] == '2'){
                        //Bloque Contenido
                        fileBlock tempFile;
                        fseek(archivoDisco,getBlockStart_2(i,sp_aux.sb_t_block),SEEK_SET);
                        fread(&tempFile,sizeof (fileBlock),1,archivoDisco);
                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i|Content: %s}}\"];\n",i,i,tempFile.fcontenapunta);
                        fputs(insert,archivoReporte);
                    }
                    if(bmBlock[i] == '3'){
                        //Bloque Apuntadores
                        pointerBlock tempPointer;
                        fseek(archivoDisco,getBlockStart_2(i,sp_aux.sb_t_block),SEEK_SET);
                        fread(&tempPointer,sizeof (pointerBlock),1,archivoDisco);
                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",i,i);
                        for(int a = 0 ; a < 16 ; a++){
                            if(tempPointer.pb_pointers[a] != -1){
                                sprintf(insert,"%s|Pointer%i: %i",insert,a,tempPointer.pb_pointers[a]);
                            }else{
                                sprintf(insert,"%s|Pointer%i: NULL",insert,a);
                            }
                        }
                        sprintf(insert,"%s}}\"];\n",insert);
                        fputs(insert,archivoReporte);
                    }
                    memset(&insert,0,2048);
                }
                fputs("\n}",archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);

            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarJOURNALING(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                char insert[2048] ={0};

                sprintf(insert, "digraph D {\n rankdir=LR;\n");

              //  sprintf(insert,"\ntbl_%d[shape=box, label=<\n ",1);
              //  sprintf(insert, "<table color=\'green\' border=\'0\' cellborder=\'1\' cellspacing=\'0\'  width=\'300\' height=\'160\' >\n ");

                fputs(insert,archivoReporte);
                int n = 1 + ((particionTrabajo.mount_size - sizeof(SuperBloque) - 1)/(sizeof(journal) + sizeof(inode) + 3 * sizeof (contentBlock) + 4));

                journal entrada;
                entrada = initJournal(entrada);
                fseek(archivoDisco,particionTrabajo.mount_start+sizeof (SuperBloque),SEEK_SET);
                for(int i = 0 ; i < n ; i ++){
                    fread(&entrada,sizeof(journal),1,archivoDisco);

                    if(entrada.j_uid != -1){char *tempPerm = getPermisos(entrada.j_perm);
                        char tempPerm2[12] = {0};
                        strcpy(tempPerm2,tempPerm);
                        free(tempPerm);

                        char user[12] = {0};
                        for(int i = 0; i < globNusers ; i++){
                            if(globusers[i].numGrupo == entrada.j_uid){
                                for(int j = 0 ; j < 12 ; j++){
                                    user[j] = globusers[i].name[j];

                                }
                                break;
                            }
                        }

                        char operacion[25] = {0};
                        if(entrada.j_op_type == '0'){
                            strcpy(operacion,"Mkgrp");
                        }else if(entrada.j_op_type == '1'){
                            strcpy(operacion,"Rmgrp");
                        }else if(entrada.j_op_type == '2'){
                            strcpy(operacion,"Mkusr");
                        }else if(entrada.j_op_type == '3'){
                            strcpy(operacion,"Rmusr");
                        }else if(entrada.j_op_type == '4'){
                            strcpy(operacion,"Chgrp");
                        }else if(entrada.j_op_type == 'a'){
                            strcpy(operacion,"Mkfile");
                        }else if(entrada.j_op_type == 'b'){
                            strcpy(operacion,"Rem");
                        }else if(entrada.j_op_type == 'c'){
                            strcpy(operacion,"Edit");
                        }else if(entrada.j_op_type == 'd'){
                            strcpy(operacion,"Ren");
                        }else if(entrada.j_op_type == 'e'){
                            strcpy(operacion,"Mkdir");
                        }else if(entrada.j_op_type == 'f'){
                            strcpy(operacion,"Cp");
                        }else if(entrada.j_op_type == 'g'){
                            strcpy(operacion,"Mv");
                        }else if(entrada.j_op_type == 'h'){
                            strcpy(operacion,"Chown");
                        }else if(entrada.j_op_type == 'z'){
                            strcpy(operacion,"Chmod");
                        }
//                        sprintf(insert, "digraph D {\n rankdir=LR;\n");

                        sprintf(insert, "Journal%i[shape=record,color=red,style=filled, fillcolor=Cyan, label=\"{{ Title:Journal_%i|Usuario: %s|Fecha_Creacion: %s|Operacion: %s|Contenido 1: %s| Contenido 2: %s}}\"];\n",
                                i,i,user,entrada.j_date,operacion,entrada.j_name,entrada.j_content);

      fputs(insert,archivoReporte);
                    }else{
                        break;
                    }
                } //for
                fputs("\n}", archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);

            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarBM_INODO(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo, archivoDisco);
            if(sp_aux.sb_magic == 61267){
                char auxInsert = '\0';
                char insert[2048] ={0};
                char auxInsertText[2048] ={0};
                int max = 0;
                int maxBlock = sp_aux.sb_inodes_count;
                sprintf(insert, "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table><tr><td>BitMap iNode</td></tr>\n");
                fputs(insert,archivoReporte);
                fseek(archivoDisco,sp_aux.sb_bm_inode,SEEK_SET);
                memset(&insert,0,2048);
                for (int i = 0 ; i < maxBlock; i++) {
                    fread(&auxInsert,1,1,archivoDisco);
                    if(auxInsert == '1' || auxInsert == '2' || auxInsert == '3'){
                        sprintf(insert,"%s - %c",insert,'1');
                    }else{
                        sprintf(insert,"%s - %c",insert,'0');
                    }
                    max++;
                    if(max == 20){
                        sprintf(auxInsertText, "<tr><td> %s - </td></tr>\n",insert);
                        fputs(auxInsertText,archivoReporte);
                        memset(&insert,0,2048);
                        max = 0;
                    }
                }
                fputs("\n</table> >  ];}", archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);

            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarBM_BLOQUE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                char auxInsert = '\0';
                char insert[2048] ={0};
                char auxInsertText[2048] ={0};
                int max = 0;
                int maxBlock = sp_aux.sb_blocks_count;
                sprintf(insert, "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table><tr><td>BitMap Block</td></tr>\n");
                fputs(insert,archivoReporte);
                fseek(archivoDisco,sp_aux.sb_bm_Block,SEEK_SET);
                memset(&insert,0,2048);
                for (int i = 0 ; i < maxBlock; i++) {
                    fread(&auxInsert,1,1,archivoDisco);
                    if(auxInsert == '1' || auxInsert == '2' || auxInsert == '3'){
                        sprintf(insert,"%s - %c",insert,'1');
                    }else{
                        sprintf(insert,"%s - %c",insert,'0');
                    }
                    max++;
                    if(max == 20){
                        sprintf(auxInsertText, "<tr><td> %s - </td></tr>\n",insert);
                        fputs(auxInsertText,archivoReporte);
                        memset(&insert,0,2048);
                        max = 0;
                    }
                }
                fputs("\n</table> >  ];}", archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarTREEiNode(FILE *archivoReporte, FILE*archivoDisco, char pathDisco[], int padre, int actual, char tipo, SuperBloque sp_aux, PMOUNT particionTrabajo){
    inode inodoM;
    fseek(archivoDisco,getiNodeStart_2(actual,sp_aux.sb_t_inode),SEEK_SET);
    fread(&inodoM,sizeof (inode),1,archivoDisco);
    //Para el contenido
    char insert[2048] = {0};
    contentBlock tempCopia;
    fileBlock tempContent;
    //Generar Primer iNodo
    char tipo2[12] = {0};
    if(inodoM.i_type == '1'){
        //Archivo
        strcpy(tipo2,"Archivo");
    }else{
        //Carpeta
        strcpy(tipo2,"Carpeta");
    }
    sprintf(insert,"iNode%i[shape=record,label=\"{{INode%i|Owner: %i|Group: %i|Permisos: %i|Size: %i|Tipo: %s|Creacion: %s|Lectura: %s|Modificacion: %s",actual,actual,inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,inodoM.i_size,tipo2,inodoM.i_ctime,inodoM.i_atime,inodoM.i_mtime);
    for (int i = 0 ; i < 15; i++) {
        if(inodoM.i_block[i] != -1){
            sprintf(insert,"%s|iBlock%i: %i",insert,i,inodoM.i_block[i]);
        }else{
            sprintf(insert,"%s|iBlock%i: -1",insert,i);
        }
    }
    sprintf(insert,"%s}}\"];\n",insert);
    fputs(insert,archivoReporte);
    sprintf(insert,"Block%i -> iNode%i;\n",padre,actual);
    fputs(insert,archivoReporte);
    if(tipo == '1'){
        //Es un archivo
        //Mientras Tenga Contenido
        for(int i = 0 ; i < 15 ; i++){
            if(inodoM.i_block[i] != -1){
                if(i < 12){
                    //Directos
                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                    //Revisar Contenido
                    fseek(archivoDisco,getBlockStart_2(inodoM.i_block[i],sp_aux.sb_t_block),SEEK_SET);
                    fread(&tempContent,sizeof(tempContent),1,archivoDisco);
                    sprintf(insert,"%s|%s}}\"];\n",insert,tempContent.fcontenapunta);
                    fputs(insert,archivoReporte);
                    sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                    fputs(insert,archivoReporte);
                    //Fin
                }else{
                    if(i == 12){
                        //Indirecto
                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                        pointerBlock pb1;
                        int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                        //Revisar Contenido Indirecto
                        fseek(archivoDisco, offset, SEEK_SET);
                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                        for( int a = 0 ; a < 16 ; a ++){
                            if(pb1.pb_pointers[a] != -1){
                                sprintf(insert,"%s|pBlock%i : %i",insert,a,pb1.pb_pointers[a]);
                            }else{
                                sprintf(insert,"%s|NULL: -1",insert);
                            }
                        }
                        sprintf(insert,"%s}}\"];\n",insert);
                        fputs(insert,archivoReporte);
                        sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                        fputs(insert,archivoReporte);
                        //Revisar contenido por Indirecto
                        for( int a = 0 ; a < 16 ; a ++){
                            if(pb1.pb_pointers[a] != -1){
                                //Bloque
                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb1.pb_pointers[a],pb1.pb_pointers[a]);

                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                fread(&tempContent,sizeof(tempContent),1,archivoDisco);
                                sprintf(insert,"%s|%s}}\"];\n",insert,tempContent.fcontenapunta);
                                fputs(insert,archivoReporte);
                                sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb1.pb_pointers[a]);
                                fputs(insert,archivoReporte);
                            }
                        }
                    }else{
                        if(i == 13){
                            //Doble Indirecto
                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                            pointerBlock pb2;
                            int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                            //Revisar Contenido Doble Indirecto
                            fseek(archivoDisco, offset, SEEK_SET);
                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    sprintf(insert,"%s|pBlock%i : %i",insert,a,pb2.pb_pointers[a]);
                                }else{
                                    sprintf(insert,"%s|NULL: -1",insert);
                                }
                            }
                            sprintf(insert,"%s}}\"];\n",insert);
                            fputs(insert,archivoReporte);
                            sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                            fputs(insert,archivoReporte);
                            //Revisar Contenido Indirecto
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    //Indirecto
                                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2.pb_pointers[a],pb2.pb_pointers[a]);
                                    pointerBlock pb2_1;
                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                    //Revisar contenido por Indirecto
                                    for(int b = 0; b < 16 ; b++){
                                        if(pb2_1.pb_pointers[b] != -1){
                                            sprintf(insert,"%s|pBlock%i : %i",insert,b,pb2_1.pb_pointers[b]);
                                        }else{
                                            sprintf(insert,"%s|NULL: -1",insert);
                                        }
                                    }
                                    sprintf(insert,"%s}}\"];\n",insert);
                                    fputs(insert,archivoReporte);
                                    sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb2.pb_pointers[a]);
                                    fputs(insert,archivoReporte);
                                }
                            }
                            //Revisar contenido por Indirecto
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    //Indirecto
                                    pointerBlock pb2_1;
                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                    //Revisar contenido por Indirecto
                                    for(int b = 0; b < 16 ; b++){
                                        if(pb2_1.pb_pointers[b] != -1){
                                            //Bloque
                                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2_1.pb_pointers[b],pb2_1.pb_pointers[b]);

                                            fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[b],sp_aux.sb_t_block),SEEK_SET);
                                            fread(&tempContent,sizeof(tempContent),1,archivoDisco);
                                            sprintf(insert,"%s|%s}}\"];\n",insert,tempContent.fcontenapunta);
                                            fputs(insert,archivoReporte);
                                            sprintf(insert,"Block%i -> Block%i;\n",pb2.pb_pointers[a],pb2_1.pb_pointers[b]);
                                            fputs(insert,archivoReporte);
                                        }
                                    }
                                }
                            }
                        }else{
                            if(i == 14){
                                //Triple indirecto
                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                                pointerBlock pb3;
                                int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                                //Revisar Contenido Triple Indirecto
                                fseek(archivoDisco, offset, SEEK_SET);
                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        sprintf(insert,"%s|pBlock%i : %i",insert,a,pb3.pb_pointers[a]);
                                    }else{
                                        sprintf(insert,"%s|NULL: -1",insert);
                                    }
                                }
                                sprintf(insert,"%s}}\"];\n",insert);
                                fputs(insert,archivoReporte);
                                sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                                fputs(insert,archivoReporte);
                                //Revisar Contenido Doble Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        //Doble Indirecto
                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3.pb_pointers[a],pb3.pb_pointers[a]);
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            if(pb3_2.pb_pointers[b] != -1){
                                                sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2.pb_pointers[b]);
                                            }else{
                                                sprintf(insert,"%s|NULL: -1",insert);
                                            }
                                        }
                                        sprintf(insert,"%s}}\"];\n",insert);
                                        fputs(insert,archivoReporte);
                                        sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb3.pb_pointers[a]);
                                        fputs(insert,archivoReporte);
                                    }
                                }
                                //Revisar Contenido Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            if(pb3_2.pb_pointers[b] != -1){
                                                //Indirecto
                                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2.pb_pointers[b],pb3_2.pb_pointers[b]);
                                                pointerBlock pb3_2_1;
                                                int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_3,SEEK_SET);
                                                fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                for(int c = 0 ; c < 16 ; c++ ){
                                                    if(pb3_2_1.pb_pointers[c] != -1){
                                                        sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2_1.pb_pointers[c]);
                                                    }else{
                                                        sprintf(insert,"%s|NULL: -1",insert);
                                                    }
                                                }
                                                sprintf(insert,"%s}}\"];\n",insert);
                                                fputs(insert,archivoReporte);
                                                sprintf(insert,"Block%i -> Block%i;\n",pb3.pb_pointers[a],pb3_2.pb_pointers[b]);
                                                fputs(insert,archivoReporte);
                                            }
                                        }
                                    }
                                }
                                //Revisar contenido por Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            pointerBlock pb3_2_1;
                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_3,SEEK_SET);
                                            fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                            for(int c = 0 ; c < 16 ; c++ ){
                                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2_1.pb_pointers[c],pb3_2_1.pb_pointers[c]);
                                                fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                fread(&tempContent,sizeof(tempContent),1,archivoDisco);
                                                sprintf(insert,"%s|%s}}\"];\n",insert,tempContent.fcontenapunta);
                                                fputs(insert,archivoReporte);
                                                sprintf(insert,"Block%i -> Block%i;\n",pb3_2.pb_pointers[b],pb3_2_1.pb_pointers[c]);
                                                fputs(insert,archivoReporte);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }else{
        //Es una Carpeta
        //Mientras Tenga Contenido
        for(int i = 0 ; i < 15 ; i++){
            if(inodoM.i_block[i] != -1){
                if(i < 12){
                    //Directos
                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                    //Revisar Contenido
                    fseek(archivoDisco,getBlockStart_2(inodoM.i_block[i],sp_aux.sb_t_block),SEEK_SET);
                    fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                    for(int k = 0 ; k < 4 ; k++ ){
                        if(tempCopia.contenapunta[k].f_inode != -1){
                            sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                        }else{
                            sprintf(insert,"%s|NULL: -1",insert);
                        }
                    }
                    sprintf(insert,"%s}}\"];\n",insert);
                    fputs(insert,archivoReporte);
                    sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                    fputs(insert,archivoReporte);
                    //Revisar por el contenido
                    for(int k = 0 ; k < 4 ; k++ ){
                        if(tempCopia.contenapunta[k].f_inode != -1){
                            if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                //Es un archivo dentro de la carpeta
                                generarTREEiNode(archivoReporte, archivoDisco,pathDisco,inodoM.i_block[i],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                            }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                //Si es una carpeta
                                if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                    generarTREEiNode(archivoReporte, archivoDisco,pathDisco,inodoM.i_block[i],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                            }
                        }
                    }
                    //Fin
                }else{
                    if(i == 12){
                        //Indirecto
                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                        pointerBlock pb1;
                        int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                        //Revisar Contenido Indirecto
                        fseek(archivoDisco, offset, SEEK_SET);
                        fread(&pb1,sizeof(pb1),1,archivoDisco);
                        for( int a = 0 ; a < 16 ; a ++){
                            if(pb1.pb_pointers[a] != -1){
                                sprintf(insert,"%s|pBlock%i : %i",insert,a,pb1.pb_pointers[a]);
                            }else{
                                sprintf(insert,"%s|NULL: -1",insert);
                            }
                        }
                        sprintf(insert,"%s}}\"];\n",insert);
                        fputs(insert,archivoReporte);
                        sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                        fputs(insert,archivoReporte);
                        //Revisar contenido por Indirecto
                        for( int a = 0 ; a < 16 ; a ++){
                            if(pb1.pb_pointers[a] != -1){
                                //Bloque
                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb1.pb_pointers[a],pb1.pb_pointers[a]);

                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                for(int k = 0 ; k < 4 ; k++ ){
                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                        sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                    }else{
                                        sprintf(insert,"%s|NULL: -1",insert);
                                    }
                                }
                                sprintf(insert,"%s}}\"];\n",insert);
                                fputs(insert,archivoReporte);
                                sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb1.pb_pointers[a]);
                                fputs(insert,archivoReporte);
                            }
                        }
                        //Revisar por contenido
                        for( int a = 0 ; a < 16 ; a ++){
                            if(pb1.pb_pointers[a] != -1){
                                fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                for(int k = 0 ; k < 4 ; k++ ){
                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                            //Es un archivo dentro de la carpeta
                                            generarTREEiNode(archivoReporte, archivoDisco,pathDisco,pb1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                            //Si es una carpeta
                                            if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                generarTREEiNode(archivoReporte, archivoDisco,pathDisco,pb1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if(i == 13){
                            //Doble Indirecto
                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                            pointerBlock pb2;
                            int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                            //Revisar Contenido Doble Indirecto
                            fseek(archivoDisco, offset, SEEK_SET);
                            fread(&pb2,sizeof(pb2),1,archivoDisco);
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    sprintf(insert,"%s|pBlock%i : %i",insert,a,pb2.pb_pointers[a]);
                                }else{
                                    sprintf(insert,"%s|NULL: -1",insert);
                                }
                            }
                            sprintf(insert,"%s}}\"];\n",insert);
                            fputs(insert,archivoReporte);
                            sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                            fputs(insert,archivoReporte);
                            //Revisar Contenido Indirecto
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    //Indirecto
                                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2.pb_pointers[a],pb2.pb_pointers[a]);
                                    pointerBlock pb2_1;
                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                    //Revisar contenido por Indirecto
                                    for(int b = 0; b < 16 ; b++){
                                        if(pb2_1.pb_pointers[b] != -1){
                                            sprintf(insert,"%s|pBlock%i : %i",insert,b,pb2_1.pb_pointers[b]);
                                        }else{
                                            sprintf(insert,"%s|NULL: -1",insert);
                                        }
                                    }
                                    sprintf(insert,"%s}}\"];\n",insert);
                                    fputs(insert,archivoReporte);
                                    sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb2.pb_pointers[a]);
                                    fputs(insert,archivoReporte);
                                }
                            }
                            //Revisar contenido por Indirecto
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    //Indirecto
                                    pointerBlock pb2_1;
                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                    //Revisar contenido por Indirecto
                                    for(int b = 0; b < 16 ; b++){
                                        if(pb2_1.pb_pointers[b] != -1){
                                            //Bloque
                                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2_1.pb_pointers[b],pb2_1.pb_pointers[b]);

                                            fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[b],sp_aux.sb_t_block),SEEK_SET);
                                            fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                            for(int k = 0 ; k < 4 ; k++ ){
                                                if(tempCopia.contenapunta[k].f_inode != -1){
                                                    sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                                }else{
                                                    sprintf(insert,"%s|NULL: -1",insert);
                                                }
                                            }
                                            sprintf(insert,"%s}}\"];\n",insert);
                                            fputs(insert,archivoReporte);
                                            sprintf(insert,"Block%i -> Block%i;\n",pb2.pb_pointers[a],pb2_1.pb_pointers[b]);
                                            fputs(insert,archivoReporte);
                                        }
                                    }
                                }
                            }
                            //Revisar por contenido
                            for( int a = 0 ; a < 16 ; a ++){
                                if(pb2.pb_pointers[a] != -1){
                                    //Indirecto
                                    pointerBlock pb2_1;
                                    int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                    fseek(archivoDisco,offset_2,SEEK_SET);
                                    fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                    //Revisar contenido por Indirecto
                                    for(int b = 0; b < 16 ; b++){
                                        if(pb2_1.pb_pointers[b] != -1){
                                            //Bloque
                                            fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[b],sp_aux.sb_t_block),SEEK_SET);
                                            fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                            for(int k = 0 ; k < 4 ; k++ ){
                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                    //Es un archivo dentro de la carpeta
                                                    generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb2_1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                    //Si es una carpeta
                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                        generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb2_1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            if(i == 14){
                                //Triple indirecto
                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                                pointerBlock pb3;
                                int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                                //Revisar Contenido Triple Indirecto
                                fseek(archivoDisco, offset, SEEK_SET);
                                fread(&pb3,sizeof(pb3),1,archivoDisco);
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        sprintf(insert,"%s|pBlock%i : %i",insert,a,pb3.pb_pointers[a]);
                                    }else{
                                        sprintf(insert,"%s|NULL: -1",insert);
                                    }
                                }
                                sprintf(insert,"%s}}\"];\n",insert);
                                fputs(insert,archivoReporte);
                                sprintf(insert,"iNode%i -> Block%i;\n",actual,inodoM.i_block[i]);
                                fputs(insert,archivoReporte);
                                //Revisar Contenido Doble Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        //Doble Indirecto
                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3.pb_pointers[a],pb3.pb_pointers[a]);
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            if(pb3_2.pb_pointers[b] != -1){
                                                sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2.pb_pointers[b]);
                                            }else{
                                                sprintf(insert,"%s|NULL: -1",insert);
                                            }
                                        }
                                        sprintf(insert,"%s}}\"];\n",insert);
                                        fputs(insert,archivoReporte);
                                        sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb3.pb_pointers[a]);
                                        fputs(insert,archivoReporte);
                                    }
                                }
                                //Revisar Contenido Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            if(pb3_2.pb_pointers[b] != -1){
                                                //Indirecto
                                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2.pb_pointers[b],pb3_2.pb_pointers[b]);
                                                pointerBlock pb3_2_1;
                                                int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_3,SEEK_SET);
                                                fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                for(int c = 0 ; c < 16 ; c++ ){
                                                    if(pb3_2_1.pb_pointers[c] != -1){
                                                        sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2_1.pb_pointers[c]);
                                                    }else{
                                                        sprintf(insert,"%s|NULL: -1",insert);
                                                    }
                                                }
                                                sprintf(insert,"%s}}\"];\n",insert);
                                                fputs(insert,archivoReporte);
                                                sprintf(insert,"Block%i -> Block%i;\n",pb3.pb_pointers[a],pb3_2.pb_pointers[b]);
                                                fputs(insert,archivoReporte);
                                            }
                                        }
                                    }
                                }
                                //Revisar contenido por Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            pointerBlock pb3_2_1;
                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_3,SEEK_SET);
                                            fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                            for(int c = 0 ; c < 16 ; c++ ){
                                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2_1.pb_pointers[c],pb3_2_1.pb_pointers[c]);
                                                fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                //Revisar contenido por Indirecto
                                                for(int k = 0 ; k < 4 ; k++ ){
                                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                                        sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                                    }else{
                                                        sprintf(insert,"%s|NULL: -1",insert);
                                                    }
                                                }
                                                sprintf(insert,"%s}}\"];\n",insert);
                                                fputs(insert,archivoReporte);
                                                sprintf(insert,"Block%i -> Block%i;\n",pb3_2.pb_pointers[b],pb3_2_1.pb_pointers[c]);
                                                fputs(insert,archivoReporte);
                                            }
                                        }
                                    }
                                }
                                //Revisar por contenido
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb3.pb_pointers[a] != -1){
                                        pointerBlock pb3_2;
                                        int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                        fseek(archivoDisco,offset_2,SEEK_SET);
                                        fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                        for(int b = 0; b < 16 ; b++){
                                            pointerBlock pb3_2_1;
                                            int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_3,SEEK_SET);
                                            fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                            for(int c = 0 ; c < 16 ; c++ ){
                                                fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                //Revisar contenido por Indirecto
                                                for(int k = 0 ; k < 4 ; k++ ){
                                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                            //Es un archivo dentro de la carpeta
                                                            generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb3_2_1.pb_pointers[c],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                            //Si es una carpeta
                                                            if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                                generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb3_2_1.pb_pointers[c],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //Mientras Tenga Contenido
    return 0;
}
int generarTREE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                //Raiz
                inode inodoM;
                fseek(archivoDisco,sp_aux.sb_t_inode,SEEK_SET);
                fread(&inodoM,sizeof (inode),1,archivoDisco);
                //Para el contenido
                char insert[2048] = {0};
                contentBlock tempCopia;
                fputs("Digraph D{\nrankdir=LR;",archivoReporte);
                //Generar Primer iNodo
                char tipo[12] = {0};
                if(inodoM.i_type == '1'){
                    //Archivo
                    strcpy(tipo,"Archivo");
                }else{
                    //Carpeta
                    strcpy(tipo,"Carpeta");
                }
                sprintf(insert,"iNode%i[shape=record,label=\"{{INode%i|Owner: %i|Group: %i|Permisos: %i|Size: %i|Tipo: %s|Creacion: %s|Lectura: %s|Modificacion: %s",0,0,inodoM.i_uid,inodoM.i_gid,inodoM.i_perm,inodoM.i_size,tipo,inodoM.i_ctime,inodoM.i_atime,inodoM.i_mtime);
                for (int i = 0 ; i < 15; i++) {
                    if(inodoM.i_block[i] != -1){
                        sprintf(insert,"%s|iBlock%i: %i",insert,i,inodoM.i_block[i]);
                    }else{
                        sprintf(insert,"%s|iBlock%i: -1",insert,i);
                    }
                }
                sprintf(insert,"%s}}\"];\n",insert);
                fputs(insert,archivoReporte);
                //Mientras Tenga Contenido
                for(int i = 0 ; i < 15 ; i++){
                    if(inodoM.i_block[i] != -1){
                        if(i < 12){
                            //Directos
                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                            //Revisar Contenido
                            fseek(archivoDisco,getBlockStart_2(inodoM.i_block[i],sp_aux.sb_t_block),SEEK_SET);
                            fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                            for(int k = 0 ; k < 4 ; k++ ){
                                if(tempCopia.contenapunta[k].f_inode != -1){
                                    sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                }else{
                                    sprintf(insert,"%s|NULL: -1",insert);
                                }
                            }
                            sprintf(insert,"%s}}\"];\n",insert);
                            fputs(insert,archivoReporte);
                            sprintf(insert,"iNode0 -> Block%i;\n",inodoM.i_block[i]);
                            fputs(insert,archivoReporte);
                            //Revisar por el contenido
                            for(int k = 0 ; k < 4 ; k++ ){
                                if(tempCopia.contenapunta[k].f_inode != -1){
                                    if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                        //Es un archivo dentro de la carpeta
                                        generarTREEiNode(archivoReporte,archivoDisco,pathDisco,inodoM.i_block[i],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                    }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                        //Si es una carpeta
                                        if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                            generarTREEiNode(archivoReporte,archivoDisco,pathDisco,inodoM.i_block[i],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                    }
                                }
                            }
                            //Fin
                        }else{
                            if(i == 12){
                                //Indirecto
                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                                pointerBlock pb1;
                                int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                                //Revisar Contenido Indirecto
                                fseek(archivoDisco, offset, SEEK_SET);
                                fread(&pb1,sizeof(pb1),1,archivoDisco);
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb1.pb_pointers[a] != -1){
                                        sprintf(insert,"%s|pBlock%i : %i",insert,a,pb1.pb_pointers[a]);
                                    }else{
                                        sprintf(insert,"%s|NULL: -1",insert);
                                    }
                                }
                                sprintf(insert,"%s}}\"];\n",insert);
                                fputs(insert,archivoReporte);
                                sprintf(insert,"iNode0 -> Block%i;\n",inodoM.i_block[i]);
                                fputs(insert,archivoReporte);
                                //Revisar contenido por Indirecto
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb1.pb_pointers[a] != -1){
                                        //Bloque
                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb1.pb_pointers[a],pb1.pb_pointers[a]);

                                        fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                        for(int k = 0 ; k < 4 ; k++ ){
                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                            }else{
                                                sprintf(insert,"%s|NULL: -1",insert);
                                            }
                                        }
                                        sprintf(insert,"%s}}\"];\n",insert);
                                        fputs(insert,archivoReporte);
                                        sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb1.pb_pointers[a]);
                                        fputs(insert,archivoReporte);
                                    }
                                }
                                //Revisar por contenido
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb1.pb_pointers[a] != -1){
                                        fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                        for(int k = 0 ; k < 4 ; k++ ){
                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                    //Es un archivo dentro de la carpeta
                                                    generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                    //Si es una carpeta
                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                        generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                                }
                                            }
                                        }
                                    }
                                }
                            }else{
                                if(i == 13){
                                    //Doble Indirecto
                                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                                    pointerBlock pb2;
                                    int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                                    //Revisar Contenido Doble Indirecto
                                    fseek(archivoDisco, offset, SEEK_SET);
                                    fread(&pb2,sizeof(pb2),1,archivoDisco);
                                    for( int a = 0 ; a < 16 ; a ++){
                                        if(pb2.pb_pointers[a] != -1){
                                            sprintf(insert,"%s|pBlock%i : %i",insert,a,pb2.pb_pointers[a]);
                                        }else{
                                            sprintf(insert,"%s|NULL: -1",insert);
                                        }
                                    }
                                    sprintf(insert,"%s}}\"];\n",insert);
                                    fputs(insert,archivoReporte);
                                    sprintf(insert,"iNode0 -> Block%i;\n",inodoM.i_block[i]);
                                    fputs(insert,archivoReporte);
                                    //Revisar Contenido Indirecto
                                    for( int a = 0 ; a < 16 ; a ++){
                                        if(pb2.pb_pointers[a] != -1){
                                            //Indirecto
                                            sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2.pb_pointers[a],pb2.pb_pointers[a]);
                                            pointerBlock pb2_1;
                                            int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_2,SEEK_SET);
                                            fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                            //Revisar contenido por Indirecto
                                            for(int b = 0; b < 16 ; b++){
                                                if(pb2_1.pb_pointers[b] != -1){
                                                    sprintf(insert,"%s|pBlock%i : %i",insert,b,pb2_1.pb_pointers[b]);
                                                }else{
                                                    sprintf(insert,"%s|NULL: -1",insert);
                                                }
                                            }
                                            sprintf(insert,"%s}}\"];\n",insert);
                                            fputs(insert,archivoReporte);
                                            sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb2.pb_pointers[a]);
                                            fputs(insert,archivoReporte);
                                        }
                                    }
                                    //Revisar contenido por Indirecto
                                    for( int a = 0 ; a < 16 ; a ++){
                                        if(pb2.pb_pointers[a] != -1){
                                            //Indirecto
                                            pointerBlock pb2_1;
                                            int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_2,SEEK_SET);
                                            fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                            //Revisar contenido por Indirecto
                                            for(int b = 0; b < 16 ; b++){
                                                if(pb2_1.pb_pointers[b] != -1){
                                                    //Bloque
                                                    sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb2_1.pb_pointers[b],pb2_1.pb_pointers[b]);

                                                    fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[b],sp_aux.sb_t_block),SEEK_SET);
                                                    fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                    for(int k = 0 ; k < 4 ; k++ ){
                                                        if(tempCopia.contenapunta[k].f_inode != -1){
                                                            sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                                        }else{
                                                            sprintf(insert,"%s|NULL: -1",insert);
                                                        }
                                                    }
                                                    sprintf(insert,"%s}}\"];\n",insert);
                                                    fputs(insert,archivoReporte);
                                                    sprintf(insert,"Block%i -> Block%i;\n",pb2.pb_pointers[a],pb2_1.pb_pointers[b]);
                                                    fputs(insert,archivoReporte);
                                                }
                                            }
                                        }
                                    }
                                    //Revisar por contenido
                                    for( int a = 0 ; a < 16 ; a ++){
                                        if(pb2.pb_pointers[a] != -1){
                                            //Indirecto
                                            pointerBlock pb2_1;
                                            int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_2,SEEK_SET);
                                            fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                            //Revisar contenido por Indirecto
                                            for(int b = 0; b < 16 ; b++){
                                                if(pb2_1.pb_pointers[b] != -1){
                                                    //Bloque
                                                    fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[b],sp_aux.sb_t_block),SEEK_SET);
                                                    fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                    for(int k = 0 ; k < 4 ; k++ ){
                                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                            //Es un archivo dentro de la carpeta
                                                            generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb2_1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                            //Si es una carpeta
                                                            if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                                generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb2_1.pb_pointers[a],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }else{
                                    if(i == 14){
                                        //Triple indirecto
                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",inodoM.i_block[i],inodoM.i_block[i]);
                                        pointerBlock pb3;
                                        int offset = getBlockStart_2(inodoM.i_block[i] , sp_aux.sb_t_block);
                                        //Revisar Contenido Triple Indirecto
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb3,sizeof(pb3),1,archivoDisco);
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                sprintf(insert,"%s|pBlock%i : %i",insert,a,pb3.pb_pointers[a]);
                                            }else{
                                                sprintf(insert,"%s|NULL: -1",insert);
                                            }
                                        }
                                        sprintf(insert,"%s}}\"];\n",insert);
                                        fputs(insert,archivoReporte);
                                        sprintf(insert,"iNode0 -> Block%i;\n",inodoM.i_block[i]);
                                        fputs(insert,archivoReporte);
                                        //Revisar Contenido Doble Indirecto
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                //Doble Indirecto
                                                sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3.pb_pointers[a],pb3.pb_pointers[a]);
                                                pointerBlock pb3_2;
                                                int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_2,SEEK_SET);
                                                fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                for(int b = 0; b < 16 ; b++){
                                                    if(pb3_2.pb_pointers[b] != -1){
                                                        sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2.pb_pointers[b]);
                                                    }else{
                                                        sprintf(insert,"%s|NULL: -1",insert);
                                                    }
                                                }
                                                sprintf(insert,"%s}}\"];\n",insert);
                                                fputs(insert,archivoReporte);
                                                sprintf(insert,"Block%i -> Block%i;\n",inodoM.i_block[i],pb3.pb_pointers[a]);
                                                fputs(insert,archivoReporte);
                                            }
                                        }
                                        //Revisar Contenido Indirecto
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                pointerBlock pb3_2;
                                                int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_2,SEEK_SET);
                                                fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                for(int b = 0; b < 16 ; b++){
                                                    if(pb3_2.pb_pointers[b] != -1){
                                                        //Indirecto
                                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2.pb_pointers[b],pb3_2.pb_pointers[b]);
                                                        pointerBlock pb3_2_1;
                                                        int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                        fseek(archivoDisco,offset_3,SEEK_SET);
                                                        fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                        for(int c = 0 ; c < 16 ; c++ ){
                                                            if(pb3_2_1.pb_pointers[c] != -1){
                                                                sprintf(insert,"%s|pBlock%i : %i",insert,b,pb3_2_1.pb_pointers[c]);
                                                            }else{
                                                                sprintf(insert,"%s|NULL: -1",insert);
                                                            }
                                                        }
                                                        sprintf(insert,"%s}}\"];\n",insert);
                                                        fputs(insert,archivoReporte);
                                                        sprintf(insert,"Block%i -> Block%i;\n",pb3.pb_pointers[a],pb3_2.pb_pointers[b]);
                                                        fputs(insert,archivoReporte);
                                                    }
                                                }
                                            }
                                        }
                                        //Revisar contenido por Indirecto
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                pointerBlock pb3_2;
                                                int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_2,SEEK_SET);
                                                fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                for(int b = 0; b < 16 ; b++){
                                                    pointerBlock pb3_2_1;
                                                    int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                    fseek(archivoDisco,offset_3,SEEK_SET);
                                                    fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                    for(int c = 0 ; c < 16 ; c++ ){
                                                        sprintf(insert,"Block%i[shape=record,label=\"{{Block%i",pb3_2_1.pb_pointers[c],pb3_2_1.pb_pointers[c]);
                                                        fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                        //Revisar contenido por Indirecto
                                                        for(int k = 0 ; k < 4 ; k++ ){
                                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                                sprintf(insert,"%s|%s : %i",insert,tempCopia.contenapunta[k].f_name,tempCopia.contenapunta[k].f_inode);
                                                            }else{
                                                                sprintf(insert,"%s|NULL: -1",insert);
                                                            }
                                                        }
                                                        sprintf(insert,"%s}}\"];\n",insert);
                                                        fputs(insert,archivoReporte);
                                                        sprintf(insert,"Block%i -> Block%i;\n",pb3_2.pb_pointers[b],pb3_2_1.pb_pointers[c]);
                                                        fputs(insert,archivoReporte);
                                                    }
                                                }
                                            }
                                        }
                                        //Revisar por contenido
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                pointerBlock pb3_2;
                                                int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_2,SEEK_SET);
                                                fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                for(int b = 0; b < 16 ; b++){
                                                    pointerBlock pb3_2_1;
                                                    int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                    fseek(archivoDisco,offset_3,SEEK_SET);
                                                    fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                    for(int c = 0 ; c < 16 ; c++ ){
                                                        fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                        //Revisar contenido por Indirecto
                                                        for(int k = 0 ; k < 4 ; k++ ){
                                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                                    //Es un archivo dentro de la carpeta
                                                                    generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb3_2_1.pb_pointers[c],tempCopia.contenapunta[k].f_inode,'1',sp_aux,particionTrabajo);
                                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                                    //Si es una carpeta
                                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0)
                                                                        generarTREEiNode(archivoReporte,archivoDisco,pathDisco,pb3_2_1.pb_pointers[c],tempCopia.contenapunta[k].f_inode,'0',sp_aux,particionTrabajo);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                fputs("\n }", archivoReporte);
                exito_reporte = 1;
                fclose(archivoDisco);
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarSUPERBLOQUE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            //shape=record,color=red,style=filled, fillcolor=Cyan,
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                char insert[1024] = {0};
                sprintf(insert,
                        "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table color=\'green\' border=\'0\' cellborder=\'1\' cellspacing=\'0\'  width=\'300\' height=\'160\'  ><tr><td>Nombre</td><td>Valor</td></tr>\n");
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Magic</td><td>%i</td></tr>",sp_aux.sb_magic);
                fputs(insert,archivoReporte);
                if(sp_aux.sb_filesystem_type == 2){
                    sprintf(insert,"<tr><td>SuperBlock FileSystem</td><td>EXT2</td></tr>");
                    fputs(insert,archivoReporte);
                }else if(sp_aux.sb_filesystem_type == 3){
                    sprintf(insert,"<tr><td>SuperBlock FileSystem</td><td>EXT3</td></tr>");
                    fputs(insert,archivoReporte);
                }
                sprintf(insert,"<tr><td>SuperBlock Mount Time</td><td>%s</td></tr>",sp_aux.sb_mtime);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Unmount Time</td><td>%s</td></tr>",sp_aux.sb_umtime);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Mount Count</td><td>%i</td></tr>",sp_aux.mount_count);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock iNode Size</td><td>%i</td></tr>",sp_aux.sb_inode_size);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Block Size</td><td>%i</td></tr>",sp_aux.sb_block_size);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock BitMap iNode</td><td>%i</td></tr>",sp_aux.sb_bm_inode);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock BitMap Block</td><td>%i</td></tr>",sp_aux.sb_bm_Block);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock iNode Table</td><td>%i</td></tr>",sp_aux.sb_t_inode);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Block Table</td><td>%i</td></tr>",sp_aux.sb_t_block);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock iNode Count</td><td>%i</td></tr>",sp_aux.sb_inodes_count);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Block Count</td><td>%i</td></tr>",sp_aux.sb_blocks_count);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Free iNode Count</td><td>%i</td></tr>",sp_aux.sb_free_inodes_count);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock Free Block Count</td><td>%i</td></tr>",sp_aux.sb_free_blocks_count);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock First Free iNode</td><td>%i</td></tr>",sp_aux.sb_first_inode);
                fputs(insert,archivoReporte);
                sprintf(insert,"<tr><td>SuperBlock First Free Block</td><td>%i</td></tr>",sp_aux.sb_first_block);
                fputs(insert,archivoReporte);
                fputs("\n</table> >  ];}", archivoReporte);
                fclose(archivoDisco);
                exito_reporte = 1;
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarFILE(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo, char rutaArchivo[]){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo,archivoDisco);
            if(sp_aux.sb_magic == 61267){
                //Buscar Archivo
                inode inodoM;
                fseek(archivoDisco,sp_aux.sb_t_inode,SEEK_SET);
                fread(&inodoM,sizeof (inode),1,archivoDisco);
                //Recorrer hacia el archivo
                int posInodo = 0;
                int i = 1;
                char aux[9999]= {0};
                while(rutaArchivo[i] != NULL){
                    char aux_char[]= {rutaArchivo[i],'\0'};
                    if(rutaArchivo[i] == '/'){
                        int auxfind = findArchivo(aux , inodoM, 0, archivoDisco);
                        if(auxfind  == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Puede que el archivo '%s' no exista\n",rutaArchivo);
                            return -1;
                        }else{
                            fseek(archivoDisco, getiNodeStart_2(auxfind, sp_aux.sb_t_inode), SEEK_SET);
                            fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                            posInodo = auxfind;
                        }
                        memset(&aux,0,9999);
                    }
                    else{
                        strcat(aux, aux_char);
                    }
                    i++;
                }
                //Encontro carpeta deseada, existencia de archivo
                archivo tempReporte;
                memset(&tempReporte.contenido,0,999);
                tempReporte = getArchivo(aux,particionTrabajo,sp_aux.sb_t_inode,getiNodeStart_2(posInodo, sp_aux.sb_t_inode),archivoDisco);
                if(tempReporte.estado != -1){
                    char insert[1024] = {0};
                    sprintf(insert, "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table><tr><td>Nombre: %s</td></tr>\n",rutaArchivo);
                    fputs(insert,archivoReporte);
                    sprintf(insert, "<tr><td>%s</td></tr>\n",tempReporte.contenido);
                    fputs(insert,archivoReporte);
                    fputs("\n</table> >  ];}", archivoReporte);
                    exito_reporte = 1;
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Puede que el archivo '%s' no exista\n",rutaArchivo);
                    return -1;
                }
                fclose(archivoDisco);
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int generarLS(char pathSalida[], char pathDisco[], PMOUNT particionTrabajo, char rutaArchivo[]){
    if(existsFile(pathSalida)==1){
        //Seguro para generar reporte nuevo
        remove(pathSalida);
    }
    int exito_reporte = 0;
    FILE *archivoReporte;
    archivoReporte = fopen(pathSalida,"w");
    if(archivoReporte!=NULL){
        //Abrio archivo de reporte
        FILE *archivoDisco;
        archivoDisco = fopen(pathDisco,"rb+");
        if(archivoDisco!=NULL){
            //Disco se pudo abrir
            SuperBloque sp_aux = getSuperBlock(particionTrabajo, archivoDisco);
            if(sp_aux.sb_magic == 61267){
                //Buscar Archivo
                inode inodoM;
                fseek(archivoDisco,sp_aux.sb_t_inode,SEEK_SET);
                fread(&inodoM,sizeof (inode),1,archivoDisco);
                //Recorrer hacia el archivo
                int posInodo = 0;
                int i = 1;
                char aux[9999]= {0};
                while(rutaArchivo[i] != NULL){
                    char aux_char[]= {rutaArchivo[i],'\0'};
                    if(rutaArchivo[i] == '/'){
                        int auxfind = findArchivo(aux , inodoM, 0, archivoDisco);
                        if(auxfind  == -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Puede que el archivo '%s' no exista\n",rutaArchivo);
                            return -1;
                        }else{
                            fseek(archivoDisco, getiNodeStart_2(auxfind, sp_aux.sb_t_inode), SEEK_SET);
                            fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                            posInodo = auxfind;
                        }
                        memset(&aux,0,9999);
                    }
                    else{
                        strcat(aux, aux_char);
                    }
                    i++;
                }
                //Revisar si es carpeta o archivo
                char insert[1024] = {0};
                sprintf(insert, "digraph D {\n  node [shape=plaintext] rankdir=LR;\n  nodoUnico[label=< <table color=\'yellow\' border=\'0\' cellborder=\'1\' cellspacing=\'0\'  width=\'300\' height=\'160\' ><tr><td>Permisos</td><td>Owner</td><td>Grupo</td><td>Size (en Bytes)</td><td>Fecha Modificacion</td><td>Tipo</td><td>Nombre</td></tr>\n");
                fputs(insert,archivoReporte);
                if(verificarExtension2(aux) == 0){
                    //Es archivo
                    char nombre[12] = {0};
                    strcpy(nombre,aux);

                    archivo tempReporte;
                    tempReporte = getArchivo(aux,particionTrabajo,sp_aux.sb_t_inode,getiNodeStart_2(posInodo, sp_aux.sb_t_inode),archivoDisco);

                    fseek(archivoDisco, getiNodeStart_2(tempReporte.position, sp_aux.sb_t_inode), SEEK_SET);
                    fread(&inodoM, sizeof(inode), 1 , archivoDisco);

                    char *tempPerm = getPermisos(inodoM.i_perm);
                    char tempPerm2[12] = {0};
                    strcpy(tempPerm2,tempPerm);
                    free(tempPerm);

                    char user[12] = {0};
                    for(int i = 0; i < globNusers ; i++){
                        if(globusers[i].numGrupo == inodoM.i_uid){
                            for(int j = 0 ; j < 12 ; j++){
                                user[j] = globusers[i].name[j];
                            }
                            break;
                        }
                    }
                    char grupo[12] = {0};
                    for(int i = 0; i < globNGrupos ; i++){
                        if(globGrupos[i].numGrupo == inodoM.i_gid){
                            for(int j = 0 ; j < 12 ; j++){
                                grupo[j] = globGrupos[i].name[j];
                            }
                            break;
                        }
                    }
                    char tipo[12] = {0};
                    if(inodoM.i_type == '1'){
                        //Archivo
                        strcpy(tipo,"Archivo");
                    }else{
                        //Carpeta
                        strcpy(tipo,"Carpeta");
                    }

                    sprintf(insert, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",tempPerm2,user,grupo,inodoM.i_size,inodoM.i_mtime,tipo,nombre);
                    fputs(insert,archivoReporte);
                }else{
                    //Es carpeta
                    char nombre[12] = {0};
                    if(strcmp(aux,"") != 0){
                        strcpy(nombre,aux);

                        int find_archivo = findArchivo(aux,inodoM,0,archivoDisco);

                        fseek(archivoDisco, getiNodeStart_2(find_archivo, sp_aux.sb_t_inode), SEEK_SET);
                        fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                    }else{
                        strcpy(nombre,"/");
                    }
                    char *tempPerm = getPermisos(inodoM.i_perm);
                    char tempPerm2[12] = {0};
                    strcpy(tempPerm2,tempPerm);
                    free(tempPerm);

                    char user[12] = {0};
                    for(int i = 0; i < globNusers ; i++){
                        if(globusers[i].numGrupo == inodoM.i_uid){
                            for(int j = 0 ; j < 12 ; j++){
                                user[j] = globusers[i].name[j];
                            }
                            break;
                        }
                    }
                    char grupo[12] = {0};
                    for(int i = 0; i < globNGrupos ; i++){
                        if(globGrupos[i].numGrupo == inodoM.i_gid){
                            for(int j = 0 ; j < 12 ; j++){
                                grupo[j] = globGrupos[i].name[j];
                            }
                            break;
                        }
                    }
                    char tipo[12] = {0};
                    if(inodoM.i_type == '1'){
                        //Archivo
                        strcpy(tipo,"Archivo");
                    }else{
                        //Carpeta
                        strcpy(tipo,"Carpeta");
                    }

                    sprintf(insert, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",tempPerm2,user,grupo,inodoM.i_size,inodoM.i_mtime,tipo,nombre);
                    fputs(insert,archivoReporte);
                    //Mientras Tenga Contenido
                    contentBlock tempCopia;
                    int auxCola = 0;
                    int auxX = 0;
                    userstats ColaArchivos[100];  // Introducir los iNodos aqui
                    for(int x = 0 ; x < 100 ; x++ ){
                        ColaArchivos[x].numGrupo = -1;
                        memset(&ColaArchivos[x].name,0,999 );
                    }
                    //Empezar analisis de contenido de carpeta
                    do{
                        if(auxX < 12){
                            //Directos
                            fseek(archivoDisco,getBlockStart_2(inodoM.i_block[auxX],sp_aux.sb_t_block),SEEK_SET);
                            fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                            for(int k = 0 ; k < 4 ; k++ ){
                                if(tempCopia.contenapunta[k].f_inode != -1){
                                    if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                        //Es un archivo dentro de la carpeta
                                        ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                        strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                        ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                        auxCola++;
                                    }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                        //Si es una carpeta
                                        if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0){
                                            ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                            strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                            ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                            auxCola++;
                                        }
                                    }
                                }
                            }
                        }else{
                            if(auxX == 12){
                                //Indirecto
                                pointerBlock pb1;
                                int offset = getBlockStart_2(inodoM.i_block[auxX] , sp_aux.sb_t_block);
                                fseek(archivoDisco, offset, SEEK_SET);
                                fread(&pb1,sizeof(pb1),1,archivoDisco);
                                for( int a = 0 ; a < 16 ; a ++){
                                    if(pb1.pb_pointers[a] != -1){
                                        fseek(archivoDisco,getBlockStart_2(pb1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                        for(int k = 0 ; k < 4 ; k++ ){
                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                    //Es un archivo dentro de la carpeta
                                                    ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                    strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                    ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                    auxCola++;
                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                    //Si es una carpeta
                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"") != 0){
                                                        ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                        strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                        ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                        auxCola++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }else{
                                if(auxX == 13){
                                    //Doble Indirecto
                                    pointerBlock pb2;
                                    int offset = getBlockStart_2(inodoM.i_block[auxX] , sp_aux.sb_t_block);
                                    fseek(archivoDisco, offset, SEEK_SET);
                                    fread(&pb2,sizeof(pb2),1,archivoDisco);
                                    for( int a = 0 ; a < 16 ; a ++){
                                        if(pb2.pb_pointers[a] != -1){
                                            pointerBlock pb2_1;
                                            int offset_2 = getBlockStart_2(pb2.pb_pointers[a], sp_aux.sb_t_block);
                                            fseek(archivoDisco,offset_2,SEEK_SET);
                                            fread(&pb2_1,sizeof(pb2_1),1,archivoDisco);
                                            for(int b = 0; b < 16 ; b++){
                                                fseek(archivoDisco,getBlockStart_2(pb2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                for(int k = 0 ; k < 4 ; k++ ){
                                                    if(tempCopia.contenapunta[k].f_inode != -1){
                                                        if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                            //Es un archivo dentro de la carpeta
                                                            ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                            strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                            ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                            auxCola++;
                                                        }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                            //Si es una carpeta
                                                            if(strcmp(tempCopia.contenapunta[k].f_name,"")!=0){
                                                                ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                                auxCola++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }else{
                                    if(auxX == 14){
                                        //Triple indirecto
                                        pointerBlock pb3;
                                        int offset = getBlockStart_2(inodoM.i_block[auxX] , sp_aux.sb_t_block);
                                        fseek(archivoDisco, offset, SEEK_SET);
                                        fread(&pb3,sizeof(pb3),1,archivoDisco);
                                        for( int a = 0 ; a < 16 ; a ++){
                                            if(pb3.pb_pointers[a] != -1){
                                                pointerBlock pb3_2;
                                                int offset_2 = getBlockStart_2(pb3.pb_pointers[a], sp_aux.sb_t_block);
                                                fseek(archivoDisco,offset_2,SEEK_SET);
                                                fread(&pb3_2,sizeof(pb3_2),1,archivoDisco);
                                                for(int b = 0; b < 16 ; b++){
                                                    pointerBlock pb3_2_1;
                                                    int offset_3 = getBlockStart_2(pb3_2.pb_pointers[b],sp_aux.sb_t_block);
                                                    fseek(archivoDisco,offset_3,SEEK_SET);
                                                    fread(&pb3_2_1,sizeof (pb3_2_1),1,archivoDisco);
                                                    for(int c = 0 ; c < 16 ; c++ ){
                                                        fseek(archivoDisco,getBlockStart_2(pb3_2_1.pb_pointers[a],sp_aux.sb_t_block),SEEK_SET);
                                                        fread(&tempCopia,sizeof(tempCopia),1,archivoDisco);
                                                        for(int k = 0 ; k < 4 ; k++ ){
                                                            if(tempCopia.contenapunta[k].f_inode != -1){
                                                                if(verificarExtension2(tempCopia.contenapunta[k].f_name) == 0){
                                                                    //Es un archivo dentro de la carpeta
                                                                    ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                    strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                    ColaArchivos[auxCola].numGrupo = 1; //Representa un archivo
                                                                    auxCola++;
                                                                }else if( strcmp(tempCopia.contenapunta[k].f_name,".") != 0 && strcmp(tempCopia.contenapunta[k].f_name,"..") != 0 ){
                                                                    //Si es una carpeta
                                                                    if(strcmp(tempCopia.contenapunta[k].f_name,"")!=0){
                                                                        ColaArchivos[auxCola].position = tempCopia.contenapunta[k].f_inode;
                                                                        strcpy(ColaArchivos[auxCola].name,tempCopia.contenapunta[k].f_name);
                                                                        ColaArchivos[auxCola].numGrupo = 0; //Representa una carpeta
                                                                        auxCola++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //revisar por mas elementos para copiar
                        auxX++;
                    }while(inodoM.i_block[auxX] != -1 && auxX < 15);
                    //Termino analisis
                    if(auxCola > 0){
                        //Hay contenido para copiar
                        int l = 0;
                        while(ColaArchivos[l].numGrupo != -1){
                            if(ColaArchivos[l].numGrupo == 0){
                                //Es carpeta
                                fseek(archivoDisco, getiNodeStart_2(ColaArchivos[l].position, sp_aux.sb_t_inode), SEEK_SET);
                                fread(&inodoM, sizeof(inode), 1 , archivoDisco);
                                //Ingresar Carpeta
                                strcpy(nombre,ColaArchivos[l].name);

                                char *tempPerm = getPermisos(inodoM.i_perm);
                                char tempPerm2[12] = {0};
                                strcpy(tempPerm2,tempPerm);
                                free(tempPerm);

                                char user[12] = {0};
                                for(int i = 0; i < globNusers ; i++){
                                    if(globusers[i].numGrupo == inodoM.i_uid){
                                        for(int j = 0 ; j < 12 ; j++){
                                            user[j] = globusers[i].name[j];
                                        }
                                        break;
                                    }
                                }
                                char grupo[12] = {0};
                                for(int i = 0; i < globNGrupos ; i++){
                                    if(globGrupos[i].numGrupo == inodoM.i_gid){
                                        for(int j = 0 ; j < 12 ; j++){
                                            grupo[j] = globGrupos[i].name[j];
                                        }
                                        break;
                                    }
                                }
                                char tipo[12] = {0};
                                if(inodoM.i_type == '1'){
                                    //Archivo
                                    strcpy(tipo,"Archivo");
                                }else{
                                    //Carpeta
                                    strcpy(tipo,"Carpeta");
                                }

                                sprintf(insert, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",tempPerm2,user,grupo,inodoM.i_size,inodoM.i_mtime,tipo,nombre);
                                fputs(insert,archivoReporte);
                            }
                            if(ColaArchivos[l].numGrupo == 1){
                                //Es archivo
                                for(int a = 0 ; a < 12 ; a++){
                                    nombre[a] = ColaArchivos[l].name[a];
                                }

                                fseek(archivoDisco, getiNodeStart_2(ColaArchivos[l].position, sp_aux.sb_t_inode), SEEK_SET);
                                fread(&inodoM, sizeof(inode), 1 , archivoDisco);

                                char *tempPerm = getPermisos(inodoM.i_perm);
                                char tempPerm2[12] = {0};
                                strcpy(tempPerm2,tempPerm);
                                free(tempPerm);

                                char user[12] = {0};
                                for(int i = 0; i < globNusers ; i++){
                                    if(globusers[i].numGrupo == inodoM.i_uid){
                                        for(int j = 0 ; j < 12 ; j++){
                                            user[j] = globusers[i].name[j];
                                        }
                                        break;
                                    }
                                }
                                char grupo[12] = {0};
                                for(int i = 0; i < globNGrupos ; i++){
                                    if(globGrupos[i].numGrupo == inodoM.i_gid){
                                        for(int j = 0 ; j < 12 ; j++){
                                            grupo[j] = globGrupos[i].name[j];
                                        }
                                        break;
                                    }
                                }
                                char tipo[12] = {0};
                                if(inodoM.i_type == '1'){
                                    //Archivo
                                    strcpy(tipo,"Archivo");
                                }else{
                                    //Carpeta
                                    strcpy(tipo,"Carpeta");
                                }

                                sprintf(insert, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",tempPerm2,user,grupo,inodoM.i_size,inodoM.i_mtime,tipo,nombre);
                                fputs(insert,archivoReporte);
                            }
                            l++;
                        }
                    }
                }
                fputs("\n</table> >  ];}", archivoReporte);
                fclose(archivoDisco);
                exito_reporte = 1;
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Sistema de archivos corrompido\n");
                return -1;
            }
        }
        fclose(archivoReporte);
    }
    return exito_reporte;
}
int ejecutarRep(char pathSalida[], char pathDisco[],int tipoRep,PMOUNT particionTrabajo, char rutaArchivo[]){
    if(existsFile(pathDisco)==1){
        if(tipoRep != 10 && tipoRep != 11 && strlen(rutaArchivo) > 0){
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Este tipo de reporte no utiliza el parametro -ruta~:~\n");
            return -1;
        }
        if(tipoRep != 1 && tipoRep != 2 && particionTrabajo.mount_start == -1){
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Este tipo de reporte requiere la particion a trabajar \n");
            return -1;
        }
        CrearPath(pathSalida);
        FILE *archivoReporte;
        archivoReporte = fopen(pathSalida,"w");
        if(archivoReporte!=NULL){
            fclose(archivoReporte);
            if(tipoRep == 1){
                return generarMBR(pathSalida,pathDisco);
            }else if(tipoRep == 2){
                return generarDISK(pathSalida,pathDisco);
            }else if(tipoRep == 3){
                return generarINODE(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 4){
                return generarJOURNALING(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 5){
                return generarBLOCK(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 6){
                return generarBM_INODO(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 7){
                return generarBM_BLOQUE(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 8){
                return generarTREE(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 9){
                return generarSUPERBLOQUE(pathSalida,pathDisco,particionTrabajo);
            }else if(tipoRep == 10){
                return generarFILE(pathSalida,pathDisco,particionTrabajo,rutaArchivo);
            }else if(tipoRep == 11){
                return generarLS(pathSalida,pathDisco,particionTrabajo,rutaArchivo);
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Tipo de reporte desconocido \n");
                return -1;
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo generar archivo de reporte \n");
            return -1;
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se pudo abrir archivo de disco de reporte \n");
        return -1;
    }
    return 0;
}
void ejecutarShowMount(){
    printf("\033[1;105m COMANDO REALIZADO \033[0m Los discos montados son: \n");
    for(int i = 0; i < 26 ; i++ ){
        //Recorrer los discos
        if(discosMontados[i].letra != -1){
            //Existe disco en i
            printf("     Disco '%c' : path '%s' particiones:\n",discosMontados[i].letra,discosMontados[i].path);
            for(int j = 0; j < 10; j++ ){
                //Recorrer particiones de disco i
                if(discosMontados[i].particiones_disco[j].mount_start != -1){
                    //Existe particion en i
                    printf("          Particion '%s' : nombre '%s' \n",discosMontados[i].particiones_disco[j].mount_id,discosMontados[i].particiones_disco[j].mount_name);
                }
            }
        }
    }
}



int cargaComando(char entrada[]){
    strcat(entrada," ");
    exitoArchivo = false;
    int i = 0;
    int parametro = -1;
    int comandoausar = -1;
    regex_t regex;
    int estadoRegex = -1;
    int fs = 2;
    int del = 0;
    int add = 0;
    int ugo_int = 0;
    int size = 0;
    int unit = 0;
    char fit = 'f';
    char type = 'p';
    int nfiles = 0;
    int tipoFDisk = 0;
    char id[999] = {0};
    char usr[999] = {0};
    char pwd[999] = {0};
    char grp[999] = {0};
    char ugo[999] = {0};
    char name[999] = {0};
    char path[999] = {0};
    char ruta[999] = {0};
    char cont[999] = {0};
    char dest[999] = {0};
    archivoN filesN[25];
    for(int x = 0 ; x < 25 ; x++){
        filesN[x].n = -1;
        memset(&filesN[x].path,0,999);
    }
    int bp = 0;
    int br = 0;
    int bid = 0;
    int bfs = 0;
    int bfit = 0;
    int badd = 0;
    int busr = 0;
    int bpwd = 0;
    int bgrp = 0;
    int bugo = 0;
    int bcont = 0;
    int bpath = 0;
    int bsize = 0;
    int btype = 0;
    int bunit = 0;
    int bname = 0;
    int bdest = 0;
    int bruta = 0;
    int bguion = 0;
    int bfilen = 0;
    int bdelete = 0;
    int bcomando = 0;
    int bcomilla = -1;
    int bdosPuntos = 0;
    int bpathactive = 0;
    int bnameactive = 0;
    char fileNAux[999] = {0};
    char aux[270000] = {0};
    estadoRegex = regcomp(&regex,"-(f|F)(i|I)(l|L)(e|E)[0-9]+=",REG_EXTENDED);
    if(estadoRegex){
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: expresion de -fileN= erronea \n");
    }
    while(entrada[i] != NULL){
        if(entrada[i] == '\r'){
            entrada[i] = ' ';
        }
        if(65 <= entrada[i] && entrada[i] <= 90){
            if( bnameactive != 1 && bcomilla != 1 && bpathactive == 0){
                entrada[i] = entrada[i] + 32;
           }
        }
        char aux_char[] = {entrada[i],'\0'};
        if(((entrada[i] == 32 || entrada[i] =='\n' || entrada[i]==35 ) && bcomilla == -1 ) || (bcomilla == 1 && entrada[i]==34)){
            if( bcomilla == 1 && entrada[i] == 34){
                bcomilla = -1;
            }
            if( bcomando == 1){
                if( comandoausar == -1){
                    if(strcasecmp(aux,"exit") == 0 ){
                        comandoausar = 0;
                    }else if(strcasecmp(aux , "mkdisk") == 0){
                        comandoausar = 1;
                    }else if(strcasecmp(aux , "rmdisk") == 0){
                        comandoausar = 2;
                    }else if(strcasecmp(aux , "fdisk") == 0){
                        comandoausar = 3;
                    }else if(strcasecmp(aux , "exec") == 0){
                        printf("ejecutando...\n");
                        comandoausar = 4;
                    }else if(strcasecmp(aux , "mount") == 0){
                        comandoausar = 5;
                    }else if(strcasecmp(aux , "unmount") == 0){
                        comandoausar = 6;
                    }else if(strcasecmp(aux , "rep") == 0){
                        comandoausar = 7;
                    }else if(strcasecmp(aux, "showmount") == 0 ){
                        comandoausar = 8;
                    }else if(strcasecmp(aux, "mkfs") == 0 ){
                        comandoausar = 9;
                    }else if(strcasecmp(aux, "login") == 0 ){
                        comandoausar = 10;
                    }else if(strcasecmp(aux, "logout") == 0 ){
                        comandoausar = 11;
                    }else if(strcasecmp(aux, "mkgrp") == 0 ){
                        comandoausar = 12;
                    }else if(strcasecmp(aux, "rmgrp") == 0 ){
                        comandoausar = 13;
                    }else if(strcasecmp(aux, "mkusr") == 0 ){
                        comandoausar = 14;
                    }else if(strcasecmp(aux, "rmusr") == 0 ){
                        comandoausar = 15;
                    }else if(strcasecmp(aux, "chmod") == 0 ){
                        comandoausar = 16;
                    }else if(strcasecmp(aux, "mkfile") == 0 ){
                        comandoausar = 17;
                    }else if(strcasecmp(aux, "cat") == 0 ){
                        comandoausar = 18;
                    }else if(strcasecmp(aux, "rem") == 0 ){
                        comandoausar = 19;
                    }else if(strcasecmp(aux, "edit") == 0 ){
                        comandoausar = 20;
                    }else if(strcasecmp(aux, "ren") == 0 ){
                        comandoausar = 21;
                    }else if(strcasecmp(aux, "mkdir") == 0 ){
                        comandoausar = 22;
                    }else if(strcasecmp(aux, "cp") == 0 ){
                        comandoausar = 23;
                    }else if(strcasecmp(aux, "mv") == 0 ){
                        comandoausar = 24;
                    }else if(strcasecmp(aux, "find") == 0 ){
                        comandoausar = 25;
                    }else if(strcasecmp(aux, "chown") == 0 ){
                        comandoausar = 26;
                    }else if(strcasecmp(aux, "chgrp") == 0 ){
                        comandoausar = 27;
                    }else if(strcasecmp(aux, "pause") == 0 ){
                        comandoausar = 28;
                    }else if(strcasecmp(aux, "convert") == 0 ){
                        comandoausar = 29;
                    }else if(strcasecmp(aux, "recovery") == 0 ){
                        comandoausar = 30;
                    }else if(strcasecmp(aux, "loss") == 0 ){
                        comandoausar = 31;
                    }


                    else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Comando '%s' no reconocido. \n",aux);
                        return 0;
                    }
                    *aux = '\0';
                }else if( strcmp(aux,"") != 0){
                    //Abstraer datos de parametro ingresado
                    if ( parametro == -1 ){
                        if( strcasecmp(aux,"-r") == 0 ){
                            //-R
                            if(br == 1){
                               //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -R ya utilizado \n");
                                 printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -R ya utilizado \n");

                            }else {
                                br = 1;
                            }
                        }else if( strcasecmp(aux,"-p") == 0 ){
                            //-P
                            if(bp == 1){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -P ya utilizado \n");

                            }else{
                                bp = 1;
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro '%s' no reconocido. \n",aux);
                        }
                    }else if( parametro == 0 ){
                        //-Size=X
                        if(bsize == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -size= ya utilizado \n");

                        }else{
                            bsize = 1;
                            size = atoi(aux);
                        }
                    }else if( parametro == 1 ){
                        //-Unit=X
                        if(bunit == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -unit= ya utilizado \n");
                        }else{
                            if(strcasecmp(aux,"k") == 0){
                                bunit = 1;
                                unit = 1024;
                            }else if(strcasecmp(aux,"m") == 0){
                                bunit = 1;
                                unit = 1024 * 1024;
                            }else if(strcasecmp(aux,"b") == 0){
                                bunit = 1;
                                unit = 1;
                            }
                        }
                    }else if( parametro == 2 ){
                        //-Path=X
                        if(bpath == 1){
                          //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Path= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Path= ya utilizado \n");

                         }else{
                            bpath = 1;
                            bpathactive = -1;
                            strcpy(path,aux);
                            *aux = '\0';
                        }
                    }else if ( parametro == 3 ){
                        //-Type=X
                        if(btype == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -type= ya utilizado \n");

                        }else{
                            if(strcasecmp(aux,"p") == 0){
                                btype = 1;
                                type = 'p'; //Tipo Primaria
                            }else if(strcasecmp(aux,"e") == 0){
                                btype = 1;
                                type = 'e'; //Tipo Extendida
                            }else if(strcasecmp(aux,"l") == 0){
                                btype = 1;
                                type = 'l'; //Tipo logica
                            }else if(strcasecmp(aux,"fast") == 0){
                                btype = 1;
                                type = 'r'; //Formateo rapido
                            }else if(strcasecmp(aux,"full") == 0){
                                btype = 1;
                                type = 'c'; //Formateo completo
                            }else{
                             printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: opcion no valida para -Type=, solo puede ser P | E | L , se tomara como P por defecto. \n");

                            }
                        }
                    }else if ( parametro == 4 ){
                        //-Fit=X
                        if(bfit == 1){
                         printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -fit= ya utilizado \n");

                        }else{
                            if(strcasecmp(aux,"ff")==0){
                                bfit = 1;
                                fit = 'f'; //primer ajuste
                            }else if(strcasecmp(aux,"wf")==0){
                                bfit = 1;
                                fit = 'w'; //peor ajuste
                            }else if(strcasecmp(aux,"bf")==0){
                                bfit = 1;
                                fit = 'b'; //mejor ajuste
                            }else{
                              //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: opcion no valida para -Fit=, solo puede ser wf | ff | bf , se tomara como ff por defecto. \n");
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: opcion no valida para -Fit=, solo puede ser wf | ff | bf , se tomara como ff por defecto. \n");

                            }
                        }
                    }else if ( parametro == 5 ){
                        //-Delete=X
                        if(bdelete == 1){
                           // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Delete= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Delete= ya utilizado. \n");

                        }else{
                            if(strcasecmp(aux,"full")==0){
                                del = 1;
                            }else if(strcasecmp(aux,"fast")==0){
                                del = 2;
                            }
                            if(del != 0 && tipoFDisk == 0){
                                tipoFDisk = 2;
                                bdelete = 1;
                            }
                        }
                    }else if ( parametro == 6 ){
                        //-Name=X
                        if(bname == 1){
                           // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Name= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Name= ya utilizado  \n");

                        }else{
                            bname = 1;
                            bnameactive = -1;
                            strcpy(name,aux);
                        }
                    }else if ( parametro == 7 ){
                        //-Add=X
                        if(badd == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -add= ya utilizado  \n");
                        }else{
                            if(tipoFDisk == 0){
                                badd = 1;
                                tipoFDisk = 1;
                            }
                            add = atoi(aux);
                        }
                    }else if ( parametro == 8 ){
                        //-id=X
                        if(bid == 1){
                          //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Id= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -Id= ya utilizado  \n");

                        }else{
                            strcpy(id,aux);
                            bid = 1;
                        }
                    }
                         /*
                    else if( parametro == 9 ){
                        //-fs=X
                        if(bfs == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -fs= ya utilizado \n");
                        }else{

                            if( strcasecmp(aux,"2fs") == 0){
                                bfs = 1;
                                fs = 2;
                            }else

                                if(strcasecmp(aux,"3fs") == 0){
                                bfs = 1;
                                fs = 3;
                            }else {
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: opcion no valida para -fs=, solo puede ser 2fs | 3fs \n");
                            }
                        }
                    }
                    */
                    else if( parametro == 10){
                        //-usr=X
                        if(busr == 1){
                          //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -usr= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -usr= ya utilizado  \n");

                        }else{
                            busr = 1;
                            bnameactive = -1;
                            strcpy(usr,aux);
                        }
                    }else if( parametro == 11){
                        //-pwd=X
                        if(bpwd == 1){
                         // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -pws= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -pws= ya utilizado  \n");

                        }else{
                            bpwd = 1;
                            bnameactive = -1;
                            strcpy(pwd,aux);
                        }
                    }else if( parametro == 12 ){
                        //-grp=X
                        if(bgrp == 1){
                          //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -grp= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -grp= ya utilizado  \n");

                        }else{
                            bgrp = 1;
                            bnameactive = -1;
                            strcpy(grp,aux);
                        }
                    }else if( parametro == 13 ){
                        //-ugo=X
                        if(bugo == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -ugo= ya utilizado  \n");

                          //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -ugo= ya utilizado \n");
                        }else{
                            bugo = 1;
                            strcpy(ugo,aux);
                            ugo_int = atoi(ugo);
                        }
                    }else if( parametro == 14 ){
                        //-R
                        if(br == 1){
                          //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -R ya utilizado \n");
                          printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -R ya utilizado  \n");


                        }else {
                            br = 1;
                        }
                    }else if( parametro == 15 ){
                        //-P
                        if(bp == 1){
                        //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -P ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -P ya utilizado  \n");

                        }else{
                            bp = 1;
                        }
                    }else if( parametro == 16 ){
                        //-cont=X
                        if(bcont == 1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Parametro -cont= ya utilizado \n");
                        }else{
                            bcont = 1;
                            bpathactive = -1;
                            strcpy(cont,aux);
                        }
                    }else if( parametro == 17 ){
                        //-fileN=X
                        if(bfilen == 0){
                            bfilen = 1;
                        }
                        if(nfiles < 25){
                            char auxNumero[999] = {0};
                            for( int x = 0 ; x < strlen(fileNAux) ; x++ ){
                                if(fileNAux[x] == '0' || fileNAux[x] == '1' || fileNAux[x] == '2' || fileNAux[x] == '3' || fileNAux[x] == '4' || fileNAux[x] == '5' || fileNAux[x] == '6' || fileNAux[x] == '7' || fileNAux[x] == '8' || fileNAux[x] == '9'){
                                    sprintf(auxNumero,"%s%c",auxNumero,fileNAux[x]);
                                }
                            }
                            filesN[nfiles].n = atoi(auxNumero);
                            strcpy(filesN[nfiles].path,aux);
                            nfiles++;
                            memset(&fileNAux,0,999);
                            bpathactive = -1;
                        }else {
                         // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Maximo numero de -fileN=\n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Maximo numero de -fileN=\n");

                        }
                    }else if( parametro == 18){
                        //-dest=X
                        if(bdest == 1){
                          // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -dest= ya utilizado \n");
                           printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -dest= ya utilizado \n");

                        }else{
                            bdest = 1;
                            bpathactive = -1;
                            strcpy(dest,aux);
                        }
                    }else if( parametro == 19){
                        if(bruta == 1){
                           // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -ruta= ya utilizado \n");
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -ruta= ya utilizado \n");

                        }else{
                            bruta = 1;
                            bpathactive = -1;
                            strcpy(ruta,aux);
                        }
                    }else{
                      // printf("\033[93m PROCESANDO . . . \033[0m");
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m:  PROCESANDO . . . \n");

                    }
                    parametro = -1;
                    *aux = '\0';
                }
            }
            if( entrada[i] == 35){
                //Eliminar contenido de comentario #
                break;
            }
        }else{
            //Comando fue ingresado
            bcomando = 1;
            if( entrada[i] == 34){
                //Activar bandera de comillas
                bcomilla = 1;
            }else{
                //Concatenar todo
                strcat(aux,aux_char);
            }
            if( entrada[i] == 61){
                // -PARAMETRO=
                //Parametro se considera completo
                //Asignar tipo de parametro para abstraer dato
                if( strcasecmp(aux,"-size=") == 0 ){
                    parametro = 0;
                }else if( strcasecmp(aux , "-unit=") == 0){
                    parametro = 1;
                }else if( strcasecmp(aux , "-path=") == 0){
                    bpathactive = 1;
                    parametro = 2;
                }else if( strcasecmp(aux , "-type=") == 0){
                    parametro = 3;
                }else if( strcasecmp(aux , "-fit=") == 0){
                    parametro = 4;
                }else if( strcasecmp(aux , "-delete=") == 0){
                    parametro = 5;
                }else if( strcasecmp(aux , "-name=") == 0){
                    bnameactive = 1;
                    parametro = 6;
                }else if( strcasecmp(aux , "-add=") == 0){
                    parametro = 7;
                }else if( strcasecmp(aux , "-id=") == 0){
                    parametro = 8;
                }
                /*
                else if( strcasecmp(aux, "-fs=") == 0){
                    parametro = 9;
                }
                */
                else if( strcasecmp(aux,"-usr=") == 0){
                    bnameactive = 1;
                    parametro = 10;
                }else if( strcasecmp(aux,"-pwd=") == 0){
                    bnameactive = 1;
                    parametro = 11;
                }else if( strcasecmp(aux,"-grp=") == 0){
                    bnameactive = 1;
                    parametro = 12;
                }else if( strcasecmp(aux,"-ugo=") == 0){
                    parametro = 13;
                }else if( strcasecmp(aux,"-r") == 0){
                    parametro = 14;
                }else if( strcasecmp(aux,"-p") == 0){
                    parametro = 15;
                }else if( strcasecmp(aux,"-cont=") == 0 ){
                    bpathactive = 1;
                    parametro = 16;
                }else if( !regexec(&regex,aux,0,NULL,0) ){
                    strcpy(fileNAux,aux);
                    bpathactive = 1;
                    parametro = 17;
                }else if( strcasecmp(aux,"-file=") == 0){
                    strcpy(fileNAux,aux);
                    bpathactive = 1;
                    parametro = 17;
                }
                else if( strcasecmp(aux,"-dest=") == 0 ){
                    bpathactive = 1;
                    parametro = 18;
                }else if( strcasecmp(aux,"-ruta=") == 0 ){
                    bpathactive = 1;
                    parametro = 19;
                }else{
                    parametro = -1;
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro '%s' no reconocido.\n" , aux);
                    return 0;
                }
                bguion = 0;
                bdosPuntos = 0;
                *aux = '\0';
            }else if( strcasecmp(aux,"-r") == 0){
                parametro = 14;
            }else if( strcasecmp(aux,"-p") == 0){
                parametro = 15;
            }
        }
        i++;
    }
    //Existe comando a ejecutar
    regfree(&regex);
    if(comandoausar == 31){
        //Loss
        if(bid == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -size= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -size= \n");

                }
                if(bunit == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -unit= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -path= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -path= \n");

                }
                if(btype == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -type= \n");
                      printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -type= \n");

                }
                if(bfit == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fit= \n");
                      printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fit= \n");

                }
                if(bdelete == 1){
                 //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -delete= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -delete= \n");

                }
                if(bname == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -name= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                 //   printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -add= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -add= \n");

                }
                if(bfs == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fs= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fs= \n");

                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -usr= \n");

                //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -pwd= \n");
                 // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                 // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -grp= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -ugo= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -r \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -r \n");
                }
                if(bp == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -p \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -p  \n");

                }
                if(bcont == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -cont= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -cont=  \n");

                }
                if(bfilen == 1){
                 // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fileN= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -fileN= \n");

                }
                if(bdest == 1){
                 // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -dest= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -dest= \n");

                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Loss no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        PMOUNT tempParticion;
                        tempParticion.mount_start = -1;
                        tempParticion.mount_size = -1;
                        char tempDisco[999] = {0};
                        for ( int a = 0 ; a < 26 ; a++ ) {
                            if(discosMontados[a].letra == -1){
                                continue;
                            }
                            if(discosMontados[a].letra != -1){
                                for ( int b = 0 ; b < 10 ; b++ ) {
                                    if(discosMontados[a].particiones_disco[b].mount_start == -1){
                                        continue;
                                    }
                                    if(discosMontados[a].particiones_disco[b].mount_start != -1){
                                        if(strcmp(discosMontados[a].particiones_disco[b].mount_id,id) == 0){
                                            strcpy(tempDisco,discosMontados[a].path);
                                            tempParticion = discosMontados[a].particiones_disco[b];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if(tempParticion.mount_start != -1){
                            if(strcmp(usuariologueadop.partUser.mount_id,tempParticion.mount_id) == 0){
                                if(verificacion() == 1){
                                    ejecutarSYSLOSS(tempDisco, tempParticion);
                                }
                            }else{
                                     printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No puede realizar LOSS en  sistema de archivos fuera de la particion en la cual esta ingresado \n");
                               // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No puede realizar LOSS en  sistema de archivos fuera de la particion en la cual esta ingresado\n");
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se encontro particion \n");

                         //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se encontro particion\n");
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede simular la perdida sistemas de archivos en particiones \n");
                    }
                }else{
                    // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: LOSS requiere un user ingresado en particion \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: LOSS requiere un user ingresado en particion \n");

                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando LOSS requiere parametro -id=VALOR \n");
            }
        }
    }else if(comandoausar == 30){
        //Recovery
        if(bid == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -size= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -unit= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -unit= \n");

                }
                if(bpath == 1 ){
               //     printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -path= \n");
                      printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -path= \n");

                }
                if(btype == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -type= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -type= \n");

                }

                if(bfit == 1){
                    //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fit= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fit= \n");

                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -delete=  \n");
                }
                if(bname == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -name= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -name=  \n");

                }
                if(badd == 1 ){
                    // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -add= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -add=   \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fs=   \n");

                    //printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -usr=  \n");
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -pwd= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -pwd=  \n");

                }
                if(bgrp == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -grp= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -grp=   \n");


                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -ugo= \n");

                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -r \n");
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -r \n");
                }
                if(bp == 1){
                  //  printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -p \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -p \n");

                }
                if(bcont == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -cont= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -cont= \n");

                }
                if(bfilen == 1){
                   // printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fileN= \n");
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -fileN= \n");

                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Recovery no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        PMOUNT tempParticion;
                        tempParticion.mount_start = -1;
                        tempParticion.mount_size = -1;
                        char tempDisco[999] = {0};
                        for ( int a = 0 ; a < 26 ; a++ ) {
                            if(discosMontados[a].letra == -1){
                                continue;
                            }
                            if(discosMontados[a].letra != -1){
                                for ( int b = 0 ; b < 10 ; b++ ) {
                                    if(discosMontados[a].particiones_disco[b].mount_start == -1){
                                        continue;
                                    }
                                    if(discosMontados[a].particiones_disco[b].mount_start != -1){
                                        if(strcmp(discosMontados[a].particiones_disco[b].mount_id,id) == 0){
                                            strcpy(tempDisco,discosMontados[a].path);
                                            tempParticion = discosMontados[a].particiones_disco[b];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if(tempParticion.mount_start != -1){
                            if(strcmp(usuariologueadop.partUser.mount_id,tempParticion.mount_id) == 0){
                                ejecutarRECOVERY(tempDisco, tempParticion);
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No puede realizar RECOVERY en  sistema de archivos fuera de la particion en la cual esta ingresado\n");
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se encontro particion\n");
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede restaurar sistemas de archivos en particiones \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RECOVERY requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando RECOVERY requiere parametro -id=VALOR\n");
            }
        }
    }else if(comandoausar == 29){
        //Convert
        if(bid == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -add= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Convert no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        PMOUNT tempParticion;
                        tempParticion.mount_start = -1;
                        tempParticion.mount_size = -1;
                        char tempDisco[999] = {0};
                        for ( int a = 0 ; a < 26 ; a++ ) {
                            if(discosMontados[a].letra == -1){
                                continue;
                            }
                            if(discosMontados[a].letra != -1){
                                for ( int b = 0 ; b < 10 ; b++ ) {
                                    if(discosMontados[a].particiones_disco[b].mount_start == -1){
                                        continue;
                                    }
                                    if(discosMontados[a].particiones_disco[b].mount_start != -1){
                                        if(strcmp(discosMontados[a].particiones_disco[b].mount_id,id) == 0){
                                            strcpy(tempDisco,discosMontados[a].path);
                                            tempParticion = discosMontados[a].particiones_disco[b];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if(tempParticion.mount_start != -1){
                            if(strcmp(usuariologueadop.partUser.mount_id,tempParticion.mount_id) == 0){
                                ejecutarCONVERT(tempParticion, tempDisco);
                            }else{
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No puede cambiar sistema de archivos fuera de la particion en la cual esta ingresado\n");
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No se encontro particion\n");
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede convertir sistemas de archivos en particiones \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: CONVERT requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CONVERT requiere parametro -id=VALOR\n");
            }
        }
    }else if(comandoausar == 28){
        //Pausar ejecucion
        if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
            if(bsize == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -size= \n");
            }
            if(bunit == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -unit= \n");
            }
            if(bpath == 1 ){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -path= \n");
            }
            if(btype == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -type= \n");
            }
            if(bfit == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -fit= \n");
            }
            if(bdelete == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -delete= \n");
            }
            if(bname == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -name= \n");
            }
            if(badd == 1 ){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -add= \n");
            }
            if(bid == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -id= \n");
            }
            if(bfs == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -fs= \n");
            }
            if(busr == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -usr= \n");
            }
            if(bpwd == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -pwd= \n");
            }
            if(bgrp == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -grp= \n");
            }
            if(bugo == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -ugo= \n");
            }
            if(br == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -r \n");
            }
            if(bp == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -p \n");
            }
            if(bcont == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -cont= \n");
            }
            if(bfilen == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -fileN= \n");
            }
            if(bdest == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -dest= \n");
            }
            if(bruta == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Pause no utiliza parametro -ruta= \n");
            }
        }else{
            verificacion();
        }
    }else if(comandoausar == 27){
        //Chgrp
        if(busr == 1 && bgrp == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || bpwd == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -fs= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -pwd= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chgrp no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        if(strlen(name) <= 10 && strlen(grp) <= 10){
                            FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                            if(archivoDisco != NULL){
                                ejecutarCHGRP(usr,grp,archivoDisco);
                                fclose(archivoDisco);
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: datos muy extensos (10 limite)\n");
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede cambiar users de grupo \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: CHGRP requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHGRP requiere parametro -usr=VALOR\n");
            }
            if(bgrp == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHGRP requiere parametro -grp=VALOR\n");
            }
        }
    }else if(comandoausar == 26){
        //Chown
        if(bpath == 1 && busr == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -fs= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -ugo= \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chown no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    int existeuser = -1;
                    char usuaroChown[999] = {0};
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        findUser(archivoDisco);
                        for(int a = 0; a < sizeof(globusers); a++ ){
                            if(strcmp(globusers[a].name,usr)== 0 && globusers[a].numGrupo > 0){
                                size = globusers[a].numGrupo;
                                strcpy(usuaroChown,globusers[a].name);
                                existeuser = 1;
                                break;
                            }
                        }
                        if(existeuser == 1){
                            exitoArchivo = true;
                            ejecutarARCHIVO(path,size,"",0,br,11,"",archivoDisco);
                            if(exitoArchivo){
                                SuperBloque sp_aux;
                                sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                    if(archivoDisco != NULL){
                                        //Actualizar sp y bms
                                        escribirBitMap(archivoDisco,sp_aux);
                                        actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                        journal entrada;
                                        entrada = initJournal(entrada);

                                        entrada.j_uid = usuariologueadop.id_user;

                                        time_t tiempoActual = time(NULL);
                                        struct tm *fecha = localtime(&tiempoActual);
                                        strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                        strcpy(entrada.j_name, path);
                                        sprintf(entrada.j_content,"%s@%i",usuaroChown,br);

                                        entrada.j_perm = 664;
                                        if(verificarExtension2(path) == 0){
                                            entrada.j_type = '1';
                                        }else{
                                            entrada.j_type = '0';
                                        }
                                        entrada.j_op_type = 'h';

                                        int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                        writeJournal(entrada, archivoDisco, nextJor);
                                    }
                                }
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: user '%s' no existe en particion\n",usr);
                        }
                        fclose(archivoDisco);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: CHOWN requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bpath == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHOWN requiere parametro -path=VALOR\n");
            }
            if(busr == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHOWN requiere parametro -usr=VALOR\n");
            }
        }
    }else if(comandoausar == 25){
        //Find
        if(bpath == 1 && bname == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bdest == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -delete= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -dest= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -fileN= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Find no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    char auxExpresion[999] = {0};
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        if(strcmp(name,"*") == 0){
                            sprintf(auxExpresion,"%s.%s","([a-zA-Z0-9])+","([a-zA-Z0-9])+");
                        }else{
                            for(int a = 0 ; a < strlen(name) ; a++ ){
                                if(name[a] == '?'){
                                    sprintf(auxExpresion,"%s%s",auxExpresion,"([a-zA-Z0-9])");
                                }else if(name[a] == '*'){
                                    sprintf(auxExpresion,"%s%s",auxExpresion,"([a-zA-Z0-9])+");
                                }else{
                                    sprintf(auxExpresion,"%s%c",auxExpresion,name[a]);
                                }
                            }
                        }
                        //Comprobante de que la exresion funcionara
                        regex_t expresion;
                        int estadoExpresion = -1;
                        estadoExpresion = regcomp(&expresion,auxExpresion,REG_EXTENDED);
                        if(estadoExpresion){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: expresion a buscar con Find erronea\n");
                            return 0;
                        }
                        if(verificarExtension(path,".txt") == 0 ){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: -path= de comando FIND debe ser dirigir a una carpeta\n");
                            return 0;
                        }
                        ejecutarARCHIVO(path,0,"",0,0,10,auxExpresion,archivoDisco);
                        fclose(archivoDisco);
                    }
                }else {
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FIND requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando FIND requiere parametro -path=VALOR\n");
            }
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando FIND requiere parametro -name=VALOR\n");
            }
        }
    }else if(comandoausar == 24){
        //Mv
        if(bpath == 1 && bdest == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -fileN= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    exitoArchivo = true;
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        ejecutarARCHIVO(path,0,"",0,0,8,dest,archivoDisco);
                        if(exitoArchivo){
                            SuperBloque sp_aux;
                            sp_aux = getSuperBlock(usuariologueadop.partUser, archivoDisco);
                            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                if(archivoDisco != NULL){
                                    //Actualizar sp y bms
                                    escribirBitMap(archivoDisco,sp_aux);
                                    actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                    journal entrada;
                                    entrada = initJournal(entrada);

                                    entrada.j_uid = usuariologueadop.id_user;

                                    time_t tiempoActual = time(NULL);
                                    struct tm *fecha = localtime(&tiempoActual);
                                    strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                    strcpy(entrada.j_name, path);
                                    strcpy(entrada.j_content, dest);

                                    entrada.j_perm = 664;
                                    if(verificarExtension2(path) == 0){
                                        entrada.j_type = '1';
                                    }else{
                                        entrada.j_type = '0';
                                    }
                                    entrada.j_op_type = 'g';

                                    int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                    writeJournal(entrada, archivoDisco, nextJor);
                                }
                            }
                        }
                        fclose(archivoDisco);
                    }
                }else {
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mv requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bpath == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MV requiere parametro -path=VALOR\n");
            }
            if(bdest == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MV requiere parametro -dest=VALOR\n");
            }
        }
    }else if(comandoausar == 23){
        //Cp
        if(bpath == 1 && bdest == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -fileN= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    int real = -1;
                    char temp[99] = {0};
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        if(verificarExtension2(path) == 0 && verificarExtension2(dest) == 0){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: no se puede copiar un archivo dirigido a otro archivo\n");
                            real = 1;
                        }
                        if(verificarExtension2(path) == 0 && verificarExtension2(dest) != 0){
                            int auxTemp = 0;
                            for(int x = 0; path[x] != NULL ; x++ ){
                                temp[auxTemp] = path[x];
                                auxTemp++;
                                if(path[x] == '/'){
                                    memset(&temp,0,99);
                                    auxTemp = 0;
                                }
                            }
                            sprintf(dest,"%s/%s",dest,temp);
                        }
                        if(real == -1){
                            exitoArchivo = true;
                            ejecutarARCHIVO(path,1,"",0,0,7,dest, archivoDisco);
                            if(exitoArchivo){
                                SuperBloque sp_aux;
                                sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                    if(archivoDisco != NULL){
                                        //Actualizar sp y bms
                                        escribirBitMap(archivoDisco,sp_aux);
                                        actualizarSP(usuariologueadop.partUser,0, archivoDisco);

                                        journal entrada;
                                        entrada = initJournal(entrada);

                                        entrada.j_uid = usuariologueadop.id_user;

                                        time_t tiempoActual = time(NULL);
                                        struct tm *fecha = localtime(&tiempoActual);
                                        strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                        strcpy(entrada.j_name, path);
                                        strcpy(entrada.j_content, dest);

                                        entrada.j_perm = 664;
                                        if(verificarExtension2(path) == 0){
                                            entrada.j_type = '1';
                                        }else{
                                            entrada.j_type = '0';
                                        }
                                        entrada.j_op_type = 'f';

                                        int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                        writeJournal(entrada, archivoDisco, nextJor);
                                    }
                                }
                            }
                        }
                        fclose(archivoDisco);
                    }
                }else {
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cp requiere un user ingresado en particion \n");
                }
            }
        }else {
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CP requiere parametro -path=VALOR\n");
            }
            if(bdest == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CP requiere parametro -dest=VALOR\n");
            }
        }
    }else if(comandoausar == 22){
        //Mkdir
        if(bpath == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -r \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    exitoArchivo = true;
                    strcat(path,"/x.txt");
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        ejecutarARCHIVO(path,1,"",0,bp,1,"",archivoDisco);
                        if(exitoArchivo){
                            SuperBloque sp_aux;
                            sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                if(archivoDisco != NULL){
                                    //Actualizar sp y bms
                                    escribirBitMap(archivoDisco,sp_aux);
                                    actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                    journal entrada;
                                    entrada = initJournal(entrada);

                                    entrada.j_uid = usuariologueadop.id_user;

                                    time_t tiempoActual = time(NULL);
                                    struct tm *fecha = localtime(&tiempoActual);
                                    strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                    strcpy(entrada.j_name, path);
                                    sprintf(entrada.j_content, "%i",bp);

                                    entrada.j_perm = 664;
                                    entrada.j_type = '0';
                                    entrada.j_op_type = 'e';

                                    int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                    writeJournal(entrada, archivoDisco, nextJor);
                                }
                            }
                        }
                        fclose(archivoDisco);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkdir requiere un user ingresado en particion \n");
                }
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKDIR requiere parametro -path=VALOR\n");
        }
    }else if(comandoausar == 21){
        //Ren
        if(bpath == 1 && bname == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -delete= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(strlen(name) <= 10){
                        exitoArchivo = true;
                        FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarARCHIVO(path,0,"",0,0,6,name,archivoDisco);
                            if(exitoArchivo){
                                SuperBloque sp_aux;
                                sp_aux = getSuperBlock( usuariologueadop.partUser,archivoDisco);
                                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                    if(archivoDisco != NULL){
                                        //Actualizar sp y bms
                                        escribirBitMap(archivoDisco,sp_aux);
                                        actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                        journal entrada;
                                        entrada = initJournal(entrada);

                                        entrada.j_uid = usuariologueadop.id_user;

                                        time_t tiempoActual = time(NULL);
                                        struct tm *fecha = localtime(&tiempoActual);
                                        strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                        strcpy(entrada.j_name, path);
                                        strcpy(entrada.j_content, name);

                                        entrada.j_perm = 664;
                                        if(verificarExtension2(path) == 0){
                                            entrada.j_type = '1';
                                        }else{
                                            entrada.j_type = '0';
                                        }
                                        entrada.j_op_type = 'd';

                                        int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                        writeJournal(entrada, archivoDisco, nextJor);
                                    }
                                }
                            }
                            fclose(archivoDisco);
                        }
                    }else {
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: nombre nuevo no debe exeder los 10 caracteres\n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Ren requiere un user ingresado en particion \n");
                }
            }
        }else {
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REN requiere parametro -path=VALOR\n");
            }
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REN requiere parametro -name=VALOR\n");
            }
        }
    }else if(comandoausar == 20){
        //Edit
        if(bpath == 1 && (bsize == 1 || bcont == 1)){
            if( bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -r \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Edit no utiliza parametro -ruta= \n");
                }
            }else{
                if(bcont == 1 && bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Comando Edit no puede aceptar -cont= y -size= juntos\n");
                }else {
                    if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                        exitoArchivo = true;
                        FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarARCHIVO(path,size,cont,bcont,bp,5,"",archivoDisco);
                            if(exitoArchivo){
                                SuperBloque sp_aux;
                                sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                    if(archivoDisco != NULL){
                                        //Actualizar sp y bms
                                        escribirBitMap(archivoDisco,sp_aux);
                                        actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                        journal entrada;
                                        entrada = initJournal(entrada);

                                        entrada.j_uid = usuariologueadop.id_user;

                                        time_t tiempoActual = time(NULL);
                                        struct tm *fecha = localtime(&tiempoActual);
                                        strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                        strcpy(entrada.j_name, path);
                                        sprintf(entrada.j_content,"%i@%s@%i",size,cont,bp);

                                        entrada.j_perm = 664;
                                        entrada.j_type = '1';
                                        entrada.j_op_type = 'c';

                                        int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                        writeJournal(entrada, archivoDisco, nextJor);
                                    }
                                }
                            }
                            fclose(archivoDisco);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Debe de estar ingresado para poder manipular archivos o carpetas\n");
                    }
                }
            }
        }else {
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando Edit requiere el parametro -path=VALOR \n");
            }
            if(bsize == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando Edit requiere el parametro -size=VALOR \n");
            }
            if(bcont == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando Edit requiere el parametro -cont=VALOR \n");
            }
        }
    }else if(comandoausar == 19){
        //Rem
        if(bpath == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rem no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    exitoArchivo = true;
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        ejecutarARCHIVO(path,0,"",0,0,3,"",archivoDisco);
                        if(exitoArchivo){
                            SuperBloque sp_aux;
                            sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                if(archivoDisco != NULL){
                                    //Actualizar sp y bms
                                    escribirBitMap(archivoDisco,sp_aux);
                                    actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                    journal entrada;
                                    entrada = initJournal(entrada);

                                    entrada.j_uid = usuariologueadop.id_user;

                                    time_t tiempoActual = time(NULL);
                                    struct tm *fecha = localtime(&tiempoActual);
                                    strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                    strcpy(entrada.j_name, path);
                                    strcpy(entrada.j_content, "");

                                    entrada.j_perm = 664;
                                    if(verificarExtension2(path) == 0){
                                        entrada.j_type = '1';
                                    }else{
                                        entrada.j_type = '0';
                                    }
                                    entrada.j_op_type = 'b';

                                    int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                    writeJournal(entrada, archivoDisco, nextJor);
                                }
                            }
                        }
                        fclose(archivoDisco);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Debe de estar ingresado para poder manipular archivos o carpetas\n");
                }
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REM requiere parametro -path=VALOR \n");
        }
    }else if(comandoausar == 18){
        //Cat (Mostrar contenido de archivo)
        if(bfilen == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -cont= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Cat no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        for(int x = 1 ; x < 25 ; x++ ){
                            for( int y = 0 ; y < 25 - x ; y++){
                                if(filesN[y].n > filesN[y+1].n && filesN[y+1].n != -1 || filesN[y].n == -1){
                                    archivoN auxOrden = filesN[y+1];
                                    filesN[y+1] = filesN[y];
                                    filesN[y] = auxOrden;
                                }
                            }
                        }
                        if(nfiles > 0){
                            for (int x = 0 ; x < nfiles ; x++ ) {
                                ejecutarARCHIVO(filesN[x].path,0,"",0,0,2,"",archivoDisco);
                            }
                        }else {
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No hay archivos para imprimir\n");
                        }
                        fclose(archivoDisco);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Debe de estar ingresado para poder manipular archivos o carpetas\n");
                }
            }
        }else{
            if(bfilen == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CAT requiere parametro(s) -file=VALOR \n");
            }
        }
        printf(" \n");

    }else if(comandoausar == 17){
        //Mkfile
        if(bpath == 1 && (bsize == 1 || bcont == 1)){
            if( bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -r \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfile no utiliza parametro -ruta= \n");
                }
            }else{
                if(bcont == 1 && bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Comando MKFile no puede aceptar -cont= y -size= juntos\n");
                }else {
                    if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                        exitoArchivo = true;
                        FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarARCHIVO(path,size,cont,bcont,bp,0,"",archivoDisco);
                            if(exitoArchivo){
                                SuperBloque sp_aux;
                                sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                                if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                    if(archivoDisco != NULL){
                                        //Actualizar sp y bms
                                        escribirBitMap(archivoDisco,sp_aux);
                                        actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                        journal entrada;
                                        entrada = initJournal(entrada);

                                        entrada.j_uid = usuariologueadop.id_user;

                                        time_t tiempoActual = time(NULL);
                                        struct tm *fecha = localtime(&tiempoActual);
                                        strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                        strcpy(entrada.j_name, path);
                                        sprintf(entrada.j_content, "%i@%s@%i",size,cont,bp);

                                        entrada.j_perm = 664;
                                        entrada.j_type = '1';
                                        entrada.j_op_type = 'a';

                                        int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                        writeJournal(entrada, archivoDisco, nextJor);
                                    }
                                }
                            }
                            fclose(archivoDisco);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Debe de estar ingresado para poder manipular archivos o carpetas\n");
                    }
                }
            }
        }else{
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKFile requiere el parametro -path=VALOR \n");
            }
            if(bsize == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKFile requiere el parametro -size=VALOR \n");
            }
            if(bcont == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKFile requiere el parametro -cont=VALOR \n");
            }
        }
    }else if(comandoausar == 16){
        //Chmod
        if(bpath == 1 && bugo == 1 ){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -grp= \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Chmod no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    exitoArchivo = true;
                    FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                    if(archivoDisco != NULL){
                        ejecutarARCHIVO(path,ugo_int,"",0,br,4,"",archivoDisco);
                        if(exitoArchivo){
                            SuperBloque sp_aux;
                            sp_aux = getSuperBlock(usuariologueadop.partUser,archivoDisco);
                            if(sp_aux.sb_magic == 61267 && sp_aux.sb_filesystem_type == 3 && noRecovery){
                                if(archivoDisco != NULL){
                                    //Actualizar sp y bms
                                    escribirBitMap(archivoDisco,sp_aux);
                                    actualizarSP(usuariologueadop.partUser,0,archivoDisco);

                                    journal entrada;
                                    entrada = initJournal(entrada);

                                    entrada.j_uid = usuariologueadop.id_user;

                                    time_t tiempoActual = time(NULL);
                                    struct tm *fecha = localtime(&tiempoActual);
                                    strftime(entrada.j_date, sizeof(entrada.j_date)-1, "%d/%m/%y %H:%M", fecha);

                                    strcpy(entrada.j_name, path);
                                    sprintf(entrada.j_content, "%i@%i",ugo_int,br);

                                    entrada.j_perm = 664;
                                    if(verificarExtension2(path) == 0){
                                        entrada.j_type = '1';
                                    }else{
                                        entrada.j_type = '0';
                                    }
                                    entrada.j_op_type = 'z';

                                    int nextJor = nextJournal( archivoDisco , sp_aux.sb_bm_inode );
                                    writeJournal(entrada, archivoDisco, nextJor);
                                }
                            }
                        }
                        fclose(archivoDisco);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Debe de estar ingresado para poder manipular archivos o carpetas\n");
                }
            }
        }else{
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHMOD requiere el parametro -path=VALOR \n");
            }
            if(bugo == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando CHMOD requiere el parametro -ugo=VALOR \n");
            }
        }
    }else if(comandoausar == 15){
        //Eliminar user en particion
        if(busr == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || bname == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -delete= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -fs= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -name= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmusr no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1 && strcasecmp(usr,"root") != 0){
                        FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarRMUSR(usr,archivoDisco); //JOURNAL dentro de metodo
                            fclose(archivoDisco);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede remover grupos \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMUSR requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando RMUSR requiere el parametro -usr=VALOR \n");
            }
        }
    }else if(comandoausar == 14){
        //Generar users en particion
        if(busr == 1 && bpwd == 1 && bgrp == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || bname == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -delete= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -fs= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -name= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkusr no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        if(strlen(usr) <= 10 && strlen(pwd) <= 10 && strlen(grp) <= 10 && strcasecmp(grp,"root") != 0){
                            FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                            if(archivoDisco != NULL){
                                ejecutarMKUSR(usr,pwd,grp,archivoDisco); //JOURNAL dentro de metodo
                                fclose(archivoDisco);
                            }
                        }else{
                            if(strlen(usr) > 10){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Size de user muy extenso (10 limite)\n");
                            }
                            if(strlen(pwd) > 10){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Size de Password muy extenso (10 limite)\n");
                            }
                            if(strlen(grp) > 10){
                                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Size de Group muy extenso (10 limite)\n");
                            }
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede generar users \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKUSR requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKUSR requiere el parametro -usr=VALOR \n");
            }
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKUSR requiere el parametro -pwd=VALOR \n");
            }
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKUSR requiere el parametro -grp=VALOR \n");
            }
        }
    }else if(comandoausar == 13){
        //Eliminar grupos en particion
        if(bname == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -delete= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rmgrp no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1 && strcasecmp(name,"root") != 0){
                        FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarRMGRP(name,archivoDisco); //JOURNAL dentro de metodo
                            fclose(archivoDisco);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede remover grupos \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMGRP requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando RMGRP requiere el parametro -Name=VALOR \n");
            }
        }
    }else if(comandoausar == 12){
        //Generar grupos en particion
        if(bname == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -delete= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkgrp no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    if(usuariologueadop.id_user == 1){
                        if(strlen(name) <= 10 && strcasecmp(name,"root") != 0){
                            FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                            if(archivoDisco != NULL){
                                ejecutarMKGRP(name,archivoDisco); //JOURNAL dentro de metodo
                                fclose(archivoDisco);
                            }
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Nombre de grupo muy extenso (10 limite)\n");
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Solo user Root puede generar grupos \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKGRP requiere un user ingresado en particion \n");
                }
            }
        }else{
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKGRP requiere el parametro -Name=VALOR \n");
            }
        }
    }else if(comandoausar == 11){
        //Logout de user en particion
        if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Logout no utiliza parametro -ruta= \n");
                }
            }else{
                FILE *archivoDisco = fopen(usuariologueadop.pathDisco,"rb+");
                if(archivoDisco != NULL){
                    actualizarSP(usuariologueadop.partUser,0,archivoDisco);
                    fclose(archivoDisco);
                }
                ejecutarLOGOUT();
                printf("\033[1;92mGeremiasLS \033[1;97m/: \033[37;42m SUCCESS \033[0m user egresado \n");
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No hay user ingresado \n");
        }
    }else if(comandoausar == 10){
        //Login de user en particion
        if(bid == 1 && busr == 1 && bpwd == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bfs == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -add= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -fs= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Login no utiliza parametro -ruta= \n");
                }
            }else{
                int existeParticion = -1;
                PMOUNT particionWork;
                memset(particionWork.mount_id,0,5);
                particionWork.mount_fit = 'x';
                memset(particionWork.mount_name,0,16);
                particionWork.mount_size = -1;
                particionWork.mount_type = 'x';
                particionWork.mount_start = -1;
                particionWork.mount_status = 'x';

                char pathDisco[999] = {0};
                for(int i = 0; i < 26 ; i++){
                    for(int j = 0; j < 10; j++){
                        if(strcmp(id,discosMontados[i].particiones_disco[j].mount_id) == 0){
                            particionWork = discosMontados[i].particiones_disco[j];
                            strcpy(pathDisco,discosMontados[i].path);
                            existeParticion = 1;
                            break;
                        }
                    }
                    if(existeParticion == 1){
                        break;
                    }
                }
                if(existeParticion == 1){
                    if(usuariologueadop.id_user == -1 && usuariologueadop.id_grupo == -1){
                        FILE *archivoDisco = fopen(pathDisco,"rb+");
                        if(archivoDisco != NULL){
                            ejecutarLOGIN(usr,pwd,pathDisco,particionWork,archivoDisco);
                            actualizarSP(usuariologueadop.partUser,0,archivoDisco);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: user ingresado en particion: '%s' de disco: '%s' (USR: %s)", usuariologueadop.namePartition, usuariologueadop.pathDisco, usuariologueadop.name);
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion a trabajar no esta montada \n");
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando LOGIN requiere el parametro -Id=VALOR \n");
            }
            if(busr == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando LOGIN requiere el parametro -usr=VALOR \n");
            }
            if(bpwd == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando LOGIN requiere el parametro -pwd=VALOR \n");
            }
        }
    }else if(comandoausar == 9){
        //Generar sistema de archivos
        if(bid == 1){
            if(bsize == 1 || bunit == 1 || bpath == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -unit= \n");
                }
                if(bpath == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -path= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -name= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -add= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKFS no utiliza parametro -ruta= \n");
                }
            }else{
                if(btype == 0 ){
                    type = 'c';
                }
                if(bfs == 0){
                    fs = 2;
                }
                if(type == 'c' || type == 'r'){
                    int existeParticion = -1;
                    PMOUNT particionWork;
                    memset(particionWork.mount_id,0,5);
                    particionWork.mount_fit = 'x';
                    memset(particionWork.mount_name,0,16);
                    particionWork.mount_size = -1;
                    particionWork.mount_type = 'x';
                    particionWork.mount_start = -1;
                    particionWork.mount_status = 'x';

                    char pathDisco[999] = {0};
                    for(int i = 0; i < 26 ; i++){
                        for(int j = 0; j < 10; j++){
                            if(strcmp(id,discosMontados[i].particiones_disco[j].mount_id) == 0){
                                particionWork = discosMontados[i].particiones_disco[j];
                                strcpy(pathDisco,discosMontados[i].path);
                                existeParticion = 1;
                                break;
                            }
                        }
                        if(existeParticion == 1){
                            break;
                        }
                    }
                    if(existeParticion == 1){
                        if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: por favor, egrese del sistema antes de formatear una particion\n");
                        }else{
                            ejecutarMKFS(pathDisco,particionWork,type,3);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Particion a trabajar no esta montada \n");
                    }
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mkfs parametro -Type= solo puede ser 2fs | 3fs\n");
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando MKFS requiere el parametro -Id=VALOR \n");
            }
        }
    }else if(comandoausar == 8){
        //Mostrar particiones montadas
        if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
            if(bsize == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -size= \n");
            }
            if(bunit == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -unit= \n");
            }
            if(bpath == 1 ){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -path= \n");
            }
            if(btype == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -type= \n");
            }
            if(bfit == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -fit= \n");
            }
            if(bdelete == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -delete= \n");
            }
            if(bname == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -name= \n");
            }
            if(badd == 1 ){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -add= \n");
            }
            if(bid == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -id= \n");
            }
            if(bfs == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -fs= \n");
            }
            if(busr == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -usr= \n");
            }
            if(bpwd == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -pwd= \n");
            }
            if(bgrp == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -grp= \n");
            }
            if(bugo == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -ugo= \n");
            }
            if(br == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -r \n");
            }
            if(bp == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -p \n");
            }
            if(bcont == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -cont= \n");
            }
            if(bfilen == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -fileN= \n");
            }
            if(bdest == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -dest= \n");
            }
            if(bruta == 1){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: ShowMount no utiliza parametro -ruta= \n");
            }
        }else{
            ejecutarShowMount();
        }
    }else if( comandoausar == 7 ){
        //Generar reporte
        if(bid == 1 && bpath == 1 && bname == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 ){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -delete= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -add= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Rep no utiliza parametro -dest= \n");
                }
            }else{
                //Verificar existencia de particion montada
                int existe_disco = -1;
                char pathDisco[999] = {0};
                for ( int i = 0 ; i < 26 ; i++ ) {
                    //Recorrer Discos
                    if(discosMontados[i].letra != -1){
                        //Existe disco
                        for ( int j = 0 ; j < 10 ; j++ ) {
                            //Recorrer Particiones
                            if(discosMontados[i].particiones_disco[j].mount_start != -1){
                                //Existe particion montada
                                if(strcmp(discosMontados[i].particiones_disco[j].mount_id,id)==0){
                                    //Particion para hacer reporte existe
                                    strcpy(pathDisco,discosMontados[i].path);
                                    existe_disco = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if(existe_disco == 1){
                        break;
                    }
                }
                //Contenido si es exitosa la busqueda de particion montada
                char salidaImpresion[999] = {0};
                char extension[100] = {0};
                for( int i = 0; i < strlen(path) ; i++){
                    sprintf(extension,"%s%c",extension,path[i]);
                    if(path[i]=='.'){
                        sprintf(extension , "");
                    }
                }
                //crear Path para archivo de reporte final
                CrearPath(path);
                char pathSalida[] = "/home/edi/reporte.dot";
                sprintf(salidaImpresion,"dot -T%s %s -o %s",extension,pathSalida,path);
                int tipoRep = -1;
                if( strcasecmp(name,"mbr") == 0 ){
                    tipoRep = 1;
                }else if( strcasecmp(name,"disk") == 0 ){
                    tipoRep = 2;
                }else if( strcasecmp(name,"inode") == 0 ){
                    tipoRep = 3;
                }else if( strcasecmp(name,"journaling") == 0 ){
                    tipoRep = 4;
                }else if( strcasecmp(name,"block") == 0 ){
                    tipoRep = 5;
                }else if( strcasecmp(name,"bm_inode") == 0 ){
                    tipoRep = 6;
                }else if( strcasecmp(name,"bm_block") == 0 ){
                    tipoRep = 7;
                }else if( strcasecmp(name,"tree") == 0 ){
                    tipoRep = 8;
                }else if( strcasecmp(name,"sb") == 0 ){
                    tipoRep = 9;
                }else if( strcasecmp(name,"file") == 0 ){
                    tipoRep = 10;
                }else if( strcasecmp(name,"ls") == 0 ){
                    tipoRep = 11;
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: parametro -name=%s incorrecto \n", name);
                    return 0;
                }
                PMOUNT tempParticion;
                tempParticion.mount_start = -1;
                tempParticion.mount_size = -1;
                for ( int a = 0 ; a < 26 ; a++ ) {
                    if(discosMontados[a].letra == -1){
                        continue;
                    }
                    if(discosMontados[a].letra != -1){
                        for ( int b = 0 ; b < 10 ; b++ ) {
                            if(discosMontados[a].particiones_disco[b].mount_start == -1){
                                continue;
                            }
                            if(discosMontados[a].particiones_disco[b].mount_start != -1){
                                if(strcmp(discosMontados[a].particiones_disco[b].mount_id,id) == 0){
                                    tempParticion = discosMontados[a].particiones_disco[b];
                                    break;
                                }
                            }
                        }
                    }
                }
                FILE *archivoDisco = fopen(pathDisco,"rb+");
                if(archivoDisco != NULL){
                    findUser2(tempParticion,archivoDisco);
                    if( ejecutarRep(pathSalida, pathDisco, tipoRep, tempParticion, ruta) == 1){
                        if(existsFile(path)==1){
                            remove(path);
                        }
                        system(salidaImpresion);
                        if(existsFile(path)==1){
                            printf("\033[1;92mGeremiasLS \033[1;97m/: \033[37;42m SUCCESS \033[0m genero reporte '%s' en '%s' \n",name,path);
                        }else{
                            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No genero reporte '%s' en '%s' verificar datos \n",name,path);
                        }
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: No genero reporte '%s' en '%s' verificar datos \n",name,path);
                    }
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REP requiere el parametro -Id=VALOR \n");
            }
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REP requiere el parametro -Path=VALOR \n");
            }
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: comando REP requiere el parametro -Name=VALOR \n");
            }
        }
    }else if( comandoausar == 6 ){
        if( bid == 1 ){
            if(bsize == 1 || bunit == 1 || bpath == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -unit= \n");
                }
                if(bpath == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -path= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -name= \n");
                }
                if(badd == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -add= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: por favor, egrese del sistema antes de desmontar una particion\n");
                }else{
                    ejecutarUnmount(id);
                }
            }
        }else{
            if(bid == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Unmount requiere parametro -id= \n");
            }
        }
    }else if( comandoausar == 5 ){
        //Montar particion
        if(bpath == 1 && bname == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -unit= \n");
                }
                if(btype == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -delete= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -add= \n");
                }
                if(bid == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount no utiliza parametro -ruta= \n");
                }
            }else{
                if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: por favor, egrese del sistema antes de montar una particion\n");
                }else{
                    ejecutarMount(path, name);
                }
            }
        }else{
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount requiere parametro -path= \n");
            }
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Mount requiere parametro -name= \n");
            }
        }
    }else if( comandoausar == 4 ){
        //Ejecutar archivo
        if(bpath == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -unit= \n");
                }
                if(btype == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -name= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -add= \n");
                }
                if(bid == 1 ){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec no utiliza parametro -ruta= \n");
                }
            }else{
                ejecutarExec(path);
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: Exec requiere parametro -path= \n");
        }
    }else if( comandoausar == 3 ){
        //FDisk
        if(bpath == 1 && bname == 1){
            if(bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -Id=\n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk no utiliza parametro -ruta= \n");
                }
            }else{
                if(type == 'p' || type == 'P' || type == 'e' || type == 'E' || type == 'l' || type == 'L'){
                    if(bunit == 0){
                        unit = 1024;
                    }
                    if(btype == 0){
                        type = 'p';
                    }
                    if(bfit == 0){
                        fit = 'w';
                    }
                    size = size * unit;
                    add = add * unit;
                    //tipoFDisk = 0 => CREAR | tipoFDisk = 1 => ADD | tipoFDisk = 2 => DELETE
                    ejecutarFDisk(path,name,size,type,fit,del,add,tipoFDisk);
                }else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk parametro -Type= solo puede ser P | E | L\n");
                }
            }
        }else{
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk requiere parametro -path= \n");
            }
            if(bname == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: FDisk requiere parametro -name= \n");
            }
        }
    }else if( comandoausar == 2 ){
        //RMDisk
        if(bpath == 1){
            if(bsize == 1 || bunit == 1 || btype == 1 || bfit == 1 || bdelete == 1 || bname == 1 || badd == 1 || bid == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 || bfilen == 1 || bdest == 1 || bruta == 1){
                if(bsize == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -size= \n");
                }
                if(bunit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -unit= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -type= \n");
                }
                if(bfit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -fit= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -delete= \n");
                }
                if(bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -name= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -add= \n");
                }
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -id= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk no utiliza parametro -ruta= \n");
                }
            }else{
                if(existsFile(path) == 1){\
                    ejecutarRMDisk(path);
                }
                else{
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo no encontrado %s \n",path);
                }
            }
        }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: RMDisk requiere parametro -path= \n");
        }
    }else if( comandoausar == 1 ){
        //MKDisk
        if( bsize == 1 && bpath == 1){
            if(bid == 1 || badd == 1 || bname == 1 || bdelete == 1 || btype == 1 || bfs == 1 || busr == 1 || bpwd == 1 || bgrp == 1 || bugo == 1 || br == 1 || bp == 1 || bcont == 1 | bfilen == 1 || bdest == 1 || bruta == 1){
                if(bid == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -id= \n");
                }
                if(badd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -add= \n");
                }
                if( bname == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -name= \n");
                }
                if(bdelete == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -delete= \n");
                }
                if(btype == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -type= \n");
                }
                if(bfs == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -fs= \n");
                }
                if(busr == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -usr= \n");
                }
                if(bpwd == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -pwd= \n");
                }
                if(bgrp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -grp= \n");
                }
                if(bugo == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -ugo= \n");
                }
                if(br == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -r \n");
                }
                if(bp == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -p \n");
                }
                if(bcont == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -cont= \n");
                }
                if(bfilen == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -fileN= \n");
                }
                if(bdest == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -dest= \n");
                }
                if(bruta == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk no utiliza parametro -ruta= \n");
                }
            }else{
                if(bunit == 0){
                    unit = 1024 * 1024;
                }
                if(bfit == 0){
                    fit = 'f';
                }
                if(bunit == 1 && unit == 1){
                    printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk -unit= no puede ser bytes \n");
                }else{
                    size = size * unit;
                    if( size > 200 ){
                        ejecutarMKDisk(path,size,fit);
                    }else{
                        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk parametro -size= debe ser positivo y mayor a 0 (No Decimales) \n");
                    }
                }
            }
        }else{
            if(bsize == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk requiere parametro -size= \n");
            }
            if(bpath == 0){
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: MKDisk requiere parametro -path= \n");
            }
        }
    }else if( comandoausar == 0 ){
        if(usuariologueadop.id_user != -1 && usuariologueadop.id_grupo != -1){
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: por favor, antes de cerrar el programa, egrese del sistema \n");
            return 0;
        }else{
            return -1;
        }
    }
    //No existe else ya que tomaria saltos de linea y comentarios como errores
    return 0;
}



































int ejecutarExec(char path[]){
    int terminal = -1;
    int finalArchivo = -1;
    char input[999] = {0};
    if(existsFile(path)==1){
        if(verificarExtension(path,".sh") == 0){ //If no necesario, solo para verificar si es script de la practica
            FILE *archivoScript;
            archivoScript=fopen(path,"r");
            if(archivoScript!=NULL){
                fseek(archivoScript,0,SEEK_END);
                finalArchivo = ftell(archivoScript);
                fseek(archivoScript,0,SEEK_SET);
                while(!feof(archivoScript)){
                    //Ciclo por cada linea del archivo, hasta llegar a final de archivo
                    //Imprime dos veces la ultima cadena del archivo
                    if(terminal < finalArchivo){
                        fgets(input,999,archivoScript);
                        printf("%s",input);
                        sprintf(input , "%s \n" , input);
                        cargaComando(input);
                    }else {
                        break;
                    }
                    terminal = ftell(archivoScript);
                }
                fclose(archivoScript);
            }else{
                printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo script no se pudo abrir, revise el archivo \n");
                return -1;
            }
        }else{
            printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo script no reconocido \n");
            return -1;
        }
    }else{
        printf("\033[1;92mGeremiasLS \033[1;97m/:  \033[101m ERROR \033[0m: archivo no existe \n");
        return -1;
    }
    return 0;
}


