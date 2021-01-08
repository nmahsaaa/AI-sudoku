#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int puzzle[9][9] ;
float chro[6][82] ;
int chroRow = 0 ;
float sscore ;
bool continueAlg = true ;
bool lastGeneration = false ;
float minFitness = 0.01 ;

float cost(int puz[][9])
{
   sscore = 0 ;
    for(int row = 0 ; row < 9 ; row++)
    {
        for(int col = 0 ; col < 9 ; col++ )
        {
            for(int k = 0 ; k < 9 ; k++)
            {
                //check row
                if (puz[row][col] == puz[row][k])
                    if(k != col)
                        sscore ++ ;
                //check columns
                if (puz[row][col] == puz[k][col])
                    if(k != row)
                        sscore++;
            }
            //check in subgrid
            for (int k = (row / 3) * 3; k<(((row / 3) * 3) + 3); k++)
            {
                for (int m = (col / 3) * 3; m<(((col / 3) * 3) + 3); m++)
                {
                    if (puz[row][col] == puz[k][m])
                        if ( m!= col || k != row)
                            sscore ++ ;
                }
            }
        }
    }
    return sscore;
}

void printSudoku()
{
    cout <<"\n\n";
    for (int i = 0; i<9; ++i)
    {
        for (int j = 0; j<9; ++j)
        {
            if(j%3 == 0)
                cout << "|" ;
            cout << puzzle[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "Cost: " << cost(puzzle) <<"\n" ;
   /* for (int i = 0 ; i < chroRow ; i++)
    {
        for (int t = 0 ; t < 82 ; t++)
            cout << chro[i][t] << " " ;
        cout << "\n" ;
    }*/
}

void ChroCostupdate()
{
    cout << "\n________NEXT GENERATION________\n" ;
    for (int i = 0 ; i < 6 ; i++)
    {
        int j = 0 ;
        for(int t = 0 ; t < 9 ; t++)
        {
            for (int y = 0 ; y < 9 ; y++)
            {
                puzzle[t][y] =  chro[i][j];
                j++;
            }
        }
        chro[i][81] = cost(puzzle);
        if(lastGeneration)
            printSudoku();
    }
}

void initialize()
{
    int chroCol = 0 ;
    for (int i = 0 ; i < 9 ; i++)
        for (int j = 0 ; j < 9 ; j++)
            puzzle[i][j]=0;

    for (int row = 0 ; row < 9 ; row+=3) // satr
        for (int col = 0 ; col < 9 ; col+=3) //sotoon
        {
            for (int k = (row / 3) * 3; k<(((row / 3) * 3) + 3); k++) //blocks row
            {
                for (int m = (col / 3) * 3; m<(((col / 3) * 3) + 3); m++) //bloks col
                {
                    int temp = rand()%10 ;
                    while(temp == 0)
                        temp = rand()%10 ;
                    bool isValid = true ;
                    int p = (row / 3) * 3;
                    while (p < (((row / 3) * 3) + 3))
                    {
                        int q = (col / 3) * 3;
                        while(q<(((col / 3) * 3)+3))
                        {
                              if (puzzle[p][q] == temp)
                              {if ( q!= m || p != k)
                                    {
                                      isValid = false;
                                      temp = rand()%10 ;
                                      while(temp == 0)
                                      temp = rand()%10 ;
                                      q = (col / 3) * 3;
                                      p = (row / 3) * 3;
                                    }
                              }
                              else
                                  {
                                      q++;
                                      isValid = true ;
                                  }

                        }
                         p++;
                    }
                    if (isValid == true)
                    {
                        puzzle[k][m] = temp ;
                    }
                }
            }
        }

        while(chroCol != 81)
        {
            for(int t = 0 ; t < 9 ; t++)
            {
                for (int y = 0 ; y < 9 ; y++)
                {
                    chro[chroRow][chroCol] = puzzle[t][y];
                    chroCol++;
                }
            }
        }
    chro[chroRow][81] = cost(puzzle) ;
    chroRow++;
}

void mutation(int p1, int p2) // ba nerkhe 1/5
{
    for(int n = 0 ; n < 16 ; n++)
    {
        int r = rand() % 81 ;
        int MIN_RAND = 1, MAX_RAND = 9;
        int range = MAX_RAND - MIN_RAND;
        int random = range * (((rand()) / RAND_MAX)) + MIN_RAND ;
        chro[p1][r] = random ;
        random = range * (((rand()) / RAND_MAX)) + MIN_RAND ;
        chro[p2][r] = random;
    }
}

void crossOver(int parent1, int parent2)
{
    int breakPoint = rand() % 81 ;
    float myswap[81] ;

    for (int i = 0 ; i < breakPoint ; i++) //swaps the values from break point
    {
        myswap[i] = chro[parent1][i] ;
        chro[parent1][i] = chro[parent2][i];
        chro[parent2][i] = myswap[i];
    }
    mutation(parent1 , parent2);
    ChroCostupdate();
}


void chooseParents()
{
    float fit[6] ;
    float sum = 0 ;
    int parent1 , parent2 ;
    cout<<"\nFitnesses are " ;
    for (int i = 0 ; i <6 ; i++)
    {
        fit[i] = 1 / (1+ chro[i][81]) ;
        sum += fit[i] ;
        cout <<i+1 <<") "<< fit[i]  << "  " ;
        if (fit[i] >= minFitness)
        {
            continueAlg = false ;
            lastGeneration = true ;
        }
    }
    float MIN_RAND = 0, MAX_RAND = sum;
    float range = MAX_RAND - MIN_RAND;
    bool p1valued = false ;
    bool p2valued = false ;
    while(p1valued == false || p2valued == false) //choosing 2 parents
    {
        float random = range * ((((float) rand()) / (float) RAND_MAX)) + MIN_RAND ;
        float min = 0 ;
        float max = fit[0] ;
        for (int j = 0 ; j < 6 ; j++)
        {
            if(random>=min && random <=max)
            {
                if(p1valued == false)
                {
                    parent1 = j ;
                    cout<< "\nParent1: " << parent1 ;
                    p1valued = true ;
                    break;
                }
                else
                {
                    if(j == parent1)
                    {
                        random = range * ((((float) rand()) / (float) RAND_MAX)) + MIN_RAND ;
                        break;
                    }
                    else
                    {
                        parent2 = j ;
                        cout<< "\nParent2: " << parent2 ;
                        p2valued = true;
                        break;
                    }
                }
            }
            min = min + fit[j] ;
            max = max + fit[j+1];
         }
    }
    crossOver(parent1 , parent2) ;
}


int main()
{
    cout << "\n________FIRST GENERATION________\n" ;
    //tashkile jame e avalie ba toole 6
    for(int i = 0 ; i < 6 ; i++)
    {
        initialize();
        printSudoku();
    }
    while (continueAlg)
    {
        chooseParents();
    }
}
