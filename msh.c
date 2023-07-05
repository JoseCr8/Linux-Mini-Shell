//  MSH main file
// Write your msh source code here

//#include "parser.h"
#include <stddef.h>			/* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMANDS 8

/* Global VARIABLE*/
int Acc = 0;
// ficheros por si hay redirección
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];




//funcion mycalc
void mycalc(char * o1, char * operador, char * o2) // los argumentos o1 operador y o2 son los pasados por el msh y corresponden a 2 operandos y la operacion
{
	char error[100];	
	if (o1!=NULL && operador!=NULL && o2!=NULL) // comprobar que los argumentos antes pasados no son nulos en caso contrario saltara a un error
	{
                int a = atoi(o1); // pasamos el argumento o1 correspondiente con el primer operando de str a int
                int b = atoi(o2); // pasamos el argumento o2 correspondiente con el segundo operando de str a int
                
        	//ADD
		if (strcmp(operador, "add") == 0) // comprobamos si el argumento pasado como operando es add para hacer la operacion correspondiente
		{	
			int result = a + b; // como estamos en add sumamos ambos argumentos
			Acc = Acc + result; // y a su vez lo sumamos a Acc para poder enseñar por pantalla el Acc correspondiente
			char buf[20];
			sprintf(buf, "%d", Acc);  // guardamos en la variable buff de tipo char el valor de Acc
			const char *buffer = buf; 
			if (setenv("Acc", buffer, 1) < 0) // comprobamos el valor de la variable de entorno 
			{ 
				printf("Error al dar valor a la variable de entorno\n"); // mostramos por la salida estandar el error correspondiente
			}
			
			fprintf(stderr, "[OK] %d + %d = %d; Acc %s\n", a, b, a + b, getenv("Acc")); // mostramos por la salida estandar de error el resultado de la suma junto con el Acc
		}

		//MOD
		else if (strcmp(operador, "mod") == 0) // comprueba que el argumento operador que se recibe es igual al string mod para ejecutar el programa
		{   
			if(strcmp(o2,"0") != 0) // comprobamos que el argumento pasado como o2 es distinto de cero ya que si lo es no se puede dividir
			{
				fprintf(stderr, "[OK] %d %% %d = %d * %d + %d\n", a, b, b, div(a, b).quot, div(a, b).rem); // imprime por la salida de error estándar la cadena especificada con los correspondientes resultados
			}
			else
			{
			    	printf("[ERROR] no se puede dividir entre 0\n"); // imprime por salida estándar la cadena indicando que se cometió un error al intentar dividir entre cero
			}
		}
		else
		{
			printf("[ERROR] La estructura del comando es <operando 1> <add/mod> <operando 2>\n"); // si el argumento pasado como operando no es add ni mod imprimimos por salida estándar el error correspondiente
		}
 	}
	else
	{
		printf("[ERROR] La estructura del comando es <operando 1> <add/mod> <operando 2>\n"); // si alguno de los argumentos pasados es null imprimimos por salida estándar el error correspondiente
	}
}

