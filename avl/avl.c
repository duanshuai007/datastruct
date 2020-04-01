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
    struct avlnode_t *left;
    struct avlnode_t *right;
    DataType data;
} avlnode_t;

void print_avltree(avlnode_t *root);

//输入节点是root节点,输出的新的root节点
/*      case 1                 case 2
          10 R                  10
        /   \                  /  \
      8      20 R             8    20
           /   \                  /  \
         15    30                15   30
                \                     / 
                 45                  25
 */
//右子树的右节点
static avlnode_t * avltree_rr (avlnode_t *root) 
{
    if (root == NULL) {
        printf("avltree_ll root = NULL\n");
        return NULL;
    }
    avlnode_t *rchild = root->right;
    if (rchild == NULL) {

        /*
            30
           /   \
         25     40
         /       \
        20       50 
        /  \       \
       15   25      60
       /    /  \
      nil  21   nil   
            \
             23    
         */
        printf("in avltree_rr speacil\n");
        avlnode_t *tmproot = root->left;
        avlnode_t *tmpparent = root;
        while (tmproot->right == NULL) {
            tmpparent = tmproot;
            tmproot = tmproot->left;
            if (tmproot == NULL)
                return root;
        }
        
        tmpparent->left = tmproot->right;
        tmproot->right = tmproot->right->left;
        tmpparent->left->left = tmproot;

        return root;
    }

    root->right = rchild->left;
    rchild->left = root;
    return rchild;
}

//输入节点是root节点,输出的新的root节点
/*     case 1              case 2
        10                  10
       /  \               /   \
      8    20            8     20
     / \                / \
    4   9              4   9
   /                   \
  2                     5  
 */
static avlnode_t * avltree_ll (avlnode_t *root) 
{
    if (root == NULL) {
        printf("avltree_ll root = NULL\n");
        return NULL;
    }
    avlnode_t * lchild = root->left;
    if (lchild == NULL) {
        /*
            30
           /   \
         27    44 (tmpparent)
         /      \
        22       74(tmproot)
        /        /  \
        8       72   84
                / \
               68  (null)
                \
                 69
         */
        printf("in avltree_ll speacil\n");
        avlnode_t *tmproot = root->right;
        avlnode_t *tmpparent = root;
        while (tmproot->left == NULL) {
            tmpparent = tmproot;
            tmproot = tmproot->right;
            if (tmproot == NULL)
                return root;
        }
        
        tmpparent->right = tmproot->left;
        tmproot->left = tmproot->left->right;
        tmpparent->right->right = tmproot;

        return root;
    }
    
    root->left = lchild->right;
    lchild->right = root;

    return lchild;
}
/*
        case 1                          case 2
        10                                 10
        / \                               /  \
       8   20                           8    20
           /  \                              / \
          15   30                           15  30
          /                                   \
         12                                   18  
 */
static avlnode_t * avltree_lr(avlnode_t *root)
{
    root->left = avltree_rr(root->left);
    return avltree_ll(root);
}
/*
        case 1                  case 2
       10                       10
      / \                       / \
     8  20                     8   20    
    / \                       /  \
   6   9                     6    9
       \                         / 
       9.5                      8.5  
 */
static avlnode_t * avltree_rl(avlnode_t *root)
{
    root->right = avltree_ll(root->right);
    return avltree_rr(root);
}

