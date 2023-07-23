/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "basic_io.h"


#include "system.h"


/* OLD BASIC_IO_H */
extern void __asm_print_char_at(char c, u32 col, u32 row);
extern void __asm_print_char_at_color(char c, u32 col, u32 row, char color);


void print_char_at(char c, u32 col, u32 row)
{
    __asm_print_char_at(c, col, row);
}

void print_char_at_color(char c, u32 col, u32 row, char color)
{
    __asm_print_char_at_color(c, col, row, color);
}

void print_string_at(char *str, u32 col, u32 row)
{
    while(*str)
    {
        print_char_at(*str, col, row);
        str++;
        col++;
    }
}
/* END BASIC_IO_H */

static u8 cursor_x = 0;
static u8 cursor_y = 0;
static u16 *video_memory = (u16 *) 0xB8000;


static void move_cursor()
{
   // The screen is 80 characters wide...
   u16 cursorLocation = cursor_y * 80 + cursor_x;

   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

static u8 get_attribute_byte(u8 bg, u8 fg)
{
    return (bg << 4) | (fg & 0x0F); // possibl
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

   // Get a space character with the default colour attributes.
   u8 attributeByte = get_attribute_byte(0x0 /* black */, 0xF /* white */);
   u16 blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= 25)
   {
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           video_memory[i] = video_memory[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++)
       {
           video_memory[i] = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = 24;
   }
}

// Writes a single character out to the screen.
void putc(char c)
{
   // The background colour is black (0), the foreground is white (15).
   u8 backColour = 0;
   u8 foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   u8  attributeByte = get_attribute_byte(0x0 /* black */, 0xF /* white */);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   u16 attribute = attributeByte << 8;
   u16 *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1);
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = video_memory + (cursor_y*80 + cursor_x);
       *location = c | attribute;
       cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y ++;
   }

   // Scroll the screen if needed.
   scroll();
   // Move the hardware cursor.
   move_cursor();
}

void puts(char *s)
{
    int i = 0;
    while(s[i])
    {
        putc(s[i++]);
    }
}

void puti(u32 n)
{

    if (n == 0)
    {
        putc('0');
        return;
    }

    i32 acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    puts(c2);
}

void clrscn()
{
   // Make an attribute byte for the default colours
   u8 attributeByte = get_attribute_byte(0x0 /* black */, 0xF /* white */);
   u16 blank = 0x20 /* space */ | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++)
   {
       video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}