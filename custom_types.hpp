/**
  @file custom_types.hpp

  @brief Header con definizioni di tipi custom

  Definisce alcuni tipi custom usati in main.cpp per simulare oggetti di
  librerie esterne e testare il comportamento della classe su questi tipi.

  @note Gli operator<< liberi sono marcati inline perche' sono definiti in un
  header. Senza inline, includere il file in piu' .cpp causerebbe un errore di
  definizione multipla in fase di link.
*/

#ifndef CUSTOM_TYPES_HPP
#define CUSTOM_TYPES_HPP

#include <iostream>

/**
  @brief Struct city che rappresenta una citta'.

  Struct city che rappresenta una citta' identificata dal nome. E'
  possibile specificare come valore opzionale anche la popolazione.
  L'uguaglianza tra due citta' e' definita sul solo nome (tramite il funtore equal_city).
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

  Rappresenta un file con un percorso path e un contenuto content. L'operatore
  operator== confronta solo il path, quindi definisce l'identita' del file sul
  filesystem. Il grafo usa invece un funtore equal_file_by_content che confronta il contenuto: due
  file con path diverso ma stesso contenuto sono lo stesso nodo per il grafo, mentre due file con
  stesso path ma contenuto diverso sono nodi distinti.
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