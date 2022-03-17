//Load Image and Display using Texture and Renderer which uses Hardware acceleration
//Using the screen surface we do software rendering using CPU which is slower compared to the hardware
//Also the surface are stored in the RAM but Textures are stored in VRAM
//This means the surface stored in RAM is easier to manipulate using blitting and other ways. Accessing is easy
//But the texture in VRAM is difficult to access and you can only access them using special methods, also the renderer has
//settings which the GPU uses for maniuplation of image. 
//The reason why they are stored this way is because its easier for GPU to understand this and manipulate

#include "SDL2/SDL.h"
#include<iostream>
#include "SDL2/SDL_image.h"
#include<stdio.h>

const unsigned int screen_width = 640;
const unsigned int screen_height = 480;

class SDL_ImageDisplay {

	SDL_Window *window; // Creating a SDL window pointer
	SDL_Surface *loaded_surface; //New loaded image to display on Surface contained by the window, Basically a 2D image seen in the window
	SDL_Renderer *renderer; //Renderer needed for drawing the texture on the screen
	SDL_Texture *texture; //Texture is basically the surface but it is used by gpu and in the VRAM
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
	       if((IMG_Init(img_Flags) != IMG_INIT_JPG)) {
			std::cout<<"Error initialzing SDL Image "<<SDL_GetError()<<std::endl;
	       }
	       //Now that window has been created we have to draw on it, so for this we create the renderer
	       // -1 here picks the first device made available that can be used for acceleration
	       renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	       if(renderer==NULL){
			std::cout<<"Could not create Renderer"<<SDL_GetError()<<std::endl;
			return false;
	       }

	       //Now we have to change the settings of the renderer and here we set the color
	       //params: renderer, r, g, b, alpha(opaqueness)
	       SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	       return true;
	}

	bool loadMedia(){

		//Loading the image into the screen
		loaded_surface = IMG_Load("maxresdefault.jpg");
		
		if(loaded_surface == NULL) {
			std::cout<<"Error loading image "<<IMG_GetError()<<std::endl;
			return false;
		}

		//We are creating the texture from the loaded surface and are providing which renderer to use	
		texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);		

		if(texture == NULL){
			std::cout<<"Could not create Texture "<<SDL_GetError()<<std::endl;
			return false;
		}

		SDL_FreeSurface(loaded_surface); //Free up this surface as it is not needed now that texture is created from it	
		int loaded_width, loaded_height;

		//load image width and height
		//This will give the essential parameters of the texture
		//params : texture, format, access, width, height
		SDL_QueryTexture(texture,NULL,NULL,&loaded_width,&loaded_height);

		//Select which portion of the loaded image should be used for display
		SDL_Rect loadedRect;

		//Top left corner from where the image should be loaded
		loadedRect.x = 0;
		loadedRect.y = 0;

		//setting the width and height of the image
		loadedRect.w = loaded_width;
		loadedRect.h = loaded_height;

		//Find out the scale
		float scale = (float) loaded_width / loaded_height;
	
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

		//Now we have to update the window like the UpdateWindow, but here its a little different
		//Clear the renderer, if this is not done the previous image on the screen will be kept as it as and new changes will be on top of it, it will clear the screen with the current set color
		SDL_RenderClear(renderer);
		
		//Now that we have the texture loaded, we have to put it on the Render, so we render the texture
		//params: the renderer, texture, source dimension, destination dimension 
		//you can put NULL in the dimensions if you do not want to resize
		SDL_RenderCopy(renderer, texture, &loadedRect, &stretchRect);

		//Update the Renderer now
		SDL_RenderPresent(renderer);

		//Let the window stay for 5s before disappearing
		//Note : When the SDL is Delaying, even though the Window is visible, it cant accept input from keyboard or mouse
		SDL_Delay(5000);

		return true;	

	}

	void close(){

		//Destroy Texture
		SDL_DestroyTexture(texture);

		//Destroy Renderer
		SDL_DestroyRenderer(renderer);

		//Destroy Window to free up memory
		SDL_DestroyWindow(window);

		//Quit all SDL Subsystems
		IMG_Quit();
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
