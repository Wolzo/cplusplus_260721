#ifndef NODE_ALREADY_EXISTS_HPP
#define NODE_ALREADY_EXISTS_HPP

#include <stdexcept>
#include <string>

/**
  @brief Classe che rappresenta l'eccezione per un nodo gia' esistente

  Eccezione custom lanciata quando si tenta di aggiungere al grafo
  un nodo il cui identificativo e' gia' presente.
*/
class node_already_exists : public std::logic_error {
public:
  node_already_exists(const std::string &msg) : std::logic_error(msg) {}
};

#endif