#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <SDL.h>

using namespace std;

#define FPS uint32_t(15)
#define frameDelay uint32_t(1000 / FPS)

class Render
{	
	public:
		//Constructor
		Render();

		//Initialize of the SDL subsystem and variables
		//Returns "true" if everything is right or "false" if not
		bool init(vector <string> path_vector);

		//Renders frames in loop
		void loop();

		//Closing SDL subsystems and dump collecting
		void closeSDL();
		
	private:
		//This method gets handle of wallpaper window
		HWND getWallpaperWindow();

		//Loads images as textures
		SDL_Texture* loadTexture(std::string path);

		//Clear renderer
		void clearRenderer();
		
		//Render an image
		void render();

		//Update number of the chosen texture
		void update();

		void fpsCap(uint32_t starting_tick);

		//The window we'll be rendering to
		SDL_Window* window = nullptr;

		//The window renderer
		SDL_Renderer* renderer = nullptr;

		//An array of textures
		vector <SDL_Texture*> textures;

		//A number of the chosen texture in array
		size_t currNum = 0;
};
