#include <enet/enet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

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
	
	server= enet_host_create(&address,3, 1, 0, 0); // 3 clients, 1 channel
	
	// Check if the server was created successfully
	if(server== nullptr){
		std::cerr<< "Failed to create ENet server"<< std::endl;
		return 1;
	}
	std::cout<< "Server started on port 1453"<< std::endl;

	std::vector<ENetPeer*> clients;

	// Main loop
	while(1){
		ENetEvent event;
		
		// Check for events
		while(enet_host_service(server, &event, 1000) > 0){	// 1 second timeout
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT: // A new client connected
					std::cout<< "A new client connected from "
							<< event.peer->address.host<< ":" 			// IP
							<< event.peer->address.port<< std::endl; 	// Port
					clients.push_back(event.peer);
					break;

				case ENET_EVENT_TYPE_RECEIVE: // A packet was received
					{
						std::cout<< "Message received: " 
								<< event.packet->data	// Message
								<< std::endl;
						
						// Send a response
						const char* msg= (const char*)event.packet->data;
						ENetPacket* packet= enet_packet_create(msg, 
															  strlen(msg) + 1, 
															  ENET_PACKET_FLAG_RELIABLE); // Reliable packet (TCP)

						// Send the packet to all clients
						for(auto& client : clients){
							if(client== event.peer) continue; // Don't send the message to the sender
							enet_peer_send(client, 0, packet);
						}

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