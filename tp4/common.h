#ifndef C_COMMON_H
#define C_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>

enum requete_t
{
  FACTORIEL,PUISSANCE,STATS,FIN
};

typedef struct
{
  enum requete_t type;
  int taille;
}requete;

int creerSocketUDP(int port);

#endif
