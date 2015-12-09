//
//  btree.hpp
//  AED_BinaryTree
//
//  Created by Alexander Arturo Baylon Ibanez on 6/10/15.
//  Copyright © 2015 Alexander Arturo Baylon Ibanez. All rights reserved.
//

#ifndef btree_hpp
#define btree_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <list>
#include <fstream>
#include <deque>
#include <iomanip>
#include <string>
#include <cmath>
#include <unistd.h>
#include <term.h>


using namespace std;

//Trait
template <class T>
struct RBTreeTrait{
    typedef  T U;
    typedef  less<T> C;
    typedef  greater<T> D;
};


// Nodo
template<class T>
class cNode{
public:
    T m_data;
    T m_height;
    bool color;
    cNode<T> * m_child[2];
    cNode<T> * p;
    cNode(){
        color = 1;
        p     = nullptr;};
    cNode(T x,cNode<T> * f){
        m_data = x;
        m_child[0] = m_child[1] = nullptr;
        p = f;
        color = 0;// 0=ROJO // 1=NEGRO
    }
    ~cNode(){};
};
//

template <class Tr>
class  CtreeIterator{
public:
    //el elemento principal es un stack
    //lleno de pares(nodo,estado)
    typedef pair<cNode<Tr>*,Tr> par;
    typedef stack<par>          pila;
    pila m_p;
    //constructor
    CtreeIterator(par p)
    {m_p = p;}
    CtreeIterator(){};
    
    //operaciones
    CtreeIterator<Tr> operator =(CtreeIterator<Tr> x)
    {
        m_p = x.m_p;
        return *this;}
    //operador !=
    bool operator != (CtreeIterator<Tr> x)
        {return m_p != x.m_p;}
    //Operador ++
    CtreeIterator<Tr> operator ++ (int){
        par * t = &m_p.top();
        //Mientras que no tengamos el siguiente elemento a mostrar
        while (t->second!=1) {
            switch (t->second) {
            //Caso 0: se va al hijo izquierdo
                case 0:
                    //ponemos el hijo izquierdo en la pila
                    //si es que hay uno
                    if (t->first->m_child[0]){
                        m_p.push(par(t->first->m_child[0],0));
                        t->second++; //cambiamos el estado
                        t = &m_p.top();} //actualizamos el temporal
                    else{//si no hay hijo izquierdo
                        t->second=1; //cambiamos el estado
                        t = &m_p.top(); //actualizamos el temporal
                    }
                    break;
            //Caso 2: se va al hijo derecho
                case 2:
                    if (t->first->m_child[1]) {
                        //ponemos el hijo derecho en la pila
                        m_p.push(par(t->first->m_child[1],0));
                        t->second=2; //cambiamos el estado
                        t = &m_p.top();} //actualizamos el temporal
                    else{//si no hay hijo derecho
                        t->second=3; //cambiamos el estado
                    }
                    break;
            //Caso 3: regresa al padre
                case 3:
                    //Si hay elementos en la pila
                    if (!m_p.empty()) {
                        m_p.pop(); //quitamos el elemento de la pila
                        if  (!m_p.empty()) {
                            t = &m_p.top();
                            if (t->second==0) m_p.top().second = 1;
                            if (t->second==2) m_p.top().second = 3;
                        }
                        break;
                    }
                    //Si la pila queda vacia
                    return *this;
                    break;
                default:
                    break;
            }
        }
        //Si tiene que irse al hijo derecho pero ya se imprimio
        if(t->first->m_child[1]) t->second=2;
        else t->second = 3;
        return *this;
    }
    Tr operator *(){
        //int temp = m_p.top().first->m_data;
        return m_p.top().first->m_data;
    }
    
    void print_stack()
    {
        cout << "m_p" << endl;
        pila temp = m_p;
        while (!temp.empty()) {
            cout << temp.top().first->m_data << " " << temp.top().second << endl;
            temp.pop();}
    }
};



//
template<class T>
cNode<T> ** rep(cNode<T> ** p)
{
    p = &((*p)->m_child[0]);
    while ((*p)->m_child[1])
        p = &((*p)->m_child[1]);
    cout << "contenido: " << (*p)->m_data << endl;
    return p;
}

