#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Cliente TCP
 */
main(argc, argv)
int argc;
char **argv;
{
    unsigned short port; 
    char qaltemp[30];  
    char tempambiente[30];  
    char tempdesejada[30];              
    char recvbuf[30];              
    struct hostent *hostnm;    
    struct sockaddr_in server; 
    int s;                     

    /*
     * O primeiro argumento (argv[1]) é o hostname do servidor.
     * O segundo argumento (argv[2]) é a porta do servidor.
     */
    if (argc != 3)
    {
        fprintf(stderr, "Use: %s hostname porta\n", argv[0]);
        exit(1);
    }

    /*
     * Obtendo o endereço IP do servidor
     */
    hostnm = gethostbyname(argv[1]);
    if (hostnm == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(2);
    }
    port = (unsigned short) atoi(argv[2]);

    /*
     * Define o endereço IP e a porta do servidor
     */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    /*
     * Cria um socket TCP (stream)
     */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(3);
    }

    /* Estabelece conexão com o servidor */
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect()");
        exit(4);
    }
	
	/* Manda para o servidor a requisição de temperatura */
	strcpy(qaltemp,"Qual a temperatura atual?");
	if (send(s, qaltemp, strlen(qaltemp)+1, 0) < 0)
    {
        perror("Send()");
        exit(5);
    }
    
    
    /* Recebe temperatura do ambiente através do servidor */
    if (recv(s, tempambiente, sizeof(tempambiente), 0) < 0)
    {
        perror("Recv()");
        exit(6);
    }
    
	/* Mostra a temperatura do ambiente para o cliente */
    printf("Temperatura ambiente: %s\n",tempambiente);
    
	/* Coleta a temperatura para o ar condicionado desejada pelo cliente */
    printf("\nTemperatura desejada:\n");
    __fpurge(stdin);
    fgets(tempdesejada,30, stdin);
    

    /* Envia a temperatura digitada pelo cliente para o sevidor */
    if (send(s, tempdesejada, strlen(tempdesejada)+1, 0) < 0)
    {
        perror("Send()");
        exit(5);
    }
	
	/* Confirma que a temperatura digitada pelo cliente foi enviada para o servidor */
    printf("Mensagem enviada ao servidor: %s\n", tempdesejada);

  

    sleep(5);

    printf("Cliente terminou com sucesso.\n");
    
     /* Fecha o socket */
    close(s);
    exit(0);

}


