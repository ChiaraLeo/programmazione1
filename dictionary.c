/* 
 * File:   dictionary.c
 * Author: Chiara Leonelli
 * Created on 7 febbraio 2015, 14.07
 * 
 * Simulazione della gestione di un dizionario della lingua italiana:
 * 
 * - Presentazione di un menu di scelta tra:
 * 
 *    -Inserisci una nuova parola
 *      Permette all'utente di inserire una nuova parola all'interno del dizionario
 *       
 *    -Ricerca una parola e visualizzane il significato
 *      Permette all'utente di ricercare una parola all'interno de dizionario 
 * 
 *    -Visualizza la parola con più consonanti
 *      Visualizza la parola con più consonanti e il suo significato
 * 
 *    -Visualizza tutte le parole del dizionario
 *      Visualizza tutte la parole e il loro significato
 *      
 *    -Esci
 *      Esce dal programma
 *      
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int max_word_lenght = 20, max_word_sense_lenght = 100;

struct word {
    char wordName[20];
    char wordSense[100];
    int numConsonant;
    int numVowels;
};

typedef struct word Word;

struct dictionary {
    Word words[200];
    Word *consonantsWord;
    int effectiveSize;
};

typedef struct dictionary Dictionary;
Dictionary new_dictionary;

void viewOptions();
void startOption();
int searchWord(char key[max_word_lenght]);
void insertNewWord();
void printAllDictionary();
int countConsonantWord(char *wordParam);

void main() {
    Word new_word = {"abete", "Nome di alcuni alberi della famiglia delle Pinacee a chioma conica.", 2, 3}, new_word2 = {"arancia", "Il frutto dell'arancio.", 3, 4};
    new_dictionary.words[0] = new_word;
    new_dictionary.words[1] = new_word2;
    new_dictionary.consonantsWord = &new_dictionary.words[1];
    new_dictionary.effectiveSize = 2;
    printf("\nDizionario lingua italiana \n");
    viewOptions();
}

/*
 * Funzione che dato in input il puntatore ad un 'Word'
 * ritorna il numero di consonanti e popola rispettivamente
 * il numConsonant e numVowels della Word a cui punta wordParam
 
 */

int countConsonantsWord(Word *wordParam) {
    int i, length, v = 0, c = 0;
    char string[max_word_lenght];
    stpcpy(string, wordParam->wordName);
    length = strlen(string);
    char vowels[15] = "aeiouAEIOU";
    for (i = 0; i < length; i++) {
        if (strchr(vowels, string[i]) != NULL) v++;
        else c++;
    }
    wordParam->numConsonant = c;
    wordParam->numVowels = v;
    return c;
}
/*
 * Funzione che visualizza la parola con più consonanti e il relativo significato
 */
void viewConsonatsWord() {
    printf("La parola con più consonanti è: %s (%d consonanti, %d vocali)\n", new_dictionary.consonantsWord->wordName, new_dictionary.consonantsWord->numConsonant, new_dictionary.consonantsWord->numVowels);
    printf("Il suo significato è: %s\n", new_dictionary.consonantsWord->wordSense);
}

/*
 * Funzione che effettua la ricerca binaria di una parola all'interno dell'array
 * di words (new_dictionary.words).
 */
int searchWord(char key[max_word_lenght]) {
    int median, first = 0, last = new_dictionary.effectiveSize - 1;
    while (first <= last) {
        median = (first + last) / 2;
        char newword[max_word_lenght];
        stpcpy(newword, new_dictionary.words[median].wordName);
        int condition = strcmp(key, newword);
        if (condition == 0) {
            return median;
        } else if (condition < 0)
            last = median - 1;
        else
            first = median + 1;
    }
    return -1;
}

/*
 * Funzione che visualizza tutte le parole contenute nel dizionario e il loro
 * significato
 */
void printAllDictionary() {
    int i;
    for (i = 0; i < new_dictionary.effectiveSize; i++) {
        printf("\n%s (%d consonanti, %d vocali): %s\n", new_dictionary.words[i].wordName, new_dictionary.words[i].numConsonant, new_dictionary.words[i].numVowels, new_dictionary.words[i].wordSense);
    }
}

/*
 * Funzione che permette l'inserimento di una nuova parola all'interno del dizionario.
 * Richiede di inserire la parola all'utente, effettua il check se la parola è già
 * presente nel dizionario, se non è presente permette l'inserimento del signifiacato.
 * Inserito il significato della parola, la inserisce nel dizionario in ordine. 
 * Controlla, inoltre, il numero di consonanti che contiene la parola appena inserita,
 * se ne contiene di più della word indirizzata da 'new_dictionary.consonantsWord' sostituisce
 * il puntatore contenuto da quest'ultima con il puntatore alla nuova parola.
 */
void insertNewWord() {
    Word new_word;
    printf("Che parola vuoi inserire? (max %d caratteri)\n", max_word_lenght);
    scanf("%s", new_word.wordName);
    if (searchWord(new_word.wordName) != -1) printf("Parola già presente \n");
    else {
        printf("Inserisci il significato (max %d caratteri):\n", max_word_sense_lenght);
        scanf(" %[^\n]s", new_word.wordSense);
        int c = countConsonantsWord(&new_word);
        int i = new_dictionary.effectiveSize - 1;
        while ((strcmp(new_dictionary.words[i].wordName, new_word.wordName) > 0) && i >= 0) {
            new_dictionary.words[i + 1] = new_dictionary.words[i];
            i--;
        };
        new_dictionary.words[i + 1] = new_word;
        new_dictionary.effectiveSize++;
        if(new_word->numConsonant > new_dictionary.consonantsWord->numConsonant) new_dictionary.consonantsWord = &(new_dictionary.words[i + 1]); 
        printf("Parola inserita correttamente\n");
    }
}

/*
 * Riceve in ingresso un integer che rappresenta la scelta dell'utente. In base 
 * alla scelta richiama altre funzioni.
 */
void startOption(int optionParam) {
    char word_key[max_word_lenght];
    switch (optionParam) {
        case 0:
            insertNewWord();
            viewOptions();
            break;
        case 1:
            printf("Che parola vuoi ricercare?\n");
            scanf("%s", word_key);
            int index_result = searchWord(word_key);
            if (index_result == -1) printf("Parola non trovata");
            else printf("Significato di %s (%d consonanti, %d vocali): %s\n", new_dictionary.words[index_result].wordName, new_dictionary.words[index_result].numConsonant, new_dictionary.words[index_result].numVowels, new_dictionary.words[index_result].wordSense);
            viewOptions();
            break;
        case 2:
            viewConsonatsWord();
            viewOptions();
            break;
        case 3:
            printAllDictionary();
            viewOptions();
            break;
        case 4:
            break;
        default:
            printf("Opzione non presente \n");
            viewOptions();
            break;
    }
}
/*
 * Visualizza, e permette la scelta, delle opzioni disponibili per il dizionario.
 */
void viewOptions() {
    int option;
    char buf[10] = {0};
    printf("\nSeleziona una delle seguenti opzioni: \n");
    printf("    0: Inserisci una nuova parola \n");
    printf("    1: Ricerca una parola e visualizzane il significato \n");
    printf("    2: Visualizza la parola con più consonanti \n");
    printf("    3: Visualizza tutte le parole del dizionario \n");
    printf("    4: Esci \n");
    if (scanf("%d", &option) != 1) {
        scanf("%s", &buf[0]);
        printf("Opzione non presente \n");
        viewOptions();
    } else {
        startOption(option);
    }
}



