#include <stdio.h>
#include <string.h>

// RESOLVER PROBLEMA DO TABLE
// ./uno bot_A bot_X -s 65708 ou 65970
// gcc main.c -o bot_X
// VERSÃO ESTÁVEL PRONTA PARA ENTREGA

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
void buy(int qte, char cartas[][100], int* hand_size){
  printf("BUY %d\n", qte);
  *hand_size += qte;
  if(qte == 4){
    scanf("%s %s %s %s", cartas[(*hand_size)-1], cartas[(*hand_size)-2], cartas[(*hand_size)-3], cartas[(*hand_size)-4]);
  }
  else if(qte == 2){
    scanf("%s %s", cartas[(*hand_size)-1], cartas[(*hand_size)-2]);
  }
  else{
    scanf("%s", cartas[(*hand_size)-1]);
  }
}

// MAIOR
int max(int a, int b){
  if(a > b){
    return a;
  }
  else{
    return b;
  }
}

// CALCULA QUANTAS CARTAS DE CADA NAIPE O BOT POSSUI 
int trocarNaipe(char cartas[][100], int hand_size, char* valor, char* naipe){
  int qte_copas, qte_ouros, qte_espadas, qte_paus;
  int maior;
  qte_copas = qte_ouros = qte_espadas = qte_paus = 0;
  for(int i = 0; i < hand_size; i++){
    if(strlen(cartas[i]) == 4){
      sscanf(cartas[i], "%1s %4s", valor, naipe); 
    }
    else{
      sscanf(cartas[i], "%2s %4s", valor, naipe); 
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

  char cartas[100][100];
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
  sscanf(my_hand, "%s %s %s %s %s %s %s", cartas[0], cartas[1], cartas[2],
         cartas[3], cartas[4], cartas[5], cartas[6]);

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
        trataCarta(cartas[i], carta_valor, carta_naipe);
        if((strcmp(carta_valor, valor) == 0) || (strcmp(carta_naipe, naipe) == 0)){
          if((strcmp(carta_valor, "A") == 0) || strcmp(carta_valor, "C") == 0 ){
            strcpy(table, carta_valor);

            // Estratégia para escolher naipe
            if(trocarNaipe(cartas, hand_size, carta_valor, carta_naipe) == naipe_espadas){
              strcat(table, espadas);
              printf("DISCARD %s %s\n", cartas[i], espadas);
            }
            else if(trocarNaipe(cartas, hand_size, carta_valor, carta_naipe) == naipe_paus){
              strcat(table, paus);
              printf("DISCARD %s %s\n", cartas[i], paus);
            }
            else if(trocarNaipe(cartas, hand_size, carta_valor, carta_naipe) == naipe_copas){
              strcat(table, copas);
              printf("DISCARD %s %s\n", cartas[i], copas);
            }
            else if(trocarNaipe(cartas, hand_size, carta_valor, carta_naipe) == naipe_ouros){
              strcat(table, ouros);
              printf("DISCARD %s %s\n", cartas[i], ouros);
            }
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
        buy(1, cartas, &hand_size); // Comprar 1
      }
    }
  }
  return 0;
}