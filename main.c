#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

//POLECENIE
/* Napisz program, który umożliwia prowadzenie spisu przestępców.
Informacje o przestępcy obejmują imię, nazwisko, wiek, wzrost, wagę, miejsce ostatnio popełnionego przestępstwa
oraz status sprawy (czy przestępca został złapany, czy nie).
Przestępców można
 dodawać,
 edytować,
  usuwać (nie można usunąć przestępcy, jeżeli został złapany)
oraz filtrować wg nazwiska (całości lub początku)
 lub wieku
  oraz wyświetlić wszystkich, którzy brali udział w podanym miejscu przestępstwa.
Przestępców można przeglądać posortowanych wg nazwiska lub wg statusu. */


//STRUKTURA PRZESTĘPCY
struct Criminal {
    char firstName[256]; //imię
    char lastName[256]; //nazwisko
    int age;          // wiek
    int height;       //wzrost (w cm)
    float weight;       // waga (w kilogramach)
    char crimeLocation[256]; //Miejsce ostatnio popełnionego przestepstwa
    bool captured;  // 0 - niezłapany, 1 - złapany
    struct Criminal *next;  // Wskaźnik na następny element listy
};

struct Criminal *loadData(struct Criminal *start) {
    FILE *criminalsData = fopen("criminals.txt", "r");
    if(criminalsData == NULL) {
        printf("xdd");
        return NULL;
    }

    while(!feof(criminalsData)) {
        struct Criminal *newCriminal = malloc(sizeof(*newCriminal));
        fscanf(criminalsData, "%s%s", newCriminal->firstName, newCriminal->lastName);
        fscanf(criminalsData, "%d%d%f", &newCriminal->age, &newCriminal->height, &newCriminal->weight);
        fscanf(criminalsData, "%s%d", newCriminal->crimeLocation, &newCriminal->captured);
        newCriminal->next = NULL;

        if(start == NULL)
            start = newCriminal;
        else {
            struct Criminal *tmp = start;
            while(tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = newCriminal;
        }
    }
    fclose(criminalsData);
    return start;
}

void saveData(struct Criminal *head) { // zapis do pliku
    FILE *criminalsData = fopen("criminals.txt", "w");
    struct Criminal *current = head;
    fprintf(criminalsData, "%s %s ", current->firstName, current->lastName);
    fprintf(criminalsData, "%d %d %.2f ", current->age, current->height, current->weight);
    fprintf(criminalsData, "%s ", current->crimeLocation);
    fprintf(criminalsData, "%d", current->captured);
    current = current->next;
    while (current != NULL) {
        fprintf(criminalsData, "\n%s %s ", current->firstName, current->lastName);
        fprintf(criminalsData, "%d %d %.2f ", current->age, current->height, current->weight);
        fprintf(criminalsData, "%s ", current->crimeLocation);
        fprintf(criminalsData, "%d", current->captured);
        current = current->next;
    }
    fclose(criminalsData);
}

struct Criminal* clearList(struct Criminal *head) // czyszczenie listy z pamięci
{
    struct Criminal *tmp = NULL;
    while(head != NULL)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
    return head;//NULL
}

void showStart() { //oddzielna funkcja do nagłówka tabeli, dla uproszczenia sobie życia
    printf("+--------------+--------------+------+--------+-------+------------------------+----------+\n");
    printf("|     Imie     |   Nazwisko   | Wiek | Wzrost |  Waga |  Miejsce przestepstwa  | Zlapany |\n");
    printf("+--------------+--------------+------+--------+-------+------------------------+----------+\n");
}

void showCriminal(struct Criminal *head) { // nie wywołuje tu ShowStart() bo wykorzystuje to w kolejnej funkcji
    struct Criminal *current = head; // tu przechowujemy aktualny element listy
    printf("|%-15s|%-15s|%2d lat|%3d cm|%3.2f kg|%-24s|", current->firstName, current->lastName, current->age, current->height, current->weight, current->crimeLocation);
    printf("   %-7s|\n", (current->captured ? "Tak" : "Nie")); // no i wypisujemy sobie dane
    printf("+--------------+--------------+------+--------+-------+------------------------+----------+\n");
}
// Funkcja wyswietla informacje o wszystkich przestepcach
void showCriminals(struct Criminal *start) {
    struct Criminal *current = start;
    showStart(); // nagłówek tabeli
    while (current != NULL) {
        showCriminal(current); // no i każdego po kolei wypisujemy
        current = current->next;
    }
}

void nameCheck(char str[]) {
    str[0] = toupper(str[0]);
    for(int i=1;str[i] != '\0'; i++)
        str[i] = tolower(str[i]);
}

struct Criminal* addCriminal(struct Criminal* start) {
    struct Criminal *newCriminal = calloc(1, sizeof(*newCriminal));
    printf("Podaj imie przestepcy: ");
    scanf("%s",  newCriminal->firstName);
    nameCheck(newCriminal->firstName);
    printf("Podaj nazwisko przestepcy: ");
    scanf("%s", newCriminal->lastName);
    nameCheck(newCriminal->lastName);
    printf("Podaj wiek przestepcy: ");
    scanf("%d", &newCriminal->age);
    printf("Podaj wzrost przestepcy (w centymetrach): ");
    scanf("%d", &newCriminal->height);
    printf("Podaj wage przestepcy (w kg): ");
    scanf("%f", &newCriminal->weight);
    printf("Podaj miejsce ostatnio popelnionego przestepstwa: ");
    scanf("%s", newCriminal->crimeLocation);
    nameCheck(newCriminal->crimeLocation);
    printf("Czy przestepca zostal zlapany? (0 - nie, 1 - tak): ");
    scanf("%d", &newCriminal->captured);
    newCriminal->next = NULL;

