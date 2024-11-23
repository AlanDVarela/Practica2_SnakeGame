#include "ripes_system.h"

void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void move_snake(unsigned int x, unsigned int y, unsigned int ant_x, unsigned int ant_y);

void main(void)
{
    unsigned int *up = D_PAD_0_UP;
    unsigned int *down = D_PAD_0_DOWN;
    unsigned int *left = D_PAD_0_LEFT;
    unsigned int *right = D_PAD_0_RIGHT;

    unsigned int x = 17; // Coordenada inicial x
    unsigned int y = 12; // Coordenada inicial y

    unsigned int ant_x = x; // Para guardar la pos anterior
    unsigned int ant_y = y;

    unsigned int key_up_pressed = 0;
    unsigned int key_down_pressed = 0;
    unsigned int key_left_pressed = 0;
    unsigned int key_right_pressed = 0;

    create_snake(x, y);

    while (1)
    {
        if (*up == 1 && key_up_pressed == 0)
        {
            key_up_pressed = 1;
            ant_x = x; 
            ant_y = y;
            y -= 1;    // Mover hacia arriba
            move_snake(x, y, ant_x, ant_y);
        }
        else if (*down == 1 && key_down_pressed == 0)
        {
            key_down_pressed = 1;
            ant_x = x;
            ant_y = y;
            y += 1; // Mover hacia abajo
            move_snake(x, y, ant_x, ant_y);
        }
        else if (*left == 1 && key_left_pressed == 0)
        {
            key_left_pressed = 1;
            ant_x = x;
            ant_y = y;
            x -= 1; // Mover hacia la izquierda
            move_snake(x, y, ant_x, ant_y);
        }
        else if (*right == 1 && key_right_pressed == 0)
        {
            key_right_pressed = 1;
            ant_x = x;
            ant_y = y;
            x += 1; // Mover hacia la derecha
            move_snake(x, y, ant_x, ant_y);
        }

        // Liberar teclas
        if (*up == 0)
        {
            key_up_pressed = 0;
        }
        if (*down == 0)
        {
            key_down_pressed = 0;
        }
        if (*left == 0)
        {
            key_left_pressed = 0;
        }
        if (*right == 0)
        {
            key_right_pressed = 0;
        }
    } // while
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned int *led_base = LED_MATRIX_0_BASE;
    unsigned int offset = x + y * LED_MATRIX_0_WIDTH;
    unsigned int *address = led_base + offset;
    *address = color;
}

void create_snake(unsigned int x, unsigned int y)
{
    // Dibujar la serpiente inicial 
    set_pixel(x, y, 0x00ff0000);
    set_pixel(x + 1, y, 0x00ff0000);
    set_pixel(x, y + 1, 0x00ff0000);
    set_pixel(x + 1, y + 1, 0x00ff0000);
}

void move_snake(unsigned int x, unsigned int y, unsigned int ant_x, unsigned int ant_y)
{
    // Quitar area anterior serpiente
    set_pixel(ant_x, ant_y, 0x00000000);         // Parte superior izquierda
    set_pixel(ant_x + 1, ant_y, 0x00000000);     // Parte superior derecha
    set_pixel(ant_x, ant_y + 1, 0x00000000);     // Parte inferior izquierda
    set_pixel(ant_x + 1, ant_y + 1, 0x00000000); // Parte inferior derecha

    // Dibujar la nueva posición de la serpiente (2x2)
    set_pixel(x, y, 0x00ff0000);         // Parte superior izquierda
    set_pixel(x + 1, y, 0x00ff0000);     // Parte superior derecha
    set_pixel(x, y + 1, 0x00ff0000);     // Parte inferior izquierda
    set_pixel(x + 1, y + 1, 0x00ff0000); // Parte inferior derecha
}
