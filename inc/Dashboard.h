#ifndef DASHBOARD_H
#define DASHBOARD_H

class Dashboard {
    public:
        bool getDoor();
        int getLampColor();
        Dashboard &setDoor(bool);
        Dashboard &setLampColor(int);
        std::string getJson();

        bool getLock();
        void lock();
        void unlock();
    private:
        bool openDoor = false;
        int lampColor = 0;
        bool lockFile = false;
};
#endif