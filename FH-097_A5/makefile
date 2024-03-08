# Makefile for the Java RMI Assignment

# Target: make all
all:
	javac Client/*.java
	javac Server/*.java

# Target: make rmi
rmi:
	rmiregistry

# Target: make server
server:
	java Server.RemoteServer

# Target: make client vigenere
client_vigenere:
	java Client.ClientVigenere

# Target: make client caeser
client_caeser:
	java Client.ClientCaesar

# Target: make clean
# clean:
# 	rm -f Client/*.class Server/*.class
clean:
	del /Q Client\*.class
	del /Q Server\*.class
