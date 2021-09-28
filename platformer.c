#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//defining struct for player and exit that contains their x and y coordinates
struct PlayerExit{
    int x_cord;
    int y_cord;
};

//making player and exit structs
struct PlayerExit P, X;

//defining struct for platforms that contains their switch and key coordinates, name, and dummy variables
struct Platform{
    char name;
    int switch_x_cord;
    int switch_y_cord;
    int key_x_cord;
    int key_y_cord;
    int active;
    int key_found;
};

//making platform structs for switches
struct Platform a, b, c, d, e, f, g, h;

//defining an array of platform structs and a counter for iteration
struct Platform switches[8];
int switches_count = 0;

//defining dummy variables for the existance of platforms
int A, B, C, D, E, F, G, H;

//defining dummy variables for the existance of switches
int aa, bb, cc, dd, ee, ff, gg, hh, p, x;

//array that contains all possible platforms
char platforms[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

//array that contains all valid tiles
char tiles[15] = {'*', '-', '+', '/', '\\', '#', '=', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

//defining a 2D array and counter for special object positions to check if two special objects are placed on the same coordinates
int obj_cord[100][2];
int obj_cord_count = 0;


//function that tests the map for loading errors
void test_map(char map[100][100], int map_len){
    int row_len[map_len]; //array that contains the length of each individual row

    //iterating through each map row
    for (int a = 0; a < map_len; a++){ 

        //finding the length of each row and storing it in row_len
        int length = 0;
        while (map[a][length] != '\n'){
            length++;
        }
        row_len[a] = length;


        if (row_len[a] != row_len[0]){ //eror case when the row lengths are inconsistent
            printf("Row %d of the level had incorrect length %d; length should have been %d.\n", a, row_len[a], row_len[0]);
            exit(0);
        }

        else if (row_len[a] > 59){ //error case when a row is longer than 60 tiles
            printf("Row %d is longer than maximum row length 60.\n", a);
            exit(0);
        }

        //iterating through each tile in the row
        for (int b = 0; b < row_len[a]; b++){

            //testing if the tile is valid by comparing it to the array that contains all possible tiles
            int found = 0;
            for (int c = 0; c < 15; c++){
                if (map[a][b] == tiles[c]){
                    found = 1;
                }
            }

            if (found == 0){ //error case when invalid tile is found
                printf("Found invalid tile at position %d, %d in the level.\n", b, a);
                exit(0);
            }

            //switch function for every tile
            switch (map[a][b]){

                    case '\\':
                        //testing EW ramp for leading to empty space
                        if (b == 0 || b == row_len[a]-1 || map[a][b-1] == ' ' || map[a][b+1] == ' '){
                            printf("Ramp at %d, %d leads off the board.\n", b, a);
                            exit(0);
                        }
                        //testing EW ramp for not starting or ending on correct tiles
                        else if (map[a][b-1] == '-'|| map[a][b-1] == '\\' || map[a][b-1] == '/' || map[a][b-1] == '=' || map[a][b-1] == '#' || map[a][b-1] == '*' || map[a][b+1] == '+' || map[a][b+1] == '\\' || map[a][b+1] == '/' || map[a][b+1] == '=' || map[a][b+1] == '#' || map[a][b+1] == '*'){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", b, a);
                            exit(0);
                        }
                        break;

                    case '/':
                        //testing WE ramp for leading to empty space
                        if (b == 0 || b == row_len[a]-1 || map[a][b-1] == ' ' || map[a][b+1] == ' '){
                            printf("Ramp at %d, %d leads off the board.\n", b, a);
                            exit(0);
                        }
                        //testing WE ramp for not starting or ending on correct tiles
                        else if (map[a][b-1] == '+'|| map[a][b-1] == '\\' || map[a][b-1] == '/' || map[a][b-1] == '=' || map[a][b-1] == '#' || map[a][b-1] == '*' || map[a][b+1] == '-' || map[a][b+1] == '\\' || map[a][b+1] == '/' || map[a][b+1] == '=' || map[a][b+1] == '#' || map[a][b+1] == '*'){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", b, a);
                            exit(0);
                        }
                        break;

                    case '#':
                        //testing SN ramp for leading to empty space
                        if (a == 0 || a == map_len-1 || map[a-1][b] == ' ' || map[a+1][b] == ' '){
                            printf("Ramp at %d, %d leads off the board.\n", b, a);
                            exit(0);
                        }
                        //testing SN ramp for not starting or ending on correct tiles
                        else if (map[a-1][b] == '-'|| map[a-1][b] == '\\' || map[a-1][b] == '/' || map[a-1][b] == '=' || map[a-1][b] == '#' || map[a-1][b] == '*' || map[a+1][b] == '+' || map[a+1][b] == '\\' || map[a+1][b] == '/' || map[a+1][b] == '=' || map[a+1][b] == '#' || map[a+1][b] == '*'){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", b, a);
                            exit(0);
                        }
                        break;

                    case '=':
                        //testing NS ramp for leading to empty space
                        if (a == 0 || a == map_len-1 || map[a-1][b] == ' ' || map[a+1][b] == ' '){
                            printf("Ramp at %d, %d leads off the board.\n", b, a);
                            exit(0);
                        }
                        //testing NS ramp for not starting or ending on correct tiles
                        else if (map[a-1][b] == '+' || map[a-1][b] == '\\' || map[a-1][b] == '/' || map[a-1][b] == '=' || map[a-1][b] == '#' || map[a-1][b] == '*' || map[a+1][b] == '-' || map[a+1][b] == '\\' || map[a+1][b] == '/' || map[a+1][b] == '=' || map[a+1][b] == '#' || map[a+1][b] == '*'){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", b, a);
                            exit(0);
                        }
                        break;
                    
                    //changing the dummy variable for the existance of platforms if they are found in the map
                    case 'A':
                        A = 1;
                        break;

                    case 'B':
                        B = 1;
                        break;

                    case 'C':
                        C = 1;
                        break;

                    case 'D':
                        D = 1;
                        break;

                    case 'E':
                        E = 1;
                        break;

                    case 'F':
                        F = 1;
                        break;

                    case 'G':
                        G = 1;
                        break;

                    case 'H':
                        H = 1;
                        break;
                    
                    default:
                        break;
            }
        }
    }
}


//function that loads the player and exit coordinates into their structs
void pe(char map[100][100], char obj[100][100], struct PlayerExit *PE, int obj_len, char name1[20], char name2[20]){ //pointer to struct is used as an argument
    
    //function that counts the number of spaces between the player and exit integers
    int count = 0;
    for (int a = 0; a < 25; a++){
        if (obj[obj_len][a] == ' ' && obj[obj_len][a+1] != ' '){ //counting the spaces is easier than integers themselves since integers can be two digits
            count++;
        }
    }

    if (count != 2){ //error case when there are not two integers since two spaces represent two integers
        printf("Could not find 2 integers for %s.\n", name1);
        exit(0);
    }

    else{
        int cords[2]; //array that stores the x and y coordinates
        int j = 2; //counting from the third index since the first two are the letter and the space

        //using atoi to convert the integers and storing them in cords
        for (int b = 0; b < 2; b++){
            cords[b] = atoi(&obj[obj_len][j]);   
            j++;

            //iterating through the line for special characters until a space is found to avoid storing the same integer twice
            while (obj[obj_len][j] != ' '){
                j++;
            }
        }

        //storing the x and y coordinates in the PlayerExit struct
        PE->x_cord = cords[0];
        PE->y_cord = cords[1];

        //checking if the x and y coordinate pair already exists in the global 2D array obj_cord
        for (int c = 0; c < obj_cord_count; c++){
            if (PE->x_cord == obj_cord[c][0] && PE->y_cord == obj_cord[c][1]){ //error case when two special objects have the same coordiantes
                printf("Two special objects are placed at the same coordinates %d, %d.\n", PE->x_cord, PE->y_cord);
                exit(0);
            }
        }

        //if the coordinates are unique, they are stored into obj_cord and the counter is incremented to move on to the next coordinate pair
        obj_cord[obj_cord_count][0] = PE->x_cord;
        obj_cord[obj_cord_count][1] = PE->y_cord;
        obj_cord_count++;

        if (map[PE->y_cord][PE->x_cord] == '*'){ //error case when the player or exit are placed on a wall
            printf("A special object was placed on a wall at %d, %d.\n", PE->x_cord, PE->y_cord);
            exit(0);
        }


        for (int j = 0; j < 8; j++){ //error case when the player or exit are placed on a platform
            if (map[PE->y_cord][PE->x_cord] == platforms[j]){ //iterating through the possible platform tiles
                printf("%s was placed on a platform.\n", name2);
                exit(0);
            }
        }
    }
}


//function that loads the platform coordinates into their structs
void pf(char map[100][100], char obj[100][100], struct Platform *PF, int i, char name){ //pointer to struct is used as an argument
    
    //function that counts the number of spaces between the player and exit integers
    int count = 0;
    for (int b = 0; b < 25; b++){
        if (obj[i][b] == ' ' && obj[i][b+1] != ' '){ //counting the spaces is easier than integers themselves since integers can be two digits
            count++;
        }
    }

    if (count != 5){ //error case when there are not five integers since two spaces represent two integers
        printf("Could not find 5 integers for switch %c.\n", name);
        exit(0);
    }

    else{
        int cords[5]; //array that stores the x and y coordinates for switch and key positions, and also the active variable
        int j = 2; //counting from the third index since the first two are the letter and the space
        
        //using atoi to convert the integers and storing them in cords
        for (int a = 0; a < 5; a++){
            cords[a] = atoi(&obj[i][j]);   
            j++;

            //iterating through the line for special characters until a space is found to avoid storing the same integer twice
            while (obj[i][j] != ' '){
                j++;
            }
        }
       
        //storing the name, x and y coordinates of switch and key positions, active variable, and setting key_found to 0 for platform structs
        PF->name = obj[i][0];
        PF->switch_x_cord = cords[0];
        PF->switch_y_cord = cords[1];
        PF->key_x_cord = cords[2];
        PF->key_y_cord = cords[3];
        PF->active = cords[4];
        PF->key_found = 0;

        //dereferencing the struct pointer and storing it in the array of structs while incrementing the counter
        switches[switches_count] = *PF;
        switches_count++;

        //checking if the switch coordinates already exist in the global obj_cord
        for (int a = 0; a < obj_cord_count; a++){
            if (PF->switch_x_cord == obj_cord[a][0] && PF->switch_y_cord == obj_cord[a][1]){ //error case when two special objects share the same coordinates
                printf("Two special objects are placed at the same coordinates %d, %d.\n", PF->switch_x_cord, PF->switch_y_cord);
                exit(0);
            }
        }

        //storing switch coordinates into the obj_cord array and incrementing the counter
        obj_cord[obj_cord_count][0] = PF->switch_x_cord;
        obj_cord[obj_cord_count][1] = PF->switch_y_cord;
        obj_cord_count++;

        //checking if the key coordinates already exist in the global obj_cord
        for (int a = 0; a < obj_cord_count; a++){
            if (PF->key_x_cord == obj_cord[a][0] && PF->key_y_cord == obj_cord[a][1]){ //error case when two special objects share the same coordinates
                printf("Two special objects are placed at the same coordinates %d, %d.\n", PF->key_x_cord, PF->key_y_cord);
                exit(0);
            }
        }

        //storing key coordinates into the obj_cord array and incrementing the counter
        obj_cord[obj_cord_count][0] = PF->key_x_cord;
        obj_cord[obj_cord_count][1] = PF->key_y_cord;
        obj_cord_count++;

        if (map[PF->switch_y_cord][PF->switch_x_cord] == '*'){ //error case when a switch is placed on a wall
            printf("A special object was placed on a wall at %d, %d.\n", PF->switch_x_cord, PF->switch_y_cord);
            exit(0);
        }
        
        else if(map[PF->key_y_cord][PF->key_x_cord] == '*'){ //error case when a key is placed on a wall
            printf("A special object was placed on a wall at %d, %d.\n", PF->key_x_cord, PF->key_y_cord);
            exit(0);
        }

        for (int j = 0; j < 8; j++){ //iterating through the possible platform tiles
            if (map[PF->switch_y_cord][PF->switch_x_cord] == platforms[j]){ //error case when a switch is placed on a platform
                printf("Switch %c was placed on a platform.\n", name);
                exit(0);
            }

            else if (map[PF->key_y_cord][PF->key_x_cord] == platforms[j]){ //error case when a key is placed on a platform
                printf("Key %c was placed on a platform.\n", name);
                exit(0);
            }
        }
    }
}


//function that tests the special objects for loading errors
void test_obj(char map[100][100], char obj[100][100], int obj_len){ 

    //array of valid special objects to read
    char special_objects[10] = {'P', 'X', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    
    //iterating through the special objects and testing if they contain the valid letters
    for (int i = 0; i < obj_len; i++){ 
        int found = 0;

        for (int j = 0; j < 10; j++){
            if (obj[i][0] == special_objects[j]){
                found = 1;
            }
        }

        if (found == 0){ //error case when an invalid letter is included in level file
            printf("Level file contained invalid object %c.\n", obj[i][0]);
            exit(0);
        }
    }

    //reiterating for the 'too many switches' test case
    for (int i = 0; i < obj_len; i++){
        //switch function for the first letter of each special objects line
        switch (obj[i][0]){
            
            case 'P':
                p = 1;
                pe(map, obj, &P, i, "player", "Player P"); //calling loading function with the address of player struct
                break;

            case 'X':
                x = 1;
                pe(map, obj, &X, i, "exit", "Exit X"); //calling loading function with the address of exit struct
                break;

            case 'a':
                if (A == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch a.\n");
                    exit(0);
                }
                aa = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &a, i, 'a'); //calling loading function with the address of platform struct
                break;
            
            case 'b':
                if (B == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch b.\n");
                    exit(0);
                }
                bb = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &b, i, 'b'); //calling loading function with the address of platform struct
                break;
            
            case 'c':
                if (C == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch c.\n");
                    exit(0);
                }
                cc = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &c, i, 'c'); //calling loading function with the address of platform struct
                break;
            
            case 'd':
                if (D == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch d.\n");
                    exit(0);
                }
                dd = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &d, i, 'd'); //calling loading function with the address of platform struct
                break;
            
            case 'e':
                if (E == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch e.\n");
                    exit(0);
                }
                ee = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &e, i, 'e'); //calling loading function with the address of platform struct
                break;
            
            case 'f':
                if (F == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch f.\n");
                    exit(0);
                }
                ff = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &f, i, 'f'); //calling loading function with the address of platform struct
                break;
            
            case 'g':
                if (G == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch g.\n");
                    exit(0);
                }
                gg = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &g, i, 'g'); //calling loading function with the address of platform struct
                break;
            
            case 'h':
                if (H == 0){ //error case when switch exists but platform doesn't
                    printf("No platform exists for switch h.\n");
                    exit(0);
                }
                hh = 1; //dummy variable for the existence of switch is turned on
                pf(map, obj, &a, i, 'h'); //calling loading function with the address of platform struct
                break;

            default:
                break;
        }
    }

    if (p == 0){ //error case when player doesn't exist
        printf("No player is defined in the level file.\n");
        exit(0);
    }

    if (x == 0){ //error case when exit doesn't exist
        printf("No exit is defined in the level file.\n");
        exit(0);
    }

    //error cases when platform exists but its switch doesn't
    if (A == 1 && aa == 0){
        printf("No switch exists for platform A.\n");
        exit(0);
    }
    if (B == 1 && bb == 0){
        printf("No switch exists for platform B.\n");
        exit(0);
    }
    if (C == 1 && cc == 0){
        printf("No switch exists for platform C.\n");
        exit(0);
    }
    if (D == 1 && dd == 0){
        printf("No switch exists for platform D.\n");
        exit(0);
    }
    if (E == 1 && ee == 0){
        printf("No switch exists for platform E.\n");
        exit(0);
    }
    if (F == 1 && ff == 0){
        printf("No switch exists for platform F.\n");
        exit(0);
    }
    if (G == 1 && gg == 0){
        printf("No switch exists for platform G.\n");
        exit(0);
    }
    if (H == 1 && hh == 0){
        printf("No switch exists for platform H.\n");
        exit(0);
    }
}