//funcion mycp
void mycp(char *  fichero_origen, char * fichero_destino) // definimos la operacion junto a sus entradas
{
	if(fichero_origen != NULL && fichero_destino != NULL)	// comprobamos que los argumentos pasados no son nulos, en caso contrario saltamos al error correspondiente
	{
		int origen = open(fichero_origen, O_RDONLY, 0644);	// abrimos el fichero de origen 
		if(origen >= 0)	// comprobamos que el archivo existe
		{
			char buffer[1024];
			int x_read, x_write;
			int destino = open(fichero_destino, O_TRUNC | O_WRONLY | O_CREAT, 0644); // abrimos o creamos el fichero de destino 
			while((x_read = read(origen, buffer, 1024)) > 0)	// leemos el archivo de origen
			{
				if ((x_write = write(destino, buffer, x_read)) < x_read) // comprueba si se produce un error en la escritura del fichero en caso afirmativo imprime por la salida estándar un error indicandolo y cierra tanto el fichero de origen como el de destino
				{
						if (close(destino) < 0) // comprueba si se produce un error al cerrar el archivo de destino en caso afirmativo imprime por la salida estándar un error indicandolo	
						{
							printf("Error al cerrar el fichero de destino\n"); 									
						}
						if (close(origen) < 0) // comprueba si se produce un error al cerrar el archivo de origen en caso afirmativo imprime por la salida estándar un error indicandolo
						{
							printf("Error al cerrar el fichero de origen\n"); 										
						}
				    	printf("Error al escribir en archivo de destino\n");
				
		        }

			}
			if (x_read < 0) // comprueba si se produce un error en la lectura del fichero en caso afirmativo imprime por la salida estándar un error indicandolo y cierra tanto el fichero de origen como el de destino
            {
				printf("Error al leer fichero\n");
				if (close(destino) < 0) // comprueba si se produce un error al cerrar el archivo de destino en caso afirmativo imprime por la salida estándar un error indicandolo	
				{
					printf("Error al cerrar el fichero de destino\n");									
				}
				if (close(origen) < 0) // comprueba si se produce un error al cerrar el archivo de origen en caso afirmativo imprime por la salida estándar un error indicandolo
				{
					printf("Error al cerrar el fichero de origen\n"); 
				}
			}
             		// Al acabar la lectura se cierran ambos ficheros comprobando si generan un error y se genera un texto por la salida estandar indicando que la copia se hizo con exito

			if (close(destino) < 0) 
			{
				printf("Error al cerrar el fichero de destino\n");										
			}
			if (close(origen) < 0) 
			{
				printf("Error al cerrar el fichero de origen\n");
			}
     		
          	printf("[OK] Copiado con exito el fichero %s a %s\n", fichero_origen, fichero_destino);			
		}

		else	// comprobamos si se produce un error al abrir el archivo de origen, mostrando un error en caso afirmativo a traves de la salida estandar
		{
			printf("[ERROR] Error al abrir el fichero origen : No such file or directory\n");
		}
	}
	else // comprobamos si se produce un error en la estructura del comando, mostrando un error en caso afirmativo a traves de la salida estandar
	{
		printf("[ERROR] La estructura del comando es mycp <fichero origen> <fichero destino>\n");
	}
}






void siginthandler(int param)
{
	printf("****  Saliendo del MSH **** \n");
	//signal(SIGINT, siginthandler);
        exit(0);
}
/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char*** argvv, int num_command) {
    //reset first
    for(int j = 0; j < 8; j++)
        argv_execvp[j] = NULL;

    int i = 0;
    for ( i = 0; argvv[num_command][i] != NULL; i++)
        argv_execvp[i] = argvv[num_command][i];
}


/**
 * Main sheell  Loop  
 */
