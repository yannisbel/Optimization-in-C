#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 100
#define NOMBRE_JOURS 7 // nombre de jours dans la semaine
#define FAMILIES 3

struct family {
    int nb_members;
    int choices[5];
    int assigned_day;
    struct family *next;
};

struct family *read_csv(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, fp); // skip header line

    struct family *head = NULL;
    struct family *tail = NULL;

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        struct family *f = malloc(sizeof(struct family));
        if (f == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }

        char *token = strtok(line, ";");
        f->nb_members = atoi(token);

        for (int i = 0; i < 5; i++) {
            token = strtok(NULL, ";");
            f->choices[i] = atoi(token);
        }

        f->next = NULL;

        if (head == NULL) {
            head = f;
            tail = f;
        } else {
            tail->next = f;
            tail = f;
        }
    }

    fclose(fp);
    return head;
}

int main() {
    // Initialisation des données d'entrée
    int fam_jours[NOMBRE_JOURS] = {1, 2, 1, 0, 0, 3, 2}; // Exemple de données
    int assigned_families[FAMILIES] = {-1}; // Tableau qui stocke l'indice de la famille assignée à chaque jour
    int family_index = 0; // Indice de la famille en cours d'assignation

    struct family *head = read_csv("familles.csv"); // On lit le fichier CSV
    struct family *current = head; // On initialise la première famille

    // Détermination de l'indice du jour minimal
    int min_loc = 0;
    for (int i = 1; i < DAYS; i++) {
        if (jours[i] < jours[min_loc]) {
            min_loc = i;
        }
    }

    // Boucle d'assignation des familles
    for (int i=0; i < 3; i++){  // On veut placer 3 familles
        current = head; // On initialise le current
        int max_choices[3] = {-1}  // On prends les numéros de choix des familles
        while(current != NULL) {
            int w = 0; // C'est l'indice de la famille à traiter
            for (int a; a=0; a++){
                int b = 4 - a;
                if (current->choices[b] == min_loc && family_index[w] == -1){
                    max_choices[w] = min_loc;
                }
            }
            w += 1;
            current = current->next;
        }
        int max = 0; // Indice du max
        for (int x = 0; x < 3; x++) {  // On relève le choix le plus élevé
            if (max_choices[x] > max_choices[max]) {
                max = x;
            }
        }
        if (max != -1){
            int maximum_choice = max_choices[max];
            assigned_families[max] = maximum_choice; // On stocke l'attribution de la familles
        }
    }

    int global;
    int somme;
    int penalties;

    current = head;
    for (int t = 0; t < 3; t++){
        if (assigned_families[t] != -1){
            current->assigned_day = assigned_families[t];
            fam_jours[assigned_families[t]] += current->nb_members;
        }
        somme += cost(current);
        penalties += penalty(current);
        global += somme + penalties;
        current = current->next;
    }

    printf("=================|||=================\n");
    printf("somme global : %d\n", somme_global);
    printf("penalties global : %d\n", penalties_global);
    printf("global score : %d\n", global_score);
    int a;
    for (a=0; a<NOMBRE_JOURS; a++){
        printf("Nombre personnes jour %d : %d\n", a+1, fam_jour[a]);
    }
    printf("\n");
}
