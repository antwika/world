#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

int main(void) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *fontSurface;
    SDL_Texture *fontTexture;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 640, 480, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 2;
    }

    if (!SDL_SetRenderLogicalPresentation(renderer, 160, 120, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set logical rendering resolution: %s", SDL_GetError());
        return 3;
    }

    surface = SDL_LoadBMP("assets/sprite.bmp");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 4;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 5;
    }
    if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set texture scale mode to nearest neighbor: %s", SDL_GetError());
        return 6;
    }
    SDL_DestroySurface(surface);

    fontSurface = SDL_LoadPNG("assets/font.png");
    if (!fontSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create font surface from image: %s", SDL_GetError());
        return 7;
    }

    if (SDL_MUSTLOCK(fontSurface)) {
        SDL_LockSurface(fontSurface);
    }
    
    const SDL_PixelFormatDetails* pixelDetails = SDL_GetPixelFormatDetails(fontSurface->format);
    Uint8 *base = (Uint8 *)fontSurface->pixels;
    Uint32 *row = (Uint32 *)(base + fontSurface->pitch * 1);

    int charIndex = 0;
    int charWidth = 0;
    int charHeight = fontSurface->h - 2;

    int characterOffsets[128];
    int characterWidths[128];

    for (int x = 1; x < fontSurface->w; x++) {
        Uint32 pixel = row[x];

        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel,
            pixelDetails,
            NULL,
            &r, &g, &b, &a);

        if (r == 128 && g == 128 && b == 128 && a == 255) {
            characterOffsets[charIndex] = x - charWidth;
            characterWidths[charIndex] = charWidth;
            charWidth = 0;
            charIndex += 1;
        } else {
            charWidth += 1;
        }
    }

    if (SDL_MUSTLOCK(fontSurface)) {
        SDL_UnlockSurface(fontSurface);
    }

    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    if (!fontTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create font texture from surface: %s", SDL_GetError());
        return 8;
    }
    if (!SDL_SetTextureScaleMode(fontTexture, SDL_SCALEMODE_NEAREST)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set font texture scale mode to nearest neighbor: %s", SDL_GetError());
        return 9;
    }
    SDL_DestroySurface(fontSurface);

    std::string str = "Hello world!";

    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);

        int rx = 0;
        for (size_t i = 0; i < str.size(); i++) {
            int ch = str[i];
            SDL_FRect srcRect = { (float)characterOffsets[ch], 1, (float)characterWidths[ch], 9 };
            SDL_FRect dstRect = { (float)rx, 0, (float)characterWidths[ch], 9 };
            SDL_RenderTexture(renderer, fontTexture, &srcRect, &dstRect);
            rx += characterWidths[ch] + 1;
        }
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(fontTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
