//Resizing an image and displaying it on Screen

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
	       img_Flags = IMG_INIT_JPG;
	       if( (IMG_Init(img_Flags) != IMG_INIT_JPG)) {
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
		//The loaded image is in 24 bit format, SDL requires it to be 32 bit format
		//This conversion is done so that blitting is done faster and coversion is not required everytime you blit the loaded surface
		optimized_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);
		
		//loaded image width and height

		float loaded_width = optimized_surface->w;
		float loaded_height = optimized_surface->h;
		float scale = loaded_width / loaded_height;

		//Resizing Image keeping the aspect ratio	
		float resized_image_width =  loaded_width > loaded_height ?  screen_width : scale * screen_height;
		float resized_image_height = loaded_height > loaded_width ? screen_height : (1/scale) * screen_width;
		float resized_image_x = (screen_width - resized_image_width)/2;
		float resized_image_y = (screen_height - resized_image_height)/2;

		//Applying the dimenstion for image
		SDL_Rect stretchRect;	

		//Top left corner where the image should be placed
		stretchRect.x = resized_image_x;
		stretchRect.y = resized_image_y;

		//setting the width and height of the image
		stretchRect.w = resized_image_width;
		stretchRect.h = resized_image_height;

		//Scale the optimized surface according to the scales provided by stretchRect on the screen_surface
		SDL_BlitScaled(optimized_surface, NULL, screen_surface, &stretchRect);

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
