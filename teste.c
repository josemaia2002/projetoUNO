#include <stdio.h>
#include <string.h>

// RESOLVER PROBLEMA DO TABLE
// ./uno bot_A bot_X -s 65708 ou 65970
// gcc main.c -o bot_X
// VERSÃO DE TESTES

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

#define UM 1
#define DOIS 2
#define QUATRO 4

char copas[4] = {226, 153, 165, 0};
char ouros[4] = {226, 153, 166, 0};
char paus[4] = {226, 153, 163, 0};
char espadas[4] = {226, 153, 160, 0};

void debug(char *message) {
  fprintf(stderr, "%s\n", message);
}

typedef enum {
  true = 1,
  false = 0,
}boolean;

enum naipes { 
  naipe_espadas = 1, 
  naipe_paus = 2, 
  naipe_copas = 3, 
  naipe_ouros = 4 
};

typedef struct {
  char carta[10];
  char valor[10];
  char naipe[10];
} Card;

// Função para verificar tamanho da string e tratá-la, separando o valor e o naipe
void trataCarta(char* carta, char* valor, char* naipe){
  if(strlen(carta) == 4){
    sscanf(carta, "%1s %4s", valor, naipe); 
  }
  else{
    sscanf(carta, "%2s %4s", valor, naipe); 
  }
}

// FUNÇÃO PARA COMPRAR CARTAS
void buy(int qte, Card* cartas, int* hand_size){
  printf("BUY %d\n", qte);
  *hand_size += qte;
  if(qte == 4){
    scanf("%s %s %s %s", cartas[(*hand_size)-1].carta, cartas[(*hand_size)-2].carta, cartas[(*hand_size)-3].carta, cartas[(*hand_size)-4].carta);
  }
  else if(qte == 2){
    scanf("%s %s", cartas[(*hand_size)-1].carta, cartas[(*hand_size)-2].carta);
  }
  else{
    scanf("%s", cartas[(*hand_size)-1].carta);
  }
}

// FUNÇÃO PARA ENCONTRAR O MAIOR ENTRE DOIS INTEIROS
int max(int a, int b){
  if(a > b){
    return a;
  }
  else{
    return b;
  }
}

// CALCULA QUANTAS CARTAS DE CADA NAIPE O BOT POSSUI 
int trocarNaipe(Card* cartas, int hand_size, char* valor, char* naipe){
  int qte_copas, qte_ouros, qte_espadas, qte_paus;
  int maior;
  qte_copas = qte_ouros = qte_espadas = qte_paus = 0;
  for(int i = 0; i < hand_size; i++){
    if(strlen(cartas[i].carta) == 4){
      sscanf(cartas[i].carta, "%1s %4s", valor, naipe); 
    }
    else{
      sscanf(cartas[i].carta, "%2s %4s", valor, naipe); 
    }
    if(strcmp(naipe, espadas) == 0){
      qte_espadas++;
    }
    else if(strcmp(naipe, paus) == 0){
      qte_paus++;
    }
    else if(strcmp(naipe, copas) == 0){
      qte_copas++;
    }
    else if(strcmp(naipe, ouros) == 0){
      qte_ouros++;
    }
  }

  maior = max(max(qte_espadas, qte_paus), max(qte_copas, qte_ouros));

  int arr[] = {qte_espadas, qte_paus, qte_copas, qte_ouros};

  for(int i = 0; i < 4; i++){
    if(arr[i] == maior){
      return i+1;
    }
  }
}

int main() {
  char temp[MAX_LINE];   // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE];  // identificador do seu bot

  setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  int hand_size = 7;
  boolean encontrado;

  //char cartas[100][100];
  Card cartas[100];
  char valor[20], naipe[20];
  char carta_valor[20], carta_naipe[20];
  
  char players[100];
  char you[100];
  char my_hand[100];
  char table[100];

  scanf("PLAYERS %[^\n]\n", players);

  scanf("YOU %s\n", my_id);

  scanf("HAND [ %[^\n]\n", temp); // Recebe as cartas inicias
  // Faz o tratamento e armazenamento das cartas
  strncpy(my_hand, temp, strlen(temp) - 1);
  sscanf(my_hand, "%s %s %s %s %s %s %s", cartas[0].carta, cartas[1].carta, cartas[2].carta,
         cartas[3].carta, cartas[4].carta, cartas[5].carta, cartas[6].carta);

  // Lê a carta aberta sobre a mesa.
  scanf("TABLE %s\n", table);

  // === PARTIDA ===

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];
  char complement2[MAX_LINE];

  while(1) {
    do {
      scanf("%s %s", action, complement);
      if(strcmp(action, "DISCARD") == 0){
        trataCarta(complement, valor, naipe);
        if((strcmp(valor, "A") == 0) || (strcmp(valor, "C") == 0)){
          scanf("%s", complement2);
          strcpy(table, valor);
          strcat(table, complement2);
        }
        else{
          strcpy(table, complement);
        }
      }
    } while (strcmp(action, "TURN") || strcmp(complement, my_id));
    trataCarta(table, valor, naipe);
    if(strcmp(valor, "C") == 0){
      buy(4, cartas, &hand_size); // Comprar 4
    } 
    else if(strcmp(valor, "V") == 0){
      buy(2, cartas, &hand_size); // Comprar 2
    }
    else{
      encontrado = true;
      for(int i = 0; i < hand_size; i++){
        trataCarta(cartas[i].carta, cartas[i].valor, cartas[i].naipe);
        if((strcmp(cartas[i].valor, valor) == 0) || (strcmp(cartas[i].naipe, naipe) == 0)){
          if((strcmp(cartas[i].valor, "A") == 0) || strcmp(cartas[i].valor, "C") == 0 ){
            strcpy(table, cartas[i].valor);

            // Estratégia para escolher naipe
            if(trocarNaipe(cartas, hand_size, cartas[i].valor, cartas[i].naipe) == naipe_espadas){
              strcat(table, espadas);
              printf("DISCARD %s %s\n", cartas[i], espadas);
            }
            else if(trocarNaipe(cartas, hand_size, cartas[i].valor, cartas[i].naipe) == naipe_paus){
              strcat(table, paus);
              printf("DISCARD %s %s\n", cartas[i], paus);
            }
            else if(trocarNaipe(cartas, hand_size, cartas[i].valor, cartas[i].naipe) == naipe_copas){
              strcat(table, copas);
              printf("DISCARD %s %s\n", cartas[i], copas);
            }
            else if(trocarNaipe(cartas, hand_size, cartas[i].valor, cartas[i].naipe) == naipe_ouros){
              strcat(table, ouros);
              printf("DISCARD %s %s\n", cartas[i], ouros);
            }
          }
          else{
            strcpy(table, cartas[i].carta);
            printf("DISCARD %s\n", cartas[i].carta);
          }

          for(int j = i; j < hand_size-1; j++){
            strcpy(cartas[j], cartas[j+1].carta);
          }
          hand_size--;
        }
        else{
          encontrado = false;
        }
      }
      if(encontrado == false){
        buy(1, cartas, &hand_size); // Comprar 1
      }
    }
  }
  return 0;
}