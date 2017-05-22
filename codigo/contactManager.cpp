/*
 * contactManager.cpp
 */

//----------------------------------------------------------

#include "contactManager.h"
#include "contactItem.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>    // std::sort

contactManager::contactManager()
{

}

contactManager::contactManager(string name, string filepath)
{
	this->name = name;
	this->filePath = filepath;
	findMaxResults = 5;
}

int contactManager::addContact(contactItem c)
{
	float minimumSimilarity = 0.5;
	for(list<contactItem>::iterator l = contacts.begin(); l != contacts.end(); l++){
		bool found = false;
		if(l->getName() == c.getName() && l->getSurname()==c.getSurname())
			return -1;
		else if(l->getEmail()==c.getEmail())
			return -3;
		else
		{
			for(int i=0;i<l->getNumbers().size();i++)
			{
				for(int j=0;j<c.getNumbers().size();j++)
				{
					if(l->getNumbers()[i].number == c.getNumbers()[j].number)
						return -2;
				}
			}
		}
	}

	int similarity = 0;
	if(true)
	{
		list<contactItem> results = contactManager::searchContact(c.getName(), SearchAttributes::All);
		for(list<contactItem>::iterator resultsIt = results.begin(); resultsIt != results.end(); resultsIt++){
			if(resultsIt->getScores().GetSum()>=minimumSimilarity)
			{
				cout << "Contact name appears to be related with one of the following contacts:" << endl;
				this->showContactList(results, 1);
				similarity=1;
			}
		}
	}

	if(true)
	{
		list<contactItem> results = contactManager::searchContact(c.getSurname(), SearchAttributes::All);
		for(list<contactItem>::iterator resultsIt = results.begin(); resultsIt != results.end(); resultsIt++){
			if(resultsIt->getScores().GetSum()>=minimumSimilarity)
			{
				cout << "Contact surname appears to be related with one of the following contacts:" << endl;
				this->showContactList(results, 1);
				similarity=2;
			}
		}
	}

	if(true)
	{
		for(int i=0;i<c.getNumbers().size();i++)
			{
				list<contactItem> results = contactManager::searchContact(c.getNumbers()[i].number, SearchAttributes::All);
				for(list<contactItem>::iterator resultsIt = results.begin(); resultsIt != results.end(); resultsIt++){
					if(resultsIt->getScores().GetSum()>=minimumSimilarity)
					{
						cout << "Contact phone appears to be related with one of the following contacts:" << endl;
						this->showContactList(results, 1);
						similarity=3;
					}
				}
		}
	}

	if(true)
	{
		list<contactItem> results = contactManager::searchContact(c.getEmail(), SearchAttributes::All);
		for(list<contactItem>::iterator resultsIt = results.begin(); resultsIt != results.end(); resultsIt++){
			if(resultsIt->getScores().GetSum()>=minimumSimilarity)
			{
				cout << "Contact email appears to be related with one of the following contacts:" << endl;
				this->showContactList(results, 1);
				similarity=4;
			}
		}
	}

	if(true)
	{
		list<contactItem> results = contactManager::searchContact(c.getAddress(), SearchAttributes::All);
		for(list<contactItem>::iterator resultsIt = results.begin(); resultsIt != results.end(); resultsIt++){
			if(resultsIt->getScores().GetSum()>=minimumSimilarity)
			{
				cout << "Contact address appears to be related with one of the following contacts:" << endl;
				this->showContactList(results, 1);
				similarity=5;
			}
		}
	}

	if(similarity!=0)
	{
		int opt = readInteger("Would you like to add the contact anyway?\n[0] No\n[1] Yes\n:", 0,1);
		if(opt==1)
		{
			contacts.push_back(c);
		}
	}
	else
	{
		contacts.push_back(c);
	}
	return 1;
}

bool contactManager::removeContact(int id)
{
	list<contactItem>::iterator l;

	if(contacts.empty()){
		cout << "There are no contacts to remove!" << endl;
		return false;
	}

	for(l = contacts.begin(); l != contacts.end(); l++){
		if(l->getId() == id){
			cout << "\n<<<Contact " << l->getName() << " " << l->getSurname() << " removed!>>>" << endl;
			contacts.erase(l);
			return true;
		}
	}

	cout << "\n<<<The selected ID does not exists>>>" << endl;
	return false;
}

