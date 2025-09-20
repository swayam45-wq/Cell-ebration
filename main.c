//Totalistic Cellular Automaton

#include <stdio.h>
#include <stdbool.h>

const int WORLD_SIZE = 65;

typedef struct cell_struct{
    int localSum; // total sum of local cells, [left] + [me] + [right]: possible values are 0-6
    int status;   // this cell's current status: 0, 1, or 2
    int count;    // running accumulated count of this cell's active status for all generations
} cell;

// convert an integer rule (0-2186) to its base-3 representation, 
// stored in reverse order, as an array of 7 status values, i.e. valArray[7] 
//   ex: rule = 777  -> [0121001], since 777  = 1*3^1 + 2*3^2 + 1*3^3 + 1*3^6   
//   ex: rule = 177  -> [0210200], since 177  = 2*3^1 + 1*3^2 + 2*3^4
//   ex: rule = 2040 -> [0210122], since 2040 = 2*3^1 + 1*3^2 + 1*3^4 + 2*3^5 + 2*3^6
//return true if input rule is valid (0-2186)
//return false if input rule is invalid
bool setValArray(int valArray[7], int rule) {

        while (rule >= 0 && rule <= 2186){
            int currNum = rule;
            for (int i = 0; i <7; i++) {
                valArray[i] = currNum%  3;
                currNum = currNum / 3;
            }

            return true;
        }
          
        return false;
}

// update the localSum subitem for each cell in the world array based on the
// current statuses for the nearby [left, me, right] cells
// note: world is periodic/cyclic, with front-to-back and back-to-front wrapping 
void setSums(cell world[WORLD_SIZE]) {
    for (int i = 0; i<WORLD_SIZE; i++) {
        
        if (i == 0) {
            world[i].localSum = world[i].status + world[WORLD_SIZE-1].status + world[i+1].status;
        }
        else if (i == WORLD_SIZE-1) {
            world[WORLD_SIZE-1].localSum = world[WORLD_SIZE-1].status + world[WORLD_SIZE-2].status + world[0].status;
        }
        else {
            world[i].localSum = world[i].status + world[i-1].status + world[i+1].status;
        }
    }
    return;
}

// Evolve the world to the next generation by updating each cell's status value 
// using its localSum subitem (assumed to be updated outside of this function), where
// ruleValArray[7] stores the status values for the 7 possible local sums, in reverse order, 
// e.g. if local sum = 4, the new status value can be found at index 4 of the ruleValArray[].
// This function must also update the count subitems for all cells in world[] 
// with the new generation status values.
// This function returns the total sum of all status values in world[] after the evolution step.
int evolveWorld(cell world[WORLD_SIZE], int ruleValArray[7]) {
    int sum = 0;
    for (int i = 0; i < WORLD_SIZE; i++) {
        int num = world[i].localSum;
        int newStatus = ruleValArray[num];
        world[i].count = world[i].count + newStatus;
        world[i].status = newStatus;
        sum = sum + newStatus;
    }

    return sum;
}


