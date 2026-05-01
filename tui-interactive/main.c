#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define sterge_ecran() system("clear")

#define culoare_normala "\x1B[0m"
#define culoare_rosu "\x1B[31m"
#define culoare_verde "\x1B[32m"
#define culoare_galben "\x1B[33m"
#define culoare_albastru "\x1B[34m"
#define culoare_magenta "\x1B[35m"
#define culoare_cyan "\x1B[36m"
#define culoare_alb "\x1B[37m"

void activeazaDoarUnCaracter() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void dezactiveazaDoarUnCaracter() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

typedef struct {
    char nume[100];
    char artist[100];
} cantec;

typedef struct {
    char nume[100];
    float nota;
    cantec cantece[100];
    int numar_de_cantece;
} playlist;

playlist playlisturi[100];
int numar_de_playlisturi = 0;

//*-------------------------------------------------------------
//*
//*               A F I Ș E A Z Ă   C U L O R I
//*
//*-------------------------------------------------------------

void afiseaza_culori() {
    printf("%s Hello World!\n", culoare_normala);
    printf("%s Hello World!\n", culoare_rosu);
    printf("%s Hello World!\n", culoare_verde);
    printf("%s Hello World!\n", culoare_galben);
    printf("%s Hello World!\n", culoare_albastru);
    printf("%s Hello World!\n", culoare_magenta);
    printf("%s Hello World!\n", culoare_cyan);
    printf("%s Hello World!\n", culoare_alb);
}

void add_cantece(int p_id, int s_id, char nume[], char artist[]) {
    strcpy(playlisturi[p_id].cantece[s_id].nume, nume);
    strcpy(playlisturi[p_id].cantece[s_id].artist, artist);
}

//*-------------------------------------------------------------
//*
//*                  A R A T Ă   C O M E N Z I
//*
//*-------------------------------------------------------------

void show_commands() {
    sterge_ecran();
    printf("%s!!!COMENZI!!!\n\n", culoare_rosu);
    printf("%sW - MUTĂ ÎN SUS\n", culoare_cyan);
    printf("%sS - MUTĂ ÎN JOS\n", culoare_cyan);
    printf("%sENTER - SELECTEAZĂ\n\n", culoare_cyan);
    printf("%s!!!OPRIȚI CAPS LOCK!!!\n\n", culoare_galben);
    printf("%sApăsați ENTER pentru a continua\n", culoare_alb);

    char input = getchar();

    while (input != '\n') {
        input = getchar();
    }
}

//*-------------------------------------------------------------
//*
//*                    E Ș T I   S I G U R ?
//*
//*-------------------------------------------------------------

int esti_sigur(char text[]) {
    int optiune = 0;

    while (1) {
        sterge_ecran();

        printf("%s%s\n\n", culoare_alb, text);
        if (optiune == 0) {
            printf("%s>>%sNu\n  Da\n", culoare_verde, culoare_alb);
        }
        else {
            printf("%s  Nu\n%s>>%sDa\n", culoare_alb, culoare_verde, culoare_alb);
        }
        char input = getchar();

        if (input == '\n') {
            return optiune;
        } else if (input == 'w' && optiune == 1) {
            optiune = 0;
        } else if (input == 's' && optiune == 0) {
            optiune = 1;
        }
    }
}

//*-------------------------------------------------------------
//*
//*                    A D A U G Ă   C Â N T E C
//*
//*-------------------------------------------------------------

int adauga_cantec(int id) {
    int optiune = 0;
    char nume[100], artist[100];

    sterge_ecran();
    dezactiveazaDoarUnCaracter();

    printf("%s---ADAUGĂ CÂNTEC---\n\n", culoare_magenta, playlisturi[id].nume);

    printf("%sNume: ", culoare_alb);
    fgets(nume, sizeof(nume), stdin);
    nume[strlen(nume) - 1] = '\0';
    printf("Artist: ");
    fgets(artist, sizeof(nume), stdin);
    artist[strlen(artist) - 1] = '\0';
    char text[100] = "Adaugă ";
    strcat(text, culoare_cyan);
    strcat(text, nume);
    strcat(text, culoare_alb);
    strcat(text, " de ");
    strcat(text, culoare_galben);
    strcat(text, artist);
    strcat(text, culoare_alb);
    strcat(text, " în ");
    strcat(text, playlisturi[id].nume);
    strcat(text, "?");
    activeazaDoarUnCaracter();
    int raspuns = esti_sigur(text);

    if (raspuns) {
        strcpy(playlisturi[id].cantece[playlisturi[id].numar_de_cantece].nume, nume);
        strcpy(playlisturi[id].cantece[playlisturi[id].numar_de_cantece].artist, artist);
        playlisturi[id].numar_de_cantece++;
    }
}

//*-------------------------------------------------------------
//*
//*                   Ș T E R G E   C Â N T E C
//*
//*-------------------------------------------------------------

