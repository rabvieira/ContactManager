/*
 * contactItem.cpp
 */

#include "contactItem.h"
#include <sstream>

contactItem::contactItem(){
	id = 0;
}

contactItem::contactItem(int id)
{
	this->id = id;
}

int contactItem::getId()
{
	return id;
}

string contactItem::getName()
{
	return name;
}

string contactItem::getSurname()
{
	return surname;
}

string contactItem::getEmail()
{
	return email;
}

string contactItem::getAddress()
{
	return address;
}

vector <phoneNumber> contactItem::getNumbers()
{
	return vPhoneNumber;
}

SearchScores contactItem::getScores()
{
	return scores;
}

void contactItem::setId(int id)
{
	this->id = id;
}

void contactItem::setName(string name)
{
	this->name = name;
}

void contactItem::setSurname(string surname)
{
	this->surname = surname;
}

void contactItem::setPhoneNumbers(vector<phoneNumber> vPhoneNumber)
{
	this->vPhoneNumber = vPhoneNumber;
}

void contactItem::setEmail(string email)
{
	this->email = email;
}

void contactItem::setAddress(string address)
{
	this->address = address;
}

void contactItem::setScores(SearchScores value)
{
	scores=value;
}
//registra todos os campos de um contato
void contactItem::registerContact()
{
	string field;
	phoneNumber p;
	vector <phoneNumber> vPhoneNumber;
	int opt = 1;

	cout << "Name: ";
	getline(cin,field);
	setName(field);

	cout << "Surname: ";
	getline(cin,field);
	setSurname(field);

	while(opt != 0){
		cout << "Number: ";
		getline(cin,field);
		p.number = field;
		cout << "Type: ";
		getline(cin,field);
		p.type = field;
		vPhoneNumber.push_back(p);
		setPhoneNumbers(vPhoneNumber);
		opt = readInteger("\nWould you like to add another number?\n[0] No\n[1] Yes\n:", 0,1);
	}

	cout << "Email: ";
	getline(cin,field);
	setEmail(field);

	cout << "Address: ";
	getline(cin,field);
	setAddress(field);

}


int contactItem::readInteger(string message, int min, int max)
{
	stringstream readInt;
	string auxReadInt;
	int num = -1;
	cout << message;
	while(true){
		fflush(stdin);
		num = -1;
		getline(cin,auxReadInt);
		readInt.str(auxReadInt);
		readInt >> num;
		if(num == -1){
			cout << "\n<<The number entered has an invalid format>>" <<endl;
			readInt.clear();
			auxReadInt.clear();
		}
		else 
			if(num < min || num > max){
				cout << "\n<<Number must be between " << min << " and " << max << ">>" << endl;
				readInt.clear();
				auxReadInt.clear();
			}
			else
				return num;

		cout << "\nReenter: ";
	}
}
