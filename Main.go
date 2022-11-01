package main

import (
	"bufio"
	"bytes"
	_ "bytes"
	"encoding/binary"
	"fmt"
	"log"
	"math"
	"math/rand"
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"strings"
	"time"
	"unsafe"
)

func main() {
	InicializarVariablesGlobales()
	leerComando() //siempre descomentar esto porque siver

}

func leerComando() {
	/*
		var bienvenida string = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n~~~~~~~~~~~ BIENVENIDO INGRESE UN COMANDO ~~~~~~~~~~~\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
		for {
			fmt.Println(bienvenida)
			reader := bufio.NewReader(os.Stdin)
			text, _ := reader.ReadString('\n')
			// convert CRLF to LF
			text = strings.Replace(text, "\n", "", -1)
			if strings.ToLower(text) == "exit" {
				break
			}
			if Verificar_Comando(text) == 0 {
				fmt.Println("RECONOCIO COMANDO CORRECTATEMTE")
			} else {
				fmt.Println("ERROR RETURN -1 EN ANALIZAR PARAMETROS")
			}
		}
		//exec -path=/home/dark/Calificacion_Proyecto2.script
	*/

	finalizar := false
	fmt.Println("MIA - PROYECTO2 (exit para salir...)")
	reader := bufio.NewReader(os.Stdin)
	//  Ciclo para lectura de multiples comandos
	for !finalizar {
		fmt.Print("<Proyecto2>: ")
		comando, _ := reader.ReadString('\n')
		comando = strings.Replace(comando, "\n", "", -1)
		if strings.Contains(comando, "exit") {
			finalizar = true
		} else {
			if comando != "" && comando != "exit\n" {
				if Verificar_Comando(comando) == 0 {
					fmt.Println("RECONOCIO COMANDO CORRECTATEMTE")
				} else {
					fmt.Println("ERROR RETURN -1 EN ANALIZAR PARAMETROS")
				}
			}
		}
	}

}

func Verificar_parametros(texto_parametros string, nombre_comando string) []Param {
	//string comando_analizar = "mkdisk -size=10  -unit=m -fit=ff  -path=\"/home/dark/documentos/nuevacarpeta/archivo.txt\" #comentario de una linea";
	var param string = ""
	var valor_param string = ""
	var hay_comentario bool = false
	var hay_parametro bool = true
	var hay_valor bool = false
	var hay_comilla bool = false

	//variables comando

	var parametros [6]Param
	for k := 0; k < 6; k++ {
		parametros[k].nombre = ""
		parametros[k].valor = ""
	}

	//iniciando analizar parametros
	var indice int = 0
	for i := 0 + len(nombre_comando); i < len(texto_parametros); i++ {

		if !hay_comentario {

			if texto_parametros[i] == 32 && !hay_comilla {
				//espacio
				hay_parametro = true
				hay_valor = false
			}
			if texto_parametros[i] == 35 {
				//comentario

				hay_comentario = true
				hay_valor = false
				hay_parametro = false

			}
			if hay_valor {
				//valor parametro
				valor_param += string(texto_parametros[i])
				if texto_parametros[i] == 34 {

					if hay_comilla == true {
						hay_comilla = false

					} else {
						hay_comilla = true

					}
				}

				if len(texto_parametros) == i+1 {
					parametros[indice].nombre = param
					parametros[indice].valor = valor_param
					param = ""
					valor_param = ""
					break
				}

				if texto_parametros[i+1] == 32 && !hay_comilla {
					parametros[indice].nombre = param
					parametros[indice].valor = valor_param
					param = ""
					valor_param = ""
					indice++

				}

			}
			if hay_parametro {
				//nombre parametro
				/*if len(texto_parametros)-1 == i {
					param += string(texto_parametros[i])
					parametros[indice].nombre = param
					parametros[indice].valor = ""
					param = ""
					valor_param = ""
					hay_parametro = false
					hay_valor = false
				} else if texto_parametros[i+1] == 32 && i < len(texto_parametros)-1 {
					param += string(texto_parametros[i])
					parametros[indice].nombre = param
					parametros[indice].valor = ""
					indice++
					param = ""
					valor_param = ""
					hay_parametro = false
					hay_valor = false

				} else if texto_parametros[i] != 32 {
					param += string(texto_parametros[i])
				}*/

				if texto_parametros[i] != 32 {
					param += string(texto_parametros[i])
				}
				if strings.ToLower(param) == "-p" {
					if i < len(texto_parametros)-1 {
						if texto_parametros[i+1] == 32 {
							parametros[indice].nombre = param
							parametros[indice].valor = ""
							indice++
							param = ""
							valor_param = ""
							hay_parametro = false
							hay_valor = false
						}
					}

					if i == (len(texto_parametros) - 1) {
						parametros[indice].nombre = param
						parametros[indice].valor = ""
						param = ""
						valor_param = ""
						hay_parametro = false
						hay_valor = false
					}

				}
				if strings.ToLower(param) == "-r" {
					if i < len(texto_parametros)-1 {
						if texto_parametros[i+1] == 32 {
							parametros[indice].nombre = param
							parametros[indice].valor = ""
							indice++
							param = ""
							valor_param = ""
							hay_parametro = false
							hay_valor = false
						}
					}

					if i == (len(texto_parametros) - 1) {
						parametros[indice].nombre = param
						parametros[indice].valor = ""
						param = ""
						valor_param = ""
						hay_parametro = false
						hay_valor = false
					}
				}
			}
			if texto_parametros[i] == 61 {
				//=
				hay_parametro = false
				hay_valor = true

			}

		}

	} //analizando parametros
	return parametros[:]

} // fin de Verificar parametros

func Verificar_Comando(texto_comando string) int {
	fmt.Println(texto_comando)
	if texto_comando[0] != 35 {
		var ins_aux string = ""
		for i := 0; i < len(texto_comando); i++ {
			if texto_comando[i] == 32 {
				break
			}
			ins_aux += string(texto_comando[i])
		}

		switch strings.ToLower(ins_aux) {
		case "mkdisk":
			Verificar_mkdisk(texto_comando, ins_aux)
			break
		case "fdisk":
			Verificar_Fdisk(texto_comando, ins_aux)
			break
		case "mkfs":
			Verificar_Mkfs(texto_comando, ins_aux)
			break
		case "rmdisk":
			Verificar_Rmdisk(texto_comando, ins_aux)
			break
		case "mount":
			Verificar_Mount(texto_comando, ins_aux)
			break
		case "login":
			Verificar_Login(texto_comando, ins_aux)
			break
		case "logout":
			if Logout() == 0 {
				fmt.Println("CERRANDO SESION CORRECTAMENTE")
			} else if Logout() == 1 {
				fmt.Println("ERROR NO HAY SESION A CERRAR")
			}
			break
		case "mkgrp":
			Verificar_Mkgrp(texto_comando, ins_aux)
			break
		case "rmgrp":
			Verificar_Rmgrp(texto_comando, ins_aux)
			break
		case "mkusr":
			Verificar_Mkusr(texto_comando, ins_aux)
			break
		case "rmusr":
			Verificar_Rmusr(texto_comando, ins_aux)
			break
		case "mkdir":
			Verificar_Mkdir(texto_comando, ins_aux)
			break
		case "mkfile":
			Verificar_Mkfile(texto_comando, ins_aux)
			break
		case "comentario":
			break
		case "exec":

			parametros := Verificar_parametros(texto_comando, ins_aux)
			if strings.ToLower(parametros[0].nombre) == "-path=" {
				EjecutarExec(parametros[0].valor)
			} else {
				fmt.Println("ERROR PARAMETRO DESCONOCIDO", parametros[0].nombre)
			}

			break

		case "rep":
			Verificar_Rep(texto_comando, ins_aux)
			break
		case "pause":
			reader := bufio.NewReader(os.Stdin)
			texto, _ := reader.ReadString('\n')
			_ = texto

			break
		default:
			return -1

		}

	} else {
		//AQUI ES UN COMENTARIO
		//fmt.Println(texto_comando)
	}

	return 0
}

