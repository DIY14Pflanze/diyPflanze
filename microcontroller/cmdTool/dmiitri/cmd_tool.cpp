#include "SerialClass.h"
#include <iostream>

using namespace std;
//using namespace Serial;
int com_port;

Serial* SP;
//string port;


int main(){
	
private char[] port;
cout << "Gib die Portnummer des COM-Ports ein, an der Dein Arduino haengt und drueck ENTER: \n";
cin >> com_port;


switch(com_port){

case 1:
    port = "COM1";
break;
case 2:
    port = "COM2";
break;
case 3:
    port = "COM3";
break;
case 4:
    port = "COM4";
break;
case 5:
    port = "COM5";
break;
case 6:
    port = "COM6";
break;
case 7:
    port = "COM7";
break;
case 8:
    port = "COM8";
break;
case 9:
    port = "COM9";
break;
}

SP = new Serial(port);

if(SP->IsConnected() == true){
    cout << "Die Verbindung zu " << port << " wurde erfolgreich hergestellt! \n";
    cout << "Was moechtest Du tun? \n";
    cout << "1.  "
}else{
    cout << "Der Verbindungsaufbau ist fehlgeschlagen!";

}











}