int main(int argc, char* argv[])
{
    /**** Do not delete this code.****/
    int end = 0; 
    int executed_cmd_lines = -1;
    char *cmd_line = NULL;
    char *cmd_lines[10];

    if (!isatty(STDIN_FILENO)) {
        cmd_line = (char*)malloc(100);
        while (scanf(" %[^\n]", cmd_line) != EOF){
            if(strlen(cmd_line) <= 0) return 0;
            cmd_lines[end] = (char*)malloc(strlen(cmd_line)+1);
            strcpy(cmd_lines[end], cmd_line);
            end++;
            fflush (stdin);
            fflush(stdout);
        }
    }

    /*********************************/

    char ***argvv = NULL;
    int num_commands;

	while (1) 
	{
		int status = 0;
	        int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		// Prompt 
		write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

		// Get command
                //********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
                executed_cmd_lines++;
                if( end != 0 && executed_cmd_lines < end) {
                    command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
                }else if( end != 0 && executed_cmd_lines == end)
                    return 0;
                else
                    command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
                //************************************************************************************************


              /************************ STUDENTS CODE ********************************/
	      if (command_counter > 0) 
	      {
			if (command_counter > MAX_COMMANDS)
                     		printf("Error: Numero máximo de comandos es %d \n", MAX_COMMANDS);
		
                	
	      }
	      
	      for (int i = 0; i < command_counter; i++) // este for incrementa el command_counter en funcion del numero de argumentos pasados a la minishell
	      {
	      		getCompleteCommand(argvv,i);
	      }
	      
	      
		//mandatos simples
		if(command_counter == 1 && (strcmp(argv_execvp[0], "mycalc") != 0 && strcmp(argv_execvp[0], "mycp") != 0)) // en caso de que se pase un unico argumento y no sea ni mycalc ni mycp se ejecuta como comando simple
		{
	      	int filehandle = 0; //  variable que sirveç guardar el file descriptor para las redirecciones
			int pid = fork();
			if (pid == -1)
			{
				printf("Error pid\n");// mensaje de error al hacer fork por la salida estandar
			}
			if(pid == 0)
			{
				//redirecciones
				if(strcmp(filev[1], "0") != 0) // redireccion de de salida
		        	{
					if((close (STDOUT_FILENO)) <0) // se cierra la salida estandar
					{
						printf("Error al cerrar descriptor\n"); // mensaje de error al cerrar el descriptor por la salida estandar
						return -1;
					}
					if ((filehandle = open(filev[1], O_TRUNC | O_WRONLY | O_CREAT, 0666)) < 0) // se abre el archivo de destino
					{
						printf("Error al abrir fichero\n");
						return -1;				
					}
		         	}
		         	if(strcmp(filev[0], "0") != 0) // redireccion de entrada
		         	{
					if((close (STDIN_FILENO))<0) // se cierra la entrada estandar
					{
						printf("Error al cerrar descriptor\n");
						return -1;
					}
					if ((filehandle = open(filev[0], O_RDWR, 0666)) < 0) // se abre el archivo del cual se va a leer
					{
						printf("Error al abrir fichero\n");
						return -1;
					}
		      	 	}
				 if(strcmp(filev[2], "0") != 0) // redireccion de salida de error
		 		 {
				 	if((close (STDERR_FILENO)) <0) // se cierra la salida estandar de error
					{
						printf("Error al cerrar descriptor\n");
						return -1;
					}
					if ((filehandle = open(filev[2], O_TRUNC | O_WRONLY | O_CREAT, 0666)) < 0) // se abre el archivo de destino de la redireccion de error
					{
						printf("Error al abrir fichero\n");
						return -1;
					}
				}
				//ejecucion
				if (execvp(argv_execvp[0], argv_execvp) == -1) // se ejecuta el mandato simple
				{
					printf("Error al ejecutar 1\n");
					return -1;
				}
				
			}
			if(in_background) // si el proceso esta en background
			{
				printf("[%d]\n", getpid()); // se imprime por la salida estandar el pid del proceso en background	
			}
	        	while(pid != wait(&status));
	      		
	      	
	    }
	      
        // cadenas de mandatos
        if(command_counter > 1) // en caso de que se pasen mas de un argumento se ejecutan como cadenas de comandos
        {
        	int pd[2]; //
            int ret, p10;
            int n = command_counter; // variable para saber en que mandato estamos(que mandato se esta ejecutando)
            pid_t pid; // pid del proceso
            int filehandle; // variable para almacenar el file descriptor para las redirecciones
             	
            for(int i = 0; i < n; i++)
            {
             	if(i != (n-1)) // si no es el ultimo proceso(hijo)
             	{
             		ret = pipe(pd); // crea una pipe
             		if(ret < 0) // comprobacion de que no ha habido error al crear la pipe
             		{
             			perror("pipe:");
             			exit(-1);
             		}
             	}
             	pid = fork(); // se crea el siguiente proceso hijo

             	if(pid < 0) // se comprueba que se ha creado el proceso hijo
             	{
             		perror("fork:");
             		exit(-1);
             	}

             	if(pid == 0) // ejecucion del hijo
             	{//hijo
             			
             		//redirecciones
             		if(strcmp(filev[2], "0") != 0) // si se recibe redireccion de salida estandar de error
		            {
						if((close (STDERR_FILENO)) <0) // se cierra la salida estandar de error
						{
							printf("Error al cerrar descriptor\n");
							return -1;
						}
						if ((filehandle = open(filev[2], O_TRUNC | O_WRONLY | O_CREAT, 0644)) < 0) // se abre el archivo de destino 
						{
							printf("Error al abrir fichero\n");
							return -1;
						}
		            }
             		if(i==0 && strcmp(filev[0], "0") != 0) // si es el primer proceso, se puede hacer redireccion de entrada
		            {
						if((close (STDIN_FILENO))<0) // se cierra la entrada estandar
						{
							printf("Error al cerrar descriptor\n");
							return -1;
						}
						
						if ((filehandle = open(filev[0], O_RDWR, 0644)) < 0) // se abre el archivo del que se va a leer
						{
							printf("Error al abrir fichero\n");
							return -1;
						}
		            }
					if(strcmp(filev[1], "0") != 0) // si se recibe redireccion de la salida estandar
		            {
						if((close (STDOUT_FILENO)) <0) // se cierra la salida estandar
						{
							printf("Error al cerrar descriptor\n");
							return -1;
						}

						if ((filehandle = open(filev[1], O_TRUNC | O_WRONLY | O_CREAT, 0644)) < 0) // se abre el archivo de destino
						{
							printf("Error al abrir fichero\n");
							return -1;				
						}
		           	}
             			
             		if(i != 0) // si no es el primer proceso se hace redireccion y limpieza de las entradas para el correcto uso de las pipes por el siguiente proceso
             		{//entrada
             			close(0); dup2(p10,STDIN_FILENO);
             			close(p10);
             		}
             		if(i != (n-1)) // si no es el ultimo proceso se hace redireccion y limpieza de las salidas para el correcto uso de las pipes por el siguiente proceso
             		{//salida
             			close(1); dup2(pd[1],STDOUT_FILENO);
             			close(pd[0]); close(pd[1]);	
             		}
             			
         			getCompleteCommand(argvv, i);
         			if(in_background)// si se encuentra en background
         			{	
						printf("[%d]\n", getpid());
					
					}
             		if (execvp(argv_execvp[0], argv_execvp) == -1) // se ejecuta el manadato 
					{
						printf("Error al ejecutar 2\n");
					
					}
				
					if(filehandle!=0) // si hay algun descriptor abierto usado en las redirecciones de entrada y salida de los procesos
		     		{
		     			if((close(filehandle)) <0) // se cierra 
						{
							printf("Error al cerrar descriptor\n");
						}
		     		}
             			
					
             	}
             	else // ejecucion padre
             	{//padre
             		if(i != (n-1)) // si no es el ultimo proceso
             		{
             			close(pd[1]);
             			p10 = pd[0];
             		}
             		else // cuando se llega al ultimo proceso 
             		{
             			close(pd[0]); 
             		}    		
             	}
             	
             	if(filehandle!=0)
             	{
             		if((close(filehandle)) <0)
					{
						printf("Error al cerrar descriptor\n");
					}
             	}
             	
             		
			while(pid != wait(&status));
					
	  }	
        }
        //mycalc
		if (strcmp(argv_execvp[0], "mycalc") == 0)
		{
			mycalc(argv_execvp[1],argv_execvp[2],argv_execvp[3]);
		}
	      
	    //mycp
	    if(strcmp(argv_execvp[0], "mycp") == 0)
	    {
		   mycp(argv_execvp[1],argv_execvp[2]);	
	    }
	}
	return 0;	
}









