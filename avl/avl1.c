#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int age;
    char name[32];
} DataType;

#define INSERT_RIGHT     1
#define INSERT_LEFT     2

static int lefttree_depth = 0;
static int righttree_depth = 0;


typedef struct avlnode_t {
    int key;
    struct avlnode_t *parent;
    struct avlnode_t *left;
    struct avlnode_t *right;
    DataType data;
} avlnode_t;

void print_avltree(avlnode_t *root);

//输入节点是旋转的中心节点，返回的是改组节点内最上端的节点
static avlnode_t * avltree_rr (avlnode_t *root) 
{
    avlnode_t *temp = NULL;
    if (root != root->parent->left) {
        //case 2
        temp = root->left;
        temp->parent = root->parent;
        root->parent->right = temp;
        root->left = temp->right;
        if (temp->right)
            temp->right->parent = root;
        temp->right = root;
        root->parent = temp;
        return temp->parent;
    } else {
        //case 1
        temp = root->parent;
        root->parent = temp->parent;
        if (temp->parent) {
            if (temp == temp->parent->left)
                temp->parent->left = root;
            else
                temp->parent->right = root;
        }
        temp->left = root->right;
        if (root->right)
            root->right->parent = temp;
        root->right = temp;
        temp->parent = root;
       return root; 
    }
}
//左旋,输入节点为根节点，返回的是新的根节点
/*
     30     30
     /       \
   12        40 root
    \          \
     9          50
 */
//输入节点是旋转的中心节点，返回的是改组节点内最上端的节点
static avlnode_t * avltree_ll (avlnode_t *root) 
{
    avlnode_t *temp = NULL;
    if (root == root->parent->left) {
        //case 1
        temp = root->right;
        temp->parent = root->parent;
        root->parent->left = temp;
        root->right = temp->left;
        if (temp->left)
            temp->left->parent = root;
        temp->left = root;
        root->parent = temp;
        return temp->parent;
    } else {
        //case 2
        temp = root->parent;
        root->parent = temp->parent;
        if (temp->parent) {
            if (temp == temp->parent->left)
                temp->parent->left = root;
            else
                temp->parent->right = root;
        }
        temp->right = root->left;
        if (root->left)
            root->left->parent = temp;
        root->left = temp;
        temp->parent = root;
       return root; 
    }
}

avlnode_t * avltree_init(int key, DataType *pData)
{
    avlnode_t * p = (avlnode_t *)malloc(sizeof(avlnode_t));
    if (p) {
        p->left = NULL;
        p->right = NULL;
        p->parent = NULL;
        p->key = key;
        memcpy(&p->data, pData, sizeof(DataType));   
        printf("root:%p\n", p);
        return p;
    }
    
    return NULL;
}

static int getTreeDepth(avlnode_t *root) {
    if (root != NULL) {
        int left = getTreeDepth(root->left);
        int right = getTreeDepth(root->right);
        return left > right ? left + 1: right + 1;
    }
    return 0;
}

//paramters: the 3 starts last one
static avlnode_t *do_three_start(avlnode_t *node)
{
    avlnode_t *parent = node->parent;
    if (parent == NULL) 
        return NULL;

    avlnode_t *gparent = parent->parent;
    if (gparent == NULL)
        return NULL;

    if (gparent->left && gparent->right)
        return NULL;
    if (parent->left && parent->right)
        return NULL;

    //只处理3个链接的节点，且父节点与祖父节点都是只有一个子节点的情况
    if (node == parent->left) {
        avlnode_t *replace = NULL;
        if (parent == gparent->left) {
            replace = avltree_rr(parent);
        } else {
            replace = avltree_rr(parent);
            replace = avltree_ll(replace->right);
        }

        if (replace->parent == NULL) {
            return replace;
        } else 
            return NULL;
    } else {
        avlnode_t *replace = NULL;
        if (parent == gparent->left) {
            replace = avltree_ll(parent);
            replace = avltree_rr(replace->left);
        } else {
            replace = avltree_ll(parent);
        }
        
        if (replace->parent == NULL) {
            return replace;
        } else 
            return NULL;
    }
}

avlnode_t * do_left_right_rotate(avlnode_t *root)
{
    int right = getTreeDepth(root->right);
    int left = getTreeDepth(root->left);
    avlnode_t *new = NULL;
    //printf("left:%d right:%d\n", left, right);
    
    if ((right > left) && (right - left > 2)) {
        new = avltree_ll(root->right);
        //new = do_left_right_rotate(root->right);
        //if (new != root->right) {
        //    new = avltree_ll(root->right);
        //}
        return new;
    } else if ((right < left) && (left - right > 2)) {
        new = avltree_rr(root->left);
        //new = do_left_right_rotate(root->left);
        //if (new != root->left) {
        //    new = avltree_ll(root->left);
        //}
        return new;
    } else {
        return root;
    }
}

