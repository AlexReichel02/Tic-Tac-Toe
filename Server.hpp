#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <stdio.h>

#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <mutex>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <thread>

using namespace std;
#define PORT 60010


class Server {
public:
	Server();
	void startServer();
	
private:
	string convertString(string test);
	void runService(int clientSocket, int clientNum);
	void playGame(int clientSocket);
	void makeMove(string request,char player);
	bool isValidMove(string request,char player);
	bool isGameOver(char player);
	bool checkNoWinner(char player);
	void createBoard();
	void printGame(int clientSocket);
	void generateServerMoves();
	void serverMove();

	int serverSocket, newClientSocket;
	long readFromClient;
	struct sockaddr_in address;
	int addressLength;
	char game[3][3];
	char whoWon;
	char playerChar;
	vector<string> serverMoves;
};

#endif /* SERVER_HPP_ */
