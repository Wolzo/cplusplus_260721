/**
  @file graph.hpp

  @brief File header della classe graph templata

  File di dichiarazioni/definizioni della classe graph templata che
  implementa un grafo orientato.
*/

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm> // std::swap
#include <cstddef>   // std::ptrdiff_t
#include <iostream>  // std::cout
#include <iterator>  // std::forward_iterator_tag
#include <new>
#include <ostream>   // std::ostream
#include <stdexcept> // std::logic_error
#include <string>    // std::string

/**
  @brief Classe che rappresenta l'eccezione per un nodo gia' esistente

  Eccezione custom lanciata quando si tenta di aggiungere al grafo
  un nodo il cui identificativo e' gia' presente.
*/
class node_already_exists : public std::logic_error {
public:
  node_already_exists(const std::string &msg) : std::logic_error(msg) {}
};

/**
  @brief Classe che rappresenta l'eccezione per un nodo inesistente

  Eccezione custom lanciata quando si referenzia un nodo il cui
  identificativo non e' presente nel grafo.
*/
class node_not_found : public std::logic_error {
public:
  node_not_found(const std::string &msg) : std::logic_error(msg) {}
};

/**
  @brief Classe che rappresenta l'eccezione per un arco gia' esistente

  Eccezione custom lanciata quando si tenta di aggiungere al grafo
  un arco gia' presente.
*/
class edge_already_exists : public std::logic_error {
public:
  edge_already_exists(const std::string &msg) : std::logic_error(msg) {}
};

/**
  @brief Classe che rappresenta l'eccezione per un arco inesistente

  Eccezione custom lanciata quando si referenzia un arco non presente nel grafo.
*/
class edge_not_found : public std::logic_error {
public:
  edge_not_found(const std::string &msg) : std::logic_error(msg) {}
};

