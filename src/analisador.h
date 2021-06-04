#include "catalogo.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <utility>
#include <iostream>
#include <list>

class Analisador {
	private:
		Catalogo catalogo;
		std::unordered_map<std::string, int> dicionario;
		std::list<std::pair<std::string, int>> lista;
		std::list<std::string> stop_words;

		void preencher_stop_words();
		void analisar();

	public:
		Analisador(Catalogo catalogo) {
			this->catalogo = catalogo;
			preencher_stop_words();
			analisar();
		}

		void print();
		void ranking(int);
};

inline void Analisador::preencher_stop_words() {
	std::ifstream arquivo;
	arquivo.open("../recursos/stop_words.txt");
	std::string s;

	while (std::getline(arquivo, s)) {
		this->stop_words.push_back(s);
	}
}

inline void Analisador::analisar() {
	std::ifstream arquivos[30];

	// varre e abre os 30 arquivos
	for (int i = 0; i < 30; i++) {
		std::string s;
		arquivos[i].open(this->catalogo.get_nome(i));
			// para cada arquivo, ler as linhas na string s
		while (std::getline(arquivos[i], s)) {
			std::regex r(R"([a-zA-Z_]+(?:['_-][a-zA-Z_]+)*)");
			std::sregex_iterator it1; // regex
			
			// aplica a regex e verifica se a palavra não é stop word
			for (it1 = std::sregex_iterator(s.begin(), s.end(), r); it1 != std::sregex_iterator(); it1++) {
  				std::smatch m = *it1;
				std::unordered_map<std::string, int>::const_iterator it = this->dicionario.find(m.str());
				bool is_stopword = false;

				// verifica se a palavra é stop word
				std::list<std::string>::const_iterator it2; // iterador da lista de stop_words
				for (it2 = this->stop_words.begin(); it2 != this->stop_words.end(); it2++) {
					if (m.str() == *it2) {
						is_stopword = true;
						break;
					}
				}
                       
				if (!is_stopword) {
					// palavra não inclusa no dicionário
					if (it == this->dicionario.end()) {
						std::pair<std::string, int> palavra(m.str(), 1);
						this->dicionario.insert(palavra);
					}
					// palavra estiver inclusa no dicionário
					else {
						this->dicionario[m.str()]++;
					}
				}

			}
		}
	}
}

inline void Analisador::print() {
	for (auto i: this->dicionario) {
		std::cout << i.first << ": " << i.second << '\n';
	}
}

inline void Analisador::ranking(int count) {
	std::unordered_map<std::string, int>::const_iterator it;
	std::list<std::pair<std::string, int>>::const_iterator it2;
    
	for (it = dicionario.begin(); it != dicionario.end(); it++) {
		lista.push_back(*it);
	}

	auto sortRuleLambda = [] (std::pair<std::string, int> const& s1, std::pair<std::string, int> const& s2) -> bool {
		return s1.second > s2.second;
	};

	lista.sort(sortRuleLambda);

	for (it2 = lista.begin(); it2 != lista.end(); it2++) {
		std::cout << (*it2).first << "-" << (*it2).second << std::endl;
		count--;
		if (count == 0) break;
    }
}