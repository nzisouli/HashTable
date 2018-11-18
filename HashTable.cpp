
#include "HashTable.h"
#include "HashTableException.h"
#include <stdio.h>

using namespace std;

const string HashTable::getString(int pos) const{
  return table[pos];
}

int HashTable::getHashCode(const char *str) {
  int code = 0;
  while(*str != 0) {
    code += *(str++);
  }
  return code;
}

int HashTable::getSize() const { return size; }
int HashTable::getCapacity() const {return capacity; }

bool HashTable::isEmpty(int pos) const { 
  if(table[pos].empty()) 
    return true;
  return false;
}

bool HashTable::isTomb(int pos) const { 
  if(table[pos].compare("##tomb##")==0)
    return true; 
  return false;
}

bool HashTable::isAvailable(int pos) const { 
  if (isEmpty(pos) || isTomb(pos)){
    return true;
  }
  return false;
}

HashTable::HashTable(int capacity) {
  if(capacity < 0){
    bad_alloc *ba = new bad_alloc();
    throw *ba;
    return;
  }
  this->capacity = capacity;
  size = 0;
  table = new (nothrow) string[capacity];
  if (table == NULL){
    bad_alloc *ba = new bad_alloc();
    throw *ba;
    return;
  }
}

HashTable::HashTable(const HashTable &ht) {
  size = ht.getSize();
  capacity = ht.getCapacity();
  table = new (nothrow) string[capacity];
  if (table == NULL){
    bad_alloc *ba = new bad_alloc();
    throw *ba;
    return;
  }
  for(int i = 0; i < capacity; i++){
    table[i] = ht.getString(i);
  }
}

bool HashTable::contains(const string &s) const {
  const char *str = s.c_str();
  return contains(str);
}

bool HashTable::contains(const char *s) const {
  int hashcode = getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (table[pos].compare(s) == 0){
      return true;
    }
  }
  return false;
}

string HashTable::print() const {
  string str;
  char buf[128];
  int j=0;
/* Remove the following comment if you want 
 * to try the iterator version.
 */
//#define __USE_ITERATOR__
#ifdef  __USE_ITERATOR__
  for(HashTable::Iterator it = begin(); it!=end(); it++) {
    sprintf(buf, "%2d. -%s-\n", j++, (*it).c_str());
    str.append(buf);
  }
#endif
#ifndef __USE_ITERATOR__
  for(int i=0, j=0; i<capacity; i++)
    if(!isAvailable(i)) {
      sprintf(buf, "%2d. -%s-\n", j++, table[i].c_str());
      str.append(buf);
    }
#endif
  sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
  str.append(buf);
  return str;
}

bool HashTable::add(const string &str) {
  const char *s = str.c_str();
  return HashTable::add(s);
}

bool HashTable::add(const char *s) {
  if(contains(s)){
    return false;
  }
  int hashcode = getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (isAvailable(pos)){
      string str(s);
      table[pos] = str;
      size += 1;
      return true;
    }
  }
  HashTableException *hte = new HashTableException();
  cout << "THROW Exception!" << endl;
  throw *hte;
  return false;
}

bool HashTable::remove(const string &str) {
  const char *s = str.c_str();
  return HashTable::remove(s);
}

bool HashTable::remove(const char *s) {
  if(!contains(s)){
    return false;
  }
  int hashcode = getHashCode(s);
  for(int i = 0; i < capacity; i++){
    int pos = (hashcode + i)%capacity;
    if (table[pos].compare(s) == 0){
      string str("##tomb##");
      table[pos] = str;
      size -= 1;
      return true;
    }
  }
  return false;
}

HashTable& HashTable::operator=(const HashTable &ht) {
  if(table!=NULL)
    delete [] table;
  size = ht.getSize();
  capacity = ht.getCapacity();
  table = new (nothrow) string[capacity];
  if(table==NULL) {
    cerr << "Memory allocation failure!" << endl;
    exit(-1);
  }
  for(int i = 0; i < capacity; i++){
    table[i] = ht.getString(i);
  }
  return *this;
}


bool HashTable::operator += (const string &str) { 
  return HashTable::add(str);
}
bool HashTable::operator += (const char* s) { 
  return HashTable::add(s);
}
bool HashTable::operator -= (const string &str) {
  return HashTable::remove(str);
}
bool HashTable::operator -= (const char *s) { 
  return HashTable::remove(s);
}

HashTable HashTable::operator + (const string &str) const {
  HashTable new_ht = *this;
  new_ht += str;
  int count=0;
  for (int i=0; i< new_ht.getCapacity();i++){
  	if (new_ht.isAvailable(i)){
  		count ++;
  	}
  }
  if (count ==0){
  	HashTableException *hte = new HashTableException();
  	cout << "THROW Exception!" << endl;
  	throw *hte;
  }
  return new_ht;
}

HashTable HashTable::operator + (const char* s) const {
  HashTable new_ht = *this;
  string str(s);
  new_ht += str;
  int count=0;
  for (int i=0; i< new_ht.getCapacity();i++){
  	if (new_ht.isAvailable(i)){
  		count ++;
  	}
  }
  if (count ==0){
  	HashTableException *hte = new HashTableException();
  	cout << "THROW Exception!" << endl;
  	throw *hte;
  }
  return new_ht;
}
HashTable HashTable::operator - (const string &str) const {
  HashTable new_ht = *this;
  new_ht -= str;
  return new_ht;
}
HashTable HashTable::operator - (const char *s) const {
  HashTable new_ht = *this;
  string str(s);
  new_ht -= s;
  return new_ht;
}

HashTable HashTable::operator+(const HashTable &t) const {
  HashTable new_ht(this->getCapacity() + t.getCapacity());
  for(int i = 0; i < this->getCapacity(); i++){
    if(!this->isAvailable(i)){
      new_ht.add(this->getString(i));
    }
  }
  for(int i = 0; i < t.getCapacity(); i++){
    if(!t.isAvailable(i)){
      new_ht.add(t.getString(i));
    }
  }
  return new_ht;
}

HashTable& HashTable::operator+=(const HashTable &t) {
  *this = *this + t;
  return *this;
}

std::ostream& operator<<(std::ostream &os, HashTable &t) {
  os << t.print();
  return os;
}

HashTable::Iterator HashTable::begin() const {
	HashTable::Iterator it(this);
	return it;
}

HashTable::Iterator HashTable::end() const {
	HashTable::Iterator it(this, false);
	return it;
}