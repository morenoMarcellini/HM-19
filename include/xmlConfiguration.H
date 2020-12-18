/* Moreno Marcellini, 2020-12-15 */

/* C++ headers */
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

/* C headers */
#include <libxml/xmlreader.h>

class XML {

private:
  virtual void parseStory(xmlDocPtr doc, xmlNodePtr cur);
  virtual void parseQueries(xmlDocPtr doc, xmlNodePtr cur);
  virtual void parseAnswers(xmlDocPtr doc, xmlNodePtr cur);

  uint32_t c_cmds=0, c_help=0, c_code=0, c_special=0;

public:
  /* Constructor */
  XML();
  
  /* Destructor */
  ~XML();

  /* Methods */
  virtual bool parsing(const string& file, const string& what);
  virtual bool find(const string& str);
  virtual bool specials(const string& str);
  
  //virtual bool simple_parsing(const string& filename);

  map<string, int32_t> query;
  map<string, int32_t> answer;
  map<string, string> help;
  map<string, int32_t> special;

  map<string, int32_t>::iterator lastCmd;
  map<string, string>::iterator lastHelp;

};
