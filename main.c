// Projekt c.1 -- Matej Delinčák, 5.11.2019 14:00
//posledna uprava 7.11.2019 - skratenie programu

#define _CRT_SECURE_NO_WARNINGS

#define FILE_NAME "autobazar.txt" //meno subora
#define NO_CLOSE "Neotvoreny subor\n" //subor sa nezavrel
#define NO_ARR "Pole nie je vytvorene\n" //pole nie je vytvorene
#define NO_SAME_ERROR "Zhoda nebola zistena\n"
#define INPUT_SIZE 50 //maximalna dlzka retazca na vstupe
#define ITEM_NUM 5	//pocet poloziek v zazname
#define SPZ_SIZE 7	//dlazka SPZ-tky
#define REWARD_TIME 9999	//index pre porovnanie ci sa ma vyplatit odmena
#define IND_REW_NEW 0.023	//index pre odmenu za nove auto
#define IND_REW_OLD 0.051	//index pre odmenu za stare auto
#define PARTSIZE_SPZ_LET 2	//dlzka retazca s pismenami v SPZ-tke
#define PARTSIZE_SPZ_NUMS 3	//dlzka retazca s pismenami v SPZ-tke
#define COUNT_OF_LETS ('Z' - 'A' + 1) //pocet pismen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vWriteData(FILE** paFR) // funkcia v - nacita subor, vypise jeho obsah a vrati pointer na subor cez parameter
{
    //inicializacia
    char riadok[INPUT_SIZE + 2], * zac;
    int por_zaz = 1;

    if (*paFR != NULL) { fclose(*paFR); }//ak je uz nejaky subor otvoreny tak ho zavrie
    if ((*paFR = fopen(FILE_NAME, "r")) == NULL) // testovanie otvorenia
    {
        printf(NO_CLOSE);
    }

    while ((zac = fgets(riadok, INPUT_SIZE + 2, *paFR)) != NULL) //pokial nie je koniec suboru, nacitava z neho riadky
    {
        //na zaklade poctu aktualne nacitanych riadkov, vypise prislusny zaciatok
        switch (por_zaz)
        {
            case 1:	printf("meno priezvisko: "); por_zaz++; break;
            case 2:	printf("SPZ: "); por_zaz++; break;
            case 3:	printf("typ auta: "); por_zaz++; break;
            case 4:	printf("cena: "); por_zaz++; break;
            case 5:	printf("datum: "); por_zaz++; break;
            case 6:	por_zaz++; break;
            default:
                break;
        }
        printf("%s", riadok);
        if (por_zaz > ITEM_NUM + 1) { por_zaz = 1; } //resetovanie pre vypis zaznamu
    }
}

void oWriteRewards(FILE** paFr) // funkcia o - nastavi sa na zaciatok suboru, najde a vypise zamestnancov, ktori maju dostat odmeny
{
    if (*paFr != NULL) {
        //inicializacia
        int act_val, rec_val, old;
        double price;
        char name[INPUT_SIZE + 2], spz[SPZ_SIZE + 2], * zac;

        rewind(*paFr); //resetovanie na zaicatok suboru
        scanf("%d", &act_val); //nacita aktualny datum zo vstupu

        while ((zac = fgets(name, INPUT_SIZE + 2, *paFr)) != NULL) //pokial nie je koniec suboru
        {
            name[strlen(name) - 1] = 0;	//do new_name da nacitany string ale bez konca riadku
            fscanf(*paFr, "%s", spz);
            fscanf(*paFr, "%d", &old);
            fscanf(*paFr, "%lf", &price);
            fscanf(*paFr, "%d\n", &rec_val); //nacita datum, kedy dany clvoek nastupil do roboty

            if (act_val - rec_val > REWARD_TIME)//ak je rozdiel rokov vacsi ako 9999, tak je v robote viac ako jeden rok(nezahrnuje priestupne roky)
            {
                if (old)
                    printf("%s %s %.2lf\n", name, spz, price * IND_REW_NEW); //vypise hodnotu odmeny ked je auto stare
                else
                    printf("%s %s %.2lf\n", name, spz, price * IND_REW_OLD); //vypise hodnotu odmeny ked je auto nove
            }
        }
    }
}

void nMakeSPZString(FILE** paFr, char** paSPZArr, int* paCount) // funkcia n - spocita kolko je zaznamov a vrati smernik, ktory ukazuje na dynamicke pole
{
    if (*paFr != NULL) {
        if (*paSPZArr != NULL) free(*paSPZArr); //dealokacia smernika na dynamicke pole
        //inicializacia
        char name[INPUT_SIZE + 2], * zac;
        int pos = 0;

        *paCount = 0;
        rewind(*paFr);
        while ((zac = fgets(name, INPUT_SIZE + 2, *paFr)) != NULL)// zisti pocet zaznamov v subore
        {
            for (int i = 0; i < ITEM_NUM; i++) { fgets(name, INPUT_SIZE + 2, *paFr); } // prejde cely subor a zakazdym zvysi pocet
            (*paCount)++;
        }
        *paSPZArr = (char*)calloc(SPZ_SIZE * (*paCount), sizeof(char)); //alokovanie miesta pre pole
        rewind(*paFr);
        while ((zac = fgets(name, INPUT_SIZE + 2, *paFr)) != NULL)
        {
            fgets(name, SPZ_SIZE + 1, *paFr); //nacita do name spz zo suboru
            for (unsigned int i = 0; i < strlen(name); i++)
            {
                *(*paSPZArr + pos) = name[i]; //pomocou smernikovej aritmetiky naplni postupne dynamicke pole
                pos++;
            }
            for (int i = 0; i < 5; i++) { fgets(name, INPUT_SIZE + 2, *paFr); }
        }
    }
}

