#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RED     0
#define BLACK   1

typedef int Type;


#define rb_parent(r)        ((r)->parent)
#define rb_color(r)         ((r)->color)
#define rb_is_red(r)        ((r)->color==RED)
#define rb_is_black(r)      ((r)->color==BLACK)
#define rb_set_black(r)     do { (r)->color  = BLACK; } while (0)
#define rb_set_red(r)       do { (r)->color  = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)   do { (r)->color  = (c); } while (0)

typedef struct RBTreeNode {
    unsigned char color;
    Type key;
    struct RBTreeNode *left;
    struct RBTreeNode *right;
    struct RBTreeNode *parent;
} Node, *RBTree;

typedef struct {
    Node *node;
} RBRoot;

static void rbtree_insert_fixup(RBRoot *root, Node *node);

/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */
static void rbtree_left_rotate(RBRoot *root, Node *x)
{
    //保存x的右子节点为y
    Node *y = x->right;

    //将y的left节点转移到x的right上。
    x->right = y->left;
    //如果y的left节点不是NIL那么还需要将left节点的parent节点设置到x上
    if (NULL != y->left)
        y->left->parent = x;
    //设置y的parent节点等于x的parent节点
    y->parent = x->parent;
    //判断x的parent是否是root
    if (NULL == x->parent) {
        //是root
        root->node = y;
    } else {
        //不是root,还需要判断x节点是parent节点的左节点还是右节点
        if (x->parent->left == x) {
            x->parent->left = y;    
        } else {
            x->parent->right = y;
        }
    }
    //将x转移到y的左节点
    y->left = x;
    //设置x的parent到y节点上
    x->parent = y;
}

/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */

static void rbtree_right_rotate(RBRoot *root, Node *y) 
{
    //x是当前节点y的左子节点
    Node *x = y->left;

    //修改y的左子节点为x的右子节点
    y->left = x->right;
    //将x的右子节点的父节点更改为y
    if (NULL != x->right) {
        x->right->parent = y;
    }

    //x的父节点更改为y的父节点
    x->parent = y->parent;
    if (NULL == y->parent) {
        //如果父节点为空，则将x设置为根节点
        root->node = x;
    } else {
        if (y == y->parent->right)
            y->parent->right = x;   //设置x替换y节点
        else
            y->parent->left = x;
    }
    //更改x的右节点为y
    x->right = y;
    //更改y的父节点为x
    y->parent = x;
}

/*
(1) 每个节点或者是黑色，或者是红色。
(2) 根节点是黑色。
(3) 每个叶子节点是黑色。 [注意：这里叶子节点，是指为空的叶子节点！]
(4) 如果一个节点是红色的，则它的子节点必须是黑色的。
(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
 */

/*
 * 添加节点：将节点(node)插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */
static void rbtree_insert(RBRoot *root, Node *node)
{
    Node *y = NULL;
    Node *x = root->node;

    //1.将红黑树当做一个二叉查找树，将节点添加到二叉查找树中。
    while (x != NULL) {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    rb_parent(node) = y;

    if (y != NULL) {
        if (node->key < y->key) 
            y->left = node;         //情况2：若“node所包含的值” < “y所包含的值”，则将node设为“y的左孩子”
        else 
            y->right = node;        // 情况3：(“node所包含的值” >= “y所包含的值”)将node设为“y的右孩子” 
    } else {
        root->node = node;           // 情况1：若y是空节点，则将node设为根
    }

    //2.设置节点颜色为红色
    node->color = RED;

    //3.将它重新修正为一个红黑树
    rbtree_insert_fixup(root, node);
}


/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */

static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;

    //如果父节点存在，并且父节点是红色
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);
        if (parent == gparent->left) {
            //case 1叔节点是红色
            Node *uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }
            //case 2:叔节点是黑色，且当前节点是右子节点
            if (parent->right == node) {
                Node *temp;
                rbtree_left_rotate(root, parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            //case 3:叔节点是黑色，并且当前节点是左子节点
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        }
        else {
            //case 1:叔节点是红色
            Node *uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }
            //case 2:叔节点是黑色，且当前节点是左子节点
            if (parent->left == node) {
                Node *temp;
                rbtree_right_rotate(root, parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            //case 3:
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    rb_set_black(root->node);
}

static void rbtree_print(RBTree tree, Type key, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
            printf("%2d(B) is root\n", tree->key);
        else                // tree是分支节点
            printf("%2d(%s) is %2d's %6s child\n", tree->key, rb_is_red(tree)?"R":"B", key, direction==1?"right" : "left");

        rbtree_print(tree->left, tree->key, -1);
        rbtree_print(tree->right,tree->key,  1);
    }
}

void print_rbtree(RBRoot *root)
{
    if (root!=NULL && root->node!=NULL)
        rbtree_print(root->node, root->node->key, 0);
}

void insert_rbtree(RBRoot * root, int key) 
{
    Node * node = (Node *)malloc(sizeof(Node));
    if (NULL == node)
        return;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = BLACK;

    rbtree_insert(root, node);
}

int main(void) 
{
    RBRoot * root = (RBRoot *)malloc(sizeof(RBRoot));
    if (NULL == root) {
        printf("malloc rbroot failed\n");
        return -1;
    }

    printf("start test!\n");

    insert_rbtree(root, 1);
    insert_rbtree(root, 3);
    insert_rbtree(root, 96);
    insert_rbtree(root, 45);
    insert_rbtree(root, 29);
    insert_rbtree(root, 68);
    insert_rbtree(root, 67);
    insert_rbtree(root, 12);
    insert_rbtree(root, 57);
    insert_rbtree(root, 88);
    insert_rbtree(root, 9);

    print_rbtree(root);

    return 0;
}