    if(start == NULL)
        return newCriminal;
    struct Criminal *tmp = start;
    while(tmp->next!=NULL)
        tmp = tmp->next;
    tmp->next = newCriminal;
    return start;
}

struct Criminal* filterByLastName(struct Criminal *start) {
    char input[256];
    printf("Podaj nazwisko, po ktorym chcesz filtrowac przestepcow: ");
    scanf("%s", input);
    nameCheck(input);
    struct Criminal* filteredList = NULL; // to bedziemy zwracac, tylko przestepcy z podanym nazwiskiem
    struct Criminal* current = start;
    while (current != NULL) {
        if (strstr(current->lastName, input) != NULL) {
            struct Criminal* newCriminal = calloc(1, sizeof(*newCriminal));
            memcpy(newCriminal, current, sizeof(struct Criminal));
            newCriminal->next = filteredList;
            filteredList = newCriminal;
        }
        current = current->next;
    }
    return filteredList;
}
struct Criminal* filterByLastNameW(struct Criminal *start, char str[]) {
    char input[256];
    strcpy(input, str);
    struct Criminal* filteredList = NULL; // to bedziemy zwracac, tylko przestepcy z podanym nazwiskiem
    struct Criminal* current = start;
    while (current != NULL) {
        if (strstr(current->lastName, input) != NULL) {
            struct Criminal* newCriminal = calloc(1, sizeof(*newCriminal));
            memcpy(newCriminal, current, sizeof(struct Criminal));
            newCriminal->next = filteredList;
            filteredList = newCriminal;
        }
        current = current->next;
    }
    return filteredList;
}

struct Criminal* filterByAge(struct Criminal* start) {
    int minAge, maxAge;
    printf("Podaj minimalny wiek, po jakim chcesz filtrowac: ");
    scanf("%d", &minAge);
    printf("Podaj maksymalny wiek, po jakim chcesz filtrowac: ");
    scanf("%d", &maxAge);
    struct Criminal *filteredList = NULL;
    struct Criminal *current = start;
    while (current != NULL) {
        if (current->age >= minAge && current->age <= maxAge) {
            struct Criminal *newCriminal = (struct Criminal *) malloc(sizeof(struct Criminal));
            memcpy(newCriminal, current, sizeof(struct Criminal));
            newCriminal->next = filteredList;
            filteredList = newCriminal;
        }
        current = current->next;
    }
    return filteredList;
}

struct Criminal *filterByCrimeLocation(struct Criminal* head) {
    char input[256];
    printf("Podaj miejsce przestepstwa, po ktorym chcesz filtrowac przestepcow: ");
    scanf("%s", input);
    nameCheck(input);
    struct Criminal *filteredList = NULL;
    struct Criminal *current = head;
    while (current != NULL) {
        if (strcmp(current->crimeLocation, input) == 0) {
            struct Criminal *newCriminal = calloc(1, sizeof(*newCriminal));
            memcpy(newCriminal, current, sizeof(struct Criminal));
            newCriminal->next = filteredList;
            filteredList = newCriminal;
        }
        current = current->next;
    }
    return filteredList;
}

struct Criminal* mergeByLastName(struct Criminal *first, struct Criminal *second) {
    struct Criminal *result = NULL;
    if (first == NULL)
        return second;
    if (second == NULL)
        return first;
    if (strcmp(first->lastName, second->lastName) <= 0) {
        result = first;
        result->next = mergeByLastName(first->next, second);
    } else {
        result = second;
        result->next = mergeByLastName(first, second->next);
    }
    return result;
}

struct Criminal* split(struct Criminal *start) {
    struct Criminal *fast = start, *slow = start;
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next; // wolny, bo przeskakuje tylko po razie...
        fast = fast->next->next; // ...a tutaj o 2 pozycje!
    }
    struct Criminal *tmp = slow->next;
    slow->next = NULL;
    return tmp;
}

