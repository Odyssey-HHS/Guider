#ifndef DASHBOARD_H
#define DASHBOARD_H

class Dashboard {
    public:
        bool getDoor();
        int getLampColour();
        Dashboard &setDoor(bool);
        Dashboard &setLampColour(int);

        bool getLock();
        void lock();
        void unlock();
    private:
        bool openDoor = false;
        int lampColour = 0;
        bool lockFile = false;
};
#endif