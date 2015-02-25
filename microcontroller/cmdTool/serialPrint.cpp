#include <iostream>
#include "include/Serial.h"

int main(){
	
	CSerial test;
	
	if (test.Open(6, 9600)){
	
	std::cout << "Verbindung offen" << std::endl;
	}{
		
	std::cout << "Verbindungsfehler" << std::endl;	
	}
	
	return 0;
}