struct Criminal* mergeSortByLastName(struct Criminal* start) { // najlepszy sposob bez koniecznosci zapamietywania poprzedniego elementu
    if (start == NULL || start->next == NULL)
        return start;
    struct Criminal *second = split(start);
    start = mergeSortByLastName(start);
    second = mergeSortByLastName(second);
    return mergeByLastName(start, second);
}

struct Criminal* mergeByCaptured(struct Criminal *first, struct Criminal *second) {
    struct Criminal *result = NULL;
    if (first == NULL)
        return second;
    if (second == NULL)
        return first;
    if (first->captured >= second->captured) {
        result = first;
        result->next = mergeByCaptured(first->next, second);
    } else {
        result = second;
        result->next = mergeByCaptured(first, second->next);
    }
    return result;
}

struct Criminal* mergeSortByCaptured(struct Criminal* start) {
    if(start == NULL || start->next == NULL)
        return start;
    struct Criminal* second = split(start);
    start = mergeSortByCaptured(start);
    second = mergeSortByCaptured(second);
    return mergeByCaptured(start, second);
}
void editCode(struct Criminal* criminal, char lastName[]) {
    int choice =0;
    do {
        printf("Wybierz co chcesz edytowac w przestepcy o nazwisku %s:\n"
               "1. Imie\n"
               "2. Wiek\n"
               "3. Wzrost\n"
               "4. Waga\n"
               "5. Miejsce ostatnio popelnionego przestepstwa\n"
               "6. Status sprawy\n"
               "7. Nie chce juz nic edytowac.\n"
               "Moj wybor: ", lastName);
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
            {
                printf("Podaj nowe imie przestepcy o nazwisku %s: ", lastName);
                scanf("%s", criminal->lastName);
                nameCheck(criminal->lastName);
                printf("Pomyslnie zmieniono imie przestepcy o nazwisku %s!\n", lastName);
                break;
            }
            case 2:
            {
                printf("Podaj nowy wiek przestepcy o nazwisku %s: ", lastName);
                scanf("%d", &criminal->age);
                printf("Pomyslnie zmieniono wiek przestepcy o nazwisku %s!\n", lastName);
                break;
            }
            case 3:
            {
                printf("Podaj nowy wzrost wieznia o nazwisku %s: ", lastName);
                scanf("%d", &criminal->height);
                break;
            }
            case 4:
            {
                printf("Podaj nowa wage wieznia o nazwisku %s: ", lastName);
                scanf("%f", &criminal->weight);
                break;
            }
            case 5:
            {
                printf("Podaj nowe miejsce ostatnio popelnionego przestepstwa wieznia o nazwisku %s: ", lastName);
                scanf("%s", criminal->crimeLocation);
                nameCheck(criminal->crimeLocation);
                break;
            }
            case 6:
            {
                printf("Podaj nowy status sprawy wieznia o nazwisku %s (1 - Zlapany 0 - Niezlapany): ", lastName);
                scanf("%d", &criminal->captured);
                break;
            }
            case 7:
            {
                break;
            }
            default:
            {
                printf("Dokonaj poprawnego wyboru!");
                scanf("%d", &choice);
                break;
            }
        }
    }while(choice != 7);
}