//Arbol Binario
template <class Tr>
class RB_Tree {
private:
    typedef typename Tr::U T;//tipo de dato
    //comparaciones
    typedef typename Tr::C C;
    typedef typename Tr::D D;
    
public:
    //iterador y stack
    typedef pair<cNode<T>*,T>   par;
    typedef stack<par>          pila;
    typedef CtreeIterator<T>    iterador;
    cNode<T> *  m_root;
    cNode<T> *  NIL;
    //instanciamos una funcion less
    less<T>     m_cmp;
    //instanciamos un iterador
    iterador i;
    iterador j;
    RB_Tree(){  m_root=nullptr;
                NIL = new cNode<T>();}
    bool find(T,cNode<T>**&,stack<cNode<T>*> & );
    bool insert(T);
    bool remove(T);
    //metodos RED-BLACK
    void right_rotate(cNode<T>*);//rotar izquierda
    void left_rotate(cNode<T>*);//rotar derecha
    void insert_fix(cNode<T>*);//arreglar el insert
    bool get_color(cNode<T>*);//obtener el color del nodo
    void transplant(cNode<T>*,cNode<T>*);
    cNode<T> * tree_minimium(cNode<T>*);
    
    //metodos de impresion
    void printTree(cNode<T>*);
    void printTreeAmplitud(cNode<T>*);
    void printNodesAndFathers(cNode<T>*);
    //nos retorna un stack con el camino hacia el primer nodo
    iterador begin()
    {
        pila first;
        cNode<T> * p;
        for (p = m_root; p->m_child[0]!=nullptr; p=p->m_child[0]){
            first.push(par(p,0));}
        //como el ultimo elemento se va a imprimir sin condiciones, entonces
        //es estado 3
        first.push(par(p,3));
        i.m_p=first;
        return i;
    }
    //nos retorna un iterador vacio
    iterador end() {return j;}
    ~RB_Tree(){};
};


//
template<class Tr>
bool RB_Tree<Tr>::find(T x, cNode<T> **& p, stack<cNode<T>*> & s){
    for (p=&m_root; (*p) && (*p)->m_data!=x; p=&( (*p)->m_child[m_cmp((*p)->m_data,x)]))
        s.push(*p);
    return (*p);
}

//
template<class Tr>
bool RB_Tree<Tr>::insert(T x){
    stack<cNode<T>*> path;
    cNode<T> ** y;
    cNode<T> * p;
    if (find(x, y,path)) return 0;
    if (!path.empty())
        p = path.top();
    else p = NIL;
    *y = new cNode<T>(x,p);//instanciamos el nuevo nodo
    cNode<T> * z = *y;   //guardamos el nodo insertado (z)
    //m_root->p = &dummy;
    //if (!path.empty()) {
       // z->p = &dummy;}
    m_root->p = NIL;
    insert_fix(z); //verificamos si las propiedades se cumplen
    //m_root->p = NIL;
    cout << "inserto: " << x << endl;
    
    return 1;
}

//
template<class Tr>
void RB_Tree<Tr>::insert_fix(cNode<T> * z)
{
    cNode<T> * y;
    cout << "color padre: " << z->p->color << endl;
    while (!get_color(z->p)) {//termina cuando el padre de 'z' es NEGRO
        // Si el padre de 'z' es el hijo izq. del abuelo de 'z'
        if (z->p == z->p->p->m_child[0]) {
            y = z->p->p->m_child[1];// 'y' apunta al tio de 'z'(hijo derecho de su abuelo)
            //if (!y->color) {   // si 'y' es rojo CASO 1
            if (!get_color(y)) {   // si 'y' es rojo CASO 1
                z->p->color     = true; // padre de 'z' a negro(1)
                y ->color       = true; // tio de 'z' a negro(1)
                z->p->p->color  = false;// abuelo de 'z' a rojo(0)
                z               = z->p->p;}// movemos el puntero al abuelo
            else{// ('y' es negro) CASO 2 y/o CASO 3
                // Si 'z' es el hijo derecho de su padre(p1)
                if(z == z->p->m_child[1]){//CASO 2
                    z = z->p;       // 'z' ahora apunta a su padre(p1)
                    left_rotate(z);}//rotacion a la izquierda
                //CASO 3
                z->p->color     = true; // el padre de 'z' a negro(1)
                z->p->p->color  = false;// el abuelo de 'z' a rojo(0)
                right_rotate(z->p->p);// rotacion a la derecha con el abuelo de 'z'(p2)
            }
        }
        else{
            y = z->p->p->m_child[0];// 'y' apunta al tio de 'z'(hijo izq. de su abuelo)
            if (!get_color(y)) {   // si 'y' es rojo CASO 1
                z->p->color     = true; // padre de 'z' a negro(1)
                y ->color       = true; // tio de 'z' a negro(1)
                z->p->p->color  = false;// abuelo de 'z' a rojo(0)
                z               = z->p->p;}// movemos el puntero al abuelo
            else{// ('y' es negro) CASO 2 y/o CASO 3
                // Si 'z' es el hijo izq. de su padre(p1)
                if(z == z->p->m_child[0]){//CASO 2
                    z = z->p;       // 'z' ahora apunta a su padre(p1)
                    right_rotate(z);}//rotacion a la izquierda
                //CASO 3
                z->p->color     = true; // el padre de 'z' a negro(1)
                z->p->p->color  = false;// el abuelo de 'z' a rojo(0)
                left_rotate(z->p->p);// rotacion a la derecha con el abuelo de 'z'(p2)
            }
        }
    }
    m_root->color = 1;// Al final coloreamos la RAIZ a NEGRO.(prop 1)
}



