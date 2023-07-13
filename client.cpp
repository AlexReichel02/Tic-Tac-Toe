
#include "client.hpp"
#include<iostream>
using namespace std;

int main(int argc, char const *argv[]) {
					
	int userSocket = 0;										
	struct sockaddr_in serverAddress;	
	memset(&serverAddress, '\0', sizeof(serverAddress));

		// Creating socket
		userSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (userSocket < 0) {
				perror("n Error in socket creation\n");
				exit(1);
		}
		

		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(60010);
		serverAddress.sin_addr.s_addr = inet_addr("127.0.0.01");

		if(connect(userSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
				perror("\nFailure in connect() method! \n");
				exit(1);
		}
		
		
		char msg[1500];
		
		while(1){
			sleep(1);
			recv(userSocket, msg, sizeof(msg), 0);
			cout << "Server: " << msg << endl;
			memset(&msg, 0, sizeof(msg));
        	
        	cout << ">";
			string userInput="";
       		cin>> userInput;
        	
        		if(userInput== "exit"){
					cout<<"Have a good day"<<endl;
            		send(userSocket, userInput.c_str(), userInput.length(), 0);
            		break;
        		}

				printf("User is sending: %s\n", userInput.c_str());
        		if(send(userSocket, userInput.c_str(), userInput.length(), 0) == -1){
    				perror("Error in send()!");
    				exit(1);
    			}
        
    	}

		close(userSocket);
    return 0;

}
