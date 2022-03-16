//Using Key presses to display different colors on window

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

	uint8_t R_palette, G_palette, B_palette;


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

	       //Initializing the values to zero, the screen will initially be black
	       R_palette = 0x00;
	       G_palette = 0x00;
	       B_palette = 0x00;
	       
	       //Now that window has been created we have to draw on it, so get window surface
	       screen_surface = SDL_GetWindowSurface(window);

	       return true;
	}

	bool loadMedia(){

		//Loading the color into the screen
		SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, R_palette, G_palette, B_palette));

		//Now that you have colored something on the screen surface you have to update the window
	        SDL_UpdateWindowSurface(window);
		//We have two buffers front and back for these windows, when we are making changes to the screen surface we are manipulating the back surface which is not displayed on the window (i.e back buffer) after updating we switch the front buffer (i.e the one displayed on the window) with the back buffer

		return true;	

	}

	bool poll() {
		
		SDL_Event event;
		
		//All events pressed by the keyboard, joystick, mouse are stored in the event queue. The SDL_PollEvent
		//polls the events in the event queue until the queue is exhausted and checks for the event type	
		while(SDL_PollEvent(&event)){
			
			if(event.type == SDL_QUIT) {
				return true;
			}
			//Based on the arrow key pressed a particular will be displayed or if ESC or q is pressed the window is quit
			else if(event.type == SDL_KEYDOWN) {
				
				switch(event.key.keysym.sym) {
				
					case SDLK_UP : R_palette = 0xFF, G_palette = 0x00, B_palette = 0x00; 
						       break;

					case SDLK_DOWN : R_palette = 0x00, G_palette = 0xFF, B_palette = 0x00; 
						         break;

					case SDLK_LEFT : R_palette = 0x00, G_palette = 0x00, B_palette = 0xFF; 
						         break;

					case SDLK_RIGHT : R_palette = 0xFF, G_palette = 0xFF, B_palette = 0xFF; 
				   		          break;
									
					case SDLK_ESCAPE: 
					case SDLK_q: 	return true;

					default : break;					       

				}

			}

		}	
		
		return false;

	}

	void close(){


		//Destroy Window to free up memory
		SDL_DestroyWindow(window);

		//Quit all SDL Subsystems
		SDL_Quit();
	}	

};

int main(int argc, char *args[]){
	
	SDL_ImageDisplay display; 
	bool success;
	bool quit = false;

	success = display.init();
	if(!success) {
		std::exit(1);
	}

	while(!quit) {
		
		success = display.loadMedia();
		if(!success){
			std::exit(1);
		}

		quit = display.poll();
	}
	
	display.close();

	return 0; 
}
