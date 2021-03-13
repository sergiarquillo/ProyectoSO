#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char nombre[80];
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexión: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "prueba",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexión: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// consulta SQL para obtener una tabla con todos los datos
	// de la base de datos
	err=mysql_query (conn, "SELECT * FROM PLAYER");
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	// En una fila hay tantas columnas como datos tiene una
	// persona. En nuestro caso hay tres columnas: dni(row[0]),
	// nombre(row[1]) y edad (row[2]).
	int i=1;
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) {
			// las columnas 0 y 1 contienen DNI y nombre 
			printf ("Player %i; Name: %s, Password: %s, Wins: %s\n", i, row[1], row[2], row[3]);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
			i++;
	}
		
		
		
		// Ahora vamos a buscar el nombre de la persona cuyo DNI es uno
		// dado por el usuario
		printf ("\nEscribe el nombre del jugador para ver su estadística:\n"); 
		scanf ("%s", nombre);
		// construimos la consulta SQL
		sprintf (consulta,"SELECT SUM(GAME.DURATION) FROM (PLAYER,GAME,BRIDGE) WHERE USERNAME= '%s' AND PLAYER.ID=BRIDGE.ID_PLY2 AND BRIDGE.ID_GA=GAME.ID",nombre); 
		// hacemos la consulta 
		err=mysql_query (conn, consulta); 
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//recogemos el resultado de la consulta 
		resultado = mysql_store_result (conn); 
		row = mysql_fetch_row (resultado);
		if (row == NULL)
			printf ("No se han obtenido datos en la consulta\n");
		else
			while (row !=NULL) {
				// las columnas 0 y 1 contienen DNI y nombre 
				printf ("%s ha jugado un total de %s minutos como jugador 2\n", nombre,row[0]);
				// obtenemos la siguiente fila
				row = mysql_fetch_row (resultado);
		}
			mysql_close (conn);
			exit(0);
}
