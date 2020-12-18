/* Moreno Marcellini, 2020-12-15 */

/* C++ headers */
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

/* C headers */
#include <libxml/xmlreader.h>

/* Various helper */
class BLE {

private:

public:
  /* Constructor */
  BLE();
  
  /* Destructor */
  ~BLE();

  /* Methods */
  virtual bool queryDevice(const string& dev);
virtual bool exists_test0 (const string& name);
virtual bool exists_test1 (const string& name);
virtual bool exists_test2 (const string& name);
virtual bool exists_test3 (const string& name);

};
