
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define false 0
#define true 1
#define CTRLD 3
#define ALLOCATION_FAILURE 4


int i,j = 0;

struct treasure {
  char code[5];
  int col;
  int row;
  int found;
};

void freeMemory(char **map, int h) {
    for (i = 0; i < h; ++i) {
        free(map[i]);
    }
    free(map);
}

void checkEOF(int result, char **map, int height, struct treasure *treasures){
    if(result == EOF ){
        printf("EOF detected. Program terminated.\n");
        freeMemory(map, height);
        free(treasures);
        exit(CTRLD);
    }
}
void checkStdin(){
    if(getchar() != '\n'){
        while (getchar() != '\n');
    }
}

int inputLength(char *message){
    int input;
    unsigned long int temp_input;
    int result = -2;
    
    while(1){
        printf("%s",message);
        result = scanf("%lu", &temp_input);
        
         if(result == EOF){
             printf("EOF detected. Program terminated.\n");
            exit(CTRLD);
         }
        
        if (result != 1  || temp_input < 2 || temp_input > 32) {
            result = -2;
            while (getchar() != '\n');
            
        } else {
           checkStdin();
            input = (unsigned int)temp_input;
               
                return input;
        }
    }
    return 0;
}
int inputTeasureNum(int width, int height, char **map){
    int number;
    int numberResult;
    int limit = (width * height)/4;
    while(1){
        printf("Enter number of treasures to add:");
        numberResult = scanf("%d", &number);
        if(numberResult == EOF ){
            printf("EOF detected. Program terminated.\n");
            freeMemory(map, height);
            exit(CTRLD);
        }

        if(number > limit || numberResult != 1 ){
            printf("Treasure numbers cannot excceed %d.\n", limit);
        }else if(number <= 0){
            printf("Treasure numbers should at least be queal or larger than 1.\n");
        }else{
            checkStdin();
            return number;
        }
        while (getchar() != '\n');
        
    }
    
}

int validateTreasureCode(char *code, struct treasure *treasures, int num){
    /* CHeck the length of code*/
    if (strlen(code) != 4) {
        return false;
    }
    
    /* Check whether treasure code begins with 'T' and with */
    if (code[0] != 'T') {
        return false;
        
    }
    for (i = 1; i < 4; i++) {
        if (code[i] < '0' || code[i] > '9') {
            if (!isdigit(code[i])) {
                return false;
            }
        }
    }
    
    for (i = 0; i < num; i++) {
        if (treasures[i].code[0] == code[0]
            &&treasures[i].code[1] == code[1]
            &&treasures[i].code[2] == code[2]
            &&treasures[i].code[3] == code[3]
            ) {
            printf("Treasure code already in use.\n");
            return false;
        }
    }
    return true;
}

void inputTreasureCode(struct treasure *treasures, int num, char **map, int height){
    int i, codeResult;
    char temp_code[5];
    for(i = 0; i < num; i++){
        while(1){
            printf("Enter treasure #%d code (e.g. T001): \n",i+1);
            codeResult= scanf("%4s", temp_code);
            checkEOF(codeResult, map, height, treasures);
            if(getchar() != '\n'){
                printf("Treasure codes must be a 'T' followed by 3 digits.\n");
                while (getchar() != '\n');
            }else{
                if(validateTreasureCode(temp_code, treasures, num) != 1){
                    printf("Treasure codes must be a 'T' followed by 3 digits.\n");
                    
                }
                else{
                    /* This won't work.
                        treasures[i].code = temp_code;
                        (1) treasures[i].code and temp_code are pointers.
                        (2) Revised as 
                            *treasures[i].code = *temp_code;
                            Only "T" will be copied because both sides an pointer point to the first element in the array.
                    */
                    
                    treasures[i].code[0] = temp_code[0];
                    treasures[i].code[1] = temp_code[1];
                    treasures[i].code[2] = temp_code[2];
                    treasures[i].code[3] = temp_code[3];
                    treasures[i].code[4] = temp_code[4];
                    break;
                } 
            }
        }
    }
}

char **allocateArray(int w, int h){
    char **map = (char **)malloc(h * sizeof(char *));
    if(map == NULL){
        printf("Do not have enough memory to allocate.");
        exit(ALLOCATION_FAILURE);
    }
    for(i = 0; i < h; i++){
        map[i] = (char *)malloc(w * sizeof(char));
        if(map[i] == NULL){
            printf("Do not have enough memory to allocate.");
            break;
         }
    }
    /* Assign an empty value to each element in the map. */
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            map[i][j] = ' ';
        }
    }
    return map;
}

char** printMap(char **map, int width, int height, int num){
    int row, col;
    printf("Map:\n\n");
    printf("   ");
    
    for (i = 0; i < width; i++) {
        printf("%d", i / 10);
    }
    printf("\n");
    printf("   ");
    for(i = 0; i < width; i++){
       
        printf("%d", i%10);
    }
     printf("\n");
     printf("   ");
     for (i = 0; i < width; i++) {
        printf("_");
    }
    printf("\n");
   
    for (row = 0; row < height; row++) {
        printf("%02d|", row);
        for (col = 0; col < width; col++) {
            printf("%c",map[row][col]);
        }
        printf("\n");
    }
    return map;
}

