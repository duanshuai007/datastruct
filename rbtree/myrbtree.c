#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent);
//static void rbtree_delete_fixup(RBRoot *root, Node *node);

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
    node->parent = y;

    if (y != NULL) {
        if (node->key < y->key) 
            y->left = node;         //情况2：若“node所包含的值” < “y所包含的值”，则将node设为“y的左孩子”
        else 
            y->right = node;        // 情况3：(“node所包含的值” >= “y所包含的值”)将node设为“y的右孩子” 
    } else {
        root->node = node;           // 情况1：若y是空节点，则将node设为根
    }

    node->left = NULL;
    node->right = NULL;
    //2.设置节点颜色为红色
    node->color = RED;

    //以上步骤是普通二叉树的插入过程，红黑树与二叉树不同点就在于红黑树会进行平衡
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
    while ((parent = node->parent) && (parent->color == RED)) {
        gparent = parent->parent;
        if (parent == gparent->left) {
            //case 1:叔节点存在并且是红色，则执行变色动作
            Node *uncle = gparent->right;
            if (uncle && (uncle->color == RED)) {
                printf("uncle:[red][right]!\n");
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            } else {
                //case 2:叔节点不存在或叔节点是黑色，且当前节点是右子节点
                if (parent->right == node) {
                    printf("uncle:[black][right]-curnode:[right]!\n");
                    Node *temp;
                    rbtree_left_rotate(root, parent);
                    temp = parent;
                    parent = node;
                    node = temp;
                }
                //case 3:叔节点是黑色，并且当前节点是左子节点
                printf("uncle:[black][right]-curnode:[left]!\n");
                parent->color = BLACK;
                gparent->color = RED;
                rbtree_right_rotate(root, gparent);
            }
        } //end of "if (parent == gparent->left) {"
        else {
            //case 1:叔节点是红色
            Node *uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) {
                printf("uncle:[red][left]!\n");
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            } else {
                //case 2:叔节点是黑色，且当前节点是左子节点
                if (parent->left == node) {
                    printf("uncle:[black][left]-curnode:[left]!\n");
                    Node *temp;
                    rbtree_right_rotate(root, parent);
                    temp = parent;
                    parent = node;
                    node = temp;
                }
                //case 3:
                printf("uncle:[black][left]-curnode:[left]!\n");
                parent->color = BLACK;
                gparent->color = RED;
                rbtree_left_rotate(root, gparent);
            }
        }
    }

    root->node->color = BLACK;
}

/*
    前继节点:表示比当前节点小的最大值
    后继节点:表示比当前节点大的最小值
 */
#if 0
static void rbtree_remove(RBRoot *root, Node *node)
{
    Node *x, *y;

    if ((NULL == node->left) || (NULL == node->right)) {
        //0 or 1 child
        y = node;
    } else {
        // 2 child
        //将node的后继节点给y
        Node *replace = node;
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        //node 不是根节点
        if (node->parent != NULL) {
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        } else {
            //node是根节点
            root->node = replace;
        }

        //取代节点的右孩子也是需要调整的节点
        //取代节点肯定不存在左孩子，因为它是一个后继节点
        Node *child = replace->right;
        Node *parent = replace->parent;
        int color = replace->color;

        if (parent == node)
            parent = replace;
        else {
            if (child)
                child->parent = parent;
            parent->left = child;

            replace->right = node->right;
            node->right->parent = replace;
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child);

        free(node);

        return;
    }

    if (NULL != y->left)
        x = y->left;
    else
        x = y->right;
    
    if (NULL != x)
        x->parent = y->parent;
    //case 1: y的父节点为空，设置x位根节点
    if (NULL == y->parent)
        root->node = x;
    else if (y == y->parent->left) 
        y->parent->left = x;
    else
        y->parent->right = y;

    if (y->color == BLACK)
        rbtree_delete_fixup(root, x);

    free(node);
}

static void rbtree_delete_fixup(RBRoot * root, Node *x)
{
    while (!x && x != root->node && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_left_rotate(root, x->parent);
                w = x->parent->right;
            } else if ((!w->left || w->left->color == BLACK) 
                   && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbtree_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rbtree_left_rotate(root, x->parent);
                x = root->node;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_right_rotate(root, x->parent);
                w = x->parent->left;
            } else if ((!w->left || w->left->color == BLACK) 
                   && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbtree_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rbtree_right_rotate(root, x->parent);
                x = root->node;
            }       
        }
    
    }
}

#else
static void rbtree_remove(RBRoot *root, Node *node)
{
    Node *child, *parent;
    int color;

    if ((NULL == node->left) || (NULL == node->right)) {
        //0 or 1 child
        if (NULL != node->left)
            child = node->left;
        else 
            child = node->right;

        parent = node->parent;

        color = node->color;
        if (child)
            child->parent = parent;
        if (parent) {
            if (parent->left == node) 
                parent->left = child;
            else
                parent->right = child;
        } else {
            root->node = child;
        }
        if (color == BLACK) 
            rbtree_delete_fixup(root, child, parent);
    } else {
        //2 child
        Node *replace = node;
        replace = replace->right;
        while (NULL != replace->left)
            replace = replace->left;

        if (rb_parent(node)) {
            if (rb_parent(node)->left == node) {
                rb_parent(node)->left = replace;
            } else {
                rb_parent(node)->right = replace;
            }
        } else {
            root->node = replace;
        }

        child = replace->right;
        parent = rb_parent(replace);
        color = rb_color(replace);

        if (parent == node) {
            parent = replace;
        } else {
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child, parent);
    }

    free(node);
}

static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;
    while ((!node || rb_is_black(node)) && node != root->node) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                //case 1:x的兄弟节点w是红色
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                //case 2:x的兄弟节点w是黑色，并且w的左孩子是黑色，右孩子是黑色
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->right || rb_is_black(other->right)) {
                    //case 3:x的兄弟节点w是黑色，并且w的左孩子是红色，右孩子是黑色
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                //case 4:case 3:x的兄弟节点w是黑色，并且w的右孩子是红色，右孩子是任意色
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        } else {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }

    if (node)
        rb_set_black(node);
}
#endif

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

static Node* search(Node *x, int key) {
    if (x == NULL || x->key == key)
        return x;
    if (key < x->key) 
        return search(x->left, key);
    else
        return search(x->right, key);
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

    const int NUM = 20;

    printf("start test!\n");

    int numarray[100];

    for (int i = 0; i < NUM; i++)
        numarray[i] = i;

    int p1, p2, temp;
    srand(time(0));
    for(int i = NUM - 1; i > 0; i--) {
        p1 = i;
        p2 = rand() % i;
        temp = numarray[p1];
        numarray[p1] = numarray[p2];
        numarray[p2] = temp;
    }
    
    printf("insert:");
    for (int i = 0; i < NUM; i++) {
        printf(" %d", numarray[i]);
    }
    printf("\r\n");

    for (int i = 0; i < NUM; i++) {
        insert_rbtree(root, numarray[i]);
    }
#if 0
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
#endif
    print_rbtree(root);

    Node *delnode = search(root->node, 10);
    printf("search node finish!\n");
    if (delnode) {
        printf("i will remove node\n");
        rbtree_remove(root, delnode);
    } else {
        printf("can't find node\n");
    }

    print_rbtree(root);
    return 0;
}