//verificar se o contato existe(searchContact())
bool contactManager::updateContact(int id)
{
	bool exists = false;
	for(list<contactItem>::iterator l = contacts.begin(); l != contacts.end(); l++){
		if(l->getId() == id)
		{
			exists=true;
			break;
		}
	}
	
	if(!exists)
		return false;

	return true;
}

bool SortScoresHigherFirst(contactItem item1, contactItem item2)
{
    return item1.getScores().GetSumPriorities() > item2.getScores().GetSumPriorities();
}

list<contactItem> contactManager::searchContact(string pattern, SearchAttributes attributes)
{
	/*
	TODO: Limit results number
	if(nContactsFound >= findMaxResults)
			break;
	*/

	int nContactsFound = 0;
	bool caseSensitive = false;
	list<contactItem> foundContacts;
	
	for(list<contactItem>::iterator l = contacts.begin(); l != contacts.end(); l++){
		bool found = false;
		SearchScores scores;
		scores.NameScore = -1;
		scores.SurnameScore = -1;
		scores.PhoneScore = -1;
		scores.EmailScore = -1;
		scores.AddressScore = -1;

		if(attributes==SearchAttributes::All || attributes==SearchAttributes::Name)
		{
			int namePos = searchString(l->getName(), pattern, caseSensitive);
			if(namePos != -1)
			{
				//cout << "Found name" << endl;
				found=true;
				float pPosition = 1-((float)namePos / (float)l->getName().length());
				float pLength = (float)pattern.length() / (float)l->getName().length();

				scores.NameScore = pPosition*pLength;
			}
		}
		if(attributes==SearchAttributes::All || attributes==SearchAttributes::Surname)
		{
			int surnamePos = searchString(l->getSurname(), pattern, caseSensitive);
			if(surnamePos != -1)
			{
				//cout << "Found surname" << endl;
				found=true;
				float pPosition = 1-((float)surnamePos / (float)l->getSurname().length());
				float pLength = (float)pattern.length() / (float)l->getSurname().length();

				scores.SurnameScore = pPosition*pLength;
			}
		}

		if(attributes==SearchAttributes::All || attributes==SearchAttributes::Phone)
		{
			scores.PhoneScore=0;
			for(int i=0;i<l->getNumbers().size();i++)
			{
				int phonePos = searchString(l->getNumbers()[i].number, pattern, caseSensitive);
				if(phonePos != -1)
				{
					//cout << "Found number" << endl;
					found=true;
					float pPosition = 1-((float)phonePos / (float)l->getNumbers()[i].number.length());
					float pLength = (float)pattern.length() / (float)l->getNumbers()[i].number.length();
					scores.PhoneScore += pPosition*pLength;
				}
			}
		}

		if(attributes==SearchAttributes::All || attributes==SearchAttributes::Email)
		{
			int emailPos = searchString(l->getEmail(), pattern, caseSensitive);
			if(emailPos != -1)
			{
				//cout << "Found email" << endl;
				found=true;
				float pPosition = 1-((float)emailPos / (float)l->getEmail().length());
				float pLength = (float)pattern.length() / (float)l->getEmail().length();

				scores.EmailScore= pPosition*pLength;
			}
		}
		if(attributes==SearchAttributes::All || attributes==SearchAttributes::Address)
		{
			int addressPos = searchString(l->getAddress(), pattern, caseSensitive);
			if(addressPos != -1)
			{
				//cout << "Found address" << endl;
				found=true;
				float pPosition = 1-((float)addressPos / (float)l->getAddress().length());
				float pLength = (float)pattern.length() / (float)l->getAddress().length();

				scores.AddressScore = pPosition*pLength;
			}
		}

		l->setScores(scores);
		if(found)
		{
			//cout << l->getName() << " " << l->getSurname() << endl;
			//cout << "Score: " << scores.GetSumPriorities() << endl;
			//cout << "---" << endl;
			nContactsFound++;
			foundContacts.push_back(*l);
		}
		
		foundContacts.sort(&SortScoresHigherFirst);
	}
	
	while(foundContacts.size()>findMaxResults)
	{
		foundContacts.pop_back();
	}
	return foundContacts;
}

