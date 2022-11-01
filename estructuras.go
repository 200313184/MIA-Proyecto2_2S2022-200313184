package main

type PARTITION struct {
	Part_status byte
	Part_type   byte
	Part_fit    [3]byte
	Part_start  int32
	Part_size   int32
	Part_name   [16]byte
}

type MBR struct {
	Mbr_tamano         int32
	Mbr_fecha_creacion [20]byte
	Mbr_disk_signature int32
	Disk_fit           [3]byte
	Mbr_partition      [4]PARTITION
}

type EBR struct {
	Part_status byte
	Part_fit    [3]byte
	Part_start  int32
	Part_size   int32
	Part_next   int32
	Part_name   [16]byte
}

type NODO struct {
	Path      string
	Name      string
	Letra     byte
	Numero    int32
	Contador  int32
	Id        string
	Siguiente *NODO
}

type LISTASIMPLE struct {
	Primero       *NODO
	Ultimo        *NODO
	Letra_temp    byte
	Numero_temp   int32
	Contador_temp int32
	Path          string
	Name          string
}

/*_________ SISTEMA DE ARCHIVOS _________*/
type SUPER_BLOQUE struct {
	S_filesystem_type   int32
	S_inodes_count      int32
	S_blocks_count      int32
	S_free_blocks_count int32
	S_free_inodes_count int32
	S_mtime             [20]byte
	//S_umtime            [20]byte
	S_mnt_count      int32
	S_magic          int32
	S_inode_size     int32
	S_block_size     int32
	S_first_ino      int32
	S_first_blo      int32
	S_bm_inode_start int32
	S_bm_block_start int32
	S_inode_start    int32
	S_block_start    int32
}

type TABLA_INODOS struct {
	I_uid   int32
	I_gid   int32
	I_size  int32
	I_atime [20]byte
	I_ctime [20]byte
	I_mtime [20]byte
	I_block [16]int32
	I_type  byte
	I_perm  int32
}

type CONTENT struct {
	B_name  [12]byte
	B_inodo int32
}

type BLOQUE_CARPETA struct {
	B_content [4]CONTENT
}

type BLOQUE_ARCHIVO struct {
	B_content [64]byte
}

//type BLOQUE_APUNTADOR struct {
//	B_pointers [16]int32
//}

type JOURNAL struct {
	J_operation_type [10]byte
	J_type           int32 //Archivo/Carpeta
	J_name           [100]byte
	J_content        [100]byte
	J_date           [20]byte
	J_owner          int32
	J_permissions    int32
}

/*_______________________ USUARIO CON SESION ______________________*/
type Sesion struct {
	Id_user     int32
	Id_grp      int32
	InicioSuper int32
	//InicioJournal int32
	Tipo_sistema int32
	Path         string
	Fit          [3]byte
	hay_Sesion   bool
}

type Usuario struct {
	Id_usr   int32
	Id_grp   int32
	Username [10]byte
	Password [10]byte
	Group    [10]byte
}

type Param struct {
	nombre string
	valor  string
}
