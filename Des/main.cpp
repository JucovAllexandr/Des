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

    des.encode("987654321", "133457799BBCDFF1");
    //return a.exec();
}