//function that displays the map
void show_map(char map[100][100], int map_len){
    //exit is placed at its coordinates since it will never change
    map[X.y_cord][X.x_cord] = 'X';

    for (int i = 0; i < map_len; i++){ //iterating through each row of the map
        int j = 0; //counter for each tile in the row, incremented after the while loop

        while (map[i][j] != '\n'){
            int key = 0; //variable that determines whether a key exists on that tile

            //setting key to 1 if the tile matches the coordinates of a key
            for (int a = 0; a < switches_count; a++){
                if (i == switches[a].key_y_cord && j == switches[a].key_x_cord && switches[a].key_found == 0){
                    key = 1;
                }
            }

            int switch_index = -1; 
            //changing the switch index if a switch is found
            for (int a = 0; a < switches_count; a++){
                if (i == switches[a].switch_y_cord && j == switches[a].switch_x_cord){
                    switch_index = a;
                }
            }

            //replacing walls with empty space
            if (map[i][j] == '*'){
                printf(" ");
            }

            //displaying player
            else if (i == P.y_cord && j == P.x_cord){
                printf("P");
            }

            //displaying key if the tile matches key coordinates
            else if (key == 1){
                printf("$");
            }

            //displaying switch if the tile matches switch coordinates
            else if (switch_index != -1){
                printf("%c", switches[switch_index].name);
            }

            //case if the tile is a platform
            else if (map[i][j] >= 'A' && map[i][j] <= 'H'){
                
                //searching for the switch of the platform in the array of switches
                int count = 0;
                while (map[i][j] != toupper(switches[count].name)){
                    count++;
                }

                //platform is treated as low ground if it is inactive
                if (switches[count].active == 0){
                    printf("-");
                }

                //otherwise printing the platform
                else {
                    printf("%c", map[i][j]); 
                }
            }

            //otherwise printing the tile
            else {
                printf("%c", map[i][j]);
            }
            j++;
        }
        printf("\n");
    }
}


