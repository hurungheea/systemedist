#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>

enum requette_t
{
  FACTORIEL,
  PUISSANCE,
  ANALYSE
};

typedef struct
{
  enum requette_t requette;
  int taille;
}Requette;


int creerSocketUDP(int port);

#endif