avlnode_t * avltree_init(int key, DataType *pData)
{
    avlnode_t * p = (avlnode_t *)malloc(sizeof(avlnode_t));
    if (p) {
        p->left = NULL;
        p->right = NULL;
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

avlnode_t * avltree_insert(avlnode_t *root, avlnode_t *p)
{
    bool isLeft = false;

    avlnode_t *node = root;
    avlnode_t *pnode = NULL;
    
    if (p->key < root->key)
        isLeft = true;

    while (node) {
        if (p->key > node->key) {
            if (node->right) {
                pnode = node;
                node = node->right;
            } else {
                avlnode_t * new = (avlnode_t *)malloc(sizeof(avlnode_t));
                if (new) {
                    node->right = new;
                    new->key = p->key;
                    new->left = NULL;
                    new->right = NULL;
                    memcpy(&new->data, &p->data, sizeof(DataType));
                    break;
                } else {
                    printf("insert malloc failed\n");
                    return NULL;
                }
            }
        } else if (p->key < node->key) {
            if (node->left) {
                pnode = node;
                node = node->left;
            } else {
                avlnode_t * new = (avlnode_t *)malloc(sizeof(avlnode_t));
                if (new) {
                    node->left = new;
                    new->key = p->key;
                    new->left = NULL;
                    new->right = NULL;
                    memcpy(&new->data, &p->data, sizeof(DataType));
                    break;
                } else {
                    printf("insert malloc failed\n");
                    return NULL;
                }
            }
        } else {
            memcpy(&node->data, &p->data, sizeof(DataType));
            return root;
        }
    }

    int right = getTreeDepth(root->right);
    int left = getTreeDepth(root->left);
    //print_avltree(root);
    //printf("do_left_right_rotate in\n");
    
    //rl 和 lr 是当出错的节点位置是左子树的右节点(该右节点的意思是其父节点是右节点而不是该节点是右节点)
    //或右子树的左节点(意义与上一个是一样的，只不过方向调换了)
    if (abs(right - left) > 1) {
        if (isLeft) {
            if (pnode->key > node->key) {
                //ll 左子树左节点出错
                root = avltree_ll(root);
            } else {
                //lr 左子树右节点出错
                root = avltree_lr(root);
            }
        } else {
            if (pnode->key > node->key) {
                //rl 右子树左节点出错
                root = avltree_rl(root);
            } else {
                //rr 右子树右节点出错
                root = avltree_rr(root);
            }
        }
    }

    return root;
}

static avlnode_t * find_max_key_node(avlnode_t *root) 
{
    avlnode_t *node = root;
    avlnode_t *pre = NULL;
    while (node->right) {
        pre = node;
        node = node->right;
    }
    if (pre)
        pre->right = NULL;

    return node;
}

static avlnode_t * find_min_key_node(avlnode_t *root)
{
    avlnode_t *node = root;
    avlnode_t *pre = NULL;
    while (node->left) {
        pre = node;
        node = node->left;
    }
    if (pre)
        pre->left = NULL;
    return node;
}

avlnode_t *avltree_remove(avlnode_t *root, avlnode_t *node)
{
    avlnode_t *cur = root;
    avlnode_t *pre = NULL;
    bool isLeft = false;

    while (cur) {
        if (node->key > cur->key) {
            pre = cur;
            cur = cur->right;
        } else if (node->key < cur->key) {
            isLeft = true;
            pre = cur;
            cur = cur->left;
        } else
            break;
    }

    if (cur == NULL)
        return root;

    if (cur->left && cur->right) {
        int rightdep = getTreeDepth(cur->left);
        int leftdep = getTreeDepth(cur->right);

        if (abs(rightdep - leftdep) > 1) {
            if (rightdep > leftdep) {
                avlnode_t * tmp = find_min_key_node(cur->right);
                cur->key = tmp->key;
                //memset(cur->data, 0, sizeof(cur->data));
                memcpy(cur->data, tmp->data, sizeof(cur->data));
                free(tmp);
            } else {
                avlnode_t * tmp = find_max_key_node(cur->left);
                cur->key = tmp->key;
                //memset(cur->data, 0, sizeof(cur->data));
                memcpy(cur->data, tmp->data, sizeof(cur->data));
                free(tmp);
            }
        }
    } else {
        if (cur->left) {
            if (pre) {
                if (cur == pre->left) {
                    pre->left = cur->left;
                } else {
                    pre->right = cur->left;
                }
            } else {
                root = cur;
            }
        } else {
            if (pre) {
                if (cur == pre->left) {
                    pre->left = cur->right;
                } else 
                    pre->right = cur->right;
            } else {
                root = cur;
            }
        }
    }

    int rightdep = getTreeDepth(root->left);
    int leftdep = getTreeDepth(root->right);
    if (abs(rightdep - leftdep) > 1) {
        if (isLeft) {   //删除的节点在左子树
                //ll 左子树左节点出错
                root = avltree_rr(root);
                //lr 左子树右节点出错
                root = avltree_rl(root);
              
        } else {    //删除的节点在右子树
                //rl 右子树左节点出错
                root = avltree_lr(root);
                //rr 右子树右节点出错
                root = avltree_ll(root);
        }
    }
    return root;
}

static void print_node(avlnode_t *node, int pkey, int leftorright)
{
    if (node) {
        printf("node key = %d, parent key = %d, is %s child\n", node->key, pkey, leftorright == 0?"left":"right");

        print_node(node->left, node->key, 0);
        print_node(node->right, node->key, 1);
    }
}

void print_avltree(avlnode_t *root)
{
    print_node(root, root->key, 0);
}

int main(void)
{
    avlnode_t *root;
    const int NUM = 20;

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
#if 1
    for (int i = 0; i < NUM; i++) {
        printf("%d ", numarray[i]);
    }
    printf("\n");
#endif
    for (int i = 0; i < NUM; i++) {
        node->key = numarray[i];
        root = avltree_insert(root, node);
        if (root == NULL) {
            printf("avltree_insert error\n");
            return -1;
        }
    }
    
    print_avltree(root);
    printf("left depth:%d\n", getTreeDepth(root->left));
    printf("right depth:%d\n", getTreeDepth(root->right));
#if 0
    printf("del root\n");
    root = avltree_remove(root, root);
    print_avltree(root);
#endif
}   