//function that returns 1 if moving to the next position is allowed, and 0 if it is not allowed
int check_pos(char current_pos, char next_pos, char direction){
    int found = 0;
    //checking if the next position is a valid tile
    for (int i = 0; i < 15; i++){
        if (next_pos == tiles[i]){
            found = 1;
        }
    }

    if (next_pos == 'X'){ 
        return 1;
    }

    else if (next_pos == '*'){ //not allowed to move to a wall
        return 0;
    }

    else if (found == 0){ //not allowed to move to an empty space
        return 0;
    }

    switch (current_pos){ //switch for current position
        case '-':
            if (next_pos == '-'){ //moving from low ground to low ground
                return 1;
            }

            else if (next_pos == '/' && direction == 'r'){ //moving right to a WE ramp from low ground
                return 1;
            }

            else if (next_pos == '\\' && direction == 'l'){ //moving left to a EW ramp from low ground
                return 1;
            }

            else if (next_pos == '#' && direction == 'u'){ //moving up to a NS ramp from low ground
                return 1;
            }

            else if (next_pos == '=' && direction == 'd'){ //moving down to a SN ramp from low ground
                return 1;
            }
            break;

        case '+':
            if (next_pos == '+' || next_pos == 'A'){ //moving from high ground to high ground or active platform
                return 1;
            }

            else if (next_pos == '/' && direction == 'l'){ //moving left to a WE ramp from high ground
                return 1;
            }

            else if (next_pos == '\\' && direction == 'r'){ //moving right to a EW ramp from high ground
                return 1;
            }

            else if (next_pos == '#' && direction == 'd'){ //moving down to a NS ramp from high ground
                return 1;
            }

            else if (next_pos == '=' && direction == 'u'){ //moving up to a SN ramp from high ground
                return 1;
            }
            break;

        case '/':
            if ((next_pos == '+' || next_pos == 'A') && direction == 'r'){ //moving right to high ground or active platform from WE ramp
                return 1;
            }

            else if (next_pos == '-' && direction == 'l'){ //moving left to low ground from WE ramp
                return 1;
            }

            else if (next_pos == '/' && (direction == 'u' || direction == 'd')){ //moving up or down to identical ramp
                return 1;
            }
            break;

        case '\\':
            if ((next_pos == '+' || next_pos == 'A') && direction == 'l'){ //moving left to high ground or active platform from EW ramp
                return 1;
            }

            else if (next_pos == '-' && direction == 'r'){ //moving right to low ground from EW ramp
                return 1;
            }

            else if (next_pos == '\\' && (direction == 'u' || direction == 'd')){ //moving up or down to identical ramp
                return 1;
            }
            break;

        case '#':
            if ((next_pos == '+' || next_pos == 'A') && direction == 'u'){ //moving up to high ground or active platform from NS ramp
                return 1;
            }

            else if (next_pos == '-' && direction == 'd'){ //moving down to low ground from NS ramp
                return 1;
            }

            else if (next_pos == '#' && (direction == 'l' || direction == 'r')){ //moving left or right to identical ramp
                return 1;
            }
            break;

        case '=':
            if ((next_pos == '+' || next_pos == 'A') && direction == 'd'){ //moving down to high ground or active platform from SN ramp
                return 1;
            }
            if (next_pos == '-' && direction == 'u'){ //moving up to low ground from SN ramp
                return 1;
            }
            else if (next_pos == '=' && (direction == 'l' || direction == 'r')){ //moving left or right to identical ramp
                return 1;
            }
            break;
        
        //case where current position is an active platform
        default:
            if (next_pos == '+' || next_pos == 'A'){ //moving from active platform to high ground or active platform
                return 1;
            }

            else if (next_pos == '/' && direction == 'l'){ //moving left to WE ramp from active platform
                return 1;
            }

            else if (next_pos == '\\' && direction == 'r'){ //moving right to EW ramp from active platform
                return 1;
            }

            else if (next_pos == '#' && direction == 'd'){ //moving down to NS ramp from active platform
                return 1;
            }

            else if (next_pos == '=' && direction == 'u'){ //moving up to SN ramp from active platform
                return 1;
            }
            break;
    }
    return 0; //0 is returned if no legal movement is detected
}


