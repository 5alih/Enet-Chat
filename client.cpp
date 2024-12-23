#include <enet/enet.h>
#include <iostream>
#include <cstring>
#include <string>

int main(){
	// Initialize ENet
	if(enet_initialize()!= 0){
		std::cerr<< "Failed to initialize ENet!"<< std::endl;
		return 1;
	}

	ENetHost* client;
	
	// Create a client host
	client= enet_host_create(nullptr, 1, 1, 0, 0); // 1 connection, 1 channel
	
	// Check if the client was created successfully
	if(client== nullptr){
		std::cerr<< "Failed to create ENet client"<< std::endl;
		return 1;
	}

	ENetAddress address;
	ENetPeer* peer;
	
	// Connect to localhost:1453
	enet_address_set_host(&address, "localhost");
	address.port= 1453;
	
	// Create a peer to connect to the server
	peer= enet_host_connect(client, &address, 1, 0);
	
	// Check if the peer was created successfully
	if(peer== nullptr){
		std::cerr<< "Failed to create peer"<< std::endl;
		return 1;
	}

	// Wait for the connection to succeed
	ENetEvent event;
	if(enet_host_service(client, &event, 5000) > 0 &&
		event.type== ENET_EVENT_TYPE_CONNECT){
		std::cout<< "Connection succeeded!"<< std::endl;
		
		// Send a message to the server
		{
			const char* msg= "Hello, world!";
			ENetPacket* packet= enet_packet_create(msg, 
												  strlen(msg) + 1, 
												  ENET_PACKET_FLAG_RELIABLE); // Reliable packet (TCP)
			enet_peer_send(peer, 0, packet);
		}
		
		while(enet_host_service(client, &event, 5000) > 0){ // 5 second timeout
			switch(event.type){
				case ENET_EVENT_TYPE_RECEIVE: // A packet was received
					std::cout<< "Message from server: " 
							<< event.packet->data // Message
							<< std::endl;
					enet_packet_destroy(event.packet); // Clean up the packet
					break;
				
				case ENET_EVENT_TYPE_DISCONNECT: // Disconnected from server
					std::cout<< "Disconnected from server"<< std::endl;
					return 0;
					
				default:
					break;
			}
		}
	}
	else{
		std::cerr<< "Connection failed!"<< std::endl;
	}

	enet_peer_reset(peer); // Reset the peer
	enet_host_destroy(client); // Destroy the client host
	enet_deinitialize(); // Deinitialize ENet and clean up
	
	return 0;
}