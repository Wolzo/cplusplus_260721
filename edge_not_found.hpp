#ifndef EDGE_NOT_FOUND_HPP
#define EDGE_NOT_FOUND_HPP

#include <stdexcept>
#include <string>

/**
  @brief Classe che rappresenta l'eccezione per un arco inesistente

  Eccezione custom lanciata quando si referenzia un arco non presente nel grafo.
*/
class edge_not_found : public std::logic_error {
public:
  edge_not_found(const std::string &msg) : std::logic_error(msg) {}
};

#endif