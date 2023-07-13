

#include "Server.hpp"


Server::Server() {
	 addressLength = sizeof(address);
	 address.sin_family = AF_INET;
	 address.sin_addr.s_addr = INADDR_ANY;
     address.sin_port = htons( PORT );
	 memset(address.sin_zero, '\0', sizeof address.sin_zero);
	 createBoard();
	 generateServerMoves();
}

string Server::convertString(string toChange){
	string newString="";
	const int length = toChange.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, toChange.c_str());
 
    for (int i = 0; i < length; i++)
    {
		if(isalnum(char_array[i])==true){
      		newString+= char_array[i];
		}
    }
	return newString;
}

bool Server::checkNoWinner(char player){
	int count=0;
	for (int i = 0; i < 3; i++) { 	 
            for (int j = 0; j < 3; j++) {
                 if(game[i][j] != '!'){
					count++;
				 }
            }
           
   	 }
	 if(count==9 && isGameOver(player)==false ){
		return true;
	 }
	 return false;
}

void Server::createBoard(){
	for (int i = 0; i < 3; i++) { 	 
            for (int j = 0; j < 3; j++) {
                 game[i][j] = '!';
            }
           
   	 }
}

void Server::generateServerMoves(){
	
	for (int i = 0; i < 3; i++) { 

            for (int j = 0; j < 3; j++) {
				 string move =to_string(i)+to_string(j); 
                 serverMoves.push_back(move);
            }
	
	}
}

void Server::serverMove(){
	bool goodMove=false;
	srand ( time(NULL) );
	while(!goodMove){
		
		 int index = (rand() % (9 - 0 + 1)) + 0;  
		 string request= serverMoves[index];
		 
		 if(isValidMove(request,'O')==true){
			makeMove(request,'O');
			goodMove=true;
		}
	}

}

void Server::playGame(int clientSocket){
	
			
	while(true){	

	string playScript = "Enter a move in grid coordinates with no spaces such as 02 \n";
	if(send(clientSocket, playScript.c_str(), playScript.length(), 0) == -1){
			perror("Error in send()!");
			exit(1);
	}

	char move[2];
	recv(clientSocket, move, sizeof(move), 0);
	printf("Server has received: %s\n", move);
	
	if(isValidMove(move,playerChar)==true){
		makeMove(move,playerChar);
	
		if(isGameOver(playerChar)==true){
			break;
		}
		if(checkNoWinner(playerChar)==true){
			break;
		}

		serverMove();

		if(isGameOver('O')==true){
			break;
		}
		if(checkNoWinner('O')==true){
			break;
		}
		printGame(clientSocket);
		
	}else{
		string takenScript = "Game position already taken try again";
		cout<<takenScript<<endl;
		if(send(clientSocket, takenScript.c_str(), takenScript.length(), 0) == -1){
			perror("Error in send()!");
			exit(1);
		}
		printGame(clientSocket);
	}


	}
	printGame(clientSocket);
	string overScript = "Game is Over ";
	overScript += whoWon ;
	overScript += " is the Winner!\n";
	cout<<overScript<<endl;
		if(send(clientSocket, overScript.c_str(), overScript.length(), 0) == -1){
			perror("Error in send()!");
			exit(1);
		}
	createBoard();
		
}

void Server::printGame(int clientSocket) {
		string gameBoard = "Current Game\n";
    	cout<<"Current Game"<<endl;

    	for (int i = 0; i < 3; i++) { 	 
            for (int j = 0; j < 3; j++) {
                cout<< game[i][j]<< " ";
				gameBoard+= game[i][j];
				gameBoard+=" ";
            }
            cout<<endl;
			gameBoard += "\n";
   	 }

	 if(send(clientSocket, gameBoard.c_str(), gameBoard.length(), 0) == -1){
			perror("Error in send()!");
			exit(1);
		}
    }

 bool Server::isGameOver(char player) {
	  int count1=0;
	  int count3=0;
	  int count4=0;
	  int count5=0;
	  bool crossDig=false;
	  bool down0=false;
	  bool down1=false;
	  bool down2=false;
	 
	 for (int i = 1; i < 3; i++) { 	
		 
		 if(game[i-1][0] == game[i][0] && game[i][0] != '!') {
			     count1++; 
		 }
		 
		 
		 if(game[i-1][1] == game[i][1] && game[i][1] != '!') {
				     count3++; 
			 }
 		 
		 
		 if(game[i-1][2] == game[i][2] && game[i][2] != '!') {
				     count4++; 
			 }
		 
		 if(game[i-1][i-1] == game[i][i] && game[i][i]!='!' ) {
   		  count5++;
   	  		}
      
 	 }

	

	if(game[0][0]==game[0][1] && game[0][0]==game[0][2] && game[0][0]!= '!'){
		down0=true;
	}

	if(game[0][1]==game[1][1] && game[0][1]==game[2][1]&& game[0][1]!= '!'){
		down1=true;
	}

	if(game[0][2]==game[1][2] && game[0][2]==game[2][2]&& game[0][2]!= '!'){
		down2=true;
	}
	  
	if(game[0][2]==game[1][1] && game[0][2] == game[2][0] && game[0][2] != '!'){
		crossDig=true;
	}

	if(count1 == 2 || count3 == 2 || count4 ==2 || count5 == 2 || crossDig ==true || down0==true || down1==true||down2==true) {
		whoWon = player;
		return true;
	}

	return false;
}