void sWriteSPZS(int paArrSize, char** paSPZArr) // funkcia s - vypise SPZ podla zadania
{
    if (*paSPZArr != NULL) {
        //inicializacia
        char oneSPZ[SPZ_SIZE + 2], town[PARTSIZE_SPZ_LET + 2], let[PARTSIZE_SPZ_LET + 2], nums[PARTSIZE_SPZ_NUMS + 2];

        for (int i = 0; i < paArrSize * SPZ_SIZE; i++)
        {
            oneSPZ[i % SPZ_SIZE] = *(*paSPZArr + i); //rozbije postupne dynamicke pole na jednotlive SPZ-tky
            if ((i + 1) % SPZ_SIZE == 0)
            {
                sscanf(oneSPZ, "%2s%3s%2s", town, nums, let);	//rozbije danu SPZ na predpisany vystup
                printf("%s %s %s\n", town, nums, let);
            }
        }
    }
    else
        printf(NO_ARR);
}

void mMostChar(int paArrSize, char** paSPZArr) // funkcia m - vypise najpocetnejsi znak v retazci SPZ
{
    if (*paSPZArr != NULL) {
        //inicializacia a vynulovanie pola
        int characters[COUNT_OF_LETS] = { 0 }, max = 0;

        for (int i = 0; i < paArrSize * SPZ_SIZE; i++) //naplnenie pola
        {
            if (*(*paSPZArr + i) >= 'A') { characters[*(*paSPZArr + i) - 'A']++; }
        }

        for (int i = 0; i < COUNT_OF_LETS; i++)	//najdenie maxima v poli
        {
            if (characters[i] > characters[max]) { max = i; }
        }

        printf("%c %d\n", max + 'A', characters[max]);//vypisanie maxim s hodnotou
    }
    else
        printf(NO_ARR);
}

void pIsPalindrom(int paArrSize, char** paSPZArr)  // funkcia p - zisti ci je dana PSZ palindrom
{
    if (*paSPZArr != NULL) {
        //inicializacia
        char oneSPZ[SPZ_SIZE + 2], town[PARTSIZE_SPZ_LET + 2];
        int isP;

        for (int i = 0; i < paArrSize * SPZ_SIZE; i++)
        {
            oneSPZ[i % SPZ_SIZE] = *(*paSPZArr + i); //rozbitie dynamickeho pola na jednotlive SPZ-tky
            if ((i + 1) % SPZ_SIZE == 0)
            {
                isP = 1;
                for (int j = 0; j <= SPZ_SIZE / 2; j++) //zistenie ci je palindrom
                {
                    if (oneSPZ[j] != oneSPZ[(SPZ_SIZE - 1) - j]) { isP = 0; break; }
                }
                if (isP) //ak je palindrom vypise iba okres danej SPZ-tky
                {
                    sscanf(oneSPZ, "%2s", town);
                    printf("%s\n", town);
                }
            }
        }
    }
    else
        printf(NO_ARR);
}

void zMostSPZ(int paArrSize, char** paSPZArr)  // funkcia z - najde najcastejsii okres v SPZ
{
    if (*paSPZArr != NULL) {
        //inicializacia
        char town[PARTSIZE_SPZ_LET + 2];
        int countOfSPZ[COUNT_OF_LETS * COUNT_OF_LETS] = { 0 }, max = 0;

        for (int i = 0; i < paArrSize * SPZ_SIZE; i += 7)	//naplnenie pola - imaginarne dvojrozmerne pole som preindexoval na jednorozmerne
        {
            town[0] = *(*paSPZArr + i);
            town[1] = *(*paSPZArr + i + 1);
            countOfSPZ[(town[0] - 'A') * COUNT_OF_LETS + (town[1] - 'A')]++;
        }

        for (int i = 0; i < COUNT_OF_LETS * COUNT_OF_LETS; i++) // najdenie maxima
        {
            if (countOfSPZ[i] >= countOfSPZ[max]) { max = i; }
        }
        for (int i = 0; i < COUNT_OF_LETS * COUNT_OF_LETS; i++) // vypisanie maxim
        {
            if (countOfSPZ[i] == countOfSPZ[max])
            {
                printf("%c%c %d\n", (i / COUNT_OF_LETS) + 'A', (i % COUNT_OF_LETS) + 'A', countOfSPZ[i]);
            }
        }
    }
}

