/* 
 * File:   beach_volley.c
 * Author: Chiara Leonelli
 * Created on 31 gennaio 2015, 10.54
 * Simulazione di un match di gioco 'bach volley' fra due giocatori:
 * 
 * - Scelta pseudocasuale della posizione dei Player all'interno del 
 *   campo (array 10 x 10) (generazione pseudo casuale dell'indice di un array 2D)
 *   
 * - Scelta pseudocasuale del primo giocatore che lancia la palla
 * 
 * - Lancio della palla nel campo avversario (generazione pseudocasuale dell'indice
 *   di un array 2D) con possibilità del 10% di cadere sulla rete, del 20% di andare fuori. 
 * 
 * - Check sulla posizione in cui è caduta la palla, con conseguente assegnazione
 *   del punto (se la palla è caduta lontana dal giocatore ricevente) e ritiro della
 *   palla.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct player {
    int rowPosition;
    int columPosition;
    char labelString[3];
    int labelInt;
    int score;
};
typedef struct player Player;

struct match {
    int (*playingField)[10];
    Player player1;
    Player player2;
};


typedef struct match Match;

void printPlayingField(Match *matchParam, int columBall, int rowBall);
void insertPlayers(Match *matchParam);
int getScore(Player playerParam, int columPosition, int rowPosition);
void launchBall(Match *matchParam, int playerLauchBall);
int selectFirstPlayer();
void newMatch();


void main() {
    srand(time(NULL));
    int getOtherMatch;
    do {
        newMatch();
        printf("\nVuoi simulare un altro match di gioco?(0/1) ");
        scanf(" %d", &getOtherMatch);
    } while (getOtherMatch == 1);
}

/*
 Funzione che visualizza il campo di gioco  
 Match *matchParam: 
   E' il dato che contiene tutte le informazioni inerenti il match di gioco 
   che si sta svolgendo
 int columBall, rowBall:
   La posizione in cui è caduta la palla, se presente (altrimenti il valore 
   delle variabili sarà 20,20 che è fuori dal campo )
 */
