#ifndef IMAGE_LIBIMAGE_HPP
#define IMAGE_LIBIMAGE_HPP

#if BUILDING_DLL
#define LIBIMAGE_API __declspec(dllexport)
#else
#define LIBIMAGE_API __declspec(dllimport)
#endif


#include <vector>
#include <string>

namespace png{
	
	typedef unsigned char png_byte;
	
	typedef struct png_color{
		png_byte r;
		png_byte g;
		png_byte b;
		png_byte a;
	}png_color;
	
	class LIBIMAGE_API Image{
	public:
		
		Image(const Image &imagen);
		Image(const std::string &filename);
		Image();
		virtual ~Image();
		
		void create(unsigned int width,unsigned int height,const png_color &color = {0,0,0,255});
		void create(unsigned int width,unsigned int height,const png_byte *pixels);
		
		bool loadFromFile(const std::string &filename);
		bool saveToFile(const std::string &filename);
		
		void setPixel(unsigned int x,unsigned int y,const png_color &color);
		void setSize(unsigned int width,unsigned int height);
		
		png_color  getPixel(unsigned int x,unsigned int y) const;
		const png_byte *getPixelsPtr();
		unsigned int getwidth();
		unsigned int getheight();
		
		void empty();
			
	private:
		std::vector<png_byte> m_pixels;
		unsigned int m_width;
		unsigned int m_height;
	};
	
}

#endif //IMAGE_LIBIMAGE_HPP