int sterge_cantec(int id) {
    int optiune = 0;

    while (1) {
        sterge_ecran();

        printf("%s---ȘTERGE CÂNTEC---\n\n", culoare_magenta);

        for (int i = 0; i < playlisturi[id].numar_de_cantece + 1; i++) {
            char sageata[3] = ">>";
            if (optiune != i) {
                strcpy(sageata, "  ");
            }

            if (i == 0) {
                printf("%s%s%sÎnapoi\n", culoare_verde, sageata, culoare_rosu);
            } else {
                printf("%s%s%s%s - %s\n", culoare_verde, sageata, culoare_alb, playlisturi[id].cantece[i - 1].nume, playlisturi[id].cantece[i - 1].artist);
            }
        }

        char input = getchar();

        if (input == 'w' && optiune > 0) {
            optiune--;
        } else if (input == 's' && optiune < playlisturi[id].numar_de_cantece) {
            optiune++;
        } else if (input == '\n') {
            if (optiune == 0) {
                return 1;
            } else {
                char text[100] = "Șterge ";
                strcat(text, culoare_cyan);
                strcat(text, playlisturi[id].cantece[optiune - 1].nume);
                strcat(text, culoare_alb);
                strcat(text, " din ");
                strcat(text, playlisturi[id].nume);
                strcat(text, "?");
                int raspuns = esti_sigur(text);
                if (raspuns) {
                    int s_id = optiune - 1;
                    for (int i = s_id; i < playlisturi[id].numar_de_cantece - 1; i++)
                    {
                        playlisturi[id].cantece[i] = playlisturi[id].cantece[i + 1];
                    }
                    playlisturi[id].numar_de_cantece--;
                    return 1;
                }
            }
        }
    }
}

//*-------------------------------------------------------------
//*
//*                 V E Z I   P L A Y L I S T
//*
//*-------------------------------------------------------------

int vezi_playlist(int id) {
    int optiune = 0, sterge;

    while (1) {
        sterge = (playlisturi[id].numar_de_cantece ? 1 : 0);
        sterge_ecran();

        printf("%s---%s---\n\n", culoare_magenta, playlisturi[id].nume);

        for (int i = 0; i < playlisturi[id].numar_de_cantece + 3; i++) {
            char sageata[3] = ">>";

            if (i != optiune) {
                strcpy(sageata, "  ");
            }
            
            if (i == 0) {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_rosu, "Înapoi");
            } else if (i == 1) {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_cyan, "Adaugă Cântec");
            } else if (i == 2 && sterge) {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_cyan, "Șterge Cântec");
            } else {
                if (sterge) {
                    printf("%s%s%s%s - %s\n", culoare_verde, sageata, culoare_alb, playlisturi[id].cantece[i - 2 - sterge].nume, playlisturi[id].cantece[i - 2 - sterge].artist);
                }
            }
        }

        char input = getchar();

        if (input == 'w' && optiune > 0) {
            optiune--;
        }

        if (input == 's' && optiune < playlisturi[id].numar_de_cantece + 1 + sterge) {
            optiune++;
        }

        if (input == '\n') {
            if (optiune == 0) {
                return 1;
            } else if (optiune == 1) {
                adauga_cantec(id);
                optiune = 0;
            } else if (optiune == 2) {
                sterge_cantec(id);
                optiune = 0;
            }
        }
    }
}

//*-------------------------------------------------------------
//*
//*                 C R E E A Z Ă   P L A Y L I S T
//*
//*-------------------------------------------------------------

int create_playlist() {
    int optiune = 0;
    char nume[100] = "\n";

    while (1) {
        sterge_ecran();

        if (!strcmp(nume, "\n")) {
            printf("%s---CREEAZĂ PLAYLIST---\n\n", culoare_magenta);

            printf("%sNume playlist: ", culoare_alb);
            dezactiveazaDoarUnCaracter();
            fgets(nume, sizeof(nume), stdin);
        } else {
            nume[strlen(nume) - 1] = '\0';
            activeazaDoarUnCaracter();
            int raspuns = esti_sigur("Creează playlist?");
            if (raspuns) {
                strcpy(playlisturi[numar_de_playlisturi].nume, nume);
                playlisturi[numar_de_playlisturi].numar_de_cantece = 0;
                numar_de_playlisturi++;
            }

            return 1;
        }
    }
}

//*-------------------------------------------------------------
//*
//*                V E Z I  P L A Y L I S T U R I
//*
//*-------------------------------------------------------------