int contactManager::searchString(string srcStr, string pattern, bool caseSensitive)
{
	int algorithm = 3;

	switch(algorithm)
	{
	case 1:
		return searchStringNaive(srcStr, pattern, caseSensitive);
		break;
	case 3:
		return searchStringKMP(srcStr, pattern, caseSensitive);
		break;
	default:
		cout << "Invalid algorithm" << endl;
		return -1;
		break;
	}
}

int contactManager::searchStringNaive(string srcStr, string pattern, bool caseSensitive)
{
	int n = srcStr.length();
	int m = pattern.length();

	if(m>n)
		return -1;

	if(n==0 || m==0)
		return -1;

	for(int i=0;i<=n-m;i++)
	{
		bool found=true;
		for(int j=0;j<m;j++)
		{
			char c1 = srcStr[i+j];
			char c2 = pattern[j];

			if(!caseSensitive)
			{
				c1=tolower(c1);
				c2=tolower(c2);
			}

			if(c1 != c2)
			{
				found=false;
				break;
			}
		}

		if(found)
			return i;
	}
	return -1;
}

int* contactManager::KMPComputePrefixFunction(string pattern, bool caseSensitive)
{
	int n = pattern.size();
	int k = -1;
	int* pi = new int[10];

	pi[0] = k;
	for(int i = 1; i < n; i++)
	{
		char c1 = pattern[k+1];
		char c2 = pattern[i];

		if(!caseSensitive)
		{
			c1=tolower(c1);
			c2=tolower(c2);
		}

		while (k > -1 && c1 != c2)
			k = pi[k];
		if (c2 == c1)
			k++;
		pi[i] = k;
	}
	return pi;
}

int *KMPpi;
string lastKMPstr;
int contactManager::searchStringKMP(string srcStr, string pattern, bool caseSensitive)
{
	int n = srcStr.length();
	int m = pattern.length();

	int i;
	// int *KMPpi = KMPComputePrefixFunction(pattern, caseSensitive);
	int k = -1;

	if(lastKMPstr != pattern)
	{
		lastKMPstr=pattern;
		KMPpi = KMPComputePrefixFunction(pattern, caseSensitive);
	}

	if (!KMPpi)
		return -1;

	for(i=0; i<n; i++)
	{
		char c1 = srcStr[i];
		char c2 = pattern[k+1];

		if(!caseSensitive)
		{
			c1=tolower(c1);
			c2=tolower(c2);
		}
		while (k>-1 && c2!=c1)
			k = KMPpi[k];
		if (c1 == c2)
			k++;
		if (k == m - 1)
		{
			// free(KMPpi);
			return i-k;
		}
	}
	// free(KMPpi);
	return -1;
}

int contactManager::getNewContactId()
{
	int newContactId = 1;
	
	for(list<contactItem>::iterator l = contacts.begin(); l != contacts.end(); l++){
		int tempId = l->getId();
		if(tempId>newContactId)
			newContactId=tempId;
	}
	
	return ++newContactId;
}

void contactManager::showContactList(list<contactItem> c, int style)
{
	list<contactItem>::iterator l;
	cout << endl << "--- Contacts List ---" << endl;
	if(contacts.empty()){
		cout << "\nNo contacts to display" << endl;
		return;
	}

	switch(style)
	{
		case 1:
		{
			for(l = c.begin(); l != c.end(); l++){
				cout << "-----" << endl;
				cout << "Id: "  << l->getId() << endl;
				cout << "Name: "  << l->getName() << endl;
				cout << "Surname: "  << l->getSurname() << endl;
				cout << "Numbers: "  << endl;
				for(int i = 0; i < l->getNumbers().size(); i++){
					cout << "\t" <<l->getNumbers()[i].number << "\t" <<l->getNumbers()[i].type << endl;
				}
				cout << "Email: "  << l->getEmail() << endl;
				cout << "Address: "  << l->getAddress() << endl;
				cout << "-----" << endl;
				cout << endl;
			}
			break;
		}
		case 2:
			{
				for(l = c.begin(); l != c.end(); l++){
				cout << "Id: "  << l->getId() << " Name: "  << l->getName() << " " << l->getSurname();
				cout << " [";
				for(int i=0;i<l->getNumbers().size();i++)
				{
					cout << l->getNumbers().at(i).number << ((i<(l->getNumbers().size()-1))?",":"");
				}
				cout << "]";
				cout << " " << l->getEmail();
				//cout << " " << l->getAddress();
				cout << endl;

			}
				break;
			}
		default:
			cout << "Invalid display style" << endl;
			break;
	}
}