func Verificar_mkdisk(lista_param string, comando_aux string) int {
	var size int32 = 0
	var unit string = ""
	var path string = ""
	var fit string = ""
	var hay_size bool = false
	var hay_path bool = false

	//inicio de comando para parametros
	if "mkdisk" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_param, comando_aux)
		for i := 0; i < len(parametros); i++ {
			if parametros[i].nombre != "" && parametros[i].valor != "" {

				if "-size=" == strings.ToLower(parametros[i].nombre) {
					s, err := strconv.Atoi(parametros[i].valor)
					if err == nil && s > 0 {
						size = int32(s)
						hay_size = true

					} else {
						return -1
					}

				} else if "-unit=" == strings.ToLower(parametros[i].nombre) {
					if strings.ToLower(parametros[i].valor) == "" {
						fmt.Println("ERROR UNIT SIN VALOR EN PARAMETRO")
						return -1
					} else if strings.ToLower(parametros[i].valor) == "k" {
						unit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "m" {
						unit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "b" {
						unit = strings.ToLower(parametros[i].valor)

					} else {
						unit = strings.ToLower(parametros[i].valor)
						fmt.Println("ERROR UNIT PARAMETRO INCORRECTO: " + unit)
						return -1
					}
				} else if "-path=" == strings.ToLower(parametros[i].nombre) {
					var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					path = strings.ReplaceAll(path_temp, "\"", "")

					if path == "" {
						fmt.Println("ERROR PATH SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_path = true

				} else if "-fit=" == strings.ToLower(parametros[i].nombre) {

					if strings.ToLower(parametros[i].valor) == "" {
						fmt.Println("ERROR FIT SIN VALOR EN PARAMETRO")
						return -1
					} else if strings.ToLower(parametros[i].valor) == "wf" {
						fit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "ff" {
						fit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "bf" {
						fit = strings.ToLower(parametros[i].valor)

					} else {
						fit = strings.ToLower(parametros[i].valor)
						fmt.Println("ERROR FIT PARAMETRO INCORRECTO: " + fit)
						return -1
					}
				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin for

		/*__________________________ CREAR DISCO ______________________*/
		if hay_size {
			if hay_path {
				Crear_carpetas(path)
				if Crear_archivo(size, path, fit, unit) == 0 {
					fmt.Println("ARCHIVO CREADO CORRECTAMENTE EN MKDISK")
				} else {
					fmt.Println("ERROR NO SE PUDO CREAR ARCHIVO EN MKDISK")
				}
			} else {
				fmt.Println("ERROR NO HAY PATH EN MKDISK")
			}
		} else {
			fmt.Println("ERROR NO HAY SIZE MKDISK")
		}

		return 0
	}
	return -1
} /*______________________ FIN DE ANALIZAR COMANDO MKDISK_____________________*/

func Verificar_Rmdisk(lista_comando string, comando_aux string) int {
	var hay_path bool = false
	var path string = ""
	if "rmdisk" == strings.ToLower(comando_aux) {

		parametros := Verificar_parametros(lista_comando, comando_aux)

		for i := 0; i < len(parametros); i++ {
			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-path=" == strings.ToLower(parametros[i].nombre) {
					path = strings.ReplaceAll(parametros[i].valor, "\"", "")
					hay_path = true
				}

			}

		} //fin de for

		/*__________________________ ELIMINAR ARCHIVO ______________________*/
		if hay_path {
			var desicion int = Ejecutar_rmdisk(path)
			if desicion == 0 {
				fmt.Println("ARCHIVO ELIMINADO EXITOSAMENTE EN RMDISK")
			} else if desicion == 1 {
				fmt.Println("ELIMINAR ARCHIVO CANCELADO EN RMDISK")
			} else if desicion == 2 {
				fmt.Println("VALOR NO VALIDO PARA ELIMARAR ARCHIVO EN RMDISK")
			} else if desicion == -1 {
				fmt.Println("ARCHIVO NO EXISTE PARA ELIMARAR EN RMDISK")
			}
		} else {
			fmt.Println("ERROR NO HAY PATH EN RMDISK")
		}

		return 0
	}
	return -1
}

/*________________________ INICIO DE COMANDO FDISK ____________________________*/
func Verificar_Fdisk(lista_comando string, comando_aux string) int {
	var size int32 = 0
	var unit string = ""
	var path string = ""
	var type_ string = ""
	var fit string = ""
	var name string = ""
	var hay_size bool = false
	var hay_path bool = false
	var hay_name bool = false
	//_ = type_
	//_ = name
	//_ = hay_name

	if "fdisk" == strings.ToLower(comando_aux) {

		parametros := Verificar_parametros(lista_comando, comando_aux)

		for i := 0; i < len(parametros); i++ {
			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-size=" == strings.ToLower(parametros[i].nombre) {
					s, err := strconv.Atoi(parametros[i].valor)
					if err == nil && s > 0 {
						size = int32(s)
						hay_size = true

					} else {
						return -1
					}

				} else if "-unit=" == strings.ToLower(parametros[i].nombre) {
					if strings.ToLower(parametros[i].valor) == "" {
						fmt.Println("ERROR UNIT SIN VALOR EN PARAMETRO")
						return -1
					} else if strings.ToLower(parametros[i].valor) == "k" {
						unit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "m" {
						unit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "b" {
						unit = strings.ToLower(parametros[i].valor)

					} else {
						unit = strings.ToLower(parametros[i].valor)
						fmt.Println("ERROR UNIT PARAMETRO INCORRECTO: " + unit)
						return -1
					}
				} else if "-path=" == strings.ToLower(parametros[i].nombre) {
					var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					path = strings.ReplaceAll(path_temp, "\"", "")

					if path == "" {
						fmt.Println("ERROR PATH SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_path = true

				} else if "-fit=" == strings.ToLower(parametros[i].nombre) {

					if strings.ToLower(parametros[i].valor) == "" {
						fmt.Println("ERROR FIT SIN VALOR EN PARAMETRO")
						return -1
					} else if strings.ToLower(parametros[i].valor) == "wf" {
						fit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "ff" {
						fit = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "bf" {
						fit = strings.ToLower(parametros[i].valor)

					} else {
						//fit = strings.ToLower(parametros[i].valor)
						fmt.Println("ERROR FIT PARAMETRO INCORRECTO: " + strings.ToLower(parametros[i].valor))
						return -1
					}
				} else if "-type=" == strings.ToLower(parametros[i].nombre) {
					//type
					if strings.ToLower(parametros[i].valor) == "" {
						fmt.Println("ERROR TYPE SIN VALOR EN PARAMETRO")
						return -1
					} else if strings.ToLower(parametros[i].valor) == "p" {
						type_ = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "l" {
						type_ = strings.ToLower(parametros[i].valor)

					} else if strings.ToLower(parametros[i].valor) == "e" {
						type_ = strings.ToLower(parametros[i].valor)

					} else {
						fmt.Println("ERROR TYPE PARAMETRO INCORRECTO: " + strings.ToLower(parametros[i].valor))
						return -1
					}

				} else if "-name=" == strings.ToLower(parametros[i].nombre) {
					//name
					if parametros[i].valor == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO")
						return -1
					}
					name = parametros[i].valor
					hay_name = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR PARTICION ______________________*/
		if hay_size {
			if hay_path {
				if hay_name {
					if Crear_particion(size, unit, path, type_, fit, name) == 0 {
						//fmt.Println("PARTICION  CREADA CORRECTAMENTE")
					} else {
						fmt.Println("ERROR NO SE PUDO CREAR PARTICION")
					}
				} else {
					fmt.Println("ERROR NO HAY NAME EN FDISK")
				}

			} else {
				fmt.Println("ERROR NO HAY PATH EN FDISK")
			}
		} else {
			fmt.Println("ERROR NO HAY SIZE FDISK")
		}

		return 0
	}
	return -1
}

/*________________________ FIN DE COMANDO FDISK _______________________________*/

/*________________________ INICIO DE COMANDO MOUNT ____________________________*/
func Verificar_Mount(lista_comando string, comando_aux string) int {
	var path string = ""
	var name string = ""
	var hay_path bool = false
	var hay_name bool = false

	if "mount" == strings.ToLower(comando_aux) {

		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {
				if "-path=" == strings.ToLower(parametros[i].nombre) {
					var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					path = strings.ReplaceAll(path_temp, "\"", "")

					if path == "" {
						fmt.Println("ERROR PATH SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_path = true

				} else if "-name=" == strings.ToLower(parametros[i].nombre) {
					//name
					if parametros[i].valor == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO")
						return -1
					}
					name = parametros[i].valor
					hay_name = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MOUNT ______________________*/

		if hay_path {
			if hay_name {

				if listaS.insertar_Nodo(path, name) == 0 {
					//fmt.Println("PARTICION  CREADA CORRECTAMENTE")
				} else {
					fmt.Println("ERROR NO SE PUDO MONTAR")
					return -1
				}

			} else {
				fmt.Println("ERROR NO HAY NAME EN MOUNT")
				return -1
			}

		} else {
			fmt.Println("ERROR NO HAY PATH EN MOUNT")
			return -1
		}
		return 0
	}
	return -1
}

/*____________________________________ FIN DE COMANDO MOUNT _____________________________*/

/*____________________________________ INICIO DE COMANDO MKFS ____________________________*/

func Verificar_Mkfs(lista_comando string, comando_aux string) int {
	var id string = ""
	var type_ string = ""
	var hay_id bool = false

	if "mkfs" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {
				if "-id=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					id = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if id == "" {
						fmt.Println("ERROR ID SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_id = true

				} else if "-type=" == strings.ToLower(parametros[i].nombre) {
					//name
					if parametros[i].valor == "" {
						fmt.Println("ERROR TYPE SIN VALOR EN PARAMETRO")
						return -1
					}
					type_ = strings.ToLower(parametros[i].valor)

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MKFS ______________________*/

		if hay_id {

			if EjecutarMkfs(id, type_) == 0 {
				fmt.Println("EXT2... FORMATEADO CON EXITO...MKFS  EJECUTADA EXITOSAMENTE")
			} else {
				fmt.Println("ERROR NO SE PUDO MKFS")
				return -1
			}

		} else {
			fmt.Println("ERROR NO HAY ID EN MKFS")
			return -1
		}
		return 0
	}
	return -1
}

/*____________________________________ FIN DE COMANDO MKFS _______________________________*/

/*___________________________________ INCIO DE LOGIN _____________________________________*/
func Verificar_Login(lista_comando string, comando_aux string) int {
	var id string = ""
	var password string = ""
	var user string = ""
	var hay_id bool = false
	var hay_password = false
	var hay_user = false

	if "login" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {
				if "-id=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					id = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if id == "" {
						fmt.Println("ERROR ID SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_id = true

				} else if "-password=" == strings.ToLower(parametros[i].nombre) {
					//name
					if parametros[i].valor == "" {
						fmt.Println("ERROR PASSWORD SIN VALOR EN PARAMETRO")
						return -1
					}
					//password = strings.ToLower(parametros[i].valor)
					password = parametros[i].valor
					hay_password = true

				} else if "-usuario=" == strings.ToLower(parametros[i].nombre) {
					//name
					if parametros[i].valor == "" {
						fmt.Println("ERROR USUARIO SIN VALOR EN PARAMETRO")
						return -1
					}
					//user = strings.ToLower(parametros[i].valor)
					user = parametros[i].valor
					hay_user = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ INICIAR LOGIN ______________________*/

		if hay_id {
			if hay_user {
				if hay_password {
					var aux_nodo *NODO = listaS.obtenerNodo(id)
					if aux_nodo != nil {
						if Log_in(aux_nodo.Path, id, user, password) == 0 {
							fmt.Println("LOGIN EJECUTADA EXITOSAMENTE")
							return 0

						} else {
							fmt.Println("ERROR USUARIO NO SE PUDO LOGEAR")
							return -1
						}
					} else {
						fmt.Println("ERROR ID NO SE ENCOTRO EN LOGIN: ", id)
						return -1
					}

				} else {
					fmt.Println("ERROR NO HAY PASSWORD EN LOGIN")
					return -1
				}
			} else {
				fmt.Println("ERROR NO HAY USER EN LOGIN")
				return -1
			}

		} else {
			fmt.Println("ERROR NO HAY ID EN LOGIN")
			return -1
		}
	}

	return -1
}

/*___________________________________ FIN DE LOGIN _______________________________________*/

/*__________________________________  INICIO DE MKGRP ___________________________________*/
func Verificar_Mkgrp(lista_comando string, comando_aux string) int {
	var name string = ""
	var hay_name bool = false

	if "mkgrp" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-name=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					name = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if name == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_name = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MKGRP ______________________*/

		if hay_name {
			if Crear_Mkgrp(name) == 0 {
				//fmt.Println("LOGIN EJECUTADA EXITOSAMENTE")
				return 0
			} else {
				fmt.Println("ERROR USUARIO NO SE PUDO LOGEAR")
				return -1
			}
		} else {
			fmt.Println("ERROR NO HAY NAME EN MKGRP")
			return -1
		}
	}

	return -1
}

/*__________________________________  FIN DE MKGRP ______________________________________*/

/*__________________________________ INICO DE RMGRP ____________________________________*/
func Verificar_Rmgrp(lista_comando string, comando_aux string) int {
	var name string = ""
	var hay_name bool = false

	if "rmgrp" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-name=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					name = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if name == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_name = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR RMGRP ______________________*/

		if hay_name {
			if Eliminar_Grupo(name, actualSesion.Path) == 0 {
				//fmt.Println("LOGIN EJECUTADA EXITOSAMENTE")
				return 0
			} else {
				fmt.Println("ERROR USUARIO NO SE PUDO ELIMINAR")
				return -1
			}
		} else {
			fmt.Println("ERROR NO HAY NAME EN RMGRP")
			return -1
		}
	}

	return -1
}

/*__________________________________ FIN DE RMGRP _____________________________________*/

/*__________________________________  INICIO DE MKUSR ___________________________________*/
func Verificar_Mkusr(lista_comando string, comando_aux string) int {
	var usuario string = ""
	var pass_word = ""
	var grupo_incluir = ""
	var hay_usuario bool = false
	var hay_pass bool = false
	var hay_grupo bool = false

	if "mkusr" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-usuario=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					usuario = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if usuario == "" {
						fmt.Println("ERROR USUARIO SIN VALOR EN PARAMETRO MKUSR")
						return -1
					}
					hay_usuario = true

				} else if "-pwd=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					pass_word = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if pass_word == "" {
						fmt.Println("ERROR PWD SIN VALOR EN PARAMETRO EN MKUSR")
						return -1
					}
					hay_pass = true

				} else if "-grp=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					grupo_incluir = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if grupo_incluir == "" {
						fmt.Println("ERROR GRUPO SIN VALOR EN PARAMETRO MKUSR")
						return -1
					}
					hay_grupo = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: "+parametros[i].nombre, "EN MKUSER")
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MKUSR ______________________*/

		if !hay_usuario {
			fmt.Println("ERROR NO HAY USUARIO EN MKUSR")
			return -1
		}
		if !hay_pass {
			fmt.Println("ERROR NO HAY PASS EN MKUSR")
			return -1
		}
		if !hay_grupo {
			fmt.Println("ERROR NO HAY GRUPO EN MKUSR")
			return -1
		}
		if actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {
			if Crear_Mkusr(usuario, pass_word, grupo_incluir) == 0 {
				fmt.Println("USUARIO CREADO CORRENTAMENTE")
				return 0
			}
		}

	}

	return -1
}

/*__________________________________  FIN DE MKUSR ______________________________________*/

/*__________________________________ INICO DE RMUSR ____________________________________*/
func Verificar_Rmusr(lista_comando string, comando_aux string) int {
	var name string = ""
	var hay_name bool = false

	if "rmusr" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" {

				if "-usuario=" == strings.ToLower(parametros[i].nombre) {
					name = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if name == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO")
						return -1
					}
					hay_name = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR RMGRP ______________________*/

		if hay_name {
			if Eliminar_Usurio(name, actualSesion.Path) == 0 {
				fmt.Println("USUARIO ELIMINADO CORRECTAMENTE")
				return 0
			} else {
				fmt.Println("ERROR USUARIO NO SE PUDO ELIMINAR")
				return -1
			}
		} else {
			fmt.Println("ERROR NO HAY NAME EN RMGRP")
			return -1
		}
	}

	return -1
}

/*__________________________________ FIN DE RMUSR _______________________________________*/

/*__________________________________  INICIO DE MKDIR___________________________________*/
func Verificar_Mkdir(lista_comando string, comando_aux string) int {
	var path_crear string = ""
	var hay_path_crear bool = false
	var hay_p bool = false

	if "mkdir" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" || parametros[i].valor == "" && parametros[i].nombre != "" {

				if "-path=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					path_crear = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if path_crear == "" {
						fmt.Println("ERROR PATH SIN VALOR EN PARAMETRO MKDIR")
						return -1
					}
					hay_path_crear = true

				} else if "-p" == strings.ToLower(parametros[i].nombre) {
					if hay_p {
						fmt.Println("ERROR P SE REPITE EN PARAMETRO EN MKDIR")
						return -1
					}
					hay_p = true

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: "+parametros[i].nombre, "EN MKDIR")
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MKDIR ______________________*/

		if !hay_path_crear {
			fmt.Println("ERROR NO HAY PATH EN MKDIR")
			return -1
		}
		if actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {
			if Crear_Mkdir(path_crear, hay_p) == 0 {
				//fmt.Println("CARPETAS CREADA CORRENTAMENTE")
				return 0
			}
		}

	}

	return -1
}

/*___________________________________ FIN DE MKDIR _____________________________*/

/*____________________________________ INICIO DE MKFILE _________________________*/
func Verificar_Mkfile(lista_comando string, comando_aux string) int {
	var path_crear_archivo string = ""
	var hay_path_crear bool = false
	var size_archivo int = 0
	var cont_archivo string = ""

	var hay_r bool = false

	if "mkfile" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_comando, comando_aux)
		for i := 0; i < len(parametros); i++ {

			if parametros[i].valor != "" && parametros[i].nombre != "" || parametros[i].valor == "" && parametros[i].nombre != "" {

				if "-path=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					path_crear_archivo = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if path_crear_archivo == "" {
						fmt.Println("ERROR PATH SIN VALOR EN PARAMETRO MKFILE")
						return -1
					}
					hay_path_crear = true

				} else if "-r" == strings.ToLower(parametros[i].nombre) {
					if hay_r {
						fmt.Println("ERROR R SE REPITE EN PARAMETRO EN MKFILE")
						return -1
					}
					hay_r = true

				} else if "-size=" == strings.ToLower(parametros[i].nombre) {
					s, err := strconv.Atoi(parametros[i].valor)
					if err == nil && s > 0 {
						size_archivo = int(s)

					} else {
						fmt.Println("ERROR SIZE  EN PARAMETRO EN MKFILE")
						return -1
					}

				} else if "-cont=" == strings.ToLower(parametros[i].nombre) {
					cont_archivo = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if cont_archivo == "" {
						fmt.Println("ERROR CONT SIN VALOR EN PARAMETRO MKFILE")
						return -1
					}

				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: "+parametros[i].nombre, "EN MKFILE")
					return -1
				}

			}

		} //fin de for

		/*__________________________ CREAR MKDIR ______________________*/
		if !hay_path_crear {
			fmt.Println("ERROR NO HAY PATH EN MKFILE")
			return -1
		}
		if actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {
			if Crear_Mkfile(path_crear_archivo, size_archivo, cont_archivo, hay_r) == 0 {
				//fmt.Println("CARPETAS CREADA CORRENTAMENTE")
				return 0
			}
		}

	}

	return -1
}

/*____________________________________ FIN DE MKFILE ____________________________*/

/*__________________________________ INICIO REPORTE _____________________________________*/
func Verificar_Rep(lista_param string, comando_aux string) int {
	var name string = ""
	var id string = ""
	var path string = ""
	var path_reporte = ""
	var extension_rep = ""
	var ruta string = ""
	var hay_name bool = false
	var hay_path bool = false
	var hay_id bool = false

	//inicio de comando para parametros
	if "rep" == strings.ToLower(comando_aux) {
		parametros := Verificar_parametros(lista_param, comando_aux)
		for i := 0; i < len(parametros); i++ {
			if parametros[i].nombre != "" && parametros[i].valor != "" {

				if "-name=" == strings.ToLower(parametros[i].nombre) {
					name = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if name == "" {
						fmt.Println("ERROR NAME SIN VALOR EN PARAMETRO REPORTE")
						return -1
					}
					hay_name = true

				} else if "-path=" == strings.ToLower(parametros[i].nombre) {
					path_reporte = strings.ReplaceAll(parametros[i].valor, "\"", "")
					var tem_ext = strings.ReplaceAll(filepath.Ext(parametros[i].valor), ".", "")
					extension_rep = strings.ReplaceAll(tem_ext, "\"", "")

					var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dot", 1)
					path = strings.ReplaceAll(path_temp, "\"", "")

					if path == "" {
						fmt.Println("ERROR ID SIN VALOR EN PARAMETRO REPORTE")
						return -1
					}
					hay_path = true
				} else if "-id=" == strings.ToLower(parametros[i].nombre) {
					//var path_temp string = strings.Replace(parametros[i].valor, filepath.Ext(parametros[i].valor), ".dk", 1)
					id = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if id == "" {
						fmt.Println("ERROR ID SIN VALOR EN PARAMETRO REPORTE")
						return -1
					}
					hay_id = true

				} else if "-ruta=" == strings.ToLower(parametros[i].nombre) {
					ruta = strings.ReplaceAll(parametros[i].valor, "\"", "")

					if ruta == "" {
						fmt.Println("ERROR RUTA SIN VALOR EN PARAMETRO REPORTE")
						return -1
					}
					//hay_id = true
				} else {
					fmt.Println("ERROR NINGUN PARAMETRO COICIDE: " + parametros[i].nombre)
					return -1
				}

			}

		} //fin for

		/*__________________________ CREAR DISCO ______________________*/
		if hay_name {
			if hay_path {
				if hay_id {
					//Crear_carpetas(path)
					if listaS.buscarParticion(id) {
						nodito := listaS.obtenerNodo(id)
						if name == "mbr" {
							if crear_rep_mbr(nodito.Path, path, path_reporte, extension_rep) == 0 {
								fmt.Println("REPORTE MBR CREADO EXITOSAMENTE EN REP")
							} else {
								fmt.Println("ERROR NO SE PUDO CREAR REPORTE MBR EN REP")
							}
						} else if name == "disk" {
							if crear_rep_disk(nodito.Path, path, path_reporte, extension_rep) == 0 {
								fmt.Println("REPORTE DISK CREADO EXITOSAMENTE EN REP")
							} else {
								fmt.Println("ERROR NO SE PUDO CREAR REPORTE DISK EN REP")
							}
						} else if name == "tree" {
							if crear_rep_tree(nodito.Path, path, path_reporte, extension_rep) == 0 {
								fmt.Println("REPORTE TREE CREADO EXITOSAMENTE EN REP")
							} else {
								fmt.Println("ERROR NO SE PUDO CREAR REPORTE TREE EN REP")
							}
						} else if name == "sb" {
							if crear_rep_super_bloque(nodito.Path, path, path_reporte, extension_rep) == 0 {
								fmt.Println("REPORTE SB CREADO EXITOSAMENTE EN REP")
							} else {
								fmt.Println("ERROR NO SE PUDO CREAR REPORTE SB EN REP")
							}
						} else if name == "file" {
							if crear_rep_file(nodito.Path, path, path_reporte, extension_rep, ruta) == 0 {
								fmt.Println("REPORTE FILE CREADO EXITOSAMENTE EN REP")
							} else {
								fmt.Println("ERROR NO SE PUDO CREAR REPORTE FILE EN REP")
							}
						}

					} else {
						fmt.Println("ERROR DISCO NO MONTADO PARA CREAR REPORTE")
					}

				} else {
					fmt.Println("ERROR NO HAY ID EN REPORTE")
				}
			} else {
				fmt.Println("ERROR NO HAY PATH EN REPORTE")
			}
		} else {
			fmt.Println("ERROR NO HAY  NAME REPORTE")
		}

		return 0
	}
	return -1
} /*______________________ FIN DE REPORTE _____________________*/

func EjecutarExec(path string) {
	var mi_extension = Obtener_extension_archivo(path)
	if strings.ToLower(mi_extension) == ".script" {
		f, err := os.Open(path)
		if err != nil {
			log.Fatalf("open file error: %v", err)
			return
		}
		defer f.Close()

		sc := bufio.NewScanner(f)
		for sc.Scan() {
			lineaTexto := sc.Text() // GET the line string

			if len(lineaTexto) > 0 {
				if Verificar_Comando(lineaTexto) == 0 {
					//descomentar si est es el comentario xD
					//fmt.Println(lineaTexto)
				} else {
					fmt.Println("ERROR AL Verificar COMANDO:", lineaTexto)
				}

			}

		}
		fmt.Println("COMANDO EXEC EJECUTADO CORRECTAMENTE")

		if err := sc.Err(); err != nil {
			log.Fatalf("scan file error: %v", err)
			return
		}
	} else {
		fmt.Println("ERROR SOLO EJECUTAR ARCHIVOS .SCRIPT ...EXTENSION ERRONEO:", Obtener_extension_archivo(path))
	}

}

func Print_fdisk() {
	fmt.Println("Estoy en Fdisk")
}

func Crear_particion(size int32, unit string, path string, type_ string, fit string, name string) int {

	if type_ == "p" {
		if buscar_indice_libre(path, "p") != -1 {
			if Buscar_Nombre_P_E_L(path, name) == -1 {
				particion_primaria(size, unit, path, type_, fit, name)
			} else {
				fmt.Println("ERROR AL CREAR PARTICION CON NOMBRE REPETIDO")
			}
		} else {
			fmt.Println("ERROR,YA EXISTEN 4 PARTICIONES")
		}

	} else if type_ == "e" {
		if buscar_indice_libre(path, "e") == -1 {
			if Buscar_Nombre_P_E_L(path, name) == -1 {
				particion_extendida(size, unit, path, type_, fit, name)
			} else {
				fmt.Println("ERROR AL CREAR PARTICION CON NOMBRE REPETIDO")
			}
		} else {
			fmt.Println("ERROR,YA EXISTE UNA PARTICION EXTENDIDA")
		}

	} else if type_ == "l" {
		if Buscar_Nombre_P_E_L(path, name) == -1 {
			particion_logica(size, unit, path, type_, fit, name)
		} else {
			fmt.Println("ERROR AL CREAR PARTICION CON NOMBRE REPETIDO")
		}

	} else {
		if Buscar_Nombre_P_E_L(path, name) == -1 {
			particion_primaria(size, unit, path, type_, fit, name)
		} else {
			fmt.Println("ERROR AL CREAR PARTICION CON NOMBRE REPETIDO")
		}
	}

	return 0
}

func particion_primaria(size int32, unit string, path string, type_ string, fit string, name string) {
	masterBoot := leer_archivo(path)
	var indice int32 = Posicion_arreglo_disponible(&masterBoot, size)

	if indice != -1 {
		if unit == "b" {
			size = size * 1
		} else if unit == "k" {
			size = size * 1024
		} else if unit == "m" {
			size = size * 1024 * 1024
		} else if unit == "" {
			size = size * 1024
		}

		if type_ == "" {
			type_ = "p"
		}

		if size <= EspacioDisponible(path) {
			if fit == "ff" {
				//primer ajuste(FF)
				if indice == 0 {

					masterBoot.Mbr_partition[indice].Part_status = 49
					masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[indice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[indice].Part_size = size
					copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[indice].Part_status = 49
					masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[indice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[indice].Part_size = size
					copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")
			} else if fit == "bf" {
				//mejor ajuste(BF)
				var mejorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[mejorIndice].Part_size > masterBoot.Mbr_partition[i].Part_size {
								mejorIndice = int32(i)
								break
							}
						}
					}
				}
				if mejorIndice == 0 {
					if fit == "ff" {
						//primer ajuste(FF)
						if indice == 0 {

							masterBoot.Mbr_partition[indice].Part_status = 49
							masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]

							copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[indice].Part_start = int32(unsafe.Sizeof(masterBoot))
							masterBoot.Mbr_partition[indice].Part_size = size
							copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])

						} else {
							masterBoot.Mbr_partition[indice].Part_status = 49
							masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]
							copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[indice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
							masterBoot.Mbr_partition[indice].Part_size = size
							copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])
						}
						Modificar_archivo(&masterBoot, path)
						fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")
					} else if fit == "bf" {
						//mejor ajuste(BF)
						var mejorIndice int32 = indice
						for i := 0; i < 4; i++ {
							if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
								if int32(i) != indice {
									if masterBoot.Mbr_partition[mejorIndice].Part_size > masterBoot.Mbr_partition[i].Part_size {
										mejorIndice = int32(i)
										break
									}
								}
							}
						}
						if mejorIndice == 0 {

							masterBoot.Mbr_partition[mejorIndice].Part_status = 49
							masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]

							copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[mejorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
							masterBoot.Mbr_partition[mejorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])

						} else {
							masterBoot.Mbr_partition[mejorIndice].Part_status = 49
							masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]
							copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[mejorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
							masterBoot.Mbr_partition[mejorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])
						}
						Modificar_archivo(&masterBoot, path)
						fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

					} else if fit == "wf" {
						//peor ajuste(WF)
						var peorIndice int32 = indice
						for i := 0; i < 4; i++ {
							if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
								if int32(i) != indice {
									if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
										peorIndice = int32(i)
										break
									}
								}
							}
						}
						if peorIndice == 0 {

							masterBoot.Mbr_partition[peorIndice].Part_status = 49
							masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

							copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
							masterBoot.Mbr_partition[peorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

						} else {
							masterBoot.Mbr_partition[peorIndice].Part_status = 49
							masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
							copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
							masterBoot.Mbr_partition[peorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
						}
						Modificar_archivo(&masterBoot, path)
						fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

					} else if fit == "" {
						//peor ajuste(WF)
						var peorIndice int32 = indice
						for i := 0; i < 4; i++ {
							if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
								if int32(i) != indice {
									if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
										peorIndice = int32(i)
										break
									}
								}
							}
						}
						if peorIndice == 0 {

							masterBoot.Mbr_partition[peorIndice].Part_status = 49
							masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

							copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
							masterBoot.Mbr_partition[peorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

						} else {
							masterBoot.Mbr_partition[peorIndice].Part_status = 49
							masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
							copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
							masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
							masterBoot.Mbr_partition[peorIndice].Part_size = size
							copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
						}
						Modificar_archivo(&masterBoot, path)
						fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

					}
					masterBoot.Mbr_partition[mejorIndice].Part_status = 49
					masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[mejorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[mejorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[mejorIndice].Part_status = 49
					masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[mejorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[mejorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

			} else if fit == "wf" {
				//peor ajuste(WF)
				var peorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
								peorIndice = int32(i)
								break
							}
						}
					}
				}
				if peorIndice == 0 {

					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

			} else if fit == "" {
				//peor ajuste(WF)
				var peorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
								peorIndice = int32(i)
								break
							}
						}
					}
				}
				if peorIndice == 0 {

					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], "wf")
					masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], "wf")
					masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				fmt.Println("PARTICION PRIMARIA CREADA CORRECTAMENTE")

			}

		} else {
			fmt.Println("ERROR AL CREAR PARTICION PRIMARIA ESPACIO INSUFICIENTE.   ESPACIO DISPONIBLE:", EspacioDisponible(path), "ESPACIO FALTANTE: ", (size - EspacioDisponible(path)))
		}

	} else {
		fmt.Println("ERROR,AL CREAR PARTICION YA EXISTE 4 PARTICIONES")
	}
}

