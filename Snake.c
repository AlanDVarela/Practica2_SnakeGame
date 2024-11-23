#include "ripes_system.h"

void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void move_snake(unsigned int x, unsigned int y, unsigned int ant_x, unsigned int ant_y);
void create_apple(unsigned int x, unsigned int y);


int check_collision_with_apple(unsigned int x, unsigned int y, unsigned int apple_x, unsigned int apple_y);


void reset_game(void);

#define MATRIX_WIDTH 35
#define MATRIX_HEIGHT 25

void main(void)
{
    //Posiciones iniciales
    unsigned int x = 17; // Coordenada inicial x
    unsigned int y = 12; // Coordenada inicial y
    unsigned int apple_x = 10; // Coordenada inicial de la manzana
    unsigned int apple_y = 8;  // Coordenada inicial de la manzana

    unsigned int ant_x = x; // Para guardar la pos anterior
    unsigned int ant_y = y;
    
     // Dirección de memoria del D-PAD y Switch reset
    unsigned int *up = (unsigned int *)D_PAD_0_UP;
    unsigned int *down = (unsigned int *)D_PAD_0_DOWN;
    unsigned int *left = (unsigned int *)D_PAD_0_LEFT;
    unsigned int *right = (unsigned int *)D_PAD_0_RIGHT;
    unsigned int *reset = (unsigned int *)SWITCHES_0_N; // Switch reset
    
    unsigned int key_up_pressed = 0;
    unsigned int key_down_pressed = 0;
    unsigned int key_left_pressed = 0;
    unsigned int key_right_pressed = 0;

    //Crear serpiente y manzana
    create_snake(x, y);
    create_apple(apple_x, apple_y);

    while (1)
    {
         if (*reset == 1)
        {
            reset_game();
        }
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
        
        // Detectar colisión con la manzana
        if (check_collision_with_apple(x, y, apple_x, apple_y))
        {
            //Manzana en una nueva pos
            apple_x = (apple_x + 5) % MATRIX_WIDTH;
            apple_y = (apple_y + 3) % MATRIX_HEIGHT;
            create_apple(apple_x, apple_y);

         
        }


        //move_snake(x, y, ant_x, ant_y);

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

void create_apple(unsigned int x, unsigned int y)
{
    // Dibujar manzana (2x2 en color verde)
    set_pixel(x, y, 0x0000ff00); 
    set_pixel(x + 1, y, 0x0000ff00); 
    set_pixel(x, y + 1, 0x0000ff00); 
    set_pixel(x + 1, y + 1, 0x0000ff00); 
}

int check_collision_with_apple(unsigned int x, unsigned int y, unsigned int apple_x, unsigned int apple_y) {
    // Detectar colisión entre la serpiente (2x2) y la manzana (2x2)
    int collision_detected = 0;

    // Verificar si cualquier parte de la serpiente toca cualquier parte de la manzana
   if ((x == apple_x && y == apple_y) || 
        (x + 1 == apple_x && y == apple_y) ||
        (x == apple_x && y + 1 == apple_y) ||
        (x + 1 == apple_x && y + 1 == apple_y)) {
        collision_detected = 1;
    }

    // Si hay colisión, borrar la manzana completamente
    if (collision_detected) {
        set_pixel(apple_x, apple_y, 0x00000000);         // Esquina superior izquierda
        set_pixel(apple_x + 1, apple_y, 0x00000000);     // Esquina superior derecha
        set_pixel(apple_x, apple_y + 1, 0x00000000);     // Esquina inferior izquierda
        set_pixel(apple_x + 1, apple_y + 1, 0x00000000); // Esquina inferior derecha
    }

    return collision_detected;
}



void reset_game(void)
{
    
}
