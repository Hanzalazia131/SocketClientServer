
all:
	g++ Server1.cpp -o Server1 -lpthread
	g++ client.cpp -o client -lpthread