func particion_extendida(size int32, unit string, path string, type_ string, fit string, name string) {
	masterBoot := leer_archivo(path)
	var indice int32 = Posicion_arreglo_disponible(&masterBoot, size)

	if indice != -1 {
		if unit == "b" {
			size = size * 1
		} else if unit == "k" {
			size = size * 1024
		} else if unit == "m" {
			size = size * 1024 * 1024
		} else if unit == "" {
			size = size * 1024
		}

		if size <= EspacioDisponible(path) {
			if fit == "ff" {
				//primer ajuste(FF)
				if indice == 0 {

					masterBoot.Mbr_partition[indice].Part_status = 49
					masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[indice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[indice].Part_size = size
					copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[indice].Part_status = 49
					masterBoot.Mbr_partition[indice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[indice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[indice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[indice].Part_size = size
					copy(masterBoot.Mbr_partition[indice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				//se crea el objeto ebr
				extendBoot := EBR{}
				extendBoot.Part_status = 0
				copy(extendBoot.Part_fit[:], fit)
				extendBoot.Part_start = masterBoot.Mbr_partition[indice].Part_start
				extendBoot.Part_size = -1
				extendBoot.Part_next = -1
				copy(extendBoot.Part_name[:], "")
				Modificar_archivo_ebr(&extendBoot, path, extendBoot.Part_start)
				fmt.Println("PARTICION EXTENDIDA CREADA CORRECTAMENTE")

			} else if fit == "bf" {
				//mejor ajuste(BF)
				var mejorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[mejorIndice].Part_size > masterBoot.Mbr_partition[i].Part_size {
								mejorIndice = int32(i)
								break
							}
						}
					}
				}
				if mejorIndice == 0 {

					masterBoot.Mbr_partition[mejorIndice].Part_status = 49
					masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[mejorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[mejorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[mejorIndice].Part_status = 49
					masterBoot.Mbr_partition[mejorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[mejorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[mejorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[mejorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[mejorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				//se crea el objeto ebr
				extendBoot := EBR{}
				extendBoot.Part_status = 0
				copy(extendBoot.Part_fit[:], fit)
				extendBoot.Part_start = masterBoot.Mbr_partition[mejorIndice].Part_start
				extendBoot.Part_size = -1
				extendBoot.Part_next = -1
				copy(extendBoot.Part_name[:], "")
				Modificar_archivo_ebr(&extendBoot, path, extendBoot.Part_start)
				fmt.Println("PARTICION EXTENDIDA CREADA CORRECTAMENTE")

			} else if fit == "wf" {
				//peor ajuste(WF)
				var peorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
								peorIndice = int32(i)
								break
							}
						}
					}
				}
				if peorIndice == 0 {

					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				//se crea el objeto ebr
				extendBoot := EBR{}
				extendBoot.Part_status = 0
				copy(extendBoot.Part_fit[:], fit)
				extendBoot.Part_start = masterBoot.Mbr_partition[peorIndice].Part_start
				extendBoot.Part_size = -1
				extendBoot.Part_next = -1
				copy(extendBoot.Part_name[:], "")
				Modificar_archivo_ebr(&extendBoot, path, extendBoot.Part_start)
				fmt.Println("PARTICION EXTENDIDA CREADA CORRECTAMENTE")

			} else if fit == "" {
				//peor ajuste(WF)
				var peorIndice int32 = indice
				for i := 0; i < 4; i++ {
					if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
						if int32(i) != indice {
							if masterBoot.Mbr_partition[peorIndice].Part_size < masterBoot.Mbr_partition[i].Part_size {
								peorIndice = int32(i)
								break
							}
						}
					}
				}
				if peorIndice == 0 {

					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]

					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], "wf")
					masterBoot.Mbr_partition[peorIndice].Part_start = int32(unsafe.Sizeof(masterBoot))
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])

				} else {
					masterBoot.Mbr_partition[peorIndice].Part_status = 49
					masterBoot.Mbr_partition[peorIndice].Part_type = ([]byte(type_))[0]
					copy(masterBoot.Mbr_partition[peorIndice].Part_fit[:], fit[:])
					masterBoot.Mbr_partition[peorIndice].Part_start = masterBoot.Mbr_partition[indice-1].Part_start + masterBoot.Mbr_partition[indice-1].Part_size
					masterBoot.Mbr_partition[peorIndice].Part_size = size
					copy(masterBoot.Mbr_partition[peorIndice].Part_name[:], name[:])
				}
				Modificar_archivo(&masterBoot, path)
				//se crea el objeto ebr
				extendBoot := EBR{}
				extendBoot.Part_status = 0
				copy(extendBoot.Part_fit[:], "wf")
				extendBoot.Part_start = masterBoot.Mbr_partition[peorIndice].Part_start
				extendBoot.Part_size = -1
				extendBoot.Part_next = -1
				copy(extendBoot.Part_name[:], "")
				Modificar_archivo_ebr(&extendBoot, path, extendBoot.Part_start)
				fmt.Println("PARTICION EXTENDIDA CREADA CORRECTAMENTE")
			}

		} else {
			fmt.Println("ERROR AL CREAR PARTICION EXTENDIDA ESPACIO INSUFICIENTE.   ESPACIO DISPONIBLE:", EspacioDisponible(path), "ESPACIO FALTANTE: ", (size - EspacioDisponible(path)))
		}

	} else {
		fmt.Println("ERROR,AL CREAR PARTICION YA EXISTE 4 PARTICIONES")
	}

}

func particion_logica(size int32, unit string, path string, type_ string, fit string, name string) {
	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
	}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(MBR{}))
	file.Seek(0, 0)
	masterBoot := ObtenerInforMbr(file, tamano_masterBoot)

	//------------------------------------------
	var indice int32 = -1
	for i := 0; i < 4; i++ {
		if string(masterBoot.Mbr_partition[i].Part_type) == "e" {
			indice = int32(i)
			break
		}
	}
	if indice != -1 {
		if fit == "" {
			fit = "wf"
		}
		if unit == "b" {
			size = size * 1
		} else if unit == "k" || unit == "" {
			size = size * 1024
		} else if unit == "m" {
			size = size * 1024 * 1024
		}

		extendedBoot := EBR{}
		var cont int32 = masterBoot.Mbr_partition[indice].Part_start
		file.Seek(int64(cont), 0)
		extendedBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))

		var espacio_disp int32 = EspacioDisponibleExtendida(path)

		if size <= espacio_disp {
			if extendedBoot.Part_size == -1 { //Si es la primera
				if masterBoot.Mbr_partition[indice].Part_size < size {
					//espacio insuficiente
					fmt.Println("ERROR AL CREAR LOGICA ESPACIO INSUFICIENTE")
				} else {
					extendedBoot.Part_status = 49
					copy(extendedBoot.Part_fit[:], fit)
					pos_actual, _ := file.Seek(0, 1)
					//extendedBoot.Part_start = masterBoot.Mbr_partition[indice].Part_start // ftell(fp) - sizeof(EBR); //Para regresar al inicio de la extendida
					extendedBoot.Part_start = int32(pos_actual) - int32(unsafe.Sizeof(EBR{}))
					extendedBoot.Part_size = size
					extendedBoot.Part_next = -1
					copy(extendedBoot.Part_name[:], name)
					//file.Seek(masterBoot.Mbr_partition[indice].Part_start,0);
					Modificar_archivo_ebr(&extendedBoot, path, masterBoot.Mbr_partition[indice].Part_start) //espremor que no de erro aqui sino mandar el archiv abierto de una
					fmt.Println("PARTICION LOGICA CREADA CORRECTAMENTE")
				}
			} else {
				for (extendedBoot.Part_next != -1) && (Ftell(file) < (masterBoot.Mbr_partition[indice].Part_size + masterBoot.Mbr_partition[indice].Part_start)) {
					file.Seek(int64(extendedBoot.Part_next), 0)
					extendedBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
					//fread(&extendedBoot,sizeof(EBR),1,fp);
				}

				var espacioNecesario int32 = extendedBoot.Part_start + extendedBoot.Part_size + size

				if espacioNecesario <= (masterBoot.Mbr_partition[indice].Part_size + masterBoot.Mbr_partition[indice].Part_start) {
					extendedBoot.Part_next = extendedBoot.Part_start + extendedBoot.Part_size
					//Escribimos el next del ultimo EBR
					mover, _ := file.Seek(int64(Ftell(file))-int64(unsafe.Sizeof(EBR{})), 0)
					Modificar_archivo_ebr(&extendedBoot, path, int32(mover)) //verificar aqui sino F para escribir el archivo
					//Escribimos el nuevo EBR
					file.Seek(int64(extendedBoot.Part_start+extendedBoot.Part_size), 0)
					extendedBoot.Part_status = 49
					copy(extendedBoot.Part_fit[:], fit)
					extendedBoot.Part_start = Ftell(file)
					extendedBoot.Part_size = size
					extendedBoot.Part_next = -1
					copy(extendedBoot.Part_name[:], name)
					//fwrite(&extendedBoot, sizeof(EBR), 1, fp)
					Modificar_archivo_ebr(&extendedBoot, path, Ftell(file))
					fmt.Println("PARTICION LOGICA CREADA CORRECTAMENTE")

				} else {
					fmt.Println("ERROR la particion logica a crear excede el")
					fmt.Println("espacio disponible de la particion extendida")
				}
			}

		} else {
			fmt.Println("ERROR AL CREAR PARTICION LOGICA ESPACIO INSUFICIENTE.   ESPACIO DISPONIBLE:", espacio_disp, "ESPACIO FALTANTE: ", (size - espacio_disp))
		}

	} else {
		fmt.Println("ERROR AL CREAR PARTICION LOGICA,NO EXISTE EXTENDIDA")
	}

	file.Close()
	//fin de particion logica
}

func leer_archivo(path string) MBR {
	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.Open(path)
	defer file.Close()
	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		os.Exit(4)
	}

	masterBoot := MBR{}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(masterBoot))
	data := leerBytes(file, tamano_masterBoot)
	buffer := bytes.NewBuffer(data)
	err = binary.Read(buffer, binary.BigEndian, &masterBoot)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		os.Exit(4)
	}

	file.Close()

	return masterBoot
}

func leerBytes(file *os.File, number int32) []byte {
	bytes := make([]byte, number)
	_, err := file.Read(bytes)
	if err != nil {
		log.Fatal("ERROR  A LEER BYTES", err)
	}

	return bytes
}

func Posicion_arreglo_disponible(masterBoot *MBR, size int32) int32 {
	for i := 0; i < 4; i++ {
		if masterBoot.Mbr_partition[i].Part_start == -1 || (masterBoot.Mbr_partition[i].Part_status == 48 && masterBoot.Mbr_partition[i].Part_size >= size) {
			return int32(i)
		}
	}
	return -1
}

func Modificar_archivo(masterBoot *MBR, path string) {
	mi_archivo, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer mi_archivo.Close()
	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		//return -1
	}

	mi_archivo.Seek(0, 0)
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, masterBoot)
	EscribirBytes(mi_archivo, master_buffer.Bytes())
	mi_archivo.Close()
}

func Modificar_archivo_ebr(extendBoot *EBR, path string, moverPuntero int32) {
	mi_archivo, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer mi_archivo.Close()
	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		//return -1
	}

	mi_archivo.Seek(int64(moverPuntero), 0)
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, extendBoot)
	EscribirBytes(mi_archivo, master_buffer.Bytes())
	mi_archivo.Close()
}

/*___________________ PARTICION EXTENDIDA _____________*/
func Buscar_Nombre_P_E_L(path string, name string) int32 {

	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
	}
	masterBoot := MBR{}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(masterBoot))
	masterBoot = ObtenerInforMbr(file, tamano_masterBoot)

	var indice_extendia int32 = -1
	for i := 0; i < 4; i++ {
		if string(bytes.Trim(masterBoot.Mbr_partition[i].Part_name[:], "\x00")) == name {
			file.Close()
			return 1
		}
		if string(masterBoot.Mbr_partition[i].Part_type) == "e" {
			indice_extendia = int32(i)
		}
	}

	if indice_extendia != -1 {
		//aqui va lo de la logica
		extendBoot := EBR{}
		file.Seek(int64(masterBoot.Mbr_partition[indice_extendia].Part_start), 0) //(cantidad_bytes_desplazar,origen_de_archivo)
		extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
		for {
			//fmt.Println("Nombre Particion: ",string(extendBoot.Part_name[:])," NOMBRE COMPARADO: ",name)
			//fmt.Println("Tamano Partcicion: ",len(string(bytes.Trim(extendBoot.Part_name[:],"\x00")))," Tamano Comparado: ",len(name))
			//se utiliza bytes.Trimp para eliminar los caracteres nulos y comparar el nombre xD
			if string(bytes.Trim(extendBoot.Part_name[:], "\x00")) == name {
				return 1
			}

			if extendBoot.Part_next == -1 {
				break
			}
			file.Seek(int64(extendBoot.Part_next), 0)
			extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{}))) //ruta quemada por el momento xD
		}
	}
	file.Close()

	return -1
}

func ObtenerInforMbr(file *os.File, tamano_leer int32) MBR {
	//file.Seek(0,0) //esto se agrego para mover el puntero en el origen del archivo sino borrar
	masterBoot := MBR{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &masterBoot)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return MBR{}
	}
	return masterBoot
}

func ObtenerInfoEbr(file *os.File, tamano_leer int32) EBR {
	extendBoot := EBR{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &extendBoot)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return EBR{}
	}
	return extendBoot
}

func Ftell(file *os.File) int32 {
	var posiconActual int64 = 0
	var err error = nil
	posiconActual, err = (file.Seek(0, 1))
	if err != nil {
		fmt.Println("EXISTEN ERRORE EN LA POSICION ACTUAL FTELL")
	}

	return int32(posiconActual)
}

func EspacioDisponible(path string) int32 {
	masterBoot := leer_archivo(path)
	var espacio_usado int32 = 0
	espacio_usado = int32(unsafe.Sizeof(MBR{}))
	for i := 0; i < 4; i++ {
		if masterBoot.Mbr_partition[i].Part_status == 49 {
			espacio_usado += masterBoot.Mbr_partition[i].Part_size
		}
	}

	return masterBoot.Mbr_tamano - espacio_usado
}

func EspacioDisponibleExtendida(path string) int32 {
	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		return -1 //aqui lo regresamos porque no encontro el archivo sino da error
	}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(MBR{}))
	file.Seek(0, 0)
	masterBoot := ObtenerInforMbr(file, tamano_masterBoot)

	var indice int32 = -1
	var espacio_usado int32 = 0
	for i := 0; i < 4; i++ {
		if masterBoot.Mbr_partition[i].Part_type == 101 && masterBoot.Mbr_partition[i].Part_status == 49 {
			indice = int32(i)
		}

	}

	extendedBoot := EBR{}
	var cont int32 = masterBoot.Mbr_partition[indice].Part_start
	file.Seek(int64(cont), 0)
	extendedBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))

	if extendedBoot.Part_size >= 0 {
		espacio_usado += extendedBoot.Part_size
	}

	for (extendedBoot.Part_next != -1) && (Ftell(file) < (masterBoot.Mbr_partition[indice].Part_size + masterBoot.Mbr_partition[indice].Part_start)) {
		file.Seek(int64(extendedBoot.Part_next), 0)
		extendedBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
		espacio_usado += extendedBoot.Part_size
		//fread(&extendedBoot,sizeof(EBR),1,fp);
	}

	return (masterBoot.Mbr_partition[indice].Part_size - espacio_usado)
}

func Ver_particion_Primaria_Extendida_Logica(path string) {
	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		os.Exit(4)
	}
	masterBoot := MBR{}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(masterBoot))
	masterBoot = ObtenerInforMbr(file, tamano_masterBoot)
	fmt.Println("*___________________________________________*")
	fmt.Println("Disco Fit: ", string(masterBoot.Disk_fit[:]))
	fmt.Println("Disco Asignatura: ", masterBoot.Mbr_disk_signature)
	fmt.Println("Fecha Creacion:", string(masterBoot.Mbr_fecha_creacion[:]))
	fmt.Println("Disco Tamano:", masterBoot.Mbr_tamano)

	var indice_extendia int32 = -1
	for i := 0; i < 4; i++ {
		fmt.Println("*___________________________________________*")
		fmt.Println("Particion[", i, "].Fit   :", string(masterBoot.Mbr_partition[i].Part_fit[:]))
		fmt.Println("Particion[", i, "].Name  :", string(masterBoot.Mbr_partition[i].Part_name[:]))
		fmt.Println("Particion[", i, "].Size  :", masterBoot.Mbr_partition[i].Part_size)
		fmt.Println("Particion[", i, "].Start :", masterBoot.Mbr_partition[i].Part_start)
		fmt.Println("Particion[", i, "].Status:", string(masterBoot.Mbr_partition[i].Part_status))
		fmt.Println("Particion[", i, "].Type  :", string(masterBoot.Mbr_partition[i].Part_type))

		if string(masterBoot.Mbr_partition[i].Part_type) == "e" {
			indice_extendia = int32(i)
		}
	}

	if indice_extendia != -1 {
		//aqui va lo de la logica
		extendBoot := EBR{}
		file.Seek(int64(masterBoot.Mbr_partition[indice_extendia].Part_start), 0) //(cantidad_bytes_desplazar,origen_de_archivo)
		extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
		for {
			fmt.Println("*___________________________________________*")
			fmt.Println("Particion[].Fit   :", string(extendBoot.Part_fit[:]))
			fmt.Println("Particion[].Name  :", string(extendBoot.Part_name[:]))
			fmt.Println("Particion[].Size  :", extendBoot.Part_next)
			fmt.Println("Particion[].Start :", extendBoot.Part_size)
			fmt.Println("Particion[].Status:", extendBoot.Part_start)
			fmt.Println("Particion[].Type  :", string(extendBoot.Part_status))

			if extendBoot.Part_next == -1 {
				break
			}
			file.Seek(int64(extendBoot.Part_next), 0)
			extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{}))) //ruta quemada por el momento xD
		}
	}
	file.Close()
}

/*
Metodos

	Error al no crear la operacion soliciada
	por Espacio
	verificarExtendida para logica y verificar tamano
	Solo una extendida no se puede crear particion logica sino existe extendia
	No repetir nombre
	Eliminar_particion_debe existir
*/
func buscar_indice_libre(path string, type_ string) int {
	masterBoot := leer_archivo(path)
	if type_ == "p" {
		for i := 0; i < 4; i++ {
			if masterBoot.Mbr_partition[i].Part_status == 0 {
				return i
			}
		}
		return -1 //indica que ya no espacio para primaria
	} else if type_ == "e" {
		for i := 0; i < 4; i++ {
			if masterBoot.Mbr_partition[i].Part_type == 101 {
				return i
			}
		}
		return -1 //Inidica que no hay extendida
	}

	return -1
}

func (nodo *NODO) incialiar(path string, name string, letra byte, numero int32, id string, contador int32) {
	nodo.Path = path
	nodo.Name = name
	nodo.Letra = letra
	nodo.Numero = numero
	nodo.Contador = contador
	nodo.Id = id
	nodo.Siguiente = nil
}

func inicialiarList() *LISTASIMPLE {
	//ls.Primero = nil
	//ls.Ultimo = nil
	//ls.Letra_temp = 48
	//ls.Numero_temp = 0
	//ls.Contador_temp = 0
	return &LISTASIMPLE{nil, nil, 0, 0, 0, "", ""}
}

func (ls *LISTASIMPLE) inicialiarLista() {
	ls.Primero = nil
	ls.Ultimo = nil
	ls.Letra_temp = 48
	ls.Numero_temp = 0
	ls.Contador_temp = 0
}

func (ls *LISTASIMPLE) Esta_Vacio() bool {
	return ls.Primero == nil
}

func (ls *LISTASIMPLE) insertar_Nodo(path string, name string) int64 {
	if existeArchivo(path) {
		if Obtener_extension_archivo(path) == ".dk" {
			if Buscar_Nombre_P_E_L(path, name) == 1 {
				ls.obtener_letra(path, name)
				var new_Node NODO //= nil //Nodo_mount(path,name,letra_temp,numero_temp,obtener_id(),contador_temp);
				new_Node.incialiar(path, name, ls.Letra_temp, ls.Numero_temp, ls.obtener_id(), ls.Contador_temp)
				if ls.Esta_Vacio() {
					ls.Primero = &new_Node
					ls.Primero.Siguiente = nil
					ls.Ultimo = ls.Primero

				} else {
					ls.Ultimo.Siguiente = &new_Node
					new_Node.Siguiente = nil
					ls.Ultimo = &new_Node
				}
				ls.mostrar_particiones()

			} else {
				fmt.Println("ERROR AL MONTAR, PARTICION NO EXISTE")
				return -1

			}

		} else {
			fmt.Println("ERROR EN MONTRAR EXTENSION DESCONOCIDO")
			return -1
		}

	} else {
		fmt.Println("ERROR EN MONTRAR ARCHIVO NO EXISTE")
		return -1

	}
	return 0
}

/*____________ FIN DE INSERTAR NODO ________*/

func (ls *LISTASIMPLE) borrar_nodo(id string) {
	var nodo_actual *NODO = ls.Primero
	var nodo_anterior *NODO = nil

	var es_encontrado bool = false
	if ls.Primero != nil {
		for nodo_actual != nil && es_encontrado != true {
			if nodo_actual.Id == id {
				if nodo_actual == ls.Primero {
					ls.Primero = ls.Primero.Siguiente

				} else if nodo_actual == ls.Ultimo {
					ls.Ultimo = nodo_anterior
					ls.Ultimo.Siguiente = nil
				} else {
					nodo_anterior.Siguiente = nodo_actual.Siguiente
				}

				es_encontrado = true
			}

			nodo_anterior = nodo_actual
			nodo_actual = nodo_actual.Siguiente

			if es_encontrado == true {
				break
			}

		}

	} else {
		fmt.Println("\tLa Lista esta vacia!!!! >:v")
	}

} /*____________ fin de borrar_nodo _______*/

