#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define FPS 60

void selection_sort(int *array, int n, int i);
void shuffle_array(int *array, int n);
void swap(int *a, int *b);

int main(int argc, char *argv[]) {
    srand(time(0));
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // atoi() is bad, change later
    if (n < 0) {
        fprintf(stderr, "Number of items must be positive\n");
        return 1;
    }

    // start sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initalizing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Sort",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // allocate, fill and shuffle array
    int *array = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        array[i] = i+1;
    }
    
    shuffle_array(array, n);
    
    // main loop
    int i = 0; // number of loops
    int quit = 0;
    while (!quit) {
        int start_tick = SDL_GetTicks();
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }

        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < n; i++) {
            SDL_Rect rect = {
                .x = i * (window_width / n),
                .y = (window_height) - (array[i] * (window_height / n)),
                .w = window_width / n,
                .h = window_height
            };
            
            SDL_RenderFillRect(renderer, &rect);
        }
        
        SDL_RenderPresent(renderer);

        // sort array by one step
        selection_sort(array, n, i);
        i++;

        SDL_Delay(50);
        
        if ((1000/FPS) > SDL_GetTicks() - start_tick) {
            SDL_Delay(1000/FPS - (SDL_GetTicks() - start_tick));
        }
    }

    // clean up
    free(array);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

void selection_sort(int *array, int n, int i) {
    int j, min;
    
    min = i;
    for (j = i + 1; j < n; j++) {
        if (array[j] < array[min]) {
            min = j;
        }
    }

    swap(&array[min], &array[i]);
}

void shuffle_array(int *array, int n) {
     for (int i = n - 1; i > 0; i--) {
         int j = rand() % (i + 1);
         swap(&array[j], &array[i]);
     }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

