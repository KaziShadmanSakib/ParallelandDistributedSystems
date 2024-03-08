CLIENT = matrixOp_client
SERVER = matrixOp_server

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = matrixOp.x

TARGETS_SVC.c = matrixOp_svc.c matrixOp_server.c matrixOp_xdr.c 
TARGETS_CLNT.c = matrixOp_clnt.c matrixOp_client.c matrixOp_xdr.c 
TARGETS = matrixOp.h matrixOp_xdr.c matrixOp_clnt.c matrixOp_svc.c matrixOp_client.c matrixOp_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)

# Compiler flags 
CC = gcc
CFLAGS += -g -DRPC_SVC_FG -I/usr/include/tirpc -I/path/to/gsl/headers
LDLIBS += -lnsl -ltirpc -lm -lgsl -lgslcblas
RPCGENFLAGS = -a -C

# Targets 
all : $(CLIENT) $(SERVER)

$(TARGETS) : $(SOURCES.x)
	@if [ ! -f matrixOp_server.c ]; then rpcgen $(RPCGENFLAGS) $(SOURCES.x); fi
	@if [ ! -f matrixOp_client.c ]; then rpcgen $(RPCGENFLAGS) $(SOURCES.x); fi

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

clean:
	$(RM) core $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)
