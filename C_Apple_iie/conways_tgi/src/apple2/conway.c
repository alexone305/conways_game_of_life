#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <peekpoke.h>
#include <apple2.h>
#include <modload.h>
#include <tgi.h>


/*****************************************************************************/
/*                                   Constants                               */
/*****************************************************************************/
# define ARR_SIZE 40
# define LOWSCR 0xC054 // Text Page 1
# define HISCR 0xC055 // Text Page 2
# define TXTCLR 0xC050 // Display graphics mode
# define MIXCLR 0xC052
# define LORES 0xC056
#define COLOR_BACK      TGI_COLOR_BLACK
#define COLOR_FORE      TGI_COLOR_WHITE

// Row addresses
const int page0_row[24]= {0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700, 
0x0780, 0x0428, 0x04A8, 0x0528, 0x05A8, 0x0628, 0x06A8, 0x0728, 0x07A8, 
0x0450, 0x04D0, 0x0550, 0x05D0, 0x0650, 0x06D0, 0x0750, 0x07D0};
const int page1_row[24]= {0x0800, 0x0880, 0x0900, 0x0980, 0x0A00, 0x0A80, 0x0B00, 
0x0B80, 0x0828, 0x08A8, 0x0928, 0x09A8, 0x0A28, 0x0AA8, 0x0B28, 0x0BA8, 
0x0850, 0x08D0, 0x0950, 0x09D0, 0x0A50, 0x0AD0, 0x0B50, 0x0BD0};

/* Driver stuff */
static unsigned MaxX;
static unsigned MaxY;
static unsigned AspectRatio;
extern void a2e_hi[];
static const unsigned char Palette[2] = { TGI_COLOR_WHITE, TGI_COLOR_BLACK };
// Maximum row and col indices
const unsigned char maxRow = 24;
const unsigned char maxCol = 40;

// Prototypes
void print_2d_array(unsigned char array[][ARR_SIZE], unsigned char rows, unsigned char cols);
void random_and_print(unsigned char rows, unsigned char cols);
void clear_screen(unsigned char rows, unsigned char cols);
void next_gen(unsigned char current[][ARR_SIZE], unsigned char next[][ARR_SIZE], unsigned char rows, unsigned char cols);
unsigned char check_cell(const unsigned char array[][ARR_SIZE], const unsigned char index_row, const unsigned char index_col);
static void CheckError (const char*);
static void DoTestPat (void);
void draw_pixel(unsigned char row, unsigned char col, unsigned char color);

// Global variables
unsigned char i = 0;
unsigned char j = 0;
unsigned char view_page = 0;
unsigned char draw_page = 1;

unsigned char test_arr[][ARR_SIZE] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                          };

/*****************************************************************************/
/*                                   Code                                    */
/*****************************************************************************/

int main (void)
{
    // Initialize  data     
    unsigned char Border;
    // Setup POKEs 
    //POKE(TXTCLR, 0xFF);
    //POKE(MIXCLR, 0xFF);
    //POKE(LORES, 0xFF);
    tgi_install (a2e_hi);
    CheckError ("tgi_install");
    tgi_init ();
    CheckError ("tgi_init");
    
    /* Get stuff from the driver */
    MaxX = tgi_getmaxx ();
    MaxY = tgi_getmaxy ();
    AspectRatio = tgi_getaspectratio ();


    /* Set the palette, set the border color */
    Border = bordercolor (COLOR_BLACK);

    //Set the draw and display pages
    tgi_setviewpage(0);
    tgi_setdrawpage(1);
    tgi_clear();
    tgi_setdrawpage(0);
    tgi_clear();
    // Wait for user input to seed the RNG
    cgetc();
    srand(PEEK(0xCD));
    while(1)
    {   
        tgi_setviewpage(0);
        tgi_setdrawpage(1);
        cgetc();
        //draw_page = 1;
        //view_page = 0;
        //DoTestPat();
        //cgetc();
        random_and_print(MaxY, MaxX);
        tgi_setviewpage(1);
        tgi_setdrawpage(0);
        cgetc();
        random_and_print(MaxY, MaxX);
        //draw_page = 0;
        //view_page = 1;
        

    }

    /* Uninstall the driver */
    tgi_uninstall ();
    /* Reset the border */
    (void) bordercolor (Border);
    return EXIT_SUCCESS;
}


void random_and_print(unsigned char rows, unsigned char cols)
{
    unsigned char Color = COLOR_WHITE;
    unsigned char arr_i = 0;
    unsigned char arr_j = 0;
    tgi_setpalette (Palette);
    tgi_setcolor (Color);
    tgi_clear ();
    for (i = 0; i <= rows; i = i + 10)
    {
        for (j = 0; j <= cols; j = j + 10)
        {
            if(test_arr[arr_i][arr_j])
                tgi_bar (j, i, j+9, i+9);
            arr_i = arr_i + 1;
        }
        arr_j = arr_j + 1;
    }
    i = 0;
    j = 0;
    return;
}

void draw_pixel(unsigned char row, unsigned char col, unsigned char color)
{
    tgi_setcolor(color);
    tgi_bar(col, row, col+10, row+10);
}

void print_2d_array(unsigned char array[][ARR_SIZE], unsigned char rows, unsigned char cols)
{
    for (i = 0; i < rows; i = i + 1)
    {
        for (j = 0; j < cols; j++)
        {
            /*
            if(array[i][j] == 1)
            {
                POKE(row[i] + j, 0xFF);
            }
            else
            {
                POKE(row[i] + j, 0x00);
            }
            */
           POKE(page0_row[i] + j, array[i][j]);
        }
        //cgetc();
    }
    i = 0;
    j = 0;
    return;
}

static void DoTestPat (void)
{
    unsigned char Color = COLOR_BACK;

    tgi_setpalette (Palette);
    tgi_clear ();

    while (1) {
      tgi_bar(10, 10, 15, 15);
      if (kbhit ()) {
          cgetc ();
          return;
      }
    }
}


static void CheckError (const char* S)
{
    unsigned char Error = tgi_geterror ();

    if (Error != TGI_ERR_OK) {
        printf ("%s: %u\n", S, Error);
        if (doesclrscrafterexit ()) {
            cgetc ();
        }
        exit (EXIT_FAILURE);
    }
}

/*
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
