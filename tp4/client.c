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
#include "calculs.h"

#define TAILLEBUF 20

void choixMenu();
void clientPuiss(int sock);
void clientFact(int sock);

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
    perror("impossible de contacter le serveur");
    exit(1);
  }
  char k;
  do
  {
    choixMenu();
    k = getchar();
  }while(k<49||k>51);
  switch (k)
  {
    case 49:
      clientFact(sock);
      break;

    case 50:
      clientPuiss(sock);
      break;

    case 51:

      break;
  }
  close(sock);
  return 0;
}

void choixMenu()
{
  printf("1. Factoriel\n");
  printf("2. Puissance\n");
  printf("3. Moyenne\n");
  printf("4. Fin\n");
}

void clientPuiss(int sock)
{
  char *buffy;
  int nb,expt,nb_octets = 0;
  requete puiss;
  long result = 0;

  puiss.type = PUISSANCE;
  puiss.taille = 0;

  buffy = malloc(sizeof(puiss) + sizeof(int));
  memcpy(buffy,&puiss,sizeof(puiss));

  nb_octets = write(sock,buffy,sizeof(buffy));

  printf("Puissance : de ? ... ");
  scanf("%d",&nb);
  nb_octets = write(sock,&nb,sizeof(int));
  printf("Puissance : exposant ? ");
  scanf("%d",&expt);
  nb_octets = write(sock,&expt,sizeof(int));
  nb_octets = read(sock,&result,sizeof(int));
  printf("la puissance de %d exposant %d est : %ld\n",nb,expt,result);
}

void clientFact(int sock)
{
  int val,nb_octets = 0;
  long result = 0;
  char* buffy;

  printf("Factoriel : de combien ? .. ");
  scanf("%d",&val);
  requete fact;

  fact.type = FACTORIEL;
  fact.taille = 0;

  buffy = malloc(sizeof(fact) + sizeof(int));

  memcpy(buffy,&fact,sizeof(fact));
  memcpy(buffy+sizeof(fact),&val,sizeof(int));

  nb_octets = write(sock,buffy,sizeof(buffy));
  nb_octets = write(sock,&val,sizeof(int));

  if(nb_octets == 0 || nb_octets == -1)
      printf("ERREUR\n");
  nb_octets = read(sock,&result,sizeof(int));
  printf("la factoriel de %d est : %ld\n",val,result);
}
