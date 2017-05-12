#include <iostream>
#include <fstream>

template<class T>
class AVLNode {
public:
    AVLNode() {
        left = right = nullptr;
        balance = 0;
    }
    AVLNode(const T& el, AVLNode *l = nullptr, AVLNode *r = nullptr) {
        key = el; left = l; right = r;
        balance = 0;
    }
    T key;
    AVLNode *left, *right;
    int balance;
};

template<class T>
class AVLTree {
public:
    AVLTree() { root = nullptr; }

    void insert(const T& key) { insert(root,key); }

    void snapShot(std::ostream& out) {
        out << "TreeForm[";
        snapShot(out,root);
        out << "]" << std::endl; }
private:
    AVLNode<T>* root;

    void rightRotation(AVLNode<T>* &node) {
        AVLNode<T>* left = node->left;

        node->left = left->right;
        left->right = node;
        node = left;
        //update balance factors
        node->right->balance++;
        if(node->balance < 0)
            node->right->balance -= node->balance;
        node->balance++;
        if(node->right->balance > 0)
            node->balance+= node->right->balance;
    }

    void leftRotation(AVLNode<T>* &node) {
        AVLNode<T>* right = node->right;

        node->right = right->left;
        right->left = node;
        node = right;
        //update balance factors
        node->left->balance--;
        if(node->balance > 0)
            node->left->balance -= node->balance;
        node->balance--;
        if(node->left->balance < 0)
            node->balance += node->left->balance;
    }

    bool insert(AVLNode<T>* &node,const T& key) {
        if(node == nullptr) {
            node = new AVLNode<T>(key);
            return true;
        }else if(node->key == key){
            return false;
        }
        else if(key > node->key) {
            if(insert(node->right,key)){
                node->balance += 1;//the balance factor needs to be updated because there was a change in the right sub-tree
                if(node->balance == 0){//we have not changed the height of the subtree and AVL is intacked
                    return false;
                }else if(node->balance > 1){//rotations back to AVL; return false bc balance factors above are not changed
                    //determine case: was the element inserted into the right of the right node or the left of the right node
                    if(node->right->balance == -1){//hard case, two rotations
                        rightRotation(node->right);
                        leftRotation(node);
                    }else{
                        leftRotation(node);
                    }
                    return false;
                }
                return true;//bc I have increased height of subtree
            }
        }else if(insert(node->left,key)){
            node->balance -= 1;
            if(node->balance == 0){
                return false;
            }else if(node->balance < -1){
                if(node->left->balance == -1){//the easy case
                    rightRotation(node);
                }else{//hard case
                    leftRotation(node->left);
                    rightRotation(node);
                }
                return false;
            }
            return true;
        }
        return false;
    }

    void snapShot(std::ostream& out,AVLNode<T> *p) {
        out << '\"' << p->key << ":" << p->balance << '\"';
        if(p->left != nullptr || p->right != nullptr) {
            out << '[';
            if(p->left==nullptr)
                out << "\"\"";
            else
                snapShot(out,p->left);
            out << ',';
            if(p->right==nullptr)
                out << "\"\"";
            else
                snapShot(out,p->right);
            out << ']';
        }
    }
};



int main() {
    AVLTree<int> x;

    std::ifstream file ("numbers.txt");
    int z;
    while(file >> z)
        x.insert(z);

    std::ofstream out("tree.txt");

    x.snapShot(out);

    out.close();

    return 0;
}