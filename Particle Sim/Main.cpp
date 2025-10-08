#include <SDL3/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "particleSystem.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Criar janela
    SDL_Window* window = SDL_CreateWindow("Simulador de Particulas",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_MAXIMIZED);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Criar renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "");
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Inicializar random
    std::srand(std::time(nullptr));

    // Criar sistema de partículas
    ParticleSystem particleSystem;
    particleSystem.setGravity(98.0f);

    // Adicionar algumas partículas iniciais
    for (int i = 0; i < 50; ++i) {
        float x = 100 + static_cast<float>(std::rand()) / RAND_MAX * 600;
        float y = 100 + static_cast<float>(std::rand()) / RAND_MAX * 400;
        float mass = 1.0f + static_cast<float>(std::rand()) / RAND_MAX * 3.0f;
        float radius = 3.0f + static_cast<float>(std::rand()) / RAND_MAX * 7.0f;

        Particle p(x, y, mass, radius);

        // Cor aleatória
        p.color = {
            static_cast<Uint8>(std::rand() % 256),
            static_cast<Uint8>(std::rand() % 256),
            static_cast<Uint8>(std::rand() % 256),
            255
        };

        particleSystem.addParticle(p);
    }

    // Loop principal
    bool running = true;
    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;

    while (running) {
        // Calcular delta time
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Processar eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // Adicionar partícula no clique do mouse
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                Particle p(mouseX, mouseY,
                    1.0f + static_cast<float>(std::rand()) / RAND_MAX * 3.0f,
                    3.0f + static_cast<float>(std::rand()) / RAND_MAX * 7.0f);

                p.color = {
                    static_cast<Uint8>(std::rand() % 256),
                    static_cast<Uint8>(std::rand() % 256),
                    static_cast<Uint8>(std::rand() % 256),
                    255
                };

                // Dar um impulso inicial
                p.vx = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 200.0f;
                p.vy = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 200.0f;

                particleSystem.addParticle(p);
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                case SDLK_C:
                    particleSystem.clear();
                    break;
                case SDLK_G:
                    particleSystem.setEnableGravity(!particleSystem.getParticles().empty());
                    break;
                case SDLK_R:
                    // Adicionar partículas aleatórias
                    for (int i = 0; i < 20; ++i) {
                        float x = static_cast<float>(std::rand()) / RAND_MAX * SCREEN_WIDTH;
                        float y = static_cast<float>(std::rand()) / RAND_MAX * SCREEN_HEIGHT;
                        Particle p(x, y,
                            1.0f + static_cast<float>(std::rand()) / RAND_MAX * 3.0f,
                            3.0f + static_cast<float>(std::rand()) / RAND_MAX * 7.0f);

                        p.color = {
                            static_cast<Uint8>(std::rand() % 256),
                            static_cast<Uint8>(std::rand() % 256),
                            static_cast<Uint8>(std::rand() % 256),
                            255
                        };

                        particleSystem.addParticle(p);
                    }
                    break;
                }
            }
        }

        // Atualizar sistema
        particleSystem.update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Renderizar
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        particleSystem.render(renderer);

        SDL_RenderPresent(renderer);

        // Limitar FPS
        SDL_Delay(16); // ~60 FPS
    }

    // Limpar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}