void kClose(FILE** paFR, char** paSPZArr) //funkcia k - zavrie subor a dealokuje pole SPZtiek
{
    if (*paSPZArr != NULL) { free(*paSPZArr); }//uvolnenie smernika
    if (*paFR != NULL && fclose(*paFR) == EOF) // testovanie zatvorenia
    {
        printf("Nezatvoreny subor\n");
        return;
    }
}

void xdelSPZ(int* paArrSize, char** paSPZArr) //funkcia x - odstrani vsetky SPZ s pociatocnym pismenom rovnakym ako zadane
{
    if (*paArrSize != 0)
    {
        char* hStr;
        int j = 0, isSame = 0;
        char sSym;

        hStr = calloc((unsigned int) *paSPZArr, (*paArrSize) * ITEM_NUM);
        strcpy(hStr, *paSPZArr);

        scanf(" %c", &sSym);
        while(j < *paArrSize * SPZ_SIZE)
        {

            if (*(hStr + j) == sSym)
            {
                isSame = 1;
                while (j < (*paArrSize) * SPZ_SIZE)
                {
                    *(hStr + j) = *(hStr + (j++) + SPZ_SIZE);
                }
                (*paArrSize)--;
                j = 0;
            }
            else
            {
                j += 7;
            }

        }

        *(hStr + *paArrSize * SPZ_SIZE) = 0;
        if (*paSPZArr != NULL) 	free(*paSPZArr);
        if (*paArrSize != 0)
        {
            *paSPZArr = calloc((unsigned int) *paSPZArr, (*paArrSize) * ITEM_NUM);
            strcpy(*paSPZArr, hStr);
        }
        else *paSPZArr = NULL;

        free(hStr);
        if (!isSame)
        {
            printf(NO_SAME_ERROR);
        }
    }
}

void hTableOfLet(int paArrSize, char** paSPZArr)  // funkcia h - vypise histogram pismen
{
    if (*paSPZArr != NULL) {
        //inicializacia a vynulovanie pola
        int characters[COUNT_OF_LETS] = { 0 }, max = 0;

        for (int i = 0; i < paArrSize * SPZ_SIZE; i++) //naplnenie pola
        {
            if (i % SPZ_SIZE == 5 || i % SPZ_SIZE == 6)
            {
                if (*(*paSPZArr + i) >= 'A') { characters[*(*paSPZArr + i) - 'A']++; }
            }
        }

        for (int i = 0; i < COUNT_OF_LETS; i++)	//vypis hlavicky a hodnot
        {
            if (characters[i] != 0)
            {
                printf("%c:%d\n", i + 'A', characters[i]);
            }
        }
    }
    else
        printf(NO_ARR);
}

void cAddSPZ(int paArrSize, char** paSPZArr)
{
    int shift, j;
    char* hStr;
    int count = 0;
    scanf("%d", &shift);

    hStr = malloc(paArrSize * 4);
    for (int i = 0; i < paArrSize * SPZ_SIZE; i++)
    {
        if (i % SPZ_SIZE == 0 || i % SPZ_SIZE == 1 || i % SPZ_SIZE == 5 || i % SPZ_SIZE == 6)
        {
            *(hStr + count) = (((*(*paSPZArr + i) + shift) - 'A') % 26) + 'A';
            count++;
        }
    }
    j = 0;

    //pridany aj vypis lebo moja funkcia "s" by nezvladla zmenu povodneho pola
    for (int i = 0; i < paArrSize * SPZ_SIZE; i++)
    {
        if (i % SPZ_SIZE == 2 || i % SPZ_SIZE == 5)
        {
            printf(" ");
        }
        printf("%c", *(*paSPZArr + i));
        if (i % SPZ_SIZE == 6)
        {
            printf("\n");
            for (j; j < ((i+1) / paArrSize + 1) * 4; j++)
            {
                printf("%c", *(hStr + j));
            }
            printf("\n");
        }
    }
    free(hStr);
}

int main() {
    //inicializacia vstupnej premennej, ukazovatel na pole SPZtiek, pocet zaznamov a smernik na subor
    char input = 'a', * SPZArr = NULL;
    int countOfRecs = 0;
    FILE* fr = NULL;

    while (input != 'k')
    {
        scanf("%c", &input);
        switch (input)
        {
            case ('v'): vWriteData(&fr); break;
            case ('o'): oWriteRewards(&fr); break;
            case ('n'): nMakeSPZString(&fr, &SPZArr, &countOfRecs); break;
            case ('s'): sWriteSPZS(countOfRecs, &SPZArr); break;
            case ('m'): mMostChar(countOfRecs, &SPZArr); break;
            case ('p'): pIsPalindrom(countOfRecs, &SPZArr); break;
            case ('z'): zMostSPZ(countOfRecs, &SPZArr); break;
            case ('k'): kClose(&fr, &SPZArr); break;
            case ('h'): hTableOfLet(countOfRecs, &SPZArr); break;
            case ('x'): xdelSPZ(&countOfRecs, &SPZArr); break;
            case ('c'): cAddSPZ(countOfRecs, &SPZArr); break;
            default:
                break;
        }

    }
    return 0;
}