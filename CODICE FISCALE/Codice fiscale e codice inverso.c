

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100
#define MAX_CODE_LENGTH 10
#define MAX_NAME_LENGTH 50
#define CF_LENGTH 16
#define CODE_LENGTH 4

char CF[CF_LENGTH];

int converti_mese(char meseChar) {
    int numero_mese;

    switch (toupper(meseChar)) {
        case 'A': numero_mese = 1; break;
        case 'B': numero_mese = 2; break;
        case 'C': numero_mese = 3; break;
        case 'D': numero_mese = 4; break;
        case 'E': numero_mese = 5; break;
        case 'H': numero_mese = 6; break;
        case 'L': numero_mese = 7; break;
        case 'M': numero_mese = 8; break;
        case 'P': numero_mese = 9; break;
        case 'R': numero_mese = 10; break;
        case 'S': numero_mese = 11; break;
        case 'T': numero_mese = 12; break;
        default:
            printf("Mese non valido\n");
            return -1; 
    }

    return numero_mese;
}

void estraiCognome(char cognome[]) {
    int lunghezza = strlen(cognome);
    char ris[4];
    int j = 0;

    for (int i = 0; i < lunghezza && j < 3; i++) {
        if (strchr("aeiou", tolower(cognome[i])) == NULL) {
            ris[j] = toupper(cognome[i]);
            j++;
        }
    }

    if (j < 3) {
        for (int i = 0; i < lunghezza && j < 3; i++) {
            if (strchr("aeiou", tolower(cognome[i])) != NULL) {
                ris[j] = toupper(cognome[i]);
                j++;
            }
        }
    }

    while (j < 3) {
        ris[j] = 'X';
        j++;
    }

    ris[3] = '\0';
    strcpy(CF, ris);
}

void estraiNome(char nome[]) {
    int lunghezza = strlen(nome);
    char consonanti[4] = "";
    char vocali[4] = "";
    int k = 0, v = 0;

    for (int i = 0; i < lunghezza; i++) {
        if (strchr("aeiou", tolower(nome[i])) == NULL) {
            consonanti[k] = toupper(nome[i]);
            k++;
        }
    }
    for (int i = 0; i < lunghezza; i++) {
        if (strchr("aeiou", tolower(nome[i])) != NULL) {
            vocali[v] = toupper(nome[i]);
            v++;
        }
    }

    if (k >= 4) {
        CF[3] = consonanti[0];
        CF[4] = consonanti[2];
        CF[5] = consonanti[3];
    } else if (k == 3) {
        CF[3] = consonanti[0];
        CF[4] = consonanti[1];
        CF[5] = consonanti[2];
    } else if (k == 2) {
        CF[3] = consonanti[0];
        CF[4] = consonanti[1];
        CF[5] = vocali[0];
    } else if (k == 1 && v == 2) {
        CF[3] = consonanti[0];
        CF[4] = vocali[0];
        CF[5] = vocali[1];
    } else if (k == 1 && v == 1) {
        CF[3] = consonanti[0];
        CF[4] = vocali[0];
        CF[5] = 'X';
    } else if (v == 2) {
        CF[4] = vocali[0];
        CF[5] = vocali[1];
        CF[6] = 'X';
    } else {
        printf("Numero non valido");
    }
}

