#include "Station.h"
#include<iostream>
#include<string>
using namespace std;

int Station:: travel_distance = 0;
int Station::station_count = 0;

void Station::setstationcount(int count)
{
	station_count = count;
}

Station::Station(int num):number(num)
{
}

bool Station::exitpassenger(int id)
{
	Passenger* temp = new Passenger("NP", id);
	if (stationpassengersForward.remove(temp))
		return true;
	if (stationpassengersBackward.remove(temp))
		return true;
	if (WheelChairQForward.remove(temp))
		return true;
	if (WheelChairQBackward.remove(temp))
		return true;
	return false;
}

void Station::exitpassengerbytype(Passenger*& leaving, int type)
{
	bool stop = false;
	fifoqueue<Passenger*> tempQ;
	Passenger* moved = nullptr;

	if (type != 10)
	{
		while (!stationpassengersForward.isempty())
		{
			stationpassengersForward.pop(moved);
			if (!stop && moved->GetPassengerPriority() == type)
			{
				leaving = moved;
				stop = true;
			}
			else
				tempQ.push(moved);
		}
		while (!tempQ.isempty())
		{
			tempQ.pop(moved);
			stationpassengersForward.push(moved, moved->GetPassengerPriority());
		}

		if (stop)
			return;

		while (!stationpassengersBackward.isempty())
		{
			stationpassengersBackward.pop(moved);
			if (!stop && moved->GetPassengerPriority() == type)
			{
				leaving = moved;
				stop = true;
			}
			else
				tempQ.push(moved);
		}
		while (!tempQ.isempty())
		{
			tempQ.pop(moved);
			stationpassengersBackward.push(moved, moved->GetPassengerPriority());
		}

		if (stop)
			return;
	}

	else
	{
		while (!WheelChairQForward.isempty())
		{
			WheelChairQForward.pop(moved);
			if (!stop && moved->GetPassengerPriority() == type)
			{
				leaving = moved;
				stop = true;
			}
			else
				tempQ.push(moved);
		}
		while (!tempQ.isempty())
		{
			tempQ.pop(moved);
			WheelChairQForward.push(moved);
		}

		if (stop)
			return;

		while (!WheelChairQBackward.isempty())
		{
			WheelChairQBackward.pop(moved);
			if (!stop && moved->GetPassengerPriority() == type)
			{
				leaving = moved;
				stop = true;
			}
			else
				tempQ.push(moved);
		}
		while (!tempQ.isempty())
		{
			tempQ.pop(moved);
			WheelChairQBackward.push(moved);
		}

		if (stop)
			return;

	}

}

void Station::displayinfo()
{
	
	printer.Print("==============Station#" + to_string(number)+"=================\n");

	printer.Print(stationpassengersForward.sizebypri(3)+ stationpassengersForward.sizebypri(2)+ stationpassengersForward.sizebypri(1)+ stationpassengersBackward.sizebypri(3) + stationpassengersBackward.sizebypri(2) + stationpassengersBackward.sizebypri(1));

	printer.Print("  Waiting SP: FWD[");

	if (stationpassengersForward.PrintByPriority(3))
		printer.Print("(AG) , ");
	if (stationpassengersForward.PrintByPriority(2))
		printer.Print("(PD) , ");
	if (stationpassengersForward.PrintByPriority(1))
		printer.Print("(PW)");

	printer.Print("]  BCK[");

	if (stationpassengersBackward.PrintByPriority(3))
		printer.Print("(AG) , ");
	if (stationpassengersBackward.PrintByPriority(2))
		printer.Print("(PD) , ");
	if (stationpassengersBackward.PrintByPriority(1))
		printer.Print("(PW)");

	printer.Print("]\n");

	printer.Print(stationpassengersForward.sizebypri(0)+ stationpassengersBackward.sizebypri(0));

	printer.Print("  Waiting NP: FWD[");

	stationpassengersForward.PrintByPriority(0);

	printer.Print("]  BCK[");

	stationpassengersBackward.PrintByPriority(0);

	printer.Print("]\n");

	printer.Print(WheelChairQBackward.sizebypri(10) + WheelChairQForward.sizebypri(10));

	printer.Print("  Waiting WP: FWD[");

	WheelChairQForward.PrintByPriority(10);

	printer.Print("]  BCK[");

	WheelChairQBackward.PrintByPriority(10);

	printer.Print("]\n");
	
}

void Station::insertpassenger(Passenger* incoming)
{
	if (incoming->getdirection())
	{
		if (incoming->GetPassengerPriority() == 10)
			WheelChairQForward.push(incoming);
		else
			stationpassengersForward.push(incoming, incoming->GetPassengerPriority());
	}
	else
	{
		if (incoming->GetPassengerPriority() == 10)
			WheelChairQBackward.push(incoming);
		else
			stationpassengersBackward.push(incoming, incoming->GetPassengerPriority());
	}
}