void printPlayingField(Match *matchParam, int columBall, int rowBall) {
    int i, j, val;
    printf("------------------------------\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (i == 5 && j == 0) {
                printf(" ----------------------------\n");
            }
            val = matchParam->playingField[i][j];
            if (val == matchParam->player1.labelInt) {
                printf(" %s", matchParam->player1.labelString);
            } else if (val == matchParam->player2.labelInt) {
                printf(" %s", matchParam->player2.labelString);
            } else if (i == rowBall && j == columBall) {
                printf(" BL");
            } else {
                printf(" %d ", matchParam->playingField[i][j]);
            }
            if (j == 9) {
                printf("\n");
            }
        }
    }
    printf("------------------------------\n");
    printf("P1 punti: %d\n", matchParam->player1.score);
    printf("P2 punti: %d\n", matchParam->player2.score);
}
/*
Funzione che inserisce i giocatori all'interno della matrice, generando gli 
indici in modo casuale
Match *matchParam: 
  E' il dato che contiene tutte le informazioni inerenti il match di gioco 
  che si sta svolgendo
*/ 
void insertPlayers(Match *matchParam) {
    int i1 = rand() % 5;
    int j1 = rand() % 10;
    int j2 = rand() % 10;
    int i2;
    do {
        i2 = rand() % 10;
    } while (i2 < 5);

    matchParam->player1.columPosition = j1;
    matchParam->player1.rowPosition = i1;
    matchParam->player2.columPosition = j2;
    matchParam->player2.rowPosition = j2;
    matchParam->playingField[i1][j1] = matchParam->player1.labelInt;
    matchParam->playingField[i2][j2] = matchParam->player2.labelInt;

}
/*
Funzione che effettua il controllo che 'la palla' si caduta nelle celle di 
distanza 1 intorno al giocatore.
Ritorna 0 se la palle è lontana dal giocatore, 1 se è vicina
Player playerParam
  contiene tutti i dati relativi al giocatore che riceve 'la palla'
int columPosition, int rowPosition 
  La posizione in cui è caduta la palla
*/ 
int getScore(Player playerParam, int columPosition, int rowPosition) {
    int checkPosition = 0;
    if (playerParam.rowPosition == rowPosition || playerParam.rowPosition == rowPosition + 1 || playerParam.rowPosition == rowPosition - 1) {
        if (playerParam.columPosition == columPosition || playerParam.columPosition == columPosition + 1 || playerParam.columPosition == columPosition - 1) {
            checkPosition = 1;
        }
    }
    return (checkPosition);
}
/*
Funzione che simula il lancio della palla 
Match *matchParam: 
  E' il dato che contiene tutte le informazioni inerenti il match di gioco 
  che si sta svolgendo
int playerLauchBall
  assume i valori 0 o 1, nel caso di 0 è il giocatore 1 che tira la palla,
  nel caso di 1 è il giocatore 2 che tira la palla
*/ 
void launchBall(Match *matchParam, int playerLauchBall) {
    if (matchParam->player1.score >= 20 || matchParam->player2.score >= 20) {
        if(matchParam->player1.score >= 20){
            printf("\nMatch di gioco finito, vince %s %d a %d \n", matchParam->player1.labelString, matchParam->player1.score, matchParam->player2.score);
        }else{
            printf("\nMatch di gioco finito, vince %s %d a %d \n", matchParam->player2.labelString, matchParam->player2.score, matchParam->player1.score);
        }
    } else {
        int errors = rand() % 100;
        if (playerLauchBall == 0) {
            printf("\n%s lancia la palla...\n", matchParam->player1.labelString);
        } else {
            printf("\n%s lancia la palla...\n", matchParam->player2.labelString);
        }
        if (errors < 10) {
            printf("Palla caduta sulla rete, punto e palla all'avversario.\n");
            if (playerLauchBall == 0) {
                matchParam->player2.score += 1;
                printPlayingField(matchParam, 20, 20);
                launchBall(matchParam, 1);
            } else {
                matchParam->player1.score += 1;
                printPlayingField(matchParam, 20, 20);
                launchBall(matchParam, 0);
            }
        }
        if (errors > 9 && errors < 20) {
            printf("\nPalla fuori, punto e palla all'avversario.\n");
            if (playerLauchBall == 0) {
                matchParam->player2.score += 1;
                printPlayingField(matchParam, 20, 20);
                launchBall(matchParam, 1);
            } else {
                matchParam->player1.score += 1;
                printPlayingField(matchParam, 20, 20);
                launchBall(matchParam, 0);
            }
        }
        if (errors > 19) {
            if (playerLauchBall == 0) {
                int j2 = rand() % 10;
                int i2;
                do {
                    i2 = rand() % 10;
                } while (i2 < 5);
                if (getScore(matchParam->player2, j2, i2) == 1) {
                    printf("\n%s intercetta la palla e la rilancia.\n", matchParam->player2.labelString);
                    printPlayingField(matchParam, j2, i2);
                    launchBall(matchParam, 1);
                } else {
                    printf("\n%s segna un punto. Rilancia la palla.\n", matchParam->player1.labelString);
                    matchParam->player1.score += 1;
                    printPlayingField(matchParam, j2, i2);
                    launchBall(matchParam, 0);
                }


            } else {
                int i1 = rand() % 5;
                int j1 = rand() % 10;
                if (getScore(matchParam->player1, j1, i1) == 1) {
                    printf("\n%s intercetta la palla e la rilancia.\n", matchParam->player1.labelString);
                    printPlayingField(matchParam, j1, i1);
                    launchBall(matchParam, 0);

                } else {
                    printf("\n%s segna un punto. Rilancia la palla.\n", matchParam->player2.labelString);
                    matchParam->player2.score += 1;
                    printPlayingField(matchParam, j1, i1);
                    launchBall(matchParam, 1);
                }
               

            }
        }
    }
};

//Funzione che sceglie il giocatore che deve tirare la palla, ritorna 0 per il
//giocatore 1 e 1 per il giocatore 2.
int selectFirstPlayer() {
    int playerLaunchBall = 0;
    if (rand() % 2 == 1) {
        playerLaunchBall = 1;
    }
    return playerLaunchBall;
};

//Funzione che simula un match di gioco completo
void newMatch() {
    Player player1 = {0, 0, "P1", 2, 0},
    player2 = {0, 0, "P2", 3, 0};

    int playingField[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    Match firstMatch;
    firstMatch.player1 = player1;
    firstMatch.player2 = player2;
    firstMatch.playingField = playingField;
    insertPlayers(&firstMatch);
    printPlayingField(&firstMatch, 20, 20);
    launchBall(&firstMatch, selectFirstPlayer());
};