void calcolaCodiceAlfanumerico() {
    int anno, mese, giorno;
    char meseChar, sesso;

    // Inserimento del sesso
    printf("Inserire il sesso inserendo m per maschio e f per femmina: ");
    scanf(" %c", &sesso);
    sesso = toupper(sesso);

    // Inserimento dell'anno di nascita
    printf("Inserire l'anno di nascita (4 cifre): ");
    scanf("%d", &anno);

    // Calcolo della cifra delle decine e delle unità dell'anno
    int decine = (anno / 10) % 10;
    int unita = anno % 10;

    // Conversione del mese di nascita in carattere alfabetico
    printf("Inserire il numero del mese di nascita (1-12): ");
    scanf("%d", &mese);

    switch(mese) {
        case 1: meseChar = 'A'; break;
        case 2: meseChar = 'B'; break;
        case 3: meseChar = 'C'; break;
        case 4: meseChar = 'D'; break;
        case 5: meseChar = 'E'; break;
        case 6: meseChar = 'H'; break;
        case 7: meseChar = 'L'; break;
        case 8: meseChar = 'M'; break;
        case 9: meseChar = 'P'; break;
        case 10: meseChar = 'R'; break;
        case 11: meseChar = 'S'; break;
        case 12: meseChar = 'T'; break;
        default: printf("Mese non valido"); return;
    }

    // Inserimento del giorno di nascita
    printf("Inserire il giorno di nascita (1-31): ");
    scanf("%d", &giorno);

    // Calcolo del giorno di nascita in base al sesso
    if (sesso == 'F') {
        giorno += 40;
    }

    int decine2 = (giorno / 10) % 10;
    int unita2 = giorno % 10;

    // Inserimento del codice alfanumerico nell'array CF
    CF[6] = decine + '0';
    CF[7] = unita + '0';
    CF[8] = meseChar;
    CF[9] = decine2 + '0';  
    CF[10] = unita2 + '0'; 
}

char* comune(const char* nomeFile, const char* nomeRicerca, char* codiceTrovato) {
    FILE *file;
    char linea[MAX_LINE_LENGTH];
    static char codice[MAX_CODE_LENGTH]; // La variabile è statica per essere restituita dalla funzione

    // Apertura del file in modalità di lettura
    file = fopen(nomeFile, "r");
    if (file == NULL) {
        printf("Impossibile aprire il file %s.\n", nomeFile);
        return NULL;
    }

    // Lettura e confronto delle righe del file
    while (fgets(linea, sizeof(linea), file)) {
        char nomeCorrente[MAX_NAME_LENGTH];
        char codiceCorrente[MAX_CODE_LENGTH];
        sscanf(linea, "%s %s", codiceCorrente, nomeCorrente);

        // Rimuove la parentesi finale dalla provincia, se presente
        char* parentesiAperta = strchr(nomeCorrente, '(');
        if (parentesiAperta != NULL) {
            *parentesiAperta = '\0';
        }

        // Confronto case-insensitive dei nomi
        if (strcasecmp(nomeCorrente, nomeRicerca) == 0) {
            strcpy(codiceTrovato, codiceCorrente);
            fclose(file);
            return codiceTrovato;
        }
    }

    fclose(file);
    return NULL; // Se il nome non è stato trovato
}

