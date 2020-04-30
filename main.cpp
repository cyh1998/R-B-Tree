#include <iostream>

using namespace std;

enum RBTColor{Red,Black};

template <typename keytype>
class RBTNode{
    public:
        RBTColor color;
        keytype key;
        RBTNode *left;
        RBTNode *right;
        RBTNode *parent;
        RBTNode(keytype k,RBTColor c):key(k),color(c),left(nullptr),right(nullptr),parent(nullptr){}
};

template <typename keytype>
class RBTree{
    typedef RBTNode<keytype> Node;

    private:
        Node *RBTRoot;
        void __insert(Node *&root,const keytype key); //插入内部接口
        void in_correct(Node *&root,Node *node); //插入修正接口
        void left_rotate(Node *&root,Node *x); //左旋
        void right_rotate(Node *&root,Node *y); //右旋
        

    public:
        RBTree();
        ~RBTree();

        void insert(const keytype key);
};

//内部实现
//插入
template <typename keytype>
void RBTree<keytype>::__insert(Node *&root,const keytype key){
    Node *node = new Node(key,Black); //新建结点
    //利用指针，非递归插入
    Node *p = nullptr;
    Node *q = root;
    while (q != nullptr){
        p = q;
        if(key < p->key) q = q->left;
        else q = q->right;
    }
    node->parent = p;
    if(p != nullptr){
        if(key < p->key) p->left = node;
        else p->right = node;
    }else root = node;
    node->color = Red;
    in_correct(root,node); //修正红黑树
}

//插入修正
template <typename keytype>
void RBTree<keytype>::in_correct(Node *&root,Node *node){
    Node *gparent = nullptr;
    while(node->parent != nullptr && node->parent->color == Red){
        gparent = node->parent->parent;
        if(node->parent == gparent->left){
            Node *uncle = gparent->right;
            if(uncle->color == Red){
                node->parent->color = Black;
                uncle->color = Black;
                gparent->color = Red;
                node = gparent;
            }else{
                if(node->parent->left == node){
                    node->parent->color = Black;
                    gparent->color = Red;
                    right_rotate(root,gparent);
                }else{
                    node = node->parent;
                    left_rotate(root,node);
                }
            }
        }else{
            Node *uncle = gparent->left;
            if(uncle->color == Red){
                node->parent->color = Black;
                uncle->color = Black;
                gparent->color = Red;
                node = gparent;
            }else{
                if(node->parent->right == node){
                    node->parent->color = Black;
                    gparent->color = Red;
                    right_rotate(root,gparent);
                }else{
                    node = node->parent;
                    left_rotate(root,node);
                }
            }
        }
    }
    root->color = Black;
}

//左旋
template <typename keytype>
void RBTree<keytype>::left_rotate(Node *&root,Node *x){
    Node *y = x->right; //设x的右孩子为y
    x->right = y->left; //将y的左孩子设为x的右孩子
    if(y->left != nullptr) y->left->parent = x; //将y左孩子的父亲设为x
    y->parent = x->parent; //将y的父亲设为x的父亲
    //情况1：x的父亲为空结点
    if(x->parent == nullptr) root = y; //将y设为根结点
    else{
        //情况2：x是其父亲的左孩子
        if(x->parent->left == x) x->parent->left = y; //将y设为x父结点的左孩子
        //情况3：x是其父亲的右孩子
        else x->parent->right = y; //将y设为x父结点的右孩子
    }
    y->left = x; //将x设为y的左孩子
    x->parent = y; //将y设为x的父结点
}

//右旋
template <typename keytype>
void RBTree<keytype>::right_rotate(Node *&root,Node *y){
    Node *x = y->left; //设y的左孩子为x
    y->left = x->right; //将x的左孩子设为y的右孩子
    if(x->right != nullptr) x->right->parent = y; //将x右孩子的父亲设为y
    x->parent = y->parent; //将x的父亲设为y的父亲
    //情况1：y的父亲为空结点
    if(y->parent == nullptr) root = x; //将x设为根结点
    else{
        //情况2：y是其父亲的右孩子
        if(y->parent->right == y) y->parent->right = x; //将x设为y父结点的右孩子
        //情况3：y是其父亲的左孩子
        else y->parent->left = x; //将x设为y父结点的作孩子
    }
    x->right = y; //将y设为x的右孩子
    y->parent = x; //将x设为y的父结点
}

//外部接口
//插入
template <typename keytype>
void RBTree<keytype>::insert(const keytype key){
    __insert(RBTRoot,key);
}


int main(){
    cout << "test" << endl;
    return 0;
}