//
template<class Tr>
bool RB_Tree<Tr>::remove(T x){
    cout << "dato" << x << endl;
    stack<cNode<T>*> path;
    cNode<T> ** p,* y;
    if (!find(x, p, path)) return 0;
    //si el nodo no tiene hijo izq.
    cNode<T> * z = (*p);
    
    bool original_color = (*p)->color;
    
    // Caso 2:tiene 2 hijos
    if ((*p)->m_child[0] && (*p)->m_child[1]) {
        //guardamos en 'q' el reemplazo del nodo a eliminar(rep)
        cNode<T> ** q = rep<T>(p);
        //hacemos un swap entre los datos de 'p' y 'q'
        (*p)->m_data = (*q)->m_data;
        //el doble puntero 'p' ahora es igual a 'q'
        p = q;
    }
    //guardamos el contenido de 'p' en 't'
    cNode<T> * t = *p;
    //guardamos el padre del nodo a eliminar
    cNode<T> * pa = t->p;
    // Caso 1:tiene 1 hijo
    *p = (*p)->m_child[(*p)->m_child[1]!=0];
    // actualizamos el padre del nuevo contenido de 'p'
    if (*p) (*p)->p = pa;
    // eliminamos el nodo
    delete t;
    
    
    /*
    cout << "z: " << z->m_data << endl;
    if (!z->m_child[0])
        transplant(z, z->m_child[1]);//transplantamos su hijo der.
    else if (!z->m_child[1]) transplant(z, z->m_child[0]);
    else{
        y = tree_minimium(z->m_child[1]);
        if (y->p != z) {
            transplant(y, y->m_child[1]);
            y->m_child[1] = z->m_child[1];
            y->m_child[1]->p = y;
        }
        transplant(z, y);
        y->m_child[0] = z->m_child[0];
        y->m_child[0]->p = y;
    }*/
    
    
    
    
    
    return 1;
}


//rotaciones
template<class Tr>
void RB_Tree<Tr>::right_rotate(cNode<T> * y)
{
    cNode<T> * x = y->m_child[0]; //guardamos el hijo izq. de 'y'
    y->m_child[0] = x->m_child[1];//el hijo izq. de 'y' ahora apunta al hijo der. de 'x'
    if (x->m_child[1])// si existe el hijo der. de 'x' entonces su padre es 'y'
        x->m_child[1]->p = y;
    x->p = y->p;//el padre de 'y' ahora es padre de 'x'
    if (y->p==NIL) {//si 'y' era la raiz entonces 'x' es la nueva raiz
        m_root = x;
        x->p = NIL;
    }
    else if (y == y->p->m_child[1])//si 'y' es el hijo der. de su padre
        y->p->m_child[1] = x;// el padre cambia de hijo der. a 'x'
    else y->p->m_child[0] = x;// el padre cambia de hijo izq. a 'x'
    x->m_child[1] = y;// el hijo Der. de 'x' ahora apunta a 'y'
    y->p = x; // el padre de 'y' ahora es 'x'
    
    /*
    if (!s.empty()) s.pop();//sacamos al nodo p del stack
    if (!s.empty()){
        cNode<T> * father = s.top();//guardamos al padre de y
        //si la raiz cambia la actualizamos
        if (father->m_child[0]->m_data == y->m_data) father->m_child[0] = y->m_child[0];
        else father->m_child[1] = y->m_child[0];}//el padre ahora apunta al hijo de y
    else m_root = y->m_child[0];//si y es la raiz
    cNode<T> * x = y->m_child[0];//guardamos el hijo izq de y
    cNode<T> * b = y->m_child[0]->m_child[1];//guardamos el hijo der. del hijo izq. de y
    y->m_child[0]->m_child[1] = y;//el hijo der. de y, ahora apunta a y
    y->m_child[0]             = b;//el hijo izq. de y, apunta a b*/
}


