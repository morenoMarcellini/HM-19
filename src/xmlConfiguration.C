/* Moreno Marcellini, 2020-12-15 */

/* C++ headers*/
#include <map>

/* C headers */
#include <cstdlib>

/* local headers */
#include "xmlConfiguration.H"

using namespace std;

/* constructor */
XML::XML() {
  
}

/* Destructor */
XML::~XML() {

}

bool XML::parsing(const string& filename, const string& what) {

  xmlDocPtr doc;
  xmlNodePtr cur;

  c_cmds=0;
  c_help=0;
  c_code=0;
 
  /* Open the file */
  doc = xmlParseFile(filename.c_str());

  if (doc == NULL) {
    cerr << "filename " << filename << " does not exist\n";
    return false;
  }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL) {
    cerr << "filename " << filename << " is empty\n";
    xmlFreeDoc(doc);
    return false;
  }
  
  if (xmlStrcmp(cur->name, (const xmlChar*) what.c_str())) {
    cerr << "root does not contain--->" << what << "\n";
    xmlFreeDoc(doc);
    return false;
  }

  /* We point to first child */
  cur = cur->xmlChildrenNode;

#if 1
  if (!what.compare("queries")) {
    // cout << "what.compare(""queries"")\n";
    parseQueries(doc, cur);
  }
  else if (!what.compare("answers")) {
    // cout << "what.compare(""answers"")\n";
    parseAnswers(doc, cur);
  }
  else {
    cerr << "We have a problem\n";
    return false;
  }
      
#else 
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "query"))) {
      cout << "found ";
      parseStory(doc, cur);
    }
    cur = cur->next;
  }
#endif
  /* ending */
  xmlFreeDoc(doc);

  cout << "Number of queries  " << query.size() << endl;
  cout << "Number of setting  " << answer.size() << endl;
  cout << "Number of helps    " << help.size() << endl;
  cout << "Number of specials " << special.size() << endl;

#if 0
#if 0 /* logging out */
  for (auto& x: query) {
    cout << x.first << ':' << x.second << endl;
  }
#else
  map<string, int32_t>::iterator it;
  for (it = query.begin(); it != query.end(); it++) {
    cout << it->first << ':' << it->second << endl;
  }
#endif
#endif /* end of logging out */
  return true;
}

//
/* method  find */
//

bool XML::find(const string& str) {
#if 0
  for  (uint32_t i=0; i < str.length(); i++)
    cout << str[i] << endl;
#endif
  if (str[str.length()-1] == '?') {
    cout << "got here cmd" << endl;
    lastCmd = query.find(str);
    if (lastCmd != query.end()) {
      cout << lastCmd->first << ':' << lastCmd->second << endl;
      return true;
    }
//    cout << lastCmd->first << "<--->" << lastCmd->second << endl;
    return false;
  }	 
  else if (str[0] == '?') {
    cout << "got here help" << endl;
    lastHelp = help.find(str);
    if (lastHelp != help.end()) {
      cout << lastHelp->first << ':' << lastHelp->second << endl;
      return true;
    }
    return false;
  }
  else {
    lastCmd = answer.find(str);
    cout << "got here answer" << endl;
    if (lastCmd != answer.end()) {
      cout << lastCmd->first << ':' << lastCmd->second << endl;
      return true;
    }
    return false;
  }

  return false;
}

bool XML::specials(const string& str) {

  string test;

  test = str.substr(0, 4);
  cout << "got here special" << endl;
  lastCmd = special.find(test);
  if (lastCmd != special.end()) {
    cout << lastCmd->first << ':' << lastCmd->second << endl;
    return true;
  }

  return false;
}

/* Private */
void XML::parseStory(xmlDocPtr doc, xmlNodePtr cur) {

  xmlChar *key_cmds, *key_help, *key_code, *key_special;
  string t_cmd, t_hlp, t_test, t_special;
  int32_t t_code;
    
  cur = cur->xmlChildrenNode;

  while (cur != NULL) {
    // cout << "cur->name " << cur->name << endl;
    /* fetching cmds */
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "cmds"))) {
      key_cmds = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      //      cout << key_cmds << endl;
      t_cmd = string((const char*) key_cmds);
      c_cmds++;
    }
    
    /* fetching code */
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "code"))) {
      int32_t t;
      key_code = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      // cout << key_code << endl;
      // t_code = stoi(string((const char*) key_code));
      t_code = atoi((const char*) key_code);
      xmlFree(key_code);
      c_code++;
    }

    /* fetching help */
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "help"))) {
      key_help = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      t_hlp = string((const char*) key_help);
      // cout << t_hlp << endl;
      xmlFree(key_help);
      c_help++;
    }

    /* fetching special */
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "special"))) {
      key_special = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      t_special = string((const char*) key_special);
      xmlFree(key_special);
      c_special++;
    }

    if (t_cmd[t_cmd.length()-1] == '?') {
	query.emplace(t_cmd, t_code);
    }
    else {
	answer.emplace(t_cmd, t_code);
    }

    /* Univocal help */
    t_test = string("?").append(t_cmd);
#if 0
    if (t_test[t_test.length()-1] == '?') {
	t_test = t_test.substr(0, t_test.length()-2);
        cout << '+' << t_test << endl;
    }
#endif

    if (!(help.find(t_test) != help.end())) {
      help.emplace(string("?").append(t_cmd), t_hlp);
    }

    if (!t_special.compare("1")) {
      special.emplace(t_cmd, t_code);
      // cout << t_cmd << endl;
    }

    cur = cur->next;
  }

  return;
}

void XML::parseQueries(xmlDocPtr doc, xmlNodePtr cur) {

  while (cur != NULL) {
    //    cout << cur->name << endl;
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "query"))) {
      //  cout << "found ";
      parseStory(doc, cur);
    }
    cur = cur->next;
  }
#if 0
  cout << "Queries:cmds " << c_cmds << "\n"
       << "Queries:code " << c_code << "\n"
       << "Queries:help " << c_help << endl;
#endif
  return;
}

void XML::parseAnswers(xmlDocPtr doc, xmlNodePtr cur) {
  
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "answer"))) {
      // cout << "found ";
      parseStory(doc, cur);
    }
    cur = cur->next;
  }
#if 0
  cout << "Queries:cmds " << c_cmds << "\n"
       << "Queries:code " << c_code << "\n"
       << "Queries:help " << c_help << endl;
#endif
  return ;
}
