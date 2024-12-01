/*
Alan Varela
30 - Nov - 2024
Practica 2 - Juego Snake

*/


#include "ripes_system.h"
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_WIDTH 35
#define MATRIX_HEIGHT 25
#define MAX_SNAKE_LENGTH 20 // Longitud máxima de la serpiente

// Variables globales para la serpiente
unsigned int snake_length = 1; // Longitud inicial en segmentos (cada segmento es 2x2)
unsigned int snake_x[MAX_SNAKE_LENGTH]; // Coordenadas X de cada segmento
unsigned int snake_y[MAX_SNAKE_LENGTH]; // Coordenadas Y de cada segmento

// Coordenadas de la manzana
unsigned int apple_x;
unsigned int apple_y;

// Funciones
void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void move_snake(unsigned int new_x, unsigned int new_y);
void create_apple();
void grow_snake();
int check_collision_with_apple(unsigned int head_x, unsigned int head_y, unsigned int apple_x, unsigned int apple_y);
int check_self_collision();
void reset_game();

void main(void) {
    // Posiciones iniciales
    unsigned int x = rand() % (MATRIX_WIDTH / 2) * 2; // Coordenada aleatoria alineada a pasos de 2
    unsigned int y = rand() % (MATRIX_HEIGHT / 2) * 2; // Coordenada aleatoria alineada a pasos de 2
    apple_x = rand() % (MATRIX_WIDTH / 2) * 2; // Coordenada aleatoria para la manzana
    apple_y = rand() % (MATRIX_HEIGHT / 2) * 2; // Coordenada aleatoria para la manzana

    // Dirección de memoria del D-PAD y Switch reset
    unsigned int *up = (unsigned int *)D_PAD_0_UP;
    unsigned int *down = (unsigned int *)D_PAD_0_DOWN;
    unsigned int *left = (unsigned int *)D_PAD_0_LEFT;
    unsigned int *right = (unsigned int *)D_PAD_0_RIGHT;
    unsigned int *reset = (unsigned int *)SWITCHES_0_BASE; // Switch reset

    unsigned int key_up_pressed = 0;
    unsigned int key_down_pressed = 0;
    unsigned int key_left_pressed = 0;
    unsigned int key_right_pressed = 0;

    // Crear serpiente inicial y manzana
    create_snake(x, y);
    create_apple();

    while (1) 
    {
        if (*reset == 1) 
        { // Si el botón de reinicio es presionado
            reset_game();
        }

        // Verificar entrada del D-PAD
        if (*up == 1 && key_up_pressed == 0) {
            key_up_pressed = 1;
            if (y < MATRIX_HEIGHT - 3) { // Movimiento hacia arriba
                y += 2;
                move_snake(x, y);
            }
        } 
        else if (*down == 1 && key_down_pressed == 0) {
            key_down_pressed = 1;
            if (y > 1) { // Movimiento hacia abajo
                y -= 2;
                move_snake(x, y);
            }
        } 
        else if (*left == 1 && key_left_pressed == 0) {
            key_left_pressed = 1;
            if (x > 1) { // Movimiento hacia la izquierda
                x -= 2;
                move_snake(x, y);
            }
        } 
        else if (*right == 1 && key_right_pressed == 0) {
            key_right_pressed = 1;
            if (x < MATRIX_WIDTH - 3) { // Movimiento hacia la derecha
                x += 2;
                move_snake(x, y);
            }
        }

        // Detectar colision con la manzana
        if (check_collision_with_apple(snake_x[0], snake_y[0], apple_x, apple_y)) 
        {
            grow_snake(); // Crecer la serpiente
            create_apple(); // Crear nueva manzana
        }

        // Detectar colision consigo mismo
        if (check_self_collision()) {
            reset_game(); // Reiniciar el juego si se toca a si mismo
        }

        // Liberar teclas
        if (*up == 0) key_up_pressed = 0;
        if (*down == 0) key_down_pressed = 0;
        if (*left == 0) key_left_pressed = 0;
        if (*right == 0) key_right_pressed = 0;
    }
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color) 
{
    unsigned int *led_base = LED_MATRIX_0_BASE;
    unsigned int offset = x + (MATRIX_HEIGHT - 1 - y) * LED_MATRIX_0_WIDTH;
    unsigned int *address = led_base + offset;
    *address = color;
}

