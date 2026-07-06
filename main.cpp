/**
  @file main.cpp

  @brief File di test della classe graph

  Test automatici dell'interfaccia pubblica della classe graph.
*/

#include "graph.hpp"
#include <cassert>
#include <iostream>

/**
  @brief Test di aggiunta dei nodi

  Verifica l'aggiunta di nodi, il conteggio dei nodi, la presenza e
  l'assenza tramite existsNode, la conservazione dei nodi gia' presenti
  dopo nuove aggiunte e il lancio dell'eccezione node_already_exists in
  caso di nodo duplicato.
*/
void test_addNode() {
  graph<int> g;
  assert(g.numNodes() == 0);

  g.addNode(10);
  assert(g.numNodes() == 1);
  assert(g.existsNode(10));
  assert(!g.existsNode(70));

  try {
    g.addNode(10);
    assert(false);
  } catch (const node_already_exists &e) {
  }

  assert(g.numNodes() == 1);

  g.addNode(40);
  assert(g.numNodes() == 2);
  assert(g.existsNode(10));
  assert(g.existsNode(40));
}

/**
  @brief Test di rimozione dei nodi

  Verifica la rimozione di nodi, il conteggio dei nodi, la presenza e
  l'assenza tramite existsNode, la conservazione dei nodi gia' presenti
  dopo eliminazioni e il lancio dell'eccezione node_not_found in
  caso di nodo non esistente.
*/
void test_removeNode() {
  graph<int> g;

  g.addNode(10);
  g.addNode(20);
  g.addNode(30);
  g.addNode(40);
  assert(g.numNodes() == 4);

  g.removeNode(20);
  assert(!g.existsNode(20));
  assert(g.existsNode(10));
  assert(g.existsNode(30));
  assert(g.existsNode(40));
  assert(g.numNodes() == 3);

  try {
    g.removeNode(50);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numNodes() == 3);

  g.removeNode(10);
  g.removeNode(30);
  g.removeNode(40);
  assert(g.numNodes() == 0);
}

/**
  @brief Test di aggiunta di archi

  Verifica l'aggiunta di archi, il conteggio di archi, la presenza e
  l'assenza tramite existsEdge, la conservazione di archi gia' presenti
  dopo nuove aggiunte e il lancio dell'eccezione edge_already_exists in
  caso di arco gia' esistente e dell'eccezione node_not_found in caso di
  aggiunta di archi tra nodi non esistenti.
*/
void test_addEdge() {
  graph<int> g;

  g.addNode(10);
  g.addNode(20);
  g.addNode(30);

  assert(g.numEdges() == 0);

  g.addEdge(10, 10);
  g.addEdge(10, 20);
  g.addEdge(20, 30);
  g.addEdge(30, 10);
  g.addEdge(30, 30);

  assert(g.numEdges() == 5);

  assert(g.existsEdge(10, 10));
  assert(g.existsEdge(10, 20));
  assert(!g.existsEdge(10, 30));

  assert(!g.existsEdge(20, 10));
  assert(!g.existsEdge(20, 20));
  assert(g.existsEdge(20, 30));

  assert(g.existsEdge(30, 10));
  assert(!g.existsEdge(30, 20));
  assert(g.existsEdge(30, 30));

  try {
    g.addEdge(99, 10);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 5);

  try {
    g.addEdge(10, 99);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 5);

  try {
    g.addEdge(99, 98);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 5);

  try {
    g.addEdge(10, 10);
    assert(false);
  } catch (const edge_already_exists &e) {
  }

  assert(g.numEdges() == 5);

  try {
    g.existsEdge(99, 10);
    assert(false);
  } catch (const node_not_found &e) {
  }
}