func (ls *LISTASIMPLE) mostrar_particiones() {
	fmt.Println("\t---------------------------------")
	fmt.Println("\t|       Particiones Montadas    |")
	fmt.Println("\t---------------------------------")
	fmt.Println("\t|        Nombre     |   ID      |")
	fmt.Println("\t---------------------------------")
	var aux *NODO = ls.Primero
	for aux != nil {
		fmt.Println("\t|\t", aux.Name, "\t|\t", aux.Id, "\t|")
		fmt.Println("\t---------------------------------")
		aux = aux.Siguiente
	}
}

func (ls *LISTASIMPLE) obtener_letra(path string, name string) {
	ls.Letra_temp = 48
	ls.Numero_temp = 0
	ls.Contador_temp = 0

	var temp *NODO = ls.Primero
	var aumentarLetra int64 = 0

	for temp != nil {

		if path == temp.Path {
			ls.Letra_temp = temp.Letra
			ls.Numero_temp = temp.Numero
			ls.Contador_temp = temp.Contador
		} else {

			ls.Contador_temp = temp.Contador
			//cout<<"\tVALOR CONTADOR:"<<contador_temp<<endl;
		}
		temp = temp.Siguiente
	}

	if ls.Letra_temp == 48 && ls.Numero_temp == 0 {
		ls.Letra_temp = 97 //aqui era A
		//cout<<"\tVALOR CONTADOR:"<<contador_temp<<endl;
		if ls.Contador_temp != 0 {
			ls.Contador_temp++
			ls.Numero_temp = ls.Contador_temp
		} else {
			ls.Contador_temp++
			ls.Numero_temp = 1
		}

	} else {
		aumentarLetra = int64(ls.Letra_temp)
		aumentarLetra++
		ls.Letra_temp = byte(aumentarLetra)
		aumentarLetra = 0
	}

}

func (ls *LISTASIMPLE) obtener_id() string {
	//201504115
	//SE UTILIZO EL CARNET DEL AUX 06
	//fmt.Println("Numero Tem: ",ls.Numero_temp)
	//strconv.Itoa(int(ls.Numero_temp))
	return "84" + string(strconv.Itoa(int(ls.Numero_temp))) + string(ls.Letra_temp)
}

//mount -path=/home/dia/disco.dk -name=Particion1

func (ls *LISTASIMPLE) buscarParticion(id string) bool {
	var temp *NODO = ls.Primero
	for temp != nil {
		if temp.Id == id {
			return true
		}
		temp = temp.Siguiente
	}

	return false
}

func (ls *LISTASIMPLE) buscar_nombre_path(path string, nombr string) bool {
	var temp *NODO = ls.Primero
	for temp != nil {
		if temp.Path == path && temp.Name == nombr {
			return true
		}
		temp = temp.Siguiente
	}

	return false
}

func (ls *LISTASIMPLE) obtenerNodo(id string) *NODO {
	var temp *NODO = ls.Primero
	for temp != nil {
		if temp.Id == id {
			return temp
		}
		temp = temp.Siguiente
	}

	return nil
}

func Log_in(path string, id string, usuario string, password string) int32 {
	var aux_nodo *NODO = listaS.obtenerNodo(id)
	if aux_nodo != nil {
		index, inicio, tamano, es_logica, fit_ := Buscar_Indice_P_E_L(aux_nodo.Path, aux_nodo.Name) //returna 4 variables
		_ = tamano

		file, err := os.OpenFile(path, os.O_RDWR, 0777)
		defer file.Close()

		if err != nil {
			log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
			return -1
		}

		if index != -1 {
			if es_logica {
				inicio += int32(unsafe.Sizeof(EBR{}))
			}

			var super SUPER_BLOQUE
			var inodo TABLA_INODOS

			file.Seek(int64(inicio), 0)
			super = Leer_SuperBloque(file, int32(unsafe.Sizeof(SUPER_BLOQUE{})))

			file.Seek(int64(super.S_inode_start)+int64(unsafe.Sizeof(TABLA_INODOS{})), 0)
			inodo = Leer_TablaInodo(file, int32(unsafe.Sizeof(TABLA_INODOS{})))

			file.Seek(int64(super.S_inode_start)+int64(unsafe.Sizeof(TABLA_INODOS{})), 0)
			copy(inodo.I_atime[:], Hora_fecha())
			Escribir_TablaInodo(file, &inodo)
			file.Close()

			actualSesion.InicioSuper = int32(inicio)

			if !actualSesion.hay_Sesion {
				//LLenenado los datos de Usuario actual
				idGU, existeUsuario := verificarDatos(usuario, password, path)
				if existeUsuario {
					actualSesion.Id_user = int32(idGU)
					actualSesion.Id_grp = int32(idGU)
					//actualSesion.InicioSuper = inicio
					actualSesion.Tipo_sistema = super.S_filesystem_type
					actualSesion.Path = aux_nodo.Path
					copy(actualSesion.Fit[:], string(fit_)) //aqui arreglar a ver que pex
					actualSesion.hay_Sesion = existeUsuario
					return 0
				}
			} else {
				fmt.Println("ERROR DEBES CERRAR SESION PARA INICIAR OTRA SESION")
				return 0
			}
			//return -1
		}

	}

	return -1
}

func verificarDatos(user string, password string, direccion string) (int32, bool) {
	file, err := os.OpenFile(direccion, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		//os.Exit(4)
		return -1, false
	}

	var cadena []byte = bytes.Repeat([]byte{0}, 400)
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS

	file.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(file, int32(unsafe.Sizeof(SUPER_BLOQUE{})))

	//Leemos el inodo del archivo users.txt
	file.Seek(int64(super.S_inode_start)+int64(unsafe.Sizeof(TABLA_INODOS{})), 0)
	inodo = Leer_TablaInodo(file, int32(unsafe.Sizeof(TABLA_INODOS{})))

	for i := 0; i < 15; i++ {
		if inodo.I_block[i] != -1 {
			var archivo BLOQUE_ARCHIVO

			file.Seek(int64(super.S_block_start)+int64(unsafe.Sizeof(BLOQUE_CARPETA{})), 0)
			for j := 0; j < int(inodo.I_block[i]); j++ {
				archivo = Leer_BloqueArchivo(file, int32(unsafe.Sizeof(BLOQUE_ARCHIVO{})))
				//fread(&archivo,sizeof(BloqueArchivo),1,fp);
			}
			copy(cadena[:], string(archivo.B_content[:]))
		}
	}

	file.Close()

	return Usuario_Contrasena(cadena, user, password)

	//return -1,false
}
func Usuario_Contrasena(cadena []byte, user string, pass string) (int32, bool) {
	cadena_sinNull := bytes.Trim([]byte(cadena), "\x00")
	grupo_usuario := bytes.Split(cadena_sinNull, []byte("\n"))

	for i := 0; i < len(grupo_usuario); i++ {
		if len(grupo_usuario[i]) > 0 {
			indice_grupo_usuario := bytes.Split([]byte(grupo_usuario[i]), []byte(","))
			if string(indice_grupo_usuario[1]) == "U" {
				if string(indice_grupo_usuario[3]) == user {
					if string(indice_grupo_usuario[4]) == pass {
						if string(indice_grupo_usuario[0]) != "0" {
							fmt.Println("CONVERTIENDOME EN STRING:", string(indice_grupo_usuario[0]))
							intID, _ := strconv.Atoi(string(indice_grupo_usuario[0]))
							var id int32 = int32(intID)
							return id, true
						} else {
							fmt.Println("ERROR EL USUARIO YA ELIMINADO PARA PODER INICIAR SESION")
							return -1, false
						}
					} else {
						fmt.Println("ERROR AUTENTIFICACION FALLIDA EN LOGIN")
						return -1, false
					}
					//return true
				}
			}
		}

	}

	fmt.Println("ERROR INCIAR LOGIN,USUARIO NO EXISTE")
	return -1, false
}

/*
	func ByteArrayToint32(arr []byte) int32 {
		//fmt.Println("AQUIIII VIENTO ^^^^^^^^^^^^^^^^^", arr)
		val := int32(0)
		size := len(arr)
		for i := 0; i < size; i++ {
			*(*uint8)(unsafe.Pointer(uintptr(unsafe.Pointer(&val)) + uintptr(i))) = arr[i]
		}
		return val
	}
*/
func Logout() int32 {
	if actualSesion.hay_Sesion {
		actualSesion.Id_user = -1
		actualSesion.Id_grp = -1
		actualSesion.InicioSuper = -1
		actualSesion.Tipo_sistema = -1
		actualSesion.Path = ""
		//temp := bytes.Repeat([]byte{0}, len(actualSesion.Fit[:]))
		copy(actualSesion.Fit[:], bytes.Repeat([]byte{0}, len(actualSesion.Fit[:])))
		actualSesion.hay_Sesion = false
		return 0

	} else {
		return 1

	}
	//return -1
}

func Crear_Mkdir(path_crear string, hay_p bool) int {
	var result int = crearCarpeta(path_crear, hay_p)
	if result == 0 {
		fmt.Println("ERROR LA CARPETA YA EXISTE")
	} else if result == 1 {
		fmt.Println("CARPETA CREADA CON EXITO")
		return 0
	} else if result == 2 {
		fmt.Println("ERROR NO TIENE PERMISO DE ESCRITURA PAR CREAR CARPETA")
	} else if result == 3 {
		fmt.Println("ERROR: NO EXISTE EL DIRECTORIO Y  NO ESTA EL PARAMETRO -P")
	}
	return -1
}

func crearCarpeta(path string, p bool) int {

	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return -1
	}

	var auxPath [500]byte
	var existe int = buscarCarpetaArchivo(fp, path)

	copy(auxPath[:], []byte(path))
	var response int = -1

	if existe != -1 {
		response = 0
	} else {
		response = nuevaCarpeta(fp, actualSesion.Fit[:], p, path, 0)
	}

	return response
}

func buscarCarpetaArchivo(fp *os.File, path string) int {
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var carpeta BLOQUE_CARPETA

	var cont int = 0
	var numInodo int = 0
	var lista []string = separa_path(path)
	cont = len(lista)

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	numInodo = int(super.S_inode_start) //Byte donde inicia el inodo

	for cont2 := 0; cont2 < cont; cont2++ {
		fp.Seek(int64(numInodo), 0)
		inodo = Leer_TablaInodo(fp, tamanoInodo)

		var siguiente int32 = 0
		for i := 0; i < 16; i++ {
			if inodo.I_block[i] != -1 { //Apuntadores directos
				var byteBloque int = byteInodoBloque(fp, int(inodo.I_block[i]), '2')
				fp.Seek(int64(byteBloque), 0)
				carpeta = Leer_BloqueCarpeta(fp, tamanoCarpeta)
				for j := 0; j < 4; j++ {
					if (cont2 == cont-1) && (strings.ToLower(aString(carpeta.B_content[j].B_name[:])) == strings.ToLower(lista[cont2])) { //Tendria que ser la carpeta
						return int(carpeta.B_content[j].B_inodo)
					} else if (cont2 != cont-1) && (strings.ToLower(aString(carpeta.B_content[j].B_name[:])) == strings.ToLower(lista[cont2])) {
						numInodo = byteInodoBloque(fp, int(carpeta.B_content[j].B_inodo), '1')
						siguiente = 1
						break
					}
				}

				if siguiente == 1 {
					break
				}

			}
		}
	}

	return -1
}

func byteInodoBloque(fp *os.File, pos int, tipo byte) int {
	var super SUPER_BLOQUE
	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	if tipo == '1' {
		return (int(super.S_inode_start) + int(tamanoInodo)*pos)
	} else if tipo == '2' {
		return (int(super.S_block_start) + int(tamanoCarpeta)*pos)
	}
	return 0
}

func nuevaCarpeta(fp *os.File, fit []byte, flagP bool, path string, index int) int {
	var super SUPER_BLOQUE
	var inodo, inodoNuevo TABLA_INODOS
	var carpeta, carpetaNueva, carpetaAux BLOQUE_CARPETA

	var copiaPath [500]byte
	var directorio [500]byte
	var nombreCarpeta [80]byte

	copy(copiaPath[:], []byte(path))
	copy(directorio[:], []byte(filepath.Dir(path))) //borrar por si las moscas
	copy(copiaPath[:], []byte(path))
	copy(nombreCarpeta[:], []byte(filepath.Base(path)))

	var cont int = 0
	var numInodo int = index
	var response int = 0

	var lista []string = separa_path(path)
	cont = len((lista))

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)

	if cont == 1 { //Solo es una carpeta '/home' | '/archivos'
		var content int = 0
		var bloque int = 0
		var libre int = buscarContentLibre(fp, numInodo, &inodo, &carpeta, &content, &bloque)
		if libre == 1 {
			//Apuntadores directos
			var permisos bool = permisosDeEscritura(int(inodo.I_perm), (inodo.I_uid == actualSesion.Id_user), (inodo.I_gid == actualSesion.Id_grp))
			if permisos || actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {
				var buffer byte = '1'
				var bitInodo int = int(buscarBit1(fp, 'I', fit[:]))
				//Agregamos la carpeta al espacio libre en el bloque
				carpeta.B_content[content].B_inodo = int32(bitInodo)
				copy(carpeta.B_content[content].B_name[:], nombreCarpeta[:])
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*inodo.I_block[bloque]), 0)
				Escribir_BloqueCarpeta(fp, &carpeta)

				//Creamos el nuevo inodo
				inodoNuevo = crearInodo(0, '0', 664)
				var bitBloque int = int(buscarBit1(fp, 'B', fit[:]))
				inodoNuevo.I_block[0] = int32(bitBloque)
				fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(bitInodo)), 0)
				Escribir_TablaInodo(fp, &inodoNuevo)

				//Creamos el nuevo bloque carpeta
				carpetaNueva = crearBloqueCarpeta()
				carpetaNueva.B_content[0].B_inodo = int32(bitInodo)
				carpetaNueva.B_content[1].B_inodo = int32(numInodo)
				copy(carpetaNueva.B_content[0].B_name[:], ".")
				copy(carpetaNueva.B_content[1].B_name[:], "..")
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*int32(bitBloque)), 0)
				Escribir_BloqueCarpeta(fp, &carpetaNueva)
				//Guardamos los bits en los bitmaps
				fp.Seek(int64(super.S_bm_inode_start+int32(bitInodo)), 0)
				Escribir_Byte(fp, &buffer)
				fp.Seek(int64(super.S_bm_block_start+int32(bitBloque)), 0)
				Escribir_Byte(fp, &buffer)
				//Sobreescribimos el super bloque
				super.S_free_inodes_count = super.S_free_inodes_count - 1
				super.S_free_blocks_count = super.S_free_blocks_count - 1
				super.S_first_ino = super.S_first_ino + 1
				super.S_first_blo = super.S_first_blo + 1
				fp.Seek(int64(actualSesion.InicioSuper), 0)
				Escribir_SuperBloque(fp, &super)
				return 1
			} else {
				return 2
			}

		} else if libre == 0 { //Todos bloques estan llenos

			fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(numInodo)), 0)
			inodo = Leer_TablaInodo(fp, tamanoInodo)
			for i := 0; i < 16; i++ {

				if inodo.I_block[i] == -1 {
					bloque = i
					break
				}

			}

			//Apuntadores directos
			var permissions bool = permisosDeEscritura(int(inodo.I_perm), (inodo.I_uid == actualSesion.Id_user), (inodo.I_gid == actualSesion.Id_grp))
			if permissions || actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {

				var buffer byte = '1'
				var bitBloque int = int(buscarBit1(fp, 'B', fit[:]))

				inodo.I_block[bloque] = int32(bitBloque)
				//Sobreescribimos el inodo
				fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(numInodo)), 0)
				//inodo = Leer_TablaInodo(fp, tamanoInodo)
				Escribir_TablaInodo(fp, &inodo)
				//Bloque carpeta auxiliar
				var bitInodo int = int(buscarBit1(fp, 'I', fit[:]))
				carpetaAux = crearBloqueCarpeta()
				carpetaAux.B_content[0].B_inodo = int32(bitInodo)
				copy(carpetaAux.B_content[0].B_name[:], nombreCarpeta[:])
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*int32(bitBloque)), 0)
				Escribir_BloqueCarpeta(fp, &carpetaAux)
				//Escribimos el bit en el bitmap de blqoues
				fp.Seek(int64(super.S_bm_block_start+int32(bitBloque)), 0)
				Escribir_Byte(fp, &buffer)

				//Creamos el nuevo inodo
				inodoNuevo = crearInodo(0, '0', 664)
				bitBloque = int(buscarBit1(fp, 'B', fit[:]))
				inodoNuevo.I_block[0] = int32(bitBloque)
				fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(bitInodo)), 0)
				Escribir_TablaInodo(fp, &inodoNuevo)
				//Escribimos el bit en el bitmap de inodos
				fp.Seek(int64(super.S_bm_inode_start+int32(bitInodo)), 0)
				Escribir_Byte(fp, &buffer)
				//Creamos el nuevo bloque carpeta
				carpetaNueva = crearBloqueCarpeta()
				carpetaNueva.B_content[0].B_inodo = int32(bitInodo)
				carpetaNueva.B_content[1].B_inodo = int32(numInodo)
				copy(carpetaNueva.B_content[0].B_name[:], ".")
				copy(carpetaNueva.B_content[1].B_name[:], "..")
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*int32(bitBloque)), 0)
				Escribir_BloqueCarpeta(fp, &carpetaNueva)

				//Guardamos el bit en el bitmap de bloques
				fp.Seek(int64(super.S_bm_block_start+int32(bitBloque)), 0)
				Escribir_Byte(fp, &buffer)

				//Sobreescribimos el super bloque
				super.S_free_inodes_count = super.S_free_inodes_count - 1
				super.S_free_blocks_count = super.S_free_blocks_count - 2
				super.S_first_ino = super.S_first_ino + 1
				super.S_first_blo = super.S_first_blo + 2
				fp.Seek(int64(actualSesion.InicioSuper), 0)
				Escribir_SuperBloque(fp, &super)
				return 1
			} else {
				return 2
			}

		}
	} else { //Es un directorio '/home/usac/archivos'
		//Verificar que exista el directorio
		var existe int = buscarCarpetaArchivo(fp, aString(directorio[:]))
		if existe == -1 {
			if flagP {
				var index int = 0
				var aux string = ""
				//Crear posibles carpetas inexistentes
				for i := 0; i < cont; i++ {
					aux += "/" + lista[i]
					var dir [500]byte
					var auxDir [500]byte
					copy(dir[:], []byte(aux))
					copy(auxDir[:], []byte(aux))
					var carpeta int = buscarCarpetaArchivo(fp, aString(dir[:]))
					if carpeta == -1 {
						response = nuevaCarpeta(fp, fit[:], false, aString(auxDir[:]), index)
						if response == 2 {
							break
						}
						copy(auxDir[:], []byte(aux))
						index = buscarCarpetaArchivo(fp, aString(auxDir[:]))
					} else {
						index = carpeta
					}

				}
			} else {
				return 3
			}

		} else { //Solo crear la carpeta en el directorio
			var dir [100]byte
			copy(dir[:], []byte("/"))
			concatenar(dir[:], nombreCarpeta[:], len(aString(dir[:])))
			return nuevaCarpeta(fp, fit[:], false, aString(dir[:]), existe)
		}
	}

	return response
}

/*__________________________________________________________*/

func buscarContentLibre(fp *os.File, numInodo int, inodo *TABLA_INODOS, carpeta *BLOQUE_CARPETA, content *int, bloque *int) int {
	var libre int = 0
	var super SUPER_BLOQUE
	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(numInodo)), 0)
	*inodo = Leer_TablaInodo(fp, tamanoInodo)

	//Buscamos un espacio libre en el bloque carpeta
	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			//Apuntadores directos
			fp.Seek(int64(super.S_block_start+tamanoCarpeta*inodo.I_block[i]), 0)
			*carpeta = Leer_BloqueCarpeta(fp, tamanoCarpeta)
			for j := 0; j < 4; j++ {
				if carpeta.B_content[j].B_inodo == -1 {
					libre = 1
					*bloque = i
					*content = j
					break
				}
			}
		}

		if libre == 1 {
			break
		}
	}

	return libre
}