/**
  @brief Classe che implementa un grafo orientato generico

  La classe implementa un grafo orientato i cui nodi sono identificati
  da un generico tipo T. Il grafo e' rappresentato tramite una matrice
  di adiacenza di booleani.
*/
template <typename T> class graph {
public:
  /**
    Costruttore di default.

    @post _ids == nullptr
    @post _adj == nullptr
    @post _size == 0
  */
  graph() : _ids(nullptr), _adj(nullptr), _size(0) {}

  /**
    Copy constructor. Crea una copia profonda del grafo other,
    allocando memoria propria e indipendente.

    @param other grafo da copiare

    @throw std::bad_alloc possibile eccezione di allocazione

    @post _size == other._size
    @post il contenuto di _ids e _adj e' una copia di quello di other
  */
  graph(const graph &other) : _ids(nullptr), _adj(nullptr), _size(other._size) {
    try {
      _ids = new T[_size];
      _adj = new bool[_size * _size];

      for (unsigned int i = 0; i < _size; ++i) {
        _ids[i] = other._ids[i];
      }
      for (unsigned int i = 0; i < _size * _size; ++i) {
        _adj[i] = other._adj[i];
      }
    } catch (...) {
      delete[] _ids;
      delete[] _adj;
      throw;
    }
  }

  /**
    Operatore di assegnamento

    @param other graph da copiare

    @return reference al graph this

    @throw std::bad_alloc possibile eccezione di allocazione
  */
  graph &operator=(const graph &other) {
    if (&other != this) {
      graph tmp(other);
      this->swap(tmp);
    }

    return *this;
  }

  /**
    Distruttore

    @post la memoria allocata da _ids e _adj e' stata liberata
  */
  ~graph() {
    delete[] _ids;
    delete[] _adj;
  }

  /**
    Funzione scambia lo stato tra l'istanza corrente di
    graph e quella passata come parametro.

    @param other graph con cui scambiare lo stato
  */
  void swap(graph &other) {
    std::swap(_ids, other._ids);
    std::swap(_adj, other._adj);
    std::swap(_size, other._size);
  }

  /**
    Ritorna il numero di nodi nel grafo.

    @return il numero di nodi nel grafo
  */
  unsigned int numNodes() const { return _size; }

  /**
    Ritorna il numero di archi nel grafo.

    @return il numero di archi nel grafo
  */
  unsigned int numEdges() const {
    unsigned int count = 0;
    for (unsigned int i = 0; i < _size * _size; ++i) {
      if (_adj[i]) {
        ++count;
      }
    }
    return count;
  }

  /**
    Ritorna se un identificatore e' presente nel grafo o meno

    @param id identificativo del nodo da cercare

    @return se un identificatore e' presente nel grafo o meno
  */
  bool existsNode(const T &id) const { return indexOf(id) != -1; }

  /**
    Ritorna se una coppia di nodi e' connessa da un arco mono-direzionale
    (src -> dst)

    @param src identificativo del nodo di partenza
    @param dst identificativo del nodo di destinazione

    @throw node_not_found se uno dei due nodi non e' presente nel grafo

    @return se una coppia di nodi e' connessa da un arco
  */
  bool existsEdge(const T &src, const T &dst) const {
    const int foundSrc = indexOf(src);
    if (foundSrc == -1) {
      throw node_not_found("Errore: il nodo di partenza non e' stato trovato");
    }

    const int foundDst = indexOf(dst);
    if (foundDst == -1) {
      throw node_not_found(
          "Errore: il nodo di destinazione non e' stato trovato");
    }

    const unsigned int srcNode = static_cast<unsigned int>(foundSrc);
    const unsigned int dstNode = static_cast<unsigned int>(foundDst);

    return at(srcNode, dstNode);
  }

  /**
    Aggiunge un nodo al grafo, identificato da id. La matrice di
    adiacenza viene estesa di una riga e una colonna, inizializzate
    a false (il nuovo nodo non ha archi).

    @param id identificativo del nodo da aggiungere

    @throw node_already_exists se un nodo con identificativo id e' gia' presente
    nel grafo
    @throw std::bad_alloc eccezione di allocazione

    @post _size == _size + 1
  */
  void addNode(const T &id) {
    if (existsNode(id)) {
      throw node_already_exists(
          "Errore: impossibile aggiungere un nodo gia' esistente");
    }

    unsigned int n_size = _size + 1;
    T *n_ids = nullptr;
    bool *n_adj = nullptr;
    try {
      n_ids = new T[n_size];
      n_adj = new bool[n_size * n_size]();

      for (unsigned int i = 0; i < _size; ++i) {
        n_ids[i] = _ids[i];
      }
      n_ids[_size] = id;

      // Ricopia la vecchia matrice nell'angolo in alto a sinistra della nuova.
      // La nuova riga e colonna restano false grazie alle () nella new.
      for (unsigned int i = 0; i < _size; ++i) {
        for (unsigned int j = 0; j < _size; ++j) {
          n_adj[offset(i, j, n_size)] = _adj[offset(i, j, _size)];
        }
      }
    } catch (...) {
      delete[] n_ids;
      delete[] n_adj;
      throw;
    }

    delete[] _ids;
    delete[] _adj;
    _ids = n_ids;
    _adj = n_adj;
    _size = n_size;
  }

  /**
    Rimuove un nodo al grafo, identificato da id. La matrice di
    adiacenza viene ridotta di una riga e una colonna.

    @param id identificativo del nodo da rimuovere

    @throw node_not_found se un nodo con identificativo id non e' presente nel
    grafo
    @throw std::bad_alloc eccezione di allocazione

    @post _size == _size - 1
  */
  void removeNode(const T &id) {
    const int found = indexOf(id);
    if (found == -1) {
      throw node_not_found("Errore: il nodo da eliminare non e' stato trovato");
    }

    // Eseguo il cast a unsigned per evitare warning nei confronti successivi
    // con gli indici relativi a _size, che e' unsigned. Essendoci gia'
    // assicurati che found e' non negativo, il cast e' safe.
    const unsigned int nodeToRemove = static_cast<unsigned int>(found);

    unsigned int n_size = _size - 1;
    T *n_ids = nullptr;
    bool *n_adj = nullptr;
    try {
      n_ids = new T[n_size];
      n_adj = new bool[n_size * n_size];

      unsigned int dst = 0;
      for (unsigned int src = 0; src < _size; ++src) {
        if (src == nodeToRemove)
          continue;

        n_ids[dst] = _ids[src];
        ++dst;
      }

      // Ricopia la matrice saltando la riga e la colonna del nodo rimosso.
      unsigned int dstRow = 0;
      for (unsigned int srcRow = 0; srcRow < _size; ++srcRow) {
        if (srcRow == nodeToRemove)
          continue;

        unsigned int dstCol = 0;
        for (unsigned int srcCol = 0; srcCol < _size; ++srcCol) {
          if (srcCol == nodeToRemove)
            continue;

          n_adj[offset(dstRow, dstCol, n_size)] =
              _adj[offset(srcRow, srcCol, _size)];
          ++dstCol;
        }
        ++dstRow;
      }
    } catch (...) {
      delete[] n_ids;
      delete[] n_adj;
      throw;
    }

    delete[] _ids;
    delete[] _adj;
    _ids = n_ids;
    _adj = n_adj;
    _size = n_size;
  }

  /**
    Aggiunge un arco mono-direzionale (src -> dst) al grafo.

    @param src identificativo del nodo di partenza
    @param dst identificativo del nodo di destinazione

    @throw node_not_found se uno dei due nodi non e' presente nel grafo
    @throw edge_already_exists se l'arco src -> dst e' gia' presente

    @post existsEdge(src, dst) == true
  */
  void addEdge(const T &src, const T &dst) { changeEdge(src, dst, true); }

  /**
    Rimuove un arco mono-direzionale (src -> dst) dal grafo.

    @param src identificativo del nodo di partenza
    @param dst identificativo del nodo di destinazione

    @throw node_not_found se uno dei due nodi non e' presente nel grafo
    @throw edge_not_found se l'arco src -> dst non e' presente

    @post existsEdge(src, dst) == false
  */
  void removeEdge(const T &src, const T &dst) { changeEdge(src, dst, false); }

  /**
    @brief Iteratore costante di tipo forward sui nodi del grafo

    L'iteratore itera sull'insieme degli identificativi dei nodi
    contenuti nel grafo. L'ordine con cui vengono ritornati riflette
    l'ordine di memorizzazione interno e non e' significativo.
  */
  class const_iterator {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T *pointer;
    typedef const T &reference;

    /**
      Costruttore di default.

      @post ptr == nullptr
    */
    const_iterator() : ptr(nullptr) {}

    /**
      Copy constructor.

      @param other iteratore da copiare

      @post ptr == other.ptr
    */
    const_iterator(const const_iterator &other) : ptr(other.ptr) {}

    /**
      Operatore di assegnamento.

      @param other iteratore da copiare

      @return reference all'iteratore this
    */
    const_iterator &operator=(const const_iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    /**
      Distruttore.
    */
    ~const_iterator() {}

    /**
      Dereferenziamento: ritorna il dato riferito dall'iteratore.

      @return reference costante al dato puntato
    */
    reference operator*() const { return *ptr; }

    /**
      Ritorna il puntatore al dato riferito dall'iteratore.

      @return puntatore costante al dato puntato
    */
    pointer operator->() const { return ptr; }

    /**
      Operatore di pre-incremento: avanza l'iteratore al nodo successivo.

      @return reference all'iteratore this dopo l'avanzamento
    */
    const_iterator &operator++() {
      ++ptr;
      return *this;
    }

    /**
      Operatore di post-incremento: avanza l'iteratore al nodo successivo.

      @return copia dell'iteratore precedente all'avanzamento
    */
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++ptr;
      return tmp;
    }

    /**
      Operatore di uguaglianza.

      @param other iteratore da confrontare

      @return true se i due iteratori puntano allo stesso nodo
    */
    bool operator==(const const_iterator &other) const {
      return ptr == other.ptr;
    }

    /**
      Operatore di diversita'.

      @param other iteratore da confrontare

      @return true se i due iteratori puntano a nodi diversi
    */
    bool operator!=(const const_iterator &other) const {
      return ptr != other.ptr;
    }

  private:
    const T *ptr; ///< Puntatore al nodo corrente

    friend class graph;

    /**
      Costruttore privato di inizializzazione, usato dalla classe
      container nei metodi begin e end.

      @param p puntatore al nodo iniziale

      @post ptr == p
    */
    const_iterator(const T *p) : ptr(p) {}
  };

  /**
    Ritorna un iteratore all'inizio della sequenza dei nodi.

    @return iteratore che punta al primo nodo del grafo
  */
  const_iterator begin() const { return const_iterator(_ids); }

  /**
    Ritorna un iteratore alla fine della sequenza dei nodi.

    @return iteratore che punta alla posizione successiva all'ultimo nodo
  */
  const_iterator end() const { return const_iterator(_ids + _size); }

  // Amicizia concessa all'operatore di stream, che accede ai membri privati del
  // grafo per stampare nodi e matrice di adiacenza.
  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const graph<U> &g);

