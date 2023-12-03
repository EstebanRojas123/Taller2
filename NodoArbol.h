#pragma once
#include <vector>
using namespace std;


class NodoArbol {
public:
    std::vector<std::vector<char>> estado;
    std::vector<NodoArbol*> hijos;

    NodoArbol(const std::vector<std::vector<char>>& estado) : estado(estado), hijos() {}

    ~NodoArbol() {
        for (NodoArbol* hijo : hijos) {
            delete hijo;
        }
    }
};