/*________________________________________________________________________*/
func permisosDeEscritura(permisos int, flagUser bool, flagGroup bool) bool {
	var aux string = strconv.Itoa(permisos)
	var propietario byte = aux[0]
	var grupo byte = aux[1]
	var otros byte = aux[2]

	if (propietario == '2' || propietario == '3' || propietario == '6' || propietario == '7') && flagUser {
		return true
	} else if (grupo == '2' || grupo == '3' || grupo == '6' || grupo == '7') && flagGroup {
		return true
	} else if otros == '2' || otros == '3' || otros == '6' || otros == '7' {
		return true
	}

	return false
}

func crearInodo(size int, type_ byte, perm int) TABLA_INODOS {
	var inodo TABLA_INODOS
	inodo.I_uid = actualSesion.Id_user
	inodo.I_gid = actualSesion.Id_grp
	inodo.I_size = int32(size)
	copy(inodo.I_atime[:], []byte(Hora_fecha()))
	copy(inodo.I_ctime[:], []byte(Hora_fecha()))
	copy(inodo.I_mtime[:], []byte(Hora_fecha()))
	for i := 0; i < 16; i++ {
		inodo.I_block[i] = -1
	}

	inodo.I_type = type_
	inodo.I_perm = int32(perm)
	return inodo
}

func crearBloqueCarpeta() BLOQUE_CARPETA {
	var carpeta BLOQUE_CARPETA

	for i := 0; i < 4; i++ {
		copy(carpeta.B_content[i].B_name[:], "")
		carpeta.B_content[i].B_inodo = -1
	}

	return carpeta
}

func Hora_fecha() string {
	tiemp_hora := time.Now()
	return tiemp_hora.Format("01-02-2006 15:04:05")
}

func Crear_archivo(size int32, path string, fit string, unit string) int {
	var size_archivo int32 = 0
	var fit_archivo string = ""

	if Obtener_extension_archivo(path) == ".dk" {
		//_ = size_archivo
		//_ = fit_archivo

		//validando el unit del archivo
		if unit == "k" {
			size_archivo = size * 1024
		} else if unit == "m" {
			size_archivo = size * 1024 * 1024
		} else if unit == "" {
			size_archivo = size * 1024 * 1024
		}

		//validando fit del archivo
		if fit == "bf" {
			fit_archivo = fit
		} else if fit == "ff" {
			fit_archivo = fit
		} else if fit == "wf" {
			fit_archivo = fit
		} else if fit == "" {
			fit_archivo = "ff"
		}

		var buffer [1024]byte
		for i := 0; i < 1024; i++ {
			buffer[i] = 0
		}
		binarios := &buffer
		var data_buffer bytes.Buffer
		binary.Write(&data_buffer, binary.BigEndian, binarios)

		mi_archivo, err := os.Create(path)
		defer mi_archivo.Close()

		if err != nil {
			log.Fatal(err)
			return -1
		}
		var j int32
		for j = 0; j < (size_archivo / 1024); j++ {
			EscribirBytes(mi_archivo, data_buffer.Bytes())
		}

		masterBoot := &MBR{}
		masterBoot.Mbr_tamano = int32(size_archivo)
		copy(masterBoot.Mbr_fecha_creacion[:], Hora_fecha())
		masterBoot.Mbr_disk_signature = int32(Numero_Random())
		copy(masterBoot.Disk_fit[:], fit_archivo)

		for i := 0; i < 4; i++ {
			masterBoot.Mbr_partition[i].Part_status = 0
			masterBoot.Mbr_partition[i].Part_type = 0
			copy(masterBoot.Mbr_partition[i].Part_fit[:], "")
			masterBoot.Mbr_partition[i].Part_start = -1
			masterBoot.Mbr_partition[i].Part_size = -1
			copy(masterBoot.Mbr_partition[i].Part_name[:], "")

		}

		mi_archivo.Seek(0, 0)
		var master_buffer bytes.Buffer
		binary.Write(&master_buffer, binary.BigEndian, masterBoot)
		EscribirBytes(mi_archivo, master_buffer.Bytes())

		mi_archivo.Close()

	} else {
		return -1
	}

	return 0
}

func EscribirBytes(file *os.File, bytes []byte) {
	_, err := file.Write(bytes)

	if err != nil {
		log.Fatal(err)
	}

}

func Numero_Random() int32 {
	var minimo int32 = 1
	var maximo int32 = 200

	return int32(rand.Intn(int(maximo) - int(minimo) + int(minimo)))
}

func Crear_carpetas(direccion string) {
	path_temp := filepath.Dir(direccion)

	if _, err := os.Stat(path_temp); os.IsNotExist(err) {
		// path/to/whatever does not exist
		if existeError := os.MkdirAll(path_temp, os.ModePerm); existeError != nil {
			log.Fatal(existeError)
		}
	}

}

func Obtener_extension_archivo(path string) string {
	return strings.ToLower(filepath.Ext(path))
}

func Obtener_nombe_archivo(path string, extension string) string {

	return strings.TrimSuffix(filepath.Base(path), extension)
}

func Crear_Mkfile(path string, size_archivo int, cont_archivo string, hay_r bool) int {
	var result int = crearArchivo(path, size_archivo, cont_archivo, hay_r)
	if result == 1 {
		fmt.Println("ARCHIVO CREADA CORRECTAMENTE")
		return 0
	} else if result == 2 {
		fmt.Println("ERROR USUARIO NO TIEME PERMISO PARA ESCRITURA")
	} else if result == 3 {
		fmt.Println("ERROR EL ARCHIVO CONTENIDO NO EXISTE")
		return 0
	} else if result == 4 {
		fmt.Println("ERROR NO EXISTE LA DIRECCION Y NO TIENE PARAMETRO -R")
		return 0
	}
	return -1
}

func crearArchivo(path string, size int, cont string, p bool) int {
	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return -1
	}

	var auxPath [500]byte
	var auxPath2 [500]byte
	copy(auxPath[:], []byte(path))
	copy(auxPath2[:], []byte(path))
	var existe int = buscarCarpetaArchivo(fp, aString(auxPath[:]))
	copy(auxPath[:], []byte(path))
	var response int = -1

	if existe != -1 {
		response = 0
	} else {
		response = nuevoArchivo(fp, actualSesion.Fit[:], p, aString(auxPath[:]), size, cont, 0, aString(auxPath2[:]))
	}
	return response
}

func nuevoArchivo(fp *os.File, fit []byte, flagP bool, path string, size int, contenido string, index int, auxPath string) int {
	var super SUPER_BLOQUE
	var inodo, inodoNuevo TABLA_INODOS
	var carpeta, carpetaNueva BLOQUE_CARPETA

	var lista = separa_path(path)
	var copiaPath [500]byte
	var directorio [500]byte
	var nombreCarpeta [80]byte
	var content string = ""
	var contentSize string = "0123456789"

	copy(copiaPath[:], []byte(path))
	copy(directorio[:], []byte((filepath.Dir(aString(copiaPath[:])))))
	copy(copiaPath[:], []byte(path))
	copy(nombreCarpeta[:], []byte(filepath.Base(aString(copiaPath[:]))))
	copy(copiaPath[:], []byte(path))

	var cont int32 = 0
	var numInodo int32 = int32(index)
	var finalSize int = size
	cont = int32(len(lista))

	if len(contenido) != 0 {
		content = Leer_Cualquier_Archivo(contenido)
		if content != "" {
			finalSize = len(content)
		} else {
			return 3
		}

	}

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)

	if cont == 1 {
		var bloque int = 0
		var b_content int = 0
		var libre int = buscarContentLibre(fp, int(numInodo), &inodo, &carpeta, &b_content, &bloque)

		if libre == 1 {
			var permisos bool = permisosDeEscritura(int(inodo.I_perm), (inodo.I_uid == actualSesion.Id_user), (inodo.I_gid == actualSesion.Id_grp))
			if permisos || (actualSesion.Id_user == 1 && actualSesion.Id_grp == 1) {
				var buffer byte = '1'
				var buffer2 byte = '2'

				//Agregamos el archivo al bloque correspondiente
				var bitInodo int = int(buscarBit1(fp, 'I', fit[:]))
				carpeta.B_content[b_content].B_inodo = int32(bitInodo)
				copy(carpeta.B_content[b_content].B_name[:], nombreCarpeta[:])
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*inodo.I_block[bloque]), 0)
				Escribir_BloqueCarpeta(fp, &carpeta)

				//Creamos el nuevo inodo archivo
				inodoNuevo = crearInodo(0, '1', 664)
				fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(bitInodo)), 0)
				Escribir_TablaInodo(fp, &inodoNuevo)

				//Registramos el inodo en el bitmap
				fp.Seek(int64(super.S_bm_inode_start+int32(bitInodo)), 0)
				Escribir_Byte(fp, &buffer)

				//Si viene el parametro -size/-cont
				if finalSize != 0 {

					var n float64 = float64(finalSize) / float64(64)
					var numBloques int = int(math.Ceil(n))
					var caracteres int = finalSize
					var charNum int = 0  //size_t
					var contChar int = 0 //size_t
					numInodo = int32(buscarCarpetaArchivo(fp, auxPath))
					for i := 0; i < numBloques; i++ {
						var archivo BLOQUE_ARCHIVO
						memseT(archivo.B_content[:], 0)

						var bitBloque int32 = buscarBit1(fp, 'B', fit[:])
						//Registramos el bloque en el bitmap
						fp.Seek(int64(super.S_bm_block_start+bitBloque), 0)
						Escribir_Byte(fp, &buffer2)

						if caracteres > 64 {
							for j := 0; j < 64; j++ {
								if len(content) != 0 { //-cont
									archivo.B_content[j] = content[contChar]
									contChar++
								} else { //-size
									archivo.B_content[j] = contentSize[charNum]
									charNum++
									if charNum == 10 {
										charNum = 0
									}
								}
							}
							//Guardamos el bloque en el respectivo inodo archivo
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							inodo = Leer_TablaInodo(fp, tamanoInodo)

							inodo.I_block[i] = bitBloque
							inodo.I_size = int32(finalSize)
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							Escribir_TablaInodo(fp, &inodo)
							//Guardamos el bloque
							fp.Seek(int64(super.S_block_start+tamanoArchivo*bitBloque), 0)
							Escribir_BloqueArchivo(fp, &archivo)

							caracteres -= 64
						} else {
							for j := 0; j < caracteres; j++ {
								if len(content) != 0 {
									archivo.B_content[j] = content[contChar]
									contChar++
								} else {
									archivo.B_content[j] = contentSize[charNum]
									charNum++
									if charNum == 10 {
										charNum = 0
									}

								}
							}
							//Guardamos el bloque en el respectivo inodo archivo
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							inodo = Leer_TablaInodo(fp, tamanoInodo)
							inodo.I_block[i] = bitBloque
							inodo.I_size = int32(finalSize)
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							Escribir_TablaInodo(fp, &inodo)

							//Guardamos el bloque
							fp.Seek(int64(super.S_block_start+tamanoArchivo*bitBloque), 0)
							Escribir_BloqueArchivo(fp, &archivo)

						}

					}
					//Modificamos el super bloque
					super.S_free_blocks_count = super.S_free_blocks_count - int32(numBloques)
					super.S_free_inodes_count = super.S_free_inodes_count - 1
					super.S_first_ino = super.S_first_ino + 1
					super.S_first_blo = super.S_first_blo + int32(numBloques)
					fp.Seek(int64(actualSesion.InicioSuper), 0)
					Escribir_SuperBloque(fp, &super)
					//fwrite(&super,sizeof(SuperBloque),1,stream)
					return 1
				}
				super.S_free_inodes_count = super.S_free_inodes_count - 1
				super.S_first_ino = super.S_first_ino + 1
				fp.Seek(int64(actualSesion.InicioSuper), 0)
				Escribir_SuperBloque(fp, &super)
				return 1
			} else {
				return 2
			}

		} else { //Todos los bloques estan llenos
			fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
			inodo = Leer_TablaInodo(fp, tamanoInodo)

			for i := 0; i < 16; i++ {
				if inodo.I_block[i] == -1 {
					bloque = i
					break
				}
			}
			//Apuntadores directos
			var permisos bool = permisosDeEscritura(int(inodo.I_perm), (inodo.I_uid == actualSesion.Id_user), (inodo.I_gid == actualSesion.Id_grp))
			if permisos || (actualSesion.Id_user == 1 && actualSesion.Id_grp == 1) {
				var buffer byte = '1'
				var buffer2 byte = '2'

				var bitBloque int32 = buscarBit1(fp, 'B', fit[:])
				//Guardamos el bloque en el inodo
				inodo.I_block[bloque] = bitBloque
				fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
				Escribir_TablaInodo(fp, &inodo)

				//Creamos el nuevo bloque carpeta
				var bitInodo int32 = buscarBit1(fp, 'I', fit[:])
				carpetaNueva.B_content[0].B_inodo = bitInodo
				carpetaNueva.B_content[1].B_inodo = -1
				carpetaNueva.B_content[2].B_inodo = -1
				carpetaNueva.B_content[3].B_inodo = -1
				copy(carpetaNueva.B_content[0].B_name[:], nombreCarpeta[:])
				copy(carpetaNueva.B_content[1].B_name[:], "")
				copy(carpetaNueva.B_content[2].B_name[:], "")
				copy(carpetaNueva.B_content[3].B_name[:], "")
				fp.Seek(int64(super.S_block_start+tamanoCarpeta*bitBloque), 0)
				Escribir_BloqueCarpeta(fp, &carpetaNueva)

				//Registramos el bloque en el bitmap
				fp.Seek(int64(super.S_bm_block_start+bitBloque), 0)
				Escribir_Byte(fp, &buffer)

				//Creamos el nuevo inodo
				inodoNuevo = crearInodo(0, '1', 664)
				fp.Seek(int64(super.S_inode_start+tamanoInodo*bitInodo), 0)
				Escribir_TablaInodo(fp, &inodoNuevo)

				fp.Seek(int64(super.S_inode_start+tamanoInodo*bitInodo), 0)
				Escribir_TablaInodo(fp, &inodoNuevo)

				//Registramos el inodo en el bitmap
				fp.Seek(int64(super.S_bm_inode_start+bitInodo), 0)
				Escribir_Byte(fp, &buffer)

				//Si viene el parametro -size/-cont
				if finalSize != 0 {
					var n float64 = float64(finalSize) / float64(64)
					var numBloques int = int(math.Ceil(n))
					var caracteres int = finalSize
					var charNum int = 0
					var contChar int = 0
					numInodo = int32(buscarCarpetaArchivo(fp, auxPath))
					for i := 0; i < numBloques; i++ {
						var archivo BLOQUE_ARCHIVO
						memseT(archivo.B_content[:], 0)
						//Apuntadores simples
						var bitBloque int32 = buscarBit1(fp, 'B', fit[:])
						//Registramos el bloque en el bitmap
						fp.Seek(int64(super.S_bm_block_start+bitBloque), 0)
						Escribir_Byte(fp, &buffer2)

						if caracteres > 64 {
							for j := 0; j < 64; j++ {
								if len(content) != 0 { //-cont
									archivo.B_content[j] = content[contChar]
									contChar++
								} else { //-size
									archivo.B_content[j] = contentSize[charNum]
									charNum++
									if charNum == 10 {
										charNum = 0
									}
								}
							}
							//Guardamos el bloque en el respectivo inodo archivo
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							inodo = Leer_TablaInodo(fp, tamanoInodo)

							inodo.I_block[i] = bitBloque
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							Escribir_TablaInodo(fp, &inodo)

							//Guardamos el bloque
							fp.Seek(int64(super.S_block_start+tamanoArchivo*bitBloque), 0)
							Escribir_BloqueArchivo(fp, &archivo)

							caracteres -= 64
						} else {
							for j := 0; j < caracteres; j++ {
								if len(content) != 0 {
									archivo.B_content[j] = content[contChar]
									contChar++
								} else {
									archivo.B_content[j] = contentSize[charNum]
									charNum++
									if charNum == 10 {
										charNum = 0
									}
								}
							}
							//Guardamos el bloque en el respectivo inodo archivo
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							inodo = Leer_TablaInodo(fp, tamanoInodo)

							inodo.I_block[i] = bitBloque
							fp.Seek(int64(super.S_inode_start+tamanoInodo*numInodo), 0)
							Escribir_TablaInodo(fp, &inodo)

							//Guardamos el bloque
							fp.Seek(int64(super.S_block_start+tamanoArchivo*bitBloque), 0)
							Escribir_BloqueArchivo(fp, &archivo)

						}

					}
					//Modificamos el super bloque
					super.S_free_blocks_count = super.S_free_blocks_count - int32(numBloques)
					super.S_free_inodes_count = super.S_free_inodes_count - 1
					super.S_first_ino = super.S_first_ino + 1
					super.S_first_blo = super.S_first_blo + int32(numBloques)
					fp.Seek(int64(actualSesion.InicioSuper), 0)
					Escribir_SuperBloque(fp, &super)

					return 1
				}
				super.S_free_inodes_count = super.S_free_inodes_count - 1
				super.S_first_ino = super.S_first_ino + 1
				fp.Seek(int64(actualSesion.InicioSuper), 0)
				Escribir_SuperBloque(fp, &super)

				return 1
			} else {
				return 2
			}

		}
	} else { //Directorio
		var existe int = buscarCarpetaArchivo(fp, aString(directorio[:]))
		if existe == -1 {
			if flagP {
				var index int = 0
				var aux string = ""
				//Crear posibles carpetas inexistentes
				for i := 0; i < int(cont); i++ {
					if int32(i) == cont-1 {
						var dir [100]byte
						copy(dir[:], []byte("/"))
						concatenar(dir[:], nombreCarpeta[:], len(aString(dir[:])))
						return nuevoArchivo(fp, fit[:], false, aString(dir[:]), size, contenido, index, auxPath)
					} else {
						aux += "/" + lista[i]
						var dir [500]byte
						var auxDir [500]byte
						copy(dir[:], []byte(aux))
						copy(auxDir[:], []byte(aux))
						var carpeta int = buscarCarpetaArchivo(fp, aString(dir[:]))
						if carpeta == -1 {
							nuevaCarpeta(fp, fit[:], false, aString(auxDir[:]), index)
							copy(auxDir[:], []byte(aux))
							index = buscarCarpetaArchivo(fp, aString(auxDir[:]))
						} else {
							index = carpeta
						}
					}
				}
			} else {
				return 4
			}
		} else { //Crear el archivo en el directorio
			var dir [100]byte
			copy(dir[:], []byte("/"))
			concatenar(dir[:], nombreCarpeta[:], len(aString(dir[:])))
			return nuevoArchivo(fp, fit, false, aString(dir[:]), size, contenido, existe, auxPath)
		}
	}

	return 0
}

func EjecutarMkfs(id string, type_ string) int32 {
	var aux *NODO = listaS.obtenerNodo(id)
	if aux != nil {
		index, inicio, tamano, es_logica, _ := Buscar_Indice_P_E_L(aux.Path, aux.Name)
		if index != -1 {
			if es_logica {
				inicio += int32(unsafe.Sizeof(EBR{}))
			}

			//Ver_particion_Primaria_Extendida_Logica(aux.Path)
			if type_ == "full" {
				formatearEXT2(inicio, tamano, aux.Path)
			} else if type_ == "" {
				formatearEXT2(inicio, tamano, aux.Path)
			} else {
				fmt.Println("ERROR  VALOR DESCONOCIDO DE TYPE: ", type_)
				return -1
			}
			//Ver_particion_Primaria_Extendida_Logica(aux.Path)
		} else {
			fmt.Println("ERROR  NO COINCIDE EN MKFS")
			return -1
		}
	} else {
		fmt.Println("ERROR ID NO MONTADA")
		return -1
	}

	return 0
}