//function that checks for the player reaching the exit or picking up a key
void check(char map[100][100]){
    //checking if the player and exit have the same coordinates
    if (P.x_cord == X.x_cord && P.y_cord == X.y_cord){
        printf("Congratulations, you win!\n");
        exit(0);
    }

    else {
        for (int a = 0; a < switches_count; a++){
            //checking if the player and any key have the same coordinates
            if (P.x_cord == switches[a].key_x_cord && P.y_cord == switches[a].key_y_cord && switches[a].key_found == 0){
                switches[a].key_found = 1; //changing dummy variable key_found for the platform to 1
                printf("You picked up the key for switch %c!\n", switches[a].name);
            }
        }
    }
}


//function that takes input and executes a command
void input(char map[100][100], int map_len){
    char command[100];
    fgets(command, 100, stdin); //receiving command from standard input with fgets, ignoring white space
    //counting the number of elements in command
    int counter = 0;
    while (command[counter]!='\n'){
        counter++;
    }

    if (counter != 1){ //case where there is more than one letter given
        printf("Command not recognised.\n\n");
        show_map(map, map_len);
        input(map, map_len);
    }

    else{
        int found; //dummy variable for the existence of a switch
        int count = 0; //dummy variable for the counter
        char current_pos, next_pos; //characters that represent the player's current and next position

        //if the player's current position is a platform
        if (map[P.y_cord][P.x_cord] >= 'A' && map[P.y_cord][P.x_cord] <= 'H'){

        //finds the platform's switch in the array of structs
            while (map[P.y_cord][P.x_cord] != toupper(switches[count].name)){
                count++;
            }

            //changing current position to low ground if the platform is inactive
            if (switches[count].active == 0){
                current_pos = '-';
            }

            //changing current position to A platform if the platform is active for simplicity sake
            else {
                current_pos = 'A';
            }
        }

        //if not a platform, current position is the tile that the player is standing on
        else{
            current_pos = map[P.y_cord][P.x_cord];
        }

        switch (command[counter-1]){ //switch function for the element before the end of line character
            case 'w':
                //if the player's next position is a platform
                if (map[P.y_cord-1][P.x_cord] >= 'A' && map[P.y_cord-1][P.x_cord] <= 'H'){

                    //finds the platform's switch in the array of structs
                    while (map[P.y_cord-1][P.x_cord] != toupper(switches[count].name)){
                        count++;
                    }

                    //changing next position to low ground if the platform is inactive
                    if (switches[count].active == 0){
                        next_pos = '-';
                    }

                    //changing next position to A platform if the platform is active for simplicity sake
                    else {
                        next_pos = 'A';
                    }
                }
                //if not a platform, next position is the tile that the player is standing on
                else {
                    next_pos = map[P.y_cord-1][P.x_cord];
                }

                if (check_pos(current_pos, next_pos, 'u') == 1){
                    P.y_cord--;
                }
                
                check(map); //calls check function to see if the exit is reached or a key is picked up
                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 's':
                //if the player's next position is a platform
                if (map[P.y_cord+1][P.x_cord] >= 'A' && map[P.y_cord+1][P.x_cord] <= 'H'){

                    //finds the platform's switch in the array of structs
                    while (map[P.y_cord+1][P.x_cord] != toupper(switches[count].name)){
                        count++;
                    }

                    //changing next position to low ground if the platform is inactive
                    if (switches[count].active == 0){
                        next_pos = '-';
                    }

                    //changing next position to A platform if the platform is active for simplicity sake
                    else {
                        next_pos = 'A';
                    }
                }
                //if not a platform, next position is the tile that the player is standing on
                else {
                    next_pos = map[P.y_cord+1][P.x_cord];
                }

                //the player's coordinate is only changed if the movement is valid
                if (check_pos(current_pos, next_pos, 'd') == 1){
                    P.y_cord++;
                }

                check(map); //calls check function to see if the exit is reached or a key is picked up
                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'a':
                //if the player's next position is a platform
                if (map[P.y_cord][P.x_cord-1] >= 'A' && map[P.y_cord][P.x_cord-1] <= 'H'){

                    //finds the platform's switch in the array of structs
                    while (map[P.y_cord][P.x_cord-1] != toupper(switches[count].name)){
                        count++;
                    }

                    //changing next position to low ground if the platform is inactive
                    if (switches[count].active == 0){
                        next_pos = '-';
                    }

                    //changing next position to A platform if the platform is active for simplicity sake
                    else {
                        next_pos = 'A';
                    }
                }
                //if not a platform, next position is the tile that the player is standing on
                else {
                    next_pos = map[P.y_cord][P.x_cord-1];
                }

                //the player's coordinate is only changed if the movement is valid
                if (check_pos(current_pos, next_pos, 'l') == 1){
                    P.x_cord--;
                }

                check(map); //calls check function to see if the exit is reached or a key is picked up
                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'd':
                //if the player's next position is a platform
                if (map[P.y_cord][P.x_cord+1] >= 'A' && map[P.y_cord][P.x_cord+1] <= 'H'){

                    //finds the platform's switch in the array of structs
                    while (map[P.y_cord][P.x_cord+1] != toupper(switches[count].name)){
                        count++;
                    }

                    //changing next position to low ground if the platform is inactive
                    if (switches[count].active == 0){
                        next_pos = '-';
                    }

                    //changing next position to A platform if the platform is active for simplicity sake
                    else {
                        next_pos = 'A';
                    }
                }
                //if not a platform, next position is the tile that the player is standing on
                else {
                    next_pos = map[P.y_cord][P.x_cord+1];
                }

                
                //the player's coordinate is only changed if the movement is valid
                if (check_pos(current_pos, next_pos, 'r') == 1){
                    P.x_cord++;
                }

                check(map); //calls check function to see if the exit is reached or a key is picked up
                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'x':
                //checking if the player's current tile is a switch
                found = 0;
                for (int i = 0; i < switches_count; i++){ //iterates through the array of switches
                    if (P.x_cord == switches[i].switch_x_cord && P.y_cord == switches[i].switch_y_cord){
                        found = 1;
                    }
                }

                if (found == 0){ //error case where the player tries to toggle a switch but is not standing on one
                    printf("You are not standing on a switch!\n");
                }

                else{
                    for (int i = 0; i < switches_count; i++){ //iterates through the array of switches
                        if (P.x_cord == switches[i].switch_x_cord && P.y_cord == switches[i].switch_y_cord){
                            if (switches[i].key_found == 0){ //error case where the key for the switch is not found
                                printf("You do not have the key for that switch!\n");
                            }

                            //toggles the switch's active variable
                            else { 
                                if (switches[i].active == 1){
                                    switches[i].active = 0;
                                }
                                
                                else {
                                    switches[i].active = 1;
                                }
                            }
                        }
                    }
                }
                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'k':
                //sets up the checking list in an alphabetical order
                for (char i = 'a'; i <= 'h'; i++){
                    for (int j = 0; j < switches_count; j++){ //iterates through the array of switches
                        //printing the name and key_found variable for the switch that matches the letter
                        if (i == switches[j].name){
                            printf("%c: %d\n", switches[j].name, switches[j].key_found);
                        }
                    }
                }

                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'e':
                //sets up the checking list in an alphabetical order
                for (char i = 'A'; i <= 'H'; i++){
                    for (int j = 0; j < switches_count; j++){ //iterates through the array of switches
                        //printing the name and key_found variable for the switch that matches the letter
                        if (i == toupper(switches[j].name)){
                            printf("%c: %d\n", i, switches[j].active);
                        }
                    }
                }

                printf("\n");
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;
                
            case 'h':
                printf("w: Move up one space.\n");
                printf("s: Move down one space.\n");
                printf("a: Move left one space.\n");
                printf("d: Move right one space.\n");
                printf("x: Toggle the state of the switch you are standing on.\n");
                printf("k: See which keys you have collected.\n");
                printf("e: See which platforms are on and which platforms are off.\n");
                printf("h: Print this help message.\n");
                printf("q: Quit the game.\n\n");

                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;

            case 'q':
                exit(0);

            default:
                printf("Command not recognised.\n\n"); //error case for invalid command
                show_map(map, map_len); //displays the current map
                input(map, map_len); //asks for input again
                break;
        }
    }
}