void contactManager::showAllContacts(){
	showContactList(contacts, 1);
	return;
}

void contactManager::interfaceManager()
{
	while(true){
		cout << "+---------------------------+" << endl;
		cout << "| Ultimate Contact Manager  |" << endl;
		cout << "+---------------------------+" << endl;
		cout << "[1] Add Contact" << endl;
		cout << "[2] Remove Contact" << endl; 
		cout << "[3] Show Contacts List" << endl; 
		cout << "[4] Search Contact" << endl;
		cout << "[5] Search Contact (fine)" << endl; 
		cout << "[8] Load Contact List" << endl;
		cout << "[9] Exit" << endl;
		cout << "+---------------------------+" << endl;
		int opt = readInteger("\nMenu Option: ",1,9);
		switch(opt){
		case 1:
			{	
				int newId = getNewContactId();
				contactItem c(newId);
				c.registerContact();
				int result = addContact(c);
				if(result<0)
				{
					cout << "You are not allowed to add this contact" << endl;
					switch(result)
					{
					case -1:
						cout << "Name already exists" << endl;
						break;
					case -2:
						cout << "Phone already exists" << endl;
						break;
					case -3:
						cout << "Email already exists" << endl;
						break;
					case 2:
						break;
					}
				}
				else
				{
					cout << "Contact added successfuly" << endl;
					updateContactsFile();
				}
			}
			break;
		case 2:
			{
				stringstream readInt;
				string auxReadInt;
				int id = 0;

				if(contacts.empty()){
					cout << "Contact list is empty. No contacts to remove." << endl;
				}
				else
				{
					while(id == 0){
						cout << "type the ID of the contact that you want remove:" << endl;
						//necessario para quando for digitado uma string no lugar de um inteiro.(caso seja realizada a leitura direta o programa para de funcionar corretamente!)
						getline(cin,auxReadInt);
						readInt.str(auxReadInt);
						readInt >> id;
						if(id == 0){
							cout << "\n<<The number entered has a invalid format>>\n" <<endl;
							readInt.clear();
							auxReadInt.clear();
							id = 0;
						}
					}
					removeContact(id);
					updateContactsFile();
				}
			}
			break;
		case 3:
			showAllContacts();
			break;
		case 4:
			{
				string pattern = "";
				cout << "--- Search Contact ---"<< endl;
				cout << "Info to search: ";
				getline(cin, pattern);
				this->showContactList(this->searchContact(pattern, SearchAttributes::All), 2);
				break;
			}
		case 5:
			{
				cout << "--- Search Contact (fine) ---"<< endl;
				
				string lastPattern = "";
				string instr;
				do
				{
					if(lastPattern!="")
						cout << "Search: " << lastPattern;
					else
						cout << "Search: ";

					getline(cin, instr);
					string pattern = lastPattern + instr;
					lastPattern=pattern;
					this->showContactList(this->searchContact(pattern, SearchAttributes::All), 2);
				} while(instr != "");
				break;
			}
		case 8:
			{
				readContactsFile();
				break;
			}
		case 9:
			{
				cout << "\nExit...\n\n" << endl;
				return;
				break;
			}
		default:
			cout << "\nInvalid selection. Please select a valid option!" << endl;
			break;
		}
		cout << "\n" << endl;
	}
}

