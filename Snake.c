#include "ripes_system.h"
 
void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void move_snake(unsigned int x, unsigned int y, unsigned int ant_x, unsigned int ant_y);
void create_apple(unsigned int x, unsigned int y);
 
 
int check_collision_with_apple(unsigned int x, unsigned int y, unsigned int apple_x, unsigned int apple_y);
 
 
void reset_game(void);
 
 
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
 
int check_collision_with_apple(unsigned int x, unsigned int y, unsigned int apple_x, unsigned int apple_y)
{
    int collision_detected = 0;

    // Verificamos todas las combinaciones de las coordenadas de la cabeza de la serpiente (4 puntos)
    // con todas las combinaciones de las coordenadas de la manzana (4 puntos)

    // Esquinas de la serpiente (x, y, x+1, y+1)
    // Esquinas de la manzana (apple_x, apple_y, apple_x+1, apple_y+1)

    if ((x == apple_x && y == apple_y) || 
    (x + 1 == apple_x && y == apple_y) || 
    (x == apple_x && y + 1 == apple_y) || 
    (x + 1 == apple_x && y + 1 == apple_y)) {
    set_pixel(apple_x, apple_y, 0x00ff0000); // Cambiar a rojo (esquina superior izquierda)
    set_pixel(apple_x + 1, apple_y, 0x00000000); // Cambiar a negro (esquina superior derecha)
    set_pixel(apple_x, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior izquierda)
    set_pixel(apple_x + 1, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior derecha)
    collision_detected = 1;
}

if ((x == apple_x + 1 && y == apple_y) || 
    (x + 1 == apple_x + 1 && y == apple_y) || 
    (x == apple_x + 1 && y + 1 == apple_y) || 
    (x + 1 == apple_x + 1 && y + 1 == apple_y)) {
    set_pixel(apple_x + 1, apple_y, 0x00ff0000); // Cambiar a rojo (esquina superior derecha)
    set_pixel(apple_x, apple_y, 0x00000000); // Cambiar a negro (esquina superior izquierda)
    set_pixel(apple_x + 1, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior derecha)
    set_pixel(apple_x, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior izquierda)
    collision_detected = 1;
}

if ((x == apple_x && y == apple_y + 1) || 
    (x + 1 == apple_x && y == apple_y + 1) || 
    (x == apple_x && y + 1 == apple_y + 1) || 
    (x + 1 == apple_x && y + 1 == apple_y + 1)) {
    set_pixel(apple_x, apple_y + 1, 0x00ff0000); // Cambiar a rojo (esquina inferior izquierda)
    set_pixel(apple_x + 1, apple_y, 0x00000000); // Cambiar a negro (esquina superior derecha)
    set_pixel(apple_x, apple_y, 0x00000000); // Cambiar a negro (esquina superior izquierda)
    set_pixel(apple_x + 1, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior derecha)
    collision_detected = 1;
}

if ((x == apple_x + 1 && y == apple_y + 1) || 
    (x + 1 == apple_x + 1 && y == apple_y + 1) || 
    (x == apple_x + 1 && y + 1 == apple_y + 1) || 
    (x + 1 == apple_x + 1 && y + 1 == apple_y + 1)) {
    set_pixel(apple_x + 1, apple_y + 1, 0x00ff0000); // Cambiar a rojo (esquina inferior derecha)
    set_pixel(apple_x, apple_y, 0x00000000); // Cambiar a negro (esquina superior izquierda)
    set_pixel(apple_x + 1, apple_y, 0x00000000); // Cambiar a negro (esquina superior derecha)
    set_pixel(apple_x, apple_y + 1, 0x00000000); // Cambiar a negro (esquina inferior izquierda)
    collision_detected = 1;
}

    return collision_detected;
}

void reset_game(void)
{
    
}
