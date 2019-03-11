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

int main(int argc, char const *argv[])
{
  // adresse de la socket locale
  static struct sockaddr_in addr_local;
  // adresse de la socket coté serveur
  static struct sockaddr_in addr_client;
  // identifiant du client
  struct hostent *host_client;
  // taille de l'addresse socket
  socklen_t lg;
  // descripteur de la socket locale
  int sock, port = 4000;
  // chaine  à envoyer en réponse
  char *reponse = "bien recu";
  // buffer de réception
  char buffer[TAILLEBUF];
  // chaine reçue
  char *chaine;
  // nombre d'octets lus ou envoyés
  int nb_octets;

  if(argc == 2)
    port = atoi(argv[1]);

  sock = creerSocketUDP(port);
  if(sock == -1)
  {
    perror("erreur création socket");
    exit(1);
  }

  lg = sizeof(struct sockaddr_in);
  nb_octets = recvfrom (sock, buffer, TAILLEBUF, 0, (struct sockaddr*)&addr_client, &lg);
  if (nb_octets ==-1)
  {
    printf("erreur réception paquet %d\n",errno);
    exit(1);
  }

  host_client = gethostbyaddr(&(addr_client.sin_addr), sizeof(long), AF_INET);
  if (host_client == NULL)
  {
    perror("erreur gethostbyaddr");
    exit(1);
  }

  chaine = (char*) malloc(nb_octets * sizeof(char));
  memcpy (chaine, buffer, nb_octets);
  printf ("reçu message %s de la part de %s sur le port %d\n", chaine, host_client ->h_name, ntohs(addr_client.sin_port));
  nb_octets = sendto(sock, reponse, strlen(reponse)+1, 0, (struct sockaddr*) &addr_client, lg);
  if (nb_octets ==-1)
  {
    perror("erreur envoi réponse");
    exit(1);
  }
  close (sock);
  return 0;
}
