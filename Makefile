#gcc discovery-nvt.c soapC.c soapClient.c namespaces.c -I./ -luuid -o discovery-nvt
#gcc discovery-nvc.c soapC.c soapClient.c namespaces.c -I./ -luuid -o discovery-nvc

#makefile

#BUILD_DIR = $(DVRPROJ)/build/$(DVR_ARCH)

#VPATH = ./$(DVR_ARCH)

LIBS += -luuid -lrt -lpthread
# -lcrypto -lssl


#LDFLAGS=
CFLAGS=-O2
CC=gcc

#LDFLAGS += -L$(BUILD_DIR)/libs -L$(CROSS_LIB_PATH)

LDFLAGS += $(LIBS)
#LDFLAGS += -Wl,-rpath-link $(BUILD_DIR)/libs
#LDFLAGS += -Wl,-rpath-link $(CROSS_LIB_PATH)
#LDFLAGS += 

#CFLAGS += -Drestrict= $(INCLUDES)
#CFLAGS += -DWITH_DOM -DWITH_OPENSSL 
#-DSOAP_DEBUG


#%.o : %.c
#	$(CC) -c $(CFLAGS) $< -o $@
#
#nvt: discovery-nvt.o soapServer.o soapC.o namespaces.o stdsoap2.o
#	$(CC) $^ $(LDFLAGS) -o $@
#
#nvc: discovery-nvc.o soapClient.o soapC.o namespaces.o stdsoap2.o
#	$(CC) $^ $(LDFLAGS) -o $@

TARGET := nvc nvt

all: 
	$(CC) $(CFLAGS) discovery-nvt.c soapC.c soapServer.c namespaces.c stdsoap2.c -luuid -o nvt
	$(CC) $(CFLAGS) discovery-nvc.c soapC.c soapClient.c namespaces.c stdsoap2.c -luuid -o nvc
#	$(CC) discovery-nvc.o soapClient.o soapC.o namespaces.o stdsoap2.o $(LDFLAGS) -o nvc
#	$(CC) discovery-nvt.o soapServer.o soapC.o namespaces.o stdsoap2.o $(LDFLAGS) -o nvt
	chmod a+x $(TARGET)


clean :
	rm -rf $(TARGET) *.o

depclean :	