//efetua a leitura do arquivo especificado por path e coloca as informaçoes dentro da instancia contact Manager
bool contactManager::readContactsFile()
{
	cout << "--- Read Contacts From File ---" << endl;
	ifstream fin;
	stringstream fieldWriter;
	string lineContent,Field;
	int commaPosition = 0;
	int beginOfSubstring = 0;
	bool createNewContact = true;
	contactItem newContact(0);
	int fillFieldControl = 0,intField;
	int pNumberInsertControl = 0;
	stringstream sstream;
	string aux;
	vector<phoneNumber> vPhoneNumber;
	phoneNumber pNumber;

	fin.open(filePath);

	if(fin.fail()){
		cout << "Unable to open the contacts file\n" << endl;
		return false;
	}

	contacts.clear();

	while(getline(fin, lineContent)){
		while(commaPosition != -1){
			commaPosition = lineContent.find_first_of('\t',commaPosition + 1);
			if(createNewContact){
				Field = lineContent.substr(beginOfSubstring, commaPosition - beginOfSubstring);
				intField = atoi(Field.c_str());
				createNewContact = false;
				newContact.setId(intField);
			}
			else{
				Field = lineContent.substr(beginOfSubstring, commaPosition - beginOfSubstring);
				switch(fillFieldControl){
				case 0:
					newContact.setName(Field);
					break;
				case 1:
					newContact.setSurname(Field);
					break;
				case 2:
					sstream << Field;
					while(sstream >> aux){
						if(pNumberInsertControl == 0){
							pNumber.number = aux;
							pNumberInsertControl++;
						}
						else{
							pNumber.type = aux;
							vPhoneNumber.push_back(pNumber);
							pNumberInsertControl--;
						}
					}
					newContact.setPhoneNumbers(vPhoneNumber);
					sstream.clear();
					aux.clear();
					vPhoneNumber.clear();
					break;
				case 3:
					newContact.setEmail(Field);
					break;
				case 4:
					newContact.setAddress(Field);
					break;
				default:
					break;
				}
				fillFieldControl++;
			}
			if(commaPosition != -1){
				beginOfSubstring = commaPosition + 1;
			}
		}
		contacts.push_back(newContact);
		commaPosition = 0;
		beginOfSubstring = 0;
		createNewContact = true;
		fillFieldControl = 0;
	}

	fin.close();
	return true;
}

//atualiza o ficheiro com a informação atual que consta em contacts(lista de contatos)
bool contactManager::updateContactsFile()
{
	ifstream fin;
	ofstream fout;
	fin.open(filePath);
	stringstream input("");
	string inputAux;
	list<contactItem>::iterator l;
	vector <phoneNumber> vPhoneNumber;

	if(fin.fail())
		cout << endl << "Unable to open the contacts file. A new one will be created."  << endl;

	for(l = contacts.begin(); l != contacts.end(); l++){
		input << l->getId() << "\t" << l->getName() << "\t" << l->getSurname() << "\t";
		vPhoneNumber = l->getNumbers();

		for(int i = 0; i < vPhoneNumber.size();i++){
			input << vPhoneNumber[i].number << " " << vPhoneNumber[i].type ;
			if(i+1 != vPhoneNumber.size() )
				input << " ";
		}
		input << "\t" << l->getEmail() << "\t" << l->getAddress() << endl;
	}

	fin.close();
	fout.open(filePath);
	if(fout.fail())
		cout << "There was a problem creating the new contacts file"  << endl;
	inputAux = input.str();
	fout << inputAux;
	fout.close();
	return true;
}

//metodo auxiliar para ler inteiros entre um intervalo
int contactManager::readInteger(string message, int min, int max)
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

//metodo auxiliar para ler strings
string contactManager::readString(string message, string option, int numOpt){
	vector<string> opt;
	string aux;
	int posIni=0, posEnd=0;
	string auxReadString;
	for(int i=0 ; i<numOpt ; i++){
		posEnd = option.find(",",posIni);
		aux = option.substr(posIni, posEnd-posIni);
		opt.push_back(aux);
		posIni = (posEnd-posIni) + 1;
	}
	cout << message;
	while(true){
		auxReadString.clear();
		fflush(stdin);
		getline(cin,auxReadString);
		for(int i=0 ; i<numOpt ; i++)
			if(auxReadString == opt[i])
				return opt[i]; 

		cout << "\n<<Acceptable answers: [ ";
		for (int i=0 ; i<numOpt ; i++){
			cout << opt[i];
			if(i<numOpt-1)
				cout << " or ";
		}
		cout << " ]>>\n" << "\nReenter: ";
	}
}

