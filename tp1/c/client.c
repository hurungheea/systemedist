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

#define my_sizeof(x) ((&x + 1) - &x)
#define TAILLEBUF 20

int main(int argc, char *argv[])
{
  struct hostent* serveur_host;
  static struct sockaddr_in addr_serveur;
  socklen_t lg;
  int sock, port = 4020;
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

  sock = creerSocketUDP(port);
  if(sock == -1)
  {
    perror("erreur création socket");
    exit(1);
  }

  serveur_host = gethostbyname(hostname);
  if (serveur_host == NULL)
  {
    perror("erreur adresse serveur");
    exit(1);
  }

  bzero(&addr_serveur, sizeof(struct sockaddr_in));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr, serveur_host -> h_addr, serveur_host -> h_length);

  lg = sizeof(struct sockaddr_in);
  nb_octets = sendto(sock, msg, strlen(msg)+1, 0, (struct sockaddr*)&addr_serveur,lg);
  if(nb_octets == -1)
  {
    perror("erreur envoi message");
    exit(1);
  }
  printf("paquet envoyé, nb_octets = %d\n", nb_octets);
  nb_octets = recvfrom(sock, buffer, TAILLEBUF, 0, (struct sockaddr*)&addr_serveur,&lg);
  if(nb_octets == -1)
  {
    perror("erreur réponse message");
    exit(1);
  }
  reponse = (char*)malloc(nb_octets * sizeof(char));
  memcpy(reponse, buffer, nb_octets);
  printf("reponse reçue du serveur : %s\n",reponse);
  close(sock);



  return 0;
}