func Buscar_Indice_P_E_L(path string, name string) (int32, int32, int32, bool, []byte) {

	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		os.Exit(4)
	}
	masterBoot := MBR{}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(masterBoot))
	masterBoot = ObtenerInforMbr(file, tamano_masterBoot)

	var indice_extendia int32 = -1
	for i := 0; i < 4; i++ {
		if string(bytes.Trim(masterBoot.Mbr_partition[i].Part_name[:], "\x00")) == name {
			file.Close()

			return int32(i), masterBoot.Mbr_partition[i].Part_start, masterBoot.Mbr_partition[i].Part_size, false, masterBoot.Mbr_partition[i].Part_fit[:]
		}
		if string(masterBoot.Mbr_partition[i].Part_type) == "e" {
			indice_extendia = int32(i)
		}
	}

	if indice_extendia != -1 {
		//aqui va lo de la logica
		extendBoot := EBR{}
		file.Seek(int64(masterBoot.Mbr_partition[indice_extendia].Part_start), 0) //(cantidad_bytes_desplazar,origen_de_archivo)
		extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
		for {
			//fmt.Println("Nombre Particion: ",string(extendBoot.Part_name[:])," NOMBRE COMPARADO: ",name)
			//fmt.Println("Tamano Partcicion: ",len(string(bytes.Trim(extendBoot.Part_name[:],"\x00")))," Tamano Comparado: ",len(name))
			//se utiliza bytes.Trimp para eliminar los caracteres nulos y comparar el nombre xD
			if string(bytes.Trim(extendBoot.Part_name[:], "\x00")) == name {
				return indice_extendia, extendBoot.Part_start, extendBoot.Part_size, true, extendBoot.Part_fit[:]
			}

			if extendBoot.Part_next == -1 {
				break
			}
			file.Seek(int64(extendBoot.Part_next), 0)
			extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{}))) //ruta quemada por el momento xD
		}
	}
	file.Close()
	var temp []byte
	return -1, 0, 0, false, temp
}

/*___________________________________ 	INICIO DE EXT2 _______________________________________*/
func formatearEXT2(inicio int32, tamano int32, direccion string) {
	var n float64 = (float64(tamano) - float64(unsafe.Sizeof(SUPER_BLOQUE{}))) / (4 + float64(unsafe.Sizeof(TABLA_INODOS{})) + 3*float64(unsafe.Sizeof(BLOQUE_ARCHIVO{})))
	var num_estructuras int32 = int32(math.Floor(n)) //Numero de inodos
	var num_bloques int32 = 3 * num_estructuras      //Numero de bloques

	//fmt.Println("NUMERO DE BITMAP INODO", num_estructuras)
	//fmt.Println("NUMERO DE BITMAP BLOQUE", num_bloques)

	var super SUPER_BLOQUE
	super.S_filesystem_type = 2
	super.S_inodes_count = num_estructuras
	super.S_blocks_count = num_bloques
	super.S_free_blocks_count = num_bloques - 2
	super.S_free_inodes_count = num_estructuras - 2
	copy(super.S_mtime[:], Hora_fecha())
	//copy(super.S_umtime[:], "")
	super.S_mnt_count = 0
	super.S_magic = 0xEF53
	super.S_inode_size = int32(unsafe.Sizeof(TABLA_INODOS{}))
	super.S_block_size = int32(unsafe.Sizeof(BLOQUE_ARCHIVO{}))
	super.S_first_ino = 2
	super.S_first_blo = 2
	super.S_bm_inode_start = inicio + int32(unsafe.Sizeof(SUPER_BLOQUE{}))
	super.S_bm_block_start = inicio + int32(unsafe.Sizeof(SUPER_BLOQUE{})) + num_estructuras
	super.S_inode_start = inicio + int32(unsafe.Sizeof(SUPER_BLOQUE{})) + num_estructuras + num_bloques
	super.S_block_start = inicio + int32(unsafe.Sizeof(SUPER_BLOQUE{})) + num_estructuras + num_bloques + (int32(unsafe.Sizeof(TABLA_INODOS{})) * num_estructuras)

	var buffer_Cero byte = 48              //Cero para bitmap no usado
	var buffer_Carpeta_Inodo_uno byte = 49 //Uno para representar Bloque Carpeta
	var buffer_Archivo byte = 50           //Dos para representar Bloque Archivo

	file, err := os.OpenFile(direccion, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		os.Exit(4)
	}

	/*___________________ SUPERBLOQUE ____________________*/
	file.Seek(int64(inicio), 0)
	Escribir_SuperBloque(file, &super)
	/*___________________ BITMAP DE INODOS _______________*/
	for i := 0; i < int(num_estructuras); i++ {
		file.Seek(int64(super.S_bm_inode_start)+int64(i), 0)
		Escribir_Byte(file, &buffer_Cero)
	}
	/*___________________ bit para / y users.txt en BM ____________*/
	file.Seek(int64(super.S_bm_inode_start), 0)
	Escribir_Byte(file, &buffer_Carpeta_Inodo_uno) //Para Root
	Escribir_Byte(file, &buffer_Carpeta_Inodo_uno) //Para archivo usuario

	/*_____________________ BITMAP DE BLOQUES ____________________*/
	for i := 0; i < int(num_bloques); i++ {
		file.Seek(int64(super.S_bm_block_start)+int64(i), 0)
		Escribir_Byte(file, &buffer_Cero)
	}
	/*____________________ bit para / y users.txt en BM ___________*/
	file.Seek(int64(super.S_bm_block_start), 0)
	Escribir_Byte(file, &buffer_Carpeta_Inodo_uno) //
	Escribir_Byte(file, &buffer_Archivo)

	/*_____________________ inodo para carpeta root ________________*/
	var inodoTabla TABLA_INODOS

	inodoTabla.I_uid = 1
	inodoTabla.I_gid = 1
	inodoTabla.I_size = 0
	copy(inodoTabla.I_atime[:], Hora_fecha())
	copy(inodoTabla.I_ctime[:], Hora_fecha())
	copy(inodoTabla.I_mtime[:], Hora_fecha())
	inodoTabla.I_block[0] = 0
	for i := 1; i < 16; i++ {
		inodoTabla.I_block[i] = -1
	}
	inodoTabla.I_type = 48
	inodoTabla.I_perm = 664
	file.Seek(int64(super.S_inode_start), 0)
	Escribir_TablaInodo(file, &inodoTabla)
	/*_____________________Bloque para carpeta root___________________*/
	var bloqueCarpeta BLOQUE_CARPETA

	copy(bloqueCarpeta.B_content[0].B_name[:], ".") //Actual (el mismo) poisble error con copy ya qye sino F haber que pex
	bloqueCarpeta.B_content[0].B_inodo = 0

	copy(bloqueCarpeta.B_content[1].B_name[:], "..") //Padre //verificar que copy no copie todo el contenido en el arreglo de bytes sino F
	bloqueCarpeta.B_content[1].B_inodo = 0

	copy(bloqueCarpeta.B_content[2].B_name[:], "users.txt")
	bloqueCarpeta.B_content[2].B_inodo = 1

	copy(bloqueCarpeta.B_content[3].B_name[:], ".")
	bloqueCarpeta.B_content[3].B_inodo = -1

	file.Seek(int64(super.S_block_start), 0)
	Escribir_BloqueCarpeta(file, &bloqueCarpeta)

	/*____________________ inodo para users.txt ____________________*/
	inodoTabla.I_uid = 1
	inodoTabla.I_gid = 1
	inodoTabla.I_size = 27
	copy(inodoTabla.I_atime[:], Hora_fecha())
	copy(inodoTabla.I_ctime[:], Hora_fecha())
	copy(inodoTabla.I_mtime[:], Hora_fecha())
	inodoTabla.I_block[0] = 1
	for i := 1; i < 16; i++ {
		inodoTabla.I_block[i] = -1
	}
	inodoTabla.I_type = 49
	inodoTabla.I_perm = 664
	file.Seek(int64(super.S_inode_start)+int64(unsafe.Sizeof(TABLA_INODOS{})), 0)
	Escribir_TablaInodo(file, &inodoTabla)
	/*___________________ Bloque para users.txt _____________________*/
	var archivo BLOQUE_ARCHIVO
	copy(archivo.B_content[:], "\x00")
	//memset(archivo.B_content, 0, sizeof(archivo.B_content))
	copy(archivo.B_content[:], "1,G,root\n1,U,root,root,123\n")
	file.Seek(int64(super.S_block_start)+int64(unsafe.Sizeof(BLOQUE_CARPETA{})), 0)
	Escribir_BloqueArchivo(file, &archivo)

	//fmt.Println("EXT2...")
	//fmt.Println("DISCO FORMATEADO CON EXITO")
	file.Close()
}

/*___________________________________ FIN DE EXT2 ____________________________________________*/

/*___________________________________ INICIO DE MODIFICAR ____________________________________*/

func Escribir_SuperBloque(file *os.File, superBloque *SUPER_BLOQUE) {
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, superBloque)
	EscribirBytes(file, master_buffer.Bytes())
}

/*___________________________________  FIN DE MODIFICAR SUPER BLOQUE _________________________*/

/*___________________________________ INICIO DE MODIFICAR BYE ____________________________________*/

func Escribir_Byte(file *os.File, mibyte *byte) {
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, mibyte)
	EscribirBytes(file, master_buffer.Bytes())
}

/*___________________________________  FIN DE MODIFICAR BYTE _________________________*/

/*___________________________________ INICIO DE MODIFICAR TABLA INODOS____________________________________*/

func Escribir_TablaInodo(file *os.File, tablaInodo *TABLA_INODOS) {
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, tablaInodo)
	EscribirBytes(file, master_buffer.Bytes())
}

/*___________________________________  FIN DE MODIFICAR TABLA INODOS _________________________*/

/*___________________________________ INICIO DE MODIFICAR TABLA INODOS____________________________________*/

func Escribir_BloqueCarpeta(file *os.File, bloqueCarpeta *BLOQUE_CARPETA) {
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, bloqueCarpeta)
	EscribirBytes(file, master_buffer.Bytes())
}

/*___________________________________  FIN DE MODIFICAR TABLA INODOS _________________________*/

/*___________________________________ INICIO DE MODIFICAR TABLA INODOS____________________________________*/

func Escribir_BloqueArchivo(file *os.File, bloqueArchiv *BLOQUE_ARCHIVO) {
	var master_buffer bytes.Buffer
	binary.Write(&master_buffer, binary.BigEndian, bloqueArchiv)
	EscribirBytes(file, master_buffer.Bytes())
}

/*___________________________________  FIN DE MODIFICAR TABLA INODOS _________________________*/

/*##############################################################-############################*/
/*___________________________________ INICIO DE LEER SUPERBLOQUE ____________________________________*/
func Leer_SuperBloque(file *os.File, tamano_leer int32) SUPER_BLOQUE {

	superBloque := SUPER_BLOQUE{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &superBloque)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return SUPER_BLOQUE{}
	}
	return superBloque
}

/*___________________________________ FIN DE LEER SUPERBLOQUE ____________________________________*/

/*___________________________________ INICIO DE LEER BYE ____________________________________*/

func Leer_Byte(file *os.File, tamano_leer int32) byte {
	var leerByt byte
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &leerByt)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return 0
	}
	return leerByt
}

/*___________________________________  FIN DE LEER BYTE _________________________*/

/*___________________________________ INICIO DE LEER TABLA INODOS____________________________________*/

func Leer_TablaInodo(file *os.File, tamano_leer int32) TABLA_INODOS {
	tablaInodo := TABLA_INODOS{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &tablaInodo)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return TABLA_INODOS{}
	}
	return tablaInodo
}

/*___________________________________  FIN DE LEER TABLA INODOS _________________________*/

/*___________________________________ INICIO DE LEER TABLA INODOS____________________________________*/

func Leer_BloqueCarpeta(file *os.File, tamano_leer int32) BLOQUE_CARPETA {
	bloqueCarpeta := BLOQUE_CARPETA{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &bloqueCarpeta)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return BLOQUE_CARPETA{}
	}
	return bloqueCarpeta
}

/*___________________________________  FIN DE LEER TABLA INODOS _________________________*/

/*___________________________________ INICIO DE LEER TABLA INODOS____________________________________*/

func Leer_BloqueArchivo(file *os.File, tamano_leer int32) BLOQUE_ARCHIVO {
	bloqueArchivo := BLOQUE_ARCHIVO{}
	data := leerBytes(file, tamano_leer)
	buffer := bytes.NewBuffer(data)
	err := binary.Read(buffer, binary.BigEndian, &bloqueArchivo)
	if err != nil {
		log.Fatal("ERROR AL LEER ARCHIVO BINARIO", err)
		return BLOQUE_ARCHIVO{}
	}
	return bloqueArchivo
}

/*___________________________________  FIN DE LEER TABLA INODOS _________________________*/

func Crear_Mkgrp(name string) int32 {
	if actualSesion.hay_Sesion {

		if actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 { //Usuario root
			if len(name) <= 10 {
				//grupo, id_grupo := Existe_Grupo(name, actualSesion.Path)
				var grupo int = buscarGrupo(name, actualSesion.Path)
				if grupo == -1 {
					var id_grupo int = getID_grp()
					var nuevo_Grupo string = strconv.Itoa(int(id_grupo)) + ",G," + name + "\n"
					agregarUsersTXT1(nuevo_Grupo, actualSesion.Path)
					fmt.Println("GRUPO CREADO CON EXITO")
					return 0
				} else {
					fmt.Println("ERROR EXISTE GRUPO CON NOMBRE IGUAL")
					return -1
				}

			} else {
				fmt.Println("ERROR NOMBRE EXECEDE 10 CARACTERES")
				return -1
			}
		} else {
			fmt.Println("ERROR SOLO ROOT EJECUTA COMANDO MKGRP")
			return -1
		}

	} else {
		fmt.Println("ERROR DEBE EXISTIR SESION PARA CREAR GRUPO")
		return -1
	}
	//return 0

}

func buscarGrupo(name string, path_disco string) int {
	file, err := os.OpenFile(path_disco, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		return -1
	}
	var cadena []byte
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS

	file.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(file, tamanoSuper)
	//Leemos el inodo del archivo users.txt
	file.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(file, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			var archivo BLOQUE_ARCHIVO
			file.Seek(int64(super.S_block_start), 0)
			for j := 0; j <= int(inodo.I_block[i]); j++ {
				archivo = Leer_BloqueArchivo(file, tamanoArchivo)
			}
			//strcat(cadena, archivo.B_content)
			cadena = append(cadena[:], archivo.B_content[:]...)
		}
	}

	var contenido_cadena string = aString(cadena)
	var contenido_ptr *string = &contenido_cadena
	var token *string = strtok_r(contenido_ptr, "\n")

	for *token != "" {
		var id [2]byte
		var tipo [2]byte
		var group [10]byte
		var token_id *string = strtok_r(token, ",")
		copy(id[:], []byte(*token_id))
		if aString(id[:]) != "0" { //Verificar que no sea un U/G eliminado
			var token_grupo *string = strtok_r(token, ",")
			copy(tipo[:], []byte(*token_grupo))
			if aString(tipo[:]) == "G" {
				//var token_nombre_grupo *string = strtok_r(token, ",")
				var token_nombre_grupo *string = token
				copy(group[:], []byte(*token_nombre_grupo))
				if aString(group[:]) == name {
					id_num, _ := strconv.Atoi(aString(id[:]))

					return id_num
				}

			}
		}
		token = strtok_r(contenido_ptr, "\n")

	}

	return -1
}

func strtok_r(cadena *string, delimitador string) *string {
	var info *string = nil

	var temp_cadena []byte = []byte(*cadena)
	var temp_info [1024]byte
	var temp_fin_cadena [1024]byte

	for i := 0; i < len(*cadena); i++ {
		if (*cadena)[i] == (delimitador)[0] {
			copy(temp_info[:], temp_cadena[:i])
			copy(temp_fin_cadena[:], temp_cadena[(i+1):])
			break
		}
	}
	var n, b string
	n = aString(temp_fin_cadena[:])
	b = aString(temp_info[:])
	*cadena = n
	info = &b
	*info = b
	return info
}

func agregarUsersTXT1(datos string, path_disco string) int {
	fp, err := os.OpenFile(path_disco, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return -1
	}
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var archivo BLOQUE_ARCHIVO
	var blockIndex int32 = 0

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	//Leemos el inodo del archivo users.txt
	fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			blockIndex = inodo.I_block[i] //Ultimo bloque utilizado del archivo
		}

	}

	fp.Seek(int64(super.S_block_start+tamanoArchivo*blockIndex), 0)
	archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
	var enUso int32 = int32(len(aString(archivo.B_content[:])))
	var libre int32 = 64 - enUso

	if int32(len(datos)) < libre {
		concatenar(archivo.B_content[:], []byte(datos), len(aString(archivo.B_content[:])))
		fp.Seek(int64(super.S_block_start+tamanoArchivo*blockIndex), 0)
		Escribir_BloqueArchivo(fp, &archivo)

		fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
		inodo = Leer_TablaInodo(fp, tamanoInodo)
		inodo.I_size = inodo.I_size + int32(len(datos))
		copy(inodo.I_mtime[:], Hora_fecha())
		fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
		Escribir_TablaInodo(fp, &inodo)

	} else {
		var aux string = ""
		var aux2 string = ""
		var i int32 = 0

		for i = 0; i < libre; i++ {
			aux += string(datos[i])
		}

		for ; i < int32(len(datos)); i++ {
			aux2 += string(datos[i])
		}

		//Guardamos lo que cabe en el primer bloque
		concatenar(archivo.B_content[:], []byte(aux), len(aString(archivo.B_content[:])))
		fp.Seek(int64(super.S_block_start+tamanoArchivo*blockIndex), 0)
		Escribir_BloqueArchivo(fp, &archivo)
		var auxArchivo BLOQUE_ARCHIVO
		copy(auxArchivo.B_content[:], []byte(aux2))
		var bit int32 = buscarBit1(fp, 'B', actualSesion.Fit[:])
		/*Guardamos el bloque en el bitmap y en la tabla de bloques*/
		fp.Seek(int64(super.S_bm_block_start+bit), 0)
		var byte_archivo byte = '2'
		Escribir_Byte(fp, &byte_archivo)

		fp.Seek(int64(super.S_block_start+tamanoArchivo*bit), 0)
		Escribir_BloqueArchivo(fp, &auxArchivo)
		/*Guardamos el modificado del inodo*/
		fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
		inodo = Leer_TablaInodo(fp, tamanoInodo)
		inodo.I_size = inodo.I_size + int32(len(datos))
		copy(inodo.I_mtime[:], Hora_fecha())
		inodo.I_block[blockIndex] = bit
		fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
		Escribir_TablaInodo(fp, &inodo)
		/*Guardamos la nueva cantidad de bloques libres y el primer bloque libre*/
		super.S_first_blo = super.S_first_blo + 1
		super.S_free_blocks_count = super.S_free_blocks_count - 1
		fp.Seek(int64(actualSesion.InicioSuper), 0)
		Escribir_SuperBloque(fp, &super)
		return 0
	}
	return -1
}

func buscarBit1(fp *os.File, tipo byte, fit []byte) int32 {
	var super SUPER_BLOQUE
	var inicio_bm int32 = 0
	var tempBit byte = '0'
	var bit_libre int32 = -1
	var tam_bm int32 = 0

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)

	if tipo == 'I' {
		tam_bm = super.S_inodes_count
		inicio_bm = super.S_bm_inode_start
	} else if tipo == 'B' {
		tam_bm = super.S_blocks_count
		inicio_bm = super.S_bm_block_start
	}

	/*----------------Tipo de ajuste a utilizar----------------*/
	if aString(fit) == "ff" { //Primer ajuste
		for i := 0; i < int(tam_bm); i++ {
			fp.Seek(int64(inicio_bm+int32(i)), 0)
			tempBit = Leer_Byte(fp, 1)
			if tempBit == '0' {
				bit_libre = int32(i)
				return bit_libre
			}
		}

		if bit_libre == -1 {
			return -1
		}

	} else if aString(fit[:]) == "bf" { //Mejor ajuste
		var libres int32 = 0
		var auxLibres int32 = -1

		for i := 0; i < int(tam_bm); i++ { //Primer recorrido
			fp.Seek(int64(inicio_bm+int32(i)), 0)
			tempBit = Leer_Byte(fp, 1)
			if tempBit == '0' {
				libres++
				if int32(i)+1 == tam_bm {
					if auxLibres == -1 || auxLibres == 0 {
						auxLibres = libres
					} else {
						if auxLibres > libres {
							auxLibres = libres
						}

					}
					libres = 0
				}
			} else if tempBit == '1' {
				if auxLibres == -1 || auxLibres == 0 {
					auxLibres = libres
				} else {
					if auxLibres > libres {
						auxLibres = libres
					}

				}
				libres = 0
			}
		}

		for i := 0; i < int(tam_bm); i++ {
			fp.Seek(int64(inicio_bm+int32(i)), 0)
			tempBit = Leer_Byte(fp, 1)
			if tempBit == '0' {
				libres++
				if int32(i)+1 == tam_bm {
					return ((int32(i) + 1) - libres)
				}
			} else if tempBit == '1' {
				if auxLibres == libres {
					return ((int32(i) + 1) - libres)
				}

				libres = 0
			}
		}

		return -1

	} else if aString(fit[:]) == "wf" { //Peor ajuste
		var libres int32 = 0
		var auxLibres int32 = -1

		for i := 0; i < int(tam_bm); i++ { //Primer recorrido
			fp.Seek(int64(inicio_bm+int32(i)), 0)
			tempBit = Leer_Byte(fp, 1)
			if tempBit == '0' {
				libres++
				if int32(i)+1 == tam_bm {
					if auxLibres == -1 || auxLibres == 0 {
						auxLibres = libres
					} else {
						if auxLibres < libres {
							auxLibres = libres
						}
					}
					libres = 0
				}
			} else if tempBit == '1' {
				if auxLibres == -1 || auxLibres == 0 {
					auxLibres = libres
				} else {
					if auxLibres < libres {
						auxLibres = libres
					}
				}
				libres = 0
			}
		}

		for i := 0; i < int(tam_bm); i++ {
			fp.Seek(int64(inicio_bm+int32(i)), 0)
			tempBit = Leer_Byte(fp, 1)
			if tempBit == '0' {
				libres++
				if int32(i)+1 == tam_bm {
					return ((int32(i) + 1) - libres)
				}
			} else if tempBit == '1' {
				if auxLibres == libres {
					return ((int32(i) + 1) - libres)
				}
				libres = 0
			}
		}

		return -1
	}

	return 0
}

