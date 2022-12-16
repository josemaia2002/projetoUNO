// Versão de 15/12/2022 19:30
// demmo e demmo2 são as versões mais avançadas do projeto
// Em demmo2, o bot sabe que descartou uma carta
// demmo5 é uma versão de testes alpha
// TODO Apimorar leitura das jogadas dos outros bots; update table
// ./uno bot_A bot_X
// gcc main.c -o bot_X

#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

#define UM 1
#define DOIS 2
#define QUATRO 4

void debug(char *message) { fprintf(stderr, "%s\n", message); }

typedef struct {
  char valor[3];
  char naipe[4];
} Carta;

typedef enum {
  true = 1,
  false = 0,
}boolean;

int main() {
  char temp[MAX_LINE];     
  char my_id[MAX_ID_SIZE]; 

  setbuf(stdin, NULL);  
  setbuf(stdout, NULL); 
  setbuf(stderr, NULL);

  int hand_size = 7;
  boolean encontrado;
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

  //debug(cartas[hand_size-1]);

  for(int i = 0; i < hand_size; i++) {
    sscanf(cartas[i], "%1s %4s", valor, naipe);
    strcpy(hand[i].naipe, naipe);
    strcpy(hand[i].valor, valor);
  }
  scanf("TABLE %s\n", table);

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];
  char complement2[MAX_LINE];

  char last_action[MAX_ACTION];
  char last_complement[MAX_LINE];

  while(1) {
    //debug(cartas[hand_size-1]);
    do {
      scanf("%s %s", action, complement);
      if(strcmp(action, "DISCARD") == 0){
        if(strlen(complement) == 4){
          sscanf(complement, "%1s %4s", valor, naipe); 
        }
        else{
          sscanf(complement, "%2s %4s", valor, naipe); 
        }

        if((strcmp(valor, "A") == 0) || (strcmp(valor, "C") == 0)){
          //strcpy(table, complement);
          scanf("%s", complement2);
          strcpy(table, valor);
          strcat(table, complement2);
        }
        else{
          strcpy(table, complement);
        }

      }
    } while(strcmp(action, "TURN") || strcmp(complement, my_id));

    /*for(int i = 0; i < hand_size; i++){
      debug(cartas[i]);
    }*/

    if(strlen(table) == 4){
      sscanf(table, "%1s %4s", valor, naipe); 
    }
    else{
      sscanf(table, "%2s %4s", valor, naipe); 
    }
    /* 
    TODO se for C, V ou R, o bot perde a vez e tem que armazenar a carta que está na mesa
    */

    // Comprar 4
    if(strcmp(valor, "C") == 0){
      printf("BUY %d\n", QUATRO);
      hand_size += 4;
      scanf("%s %s %s %s", cartas[hand_size-1], cartas[hand_size-2], cartas[hand_size-3], cartas[hand_size-4]);
    } 
    // Comprar 2
    else if(strcmp(valor, "V") == 0){
      printf("BUY %d\n", DOIS);
      hand_size += 2;
      scanf("%s %s", cartas[hand_size-1], cartas[hand_size-2]);
    }
    else{
      encontrado = true;
      for(int i = 0; i < hand_size; i++) {
        if(strlen(cartas[i]) == 4){
          sscanf(cartas[i], "%1s %4s", carta_valor, carta_naipe); 
        }
        else{
          sscanf(cartas[i], "%2s %4s", carta_valor, carta_naipe); 
        }
        if((strcmp(carta_valor, valor) == 0) || (strcmp(carta_naipe, naipe) == 0)){
          if((strcmp(carta_valor, "A") == 0) || strcmp(carta_valor, "C") == 0 ){
            strcpy(table, carta_valor);
            strcat(table, espadas);
            printf("DISCARD %s %s\n", cartas[i], espadas);
          }
          else{
            strcpy(table, cartas[i]);
            printf("DISCARD %s\n", cartas[i]);
          }
          for(int j = i; j < hand_size-1; j++){
            strcpy(cartas[j], cartas[j+1]);
          }
          hand_size--;
        }
        else{
          encontrado = false;
        }
      }
      if(encontrado == false){
        printf("BUY %d\n", UM);
        hand_size++;
        scanf("%s", cartas[hand_size-1]);
        //debug(cartas[hand_size-1]);
      }
    }
  }
  return 0;
}