/**
  @brief Test di rimozione di archi

  Verifica la rimozione di archi, il conteggio di archi, la presenza e
  l'assenza tramite existsEdge, la conservazione di archi gia' presenti
  dopo l'eliminazione e il lancio dell'eccezione edge_not_found in
  caso di arco non esistente e dell'eccezione node_not_found in caso di
  rimozione di archi tra nodi non esistenti.
*/
void test_removeEdge() {
  graph<int> g;

  g.addNode(10);
  g.addNode(20);
  g.addNode(30);

  g.addEdge(10, 10);
  g.addEdge(10, 20);
  g.addEdge(20, 30);
  g.addEdge(30, 10);
  g.addEdge(30, 30);

  assert(g.numEdges() == 5);

  g.removeEdge(20, 30);

  assert(g.numEdges() == 4);

  assert(g.existsEdge(10, 10));
  assert(g.existsEdge(10, 20));
  assert(!g.existsEdge(10, 30));

  assert(!g.existsEdge(20, 10));
  assert(!g.existsEdge(20, 20));
  assert(!g.existsEdge(20, 30));

  assert(g.existsEdge(30, 10));
  assert(!g.existsEdge(30, 20));
  assert(g.existsEdge(30, 30));

  try {
    g.removeEdge(99, 10);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 4);

  try {
    g.removeEdge(10, 99);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 4);

  try {
    g.removeEdge(99, 98);
    assert(false);
  } catch (const node_not_found &e) {
  }

  assert(g.numEdges() == 4);

  try {
    g.removeEdge(20, 30);
    assert(false);
  } catch (const edge_not_found &e) {
  }

  assert(g.numEdges() == 4);

  g.removeEdge(10, 10);
  g.removeEdge(10, 20);
  g.removeEdge(30, 10);
  g.removeEdge(30, 30);
  assert(g.numEdges() == 0);
}

/**
  @brief Test completo nodi + archi

  Verifica la coerenza dell'intera struttura sottoponendola a sequenze
  alternate di aggiunte e rimozioni di nodi e archi. In particolare
  controlla che la rimozione di un nodo elimini tutti e soli gli archi
  a esso incidenti (in entrata e in uscita) e che gli archi rimanenti
  restino associati alle coppie di nodi corrette nonostante il
  riallineamento degli indici nella matrice di adiacenza.
*/
void test_completo_nodi_archi() {
  graph<int> g;

  g.addNode(10);
  g.addNode(20);
  g.addNode(30);
  g.addNode(40);
  assert(g.numNodes() == 4);
  assert(g.numEdges() == 0);

  g.addEdge(10, 20);
  g.addEdge(20, 30);
  g.addEdge(20, 20);
  g.addEdge(40, 20);
  g.addEdge(20, 40);
  g.addEdge(10, 40);
  g.addEdge(40, 40);
  assert(g.numEdges() == 7);

  assert(g.existsEdge(10, 20));
  assert(g.existsEdge(20, 30));
  assert(g.existsEdge(20, 20));
  assert(g.existsEdge(40, 20));
  assert(g.existsEdge(20, 40));

  // rimozione del nodo 20: devono sparire tutti gli archi di 20 (entranti,
  // uscenti e self-loop), cioe' 5 archi. Restano solo (10,40) e (40,40).
  g.removeNode(20);
  assert(!g.existsNode(20));
  assert(g.numNodes() == 3);
  assert(g.numEdges() == 2);

  assert(g.existsEdge(10, 40));
  assert(g.existsEdge(40, 40));

  try {
    g.existsEdge(10, 20);
    assert(false);
  } catch (const node_not_found &e) {
  }

  // re-inserimento di un nodo con lo stesso identificativo:
  // deve tornare a esistere ma senza i vecchi archi
  g.addNode(20);
  assert(g.existsNode(20));
  assert(g.numNodes() == 4);
  assert(g.numEdges() == 2);
  assert(!g.existsEdge(10, 20));
  assert(!g.existsEdge(20, 20));
  assert(!g.existsEdge(40, 20));

  g.addEdge(20, 10);
  g.addEdge(30, 20);
  assert(g.numEdges() == 4);
  assert(g.existsEdge(20, 10));
  assert(g.existsEdge(30, 20));

  // rimozione di un arco seguita da rimozione di un nodo
  g.removeEdge(10, 40);
  assert(g.numEdges() == 3);
  g.removeNode(40);
  assert(!g.existsNode(40));
  assert(g.numNodes() == 3);
  assert(g.numEdges() == 2);
  assert(g.existsEdge(20, 10));
  assert(g.existsEdge(30, 20));

  g.removeNode(10);
  assert(g.numNodes() == 2);
  assert(g.numEdges() == 1);
  assert(g.existsEdge(30, 20));

  g.removeNode(30);
  g.removeNode(20);
  assert(g.numNodes() == 0);
  assert(g.numEdges() == 0);
}

