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

void displayChoice();
long clientFactoriel(int nb, int sock);

int main(int argc, char *argv[])
{
  int sock;
  long res = 0;
  sock = creerSocketUDP(0);
  char k;
  int nb = 0;
  do
  {
    displayChoice();
    scanf(" %c",&k);
    if(k<49 || k>51)
      system("clear");
  }while((k<49)||(k>51));
  switch (k)
  {
    case 49:
      printf("Factoriel de ?:");
      scanf(" %d\n",&nb);
      res = clientFactoriel(nb,sock);
      break;
    case 50:
      printf("2\n");
      break;
    case 51:
      printf("3looll\n");
      break;
  }
  return 0;
}

void displayChoice()
{
  printf("1. Factoriel(n)\n2. Puissance(n)\n3. Moyenne | Min | Max(1..N)\n");
  printf("Entré un nombre entre 1 et 3 ... ");
}

long clientFactoriel(int nb,int sock)
{
  requete req;
  char* message;
  int taille;
  int nb_octets;
  long resultat;
  req.type = FACTORIEL;
  req.taille = sizeof(int);
  taille = sizeof(requete)+ sizeof(int);
  message = (char*)malloc(taille);
  memcpy(message,&req,sizeof(req));
  memcpy(message+sizeof(req),&nb,sizeof(nb));
  nb_octets = write(sock,message,taille);
  free(message);
  if((nb_octets)==0||(nb_octets == -1))
    return (long)-1;
  nb_octets = read(sock,&resultat,sizeof(long));
  if((nb_octets)==0||(nb_octets == -1))
    return (long)-1;
  return resultat;
}
