#include <QCoreApplication>
#include <iostream>
#include <bitset>
#include <string>
#include "des.h"

using namespace std;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    cout << "Enter key(min 16 simbols): ";
    Des des;

    //des.encrypt("12345678", "qwerty12");
    des.decrypt("12345678", "8d7bd49c9aa5652a");
    //return a.exec();
}
