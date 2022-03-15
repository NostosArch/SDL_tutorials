//Creating a Window using SDL

#include "SDL2/SDL.h"
#include<iostream>

const unsigned int screen_width = 640;
const unsigned int screen_height = 480;

int main(int argc, char *args[]){

	SDL_Window *window = NULL; // Creating a SDL window pointer
	SDL_Surface *screen_surface = NULL; //Surface contained by the window, Basically a 2D image seen in the window

	//Once the pointers are created it's time to initialize the SDL
	
       if(SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL_INIT_VIDEO says the SDL is used for Video purposes
		std::cout<<"Error "<<SDL_GetError()<<std::endl;
		std::exit(1);
       } else {
		//Create Window 
		window = SDL_CreateWindow("SDL Tutorial", 50, 50, screen_width, screen_height, SDL_WINDOW_SHOWN);
		//params : title, x pos, y pos, screen width, screen height, WINDOW_SHOWN shows the window on creation
	
		if(window == NULL){
			std::cout<<"Error Creating Window"<<SDL_GetError()<<std::endl;
			std::exit(1);
		}

		//Now that window has been created we have to draw on it, so get window surface
		screen_surface = SDL_GetWindowSurface(window);

		//Filling  the surface with white screen
		SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
		
		//Now that you have drawn something on the screen surface you have to update the window

	        SDL_UpdateWindowSurface(window);

		//Let the window stay for 5s before disappearing
		//Note : When the SDL is Delaying, even though the Window is visible, it cant accept input from keyboard or mouse
		SDL_Delay(5000);	

		//Destroy Window to free up memory

		SDL_DestroyWindow(window);

		//Quit all SDL Subsystems
		SDL_Quit();
       }
	
       return 0; 
}
