
#include "HashTable.h"
#include "HashTableException.h"
#include <stdio.h>

using namespace std;

string* HashTable::Iterator::getCurr() const{
	return curr;
}

const HashTable* HashTable::Iterator::getHt() const{
	return ht;
}

bool HashTable::Iterator::getEnd() const{
	return end;
}

HashTable::Iterator::Iterator(const HashTable *t) {
	end = false;
	ht = new HashTable(*t);
	for (int i = 0; i < t->getCapacity(); i++){
		if(!ht->isAvailable(i)){
			curr = new string(t->getString(i));
			break;
		}
	}
}

HashTable::Iterator::Iterator(const HashTable *t, bool start){
	ht = new HashTable(*t);
	if(start){
		end = false;
		for (int i = 0; i < t->getCapacity(); i++){
			if(!ht->isAvailable(i)){
				curr = new string(t->getString(i));
				break;
			}
		}
	}
	else{
		end = true;
		curr = NULL;
	}
}

HashTable::Iterator::Iterator(const HashTable::Iterator &it) {
	ht = new HashTable(*it.getHt());
	curr = new string(*it.getCurr());
	end = it.end;
}

HashTable::Iterator& HashTable::Iterator::operator=(const HashTable::Iterator &it) {
	ht = new HashTable(*it.getHt());
	curr = new string(*it.getCurr());
	end = it.end;
	return *this;
}

HashTable::Iterator HashTable::Iterator::operator++() {
 
  int i;
  for (i = 0; i < ht->getCapacity(); i++){
    if(ht->table[i] == *curr && curr!= NULL){
      break;
    }
  }
  
  if(i+1 == ht->getCapacity()){
    end = true;
    curr = NULL;
  }
  for (int j = i+1; j < ht->getCapacity(); j++){
    if(!ht->isAvailable(j)){
      curr = new string(ht->getString(j));
      break;
    }
    if(j == ht->getCapacity()-1){
      end = true;
      curr = NULL;
   }
  }
  Iterator old(*this);

  return old;


}

HashTable::Iterator HashTable::Iterator::operator++(int a) {
  Iterator old(*this);
  int i;
  for (i = 0; i < ht->getCapacity(); i++){
  	if(ht->table[i] == *curr){
  		break;
  	}
  }
  if(i+1 == ht->getCapacity()){
	 end = true;
	 curr = NULL;
  }
  for (int j = i+1; j < ht->getCapacity(); j++){
  	if(!ht->isAvailable(j)){
		  curr = new string(ht->getString(j));
		  break;
	  }
	  if(j == ht->getCapacity()-1){
		  end = true;
		  curr = NULL;
	  }
  }
  return old;
}

bool HashTable::Iterator::operator==(const HashTable::Iterator &it) const {
  HashTable it_ht(*it.getHt());
  if (!(ht->getCapacity() == it_ht.getCapacity() && ht->getSize() == it_ht.getSize())){
  	return false;
  }
  for (int i = 0; i < ht->getCapacity(); i++){
  	if(ht->getString(i) != it_ht.getString(i)){
  		return false;
  	}
  }

  if(*curr != *it.getCurr()){
  	return false;
  }

  if(end != it.getEnd()){
  	return false;
  }  

  return true;
}

bool HashTable::Iterator::operator!=(const HashTable::Iterator &it) const {
  HashTable it_ht(*it.getHt());
  bool check1 = true;
  if (ht->getCapacity() == it_ht.getCapacity() && ht->getSize() == it_ht.getSize()){
  	check1 = false;
  }

  bool check2 = true;
  for (int i = 0; i < ht->getCapacity(); i++){
  	if(ht->getString(i) == it_ht.getString(i)){
  		check2 = false;
  		break;
  	}
  }

  bool check3 = true;
  if (curr == NULL && it.getCurr() == NULL){
  	check3 = false;
  }
  else if(curr == NULL || it.getCurr() == NULL){

  }
  else if(*curr == *it.getCurr()){
  	check3 = false;
  }

  bool check4 = true;
  if(end == it.getEnd()){
  	check4 = false;
  }
  return check1 || check2 || check3 || check4;
}

const string& HashTable::Iterator::operator*() {
  return *curr;
}

const string* HashTable::Iterator::operator->() {
  return curr;
}