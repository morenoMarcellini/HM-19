/* Moreno Marcellini, 2020-12-16 */

/* C++ headers */
#include <algorithm>

/* local headers */
#include "SerialPort.H"
#include "xmlConfiguration.H"
#include "Bluetooth.H"

/* namespace */
//using namespace mn::CppLinuxSerial;
using namespace std;

int main(int argc, char **argv) {

	string readData;
	string init = "AT";
	const string ok = "OK";
	string base = "AT+";
	string dev0 = "/dev/serial0";
	string dev;
	string toBeWritten, output;
	bool go = false;
	string boh;

	/* Helpers */
	BLE ble;

	/* parsing the command line */
	cout << argc << endl;

	if (argc > 1) {
		/* the device to open is argv[1] */
		dev = argv[1];

		/* check if dev exist */
		if (ble.queryDevice(dev) == false ) {
			cerr << "Device " << dev << " does not exist. Opening /dev/serial0? [Y/n] ";
			cin >> boh;
			/* Everything to capital letter */
			transform(boh.begin(), boh.end(), boh.begin(), static_cast<int (*)(int)>(std::toupper));
			// cout << boh << endl;
			if (!boh.compare("Y")) {
				dev = dev0;
			}
			else {
				return -1;
			}
		}
	}
	else {
		dev = dev0;
	}

	/* reading the xml file with queries and settings */
	XML query;

	if (!query.parsing("query.xml", "queries")) {
		cerr << "You have a problem\n";
		return -1;
	}

	if (!query.parsing("setting.xml", "answers")) {
		cerr << "You have a problem\n";
		return -1;
	}

	/* Create serial port object and open serial port */
	cout << "Opening device " << dev << " @ 9600,8,N,1\n";
	CppLinuxSerial::SerialPort serialPort(dev, CppLinuxSerial::BaudRate::B_9600);
	serialPort.SetTimeout(0); // -1 Block when reading until any data is received
	                           // 0 Not blocking read
	serialPort.Open();

	// Initialization with "AT"
	serialPort.Write(init);

	// Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above
	serialPort.Read(readData);
	cout << readData << endl;

	if (readData.compare(ok) == 0) {
		cout << "Line is OK " << readData << endl;
	}
	else {
		cerr << "You have a problem\n";
	}
#if 0
	cout << "Initialization with ""AT"" " << endl;
	serialPort.Write("AT");

	// Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above
	serialPort.Read(readData);
	cout << readData << endl;

	if (!readData.compare(ok)) {
		cout << "You are " << readData << endl;
	}
	else {
		cerr << "You have a problem\n";
	}
#endif

	/* Parsing the commands */
	while (1) {
          boh.clear();
		cout << "Query/setting ";
		cin >> boh;
                cout << "boh " << boh << endl;

		/* Everything to capital letter */
		transform(boh.begin(), boh.end(), boh.begin(), static_cast<int (*)(int)>(std::toupper));
		// cout << boh << endl;

		if (!boh.compare("!QUIT")) {
			break;
		}
                else if (!boh.compare("AT")) {
			cout << "Resetting " << endl;
                         /* AT is a special command */
			serialPort.Write("AT");
			cout << "Waiting the answer ..." << endl;
			output.clear(); // cleaning output
                	serialPort.Read(readData);
			cout << readData << endl;
                        go = false;
		}
                else if (query.find(boh)) {
			cout << "Command found " << endl;
                        go = true;
		}
		else if (query.specials(boh)) {
                  cout << "Special commands " << endl;
		}
		else {
			cerr << "Command not found" << endl;
			go = false; //break; //return -1;
		}

		toBeWritten.clear();
		/* Write the command */
		if (go) {
		  /* Addind AT+ to the command */
                  toBeWritten = toBeWritten.append(base);
		  toBeWritten = toBeWritten.append(boh);

		 cout << "toBeWritten " << toBeWritten << endl;
		  serialPort.Write(toBeWritten);
		  cout << "Waiting the answer ..." << endl;
		  output.clear(); // cleaning output
		  while (1) {
			/* Read some data back (will block until at least 1 byte is received
			 due to the SetTimeout(-1) call above
			 The serial port or the modem is buffered at 8 bytes at the time, therefore
			 we read 8 bytes and repeat until readData is less than 8 */
			serialPort.Read(readData);
			cout << readData << flush;
			output.append(readData);
			if (readData.length() < 8) break;
		}
                serialPort.Read(readData);
		cout << endl;
		if (!output.compare(0, 2, ok)) {
			cout << output << endl;
		}
		else {
			cerr << "something was wrong: you asked " << toBeWritten << "\n"
				<< "check the .xml files and the manual" << endl;
		}
		}

	}
	// Close the serial port
	serialPort.Close();

	return 0;
}
