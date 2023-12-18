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
    int max_choosen;
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

    int global_score = 10000000;
    int somme_global;
    int penalties_global;

    int k;
    for (k=0; k<300; k++){
        current = head;
        int somme = 0;
        int penalties = 0;
        int indice_min = 0;
        int indice_max = 0;
        int min_loc = 500;
        int max_loc = 0;
        int i;

        for (i = 0; i < NOMBRE_JOURS; i++) {
            if (fam_jour[i] < min_loc) {
                min_loc = fam_jour[i];
                indice_min = i;
            }
            if (fam_jour[i] > max_loc) {
                max_loc = fam_jour[i];
                indice_max = i;
            }
        }

        // printf("le min est %d et le max est %d\n", indice_min, indice_max);

        int c = 1;

        for (i = 1; i <= 4; i++) {
            current = head;
            int size_family = 2;
            bool found_family = false;
            while (current != NULL) {
                if (current->choices[i] == indice_min && current->assigned_day == indice_max) {
                    if (current->nb_members > size_family){
                        size_family = current->nb_members;
                        current->max_choosen = c;
                        c += 1;
                        found_family = true;
                    }
                }
                current = current->next;
            }
            if (found_family) {
                break; // on sort complètement de la boucle for dès qu'on a trouvé une famille qui convient
            }
        }

        current = head;
        while(current != NULL){
            if (current->max_choosen == c-1){
                current->assigned_day = indice_min;
                fam_jour[indice_min] += current->nb_members;
                fam_jour[indice_max] -= current->nb_members;
                break;
            }
            current = current->next;
        }

        current = head;
        somme = cost(head);
        penalties = penalty(head);

        //int fam_jour_2[NOMBRE_JOURS] = {0};

        //while (current != NULL) {
        //    if (current->assigned_day >= 0 && current->assigned_day < NOMBRE_JOURS) {
        //        fam_jour_2[current->assigned_day] += 1;
        //    }
        //    current = current->next;
        //}

        //int x = 0;
        //for (x = 0; x < NOMBRE_JOURS; x++) {
        //    if (x < 6){
        //        penalties += ((fam_jour[x] - 125.0) / 400.0) * pow(fam_jour[x], fabs((fam_jour[x] - fam_jour[x+1]) / 50.0));
        //    }
        //    if (x == 6){
        //        penalties += ((fam_jour[x] - 125.0) / 400.0);
        //    }
        //}

        printf("penalties : %d\n", penalties);
        printf("somme : %d\n", somme);
        printf("sum of all %d\n", somme+penalties);

        printf("\n");

        if (global_score>somme+penalties){
            somme_global = somme;
            penalties_global = penalties;
            global_score = somme+penalties;
        }

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
    printf("=================|||=================");

}