struct Criminal* editCriminal(struct Criminal* start) {
    if(start == NULL) {
        printf("Lista jest pusta! nie ma czego w niej edytowac!\n");
        return start;
    }
    char input[256];
    int choice = 0;
    printf("Podaj nazwisko przestepcy, ktorego chcesz edytowac: ");
    scanf("%s", input);
    nameCheck(input);
    int count = 0;
    struct Criminal *tmp = start;
    while(tmp != NULL) {
        if(strcmp(tmp->lastName, input) == 0) {
            count++;
        }
        tmp = tmp->next;
    }
    if(count == 1) {
        tmp = start;
        while(tmp != NULL && strcmp(tmp->lastName, input) != 0)
            tmp = tmp->next;
        editCode(tmp, input);
    } else if(count > 1) {
        printf("Istnieje kilku przestepcow o takim nazwisku, prosze podac dodatkowe kryterium:\n");
        showCriminals(filterByLastNameW(start, input));
        printf("1. Imie\n");
        printf("2. Wiek\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1: {
                char firstName[256];
                printf("Podaj imie przestepcy: ");
                scanf("%s", firstName);
                nameCheck(firstName);
                tmp = start;
                while(tmp != NULL && (strcmp(tmp->lastName, input) != 0 || strcmp(tmp->firstName, firstName) != 0)) {
                    tmp = tmp->next;
                }
                if(tmp == NULL) {
                    printf("Nie znaleziono przestepcy o podanym nazwisku i imieniu\n");
                    return start;
                }
                editCode(tmp, input);
                break;
            }
            case 2: {
                int age;
                printf("Podaj wiek przestepcy: ");
                scanf("%d", &age);
                tmp = start;
                while(tmp != NULL && (strcmp(tmp->lastName, input) != 0 || tmp->age != age)) {
                    tmp = tmp->next;
                }
                if(tmp == NULL) {
                    printf("Nie znaleziono przestepcy o podanym nazwisku i wieku\n");
                    return start;
                }
                editCode(tmp, input);
                break;
            }
            default:
            {
                printf("Nieprawidlowy wybor, spróbuj ponownie\n"
                       "Moj wybor: ");
                scanf("%d", &choice);
                break;
            }
        }
    } else {
        printf("Nie znaleziono przestepcy o nazwisku %s w bazie!\n", input);
        return start;
    }
}

struct Criminal* deleteCriminal(struct Criminal* start) {
    if(start == NULL) {
        printf("Lista jest pusta! nie ma czego w niej usunac!\n");
        return start;
    }
    char input[256];
    struct Criminal* tmp = NULL;
    int count = 0;
    printf("Podaj nazwisko przestepcy, ktorego chcesz usunac: ");
    scanf("%s", input);
    nameCheck(input);

    // sprawdzenie ilości przestępców o podanym nazwisku
    tmp = start;
    while(tmp != NULL) {
        if(strcmp(tmp->lastName, input) == 0) {
            count++;
        }
        tmp = tmp->next;
    }