void Station::EnqueueBus(Bus* incoming, fifoqueue<Passenger*> &finished_queue)
{
	incoming->exit_passenger(finished_queue);
	if (incoming->getDirection())
	{
		if (incoming->getBusType())
		{
			MBusInStationForward.push(incoming);
			while (!stationpassengersForward.isempty()&&!incoming->is_full())
			{
				
					Passenger* psg;

					stationpassengersForward.pop(psg);

					incoming->enter_passenger(psg);
				
			}
			
		}
		else
		{
			WBusInStationForward.push(incoming);
			if (!WheelChairQForward.isempty())
			{
				Passenger* psg;

				WheelChairQForward.pop(psg);

				incoming->enter_passenger(psg);
			}
		}
	}
	else
	{
		if (incoming->getBusType())
		{
			MBusInStationBackward.push(incoming);
			if (!stationpassengersBackward.isempty())
			{
				Passenger* psg;

				stationpassengersBackward.pop(psg);

				incoming->enter_passenger(psg);
			}
		}
		else
		{
			WBusInStationBackward.push(incoming);
			if (!WheelChairQBackward.isempty())
			{
				Passenger* psg;

				WheelChairQBackward.pop(psg);

				incoming->enter_passenger(psg);
			}
		}
	}

}

void Station::DequeueBus(int curr_time)
{
	Bus* Mbus = nullptr;
	if (!MBusInStationForward.isempty())
	{
		if (MBusInStationForward.peek()->is_full() || stationpassengersForward.isempty())
		{
			MBusInStationForward.peek()->setInStation(false);
			MBusInStationForward.peek()->setArriveTime(travel_distance + curr_time);
			MBusInStationForward.pop(Mbus);
		}
	}
	else if (!MBusInStationBackward.isempty())
	{
		if (MBusInStationBackward.peek()->is_full() || stationpassengersBackward.isempty())
		{
			MBusInStationBackward.peek()->setInStation(false);
			MBusInStationBackward.peek()->setArriveTime(travel_distance + curr_time);
			MBusInStationBackward.pop(Mbus);
		}
	}
	else  if (!WBusInStationForward.isempty())
	{
		if (WBusInStationForward.peek()->is_full() || WheelChairQForward.isempty())
		{
			WBusInStationForward.peek()->setInStation(false);
			WBusInStationForward.peek()->setArriveTime(travel_distance + curr_time);
			WBusInStationForward.pop(Mbus);
		}
	}
	 else if (!WBusInStationBackward.isempty())
	{
		if (WBusInStationBackward.peek()->is_full() || WheelChairQBackward.isempty())
		{
			WBusInStationBackward.peek()->setInStation(false);
			WBusInStationBackward.peek()->setArriveTime(travel_distance + curr_time);
			WBusInStationBackward.pop(Mbus);
		}
	}

	if (Mbus != nullptr)
	{
		if (Mbus->get_station() == station_count)
		{
			Mbus->set_direction(false);


		}
		else if (Mbus->get_station() == 1)
		{
			Mbus->set_direction(true);
		}
		Mbus->upgrade_station();
	}
}

void Station::setTravelDistance(int dist)
{
	travel_distance = dist;
}

void Station::promotePassengers(int curr_time)
{
	fifoqueue<Passenger*> notpromotedQ;
	fifoqueue<Passenger*> promotedQ;
	Passenger* temp;

	while (!stationpassengersForward.isempty())
	{
		stationpassengersForward.pop(temp);
		if (curr_time - temp->getarrivetime() == temp->getmaxwait())
		{
			temp->UpgradePriority();
			promotedQ.push(temp);
		}
		else
			notpromotedQ.push(temp);
	}

	while (!promotedQ.isempty())
	{
		promotedQ.pop(temp);
		stationpassengersForward.push(temp, temp->GetPassengerPriority());
	}

	while (!notpromotedQ.isempty())
	{
		notpromotedQ.pop(temp);
		stationpassengersForward.push(temp, temp->GetPassengerPriority());
	}

	while (!stationpassengersBackward.isempty())
	{
		stationpassengersBackward.pop(temp);
		if (curr_time - temp->getarrivetime() == temp->getmaxwait())
		{
			temp->UpgradePriority();
			promotedQ.push(temp);
		}
		else
			notpromotedQ.push(temp);
	}

	while (!promotedQ.isempty())
	{
		promotedQ.pop(temp);
		stationpassengersBackward.push(temp, temp->GetPassengerPriority());
	}

	while (!notpromotedQ.isempty())
	{
		notpromotedQ.pop(temp);
		stationpassengersBackward.push(temp, temp->GetPassengerPriority());
	}
}

Station::~Station()
{
}