private:
  T *_ids;            ///< Array degli identificativi dei nodi
  bool *_adj;         ///< Matrice di adiacenza linearizzata
  unsigned int _size; ///< Numero di nodi presenti nel grafo

  /**
    Funzione che restituisce l'indice di posizione di un nodo a partire dal suo
    identificativo.

    @param id identificativo nodo

    @return l'indice di posizione del nodo, oppure -1 se non presente
  */
  int indexOf(const T &id) const {
    for (unsigned int i = 0; i < _size; ++i) {
      if (_ids[i] == id) {
        return i;
      }
    }

    return -1;
  }

  /**
    Calcola l'offset lineare della cella (i, j) in una matrice di larghezza
    linearizzata width.

    @param i riga
    @param j colonna
    @param width larghezza della riga

    @return offset lineare della cella (i, j)
  */
  static unsigned int offset(unsigned int i, unsigned int j,
                             unsigned int width) {
    return i * width + j;
  }

  /**
    Riferimento alla cella (i, j) della matrice di adiacenza.

    @param i riga
    @param j colonna

    @return riferimento alla cella (i, j)
  */
  bool &at(unsigned int i, unsigned int j) { return _adj[offset(i, j, _size)]; }

  /**
    Riferimento in sola lettura alla cella (i, j) della matrice di adiacenza.

    @param i riga
    @param j colonna

    @return riferimento costante alla cella (i, j)
  */
  const bool &at(unsigned int i, unsigned int j) const {
    return _adj[offset(i, j, _size)];
  }

  /**
    Modifica lo stato dell'arco mono-direzionale (src -> dst), impostandolo
    al valore indicato. Metodo condiviso da addEdge e removeEdge: il
    parametro value determina sia il valore da scrivere nella matrice sia
    il tipo di errore in caso di operazione non valida.

    Se value e' true (aggiunta) e l'arco esiste gia', lancia
    edge_already_exists. Se value e' false (rimozione) e l'arco non esiste,
    lancia edge_not_found.

    @param src identificativo del nodo di partenza
    @param dst identificativo del nodo di destinazione
    @param value nuovo stato dell'arco (true = presente, false = assente)

    @throw node_not_found se uno dei due nodi non e' presente nel grafo
    @throw edge_already_exists se value e' true e l'arco e' gia' presente
    @throw edge_not_found se value e' false e l'arco non e' presente
  */
  void changeEdge(const T &src, const T &dst, const bool value) {
    const int foundSrc = indexOf(src);
    if (foundSrc == -1) {
      throw node_not_found("Errore: il nodo di partenza non e' stato trovato");
    }

    const int foundDst = indexOf(dst);
    if (foundDst == -1) {
      throw node_not_found(
          "Errore: il nodo di destinazione non e' stato trovato");
    }

    const unsigned int srcNode = static_cast<unsigned int>(foundSrc);
    const unsigned int dstNode = static_cast<unsigned int>(foundDst);

    const bool currentValue = at(srcNode, dstNode);
    if (value && currentValue) {
      throw edge_already_exists(
          "Errore: impossibile aggiungere un arco gia' esistente");
    }

    if (!value && !currentValue) {
      throw edge_not_found("Errore: impossibile rimuovere un arco inesistente");
    }

    at(srcNode, dstNode) = value;
  }
};

/**
  @brief Operatore di stream

  Funzione globale templata che invia sullo stream gli identificativi
  dei nodi e la matrice di adiacenza del grafo.

  @param os stream di output
  @param g grafo da inviare sullo stream

  @return lo stream di output
*/
template <typename T>
std::ostream &operator<<(std::ostream &os, const graph<T> &g) {
  os << "Nodi:\n[";
  for (unsigned int i = 0; i < g._size; ++i) {
    if (i > 0) {
      os << " ";
    }
    os << g._ids[i];
  }
  os << "]\n";

  os << "Matrice di adiacenza:\n";
  if (g._size == 0) {
    os << "||\n";
  } else {
    for (unsigned int i = 0; i < g._size; ++i) {
      os << "|";
      for (unsigned int j = 0; j < g._size; ++j) {
        if (j > 0) {
          os << " ";
        }
        os << g.at(i, j);
      }
      os << "|\n";
    }
  }

  return os;
}

#endif