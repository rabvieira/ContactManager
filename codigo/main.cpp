#include <iostream>
#include "contactItem.h"
#include "contactManager.h"

using namespace std;

int main(){
	contactManager agenda("Agenda", "ContactsFile.txt");
	agenda.readContactsFile();
	agenda.interfaceManager();
	return(0);
}
