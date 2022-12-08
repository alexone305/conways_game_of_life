# include <stdio.h>

// Constants
# define ARR_SIZE 10
// Globals


// Prototypes
void print_2d_array(char array[][ARR_SIZE], int rows, int cols);
void next_gen(char current[][ARR_SIZE], char next[][ARR_SIZE], int rows, int cols);
char check_cell(const char array[][ARR_SIZE], const int index_row, const int index_col);

// Main Function
int main()
{
    // Get number of iterations from the user
    int iterations;
    char input[10];
    printf("Enter the number of iterations: ");
    scanf("%s", input);
    iterations = atoi(input);
    fflush(stdin);
    printf("\n%d Iterations\n\n\n", iterations);
    // Initial array declarations
    // Blinker
    /*
    char gen_a[][ARR_SIZE] = {{0, 0, 0, 0, 0},
                             {0, 0, 1, 0, 0},
                             {0, 0, 1, 0, 0},
                             {0, 0, 1, 0, 0},
                             {0, 0, 0, 0, 0}};
    */
    // Glider
    /*
    char gen_a[][ARR_SIZE] = {{0, 0, 0, 0, 0},
                             {0, 0, 1, 0, 0},
                             {0, 0, 0, 1, 0},
                             {0, 1, 1, 1, 0},
                             {0, 0, 0, 0, 0}};
    */
   char gen_a[][ARR_SIZE] = {{0,1,0,0,0,0,0,0,0,0},
                             {0,1,0,0,0,0,0,0,0,0},
                             {0,1,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0,0},
                             {0,0,1,0,0,0,1,0,0,0},
                             {0,0,1,0,0,0,1,0,0,0},
                             {0,0,1,0,0,0,1,0,0,0},
                             {0,0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0,0}};
    char gen_b[ARR_SIZE][ARR_SIZE];                                          
    // Declare and set up a tracker to flip between two arrays. Was using bool, but wanted to convert to char to eliminate the bool header.
    char pick_a = 0x00;

    for(int i = 0; i < iterations; i++)
    {
        // Print Iteration number
        printf("Iteration %u:\n\n", i);
        // Determine which array to display
        if (pick_a == 0x00)
        {
            // Display current array
            print_2d_array(gen_a, ARR_SIZE, ARR_SIZE);
            // Calculate new array
            next_gen(gen_a, gen_b, ARR_SIZE, ARR_SIZE);
            // Flip boolean
            pick_a = ~pick_a;
            
        }
        else // Repeat but flip the arrays
        {
            // Display current array
            print_2d_array(gen_b, ARR_SIZE, ARR_SIZE);
            // Calculate new array
            next_gen(gen_b, gen_a, ARR_SIZE, ARR_SIZE);
            // Flip boolean
            pick_a = ~pick_a;
        }
        //printf("%s", pick_a ? "true" : "false");

        // Prompt user to press enter to continue
        if (i < iterations - 1)
        {
            printf("Press Enter to continue...");
            while(getchar() != '\n' );
        }
    }

    return 0;
}


void print_2d_array(char array[][ARR_SIZE], int rows, int cols)
{
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        // Format beginning of line
        printf("    ");
        for (int j = 0; j < cols; j++)
        {
            printf("%u  ", array[i][j]);
        }
        // Format end of line
        printf("\n");
    }
    return;
}

// Finds all 8 neighbors of a given cell in an array
// Neighbors defined in this order:
// 0 1 2
// 3 N 4
// 5 6 7
char check_cell(const char array[][ARR_SIZE], const int index_row, const int index_col)
{
    // Declarations
    char neighbors[8];
    char sum_neighbors = 0;
    const char test_cell = array[index_row][index_col];
    // Find neighbors to the left and the right
    neighbors[3] = array[index_row][((index_col - 1 + ARR_SIZE) % ARR_SIZE)];
    neighbors[4] = array[index_row][((index_col + 1 + ARR_SIZE) % ARR_SIZE)];
    // Find neighbors to the top and bottom
    neighbors[1] = array[((index_row - 1 + ARR_SIZE) % ARR_SIZE)][index_col];
    neighbors[6] = array[((index_row + 1 + ARR_SIZE) % ARR_SIZE)][index_col];
    // Find diagonal neighbors
    neighbors[0] = array[((index_row - 1 + ARR_SIZE) % ARR_SIZE)][((index_col - 1 + ARR_SIZE) % ARR_SIZE)];
    neighbors[2] = array[((index_row - 1 + ARR_SIZE) % ARR_SIZE)][((index_col + 1 + ARR_SIZE) % ARR_SIZE)];
    neighbors[5] = array[((index_row + 1 + ARR_SIZE) % ARR_SIZE)][((index_col - 1 + ARR_SIZE) % ARR_SIZE)];
    neighbors[7] = array[((index_row + 1 + ARR_SIZE) % ARR_SIZE)][((index_col + 1 + ARR_SIZE) % ARR_SIZE)];
    // printf("%u %u %u\n%u %u %u\n%u %u %u\n\n", neighbors[0], neighbors[1], neighbors[2], neighbors[3], array[index_row][index_col], neighbors[4],neighbors[5],neighbors[6],neighbors[7]);
    // Add neighbors together to determine how many are alive (assume 1 = alive, 0 = dead)
    for (int i = 0; i < 8; i++)
    {
        sum_neighbors = sum_neighbors + neighbors[i];
    }
    // Rules of the game of life:
    // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    // Any live cell with two or three live neighbours lives on to the next generation.
    // Any live cell with more than three live neighbours dies, as if by overpopulation.
    // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

    //printf("%d", sum_neighbors);
    if (test_cell == 1)
    {
        if(sum_neighbors < 2 || sum_neighbors > 3) // Death by starvation or overpopulation
        {
            return 0;
        }
        else // Must be 2 or 3, so survive
        {
            return 1;
        }
    }
    else // Dead cell, check if new cell is born
    {
        if(sum_neighbors == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

// Calculates the next generation of the current array
void next_gen(char current[][ARR_SIZE], char next[][ARR_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            next[i][j] = check_cell(current, i, j);
            //printf("%u", next[i][j]);
        }
    }
    return;
}