/**
  @brief Test delle funzionalita' di copia e della stampa

  Verifica copy constructor, operatore di assegnamento e
  auto-assegnamento sia sul caso limite del grafo vuoto (_size == 0)
  sia su un grafo popolato. Verifica di stampa del grafo su cout.
*/
void test_copia_e_stampa() {
  // Grafo vuoto
  graph<int> vuoto1;
  graph<int> vuoto2(vuoto1); // copy constructor su grafo vuoto
  graph<int> vuoto3;
  vuoto3 = vuoto1; // assegnamento su grafo vuoto
  vuoto3 = vuoto3; // auto-assegnamento

  assert(vuoto2.numNodes() == 0);
  assert(vuoto3.numNodes() == 0);
  assert(vuoto2.begin() == vuoto2.end());

  // stampa del grafo vuoto
  std::cout << vuoto1 << std::endl;

  // Grafo popolato
  graph<int> g1;
  g1.addNode(10);
  g1.addNode(20);
  g1.addNode(30);
  g1.addEdge(10, 20);
  g1.addEdge(20, 30);

  // copy constructor su grafo non vuoto
  graph<int> g2(g1);
  assert(g2.numNodes() == 3);
  assert(g2.numEdges() == 2);
  assert(g2.existsEdge(10, 20));
  assert(g2.existsEdge(20, 30));

  // modifico g2 e verifico che g1 resti invariato
  g2.addNode(40);
  g2.addEdge(30, 40);
  assert(g2.numNodes() == 4);
  assert(g1.numNodes() == 3);
  assert(g1.numEdges() == 2);
  try {
    g1.existsEdge(30, 40); // il nodo 40 non esiste in g1
    assert(false);
  } catch (const node_not_found &e) {
  }

  // operatore di assegnamento su un grafo gia' popolato:
  // il vecchio contenuto deve essere sostituito (copy-and-swap)
  graph<int> g3;
  g3.addNode(99);
  g3 = g1;
  assert(g3.numNodes() == 3);
  assert(g3.numEdges() == 2);
  assert(!g3.existsNode(99));
  assert(g3.existsNode(10));

  // auto-assegnamento su grafo popolato
  g3 = g3;
  assert(g3.numNodes() == 3);
  assert(g3.numEdges() == 2);

  // indipendenza anche dopo assegnamento
  g3.removeNode(10);
  assert(g3.numNodes() == 2);
  assert(g1.numNodes() == 3);

  std::cout << "GRAFO G1" << std::endl;
  std::cout << g1 << std::endl;
  std::cout << "GRAFO G3" << std::endl;
  std::cout << g3 << std::endl;
}

/**
  @brief Test del const_iterator forward

  Verifica che l'iteratore percorra tutti e soli gli identificativi
  dei nodi presenti nel grafo.
*/
void test_const_iterator() {
  graph<int> g;

  assert(g.begin() == g.end());

  g.addNode(10);
  g.addNode(20);
  g.addNode(30);
  g.addNode(40);

  unsigned int visitati = 0;
  for (graph<int>::const_iterator i = g.begin(); i != g.end(); ++i) {
    assert(g.existsNode(*i));
    ++visitati;
  }

  assert(visitati == g.numNodes());

  // due iteratori indipendenti non interferiscono
  graph<int>::const_iterator a = g.begin();
  graph<int>::const_iterator b = a; // copy constructor dell'iteratore
  ++a;
  assert(a != b);         // avanzare a non tocca b
  assert(b == g.begin()); // b e' ancora all'inizio

  // post-incremento: ritorna il valore precedente all'avanzamento
  graph<int>::const_iterator c = g.begin();
  graph<int>::const_iterator d = c++;
  assert(d == g.begin()); // d ha il valore pre-incremento
  assert(c != d);         // c e' avanzato
}