func concatenar(contenido_original []byte, copiar []byte, inicio int) {
	var contador int = 0
	for i := inicio; i < inicio+len(copiar); i++ {
		contenido_original[i] = copiar[contador]
		contador++
	}

}

func getID_grp() int {

	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return -1
	}

	var cadena [400]byte
	var aux_id int = -1
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	//Leemos el inodo del archivo users.txt
	fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			var archivo BLOQUE_ARCHIVO
			fp.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[i]), 0)
			//for j := 0; j < int(inodo.I_block[i]); j++ {
			archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
			//}

			concatenar(cadena[:], archivo.B_content[:], len(aString(cadena[:])))
		}
	}

	//cadena := []byte("1,G,Root\n2,G,Dark\n3,G,Ghost\n4,G,Deep\n5,G,Camino\n6,G,Usuarios\n")
	var contenido_cadena string = aString(cadena[:])
	var contenido_ptr *string = &contenido_cadena

	var token *string = strtok_r(contenido_ptr, "\n")
	for *token != "" {
		var id [2]byte
		var tipo [2]byte
		var token_id *string = strtok_r(token, ",")
		copy(id[:], *token_id)
		if aString(id[:]) != "0" { //Verificar que no sea un U/G eliminado
			var token_grupo *string = strtok_r(token, ",")
			copy(tipo[:], *token_grupo)
			if aString(tipo[:]) == "G" {
				temp_id, _ := strconv.Atoi(aString(id[:]))
				aux_id = temp_id
			}

		}
		token = strtok_r(contenido_ptr, "\n")
	}
	aux_id++
	return aux_id
}

func Ejecutar_rmdisk(path string) int {

	if _, err := os.Stat(path); err == nil {
		// path/to/whatever exists
		var mensaje string = "Deseas Eliminar el archivo S o N?:"
		fmt.Println(mensaje)
		reader := bufio.NewReader(os.Stdin)
		text, _ := reader.ReadString('\n')
		// convert CRLF to LF
		text = strings.Replace(text, "\n", "", -1)
		if strings.ToLower(text) == "s" {
			e := os.Remove(path)
			if e != nil {
				log.Fatal(e)
			}
			return 0
		} else if strings.ToLower(text) == "n" {
			return 1
		} else {
			return 2
		}

	}

	return -1
}

func Crear_Mkusr(usuario string, pass string, grupo string) int {
	if len(usuario) > 10 && len(grupo) > 10 {
		return -1
	}

	if buscarGrupo(grupo, actualSesion.Path) != -1 {
		if !buscarUsuario(usuario) {
			var id = getID_usr()

			var datos string = strconv.Itoa(id) + ",U," + grupo + "," + usuario + "," + pass + "\n"
			agregarUsersTXT1(datos, actualSesion.Path)

			return 0
		}

	}
	return -1
}

func buscarUsuario(name string) bool {
	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return false
	}

	var cadena [400]byte
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	//Nos posicionamos en el inodo del archivo users.txt
	fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			var archivo BLOQUE_ARCHIVO
			fp.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[i]), 0)
			//for j := 0; j < int(inodo.I_block[i]); j++ {
			//fread(&archivo,sizeof(BloqueArchivo),1,fp);
			archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
			//}
			//strcat(cadena,archivo.b_content);
			concatenar(cadena[:], archivo.B_content[:], len(aString(cadena[:])))
		}
	}

	var contenido_cadena string = aString(cadena[:])
	var contenido_ptr *string = &contenido_cadena
	var token *string = strtok_r(contenido_ptr, "\n")

	for *token != "" {
		var id [2]byte
		var tipo [2]byte
		var user [10]byte
		//id,Tipo,NombreGrupo,nombre_usuario,contrasena
		//1,U,root,root,123\n
		var token_id *string = strtok_r(token, ",")
		copy(id[:], []byte(*token_id))
		if aString(id[:]) != "0" { //Verificar que no sea un U/G eliminado
			var token_tipo *string = strtok_r(token, ",")
			copy(tipo[:], []byte(*token_tipo))

			if aString(tipo[:]) == "U" {
				var token_nombre_grupo *string = strtok_r(token, ",")
				_ = token_nombre_grupo
				var token_nombre_usuario *string = strtok_r(token, ",")
				copy(user[:], []byte(*token_nombre_usuario))
				if aString(user[:]) == name {
					fmt.Println("USUARIO ENCONTRADO", aString(user[:]))
					return true
				}

			}
		}
		token = strtok_r(contenido_ptr, "\n")

	}

	return false
}

func getID_usr() int {
	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {
		return -1
	}
	var cadena [400]byte
	var res int = 0
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	//Nos posicionamos en el inodo del archivo users.txt
	fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			var archivo BLOQUE_ARCHIVO
			fp.Seek(int64(super.S_block_start), 0)
			for j := 0; j < int(inodo.I_block[i]); j++ {
				archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
			}
			concatenar(cadena[:], archivo.B_content[:], len(aString(cadena[:])))
		}
	}

	var contenido_cadena string = aString(cadena[:])
	var contenido_ptr *string = &contenido_cadena

	var token *string = strtok_r(contenido_ptr, "\n")
	for *token != "" {
		var id [2]byte
		var tipo [2]byte

		var token_id *string = strtok_r(token, ",")
		copy(id[:], *token_id)
		if aString(id[:]) != "0" { //Verificar que no sea un U/G eliminado
			var token_tipo *string = strtok_r(token, ",")
			copy(tipo[:], *token_tipo)
			if aString(tipo[:]) == "U" {
				res++
			}
		}
		token = strtok_r(contenido_ptr, "\n")
	}
	return res
}

func Eliminar_Grupo(name string, path string) int32 {
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		return -1
	}
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var archivo BLOQUE_ARCHIVO

	var col int32 = 1
	var actual byte
	var posicion int32 = 0
	var numBloque int32 = 0
	var id int32 = -1
	var tipo byte = '\x00'
	var grupo string = ""
	var palabra string = ""
	var flag bool = false

	file.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(file, tamanoSuper)

	//Nos posicionamos en el inodo del archivo users.txt
	file.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(file, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {

			file.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[i]), 0)
			archivo = Leer_BloqueArchivo(file, tamanoArchivo)
			for j := 0; j < 64; j++ {
				actual = archivo.B_content[j]
				if actual == '\n' {
					if tipo == 'G' {
						grupo = palabra
						if grupo == name {
							file.Seek(int64(super.S_block_start+tamanoArchivo*numBloque), 0)
							archivo = Leer_BloqueArchivo(file, tamanoArchivo)
							//fread(&archivo,sizeof(BloqueCarpeta),1,fp);
							archivo.B_content[posicion] = '0'
							file.Seek(int64(super.S_block_start+tamanoArchivo*numBloque), 0)
							Escribir_BloqueArchivo(file, &archivo)
							//fwrite(&archivo,sizeof(BloqueArchivo),1,fp);
							fmt.Println("Grupo eliminado con exito")
							flag = true
							break
						}
					}
					col = 1
					palabra = ""
				} else if actual != ',' {
					palabra += string(actual)
					col++
				} else if actual == ',' {
					if col == 2 {
						idd, _ := strconv.Atoi(palabra)
						id = int32(idd)
						_ = id
						posicion = int32(j - 1)
						numBloque = inodo.I_block[i]
					} else if col == 4 {
						tipo = palabra[0]
					}

					col++
					palabra = ""
				}
			}
			if flag {
				return 0
				//break

			}

		}
	}

	return -1
}

func Eliminar_Usurio(name string, path_disco string) int {
	if actualSesion.Id_user == 1 && actualSesion.Id_grp == 1 {
		if buscarUsuario(name) {
			eliminarUsuario(name)
			return 0
		}
	}

	return -1
}

func eliminarUsuario(name string) {
	fp, err := os.OpenFile(actualSesion.Path, os.O_RDWR, 0777)
	defer fp.Close()

	if err != nil {

	}

	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var archivo BLOQUE_ARCHIVO

	var col int32 = 1
	var actual byte
	var palabra string = ""
	var posicion int32 = 0
	var numBloque int32 = 0
	var id int32 = -1
	var tipo byte = '\x00'
	var grupo string = ""
	var usuario string = ""
	var flag bool = false

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	//Nos posicionamos en el inodo del archivo users.txt
	fp.Seek(int64(super.S_inode_start+tamanoInodo), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	for i := 0; i < 16; i++ {
		if inodo.I_block[i] != -1 {
			fp.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[i]), 0)
			archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
			for j := 0; j < 64; j++ {
				actual = archivo.B_content[j]
				if actual == '\n' {
					if tipo == 'U' {
						if usuario == name {
							fp.Seek(int64(super.S_block_start+tamanoArchivo*numBloque), 0)
							archivo = Leer_BloqueArchivo(fp, tamanoArchivo)

							archivo.B_content[posicion] = '0'
							fp.Seek(int64(super.S_block_start+tamanoArchivo*numBloque), 0)
							Escribir_BloqueArchivo(fp, &archivo)
							flag = true
							//return 0
							break

						}
						usuario = ""
						grupo = ""
					}
					col = 1
					palabra = ""
				} else if actual != ',' {
					palabra += string(actual)
					col++
				} else if actual == ',' {
					if col == 2 {
						tem_id, _ := strconv.Atoi(palabra)
						id = int32(tem_id)
						_ = id
						posicion = int32(j) - 1
						numBloque = inodo.I_block[i]
					} else if col == 4 {
						tipo = palabra[0]
					} else if grupo == "" {
						grupo = palabra
					} else if usuario == "" {
						usuario = palabra
					}
					col++
					palabra = ""
				}
			}
			if flag {
				//return 0
				break
			}

		}
	}

}

func existeArchivo(path string) bool {
	filiInfo, err := os.Stat(path)

	if err != nil {
		fmt.Println("ERROR EL ARCHIVO NO EXISTE")
		return false
	}
	_ = filiInfo // no se utilizo para nada solo para verificar que exist archivo
	return true
}

/*_________________________ INICIO SUPER BLOQUE ___________________*/
func inicio_super_bloque(path string, name string) int32 {

	//os.Open sirve para leer el archivo nada mas y no para modificar
	file, err := os.OpenFile(path, os.O_RDWR, 0777)
	defer file.Close()

	if err != nil {
		log.Fatal("ERROR AL ABRIR ARCHIVO,ARCHIVO NO EXISTE", err)
		return -1
	}
	masterBoot := MBR{}

	var tamano_masterBoot int32 = int32(unsafe.Sizeof(masterBoot))
	masterBoot = ObtenerInforMbr(file, tamano_masterBoot)

	var indice_extendia int32 = -1
	for i := 0; i < 4; i++ {
		if string(bytes.Trim(masterBoot.Mbr_partition[i].Part_name[:], "\x00")) == name {
			file.Close()
			return masterBoot.Mbr_partition[i].Part_start
		}
		if string(masterBoot.Mbr_partition[i].Part_type) == "e" {
			indice_extendia = int32(i)
		}
	}

	if indice_extendia != -1 {
		//aqui va lo de la logica
		extendBoot := EBR{}
		file.Seek(int64(masterBoot.Mbr_partition[indice_extendia].Part_start), 0) //(cantidad_bytes_desplazar,origen_de_archivo)
		extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{})))
		for {

			if string(bytes.Trim(extendBoot.Part_name[:], "\x00")) == name {
				return extendBoot.Part_start
			}

			if extendBoot.Part_next == -1 {
				break
			}
			file.Seek(int64(extendBoot.Part_next), 0)
			extendBoot = ObtenerInfoEbr(file, int32(unsafe.Sizeof(EBR{}))) //ruta quemada por el momento xD
		}
	}
	file.Close()

	return -1
}

/*_______________________________ SEPARAR PATH CARPETA_____*/
func separa_path(path string) []string {
	var respuesta = []string{}
	var temp_path = path
	path_separado := strings.Split(temp_path, "/")
	for i := 0; i < len(path_separado); i++ {
		if path_separado[i] != "" {
			respuesta = append(respuesta, path_separado[i])
		}
	}
	return respuesta
}

/*_____________________ MEMSET RELLENA UN ARRAGLO DE BYTES CON UN CARACTER ESPECIFICO _______*/
func memseT(a []byte, v byte) {
	for i := range a {
		a[i] = v
	}
}

/*_____________________ LEER ARCHIVO ______________________________________*/
func Leer_Cualquier_Archivo(path string) string {

	f, err := os.Open(path)
	if err != nil {
		//log.Fatalf("open file error: %v", err)
		fmt.Println("ERROR NO SE PUDO LEER CONTENIDO EN CREAR MKFILE")
		return ""
	}
	defer f.Close()
	/*
		var contenido_retornar string = ""
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			lineaTexto := sc.Text() // GET the line string
			//fmt.Println("Contenido Linea:")
			if len(lineaTexto) > 0 {
				contenido_retornar = contenido_retornar + lineaTexto
			}

		}
	*/

	_, err = f.Seek(0, 0)
	r4 := bufio.NewReader(f)
	b4, err := r4.Peek(1024)

	//return contenido_retornar
	return aString(b4[:])
}

func crear_rep_mbr(direccion_disco string, destino_reporte_dot string, destino_reporte_grafico string, extension string) int {
	Crear_carpetas(destino_reporte_dot) //para crear las carpetas que no existen
	var contenidoDot string = ""
	//______________ LEYENDO ARCHIVO ____________________
	fp, err := os.OpenFile(direccion_disco, os.O_RDWR, 0777)
	defer fp.Close()
	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		fmt.Println("ERROR AL ABRIR ARCHIVO:", direccion_disco)
		return -1
	}

	if err == nil {

		contenidoDot += "digraph G{ \n"
		contenidoDot += "\ntbl[shape=box,label=<\n"
		contenidoDot += "<table border='0' cellborder='1' cellspacing='0' width='300'  height='200' >\n"
		var tamanoMBR int32 = int32(unsafe.Sizeof(MBR{}))
		var tamanoEBR int32 = int32(unsafe.Sizeof(EBR{}))
		var masterBoot MBR = ObtenerInforMbr(fp, tamanoMBR)

		contenidoDot += "<tr >  <td  width='300' bgcolor=\"#EBEBEB\" colspan=\"4\" >REPORTE MBR</td> </tr>"
		contenidoDot += "<tr>  <td  width='150'><b>mbr_tamaño</b></td><td  width='150'>" + strconv.Itoa(int(masterBoot.Mbr_tamano)) + "</td></tr>\n"
		contenidoDot += "<tr>  <td bgcolor=\"#EBEBEB\"><b>mbr_fecha_creacion</b></td> <td>" + aString(masterBoot.Mbr_fecha_creacion[:]) + "</td> </tr>\n"
		contenidoDot += "<tr>  <td width='150'><b>mbr_disk_signature</b></td> <td width='150'>" + strconv.Itoa(int(masterBoot.Mbr_disk_signature)) + "</td>  </tr>\n"
		contenidoDot += "<tr>  <td bgcolor=\"#EBEBEB\" width='150'><b>Disk_fit</b></td> <td width='150'>" + aString(masterBoot.Disk_fit[:]) + "</td>  </tr>\n"

		var index_Extendida int = -1
		for i := 0; i < 4; i++ {
			if masterBoot.Mbr_partition[i].Part_start != -1 && masterBoot.Mbr_partition[i].Part_status != 0 {
				if masterBoot.Mbr_partition[i].Part_type == 101 {
					index_Extendida = i
				}

				contenidoDot += "<tr><td colspan=\"4\" bgcolor=\"#EBEBEB\">PARTICION</td> </tr>"
				contenidoDot += "<tr>  <td bgcolor=\"#EBEBEB\"><b>part_status</b></td> <td>" + string(masterBoot.Mbr_partition[i].Part_status) + "</td></tr>\n"
				contenidoDot += "<tr>  <td><b>part_type</b></td> <td>" + string(masterBoot.Mbr_partition[i].Part_type) + "</td></tr>\n"
				contenidoDot += "<tr>  <td bgcolor=\"#EBEBEB\"><b>part_fit</b></td> <td>" + aString(masterBoot.Mbr_partition[i].Part_fit[:]) + "</td></tr>\n"
				contenidoDot += "<tr>  <td><b>part_start</b></td> <td>" + strconv.Itoa(int(masterBoot.Mbr_partition[i].Part_start)) + "</td></tr>\n"
				contenidoDot += "<tr>  <td bgcolor=\"#EBEBEB\"><b>part_size</b></td> <td>" + strconv.Itoa(int(masterBoot.Mbr_partition[i].Part_size)) + "</td></tr>\n"
				contenidoDot += "<tr>  <td><b>part_name</b></td> <td>" + aString(masterBoot.Mbr_partition[i].Part_name[:]) + "</td></tr>\n"
			}
		}

		contenidoDot += "</table>\n"
		contenidoDot += ">];"

		if index_Extendida != -1 {
			fp.Seek(int64(masterBoot.Mbr_partition[index_Extendida].Part_start), 0)
			var extendedBoot EBR = ObtenerInfoEbr(fp, tamanoEBR)
			contenidoDot += "\ntbl_1[shape=box, label=<\n "
			contenidoDot += "<table border='0' cellborder='1' cellspacing='0'  width='300' height='160' >\n "

			for {
				if extendedBoot.Part_status != 0 {

					contenidoDot += "<tr>  <td colspan=\"4\" bgcolor=\"#EBEBEB\" width='300'>REPORTE EBR</td> </tr>"
					contenidoDot += "<tr>  <td width='150'><b>part_status</b></td> <td width='150'>" + string(extendedBoot.Part_status) + "</td>  </tr>\n"
					contenidoDot += "<tr>  <td><b>part_fit</b></td> <td>" + aString(extendedBoot.Part_fit[:]) + "</td>  </tr>\n"
					contenidoDot += "<tr>  <td><b>part_size</b></td> <td>" + strconv.Itoa(int(extendedBoot.Part_size)) + "</td>  </tr>\n"
					contenidoDot += "<tr>  <td><b>part_start</b></td> <td>" + strconv.Itoa(int(extendedBoot.Part_start)) + "</td>  </tr>\n"
					contenidoDot += "<tr>  <td><b>part_next</b></td> <td>" + strconv.Itoa(int(extendedBoot.Part_next)) + "</td>  </tr>\n"
					contenidoDot += "<tr>  <td><b>part_name</b></td> <td>" + aString(extendedBoot.Part_name[:]) + "</td>  </tr>\n"

				}

				if extendedBoot.Part_next == -1 {
					break
				}
				fp.Seek(int64(extendedBoot.Part_next), 0)
				extendedBoot = ObtenerInfoEbr(fp, tamanoEBR) //ruta quemada por el momento xD
			}
			contenidoDot += "</table>\n"
			contenidoDot += ">];\n"
		}
		contenidoDot += "}\n"

		crear_archivo_reporte(destino_reporte_dot, contenidoDot)
		contenidoDot = ""
		//var comando string = "dot -T" + extension + " " + destino_reporte_dot + " -o " + destino_reporte_grafico
		return systema_comando(destino_reporte_dot, destino_reporte_grafico, extension)

	} else {
		return -1
	}
}

/*____________________________________________ FUNCIONES IMPORTANTES PARA REPORTE __________________________________*/
func crear_archivo_reporte(path string, contenido_archivo string) {
	crear_archivo, err := os.Create(path)

	if err != nil {
		log.Fatal(err)
	}

	defer crear_archivo.Close()

	_, err2 := crear_archivo.WriteString(contenido_archivo)

	if err2 != nil {
		//log.Fatal(err2)
		fmt.Println("ERROR EN FUNCION CREAR ARCHIVO REPORTE")
	}

}

