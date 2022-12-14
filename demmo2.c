// Versão de 14/12 /2022 12:08
// demmo e demmo2 são as versões mais avançadas do projeto
// Em demmo2, o bot sabe que descartou uma carta

#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

#define DOIS 2
#define QUATRO 4

void debug(char *message) { fprintf(stderr, "%s\n", message); }

typedef struct {
  char valor[3];
  char naipe[4];
} Carta;

int main() {
  char temp[MAX_LINE];     // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE]; // identificador do seu bot

  setbuf(stdin, NULL);  // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL); // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  int hand_size = 8;
  char valor[20], naipe[20], dtm[100];
  char carta_valor[20], carta_naipe[20];
  char copas[4] = {226, 153, 165, 0};
  char ouros[4] = {226, 153, 166, 0};
  char paus[4] = {226, 153, 163, 0};
  char espadas[4] = {226, 153, 160, 0};
  Carta hand[100];
  char players[100];
  char you[100];
  char my_hand[100];
  char table[100];

  scanf("PLAYERS %[^\n]\n", players);

  scanf("YOU %s\n", my_id);

  scanf("HAND [ %[^\n]\n", temp);
  strncpy(my_hand, temp, strlen(temp) - 1);

  char cartas[100][100];

  sscanf(my_hand, "%s %s %s %s %s %s %s", cartas[0], cartas[1], cartas[2],
         cartas[3], cartas[4], cartas[5], cartas[6]);

  for(int i = 0; i < hand_size; i++) {
    sscanf(cartas[i], "%1s %4s", valor, naipe);
    strcpy(hand[i].naipe, naipe);
    strcpy(hand[i].valor, valor);
  }

  // ./uno bot_A bot_X
  // gcc main.c -o bot_X

  scanf("TABLE %s\n", table);

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];

  char last_action[MAX_ACTION];
  char last_complement[MAX_LINE];

  while (1) {
    do {
      scanf("%s %s", action, complement);
      if (strcmp(action, "DISCARD") == 0) {
        strcpy(last_action, "DISCARD");
        strcpy(last_complement, complement);
        strcpy(table, complement);
        debug(table);
      }

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));

    debug("----- MINHA VEZ -----");

    //char card[] = "A♥ ♥";
    //printf("DISCARD %s\n", card);
    //sscanf(table, "%1s %4s", valor, naipe);


    /*
    "COMPRE 4" : C = CORINGA
    "COMPRE 2" : V = VALETE
    "VOLTA" : D = DAMA
    "PULA A VEZ" : R = REI
    "MUDE A COR" : A = ÀS (muda o naipe)
    */

    // **Tratamento da carta que está na mesa para que o valor e o naipe sejam conhecidos 
    debug(table);
    if(strlen(table) == 4){
      sscanf(table, "%1s %4s", valor, naipe); // Para qualquer carta que não tenha valor 10
    }
    else{
      sscanf(table, "%2s %4s", valor, naipe); // Para carta com valor 10
    }


    // ESTAMOS EM OBRAS
    if(strcmp(valor, "C") == 0){
      // "COMPRE 4"
      printf("BUY %d\n", QUATRO);
    } 
    else if(strcmp(valor, "V") == 0){
      // "COMPRE 2"
      printf("BUY %d\n", DOIS);
    }
    /*else if(strcmp(valor, "D") == 0){
      // "VOLTA"
      printf("VOLTA");
    }*/
    /*else if(strcmp(valor, "R") == 0){
      // "PULA A VEZ"
      printf("PULA A VEZ");
    }*/
    /*else if(strcmp(valor, "A") == 0){
      // "MUDE A COR"
      printf("MUDE A COR");
    }*/
    else{
      // "DISCARTE"
      // CASO NÃO TENHA CARTA VÁLIDA, COMPRE 1
      //printf("DISCARTE");

      // ***TODO Caso seja possível fazer o DISCARD, o trecho seguinte será executado***
      // Procura uma carta compatível com a da mesa
      for(int i = 0; i < hand_size; i++) {
        //sscanf(cartas[i], "%1s %4s", carta_valor, carta_naipe);

        // Tratamento de carta que está na mão do bot
        if(strlen(cartas[i]) == 4){
          sscanf(cartas[i], "%1s %4s", carta_valor, carta_naipe); // Para qualquer carta que não tenha valor 10
        }
        else{
          sscanf(cartas[i], "%2s %4s", carta_valor, carta_naipe); // Para carta com valor 10
        }

        // Verifica compatibilidade
        if((strcmp(carta_valor, valor) == 0) || (strcmp(carta_naipe, naipe) == 0)){
          printf("DISCARD %s\n", cartas[i]);
          strcpy(table, cartas[i]);
          debug(table);

          for(int j = i; j < hand_size-1; j++){
            strcpy(cartas[j], cartas[j+1]);
          }
          hand_size--;
        }
      }
    }
  }

  return 0;
}