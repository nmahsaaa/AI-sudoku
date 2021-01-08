#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int puzzle[9][9] ;
int childpuzzle[9][9] ;
float sscore ;

void initialize()
{
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
                              {
                                  if ( q!= m || p != k)
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
                        puzzle[k][m] = temp ;
                }
            }
        }
}

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

float chooseChild()
{
    for (int i = 0 ; i < 9 ; i++)
        for(int j = 0 ; j < 9 ; j++)
            childpuzzle[i][j] = puzzle[i][j];

    for (int row = 0 ; row < 9 ; row+=3) // satr
        for (int col = 0 ; col < 9 ; col+=3) //sotoon
        {
            bool goOn = true ;
            while(goOn)
            {
                int k= ((row / 3) * 3)+ (rand() % 3);
                int m= ((col / 3) * 3)+ (rand() % 3);
                int t= ((row / 3) * 3)+ (rand() % 3);
                int y= ((col / 3) * 3)+ (rand() % 3);
                int temp ;
                if(k!=t || m!=y) //swap
                {
                    temp = puzzle[k][m] ;
                    childpuzzle[k][m] = childpuzzle[t][y] ;
                    childpuzzle[t][y] = temp ;
                    goOn = false ;
                }
            }
        }
    return cost(childpuzzle) ;
}

bool acceptance(int fcost , int ccost)
{
    float temp = 50;
    float delta ;
    delta = (ccost - fcost) / temp;
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    cout << "\n(random: " << r << "  r: " << exp(-delta) <<")\n" ;
    if (r > exp(-delta))
        return false ;
    else
        return true ;
}

void printSudoku()
{
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
}


int main()
{
    int algCount = 0 ;
    int fatherCost ;
    int childCost;
    //initializing
    initialize();
    cout <<  "\n Before... \n" ;
    printSudoku();
    fatherCost = cost(puzzle);

    cout << "Fitness: "<< fatherCost << "\n \n";
    while(algCount <= 20)
    {
        algCount ++;
        childCost = chooseChild() ;
        if (childCost < fatherCost)
        {//entekhab ghatii
            for (int i = 0 ; i < 9 ; i++)
                for(int j = 0 ; j < 9 ; j++)
                    puzzle[i][j] = childpuzzle[i][j];
            printSudoku();
            fatherCost = cost(puzzle);
            cout << "Fitness: " << fatherCost << "\n \n";
        }
        else
        {
            if(acceptance(fatherCost , childCost)) //entekhab ghatii
            {
                cout << "THE CHOSEN CHILD: \n";
                for (int i = 0 ; i < 9 ; i++)
                    for(int j = 0 ; j < 9 ; j++)
                        puzzle[i][j] = childpuzzle[i][j];
                printSudoku();
                fatherCost = cost(puzzle);
                cout<< "Fitness: " << fatherCost << "\n \n";
            }
            else
                cout <<"THE CHILD DIDNT GET CHOSEN \n" ;
        }
    }
}