func aString(c []byte) string {
	n := -1
	for i, b := range c {
		if b == '\x00' {
			break
		}
		n = i
	}
	return string(c[:n+1])
}

func systema_comando(dir_dot string, dir_rep string, extension_rep string) int {
	var arg1 string = "dot"
	var arg2 string = "-T" + extension_rep
	var arg3 string = dir_dot
	var arg4 string = "-o"
	var arg5 string = dir_rep

	cmd := exec.Command(arg1, arg2, arg3, arg4, arg5)

	err := cmd.Run()

	if err != nil {
		//log.Fatal(err)
		fmt.Println("ERROR AL CREAR REPORTE EN FUNCION SISTEMA COMANDO......")
		return -1
	}
	return 0
}

func calcular_porcentaje_usado(tamano_particion int, tamano_disco int) int {

	return int(math.Round(((float64(tamano_particion) * 100) / float64(tamano_disco))))
}

func obtener_espacio_libre(inicio_part int, fin_part int, tamano_part int, tipo string) int {
	if tipo == "normal" {
		return fin_part - (inicio_part + tamano_part)
	} else if tipo == "extendida" {
		return fin_part - (inicio_part + tamano_part + int(unsafe.Sizeof(EBR{})))
	}
	return 0
}

/*____________________________________________ FIN FUNCIONES IMPORTANTES PARA REPORTE __________________________________*/
func crear_rep_disk(direccion_disco string, destino_reporte_dot string, destino_reporte_grafico string, extension string) int {
	Crear_carpetas(destino_reporte_dot)

	var contenidoDot string = ""

	archivo_rep, err := os.OpenFile(direccion_disco, os.O_RDWR, 0777)
	defer archivo_rep.Close()
	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		fmt.Println("ERROR AL ABRIR ARCHIVO:", direccion_disco)
		return -1
	}

	if err == nil {
		if true {
			contenidoDot += "digraph{\n\tnode [shape=plaintext]\n\tReporte_Disco[label=<\n\t<TABLE cellpadding=\"0\">\n\t<TR>\n\t\t<TD BGCOLOR=\"#EBEBEB\">MBR<br/></TD>\n"
			var tamanoMBR int32 = int32(unsafe.Sizeof(MBR{}))
			var tamanoEBR int32 = int32(unsafe.Sizeof(EBR{}))

			var mbr_temp MBR = ObtenerInforMbr(archivo_rep, tamanoMBR)

			var i int = 0
			var indice_usado_final int = 0

			for i = 0; i < 4; i++ {
				if mbr_temp.Mbr_partition[i].Part_status != '\x00' {
					indice_usado_final = i
					if mbr_temp.Mbr_partition[i].Part_type == 112 {

						if mbr_temp.Mbr_partition[i].Part_status == '\x00' {
							contenidoDot = contenidoDot + "<TD BGCOLOR=\"#EBEBEB\">" + "LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(mbr_temp.Mbr_partition[i].Part_size), int(mbr_temp.Mbr_tamano))) + "%</TD>"
						} else {
							contenidoDot = contenidoDot + "<TD BGCOLOR=\"#EBEBEB\">" + "PRIMARIA<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(mbr_temp.Mbr_partition[i].Part_size), int(mbr_temp.Mbr_tamano))) + "%</TD>"
						}

						if i < 3 {
							var espacio_no_usado int = obtener_espacio_libre(int(mbr_temp.Mbr_partition[i].Part_start), int(mbr_temp.Mbr_partition[i+1].Part_start), int(mbr_temp.Mbr_partition[i].Part_size), "normal")
							if espacio_no_usado > 0 {
								contenidoDot = contenidoDot + "<TD BGCOLOR=\"#EBEBEB\">" + "LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(espacio_no_usado, int(mbr_temp.Mbr_tamano))) + "%</TD>"
							}
						}

					}

					if mbr_temp.Mbr_partition[i].Part_type == 101 {
						//crear metodo para obtener el string porque si esta antes o en medio F para reporte
						archivo_rep.Seek(int64(mbr_temp.Mbr_partition[i].Part_start), 0)
						var extendedBoot EBR = ObtenerInfoEbr(archivo_rep, tamanoEBR)
						var extend_temp EBR = EBR{}
						var espacio_libre int

						contenidoDot = contenidoDot + "<TD BORDER=\"0\">\n\t<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n\t\t<TR>\n\t\t\t<TD BGCOLOR=\"#EBEBEB\" COLSPAN=\"23\">EXTENDIDA</TD>\n\t\t</TR>\n\t\t<TR>"

						for {

							if extendedBoot.Part_next == -1 {
								contenidoDot = contenidoDot + "\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">EBR</TD>\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">LOGICA<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(extendedBoot.Part_size), int(mbr_temp.Mbr_tamano))) + "%</TD>"
								var termina_ultima_extendida int32 = extendedBoot.Part_start + extendedBoot.Part_size
								var total_no_usado = mbr_temp.Mbr_partition[i].Part_size - termina_ultima_extendida
								if total_no_usado > 0 {
									//contenidoDot = contenidoDot + "\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">EBR</TD>\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(total_no_usado), int(mbr_temp.Mbr_tamano))) + "%</TD>"
									contenidoDot = contenidoDot + "<TD BGCOLOR=\"#EBEBEB\">" + "LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(total_no_usado), int(mbr_temp.Mbr_tamano))) + "%</TD>"
								}

								break
							} else {
								extend_temp = extendedBoot
								archivo_rep.Seek(int64(extendedBoot.Part_next), 0)
								extendedBoot = ObtenerInfoEbr(archivo_rep, tamanoEBR)
								espacio_libre = obtener_espacio_libre(int(extend_temp.Part_start), int(extendedBoot.Part_start), int(extend_temp.Part_size), "extendida")
								if espacio_libre > 0 {

									contenidoDot = contenidoDot + "\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">EBR</TD>\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">LOGICA<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(extend_temp.Part_size), int(mbr_temp.Mbr_tamano))) + "%</TD>"
									contenidoDot = contenidoDot + "\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">EBR</TD>\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(espacio_libre, int(mbr_temp.Mbr_tamano))) + "%</TD>"

								} else {

									contenidoDot = contenidoDot + "\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">EBR</TD>\n\t\t\t\t<TD bgcolor=\"#EBEBEB\">LOGICA<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(extend_temp.Part_size), int(mbr_temp.Mbr_tamano))) + "%</TD>"
								}
							}
						} //fin de while
						contenidoDot = contenidoDot + "\n\t\t</TR>\n\t</TABLE>\n</TD>"
					}

				}

			} //fin de for
			//____________________________________________________________PARA ESPACCIO FINAL EN  DE DISCO _______
			var termina_ultima_particion int32 = mbr_temp.Mbr_partition[indice_usado_final].Part_start + mbr_temp.Mbr_partition[indice_usado_final].Part_size
			var espacio_no_usado = mbr_temp.Mbr_tamano - termina_ultima_particion
			if espacio_no_usado > 0 {
				contenidoDot = contenidoDot + "<TD BGCOLOR=\"#EBEBEB\">" + "LIBRE<br/>" + strconv.Itoa(calcular_porcentaje_usado(int(espacio_no_usado), int(mbr_temp.Mbr_tamano))) + "%</TD>"
			}

			//____________________________________________________________ FIN PARA ESPACION FINAL EN DISCO

			contenidoDot += "\n\t</TR>\n\t</TABLE>\n\t>];}"
			crear_archivo_reporte(destino_reporte_dot, contenidoDot)
			contenidoDot = ""

			return systema_comando(destino_reporte_dot, destino_reporte_grafico, extension)

		} else {

			fmt.Println("No se puede crear archivo dot en reporte disk")
		}

	} else {
		fmt.Println("Error al abrir archivo para crear reporte disk")
	}
	return -1
} //fin de metodo crear_rep_disk

/*_____________________________REPORTE TREE ___________________*/
func crear_rep_tree(direccion_disco string, destino_reporte_dot string, destino_reporte_grafico string, extension string) int {
	Crear_carpetas(destino_reporte_dot)

	var contenidoDot string = ""

	fp, err := os.OpenFile(direccion_disco, os.O_RDWR, 0777)
	defer fp.Close()
	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		fmt.Println("ERROR AL ABRIR ARCHIVO:", direccion_disco)
		return -1
	}

	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var carpeta BLOQUE_CARPETA
	var archivo BLOQUE_ARCHIVO

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)

	var aux int32 = super.S_bm_inode_start //super.s_bm_inode_start
	var i int32 = 0

	var buffer byte

	contenidoDot = "digraph G{\n\n"
	contenidoDot = contenidoDot + "    rankdir=\"LR\" \n"
	//Creamos los inodos
	for aux < super.S_bm_block_start {
		//fmt.Println("INicio de super bloque:",supe)

		fp.Seek(int64(super.S_bm_inode_start+i), 0)
		buffer = Leer_Byte(fp, 1)
		aux++
		var port int = 0
		if buffer == '1' {
			fp.Seek(int64(super.S_inode_start+tamanoInodo*i), 0)
			inodo = Leer_TablaInodo(fp, tamanoInodo)
			contenidoDot = contenidoDot + "    inodo_" + strconv.Itoa(int(i)) + "[ shape=plaintext fontname=\"Century Gothic\" label=<\n"
			contenidoDot = contenidoDot + "   <table bgcolor=\"#EBEBEB\" border='0' >"
			contenidoDot = contenidoDot + "    <tr> <td colspan='2'><b>Inode " + strconv.Itoa(int(i)) + "</b></td></tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_uid </td> <td bgcolor=\"#EBEBEB\">" + strconv.Itoa(int(inodo.I_uid)) + "</td>  </tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_gid </td> <td bgcolor=\"#EBEBEB\">" + strconv.Itoa(int(inodo.I_gid)) + "</td>  </tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_size </td><td bgcolor=\"#EBEBEB\">" + strconv.Itoa(int(inodo.I_size)) + "</td> </tr>\n"

			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_atime </td> <td bgcolor=\"#EBEBEB\">" + aString(inodo.I_atime[:]) + "</td> </tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_ctime </td> <td bgcolor=\"#EBEBEB\">" + aString(inodo.I_ctime[:]) + "</td> </tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_mtime </td> <td bgcolor=\"#EBEBEB\">" + aString(inodo.I_mtime[:]) + "</td> </tr>\n"

			for b := 0; b < 16; b++ {
				contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_block_" + strconv.Itoa(port) + "</td> <td bgcolor=\"#EBEBEB\" port=\"f" + strconv.Itoa(b) + "\"> " + strconv.Itoa(int(inodo.I_block[b])) + "</td></tr>\n"
				port++
			}
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_type </td> <td bgcolor=\"#EBEBEB\">" + string(inodo.I_type) + "</td>  </tr>\n"
			contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> i_perm </td> <td bgcolor=\"#EBEBEB\">" + strconv.Itoa(int(inodo.I_perm)) + "</td>  </tr>\n"
			contenidoDot = contenidoDot + "   </table>>]\n\n"
			//Creamos los bloques relacionados al inodo
			for j := 0; j < 16; j++ {
				port = 0
				if inodo.I_block[j] != -1 {
					fp.Seek(int64(super.S_bm_block_start+inodo.I_block[j]), 0)
					buffer = Leer_Byte(fp, 1)
					if buffer == '1' { //Bloque carpeta
						fp.Seek(int64(super.S_block_start+tamanoCarpeta*inodo.I_block[j]), 0)
						carpeta = Leer_BloqueCarpeta(fp, tamanoCarpeta)
						contenidoDot = contenidoDot + "    bloque_" + strconv.Itoa(int(inodo.I_block[j])) + "[shape=plaintext fontname=\"Century Gothic\" label=< \n"
						contenidoDot = contenidoDot + "   <table bgcolor=\"#EBEBEB\" border='0'>\n"
						contenidoDot = contenidoDot + "    <tr> <td colspan='2'><b>Bloque Carpeta " + strconv.Itoa(int(inodo.I_block[j])) + "</b></td></tr>\n"
						contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> b_name </td> <td bgcolor=\"#EBEBEB\"> b_inode </td></tr>\n"
						for c := 0; c < 4; c++ {
							contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\" >" + aString(carpeta.B_content[c].B_name[:]) + "</td> <td bgcolor=\"#EBEBEB\"  port=\"f" + strconv.Itoa(port) + "\">" + strconv.Itoa(int(carpeta.B_content[c].B_inodo)) + "</td></tr>\n"
							port++
						}
						contenidoDot = contenidoDot + "   </table>>]\n\n"
						//Relacion de bloques a inodos
						for c := 0; c < 4; c++ {
							if carpeta.B_content[c].B_inodo != -1 {
								if aString(carpeta.B_content[c].B_name[:]) != "." && aString(carpeta.B_content[c].B_name[:]) != ".." {
									contenidoDot = contenidoDot + "    bloque_" + strconv.Itoa(int(inodo.I_block[j])) + ":f" + strconv.Itoa(c) + "-> inodo_" + strconv.Itoa(int(carpeta.B_content[c].B_inodo)) + ";\n"
								}

							}
						}
					} else if buffer == '2' { //Bloque archivo
						fp.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[j]), 0)
						archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
						contenidoDot = contenidoDot + "    bloque_" + strconv.Itoa(int(inodo.I_block[j])) + "[shape=plaintext fontname=\"Century Gothic\" label=< \n"
						contenidoDot = contenidoDot + "   <table border='0' bgcolor=\"#EBEBEB\">\n"
						contenidoDot = contenidoDot + "    <tr> <td> <b>Bloque Archivo " + strconv.Itoa(int(inodo.I_block[j])) + "</b></td></tr>\n"
						contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> " + aString(archivo.B_content[:]) + "</td></tr>\n"
						contenidoDot = contenidoDot + "   </table>>]\n\n"
					}
					//Relacion de inodos a bloques
					contenidoDot = contenidoDot + "    inodo_" + strconv.Itoa(int(i)) + ":f" + strconv.Itoa(j) + " -> bloque_" + strconv.Itoa(int(inodo.I_block[j])) + "; \n"
				}
			}
		}
		i++
	}

	contenidoDot = contenidoDot + "\n\n}"
	crear_archivo_reporte(destino_reporte_dot, contenidoDot)
	contenidoDot = ""

	return systema_comando(destino_reporte_dot, destino_reporte_grafico, extension)

	//Original----->string comando = "dot -T"+extension.toStdString()+" grafica.dot -o "+destino.toStdString();
	//Original---->system(comando.c_str());

	//string comando = "dot -T"+extension.toStdString()+" "+obtener_ruta_carpeta(destino_reporte.toStdString())+"/"+obtener_nombre_archivo(destino_reporte).toStdString()+".dot -o "+destino_reporte.toStdString();
	//system(comando.c_str());
	//cout << "\n\tReporte Tree generado con exito " << endl;
	//return 0
}

func crear_rep_super_bloque(direccion_disco string, destino_reporte_dot string, destino_reporte_grafico string, extension string) int {
	Crear_carpetas(destino_reporte_dot)

	var contenidoDot string = ""

	fp, err := os.OpenFile(direccion_disco, os.O_RDWR, 0777)
	defer fp.Close()
	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		fmt.Println("ERROR AL ABRIR ARCHIVO:", direccion_disco)
		return -1
	}
	var super SUPER_BLOQUE

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)

	contenidoDot = contenidoDot + "digraph G{\n"
	contenidoDot = contenidoDot + "    nodo [shape=none, fontname=\"Century Gothic\" label=<"
	contenidoDot = contenidoDot + "   <table border='0' cellborder='1' cellspacing='0' bgcolor=\"#EBEBEB\">"
	contenidoDot = contenidoDot + "    <tr> <td COLSPAN='2'> <b>SUPER_BLOQUE</b> </td></tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_inodes_count </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_inodes_count)) + "</td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_blocks_count </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_blocks_count)) + "</td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_free_block_count </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_free_blocks_count)) + "</td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_free_inodes_count </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_free_inodes_count)) + " </td> </tr>\n"

	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_mtime </td> <td bgcolor=\"#EBEBEB\">" + aString(super.S_mtime[:]) + " </td></tr>\n"
	//contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_umtime </td> <td bgcolor=\"#EBEBEB\"> " + aString(super.S_umtime[:]) + "</td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_mnt_count </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_mnt_count)) + "</td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_magic </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_magic)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_inode_size </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_inode_size)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_block_size </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_block_size)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_first_ino </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_first_ino)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_first_blo </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_first_blo)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_bm_inode_start </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_bm_inode_start)) + " </td></tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_bm_block_start </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_bm_block_start)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_inode_start </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_inode_start)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "    <tr> <td bgcolor=\"#EBEBEB\"> s_block_start </td> <td bgcolor=\"#EBEBEB\"> " + strconv.Itoa(int(super.S_block_start)) + " </td> </tr>\n"
	contenidoDot = contenidoDot + "   </table>>]\n"
	contenidoDot = contenidoDot + "\n}"

	crear_archivo_reporte(destino_reporte_dot, contenidoDot)
	contenidoDot = ""
	return systema_comando(destino_reporte_dot, destino_reporte_grafico, extension)

}

func crear_rep_file(direccion_disco string, destino_reporte_dot string, destino_reporte_grafico string, extension string, ruta_archivo string) int {
	Crear_carpetas(destino_reporte_dot)

	var contenidoDot string = ""

	fp, err := os.OpenFile(direccion_disco, os.O_RDWR, 0777)
	defer fp.Close()
	if err != nil {
		//log.Fatal("ERROR AL ABRIR ARCHIVO", err)
		fmt.Println("ERROR AL ABRIR ARCHIVO:", direccion_disco)
		return -1
	}
	var super SUPER_BLOQUE
	var inodo TABLA_INODOS
	var archivo BLOQUE_ARCHIVO

	fp.Seek(int64(actualSesion.InicioSuper), 0)
	super = Leer_SuperBloque(fp, tamanoSuper)
	var numero_inodo int = buscarCarpetaArchivo(fp, ruta_archivo)
	if numero_inodo == -1 {
		fmt.Println("ERROR EL ARCHIVO:", ruta_archivo, "NO EXISTE PARA REPORTE")
		return -1
	}
	var name string = filepath.Base(ruta_archivo)
	fp.Seek(int64(super.S_inode_start+tamanoInodo*int32(numero_inodo)), 0)
	inodo = Leer_TablaInodo(fp, tamanoInodo)

	//FILE *graph = fopen("grafica.dot","w");
	contenidoDot = contenidoDot + "digraph G{\n"
	contenidoDot = contenidoDot + "    nodo [shape=none, fontname=\"Century Gothic\" label=<"
	contenidoDot = contenidoDot + "   <table border='0' cellborder='1' cellspacing='0' bgcolor=\"#EBEBEB\">"
	contenidoDot = contenidoDot + "    <tr><td align=\"left\"> <b>" + name + "</b> </td></tr>\n"
	contenidoDot = contenidoDot + "    <tr><td bgcolor=\"#EBEBEB\">"
	for i := 0; i < 15; i++ {
		if inodo.I_block[i] != -1 {
			//Apuntadores directos
			fp.Seek(int64(super.S_block_start+tamanoArchivo*inodo.I_block[i]), 0)
			archivo = Leer_BloqueArchivo(fp, tamanoArchivo)
			contenidoDot = contenidoDot + aString(archivo.B_content[:]) + "<br/>"

		}
	}
	contenidoDot = contenidoDot + "    </td></tr>\n"
	contenidoDot = contenidoDot + "   </table>>]\n"
	contenidoDot = contenidoDot + "\n}"
	//cout << "Reporte file generado con exito " << endl;
	crear_archivo_reporte(destino_reporte_dot, contenidoDot)
	contenidoDot = ""
	return systema_comando(destino_reporte_dot, destino_reporte_grafico, extension)

}

/*_________________________ VARIABLES GLOBALES _______________*/
var listaS LISTASIMPLE
var actualSesion Sesion
var tamanoCarpeta int32 = int32(unsafe.Sizeof(BLOQUE_CARPETA{}))
var tamanoArchivo int32 = int32(unsafe.Sizeof(BLOQUE_ARCHIVO{}))
var tamanoSuper int32 = int32(unsafe.Sizeof(SUPER_BLOQUE{}))
var tamanoInodo int32 = int32(unsafe.Sizeof(TABLA_INODOS{}))

func InicializarVariablesGlobales() {
	listaS.inicialiarLista()

}
