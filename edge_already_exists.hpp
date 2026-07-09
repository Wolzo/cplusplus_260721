#ifndef EDGE_ALREADY_EXISTS_HPP
#define EDGE_ALREADY_EXISTS_HPP

#include <stdexcept>
#include <string>

/**
  @brief Classe che rappresenta l'eccezione per un arco gia' esistente

  Eccezione custom lanciata quando si tenta di aggiungere al grafo
  un arco gia' presente.
*/
class edge_already_exists : public std::logic_error {
public:
  edge_already_exists(const std::string &msg) : std::logic_error(msg) {}
};

#endif