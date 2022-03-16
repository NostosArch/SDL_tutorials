//Getting an Image on Screen

#include "SDL2/SDL.h"
#include<iostream>
#include "SDL2/SDL_image.h"
#include<stdio.h>

const unsigned int screen_width = 640;
const unsigned int screen_height = 480;

class SDL_ImageDisplay {

	SDL_Window *window; // Creating a SDL window pointer
	SDL_Surface *screen_surface; //Surface contained by the window, Basically a 2D image seen in the window
	SDL_Surface *loaded_surface; //New loaded image to display on Surface contained by the window, Basically a 2D image seen in the window
	SDL_Surface *optimized_surface; //New loaded image to display on Surface contained by the window, Basically a 2D image seen in the window
	int img_Flags;

	public:

	bool init(){

	       if(SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL_INIT_VIDEO says the SDL is used for Video purposes
			std::cout<<"Error "<<SDL_GetError()<<std::endl;
			return false;
	       } else {
			//Create Window 
			window = SDL_CreateWindow("SDL Tutorial", 50, 50, screen_width, screen_height, SDL_WINDOW_SHOWN);
			//params : title, x pos, y pos, screen width, screen height, WINDOW_SHOWN shows the window on creation
		
			if(window == NULL){
				std::cout<<"Error Creating Window"<<SDL_GetError()<<std::endl;
				return false;
			}
	       }
	       
	       //Initialize PNG flags for using SDL_image
	       img_Flags = IMG_INIT_PNG;
	       if( !(IMG_Init(img_Flags))) {
			std::cout<<"Error initialzing SDL Image "<<SDL_GetError()<<std::endl;
	       }
	       //Now that window has been created we have to draw on it, so get window surface
	       screen_surface = SDL_GetWindowSurface(window);

	       return true;
	}

	bool loadMedia(){

		//Loading the image into the screen
		loaded_surface = IMG_Load("maxresdefault.jpg");
		
		if(loaded_surface == NULL) {
			std::cout<<"Error loading image "<<IMG_GetError()<<std::endl;
			return false;
		}	
		
		//optimized the loaded according to the screen_surface
		optimized_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);

		//Apply the Image
		SDL_BlitSurface(optimized_surface, NULL, screen_surface, NULL);
		//params : source surface, null, destination surface, null
		//Blitting stamps a copy of the source onto the destination
		//Blit means copy pixels from one part of computer graphical memory to another part
		//Blitting is fast because it only changes element which are changing
		
		//Now that you have drawn something on the screen surface you have to update the window
	        SDL_UpdateWindowSurface(window);
		//We have two buffers front and back for these windows, when we are making changes to the screen surface we are manipulating the back surface which is not displayed on the window (i.e back buffer) after updating we switch the front buffer (i.e the one displayed on the window) with the back buffer


		//Let the window stay for 5s before disappearing
		//Note : When the SDL is Delaying, even though the Window is visible, it cant accept input from keyboard or mouse
		SDL_Delay(5000);

		return true;	

	}

	void close(){

		//Destroy Surfaces
		SDL_FreeSurface(loaded_surface);	
		SDL_FreeSurface(optimized_surface);	

		//Destroy Window to free up memory
		SDL_DestroyWindow(window);

		//Quit all SDL Subsystems
		SDL_Quit();
	}	

};

int main(int argc, char *args[]){
	
	SDL_ImageDisplay display; 
	bool success;

	success = display.init();
	if(!success) {
		std::exit(1);
	}
	success = display.loadMedia();
	if(!success){
		std::exit(1);
	}
	display.close();

	return 0; 
}
