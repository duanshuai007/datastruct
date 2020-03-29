#红黑树

本文完全参考: `https://www.cnblogs.com/skywang12345/p/3245399.html`,感谢原作者。

##一 简介
红黑树(red-Black Tree)是一种特殊的二叉树，红黑树上每个节点都有存储位表示节点的颜色，可以是红(Red)或黑(Black).

特性：  

- 1.每个节点只能是红色或者黑色。
- 2.根节点必须是黑色。
- 3.每个叶子节点(NIL)是黑色。
- 4.如果一个节点是红色，则它的子节点必须是黑色。
- 5.从一个节点到该节点的子孙节点所有路径上包含相同数目的黑节点。

特性5确保没有一条路径会比其他路径长出两倍，因为红黑树是相对的接近平衡的二叉树。

##二 红黑树的基本操作

###1.左旋和右旋

红黑树的基本操作是添加和删除，在对红黑树进行了添加或删除之后，都会用到旋转方法。因为在添加新的节点之后，红黑树就发生了变化，可能满足不了那五条特性，所以就需要通过旋转使红黑树继续保持平衡。

1）左旋    

条件:当前节点的父节点是红色 && 当前节点的叔节点是黑色或不存在 && 当前节点是其父节点的右子节点。
  
步骤:将父节点左旋，将指针指向父节点
  