template<class Tr>
void RB_Tree<Tr>::left_rotate(cNode<T> * x)
{
    cNode<T> * y = x->m_child[1]; //guardamos el hijo derecho de 'x'
    x->m_child[1] = y->m_child[0];//el hijo der. de 'x' ahora apunta al hijo izq. de 'y'
    if (y->m_child[0])// si existe el hijo izq. de 'y' entonces su padre es 'x'
        y->m_child[0]->p = x;
    y->p = x->p;//el padre de 'x' ahora es padre de 'y'
    if (x->p==NIL) {//si 'x' era la raiz entonces 'y' es la nueva raiz
        m_root = y;
        y->p = NIL;
    }
    else if (x == x->p->m_child[0])//si 'x' es el hijo izq. de su padre
         x->p->m_child[0] = y;// el padre cambia de hijo izq. a 'y'
    else x->p->m_child[1] = y;// el padre cambia de hijo der. a 'y'
    y->m_child[0] = x;// el hijo izq. de 'y' ahora apunta a x
    x->p = y; // el padre de 'x' ahora es 'y'
    /*
    if (!s.empty()) s.pop();//sacamos al nodo x del stack
    if (!s.empty()){
        cNode<T> * father = s.top();//guardamos al padre de x
        //si la raiz cambia la actualizamos
        if (father->m_child[0]->m_data == x->m_data) father->m_child[0] = x->m_child[1];
        else father->m_child[1] = x->m_child[1];}//el padre ahora apunta al hijo der. de x
    else m_root = x->m_child[1];//si x es la raiz
    cNode<T> * y = x->m_child[1];//guardamos el hijo der de x
    cNode<T> * b = x->m_child[1]->m_child[0];//guardamos el hijo der. del hijo izq. de x
    x->m_child[1]->m_child[0] = x;//el hijo izq. de x, ahora apunta a x
    x->m_child[1]             = b;//el hijo der. de x, apunta a a b*/
}

template<class Tr>
bool RB_Tree<Tr>::get_color(cNode<T> * x)
{
    if (x==nullptr) {
        return 1;
    }
    else return x->color;
}

//reemplaza uno de los hijos del padre de 'u' con 'v'
template<class Tr>
void RB_Tree<Tr>::transplant(cNode<T> * u, cNode<T> * v)
{
    cout << "u: " << u->m_data << "v: " << v->m_data;
    // si 'u' es la raiz, entonces 'v' es la nueva raiz
    if (u->p==NIL) m_root = v;
    // si 'u' es el hijo izquierdo
    if(u==u->p->m_child[0])
        u->p->m_child[0] = v;//transplantamos 'v' como hijo izq. del padre de 'u'
    else//si 'u' es el hijo derecho
        u->p->m_child[1] = v;//transplantamos 'v' como hijo der. del padre de 'u'
    //actualizamos el padre de 'v' como el padre de 'u'
    /*
    if (v!=NIL) {
        v->p = u->p;
    }*/
    if(v->p && v->p!=NIL)
        v->p = u->p;
}


template<class Tr>
cNode<typename Tr::U> * RB_Tree<Tr>::tree_minimium(cNode<T> * x)
{
    while (x->m_child[0]!=0) {
        x = x->m_child[0];
    }
    return x;
}

//imprimir arbol en inorden
template<class Tr>
void RB_Tree<Tr>::printTree(cNode<T> * p)
{
    if (p != nullptr) {
        printTree(p->m_child[0]);
        cout << p->m_data << " ";
        printTree(p->m_child[1]);
    }
}

