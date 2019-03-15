#ifndef _CALCULS_H_
#define _CALCULS_H_

struct res_analyse_donnees {
  float moy;
  int max;
  int min;
};

long factoriel(int nb);

void analyser_donnees(int donnees[], int taille, struct res_analyse_donnees *res);

long puissance(int nb, int puiss);

#endif