void randomTreasures(struct treasure *treasures, int num, int width, int height) {
    srand(time(0)); /* Random seed */
    for (i = 0; i < num; i++) {
        /* Generate a new treasure */
        int new_col, new_row;
        do {
            new_col = rand() % width;
            new_row = rand() % height;
            /* Check if the new treasure overlaps with any existing treasures */
            for (j = 0; j < i; j++) {
                if (new_col == treasures[j].col && new_row == treasures[j].row) {
                    /* If overlap found, generate new coordinates */
                    new_col = rand() % width;
                    new_row = rand() % height;
                    /* Reset loop to check again */
                    j = -1;
                }
            }
        } while (j != i); /* Exit loop when no overlap */

        /* Assign coordinates to the new treasure */
        treasures[i].col = new_col;
        treasures[i].row = new_row;
    }
}
/* Check wheter the whole map is dug. */
int allDig(int width, int height, char **map){
    int isDug = true;
    for (i = 0; i < height ; i++) {
        for (j = 0; j < width; j++) {
            if(map[i][j] == ' '){
                isDug = false;
                break;
            }else{
                continue;
            }
        }
    }
    return isDug;
}

int foundAll(struct treasure *treasures, int num){
    int found = true;
    for (i = 0; i < num; i++) {
        if(treasures[i].found == false){
            found = false;
            break;
        }
    }
    return found;
}
void digMap(struct treasure *treasures, int width, int height, int num, char **map){
    int row,col;
    int rowResult, colResult;
    int enterAgain = 1;
    int foundTreasure = false;
    printMap(map, width, height, num);
    do {
        printf("Where would you like to dig?\n");
        printf("x = ");
        colResult = scanf("%d", &col);
        checkEOF(colResult, map, height, treasures);
        checkStdin();
        printf("y = ");
        rowResult = scanf("%d", &row);
        checkEOF(rowResult, map, height, treasures);
        checkStdin();
        if (col < 0 || col >= width || row < 0 || row >= height || rowResult != 1 || colResult != 1) {
            printf("x or y should be equal or larger than 0.\n");
        } else {
            foundTreasure = false; 
            for (i = 0; i < num; i++) {
                if (col == treasures[i].col && row == treasures[i].row) {
                    if(treasures[i].found != true){
                        foundTreasure = true; 
                        printf("Congratulations, you found a treasure! (code: %s)\n", treasures[i].code);
                        map[row][col] = '$';
                        treasures[i].found = true;
                        enterAgain = 0;
                        break;
                    }
                    
                }
            }
            if (!foundTreasure) {
                if (map[row][col] == ' ') {
                    map[row][col] = 'X';
                    printf("You found nothing there!\n");
                    enterAgain = 0;
                } else if (map[row][col] == 'X') {
                    printf("The place was already dug.\n");
                } else if (map[row][col] == '$') {
                    printf("The treasure was already dug from that place.\n");
                } else if (map[row][col] == '!') {
                    printf("You found nothing there!\n");
                    enterAgain = 0;
                }
            }
        }
    } while (!enterAgain == 0);
    
}
void cheat(struct treasure *treasures, int width, int height, int num, char **map){
    int col,row;
    for (i = 0; i < num; i++) {
        col = treasures[i].col;
        row = treasures[i].row;
        if(map[row][col] != '$'){
             map[row][col] = '!';
        }else{
            continue;
        }
       
    }
    printMap(map, width, height, num);
}


void displayMenu(int width, int height, int num, struct treasure *treasures, char **map){
    int option;
    int result;
    int cheating = 0;
    
    while(1){
        if(foundAll(treasures, num)== true){
            printf("Congratulations, you found all the treasures!\n");
            free(treasures);
            freeMemory(map, height);
            exit(false);
            
            break;
        }
        printf("1 - Dig at a spot  | 2 - Exit Game      | 3 - Cheat!        \n\n");
        if(cheating == 0){
            printf("-> ");
        }else{
            printf("CHEAT-> ");
        }
        
        result = scanf("%d", &option);
        checkEOF(result, map ,height, treasures);

        switch(option){
        case 1:
            digMap(treasures, width, height, num, map);
            break;

        case 2:
            cheating = 0;
            free(treasures);
            freeMemory(map, height);
            exit(false);
            break;

        case 3: 
            cheating = 1;
            if(cheating == 1){
                printf("Cheat mode already enabled!\n");
            }
            cheat(treasures, width, height, num, map);
            break;

        default:
            printf("Please enter number 1 to 3.\n");
            checkStdin();
            break;
        }

    }
    
}

int main(){
    int width, height;
    int treasureNum;
    struct treasure *treasures;
    char **map;

    width = inputLength("Enter map width:");
    height = inputLength("Enter map height:");
    map = allocateArray(width, height);
    treasureNum = inputTeasureNum(width,height,map);
    treasures = malloc(treasureNum * sizeof(struct treasure));
    inputTreasureCode(treasures, treasureNum, map, height);
    randomTreasures(treasures, treasureNum, width, height);
    
    displayMenu(width, height,treasureNum, treasures, map);
    
    return 0;
}
