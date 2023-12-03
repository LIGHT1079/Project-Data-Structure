#pragma once
#include"Bus.h"
#include"Passenger.h"
#include"fifoqueue.h"
#include"priorityqueue.h"

class Station
{
private:
	static int travel_distance;
	const int number;
	static int station_count;
	fifoqueue<Bus*> MBusInStationForward;
	fifoqueue<Bus*> WBusInStationForward;
	fifoqueue<Bus*> MBusInStationBackward;
	fifoqueue<Bus*> WBusInStationBackward;
	priorityqueue<Passenger*> stationpassengersForward;
	priorityqueue<Passenger*> WheelChairQForward;
	priorityqueue<Passenger*> stationpassengersBackward;
	priorityqueue<Passenger*> WheelChairQBackward;

public:
	Station(int);
	
	bool exitpassenger(int);

	void displayinfo();
	
	void setTravelDistance(int);
	
	void promotePassengers(int);
	
	void insertpassenger(Passenger*);
	
	void EnqueueBus(Bus* incoming,fifoqueue<Passenger*> &finished_queue);

	void DequeueBus(int curr_time);

	Passenger* get_random_passenger(int random_number);

	static void setstationcount(int);
	~Station(); 
};