char calcolaCarattereAlfabeticoControllo() {
    char nPari[] = "0123456789xxxxxxxxxxxxxxxx";
    char letterePari[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char nDispari[] = "10xxx2x3x4xxx5x6x7x8x9xxx";
    char lettereDispari[] = "BAKPLCQDREVOSFTGUHMINJWZYX";
    int somma = 0;

    for (int i = 0; i < 16; i++) {
        if (i % 2 != 0) {
            for (int j = 0; j < strlen(letterePari); j++) {
                if (toupper(CF[i]) == letterePari[j] || CF[i] == nPari[j]) {
                    somma += j;
                    break;
                }
            }
        } else {
            for (int j = 0; j < strlen(letterePari); j++) {
                if (toupper(CF[i]) == lettereDispari[j] || CF[i] == nDispari[j]) {
                    somma += j;
                    break;
                }
            }
        }
    }
    somma = somma % 26;
    return letterePari[somma];
}
int main() {
    int scelta;

    do {
        // Schermata iniziale con le opzioni
        printf("\n1. Calcola Codice Fiscale\n");
        printf("2. Calcola Dati Anagrafici da Codice Fiscale\n");
        printf("0. Esci\n");
        printf("Seleziona un'opzione: ");
        scanf("%d", &scelta);

        switch (scelta) {
            case 1:
                char cognome[MAX_NAME_LENGTH];
    char nome[MAX_NAME_LENGTH];
    char filename[] = "comuni.txt"; // Nome del file da leggere
    char searchName[MAX_NAME_LENGTH];

    printf("Inserisci un cognome(tutto attaccato senza spazzi): ");
    scanf("%s", cognome);

    estraiCognome(cognome);

    printf("Inserisci un nome: ");
    scanf("%s", nome);

    estraiNome(nome);

    calcolaCodiceAlfanumerico();

    // Input del nome da cercare
    printf("Inserisci il comune di nascita: ");
    scanf("%s", searchName);

    // Chiamata alla funzione comune per ottenere il codice alfanumerico
    char foundCode[MAX_CODE_LENGTH];
    char* code = comune(filename, searchName, foundCode);
    if (code != NULL) {
        
        // Copia il codice trovato nella posizione corretta nell'array CF
        strncpy(CF + 11, code, 4);
    } else {
        printf("Nessun risultato trovato per %s.\n", searchName);
    }

    CF[15] = calcolaCarattereAlfabeticoControllo();

    printf("Il tuo codice fiscale è: %s\n", CF);

                break;
            case 2:
                char codice_fiscale[17];
    printf("Inserisci il codice fiscale (16 caratteri): ");
    scanf("%s", codice_fiscale);

    char giorno[3];
    strncpy(giorno, codice_fiscale + 9, 2);
    giorno[2] = '\0';

    char meseChar = codice_fiscale[8];
    int numero_mese = converti_mese(meseChar);

    if (numero_mese != -1) {
        char anno[3];
        strncpy(anno, codice_fiscale + 6, 2);
        anno[2] = '\0';

        char sesso;
        int giorno_int = atoi(giorno);
        if (giorno_int > 40) {
            sesso = 'F';
            giorno_int = giorno_int - 40;
        } else {
            sesso = 'M';
        }

        printf("Data di nascita: %d/%d/", giorno_int, numero_mese);

        int anno_int = atoi(anno);
        if (anno_int > 24) {
            printf("19%s\n", anno);
        } else {
            printf("20%s\n", anno);
        }

        printf("Sesso: %c\n", sesso);

        // Estrai il codice alfanumerico del comune dal codice fiscale (posizioni 11-14)
        char codice_comune[CODE_LENGTH + 1];
        strncpy(codice_comune, codice_fiscale + 11, CODE_LENGTH);
        codice_comune[CODE_LENGTH] = '\0';

        // Apri il file contenente i codici dei comuni
        FILE *file;
        file = fopen("comuni.txt", "r");
        if (file == NULL) {
            printf("Impossibile aprire il file comuni.txt.\n");
            return 1;
        }

        char riga[MAX_LINE_LENGTH];
        char codice_file[CODE_LENGTH + 1];

        char luogo_nascita[MAX_LINE_LENGTH]; // Definisci una variabile per il luogo di nascita
        int trovato = 0; // Flag per indicare se il codice è stato trovato nel file

        // Scansiona il file riga per riga
        while (fgets(riga, MAX_LINE_LENGTH, file) != NULL) {
            strncpy(codice_file, riga, CODE_LENGTH);
            codice_file[CODE_LENGTH] = '\0';

            // Confronta il codice estratto dal CF con i codici nel file
            if (strcmp(codice_file, codice_comune) == 0) {
                char *nome_comune_ptr = riga + CODE_LENGTH + 1;
                strtok(nome_comune_ptr, "\n"); // Rimuovi il carattere newline (se presente)
                strcpy(luogo_nascita, nome_comune_ptr);
                trovato = 1;
                break;
            }
        }

        fclose(file);

        // Se il codice è stato trovato, stampa il luogo di nascita
        if (trovato) {
            printf("Luogo di nascita: %s\n", luogo_nascita);
        } else {
            printf("Codice comune non trovato nel file.\n");
        }
    }
                break;
            case 0:
                printf("Uscita dal programma.\n");
                break;
            default:
                printf("Opzione non valida. Riprova.\n");
                break;
        }

    } while (scelta != 0);

    return 0;
}
