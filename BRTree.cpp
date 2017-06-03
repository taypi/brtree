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
    
    RedBlackTree (){
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
    
    void printWord(string word){
        int start = (26 - word.length())/2;
        for (int i = 0; i < 26; i++){
            if (i < start || i >= start+word.length()) cout << " ";
            else if (i == start) cout << word;
        }
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
        //make y
        y = x->right;
        
        //connect x->right to b
        x->right = y->left;
        if (x->right!=NIL){
            x->right->parent = x;
        }
        
        //connect y to parent's x
        y->parent = x->parent;
        if (x->parent == NIL) root = y;
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        
        //connect y to x
        y->left = x;
        x->parent = y;
    }
    
    // copy from leftRotate but change x to y, left to right and vice versa
    void rightRotate(Node *y){
        x = y->left;
        
        y->left = x->right;
        if (y->left!=NIL) y->left->parent = y;
        
        x->parent = y->parent;
        if (y->parent == NIL) root = x;
        else if (y->parent->left == y) y->parent->left = x;
        else y->parent->right = x;
        
        x->right = y;
        y->parent = x;
    }

    void insert(string key){
        z = new Node (key);
        z->left = z->right = z->parent = NIL;
        
        x = root;
        y = NIL;
        
        while (x!=NIL){
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        
        if (y==NIL) root = z;
        else if (z->key <  y->key) y->left = z;
        else y->right = z;
        
        z->parent = y;
        
        insertFixup(z);

        cout << "*     Insert     *";
        printWord(key);
        cout << "*    Success    *\n";
        cout << "*************************************************************\n";
    }
    
    void insertFixup(Node *x){
        while (x->parent->color == RED){
            if (x->parent->parent->left == x->parent){
                y = x->parent->parent->right;
                if (y->color == RED){
                    y->color = BLACK;
                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                }else{
                    if (x->parent->right == x){
                        x = x->parent;
                        leftRotate(x);
                    }
                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    rightRotate(x->parent->parent);
                }
            }
            else{
                y = x->parent->parent->left;
                if (y->color == RED){
                    y->color = BLACK;
                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                }else{
                    if (x->parent->left == x){
                        x = x->parent;
                        rightRotate(x);
                    }
                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    leftRotate(x->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node* minimum(Node *x){
        while (x->right != NIL) x = x->right;
        return x;
    }

    void transplant(Node *u, Node *v){
        if (u->parent == NIL) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    void delete_(string key){
        cout << "*     Delete     *";
        printWord(key);
        z = search(key);
        int y_original_color;
        if (z == NIL) {    
            cout << "*      Fail     *\n";
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
            cout << "*    Success    *\n";
        }
        cout << "*************************************************************\n";
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
    
    int blackHeight(){
        return blackHeight(root, root);
    }
    
    int blackHeight(Node *o, Node *n){
        if (n == NIL) return 1;
        else if (n->color == RED || o == n)
            return max(blackHeight(o, n->left), blackHeight(o, n->right));
        else
            return 1 + max(blackHeight(o, n->left), blackHeight(o, n->right));
    }

    string printColor(int n){
        if (n == 0) return "red";
        else return "black";
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
        cout << "(" << printKey(n->parent) << ", " << n->key << ", " << printColor(n->color) << ", " << blackHeight(n, n) << ", " << printKey(n->left) << ", " << printKey(n->right) << ")\n";
        check(n->left);
        check(n->right);
    }
};

int readFile(int *, string *, char *argv[]);

int main (int argc, char *argv[]){
    string word[1000];
    int size, number[1000];

    if (argc != 2) {
        cout<<"Usage: "<< argv[0] <<" <filename>\n";
        return 0;
    }
    else size = readFile(number, word, argv);

    RedBlackTree rbt;

    cout << "\n                    Black-Red Tree Build                    \n\n";
    cout << "*************************************************************\n";
    cout << "*    Function    *           Word           *     Status    *\n";
    cout << "*************************************************************\n";
    for (int i = 0; i < size; i++){
        if (number[i] == 1) rbt.insert(word[i]);
        else rbt.delete_(word[i]);
    }

    cout << "\n\n********************Black-Red Tree Print*********************\n\n";
    rbt.print();
    cout << "\n*************************************************************\n\n";
    cout << "\n********************Black-Red Tree Check*********************\n\n";
    rbt.check();
    cout << "\n*************************************************************\n\n";

    return 0;
}

int readFile(int *number, string *word, char *argv[]) {
    string line;
    int size = 0;
    cout << argv[1] << endl;
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
