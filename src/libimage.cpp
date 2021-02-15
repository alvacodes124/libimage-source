/* Replace "dll.h" with the name of your header */
#include "../include/libimage.hpp"
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../extlibs/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extlibs/stb_image_write.h"
#include <iostream>

#include <algorithm>
#include <cstring>

namespace{
	
	std::string toLower(std::string str){
		for(std::string::iterator i = str.begin(); i != str.end(); ++i)
			*i = static_cast<char>(std::tolower(*i));
			return str;
	}
	
}

namespace png{
	
	Image::Image() : m_width(0) , m_height(0) {}
	Image::~Image(){}
	
	void Image::create(unsigned int width, unsigned int height, const png_color& color){
		if(width && height){
			std::vector<png_byte> newPixels(width * height * 4);
			
			png_byte* ptr = &newPixels[0];
			png_byte* end = ptr + newPixels.size();
			
			while(ptr < end){
				*ptr++ = color.r;
				*ptr++ = color.g;
				*ptr++ = color.b;
				*ptr++ = color.a;
			}
			
			this->m_pixels.swap(newPixels);
			
			this->m_width = width;
			this->m_height = height;
		}
		else{
			
			std::vector<png_byte>().swap(this->m_pixels);
			
			this->m_width = 0;
			this->m_height = 0;
		}
	}
	
	void Image::create(unsigned int width, unsigned int height, const png_byte* pixels){
		if(pixels && width && height){
			std::vector<png_byte> newPixels(pixels, pixels + width * height * 4);
				
			this->m_pixels.swap(newPixels);
			
			this->m_width  = width;
			this->m_height = height;
		}
		else{
			
			std::vector<png_byte>().swap(m_pixels);
			this->m_width  = 0;
			this->m_height = 0;
		}
	}
	
	bool Image::loadFromFile(const std::string &filename){
		this->m_pixels.clear();
		
		int width = 0;
		int height = 0;
		int channels = 0;
		png_byte* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		
		if(ptr){
				
				this->m_width = width;
				this->m_height = height;
				
				if(width && height){
					this->m_pixels.resize(width * height * 4);
					memcpy(&this->m_pixels[0], ptr, this->m_pixels.size());
				}
				
				stbi_image_free(ptr);
				
				return true;
		}
		else{
				std::cerr << "Failed to load image \"" << filename << "\". Reason: " << stbi_failure_reason() << std::endl;
				
				return false;
		}
	}
		
	bool Image::saveToFile(const std::string& filename){
		
		if(!m_pixels.empty() && (this->m_width > 0) && (this->m_height > 0)){
				
				const std::size_t dot = filename.find_last_of('.');
				const std::string extension = dot != std::string::npos ? toLower(filename.substr(dot + 1)) : "";
				
				if(extension == "bmp"){
					if(stbi_write_bmp(filename.c_str(), this->m_width, this->m_height, 4, &this->m_pixels[0]))
						return true;
				}
				
				else if(extension == "tga"){
					if(stbi_write_tga(filename.c_str(), this->m_width, this->m_height, 4, &this->m_pixels[0]))
						return true;
				}
				
				else if(extension == "png"){
					if(stbi_write_png(filename.c_str(), this->m_width, this->m_height, 4, &this->m_pixels[0], 0))
						return true;
				}
				else if (extension == "jpg" || extension == "jpeg"){
					if(stbi_write_jpg(filename.c_str(), this->m_width, this->m_height, 4, &this->m_pixels[0], 90))
						return true;
				}
		}
		
		std::cerr << "Failed to save image \"" << filename << "\"" << std::endl;
		return false;
	}
	
	void Image::empty(){
		this->create(this->m_width,this->m_height,{0,0,0,255});
	}
	
	const png_byte* Image::getPixelsPtr(){
		if(!this->m_pixels.empty()){
				return &this->m_pixels[0];
		}
		else{
				std::cerr << "Trying to access the pixels of an empty image" << std::endl;
				return NULL;
		}
	}
	
	
	void Image::setPixel(unsigned int x, unsigned int y, const png_color& color){
		png_byte* pixel = &this->m_pixels[(x + y * this->m_width) * 4];
		*pixel++ = color.r;
		*pixel++ = color.g;
		*pixel++ = color.b;
		*pixel++ = color.a;
	}
	
	png_color Image::getPixel(unsigned int x,unsigned int y) const{
		const png_byte* pixel = &this->m_pixels[(x + y * this->m_width) * 4];
		return {pixel[0], pixel[1], pixel[2], pixel[3]};
	}
	
	void Image::setSize(unsigned int width,unsigned int height){
		this->m_width  = width;
		this->m_height = height;
		this->m_pixels.reserve(this->m_width*this->m_height);
	}
	
}
