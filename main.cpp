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
        void __traversal(Node *&root); //遍历(中序)内部接口
        void __remove(Node *&root,Node *node); //删除内部接口
        void out_correct(Node *&root,Node *node); //删除修正接口
        Node *__search(Node *&root,const keytype key); //查找内部接口
        Node *find_successor_node(Node *node); //寻找后继结点
        void deleteTree(Node *&root); //删除树

    public:
        RBTree():RBTRoot(nullptr){};
        ~RBTree();
        bool search(const keytype key); //查找外部接口
        void insert(const keytype key); //插入外部接口
        void remove(const keytype key); //删除外部接口
        void traversal(); //遍历(中序)外部接口
};

//内部实现
//插入
template <typename keytype>
void RBTree<keytype>::__insert(Node *&root,const keytype key){
    // cout << key << endl;
    Node *node = new Node(key,Black); //新建结点
    //利用指针，非递归插入
    Node *p = nullptr;
    Node *q = root;
    while (q != nullptr){
        p = q;
        if(key < q->key) q = q->left;
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

//查找
template <typename keytype>
RBTNode<keytype> *RBTree<keytype>::__search(Node *&root,const keytype key){
    Node *p = root;
    while(p != nullptr){
        if(key == p->key) return p;
        else if(key > p->key) p = p->right;
        else p = p->left;
    }
    return p;
}

//寻找后继结点
template <typename keytype>
RBTNode<keytype> *RBTree<keytype>::find_successor_node(Node *node){
    Node *p = node->right;
    while (p != nullptr){
        p = p->left;
    }
    return p;
}

//遍历(中序)
template <typename keytype>
void RBTree<keytype>::__traversal(Node *&root){
    if(root == nullptr) return;
    __traversal(root->left);
    // cout << root->key << "(" << root->color << ") ";
    cout << root->key << " ";
    __traversal(root->right);
}

//删除
template <typename keytype>
void RBTree<keytype>::__remove(Node *&root,Node *node){
    Node *parent,*child;
    //待删除结点的左右孩子都不为空
    if((node->left != nullptr) && (node->right != nullptr)){
        Node *replace = find_successor_node(node);
        node->key = replace->key;
        parent = replace->parent;
        child = replace->right;
        if(child){
            if(parent == node){
                node->right = child;
                child->parent = node;
            }else{
                parent->left = child;
                child->parent = parent;
            }
        }
        if(replace->color == Black) out_correct(root,child);
        delete replace;
        return;
    }
    //待删除结点有一个孩子或没有孩子结点
    if(node->left != nullptr) child = node->left; //待删除结点有左孩子
    else child = node->right; //待删除结点有右孩子
    parent = node->parent;
    if(child) child->parent = parent;
    if(parent){ //待删除结点不是根结点
        if(parent->left == node){
            parent->left = child;
        }else{
            parent->right = child;
        }
    }else{
        root = child;
    }
    if(node->color == Black) out_correct(root,child);
    delete node;
}

//删除修正
template <typename keytype>
void RBTree<keytype>::out_correct(Node *&root,Node *node){
    while(node != root && node->color == Black){
        Node *brother;
        Node *parent = node->parent;
        if(parent->left = node){
            brother = parent->right;
            if(brother->color == Red){
                brother->color = Black;
                parent->color = Red;
                left_rotate(root,parent);
                brother = parent->right;
            }
            if((brother->left->color == Black) && (brother->right->color == Black)){
                brother->color = Red;
                node = node->parent;
            }else if(brother->right->color == Black){
                brother->left->color = Black;
                brother->color = Red;
                right_rotate(root,brother);
                brother = parent->right;
            }else{
                brother->color = parent->color;
                parent->color = Black;
                brother->right->color = Black;
                left_rotate(root,parent);
                node = root;
            }
        }else{
            brother = parent->left;
            if(brother->color == Red){
                brother->color = Black;
                parent->color = Red;
                right_rotate(root,parent);
                brother = parent->left;
            }
            if((brother->left->color == Black) && (brother->right->color == Black)){
                brother->color = Red;
                node = node->parent;
            }else if(brother->left->color == Black){
                brother->right->color = Black;
                brother->color = Red;
                left_rotate(root,brother);
                brother = parent->left;
            }else{
                brother->color = parent->color;
                parent->color = Black;
                brother->left->color = Black;
                right_rotate(root,parent);
                node = root;
            }
        }
    }
    node->color = Black;
}

//删除树
template <typename keytype>
void RBTree<keytype>::deleteTree(Node *&root){
    if(root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    root = nullptr;
    return;
}

//外部接口
//插入
template <typename keytype>
void RBTree<keytype>::insert(const keytype key){
    __insert(RBTRoot,key);
}

//查找
template <typename keytype>
bool RBTree<keytype>::search(const keytype key){
    return __search(RBTRoot,key) == nullptr ? false : true;
}

//遍历(中序)
template <typename keytype>
void RBTree<keytype>::traversal(){
    __traversal(RBTRoot);
}

//删除
template <typename keytype>
void RBTree<keytype>::remove(const keytype key){
    Node *p = __search(RBTRoot,key);
    if(p != nullptr) __remove(RBTRoot,p);    
}

//析构函数
template <typename keytype>
RBTree<keytype>::~RBTree(){
    deleteTree(RBTRoot);
}

int main(){
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(50);
    tree.insert(15);
    tree.insert(40);
    tree.traversal();
    cout << endl;

    cout << tree.search(15) << endl;
    tree.remove(15);
    tree.traversal();
    cout << endl;
    return 0;
}