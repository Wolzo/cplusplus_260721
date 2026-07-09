/**
  @file custom_types.hpp

  @brief File header per definizioni di tipi custom

  File di dichiarazioni/definizioni di tipi custom. Servono a simulare in
  main.cpp oggetti provenienti da librerie esterne, per testare a fondo il
  comportamento della classe sui tipi custom.

  @note Gli operatori operator<< liberi sono marcati inline perche' sono
  funzioni definite in un header: senza inline, includere il file in piu'
  file .cpp darebbe errore di definizione multipla in fase di link.
*/

#ifndef CUSTOM_TYPES_HPP
#define CUSTOM_TYPES_HPP

#include <iostream>

/**
  @brief Struct city che rappresenta una citta'.

  Struct city che rappresenta una citta' identificata dal nome. E'
  possibile specificare come valore opzionale anche la popolazione.
  L'uguaglianza tra due citta' e' definita sul solo nome. Si ipotizza che
  gli archi siano collegamenti mono-direzionali da una citta' all'altra.
*/
struct city {
  std::string name;
  int population;

  city() : name(""), population(0) {}

  city(const std::string &n, int p) : name(n), population(p) {}
};

struct equal_city {
  bool operator()(const city &a, const city &b) const { return a.name == b.name; }
};

inline std::ostream &operator<<(std::ostream &os, const city &c) {
  os << c.name << "(" << c.population << ")";
  return os;
}

/**
  @brief Struct file che rappresenta un file.

  Struct file che rappresenta un file dotato di un percorso path e di un
  contenuto content. L'operatore operator== confronta il solo path, e
  stabilisce quindi l'identita' del file sul filesystem. Il criterio di
  identita' usato dal grafo e' invece definito da fuori, dal funtore
  equal_file_by_content, che confronta il contenuto: due file con path
  diversi ma stesso contenuto sono lo stesso nodo per il grafo, mentre due file
  con lo stesso path ma contenuto diverso sono nodi distinti.
*/
struct file {
  std::string path;
  std::string content;

  file() : path(""), content("") {}
  file(const std::string &p, const std::string &c) : path(p), content(c) {}

  bool operator==(const file &other) const { return path == other.path; }
};

struct equal_file_by_content {
  bool operator()(const file &a, const file &b) const { return a.content == b.content; }
};

inline std::ostream &operator<<(std::ostream &os, const file &f) {
  os << f.path << "[" << f.content << "]";
  return os;
}

#endif