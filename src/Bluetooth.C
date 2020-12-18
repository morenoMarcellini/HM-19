/* Moreno Marcellini*/

/* C++ headers */
#include <string>
#include <fstream>

/* C headers */
#include <unistd.h>
#include <sys/stat.h>

/* namespace */
using namespace std;

/* Local headers */
#include "Bluetooth.H"

/* Constructor */
BLE::BLE() {
}

BLE::~BLE() {
}

bool BLE::queryDevice(const string& device) {

    return !access(device.c_str(), F_OK);
}

bool BLE::exists_test0 (const std::string& name) {

    ifstream f(name.c_str());

    return f.good();
}

bool BLE::exists_test1 (const std::string& name) {

    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

bool BLE::exists_test2 (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

bool BLE::exists_test3 (const std::string& name) {

    struct stat buffer;

    return (stat (name.c_str(), &buffer) == 0);
}