//imprimir arbol por amplitud
template<class Tr>
void RB_Tree<Tr>::printTreeAmplitud(cNode<T> * p)
{
    //vector se asemeja a una cola
    vector<cNode<T>*> l;
    l.push_back(p);
    while (!l.empty()) {
        cNode<T> * t = l.front();
        l.erase(l.begin());
        cout << t->m_data <<" ";
        if(t->m_child[0]) l.push_back(t->m_child[0]);
        if(t->m_child[1]) l.push_back(t->m_child[1]);
        cout << endl;
    }
    
}

//imprimir arbol por amplitud
template<class Tr>
void RB_Tree<Tr>::printNodesAndFathers(cNode<T> * p)
{
    if (p != nullptr) {
        printNodesAndFathers(p->m_child[0]);
        cout << "Nodo: " << p->m_data << endl;
        cout << "Padre: " << p->p->m_data << endl << endl;
        printNodesAndFathers(p->m_child[1]);
    }
    
}

string print_color(bool x)
{
    if (x) {
        return "(*)";
    }
    else return "( )";
}

string intToString(int val) {
    ostringstream ss;
    ss << val;
    return ss.str();
}

int maxHeight(cNode<int> *p) {
    if (!p) return 0;
    int leftHeight = maxHeight(p->m_child[0]);
    int rightHeight = maxHeight(p->m_child[1]);
    return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
}


// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel / 2; i++) {
        out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
        out << setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
    }
    out << endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->m_child[0]) ? setfill(' ') : setfill(' '));
        out << setw(branchLen+2) << ((*iter) ? print_color((*iter)->color) : "");
        out << ((*iter && (*iter)->m_child[1]) ? setfill(' ') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
    }
    out << endl;
    iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->m_child[0]) ? setfill('_') : setfill(' '));
        out << setw(branchLen+2) << ((*iter) ? intToString((*iter)->m_data) : "");
        out << ((*iter && (*iter)->m_child[1]) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
    }
    out << endl;
    
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<cNode<int>*>& nodesQueue, ostream& out) {
    deque<cNode<int>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(indentSpace+2) : setw(2*level+2)) << ((*iter) ? print_color((*iter)->color) : "");}
    iter = nodesQueue.begin();
    out << endl;
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        out << ((i == 0) ? setw(indentSpace+2) : setw(2*level+2)) << ((*iter) ? intToString((*iter)->m_data) : "");}
    out << endl;
}

void printPretty(cNode<int>*root, int level, int indentSpace, ostream& out) {
    int h = maxHeight(root);
    int nodesInThisLevel = 1;
    
    int branchLen = 2*((int)pow(2.0,h)-1) - (3-level)*(int)pow(2.0,h-1);
    int nodeSpaceLen = 2 + (level+1)*(int)pow(2.0,h);
    int startLen = branchLen + (3-level) + indentSpace;
    
    deque<cNode<int>*> nodesQueue;
    nodesQueue.push_back(root);
    for (int r = 1; r < h; r++) {
        printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
        branchLen = branchLen/2 - 1;
        nodeSpaceLen = nodeSpaceLen/2 + 1;
        startLen = branchLen + (3-level) + indentSpace;
        printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
        
        for (int i = 0; i < nodesInThisLevel; i++) {
            cNode<int> * currNode = nodesQueue.front();
            nodesQueue.pop_front();
            if (currNode) {
                nodesQueue.push_back(currNode->m_child[0]);
                nodesQueue.push_back(currNode->m_child[1]);
            } else {
                nodesQueue.push_back(NULL);
                nodesQueue.push_back(NULL);
            }
        }
        nodesInThisLevel *= 2;
    }
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
    printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}

int ingresar_opcion()
{
    cout << "Ingrese una OPCION : ";
    int val;
    cin >> val;
    return val;
}

int ingresar_valor()
{
    cout << "dato: ";
    int val;
    cin >> val;
    return val;
}


void pausa()
{
    cout << "Presione una tecla para continuar.... ";
    cin.ignore().get();
}

void clearScreen(){
    for (int i=0; i<100; i++) {
        cout << endl;
    }
}

#endif /* btree_hpp */
