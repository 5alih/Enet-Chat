#include <enet/enet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

//test

struct Client{
	ENetPeer* peer;
	std::string username;
};

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
	address.port= 1919;
	
	server= enet_host_create(&address,3, 1, 0, 0); // 3 clients, 1 channel
	
	// Check if the server was created successfully
	if(server== nullptr){
		std::cerr<< "Failed to create ENet server"<< std::endl;
		return 1;
	}
	std::cout<< "Server started on port 1919"<< std::endl;

	std::vector<Client> clients;

	// Main loop
	while(1){
		ENetEvent event;
		
		// Check for events
		while(enet_host_service(server, &event, 1000) > 0){	// 1 second timeout
			switch(event.type){
				case ENET_EVENT_TYPE_CONNECT: {
				// A new client connected
					std::cout<< "A new client connected from "
							<< event.peer->address.host<< ":" 			// IP
							<< event.peer->address.port<< std::endl; 	// Port
					Client newClient= {event.peer, "Unknown"};
					clients.push_back(newClient);
					break;
				}
                case ENET_EVENT_TYPE_RECEIVE: {
                    std::string message((char*)event.packet->data);
                    
                    // Find the client who sent the message
                    auto sender = std::find_if(clients.begin(), clients.end(),
                        [&](const Client& c) { return c.peer == event.peer; });
                    
                    if (sender != clients.end()) {
                        if (sender->username == "Unknown") {
                            // This is the username registration message
                            sender->username = message;
                            std::string welcome = sender->username + " joined the chat!";
                            std::cout << welcome << std::endl;
                            
                            // Broadcast join message to all clients
                            for (const auto& client : clients) {
                                if (client.peer != event.peer) {
                                    ENetPacket* packet = enet_packet_create(
                                        welcome.c_str(),
                                        welcome.length() + 1,
                                        ENET_PACKET_FLAG_RELIABLE
                                    );
                                    enet_peer_send(client.peer, 0, packet);
                                }
                            }
                        } else {
                            // Regular chat message
                            std::string formatted = sender->username + ": " + message;
                            std::cout << formatted << std::endl;
                            
                            // Broadcast to all other clients
                            for (const auto& client : clients) {
                                if (client.peer != event.peer) {
                                    ENetPacket* packet = enet_packet_create(
                                        formatted.c_str(),
                                        formatted.length() + 1,
                                        ENET_PACKET_FLAG_RELIABLE
                                    );
                                    enet_peer_send(client.peer, 0, packet);
                                }
                            }
                        }
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }

				case ENET_EVENT_TYPE_DISCONNECT: {
					auto it = std::find_if(clients.begin(), clients.end(),
						[&](const Client& c) { return c.peer == event.peer; });
					
					if (it != clients.end()) {
						std::string disconnect_msg = it->username + " left the chat!";
						std::cout << disconnect_msg << std::endl;
						
						// Broadcast disconnect message
						for (const auto& client : clients) {
							if (client.peer != event.peer) {
								ENetPacket* packet = enet_packet_create(
									disconnect_msg.c_str(),
									disconnect_msg.length() + 1,
									ENET_PACKET_FLAG_RELIABLE
								);
								enet_peer_send(client.peer, 0, packet);
							}
						}
						clients.erase(it);
					}
					event.peer->data = nullptr;
					break;
				}
			}
		}
	}

	enet_host_destroy(server); // Destroy the server host
	enet_deinitialize(); // Deinitialize ENet and cleanup
	
	return 0;
}