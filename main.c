#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 60

int main(int argc, char *argv[]) {
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
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
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

    // allocate and fill array
    int *array = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        array[i] = i+1;
    }
    
    // print array
    /*for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");*/
    
    // main loop
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < n; i++) {
            SDL_Rect rect = {
                .x = i * (WINDOW_WIDTH / n),
                .y = (WINDOW_HEIGHT) - (array[i] * (WINDOW_HEIGHT / n)),
                .w = WINDOW_WIDTH / n,
                .h = WINDOW_HEIGHT
            };
            
            SDL_RenderFillRect(renderer, &rect);
        }
        
        SDL_RenderPresent(renderer);

        if ((1000/FPS) > SDL_GetTicks() - start_tick) {
            SDL_Delay(1000/FPS - (SDL_GetTicks() - start_tick));
        }
    }

    // clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