avlnode_t * avltree_insert(avlnode_t *root, avlnode_t *p)
{
    bool isLeft = false;
    int insertLocation = 0;

    avlnode_t * node = root;
    
    if (p->key < root->key)
        isLeft = true;

    while (node) {
        if (p->key > node->key) {
            if (node->right)
                node = node->right;
            else {
                avlnode_t * new = (avlnode_t *)malloc(sizeof(avlnode_t));
                if (new) {
                    node->right = new;
                    new->key = p->key;
                    new->parent = node;
                    new->left = NULL;
                    new->right = NULL;
                    memcpy(&new->data, &p->data, sizeof(DataType));
                    insertLocation = INSERT_RIGHT;
                    avlnode_t *newroot = do_three_start(new);
                    if (newroot)
                        root = newroot;
                    break;
                } else {
                    return NULL;
                }
            }
        } else if (p->key < node->key) {
            if (node->left)
                node = node->left;
            else {
                avlnode_t * new = (avlnode_t *)malloc(sizeof(avlnode_t));
                if (new) {
                    node->left = new;
                    new->key = p->key;
                    new->parent = node;
                    new->left = NULL;
                    new->right = NULL;
                    memcpy(&new->data, &p->data, sizeof(DataType));
                    insertLocation = INSERT_LEFT;
                    avlnode_t *newroot = do_three_start(new);
                    if (newroot)
                        root = newroot;
                    break;
                } else {
                    return NULL;
                }
            }
        } else {
            memcpy(&node->data, &p->data, sizeof(DataType));
            return root;
        }
    }

    root = do_left_right_rotate(root);

    return root;

}

static avlnode_t * find_max_key_node(avlnode_t *root) 
{
    avlnode_t *node = root;
    while (node->right) {
        node = node->right;
    }

    return node;
}

static avlnode_t * find_min_key_node(avlnode_t *root)
{
    avlnode_t *node = root;
    while (node->left) {
        node = node->left;
    }
    return node;
}
#if 0
avlnode_t *avltree_remove(avlnode_t *root, avlnode_t *p)
{
    avlnode_t * node = root;
    while(node) {
        if (p->key > node->key)
            node = node->right;
        else if (p->key < node->key)
            node = node->left;
        else
            break;
    }
    
    if (node) {
        printf("find will remove node!\n");
        if (node->parent == NULL) {
            printf("is root\n");
            //root
            /* 如果左右子树都非空，则分为两种情况：
               1.左子树高于右子树，将左子树最大的那个元素赋给当前根节点，然后删除左子树中元素值最大的那个节点
               2.右子树高于左子树，将右子树最小的那个元素赋给当前根节点，然后删除右子树中元素值最小的那个节点
             */
            if (node->left && node->right) {
                int leftdep = getTreeDepth(node->left);
                int rightdep = getTreeDepth(node->right);
                if (leftdep > rightdep) {
                    //用root节点的左孩子替换root
                    node = avltree_rr(node);
                    printf("in remove-----------\n");
                    print_avltree(node);
#if 1
                    //从右子树中找到最小值的节点替换被删除的节点
                    avlnode_t * temp = NULL;
                    if(node->right->left)
                        temp = find_min_key_node(node->right->left);
                    else
                        temp = find_min_key_node(node->right->right);

                    avlnode_t * right = node->right;
                    node->right = temp;
                    if (temp->parent != right) {
                        temp->left  = right->left;
                        temp->right = right->right;
                        if (temp == temp->parent->left)
                            temp->parent->left = NULL; //原本指向该节点的指针晴空
                        else
                            temp->parent->right = NULL;
                    }
                    temp->parent = node;
                    free(right);
#endif
                    return node;
                } else {
                    node = avltree_ll(node);
#if 1
                    //从左子树中找到最大值的节点替换被删除的节点
                    avlnode_t * temp = NULL;
                    if (node->left->right)
                        temp = find_max_key_node(node->left->right);
                    else
                        temp = find_max_key_node(node->left->left);
                    avlnode_t * left = node->left;
                    node->left = temp;
                    if (temp->parent != left) { //如果被删除的节点与替换的节点不是父子关系
                        temp->left = left->left;
                        temp->right = left->right;
                        if (temp == temp->parent->right)
                            temp->parent->right = NULL; //原本指向该节点的指针晴空
                        else
                            temp->parent->left = NULL;
                    }
                    temp->parent = node;
                    free(left);
#endif
                    return node;
                }
            }
            /* 如果左右子树有空的，那么直接用非空子树或NULL替换当前根节点。
             */
            else {
                if (node->left) {
                    avlnode_t *newroot = node->left;
                    newroot->parent = NULL;
                    free(node);
                    return newroot;
                } else {
                    avlnode_t *newroot = node->right;
                    newroot->parent = NULL;
                    free(node);
                    return newroot;
                }
            }

        }
    }

    return NULL;
}
#endif
static void print_node(avlnode_t *node, int leftorright)
{
    if (node) {
        if (node->parent)
            printf("node key = %d, parent key = %d, is %s child\n", node->key, node->parent->key, leftorright == 0?"left":"right");
        else
            printf("root key = %d\n", node->key);

        print_node(node->left, 0);
        print_node(node->right, 1);
    }
}

void print_avltree(avlnode_t *root)
{
    print_node(root, 0);
}

int main(void)
{
    avlnode_t *root;
    const int NUM = 10000;

    avlnode_t *node = (avlnode_t *)malloc(sizeof(avlnode_t));
    if (node == NULL) {
        printf("malloc error\n");
        return -1;
    }

    DataType dat;
    root = avltree_init(30, &dat);
    if (root) {
        printf("avltree_init success\n");
    } else {
        printf("avltree_init failed\n");
        return -1;
    }

    int numarray[NUM];
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
#if 0
    for (int i = 0; i < NUM; i++) {
        printf("%d ", numarray[i]);
    }
    printf("\n");
#endif
    for (int i = 0; i < NUM; i++) {
        node->key = numarray[i];
        root = avltree_insert(root, node);
    }
    
    printf("left depth:%d\n", getTreeDepth(root->left));
    printf("right depth:%d\n", getTreeDepth(root->right));
    //`print_avltree(root);
#if 0
    printf("del root\n");
    root = avltree_remove(root, root);
    print_avltree(root);
#endif
}   
