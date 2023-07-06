#define ROWS 25
#define COLS 80

extern void __asm_print_char_at(char c, int col, int row);
extern void __asm_print_char_at_color(char c, int col, int row, char color);

void print_char_at(char c, int col, int row)
{
    __asm_print_char_at(c, col, row);
}

void print_char_at_color(char c, int col, int row, char color)
{
    __asm_print_char_at_color(c, col, row, color);
}

void main()
{
	char color = 0x0;

    int x, y;
    for(y = 0; y < ROWS; y++)
    {
        for(x = 0; x < COLS; x++)
        {
            print_char_at_color('*', x, y, color);
            color++;
            if(color == 0xFF)
            {
                color = 0x0;
            }
        }
    }

    print_char_at_color('J', 3, 0, 0x17);
    print_char_at_color('O', 5, 1, 0x17);
    print_char_at('N', 7, 2); //, 0b00010111);
}