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
    DataType *data;
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
    avlnode_t *rchild = root->right;
    if (rchild == NULL) {
        return root;
    }

    root->right = rchild->left;
    rchild->left = root;
    return rchild;
}

static avlnode_t * create_new_node(void)
{
    avlnode_t * new = (avlnode_t *)malloc(sizeof(avlnode_t));
    if (new) {
        new->data = (DataType *)malloc(sizeof(DataType));
        if (!new->data) {
            free(new);
            goto FAILED;
        } else {
            return new;
        }
    }
   
FAILED:
    printf("create_new_node failed\n");
    return NULL;
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
    avlnode_t * lchild = root->left;
    if (lchild == NULL) {
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
     case 1                   case 2
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
    if (root == NULL) {
        //root = (avlnode_t *)malloc(sizeof(avlnode_t));
        root = create_new_node();
        if (root == NULL) {
            printf("avltree_insert malloc failed\n");
            return NULL;
        }
        root->key = p->key;
        memcpy(root->data, p->data, sizeof(DataType));
        root->left = NULL;
        root->right = NULL;
    } else {
        int leftdep, rightdep;
        if (p->key < root->key) {
            root->left = avltree_insert(root->left, p);
            leftdep = getTreeDepth(root->left);
            rightdep = getTreeDepth(root->right);
            if (abs(leftdep - rightdep) > 1) {
                if (p->key < root->left->key)
                    root = avltree_ll(root);
                else
                    root = avltree_lr(root);
            }
        } else if (p->key > root->key) {
            root->right = avltree_insert(root->right, p);
            leftdep = getTreeDepth(root->left);
            rightdep = getTreeDepth(root->right);
            if (abs(leftdep - rightdep) > 1) {
                if (p->key > root->right->key)
                    root = avltree_rr(root);
                else
                    root = avltree_rl(root);
            }
        } else {
            //p->key == root->key
            memcpy(root->data, p->data, sizeof(DataType));
        }
    }

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
{   //获取最小值节点
    avlnode_t *node = root;
    while (node->left) {
        node = node->left;
    }
    return node;
}

avlnode_t *avltree_remove(avlnode_t *root, avlnode_t *p)
{
    if (root == NULL) {
        printf("avltree_remove root=null\n");
        return NULL;
    }

    if (p->key < root->key) {
        root->left = avltree_remove(root->left, p);
    } else if (p->key > root->key) {
        root->right = avltree_remove(root->right, p);
    } else {
        if (root->left && root->right) {
            //获取右子树中最小值的节点
            int left, right;
            avlnode_t *tmp = find_min_key_node(root->right);
            printf("find right min node:key=%d\n", tmp->key);
            //将替换节点的内容转移到被删除的节点上
            root->key = tmp->key;
            memcpy(root->data, tmp->data, sizeof(DataType));
            root->right = avltree_remove(root->right, tmp);
            left = getTreeDepth(root->left);
            right = getTreeDepth(root->right);
            if (abs(left - right) > 1) {
                if (root->left->right != NULL) {
                    int lleft = getTreeDepth(root->left->left);
                    int lright = getTreeDepth(root->left->right);
                    if (lright > lleft) {
                        root = avltree_lr(root);
                    }
                } else {
                    root = avltree_ll(root);
                }
            }
        } else {
            avlnode_t *tmp = root;
            if (root->left) {
                //用左节点替代它自己
                root = root->left;
            } else if (root->right) {
                root = root->right; 
            } else {
                root = NULL;
            }
            //这里释放avlnode结构体，其中的数据指针
            free(tmp->data);
            free(tmp);
        }       
    }

    return root;
}

avlnode_t * find_avlnode(avlnode_t *root, int key) 
{
    if (root == NULL)
        return NULL;
    if (key < root->key)
        return find_avlnode(root->left, key);
    else if (key > root->key)
        return find_avlnode(root->right, key);
    else
        return root;
}

static void print_node(avlnode_t *node, int pkey, int leftorright)
{
    if (node) {
        printf("node[%d] parent[%d][%s]:age=%d,name=%s\n", node->key, pkey, leftorright == 0?"left":"right", node->data->age, node->data->name);

        print_node(node->left, node->key, 0);
        print_node(node->right, node->key, 1);
    }
}

void print_avltree(avlnode_t *root)
{
    print_node(root, root->key, 0);
}

int main(int argc, char *argv[])
{
    avlnode_t *root = NULL;
    int NUM = 10;

    if (argc < 2) {
        printf("./a.out num\n");
        return -1;
    }

    //NUM = atoi(argv[1]);
    printf("NUM=%d\n", NUM);

    avlnode_t *node = (avlnode_t *)malloc(sizeof(avlnode_t));
    if (node == NULL) {
        printf("malloc error\n");
        return -1;
    }
    node->data = (DataType *)malloc(sizeof(DataType));
    if (node->data == NULL) {
        printf("malloc error\n");
        free(node);
        return -1;
    }

    int numarray[NUM];
    for (int i = 0; i < NUM; i++)
        numarray[i] = i;
   
    DataType student[] = {
        { .age = 10, .name = "litianyi"},
        { .age = 9, .name = "wangminger"},
        { .age = 7, .name = "zhaosansan"},
        { .age = 12, .name = "qianxisi"},
        { .age = 11, .name = "chenwu"},
        { .age = 13, .name = "zhangliu"},
        { .age = 14, .name = "huoqiqi"},
        { .age = 15, .name = "zhugebaba"},
        { .age = 16, .name = "murongjiujiu"},
        { .age = 19, .name = "xiaoshi"},
    };

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
        node->data->age = student[i].age;
        memcpy(node->data->name, student[i].name, sizeof(node->data->name));
        root = avltree_insert(root, node);
        if (root == NULL) {
            printf("avltree_insert error\n");
            return -1;
        }
    }
    
    print_avltree(root);
    printf("left depth:%d\n", getTreeDepth(root->left));
    printf("right depth:%d\n", getTreeDepth(root->right));
#if 1
    printf("del root, root->key=%d\n", root->key);
    root = avltree_remove(root, root);
    //print_avltree(root);

    node->key = 3;
    root = avltree_remove(root, node);
END:
    printf("remove end\n");
    print_avltree(root);
#endif
}   