int main() {
    cell world[WORLD_SIZE];
    int starterNum;
    int firstArr[7];

    printf("Welcome to the Totalistic Cellular Automaton!\n");

    printf("Enter the rule # (0-2186): ");
    scanf("%d", &starterNum);
    bool ans = setValArray(firstArr , starterNum);

    while (ans == false) {
        printf("Enter the rule # (0-2186): \n");
        scanf("%d", &starterNum);
        ans = setValArray(firstArr , starterNum);
    }

    printf("The value array for rule #%d is ", starterNum);
    for (int i = 6; i >= 0; i--) {
        printf("%d", firstArr[i]);
    }
    printf("\n");
    
    //      generate the rule's 7-trit (i.e. 7 base-3 digits) status value array,  
    //      print the status value array in correct ternary (i.e. base-3) number order
    
    printf("The evolution of all possible states are as follows:\n");
    printf("Local Sum:   6       5       4       3       2       1       0      \n");
    printf("States:    |+++|   |++-|   |++ |   |+- |   |-- |   |-  |   |   |    \n");
    printf("                   |+-+|   |+ +|   |+ -|   |- -|   | - |            \n");
    printf("                   |-++|   | ++|   |-+ |   | --|   |  -|            \n");
    printf("                           |+--|   | +-|   |+  |                    \n");
    printf("                           |-+-|   |- +|   | + |                    \n");
    printf("                           |--+|   | -+|   |  +|                    \n");
    printf("                                   |---|                            \n");
    printf("New Status: ");
    
       
    for (int i = 6; i >= 0; i--){
        int number = firstArr[i];
        char sign;
        if (number == 0) {
            sign = ' ';
        }
        else if (number == 1) {
            sign = '-';
        }
        else {
            sign = '+';
        }

        if (i==0) {
            printf("|%c|\n", sign);
        } else {
            printf("|%c|     ", sign);
        }

    }   

    //      step for all possible cell neighbor states [left, me, right].
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    int gen;
    printf("Enter the number of generations (1-49): \n");
    scanf("%d", &gen);

    while (gen<=0 || gen>49) {
        printf("Enter the number of generations (1-49): \n");
        scanf("%d", &gen);
    }

    //      from the user, allowing repeated attempts for invalid values.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    int initial;
    char initialSign;
    printf("Enter the value (1 or 2) for the initial active cell: \n");
    scanf("%d", &initial);

    while (initial<=0 || initial>2) {
        printf("Enter the value (1 or 2) for the initial active cell: \n");
        scanf("%d", &initial);
    }
    //      allowing repeated attempts for invalid values. 
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    for (int i = 0; i<WORLD_SIZE; i++) {
        world[i].status = 0;
        world[i].localSum = 0;
        world[i].count = 0;
    }

    if (initial == 1) {
        world[WORLD_SIZE/2].status =  initial;
        initialSign = '-';
    } else {
        world[WORLD_SIZE/2].status = initial;
        initialSign = '+';
    }
    world[WORLD_SIZE/2].count = initial;

    //world[WORLD_SIZE/2].status = 
    printf("Initializing world & evolving...\n");
    printf("                                %c                                 %d ", initialSign, initial);
    //      exact middle of the world, whose status value is set using the 
    //      scanned-in value from the user above. Make sure to also set the 
    //      localSum subitem for each cell of the initial world.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
         

    //      printing the world after each evolution step, where each cell prints as
    //      '+', '-', or ' ' (whitespace) for status value of 2, 1, or 0, respectively.
    //      After the world display, also print out the total sum of all status values.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    //
    //      Ex: world = [00000000000000000020101212111001210011121210102000000000000000000]
    //          display->                  + - -+-+---  -+-  ---+-+- - +                   28 <-display
    //
    //      At the end of ALL evolution steps, print a line under the last generation. 
    //      Ex: world = [02211210000121122111001121121001210012112110011122112100001211220]
    //          display-> ++--+-    -+--++---  --+--+-  -+-  -+--+--  ---++--+-    -+--++  64 <-display
    //          display->_________________________________________________________________<-display
    for (int i = 1; i<gen; i++) {
        setSums(world);
        int Total = evolveWorld(world, firstArr);
        for (int j = 0; j<WORLD_SIZE; j++) {
            int number = world[j].status;
            char sign;
            if (number == 0) {
                sign = ' ';
            }
            else if (number == 1) {
                sign = '-';
            }
            else {
                sign = '+';
            }
            printf("%c", sign);      
        }
        printf(" %d \n", Total );
    }
    printf("_________________________________________________________________\n");

    //  first for loop for first line 
    for (int i = 0; i<WORLD_SIZE; i++) {
        int number = world[i].count;
        if (number == 0){
            printf(" ");
        }
        else if(number > 0 && number < 10) {
            printf(" ");
        }
        else if(number > 9) {
            int tens = number / 10;
            printf("%d", tens);
        }
    }
    printf("\n");

    // Second for loop for seconond line
    for (int j = 0; j<WORLD_SIZE; j++) {
        int secondNum = world[j].count;
        if (secondNum == 0) {
            printf(" ");
        }
        else if (secondNum > 0 && secondNum < 10) {
            printf("%d", secondNum);
        }
        else if (secondNum > 9){
            int ones = secondNum % 10;
            printf("%d", ones);
        }
    }
  
    printf("\n"); 
    return 0;
}
