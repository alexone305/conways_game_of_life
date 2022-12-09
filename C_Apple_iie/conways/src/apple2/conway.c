#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <peekpoke.h>
#include <apple2.h>
#include <unistd.h> // For sleep



/*****************************************************************************/
/*                                   Constants                               */
/*****************************************************************************/
# define ARR_SIZE 40
# define TXT1 0x0400 // Text Page 1
# define TXT2 0x0800 // Text Page 2
# define TXTCLR 0xC050 // Display graphics mode
# define MIXCLR 0xC052
# define LORES 0xC056
// Row addresses
const int row[24]= {0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700, 
0x0780, 0x0428, 0x04A8, 0x0528, 0x05A8, 0x0628, 0x06A8, 0x0728, 0x07A8, 
0x0450, 0x04D0, 0x0550, 0x05D0, 0x0650, 0x06D0, 0x0750, 0x07D0};
// Maximum row and col indices
const unsigned char maxRow = 47;
const unsigned char maxCol = 39;

// Prototypes
void print_2d_array(unsigned char array[][ARR_SIZE], unsigned char rows, unsigned char cols);
void next_gen(unsigned char current[][ARR_SIZE], unsigned char next[][ARR_SIZE], unsigned char rows, unsigned char cols);
unsigned char check_cell(const unsigned char array[][ARR_SIZE], const unsigned char index_row, const unsigned char index_col);

/*****************************************************************************/
/*                                   Code                                    */
/*****************************************************************************/

int main (void)
{
    // Initialize array data     
    int i = 0;
    unsigned char xCord = 0;
    unsigned char yCord = 0;

    POKE(TXTCLR, 0xFF);
    POKE(MIXCLR, 0xFF);
    POKE(LORES, 0xFF);

    // Wait for user input to seed the RNG
    cgetc();



    // Run in a loop
    while(1)
    {
        POKE((row[yCord] + xCord), 0xFF); // Each byte is half the pixel
        xCord = (xCord + 1) % maxCol;
        yCord = (yCord + 1) % maxRow;

        for (i = 0; i < 1500; i = i + 1);
        if (kbhit ()) 
        {
            cgetc ();
            return 0;
        }
    }
}

/*
void print_2d_array(unsigned char array[][ARR_SIZE], unsigned char rows, unsigned char cols)
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
unsigned char check_cell(const unsigned char array[][ARR_SIZE], const unsigned char index_row, const unsigned char index_col)
{
    // Declarations
    unsigned char neighbors[8];
    unsigned char sum_neighbors = 0;
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
    for (unsigned char i = 0; i < 8; i++)
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
void next_gen(unsigned char current[][ARR_SIZE], unsigned char next[][ARR_SIZE], unsigned char rows, unsigned char cols)
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
*/