int vezi_playlisturi() {
    int optiune = 0;

    while (1) {
        sterge_ecran();
        printf("%s---PLAYLISTURILE MELE---\n", culoare_magenta);
        for (int i = 0; i < numar_de_playlisturi + 1; i++) {
            char sageata[3] = ">>";

            if (i != optiune) {
                strcpy(sageata, "  ");
            }
            if (i == 0) {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_rosu, "Înapoi");
            }
            else {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_alb, playlisturi[i - 1]);
            }
        }

        char input = getchar();

        if (input == 'w' && optiune > 0) {
            optiune--;
        }

        if (input == 's' && optiune < numar_de_playlisturi) {
            optiune++;
        }

        if (input == '\n') {
            if (optiune == 0) {
                return 1;
            } else {
                vezi_playlist(optiune - 1);
            }
        }
    }
}

//*-------------------------------------------------------------
//*
//*                Ș T E R G E   P L A Y L I S T
//*
//*-------------------------------------------------------------

int delete_playlist() {
    int optiune = 0;

    while (1) {
        sterge_ecran();

        printf("%s---Șterge Playlist---\n\n", culoare_magenta);

        for (int i = 0; i < numar_de_playlisturi + 1; i++) {
            char sageata[3] = ">>";
            if (i != optiune)
            {
                strcpy(sageata, "  ");
            }
            if (i == 0)
            {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_rosu, "Anulează");
            }
            else
            {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_alb, playlisturi[i - 1].nume);
            }
        }
        char input = getchar();

        if (input == 'w' && optiune > 0)
        {
            optiune--;
        }
        else if (input == 's' && optiune < numar_de_playlisturi)
        {
            optiune++;
        }
        else if (input == '\n')
        {
            if (optiune == 0)
            {
                return 1;
            }
            else
            {
                char text[100] = "Șterge ";
                strcat(text, playlisturi[optiune - 1].nume);
                strcat(text, "?");
                int raspuns = esti_sigur(text);
                if (raspuns)
                {
                    int id = optiune - 1;
                    for (int i = id; i < numar_de_playlisturi - 1; i++)
                    {
                        playlisturi[i] = playlisturi[i + 1];
                    }
                    numar_de_playlisturi--;
                    return 1;
                }
            }
        }
    }
}

//*-------------------------------------------------------------
//*
//*                 M E N I U   P R I N C I P A L
//*
//*-------------------------------------------------------------

int main_menu()
{
    int optiune = 0;
    char optiunes[4][100] = {"Vezi playlisturi", "Creeaza Playlist", "Șterge Playlist", "Ieșire"};

    while (1)
    {
        sterge_ecran();
        printf("%s---APLICAȚIE PLAYLISTuri---\n", culoare_magenta);
        for (int i = 0; i < 4; i++)
        {
            char sageata[3] = ">>";

            if (i != optiune)
            {
                strcpy(sageata, "  ");
            }
            if (i == 3)
            {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_rosu, optiunes[i]);
            }
            else
            {
                printf("%s%s%s%s\n", culoare_verde, sageata, culoare_alb, optiunes[i]);
            }
        }

        char input = getchar();

        if (input == 'w' && optiune > 0)
        {
            optiune--;
        }

        if (input == 's' && optiune < 3)
        {
            optiune++;
        }

        if (input == '\n')
        {
            if (optiune == 0)
            {
                vezi_playlisturi();
            }
            else if (optiune == 1)
            {
                create_playlist();
            }
            else if (optiune == 2) {
                delete_playlist();
            } else {
                dezactiveazaDoarUnCaracter();
                sterge_ecran();
                return 1;
            }
        }
    }
}

//*-------------------------------------------------------------
//*
//*                         M A I N
//*
//*-------------------------------------------------------------

int main() {
    strcpy(playlisturi[0].nume, "playlist 1");
    strcpy(playlisturi[1].nume, "playlist 2");
    strcpy(playlisturi[2].nume, "playlist 3");
    
    numar_de_playlisturi = 3;
    
    add_cantece(0, 0, "Song 1", "Artist 1");
    add_cantece(0, 1, "Song 2", "Artist 2");
    add_cantece(0, 2, "Song 3", "Artist 3");
    add_cantece(0, 3, "Song 4", "Artist 4");

    add_cantece(1, 0, "Song 5", "Artist 5");
    add_cantece(1, 1, "Song 6", "Artist 6");
    add_cantece(1, 2, "Song 7", "Artist 7");
    add_cantece(1, 3, "Song 8", "Artist 8");

    add_cantece(2, 0, "Song 9", "Artist 9");
    add_cantece(2, 1, "Song 10", "Artist 10");
    add_cantece(2, 2, "Song 11", "Artist 11");
    add_cantece(2, 3, "Song 12", "Artist 12");
    add_cantece(2, 4, "Song 13", "Artist 13");
    playlisturi[0].numar_de_cantece = 4;
    playlisturi[1].numar_de_cantece = 4;
    playlisturi[2].numar_de_cantece = 5;

    activeazaDoarUnCaracter();
    show_commands();

    main_menu();

    dezactiveazaDoarUnCaracter();
}
