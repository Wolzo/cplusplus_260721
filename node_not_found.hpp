#ifndef NODE_NOT_FOUND_HPP
#define NODE_NOT_FOUND_HPP

#include <stdexcept>
#include <string>

/**
  @brief Classe che rappresenta l'eccezione per un nodo inesistente

  Eccezione custom lanciata quando si referenzia un nodo il cui
  identificativo non e' presente nel grafo.
*/
class node_not_found : public std::logic_error {
public:
  node_not_found(const std::string &msg) : std::logic_error(msg) {}
};

#endif