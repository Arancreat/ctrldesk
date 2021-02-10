#include "render.h"

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    HWND p = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam;

    if (p)
    {
        // Gets the WorkerW Window after the current one.
        *ret = FindWindowExW(NULL, hwnd, L"WorkerW", NULL);
    }
    return true;
}

//Private

HWND Render::getWallpaperWindow()
{
    // Fetch the Progman window
    HWND progman = FindWindowW(L"ProgMan", NULL);
    // Send 0x052C to Progman. This message directs Progman to spawn a 
    // WorkerW behind the desktop icons. If it is already there, nothing 
    // happens.
    SendMessageTimeoutW(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    // We enumerate all Windows, until we find one, that has the SHELLDLL_DefView 
    // as a child. 
    // If we found that window, we take its next sibling and assign it to workerw.
    HWND wallpaper = nullptr;
    EnumWindows(enumWindowsProc, (LPARAM)&wallpaper);
    // Return the handle you're looking for.
    return wallpaper;
}

SDL_Texture* Render::loadTexture(std::string path) 
{
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == nullptr)
    {
        cout << "Unable to load image \"" << path << "\"! SDL Error: " << SDL_GetError() << endl;
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
        {
            cout << "Unable to create texture from \"" << path << "\"! SDL Error: " << SDL_GetError() << endl;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void Render::clearRenderer()
{
    SDL_RenderClear(renderer);
}

void Render::render()
{
    //TODO: Support of multiple screens

    SDL_Rect stretchRect{ 0, 0, 2560, 1440 }; //wtf... TODO: Get width and height of screen from system
    SDL_RenderCopy(renderer, textures[currNum], nullptr, &stretchRect);
    SDL_RenderPresent(renderer);
}

void Render::update()
{
    if (currNum < textures.size() - 1)
    {
        currNum++;
    }
    else
    {
        currNum = 0;
    }
}

void Render::fpsCap(uint32_t starting_tick)
{
    if ((frameDelay) > SDL_GetTicks() - starting_tick)
    {
        SDL_Delay((frameDelay)-(SDL_GetTicks() - starting_tick));
    }
}

//Public

Render::Render()
{
}

bool Render::init(vector <string> path_vector) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        cout << "SDL couldn't initialize: " << SDL_GetError() << endl;
        return false;
    }
    else 
    {
        window = SDL_CreateWindowFrom((void*)getWallpaperWindow());
        if (window == nullptr) 
        {
            cout << "Can't get desktop window: " << SDL_GetError() << endl;
            return false;
        }
        else 
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == nullptr) 
            {
                cout << "Renderer couldn't not be created: " << SDL_GetError() << endl;
                return false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                for (vector <string>::iterator iter = path_vector.begin(); iter != path_vector.end(); ++iter)
                {
                    textures.push_back(loadTexture(*iter));
                    if (textures.front() == nullptr)
                    {
                        cout << "Failed to load texture image." << endl;
                        return false;
                    }
                }
                return true;
            }
        }
    }

}

void Render::loop()
{
    //Starting Tick
    uint32_t startingTick;

    //Loop flag
    bool quit = false;

    //Event handler
    SDL_Event event;

    while (!quit)
    {
        SDL_PollEvent(&event);
        switch (event.type) 
        {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
        }

        startingTick = SDL_GetTicks();

        clearRenderer();

        render();

        update();

        fpsCap(startingTick);
    }
}

void Render::closeSDL()
{
    //Free loaded image
    for (vector <SDL_Texture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        SDL_DestroyTexture(*iter);
        *iter = nullptr;
    }

    //Destroy window    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    //Quit SDL
    SDL_Quit();
}
