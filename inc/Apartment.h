#ifndef APARTMENT_H
#define APARTMENT_H

#include "Bed.h"
#include "Chair.h"
#include "Dashboard.h"
#include "Door.h"
#include "Server.h"
#include "TableLamp.h"
#include "Wall.h"
#include "Column.h"

class Apartment
{
private:
  Bed bed;
  Chair chair;
  Door door;
  TableLamp tableLamp;
  Wall wall;
  Column column;
  Dashboard dashboard;
  Server server = Server(8000);

public:
  Apartment();
  Bed *getBed();
  Chair *getChair();
  Door *getDoor();
  TableLamp *getTableLamp();
  Wall *getWall();
  Column *getColumn();
  Dashboard *getDashboard();
  Server getServer();
};

#endif // APARTMENT_H