void create_snake(unsigned int x, unsigned int y) 
{
    snake_length = 1; // Longitud inicial de la serpiente
    snake_x[0] = x;
    snake_y[0] = y;

    // Dibujar el segmento inicial (2x2)
    set_pixel(x, y, 0x00ff0000);
    set_pixel(x + 1, y, 0x00ff0000);
    set_pixel(x, y + 1, 0x00ff0000);
    set_pixel(x + 1, y + 1, 0x00ff0000);
}

void move_snake(unsigned int new_x, unsigned int new_y) 
{
    // Borrar la cola de la serpiente
    set_pixel(snake_x[snake_length - 1], snake_y[snake_length - 1], 0x00000000);
    set_pixel(snake_x[snake_length - 1] + 1, snake_y[snake_length - 1], 0x00000000);
    set_pixel(snake_x[snake_length - 1], snake_y[snake_length - 1] + 1, 0x00000000);
    set_pixel(snake_x[snake_length - 1] + 1, snake_y[snake_length - 1] + 1, 0x00000000);

    // Desplazar segmentos hacia adelante
    for (unsigned int i = snake_length - 1; i > 0; i--) 
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Actualizar la cabeza
    snake_x[0] = new_x;
    snake_y[0] = new_y;

    // Dibujar la nueva posición de la cabeza (2x2)
    set_pixel(new_x, new_y, 0x00ff0000);
    set_pixel(new_x + 1, new_y, 0x00ff0000);
    set_pixel(new_x, new_y + 1, 0x00ff0000);
    set_pixel(new_x + 1, new_y + 1, 0x00ff0000);
}

void create_apple() 
{
    // Generar nueva posición de la manzana
    int valid_position = 0;

    while (!valid_position) 
    {
        valid_position = 1;
        apple_x = rand() % (MATRIX_WIDTH / 2) * 2;
        apple_y = rand() % (MATRIX_HEIGHT / 2) * 2;

        // Verificar que la manzana no se superponga con la serpiente
        for (unsigned int i = 0; i < snake_length; i++) 
        {
            if (check_collision_with_apple(snake_x[i], snake_y[i], apple_x, apple_y))
             {
                valid_position = 0;
                break;
            }
        }
    }

    // Dibujar manzana (2x2 en color verde)
    set_pixel(apple_x, apple_y, 0x0000ff00);
    set_pixel(apple_x + 1, apple_y, 0x0000ff00);
    set_pixel(apple_x, apple_y + 1, 0x0000ff00);
    set_pixel(apple_x + 1, apple_y + 1, 0x0000ff00);
}

void grow_snake() 
{
    if (snake_length < MAX_SNAKE_LENGTH) 
    {
        unsigned int new_x = snake_x[snake_length - 1];
        unsigned int new_y = snake_y[snake_length - 1];

        // Ajustar nueva posición del segmento
        if (snake_x[snake_length - 2] < new_x) new_x += 2;
        else if (snake_x[snake_length - 2] > new_x) new_x -= 2;
        else if (snake_y[snake_length - 2] < new_y) new_y += 2;
        else if (snake_y[snake_length - 2] > new_y) new_y -= 2;

        // Agregar nuevo segmento
        snake_x[snake_length] = new_x;
        snake_y[snake_length] = new_y;
        snake_length++;
    }
}

int check_collision_with_apple(unsigned int x, unsigned int y, unsigned int apple_x, unsigned int apple_y) {
    return ((x == apple_x && y == apple_y) ||
            (x + 1 == apple_x && y == apple_y) ||
            (x == apple_x && y + 1 == apple_y) ||
            (x + 1 == apple_x && y + 1 == apple_y));
}

int check_self_collision() {
    for (unsigned int i = 1; i < snake_length; i++) 
    {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) 
        {
            return 1; // Se toco a sí misma
        }
    }
    return 0;
}

void reset_game() {
    for (unsigned int i = 0; i < MATRIX_WIDTH; i++) 
    {
        for (unsigned int j = 0; j < MATRIX_HEIGHT; j++) 
        {
            set_pixel(i, j, 0x00000000); // Borrar toda la matriz
        }
    }

    // Reiniciar la serpiente y la manzana
    main();
}
