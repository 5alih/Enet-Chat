#include <enet/enet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>

int main(){
	// Initialize ENet
	if(enet_initialize()!= 0){
		std::cerr<< "Failed to initialize ENet"<< std::endl;
		return 1;
	}

	ENetAddress address;
	ENetHost* server;
	
	// Create a server host
	address.host= ENET_HOST_ANY;
	address.port= 1453;
	
	server= enet_host_create(&address,1, 1, 0, 0); // 1 client, 1 channel
	
	// Check if the server was created successfully
	if(server== nullptr){
		std::cerr<< "Failed to create ENet server"<< std::endl;
		return 1;
	}

	std::cout<< "Server started on port 1453"<< std::endl;
	std::cout<< "Server running. Type 'quit' to stop."<< std::endl;

	bool running= true;
	
	// Create a thread to handle input (only 'quit' for now)
	std::thread input_thread([&running](){
		std::string input;
		while(running){
			std::getline(std::cin, input);
			if(input== "quit"){
				running= false;
			}
		}
	});
	// Detach the thread so it can run in the background
	input_thread.detach();

	// Main loop
	while(running){
		ENetEvent event;
		
		// Check for events
		while(enet_host_service(server, &event, 1000) > 0){	// 1 second timeout
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT: // A new client connected
					std::cout<< "A new client connected from "
							<< event.peer->address.host<< ":" 			// IP
							<< event.peer->address.port<< std::endl; 	// Port
					break;

				case ENET_EVENT_TYPE_RECEIVE: // A packet was received
					{
						std::cout<< "Message received: " 
								<< event.packet->data	// Message
								<< std::endl;
						
						// Send a response
						const char* msg= "Server received your message!";
						ENetPacket* packet= enet_packet_create(msg, 
															  strlen(msg) + 1, 
															  ENET_PACKET_FLAG_RELIABLE); // Reliable packet (TCP)
						enet_peer_send(event.peer, 0, packet); // Send the packet to channel 0
						
						// Clean up the packet
						enet_packet_destroy(event.packet);
						break;
					}

				case ENET_EVENT_TYPE_DISCONNECT: // A client disconnected from the server
					std::cout<< "Client disconnected!"<< std::endl;
					event.peer->data= nullptr;
					break;
				
				case ENET_EVENT_TYPE_NONE: // No events occurred within the timeout period (1 second)
					break;
			}
		}
	}

	enet_host_destroy(server); // Destroy the server host
	enet_deinitialize(); // Deinitialize ENet and cleanup
	
	return 0;
}