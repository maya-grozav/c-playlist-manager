#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE "\e[0m"
#define RED "\e[0m\e[0;31m"
#define YELLOW "\e[0m\e[0;33m"
#define YELLOW_I "\e[0m\e[3;33m"
#define MAGENTA "\e[0m\e[0;35m"
#define MAGENTA_B "\e[0m\e[1;35m"
#define CYAN "\e[0m\e[0;36m"

typedef struct {
    char name[40];
    char artist[40];
} song;

typedef struct {
    char name[40];
    float rating;
    int number_of_songs;
    song songs[100];
} playlist;

void clear_buff() {
    while (getchar() != '\n');
}

void read(playlist* playlists, int* number_of_playlists, const char *filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        char aux;
        fscanf(file, "%d", number_of_playlists);
        fscanf(file, "%c", &aux);
        for (int i = 0; i < *number_of_playlists; i ++) {
            fgets(playlists[i].name, 40, file);
            playlists[i].name[strlen(playlists[i].name) - 1] = '\0';
            fscanf(file, "%f", &playlists[i].rating);
            fscanf(file, "%d", &playlists[i].number_of_songs);
            fscanf(file, "%c", &aux);
            for (int j = 0; j < playlists[i].number_of_songs; j ++) {
                fgets(playlists[i].songs[j].name, 40, file);
                fgets(playlists[i].songs[j].artist, 40, file);
                playlists[i].songs[j].name[strlen(playlists[i].songs[j].name) - 1] = '\0';
                playlists[i].songs[j].artist[strlen(playlists[i].songs[j].artist) - 1] = '\0';
            }
        }
        fclose(file);
    }
}

void display(playlist* playlists, int number_of_playlists) {
    printf("%sYour playlists\n", WHITE);
    for (int i = 0; i < number_of_playlists; i ++) {
        printf("%s%s\n", MAGENTA_B, playlists[i].name);
        printf("%srating: %s%.2f\n", WHITE, YELLOW_I, playlists[i].rating);
        for (int j = 0; j < playlists[i].number_of_songs; j ++) {
            printf("%s%s%s - %s\n", CYAN, playlists[i].songs[j].name, WHITE, playlists[i].songs[j].artist);
        }
        printf("\n");
    }
}

void write(playlist* playlists, int number_of_playlists, const char* filename) {
    FILE* file = fopen("temporaryFile.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", number_of_playlists);
        for (int i = 0; i < number_of_playlists; i ++) {
            fprintf(file, "%s\n%.2f %d\n", playlists[i].name, playlists[i].rating, playlists[i].number_of_songs);
            for (int j = 0; j < playlists[i].number_of_songs; j ++) {
                fprintf(file, "%s\n%s\n", playlists[i].songs[j].name, playlists[i].songs[j].artist);
            }
        }
    }
    fclose(file);
    remove(filename);
    rename("temporaryFile.txt", filename);
}

void read_new_playlist(playlist* new_playlist) {
    printf("Playlist name:\n");
    scanf("%[^\n]", &new_playlist->name);
    while (strlen(new_playlist->name) < 3) {
        clear_buff();
        printf("Name must be at least 3 characters long\n");
        scanf("%[^\n]", &new_playlist->name);
    }
    
    printf("Playlist rating(0-5):\n");
    int is_float = scanf("%f", &new_playlist->rating);
    
    while (new_playlist->rating > 5 || new_playlist->rating < 0 || !is_float) {
        clear_buff();
        printf("Rating must be a number between 0 and 5 (eg. 4.87)\n");
        is_float = scanf("%f", &new_playlist->rating);
    }

    new_playlist->number_of_songs = 0;
}

void read_new_song(song* new_song) {
    clear_buff();
    printf("Song name:\n");
    scanf("%[^\n]", &new_song->name);
    while (strlen(new_song->name) < 3) {
        clear_buff();
        printf("Name must be at least 3 characters long\n");
        scanf("%[^\n]", &new_song->name);
    }
    
    printf("Song artist(eg. Wolfgang Mozart):\n");
    int is_float = scanf("%s", &new_song->artist);
    
    while (strlen(new_song->artist) < 3 || new_song->artist[0] < 'A' || new_song->artist[0] > 'Z') {
        clear_buff();
        printf("Artist name must be at least 3 characters long and start with a capital letter\n");
        is_float = scanf("%s", &new_song->artist);
    }
}

void sort_playlists(playlist* playlists, int number_of_playlists) {
    playlist aux_playlist;
    for (int i = 0; i < number_of_playlists - 1; i ++) {
        for (int j = i + 1; j < number_of_playlists; j ++) {
            if (playlists[i].rating < playlists[j].rating) {
                aux_playlist = playlists[i];
                playlists[i] = playlists[j];
                playlists[j] = aux_playlist;
            }
        }
    }
}

void add_playlist(playlist* playlists) {
    playlist new_playlist;
    int number_of_playlists;
    read(playlists, &number_of_playlists, "playlists.txt");
    if (number_of_playlists == 10) {
        printf("You have the maximum number of playlists.\n");
        return;
    }
    read_new_playlist(&new_playlist);
    playlists[number_of_playlists ++] = new_playlist;
    write(playlists, number_of_playlists, "playlists.txt");
    printf("Playlist added sucessfully!");
}

