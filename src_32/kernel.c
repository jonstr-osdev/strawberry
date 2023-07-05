#define ROWS 25
#define COLS 80

void main()
{
    char *mem = (char *) 0xB8000;

    int r, c;
    for(c = 0; c < COLS; c++)
    {
        for(r = 0; r < ROWS; r++)
        {
            mem[(c + COLS * r) * 2] = 'x';
        }
    }

    mem[0] = '-';
    mem[2] = '-';
    mem[4] = '-';
    mem[6] = 'J';
    mem[8] = 'O';
    mem[10]= 'N';
    mem[12]= '-';
    mem[14]= 'S';
    mem[16]= 'T';
    mem[18]= 'R';
    mem[20]= '!';
}