//
//  main.cpp
//  red_black_tree
//
//  Created by Alexander Arturo Baylon Ibanez on 30/11/15.
//  Copyright © 2015 aunesto17. All rights reserved.
//

#include "rb_tree.hpp"

int main(int argc, const char * argv[]) {
    
    RB_Tree<RBTreeTrait<int>> prueba;
    
    cout << prueba.insert(11) << endl;
    //cout << "padre: " << prueba.m_root->p->m_data << endl;
    cout << prueba.insert(2) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(14) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(1) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(7) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(15) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(5) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    cout << prueba.insert(8) << endl;
    
    
    cout << prueba.insert(4) << endl;
    printPretty(prueba.m_root, 1, 0, cout);
    //prueba.remove(5);
    prueba.remove(7);
    printPretty(prueba.m_root, 1, 0, cout);
    prueba.remove(15);
    printPretty(prueba.m_root, 1, 0, cout);
    prueba.remove(5);
    
    //cout << prueba.insert(10) << endl;
    //cout << prueba.insert(11) << endl;
    //printPretty(prueba.m_root, 1, 0, cout);
    //cout << prueba.insert(10) << endl;
    //printPretty(prueba.m_root, 1, 5, cout);
    //cout << prueba.insert(11) << endl;
    //printPretty(prueba.m_root, 1, 5, cout);
    //cout << prueba.insert(12) << endl;
    
    

    /*
    cout << prueba.insert(7) << endl;
    cout << prueba.insert(6) << endl;
    cout << prueba.insert(8) << endl;
    cout << prueba.insert(12) << endl;
    cout << prueba.insert(11) << endl;
    cout << prueba.insert(13) << endl;*/
    
    
    //printPretty(prueba.m_root, 1, 0, cout);
    //prueba.printTree(prueba.m_root);
    //prueba.printNodesAndFathers(prueba.m_root);
    printPretty(prueba.m_root, 2, 20, cout);
    return 0;
}