void add_song(playlist* playlists) {
    song new_song;
    char s_name[40];
    int number_of_playlists, found = -1;
    read(playlists, &number_of_playlists, "playlists.txt");
    if (number_of_playlists == 0) {
        printf("You have no playlists.\n");
        return;
    }
    printf("What playlist do you want to add a song to?\n");
    scanf("%[^\n]", &s_name);
    for (int i = 0; i < number_of_playlists; i ++) {
        if (strcmp(playlists[i].name, s_name) == 0) {
            found = i;
        }
    }
    while (found == -1) {
        clear_buff();
        printf("That playlist doesn't exist.\n");
        scanf("%[^\n]", &s_name);
        for (int i = 0; i < number_of_playlists; i ++) {
            if (strcmp(playlists[i].name, s_name) == 0) {
                found = i;
            }
        }
    }

    if (playlists[found].number_of_songs == 100) {
        printf("This playlist is full.\n");
        return;
    }
    
    read_new_song(&new_song);
    playlists[found].songs[playlists[found].number_of_songs ++] = new_song;
    write(playlists, number_of_playlists, "playlists.txt");
    printf("Song added sucessfully!");
}

void delete_playlist(playlist* playlists) {
    char p_name[40];
    int number_of_playlists, found = -1;
    read(playlists, &number_of_playlists, "playlists.txt");
    if (number_of_playlists == 0) {
        printf("You have no playlists.\n");
        return;
    }
    printf("What playlist do you want to delete?\n");
    scanf("%[^\n]", &p_name);
    for (int i = 0; i < number_of_playlists; i ++) {
        if (strcmp(playlists[i].name, p_name) == 0) {
            found = i;
        }
    }
    while (found == -1) {
        clear_buff();
        printf("That playlist doesn't exist.\n");
        scanf("%[^\n]", &p_name);
        for (int i = 0; i < number_of_playlists; i ++) {
            if (strcmp(playlists[i].name, p_name) == 0) {
                found = i;
            }
        }
    }
    for (int i = found; i < number_of_playlists - 1; i ++) {
        playlists[i] = playlists[i + 1];
    }
    number_of_playlists --;
    write(playlists, number_of_playlists, "playlists.txt");
    printf("Playlist deleted sucessfully!");
}

void delete_song(playlist* playlists) {
    char p_name[40], s_name[40];
    int number_of_playlists, found_p = -1, found_s = -1;
    read(playlists, &number_of_playlists, "playlists.txt");
    if (number_of_playlists == 0) {
        printf("You have no playlists.\n");
        return;
    }
    printf("What playlist do you want to delete a song from?\n");
    scanf("%[^\n]", &p_name);
    for (int i = 0; i < number_of_playlists; i ++) {
        if (strcmp(playlists[i].name, p_name) == 0) {
            found_p = i;
        }
    }
    while (found_p == -1) {
        clear_buff();
        printf("That playlist doesn't exist.\n");
        scanf("%[^\n]", &p_name);
        for (int i = 0; i < number_of_playlists; i ++) {
            if (strcmp(playlists[i].name, p_name) == 0) {
                found_p = i;
            }
        }
    }

    if (playlists[found_p].number_of_songs == 0) {
        printf("This playlist is empty.\n");
        return;
    }

    printf("What song do you want to delete?\n");
    clear_buff();
    scanf("%[^\n]", &s_name);
    for (int i = 0; i < playlists[found_p].number_of_songs; i ++) {
        printf("verificam %s %d\n", playlists[found_p].songs[i].name, strcmp(playlists[found_p].songs[i].name, s_name));
        if (strcmp(playlists[found_p].songs[i].name, s_name) == 0) {
            found_s = i;
        }
    }
    while (found_s == -1) {
        clear_buff();
        printf("That song doesn't exist.\n");
        scanf("%[^\n]", &s_name);
        for (int i = 0; i < playlists[found_p].number_of_songs; i ++) {
            if (strcmp(playlists[found_p].songs[i].name, s_name) == 0) {
                found_s = i;
            }
        }
    }

    for (int i = found_s; i < playlists[found_p].number_of_songs - 1; i ++) {
        playlists[found_p].songs[i] = playlists[found_p].songs[i + 1];
    }
    playlists[found_p].number_of_songs --;
    write(playlists, number_of_playlists, "playlists.txt");
    printf("Song deleted sucessfully!");
}

int main(int argc, char *argv[]) {
    printf(WHITE);
    if (argc == 2 && strcmp(argv[1], "view") == 0) {
        playlist playlists[10];
        int number_of_playlists;
        read(playlists, &number_of_playlists, "playlists.txt");
        sort_playlists(playlists, number_of_playlists);
        display(playlists, number_of_playlists);
        playlist new_playlist;
    } else if (argc == 2 && strcmp(argv[1], "add_playlist") == 0) {
        playlist playlists[10];
        add_playlist(playlists);
    } else if (argc == 2 && strcmp(argv[1], "add_song") == 0) {
        playlist playlists[10];
        add_song(playlists);
    }  else if (argc == 2 && strcmp(argv[1], "delete_playlist") == 0) {
        playlist playlists[10];
        delete_playlist(playlists);
    }  else if (argc == 2 && strcmp(argv[1], "delete_song") == 0) {
        playlist playlists[10];
        delete_song(playlists);
    }  else if (argc == 2 && strcmp(argv[1], "help") == 0) {
        printf("%sview%s: view your playlists\n", RED, WHITE);
        printf("%sadd_playlist%s: add a playlist\n", RED, WHITE);
        printf("%sadd_song%s: add a song\n", RED, WHITE);
        printf("%sdelete_playlist%s: delete a playlist\n", RED, WHITE);
        printf("%sdelete_song%s: delete a song\n", RED, WHITE);
    } else{
        printf("%sUnknown command%s\n", RED, WHITE);
    }
}
