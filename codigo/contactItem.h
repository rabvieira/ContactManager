/*! Classe representativa do registro */

#ifndef CONTACTITEM_H_
#define CONTACTITEM_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/** Estrutura de dados que guarda os números de telefone de um contacto */
struct phoneNumber{
	string number, /**< Número de telefone */
	type; /**< Tipo de número */
};

/** Estrutura de dados que guarda o resultado das pesquisas por cada atributo */
struct SearchScores
{
public:
	float NameScore; /**< Pontuação do nome */
	float SurnameScore;  /**< Pontuação do sobrenome */
	float PhoneScore;  /**< Pontuação do telefone */
	float EmailScore;  /**< Pontuação do email */
	float AddressScore;  /**< Pontuação da morada */

	/*!
      Soma das pontuações
    */
	float GetSum() 
	{
		return (NameScore==-1?0:NameScore) + (SurnameScore==-1?0:SurnameScore) + (PhoneScore==-1?0:PhoneScore) + (EmailScore==-1?0:EmailScore) + (AddressScore==-1?0:AddressScore);
	}

	/*!
      Soma das pontuações tendo em conta a prioridade dos atributos
    */
	float GetSumPriorities()
	{
		return (NameScore==-1?0:NameScore)*1 + (SurnameScore==-1?0:SurnameScore)*0.9 + (PhoneScore==-1?0:PhoneScore)*1 + (EmailScore==-1?0:EmailScore)*0.7 + (AddressScore==-1?0:AddressScore)*0.5;
	}
};

class contactItem {
private:
	int id;
	SearchScores scores;
	string name;
	string surname;
	vector <phoneNumber> vPhoneNumber; // numbers
	string email;
	string address;

public:
	

	/*!
      Construtor por defeito
    */
	contactItem();

	/*!
      Construtor que inicializa o identificador do contacto
	  \param id Id do contacto
    */
	contactItem(int id);

	/*!
      Regista um novo contacto perguntando ao utilizador os atributos do mesmo
    */
	void registerContact();

	/*!
      Obtém o Id de um contacto
	  \return Retorna o Id do contacto
    */
	int getId();

	/*!
      Obtém o nome de um contacto
	  \return Retorna o nome do contacto
    */
	string getName();

	/*!
      Obtém o sobrenome de um contacto
	  \return Retorna o sobrenome do contacto
    */
	string getSurname();

	/*!
      Obtém o email de um contacto
	  \return Retorna o email do contacto
    */
	string getEmail();

	/*!
      Obtém a morada de um contacto
	  \return Retorna a morada do contacto
    */
	string getAddress();

	/*!
      Obtém o rank de um contacto
	  \return Retorna o rank do contacto
    */
	SearchScores getScores();

	/*!
      Obtém os números de telefone de um contacto
	  \return Retorna os números de telefone do contacto
    */
	vector <phoneNumber> getNumbers();

	/*!
      Redefine o Id do contacto
	  \param id O id do contacto
    */
	void setId(int id);

	/*!
      Redefine o nome do contacto
	  \param name O nome do contacto
    */
	void setName(string name);

	/*!
      Redefine o sobrenome do contacto
	  \param surname O sobrenome do contacto
    */
	void setSurname(string surname);

	/*!
      Redefine o email do contacto
	  \param email O email do contacto
    */
	void setEmail(string email);

	/*!
      Redefine a morada do contacto
	  \param address A morada do contacto
    */
	void setAddress(string address);

	/*!
      Redefine os números de telefone do contacto
	  \param vPhoneNumbers Os números de telefone do contacto
    */
	void setPhoneNumbers(vector<phoneNumber> vPhoneNumbers);
	/*!
      Redefine a score do contacto
	  \param value A score do contacto
    */
	void setScores(SearchScores value);

	/*!
      Lê e valida um número inserido através da linha de comandos
	  \param message Mensagem que contém o número a ser lido
	  \param min Valor mínimo que o número pode tomar
	  \param max Valor máximo que o número pode tomar
	  \return Retorna o número que foi convertido
    */
	int readInteger(string message, int min, int max);
};

#endif
