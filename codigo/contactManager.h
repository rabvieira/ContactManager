/*! Classe representativa do Gerenciador de Contatos */

#ifndef CONTACTMANAGER_H_
#define CONTACTMANAGER_H_

#include "contactItem.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>
using namespace std;
const int nTopResults=5;

/** Classe que representa o gestor de contactos */
class contactManager {
private:
	// Configs
	string filePath;
	int findMaxResults;

	string name;
	
	list<contactItem> contacts;
	int readInteger(string message, int min, int max);
	string readString(string message, string option, int numOpt);

	int searchString(string srcStr, string textToFind, bool caseSensitive);
	int searchStringNaive(string srcStr, string textToFind, bool caseSensitive);
	int searchStringKMP(string srcStr, string textToFind, bool caseSensitive);
	int* KMPComputePrefixFunction(string pattern, bool caseSensitive);

public:
	/** Estrutura de dados que representa o tipo de pesquisa a efectuar */
	enum SearchAttributes
	{
		All,  /**< Pesquisar por todos os atributos */
		Name, /**< Pesquisar por nome */
		Surname, /**< Pesquisar por sobrenome */
		Phone, /**< Pesquisar por telefone */
		Email, /**< Pesquisar por email */
		Address /**< Pesquisar por morada */
	};

	/*!
      Construtor por defeito
    */
	contactManager();

	/*!
      Construtor
	  \param name Nome da lista de contactos
	  \param filepath Path onde a lista de contactos deverá ser guardada
    */
	contactManager(string name, string filepath);
	/*!
      Adiciona um contacto à lista de contactos
	  \param contact Contacto a adicionar
	  \return Retorna 1 se o contacto tiver sido adicionado com sucesso. Caso contrário retorna um número negativo dependendo do tipo de erro
    */
	int addContact(contactItem contact); //fazer verificações se os campos ja existem, sugerir fusões
	/*!
      Remove um contacto da lista de contactos
	  \param id O Id do contacto a remover
	  \return Retorna verdadeiro se o contacto for removido com sucesso
    */
	bool removeContact(int id);

	/*!
      Actualiza um contacto da lista de contactos
	  \param id Id do contacto a actualizar
	  \return Retorna verdadeiro se o contacto for actualizado com sucesso
    */
	bool updateContact(int id);

	/*!
      Pesquisa um contacto através de todos os seus atributos
	  \param textToFind Conteúdo a procurar nos atributos do contacto
	  \param attributesToSearch Em que atributos pesquisar
	  \return Retorna uma lista ordenada com os contactos que mais se assemelham ao conteúdo da pesquisa
    */
	list<contactItem> searchContact(string textToFind, SearchAttributes attributesToSearch);

	/*!
      Mostra a lista de contactos e respectivos atributos textualmente
	  \param contacts A lista de contactos
	  \param style O estilo da lista. 1=Completo, 2=Compacto
    */
	void showContactList(list<contactItem> contacts, int style);

	/*!
      Lê a lista de contactos a partir de um ficheiro
	  \return Retorna verdadeiro se o ficheiro de contactos tiver sido lido com sucesso
    */
	bool readContactsFile();

	/*!
      Guarda a lista de contactos num ficheiro
	  \return Retorna verdadeiro se a lista de contactos tiver sido guardada com sucesso
    */
	bool updateContactsFile();

	/*!
      Interface de interacção com a aplicação
    */
	void interfaceManager();

	/*!
      Obtém o próximo id de contacto disponível
	  \return Retorna um Id de contacto que esteja disponível
    */
	int getNewContactId();
	
	/*!
      Mostra todos os contactos da lista
    */
	void showAllContacts();
};

#endif