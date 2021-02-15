#include <iostream>
#include <libimage.hpp>

using namespace std;
using namespace png;

int main(){
	
	Image imagen;
	
	imagen.create(10,10,{126,256,267,255});
	
	if(imagen.saveToFile("imagen.png")){
		cout<<"\" imagen.png \" se ha compilado con exito :)";
	}
	else{
		cout<<"\" imagen.png \" se ha compilado mal :(";
	}
	
}