//function that loads map and special objects while returning the number of rows that the map has
int load(char* file_name, char map[100][100], char obj[100][100]){ //takes file name as a string, and 2D arrays representing map and special objects
    if (access(file_name, F_OK) == 0){ //unistd.h function that checks if the given file exists
        FILE *fp = fopen(file_name, "r"); //file pointer that opens the file in read mode
        char buff[255]; 
        int i = 0;
        int j = 0;

        while (fgets(buff, 255, fp) != NULL && strcmp(buff, "\n") != 0){ //reading individual rows of the map from file pointer to buffer until an empty line appears
            memcpy(&map[i], buff, strlen(buff)); //copying the memory of each individual map row into the 2D map array
            i++; //incrementing i to move on to the next row
        }

        if (i > 19){ //error case when there are more than 20 rows for the map
            printf("Level has more rows than maximum row number 20.\n");
            exit(0);
        }

        else{
            while (fgets(buff, 255, fp) != NULL){ //continues reading to buffer from the beginning of special objects
                memcpy(&obj[j], buff, strlen(buff)); //copying the memory of each individual special object row into the 2D obj array
                j++; //incrementing j to move on to the next row
            }

            if (j == 0){ //error case where the special objects section has 0 rows
                printf("The file ended before any special objects had been defined.\n");
                exit(0);
            }

            fclose(fp); //closing file
            test_map(map, i); //calling function that tests map for loading errors with the number of map rows
            test_obj(map, obj, j); //calling function that tests obj for loading errors with the number of obj rows
            return i; //returning number of map rows to main
        }
    }

    else{ //error case when file passed in from command line doesn't exist
        printf("File %s does not exist.\n", file_name);
        exit(0);
    }
}


int main(int argc, char* argv[]){
    //error case for no file name given
    if (argc == 1){
        printf("Usage: ./platformer <level file>\n");
        return 0;
    }

    else{
        //declearing 2D arrays for map and special objects
        char map[100][100];
        char obj[100][100]; 

        char* file_name = argv[1];
        int map_len = load(file_name, map, obj); //calls load and outputs map length
        show_map(map, map_len);
        input(map, map_len);
        return 0;
    }
}
