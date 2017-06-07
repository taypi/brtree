/* 
dictionary implemented using red black tree
usage: g++ RBTree.cpp -o RBTree && ./RBTree <filename>
       the file must have a word followed by the number 1 (to insert) or 0 (to delete) per line
author: Taynara Pinheiro
*/ 

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int const RED = 0;
int const BLACK = 1;

class RedBlackTree {
public:
    struct Node{
        string key;
        Node *left, *right, *parent;
        int color;
        
        Node(string k){
            key = k;
            left = right = parent = NULL;
            color = RED;
        }
        
        Node(){
            left = right = parent = NULL;
            color = BLACK;
        }
        
    } *root, *x, *y, *z, *NIL;
    
    RedBlackTree(){
        NIL = new Node();
        root = NIL;
    }
    
    ~RedBlackTree(){
        clear();
        delete(NIL);
    }

    void clear(){
        clear(root);
        root = NIL;
    }
    
    void clear(Node *n){
        if (n == NIL) return;
        clear(n->left);
        clear(n->right);
        delete(n);
    }
    
    void print(){
        print(root);
        cout << endl;
    }
    
    void print(Node *n){
        if (n == NIL) return;
        print(n->left);
        cout << n->key << " ";
        print(n->right);
    }

    Node* search(string key){
        x = root;
        
        while (x!=NIL){
            if (key < x->key){
                x = x->left;
            }
            else if (key > x->key){
                x = x->right;
            }
            else break;
        }
        
        return x;
    }
    
    void leftRotate(Node *x){
        y = x->right;
        
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        
        y->parent = x->parent;
        if (x->parent == NIL) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        
        y->left = x;
        x->parent = y;
    }
    
    void rightRotate(Node *x){
        y = x->left;
        
        x->left = y->right;
        if (y->right != NIL) y->right->parent = x;
        
        y->parent = x->parent;
        if (x->parent == NIL) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        
        y->right = x;
        x->parent = y;
    }

    void insert(string key){
        z = search(key);
        if (z == NIL) {
            z = new Node(key);
            z->left = z->right = z->parent = NIL;
            
            y = NIL;
            x = root;
            
            while (x != NIL){
                y = x;
                if (z->key < x->key) x = x->left;
                else x = x->right;
            }

            z->parent = y;      
            if (y == NIL) root = z;
            else if (z->key <  y->key) y->left = z;
            else y->right = z;
            
            insertFixup(z);
        }
        else cout << "Palavra " << key << " ja foi inserida anteriormente\n\n";
    }
    
    void insertFixup(Node *z){
        while (z->parent->color == RED){
            if (z->parent == z->parent->parent->left){
                y = z->parent->parent->right;
                if (y->color == RED){
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else{
                    if (z == z->parent->right){
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else{
                y = z->parent->parent->left;
                if (y->color == RED){
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }else{
                    if (z->parent->left == z){
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node* minimum(Node *x){
        while (x->left != NIL) x = x->left;
        return x;
    }

    void transplant(Node *u, Node *v){
        if (u->parent == NIL) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    void delete_(string key){
        z = search(key);
        int y_original_color;
        if (z == NIL) {    
            cout << "A palavra " << key << " foi removida anteriormente ou nao foi inserida.\n\n";
        }
        else {
            y = z;
            y_original_color = y->color;
            if (z->left == NIL){
                x = z->right;
                transplant(z, z->right);
            }
            else if (z->right == NIL){
                x = z->left;
                transplant(z, z->left);
            }
            else {
                y = minimum(z->right);
                y_original_color = y->color;
                x = y->right;
                if (y->parent == z) x->parent = y;
                else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            if (y_original_color == BLACK) deleteFixup(x);
            cout << "Removendo a palavra " << key << endl;
            print();
            check();
        }
    }

    void deleteFixup(Node *x){
        while (x != root && x->color == BLACK){
            if (x == x->parent->left){
                y = x->parent->right;
                if (y->color == RED){
                    y->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    y = x->parent->right;
                }
                if (y->left->color == BLACK && y->right->color == BLACK){
                    y->color = RED;
                    x = x->parent;
                }
                else{
                    if (y->right->color == BLACK){
                        y->left->color = BLACK;
                        y->color = RED;
                        rightRotate(y);
                        y = x->parent->right;
                    }
                    y->color = x->parent->color;
                    x->parent->color = BLACK;
                    y->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else{
                y = x->parent->left;
                if (y->color == RED){
                    y->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    y = x->parent->left;
                }
                if (y->right->color == BLACK && y->left->color == BLACK){
                    y->color = RED;
                    x = x->parent;
                }
                else{
                    if (y->left->color == BLACK){
                        y->right->color = BLACK;
                        y->color = RED;
                        leftRotate(y);
                        y = x->parent->left;
                    }
                    y->color = x->parent->color;
                    x->parent->color = BLACK;
                    y->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    int blackHeight(Node *n){
        int blackHeight = 0;
        while (n->left != NIL){
            if (n->left->color == BLACK) blackHeight++;
            n = n->left;
        }
        return blackHeight+1;
    }

    string printColor(int n){
        if (n == RED) return "vermelho";
        else return "preto";
    }

    string printKey(Node *n){
        string key;
        return key = (n == NIL) ? "Nil" : n->key;
    }

    void check(){
        check(root);
    }

    void check(Node *n){
        if (n == NIL) return;
        cout << "(" << printKey(n->parent) << ", " << n->key << ", " << printColor(n->color) << ", " << blackHeight(n) << ", " << printKey(n->left) << ", " << printKey(n->right) << ")\n\n";
        check(n->left);
        check(n->right);
    }
};

int readFile(int *, string *, char *argv[]);

int main (int argc, char *argv[]){
    string word[100000];
    int size, number[100000];

    if (argc != 2) {
        cout<<"Usage: "<< argv[0] <<" <filename>\n";
        return 0;
    }
    size = readFile(number, word, argv);

    RedBlackTree rbt;

    for (int i = 0; i < size; i++){
        if (number[i] == 1) rbt.insert(word[i]);
        else if (number[i] == 0) rbt.delete_(word[i]);
    }

    rbt.print();
    rbt.check();

    return 0;
}

int readFile(int *number, string *word, char *argv[]) {
    string line;
    int size = 0;
    ifstream myfile(argv[1]);
    if (myfile.is_open()) {
        while (getline(myfile,line)){
            stringstream s(line);
            s >> word[size] >> number[size];
            size++;
        }
        myfile.close();
    }
    return size;
}
