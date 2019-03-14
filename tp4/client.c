#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include "common.h"

#define TAILLEBUF 20

int main(int argc, char *argv[])
{
  static struct sockaddr_in addr_serveur;
  struct hostent* host_serveur;
  socklen_t lg;
  int sock, port = 4000;
  char* hostname = "localhost";
  char *msg = "bonjour";
  char buffer[TAILLEBUF];
  char* reponse;
  int nb_octets;

  if(argc >= 2)
  {
    hostname = malloc(sizeof(strlen(argv[1]) * sizeof(char)));
    strcpy(hostname,argv[1]);
  }
  if(argc == 3)
    port = atoi(argv[2]);

  sock = creerSocketUDP(0);
  if(sock == -1)
  {
    perror("erreur création socket");
    exit(1);
  }

  host_serveur = gethostbyname(hostname);
  if (host_serveur == NULL)
  {
    perror("erreur adresse serveur");
    exit(1);
  }

  bzero((char*)&addr_serveur, sizeof(struct sockaddr_in));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr, host_serveur -> h_addr, host_serveur -> h_length);

  if(connect(sock,(struct sockaddr*)&addr_serveur,sizeof(struct sockaddr_in)) == -1)
  {
    printf("erreur connexion serveur %d\n",errno);
    exit(1);
  }

  nb_octets = write(sock,msg,strlen(msg)+1);
  nb_octets = read(sock,buffer,TAILLEBUF);

  printf("reponse reçu : %s\n",buffer);

  close(sock);

  return 0;
}