![](https://raw.githubusercontent.com/duanshuai007/hodgepodge/master/pictures/document/Linux/zuoxuan.jpg) 

2）右旋  

条件:当前节点的父节点是红色 && 当前节点的叔节点是黑色或不存在 && 当前节点是其父节点的左子节点。
  
步骤:将父节点变为黑色，将祖父节点变为红色，将祖父节点右旋，将指针指向祖父节点

![](https://raw.githubusercontent.com/duanshuai007/hodgepodge/master/pictures/document/Linux/youxuan.jpg)

观察两张图片，能够发现，这两张图是一个正一个反的过程。  
对x节点进行左旋，意味着，将x的右孩子设为x的父节点，x变成了x右孩子的一个左节点。  
对y节点进行右旋，意味着，将y的左孩子设为y的父节点，y变成了y左孩子的一个右节点。  


3）变色  

条件:当前节点的父节点是红色并且当前节点的叔叔节点也是红色

步骤:
> 当前节点是根节点则直接变为黑色  
> 当前节点不是根节点  
>> 将父节点与叔节点变为黑色  
>> 将祖父节点变为红色  
>> 将指针指向祖父节点  
		
###2.添加   
第一步: 将红黑树当作一颗二叉查找树，将节点插入。    
红黑树本身就是一颗二叉查找树，将节点插入后，该树仍然是一颗二叉查找树。也就意味着，树的键值仍然是有序的。此外，无论是左旋还是右旋，若旋转之前这棵树是二叉查找树，旋转之后它一定还是二叉查找树。这也就意味着，任何的旋转和重新着色操作，都不会改变它仍然是一颗二叉查找树的事实。    
       
第二步：将插入的节点着色为"红色"。  
将插入的节点着色为红色，不会违背"特性(5)"！少违背一条特性，就意味着我们需要处理的情况越少。接下来，就要努力的让这棵树满足其它性质即可；满足了的话，它就又是一颗红黑树了。  

第三步: 通过一系列的旋转或着色等操作，使之重新成为一颗红黑树。  

添加操作的伪代码《算法导论》  

```
RB-INSERT(T, z)  
 y ← nil[T]                        // 新建节点“y”，将y设为空节点。
 x ← root[T]                       // 设“红黑树T”的根节点为“x”
 while x ≠ nil[T]                  // 找出要插入的节点“z”在二叉树T中的位置“y”
     do y ← x                      
        if key[z] < key[x]  
           then x ← left[x]  
           else x ← right[x]  
 p[z] ← y                          // 设置 “z的父亲” 为 “y”
 if y = nil[T]                     
    then root[T] ← z               // 情况1：若y是空节点，则将z设为根
    else if key[z] < key[y]        
            then left[y] ← z       // 情况2：若“z所包含的值” < “y所包含的值”，则将z设为“y的左孩子”
            else right[y] ← z      // 情况3：(“z所包含的值” >= “y所包含的值”)将z设为“y的右孩子” 
 left[z] ← nil[T]                  // z的左孩子设为空
 right[z] ← nil[T]                 // z的右孩子设为空。至此，已经完成将“节点z插入到二叉树”中了。
 color[z] ← RED                    // 将z着色为“红色”
 RB-INSERT-FIXUP(T, z)             // 通过RB-INSERT-FIXUP对红黑树的节点进行颜色修改以及旋转，让树T仍然是一颗红黑树
```

结合伪代码以及为代码上面的说明，先理解RB-INSERT。理解了RB-INSERT之后，我们接着对 RB-INSERT-FIXUP的伪代码进行说明。

添加修正操作的伪代码《算法导论》

```
RB-INSERT-FIXUP(T, z)
while color[p[z]] = RED                                                  // 若“当前节点(z)的父节点是红色”，则进行以下处理。
    do if p[z] = left[p[p[z]]]                                           // 若“z的父节点”是“z的祖父节点的左孩子”，则进行以下处理。
          then y ← right[p[p[z]]]                                        // 将y设置为“z的叔叔节点(z的祖父节点的右孩子)”
               if color[y] = RED                                         // Case 1条件：叔叔是红色
                  then color[p[z]] ← BLACK                    ▹ Case 1   //  (01) 将“父节点”设为黑色。
                       color[y] ← BLACK                       ▹ Case 1   //  (02) 将“叔叔节点”设为黑色。
                       color[p[p[z]]] ← RED                   ▹ Case 1   //  (03) 将“祖父节点”设为“红色”。
                       z ← p[p[z]]                            ▹ Case 1   //  (04) 将“祖父节点”设为“当前节点”(红色节点)
                  else if z = right[p[z]]                                // Case 2条件：叔叔是黑色，且当前节点是右孩子
                          then z ← p[z]                       ▹ Case 2   //  (01) 将“父节点”作为“新的当前节点”。
                               LEFT-ROTATE(T, z)              ▹ Case 2   //  (02) 以“新的当前节点”为支点进行左旋。
                          color[p[z]] ← BLACK                 ▹ Case 3   // Case 3条件：叔叔是黑色，且当前节点是左孩子。(01) 将“父节点”设为“黑色”。
                          color[p[p[z]]] ← RED                ▹ Case 3   //  (02) 将“祖父节点”设为“红色”。
                          RIGHT-ROTATE(T, p[p[z]])            ▹ Case 3   //  (03) 以“祖父节点”为支点进行右旋。
       else (same as then clause with "right" and "left" exchanged)      // 若“z的父节点”是“z的祖父节点的右孩子”，将上面的操作中“right”和“left”交换位置，然后依次执行。
color[root[T]] ← BLACK
```

###3.删除

第一步：将红黑树当作一颗二叉查找树，将节点删除。  
这和"删除常规二叉查找树中删除节点的方法是一样的"。分3种情况：  
       ① 被删除节点没有儿子，即为叶节点。那么，直接将该节点删除就OK了。  
       ② 被删除节点只有一个儿子。那么，直接删除该节点，并用该节点的唯一子节点顶替它的位置。  
       ③ 被删除节点有两个儿子。那么，先找出它的后继节点；然后把“它的后继节点的内容”复制给“该节点的内容”；之后，删除“它的后继节点”。在这里，后继节点相当于替身，在将后继节点的内容复制给"被删除节点"之后，再将后继节点删除。这样就巧妙的将问题转换为"删除后继节点"的情况了，下面就考虑后继节点。 在"被删除节点"有两个非空子节点的情况下，它的后继节点不可能是双子非空。既然"的后继节点"不可能双子都非空，就意味着"该节点的后继节点"要么没有儿子，要么只有一个儿子。若没有儿子，则按"情况① "进行处理；若只有一个儿子，则按"情况② "进行处理。  
       
第二步：通过"旋转和重新着色"等一系列来修正该树，使之重新成为一棵红黑树。  


删除操作的伪代码《算法导论》

```
RB-DELETE(T, z)
if left[z] = nil[T] or right[z] = nil[T]         
   then y ← z                                  // 若“z的左孩子” 或 “z的右孩子”为空，则将“z”赋值给 “y”；
   else y ← TREE-SUCCESSOR(z)                  // 否则，将“z的后继节点”赋值给 “y”。
if left[y] ≠ nil[T]
   then x ← left[y]                            // 若“y的左孩子” 不为空，则将“y的左孩子” 赋值给 “x”；
   else x ← right[y]                           // 否则，“y的右孩子” 赋值给 “x”。
p[x] ← p[y]                                    // 将“y的父节点” 设置为 “x的父节点”
if p[y] = nil[T]                               
   then root[T] ← x                            // 情况1：若“y的父节点” 为空，则设置“x” 为 “根节点”。
   else if y = left[p[y]]                    
           then left[p[y]] ← x                 // 情况2：若“y是它父节点的左孩子”，则设置“x” 为 “y的父节点的左孩子”
           else right[p[y]] ← x                // 情况3：若“y是它父节点的右孩子”，则设置“x” 为 “y的父节点的右孩子”
if y ≠ z                                    
   then key[z] ← key[y]                        // 若“y的值” 赋值给 “z”。注意：这里只拷贝z的值给y，而没有拷贝z的颜色！！！
        copy y's satellite data into z         
if color[y] = BLACK                            
   then RB-DELETE-FIXUP(T, x)                  // 若“y为黑节点”，则调用
return y
```

结合伪代码以及为代码上面的说明，先理解RB-DELETE。理解了RB-DELETE之后，接着对 RB-DELETE-FIXUP的伪代码进行说明

```
RB-DELETE-FIXUP(T, x)
while x ≠ root[T] and color[x] = BLACK  
    do if x = left[p[x]]      
          then w ← right[p[x]]                                             // 若 “x”是“它父节点的左孩子”，则设置 “w”为“x的叔叔”(即x为它父节点的右孩子)                                          
               if color[w] = RED                                           // Case 1: x是“黑+黑”节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。
                  then color[w] ← BLACK                        ▹  Case 1   //   (01) 将x的兄弟节点设为“黑色”。
                       color[p[x]] ← RED                       ▹  Case 1   //   (02) 将x的父节点设为“红色”。
                       LEFT-ROTATE(T, p[x])                    ▹  Case 1   //   (03) 对x的父节点进行左旋。
                       w ← right[p[x]]                         ▹  Case 1   //   (04) 左旋后，重新设置x的兄弟节点。
               if color[left[w]] = BLACK and color[right[w]] = BLACK       // Case 2: x是“黑+黑”节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。
                  then color[w] ← RED                          ▹  Case 2   //   (01) 将x的兄弟节点设为“红色”。
                       x ←  p[x]                               ▹  Case 2   //   (02) 设置“x的父节点”为“新的x节点”。
                  else if color[right[w]] = BLACK                          // Case 3: x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。
                          then color[left[w]] ← BLACK          ▹  Case 3   //   (01) 将x兄弟节点的左孩子设为“黑色”。
                               color[w] ← RED                  ▹  Case 3   //   (02) 将x兄弟节点设为“红色”。
                               RIGHT-ROTATE(T, w)              ▹  Case 3   //   (03) 对x的兄弟节点进行右旋。
                               w ← right[p[x]]                 ▹  Case 3   //   (04) 右旋后，重新设置x的兄弟节点。
                        color[w] ← color[p[x]]                 ▹  Case 4   // Case 4: x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的。(01) 将x父节点颜色 赋值给 x的兄弟节点。
                        color[p[x]] ← BLACK                    ▹  Case 4   //   (02) 将x父节点设为“黑色”。
                        color[right[w]] ← BLACK                ▹  Case 4   //   (03) 将x兄弟节点的右子节设为“黑色”。
                        LEFT-ROTATE(T, p[x])                   ▹  Case 4   //   (04) 对x的父节点进行左旋。
                        x ← root[T]                            ▹  Case 4   //   (05) 设置“x”为“根节点”。
       else (same as then clause with "right" and "left" exchanged)        // 若 “x”是“它父节点的右孩子”，将上面的操作中“right”和“left”交换位置，然后依次执行。
color[x] ← BLACK
```
    