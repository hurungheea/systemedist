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
void clientFact(int sock);
void clientPuiss(int sock);
void moyenClient(int sock);
void fermerConnexion(int sock);

int main(int argc, char *argv[])
{
  static struct sockaddr_in addr_serveur;
  struct hostent* host_serveur;
  socklen_t lg;
  int sock, port = 8080;
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
  }while(k<49||k>52);
  switch (k)
  {
    case 49:
      clientFact(sock);
      break;

    case 50:
      clientPuiss(sock);
      break;

    case 51:
      moyenClient(sock);
      break;

    case 52:
      fermerConnexion(sock);
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
  printf("\033[32mla puissance de %d exposant %d est : %ld\033[0m\n",nb,expt,result);
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

  nb_octets = write(sock,buffy,sizeof(fact) + sizeof(int));

  if(nb_octets == 0 || nb_octets == -1)
      printf("ERREUR\n");
  nb_octets = read(sock,&result,sizeof(int));
  printf("\033[32mla factoriel de %d est : %ld\033[0m\n",val,result);
}

void moyenClient(int sock)
{
  int *val;
  int k, nb_octets = 0;
  int N = 1;
  char* buffy;
  long result;

  printf("De combien de valeur voulez vous faire la moyenne ? : \n");
  scanf("%d",&N);
  val = malloc(sizeof(int) * N);
  for(int i=0; i < N; i++)
  {
    printf("Entre le %d eme nombre", i+1);
    scanf("%d", &k);
    val[i]=k;
  }
  requete moy;

  moy.type = STATS;
  moy.taille = N;

  buffy = malloc(sizeof(moy)+ sizeof(int) * N);
  memcpy(buffy, &moy, sizeof(moy));
  memcpy(buffy+sizeof(moy), val, sizeof(int) * N);

  nb_octets = write(sock, buffy, sizeof(moy)+ sizeof(int) * N);
  if(nb_octets == 0 || nb_octets == -1)
      printf("ERREUR\n");
  nb_octets = read(sock,&result,sizeof(long));
  if(nb_octets == 0 || nb_octets == -1)
      printf("ERREUR\n");
  printf("\033[32mla moyenne est : %ld\033[0m\n",result);
}

void fermerConnexion(int sock)
{
  int nb_octets = 0;
  char *buffy;
  requete end;

  end.type = FIN;
  end.taille = 0;
  buffy = malloc(sizeof(end));
  memcpy(buffy,&end,sizeof(end));

  nb_octets = write(sock,&end,sizeof(end));
  if(nb_octets == 0 || nb_octets == -1)
      printf("ERREUR\n");
}
