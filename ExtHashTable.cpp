#include <iostream>
#include <cstdlib>
#include <string.h>

#include "ExtHashTable.h"

using namespace std;


ExtHashTable::ExtHashTable( double upper_bound_ratio, 
                            double lower_bound_ratio, 
                            int size) : HashTable(size) {
  	
  this->upper_bound_ratio = upper_bound_ratio;
  this->lower_bound_ratio = lower_bound_ratio;
}
  
ExtHashTable::ExtHashTable(const ExtHashTable &t) : HashTable(t) {
  upper_bound_ratio = t.getUpper();
  lower_bound_ratio = t.getLower();
}

double ExtHashTable::getUpper() const{
	return upper_bound_ratio;
}
 
double ExtHashTable::getLower() const{
	return lower_bound_ratio;
}

void ExtHashTable::rehash() {
  string *old_table = new string[capacity];
  for (int i = 0; i < capacity; i++){
  	old_table[i] = table[i];
  }

  if((double)size/(double)capacity > upper_bound_ratio){
  	delete [] table;
  	table = new string[capacity*2];
    size = 0;
  	for (int i = 0; i < capacity; i++){
    	if (!old_table[i].empty() && (old_table[i].compare("##tomb##")!=0)){
    	  HashTable::add(old_table[i]);
    	}
  	}
  	capacity = capacity*2;
  	cout << "--> Size: " << size << ", New capacity: " << capacity << endl;
  }
  else if((double)size / (double)capacity < lower_bound_ratio ){
  	delete [] table;
  	table = new string[capacity/2];
    capacity = capacity/2;
    size = 0;
  	for (int i = 0; i < capacity*2; i++){
  		if (!old_table[i].empty() && (old_table[i].compare("##tomb##")!=0)){
  			HashTable::add(old_table[i]);
  		}
  	}
    cout << "--> Size: " << size << ", New capacity: " << capacity << endl;
  }
}

bool ExtHashTable::add(const string &str) {
  const char *s = str.c_str();
  if(HashTable::contains(s)){
    return false;
  }
  bool check = false;
  int hashcode = HashTable::getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (HashTable::isAvailable(pos)){
      //string str(s);
      table[pos] = str;
      size += 1;
      check = true;
      break;
    }
  }
  rehash();
  if(check == false){
  	HashTable::add(str);
    return true;
  }
  return check;
}

bool ExtHashTable::add(const char *s) {
  if(HashTable::contains(s)){
    return false;
  }
  bool check = false;
  int hashcode = HashTable::getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (HashTable::isAvailable(pos)){
      string str(s);
      table[pos] = str;
      size += 1;
      check = true;
      break;
    }
  }
  rehash();
  if(check == false){
  	HashTable::add(s);
    return true;
  }
  return check;
}

bool ExtHashTable::remove(const string &str) {
  const char *s = str.c_str();
  if(!HashTable::contains(s)){
    return false;
  }
  bool check = false;
  int hashcode = HashTable::getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (table[pos].compare(s) == 0){
      string st("##tomb##");
      table[pos] = st;
      size -= 1;
      check = true;
      break;
    }
  }
  if(check == true){
  	rehash();
  	return true;
  }
  return false;
}

bool ExtHashTable::remove(const char *s) {
  if(!HashTable::contains(s)){
    return false;
  }
  bool check = false;
  int hashcode = HashTable::getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (table[pos].compare(s) == 0){
      string st("##tomb##");
      table[pos] = st;
      size -= 1;
      check = true;
      break;
    }
  }
  if(check == true){
  	rehash();
  	return true;
  }
  return false;
}

ExtHashTable ExtHashTable::operator+(const string &str) const{
	ExtHashTable new_exHashTable = ExtHashTable(*this);
	new_exHashTable.add(str);
	return new_exHashTable;
}

ExtHashTable ExtHashTable::operator+(const char* s) const{
	ExtHashTable new_exHashTable = ExtHashTable(*this);
	new_exHashTable.add(s);
	return new_exHashTable;
}
  
ExtHashTable ExtHashTable::operator-(const string &str) const{
	ExtHashTable new_exHashTable = ExtHashTable(*this);
	new_exHashTable.remove(str);
	return new_exHashTable;
}

ExtHashTable ExtHashTable::operator-(const char *s) const{
	ExtHashTable new_exHashTable = ExtHashTable(*this);
	new_exHashTable.remove(s);
	return new_exHashTable;
}

bool ExtHashTable::operator += (const string &str) {
	return add(str);
  
}
bool ExtHashTable::operator += (const char* s) { 
	return add(s);
}
bool ExtHashTable::operator -= (const string &str) {
  return remove(str);
}
bool ExtHashTable::operator -= (const char *s) {
  return remove(s);
}

ExtHashTable ExtHashTable::operator+(const ExtHashTable &table) const {
  ExtHashTable new_exHashTable = ExtHashTable(*this);

  for (int i = 0; i < table.getCapacity(); i++){
  	if(!table.isAvailable(i)){
  		new_exHashTable.add(table.getString(i));
  	}
  }
  return new_exHashTable;
}

ExtHashTable & ExtHashTable::operator+=(const ExtHashTable &table) {
  for (int i = 0; i < table.getCapacity(); i++){
  	if(!table.isAvailable(i)){
  		add(table.getString(i));
  	}
  }
  return *this;
}

ExtHashTable & ExtHashTable::operator=(const ExtHashTable &t) {
  if(table!=NULL)
    delete [] table;
  size = t.getSize();
  capacity = t.getCapacity();
  table = new (nothrow) string[capacity];
  if(table==NULL) {
    cerr << "Memory allocation failure!" << endl;
    exit(-1);
  }
  for(int i = 0; i < capacity; i++){
    table[i] = t.getString(i);
  }
  upper_bound_ratio = t.getUpper();
  lower_bound_ratio = t.getLower();
  return *this;
}