#include <iostream>
#include <sstream>
#include <string>

#include "SDL.h"
#include "SDL_net.h"

#include "BinaryStream.h"
#include "ShapeManager.h"

int main() {
	if(SDL_Init(SDL_INIT_NOPARACHUTE)==-1) {
		std::cerr << "SDL_Init: %s\n" << SDL_GetError();
		return 1;
	}
	if(SDLNet_Init()==-1) {
		std::cerr << "SDLNet_Init: %s\n" << SDLNet_GetError();
		return 2;
	}
	
	UDPsocket socket = SDLNet_UDP_Open(54321);
	
	UDPpacket* packet = SDLNet_AllocPacket(2000);
	
	while(true) {
		int numrecv=SDLNet_UDP_Recv(socket, packet);
		if(numrecv) {
			std::stringstream stream(std::string(reinterpret_cast<char*>(packet->data), packet->len));
			bistream in(stream);
			ShapeManager2d manager;
			in >> manager;
			std::cout << manager;
		}
	}
}