    if(count == 0) {
        printf("Nie ma przestepcy o takim nazwisku!\n");
        return start;
    } else if(count == 1) {
        if(strcmp(start->lastName, input) == 0) {
            if(start->captured == true) {
                printf("Nie mozna usunac zlapanego przestepcy!\n");
                return start;
            }
            else {
                tmp = start->next;
                free(start);
                return tmp;
            }
        }
        else {
            tmp = start;
            while(tmp->next != NULL && strcmp((tmp->next)->lastName, input) != 0)
                tmp = tmp->next;
            if((tmp->next)->captured == true) {
                printf("Nie mozna usunac zlapanego przestepcy!\n");
                return start;
            }
            else {
                struct Criminal* deleted = tmp->next;
                tmp->next = deleted->next;
                free(deleted);
            }
            return start;
        }
    } else {
        system("cls");
        showCriminals(filterByLastNameW(start, input));
        printf("\nIstnieje kilku przestepcow o takim nazwisku, prosze podac dodatkowe kryterium:\n");
        printf("1. Imie\n");
        printf("2. Wiek\n"
               "3. Nie chce usuwac zadnego przestepcy\n");
        int choice;
        scanf("%d", &choice);
        switch(choice) {
            case 1: {
                char firstName[256];
                printf("Podaj imie przestepcy: ");
                scanf("%s", firstName);
                nameCheck(firstName);
                tmp = start;
                while(tmp->next != NULL && (strcmp(tmp->next->lastName, input) != 0 || strcmp(tmp->next->firstName, firstName) != 0)) {
                    tmp = tmp->next;
                }
                if(tmp->next == NULL) {
                    printf("Nie znaleziono przestepcy o podanym nazwisku i imieniu\n");
                    return start;
                } else {
                    if(tmp->next->captured == true) {
                        printf("Nie mozna usunac zlapanego przestepcy!\n");
                        return start;
                    } else {
                        struct Criminal* deleted = tmp->next;
                        tmp->next = deleted->next;
                        free(deleted);
                        return start;
                    }
                }
            }
            case 2: {
                int age;
                printf("Podaj wiek przestepcy: ");
                scanf("%d", &age);
                tmp = start;
                while(tmp->next != NULL && (strcmp(tmp->next->lastName, input) != 0 || tmp->next->age != age)) {
                    tmp = tmp->next;
                }
                if(tmp->next == NULL) {
                    printf("Nie znaleziono przestepcy o podanym nazwisku i wieku\n");
                    return start;
                } else {
                    if(tmp->next->captured == true) {
                        printf("Nie mozna usunac zlapanego przestepcy!\n");
                        return start;
                    } else {
                        struct Criminal* deleted = tmp->next;
                        tmp->next = deleted->next;
                        free(deleted);
                        return start;
                    }
                }
            }
            case 3:
            {
                return start;
            }
            default:
            {
                printf("Nieprawidlowy wybor, spróbuj ponownie\n");
                scanf("%d", &choice);
                break;
            }

        }
        return start;
    }
}


void wait() {
    printf("\nNacisnij dowolny przycisk, aby kontynuowac...\n");
    getch();
}

int main() {
    struct Criminal *criminals = NULL;
    criminals = loadData(criminals); //wczytywanie danych z pliku
    int userChoice = 0;
    do {
        system("cls");
        printf("MENU\n"
               "Wpisz odpowiednia cyfre, w zaleznosci od tego co chcesz zrobic:\n"
               "1. Zobacz wszystkich przestepcow\n"
               "2. Dodaj nowego przestepce\n"
               "3. Edytuj przestepece o wybranym nazwisku\n"
               "4. Usun przestepce o wybranym nazwisku\n"
               "5. Filtruj przestepcow wedlug nazwiska\n"
               "6. Filtruj przestepcow wedlug wieku\n"
               "7. Wyswietl wszytkich przestepcow, ktorzy brali udzial w przestepstwie w podanym miejscu\n"
               "8. Wyswietl przestepcow posortowanych wedlug nazwiska\n"
               "9. Wyswietl przestepcow posortowanych wedlug statusu sprawy\n"
               "10. Zakoncz program.\n"
               "Moj wybor: ");
        scanf("%d", &userChoice);
        switch(userChoice) {
            case 1:
            {
                showCriminals(criminals);
                wait();
                break;
            }
            case 2:
            {
                criminals = addCriminal(criminals);
                wait();
                break;
            }
            case 3:
            {
                criminals = editCriminal(criminals);
                wait();
                break;
            }
            case 4:
            {
                criminals = deleteCriminal(criminals);
                wait();
                break;
            }
            case 5:
            {
                showCriminals(filterByLastName(criminals));
                wait();
                break;
            }
            case 6:
            {
                showCriminals(filterByAge(criminals));
                wait();
                break;
            }
            case 7:
            {
                showCriminals((filterByCrimeLocation(criminals)));
                wait();
                break;
            }
            case 8:
            {
                criminals = mergeSortByLastName(criminals);
                showCriminals(criminals);
                wait();
                break;
            }
            case 9:
            {
                criminals = mergeSortByCaptured(criminals);
                showCriminals(criminals);
                wait();
                break;
            }
            case 10:
            {
                saveData(criminals);
                criminals = clearList(criminals);
                printf("Zapisano zmiany i wyczyszczono pamiec!\n");
                wait();
                break;
            }
            default:
            {
                printf("Dokonaj prawidlowego wyboru!\n");
                scanf("%d", &userChoice);
                break;
            }
        }
    }while(userChoice != 10);
    return 0;
}