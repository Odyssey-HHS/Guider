#ifndef APARTMENT_H
#define APARTMENT_H

#include "Bed.h"
#include "Chair.h"
#include "Dashboard.h"
#include "Door.h"
#include "Server.h"
#include "TableLamp.h"
#include "Wall.h"

class Apartment
{
private:
  Bed bed;
  Chair chair;
  Door door;
  TableLamp tableLamp;
  Wall wall;
  Dashboard dashboard;
  Server server;

public:
  Apartment();
  Bed *getBed();
  Chair *getChair();
  Door *getDoor();
  TableLamp *getTableLamp();
  Wall *getWall();
  Dashboard *getDashboard();
  Server *getServer();
};

#endif // APARTMENT_H