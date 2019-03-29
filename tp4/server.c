#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "common.h"
#include "calculs.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

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
  pid_t pid_fils;
  int port = 8080;
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
  printf("processus père --> :  %d\n",(int) getpid());
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
      pid_fils = getpid();
      printf("processus fils --> :  %d\n",(int) getpid());
      close(socket_ecoute);
      traiter_communication(socket_service);
      printf("destruction du fils --> :  %d\n",(int) getpid());
      exit(0);
    }
    wait(NULL);
    printf("retour wait : %d\n",wait(NULL));
  }
  return 0;
}

void traiter_communication(int socket_service)
{
  requete req;
  char* message;
  int* tab;
  int taille, nb_octets = 0, finis = 0;
  int nb,expt;
  struct res_analyse_donnees c;
  long res = 0;
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
        finis = 1;
        break;

      case FACTORIEL:
        nb_octets = read(socket_service,&nb,sizeof(int));
        res = factoriel(nb);
        nb_octets = write(socket_service,&res,sizeof(int));
        printf("\033[32mle client nous a demandé FACTORIEL de %d = %ld\033[0m\n",nb,res);
        break;

      case PUISSANCE:
        nb_octets = read(socket_service,&nb,sizeof(int));
        nb_octets = read(socket_service,&expt,sizeof(int));
        res = puissance(nb,expt);
        nb_octets = write(socket_service,&res,sizeof(int));
        printf("le client nous a demandé PUISSANCE de %d exposant %d = %ld\n",nb,expt,res);
        break;

      case STATS:
        tab = malloc(sizeof (int) * req.taille);
        nb_octets = read(socket_service, tab, sizeof(tab));
        analyser_donnees(tab, req.taille,&c);
        break;
    }
  }
  close(socket_service);
}