bool Server::isValidMove(string request,char player){

	int length = request.length();
    char* playerMove = new char[length + 1];
    strcpy(playerMove, request.c_str());

	
    	int X = int(playerMove[0])-48;
    	int Y = int(playerMove[1])-48;

		cout<<player<<" " << X<<" "<< Y << " "<<endl;
    	
    	for (int i = 0; i < 3; i++) { 	
            for (int j = 0; j < 3; j++) {
            	
            	if(i == X && j == Y) {
            	
            		if(game[i][j]=='!') {
            			return true;
					}
            	}
    		}
		}
    	return false;
}

void Server::makeMove(string request,char player){
		
    int length = request.length();
    char* playerMove = new char[length + 1];
    strcpy(playerMove, request.c_str());

	
    	int X = int(playerMove[0])-48;
    	int Y = int(playerMove[1])-48;
    	
    
    	for (int i = 0; i < 3; i++) { 	
            for (int j = 0; j < 3; j++) {
            	
            	if(i == X && j == Y) {
            			game[i][j] = player;
            		}
            	}
    		}
}


void Server::startServer(){

	int clientNum=0;
	
	// Creating socket file descriptor
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	    perror("Error in socket() method");
	    exit(EXIT_FAILURE);
	}

	if (::bind(serverSocket, (struct sockaddr *)&address, sizeof(address))==-1) {
	    perror("Erro in bind() method");
	    exit(EXIT_FAILURE);
	}

	if (listen(serverSocket, 10) < 0) {
	    perror("Error in listen() method");
	    exit(EXIT_FAILURE);
	}
		
	int pid;
	printf("=====Waiting for New connection=====\n");
   	while(1){

		if ((newClientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addressLength))<0) {
	        perror("Error in accept() method");
	        exit(EXIT_FAILURE);
	    }
		
		clientNum++;
		pid = fork();
		while(pid==0){
			runService(newClientSocket,clientNum);
		}

	}

	close(newClientSocket);
}

void Server::runService(int clientSocket, int clientNum) {
		string optionScript = " Welcome!\n  Type Yes to play Tic Tac Toe\n  Type exit to quit\n";

		while(true){

			if(send(clientSocket, optionScript.c_str(), optionScript.length(), 0) == -1){
				perror("Error in send()!");
				exit(1);
				}
			

			char msg[4];
			string checkResponse;
			recv(clientSocket, msg, sizeof(msg), 0);
			printf("Server has received: %s\n", msg);
			string response = msg;
			checkResponse = convertString(response);

			
			if((checkResponse =="Yes")||(checkResponse=="yes")) {

				string playScript = "Enter a single character to play as :\n";
				if(send(clientSocket, playScript.c_str(), playScript.length(), 0) == -1){
				perror("Error in send()!");
				exit(1);
				}
				char player[1];
				recv(clientSocket, player, sizeof(player), 0);
				playerChar = player[0];
				printf("Player playing as: %c\n", playerChar);
				playGame(clientSocket);
				memset(&player, 0, sizeof(player));
				
			}

			if((checkResponse=="Exit")||(checkResponse=="exit")) {
				cout << "Client " << clientNum << " has quit the session" << endl;
				break;
			}

			memset(&checkResponse, 0, sizeof(checkResponse));//clear the buffer
			
		}
		close(clientSocket);
		
}

int main()
{

	Server serv = Server();
	serv.startServer();

    return 0;
}