//--------------------------------------------------------------------
/**
  @brief Struct city che implementa una citta'.

  Struct city che implementa una citta' identificata dal nome. E'
  possibile specificare come valore opzionale anche la popolazione.
  L'uguaglianza tra due citta' e' definita sul solo nome. Si ipotizza
  che gli archi siano collegamenti mono-direzionali da una citta'
  all'altra.
*/
struct city {
  std::string name;
  int population;

  city() : name(""), population(0) {}

  city(const std::string &n, int p) : name(n), population(p) {}

  bool operator==(const city &other) const { return name == other.name; }
};

std::ostream &operator<<(std::ostream &os, const city &c) {
  os << c.name;
  return os;
}

void test_custom_type() {
  graph<city> g;

  city milano = city("Milano", 1400000);
  city roma = city("Roma", 2700000);
  city napoli = city("Napoli", 900000);

  g.addNode(milano);
  g.addNode(roma);
  g.addNode(napoli);

  // l'oggetto city e' diverso ma corrisponde ad un nodo gia' esistente secondo
  // la logica della struct (test operator==)
  try {
    g.addNode(city("Milano", 2000000));
    assert(false);
  } catch (node_already_exists &e) {
  }

  try {
    g.removeNode(city("Venezia", 1000000));
    assert(false);
  } catch (node_not_found &e) {
  }

  assert(g.numNodes() == 3);

  g.addEdge(milano, roma);
  g.addEdge(roma, napoli);
  g.addEdge(napoli, roma);
  g.addEdge(roma, milano);

  try {
    g.existsEdge(city("Venezia", 1000000), milano);
    assert(false);
  } catch (node_not_found &e) {
  }

  try {
    g.removeEdge(milano, napoli);
    assert(false);
  } catch (edge_not_found &e) {
  }

  assert(g.numEdges() == 4);
  assert(g.existsEdge(milano, roma));
  assert(g.existsEdge(roma, napoli));
  assert(g.existsEdge(napoli, roma));
  assert(g.existsEdge(roma, milano));
  assert(!g.existsEdge(milano, napoli));

  // l'oggetto city e' diverso ma corrisponde ad un nodo gia' esistente secondo
  // la logica della struct (test operator==)
  try {
    g.addEdge(city("Milano", 2000000), roma);
    assert(false);
  } catch (edge_already_exists &e) {
  }

  // test iteratore su tipo custom
  unsigned int visitati = 0;
  for (graph<city>::const_iterator ct = g.begin(); ct != g.end(); ++ct) {
    assert(g.existsNode(*ct));
    ++visitati;
  }

  assert(visitati == g.numNodes());

  std::cout << g << std::endl;
}

int main() {
  std::cout << "===== Test graph.hpp - 260721 =====" << std::endl << std::endl;

  test_addNode();
  std::cout << "[OK] Test addNode" << std::endl;

  test_removeNode();
  std::cout << "[OK] Test removeNode" << std::endl;

  test_addEdge();
  std::cout << "[OK] Test addEdge" << std::endl;

  test_removeEdge();
  std::cout << "[OK] Test removeEdge" << std::endl;

  test_completo_nodi_archi();
  std::cout << "[OK] Test completo (nodi + archi)" << std::endl;

  test_copia_e_stampa();
  std::cout << "[OK] Test copia profonda" << std::endl;

  test_const_iterator();
  std::cout << "[OK] Test const iterator" << std::endl;

  test_custom_type();
  std::cout << "[OK] Test custom type city" << std::endl;

  std::cout << std::endl << "===== Tutti i test superati =====" << std::endl;
  return 0;
}