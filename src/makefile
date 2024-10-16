CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES= simulation itinerary_test ChargeMate

all: $(ALL_EXECUTABLES)

# ITINERARY
itinerary.o : itinerary.c itinerary.h en_tete.h
	$(CC) $(CFLAGS) -c itinerary.c


# ITINERARY_TEST
itinerary_test: itinerary_test.o itinerary.o 
	$(CC) $(LDFLAGS) itinerary_test.o itinerary.o -o itinerary_test

itinerary_test.o: itinerary_test.c itinerary.h en_tete.h
	$(CC) $(CFLAGS) -c itinerary_test.c


# SIMULATION
simulation : simu.o simu_test.o
	$(CC) $(LDFLAGS) simu.o simu_test.o -o simu

simu.o : simu.c  simu.h en_tete.h
	$(CC) $(CFLAGS) -c simu.c

simu_test.o : simu_test.c simu.h en_tete.h
	$(CC) $(CFLAGS) -c simu_test.c

# APPLICATION PRINCIPALE
ChargeMate : ChargeMate.o simu.o itinerary.o
	$(CC) $(LDFLAGS) ChargeMate.o simu.o itinerary.o -o ChargeMate 

ChargeMate.o : ChargeMate.c en_tete.h simu.h itinerary.h 
	$(CC) $(CFLAGS) -c ChargeMate.c

clean:
	rm -f *.o $(ALL_EXECUTABLES) simu 
