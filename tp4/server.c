#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include "calculs.h"
#include "common.h"

#define TAILLEBUF 20

void traiter_communication(int socket_service);

int main(int argc, char const *argv[])
{
  // adresse de la socket coté client
  static struct sockaddr_in addr_client;
  // adresse de la socket locale
  static struct sockaddr_in addr_serveur;
  // taille de l'addresse socket
  int lg_addr;
  int port = 4000;
  int socket_ecoute,socket_service;
  char message[TAILLEBUF];
  char *chaine_recue;
  char *reponse = "bien recu";
  int nb_octets;

  socket_ecoute = creerSocketUDP(port);
  bzero((char*)&addr_serveur,sizeof(addr_serveur));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  addr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
  if(listen(socket_ecoute,5) == -1)
  {
    printf("erreur listen %d\n",errno);
    exit(1);
  }

  lg_addr = sizeof(struct sockaddr_in);
  while(1)
  {
    socket_service = accept(socket_ecoute,(struct sockaddr*)&addr_client,&lg_addr);
    if(socket_service == -1)
    {
      printf("erreur accept %d\n",errno);
      exit(1);
    }
    if(fork() == 0)
    {
      close(socket_ecoute);
      printf("coucou :)\n");
      traiter_communication(socket_service);
      exit(0);
    }
  }
  return 0;
}

void traiter_communication(int socket_service)
{
  requete req;
  char* message;
  int taille, nb_octets = 0, finis = 0;
  int nb;
  while(!finis)
  {
    nb_octets = read(socket_service,&req,sizeof(req));
    if((nb_octets == 0)||(nb_octets == -1))
    {
      finis = 1;
      break;
    }
    switch (req.type)
    {
      case FIN:
        finis = -1;
        break;

      case FACTORIEL:
        nb_octets = read(socket_service,&nb,sizeof(int));
        long res = factoriel(nb);
        nb_octets = write(socket_service,&req,sizeof(req));
        break;
    }
  }
  close(socket_service);
}
