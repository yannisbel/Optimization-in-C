#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 100
#define NOMBRE_JOURS 7 // nombre de jours dans la semaine

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

// On cherche à assigner toutes les familles à leurs choix numéro 1 (objectif : avoir le coût le plus faible)
void assign_days(struct family *fam) {
    fam->assigned_day = fam->choices[0];
}


// Calcul des scores

// Coût
int cost(struct family *head){
    int somme = 0;
    struct family *current = head;
    while (current != NULL){
        if (current -> assigned_day == current->choices[1]){
            somme += 50;
        }
        if (current -> assigned_day == current->choices[2]){
            somme += 50 + 9*(current->nb_members);
        }
        if (current -> assigned_day == current->choices[3]){
            somme += 100 + 9*(current->nb_members);
        }
        if (current -> assigned_day == current->choices[4]){
            somme += 200 + 9*(current->nb_members);
        }
        current = current -> next;
    }
    return somme;
}

// Penalités
int penalty(struct family *head){
    struct family *current = head;
    int penalty = 0;
    int i;
    int fam_jour[NOMBRE_JOURS] = {0};

    while (current != NULL) {
        if (current->assigned_day >= 0 && current->assigned_day < NOMBRE_JOURS) {
            fam_jour[current->assigned_day] += current->nb_members;
        }
        current = current->next;
    }

    for (i = 0; i < NOMBRE_JOURS; i++) {
        if (i < 6){
            penalty += ((fam_jour[i] - 125.0) / 400.0) * pow(fam_jour[i], fabs((fam_jour[i] - fam_jour[i+1]) / 50.0));
        }
        if (i == 6){
            penalty += ((fam_jour[i] - 125.0) / 400.0);
        }
    }
    return penalty;
}

int main(void) {

    struct family *head = read_csv("pb50.csv"); // On lit le fichier CSV
    struct family *current = head; // On initialise la première famille

    int fam_jour[NOMBRE_JOURS] = {0};

    // assign on assigne chaque famille à leur choix numéro 1
    while (current != NULL) {
        assign_days(current);
        if (current->assigned_day >= 0 && current->assigned_day < NOMBRE_JOURS) {
            fam_jour[current->assigned_day] += current->nb_members;
        }
        current = current->next;
    }

    current = head;
    int somme = cost(head);
    int penalties = penalty(head);

    printf("penalties : %d\n", penalties);
    printf("somme : %d\n", somme);
    printf("sum of all %d\n", somme+penalties);

    printf("\n");

    printf("=================|||=================\n");
    int a;
    for (a=0; a<NOMBRE_JOURS; a++){
                printf("Nombre personnes jour %d : %d\n", a+1, fam_jour[a]);
            }
            printf("\n");
    printf("=================|||=================");

}

