#include "enableModules.h"
#include "Apartment.h"

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "ModuleAddresses.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Apartment::Apartment()
{
  // Create an new server instance
  this->server = Server(8000);

  // Create new dashboard module
  this->dashboard = Dashboard();

  // Create a new connection to the Wemos board.
#ifdef USE_CHAIR
  std::cout << "Connecting to Chair.\n";
  Client chairClient(CHAIR_MODULE, 8080);
#endif

#ifdef USE_BED
  std::cout << "Connecting to Bed.\n";
  Client bedClient(BED_MODULE, 8080);
#endif

#ifdef USE_TABLELAMP
  std::cout << "Connecting to TableLamp..\n";
  Client lampClient(LAMP_MODULE, 8080);
#endif

#ifdef USE_DOOR
  std::cout << "Connecting to Door.\n";
  Client doorClient(DOOR_MODULE, 8080);
#endif
#ifdef USE_WALL
  std::cout << "Connecting to Wall.\n";
  Client wallClient(WALL_MODULE, 8080);
#endif

  // Create a new module using the connection created above.
#ifdef USE_BED
  bed = Bed(bedClient);
#endif
#ifdef USE_TABLELAMP
  tableLamp = TableLamp(lampClient);
#endif
#ifdef USE_DOOR
  door = Door(doorClient);
#endif
#ifdef USE_CHAIR
  chair = Chair(chairClient);
#endif
#ifdef USE_WALL
  wall = Wall(wallClient);
#endif
}

Bed *Apartment::getBed()
{
  return &this->bed;
}

Chair *Apartment::getChair()
{
  return &this->chair;
}

Door *Apartment::getDoor()
{
  return &this->door;
}

TableLamp *Apartment::getTableLamp()
{
  return &this->tableLamp;
}

Wall *Apartment::getWall()
{
  return &this->wall;
}

Dashboard *Apartment::getDashboard()
{
  return &this->dashboard;
}

Server *Apartment::getServer()
{
  return &this->server;
}