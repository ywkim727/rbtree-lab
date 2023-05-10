#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));

  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = nilNode;
  return p;
}

void left_Rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;                     //y를 설정한다
  x->right = y->left;                       //y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 옮긴다

  if(y->left != t->nil) {
    y->left->parent = x;                    //y의 왼쪽 서브트리의 부모는 x
  }

  y->parent = x->parent;                    //x의 부모를 y로 연결한다

  if(x->parent == t->nil) {
    t->root = y;                            //x가 루트 노드라면 y가 루트 노드가 된다
  }else if (x == x->parent->left) {
    x->parent->left = y;
  }else {
    x->parent->right = y;
  }
  y->left = x;                              //x를 y의 왼쪽으로 놓는다
  x->parent = y;                            //x의 부모는 y
}

void right_Rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if(y->right != t->nil) {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == t->nil) {
    t->root = y;
  }else if (x == x->parent->right) {
    x->parent->right = y;
  }else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void _delete_rbtree(rbtree *t, node_t *x) {
  if(x == t->nil || x == NULL) {
    return;
  }
  _delete_rbtree(t, x->left);                   //후위순회 방식으로 메모리를 반환
  _delete_rbtree(t, x->right);
  free(x);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *temp = t->root;
  _delete_rbtree(t, temp);
  free(t->nil);
  free(t);
}

void rb_insert_fixup(rbtree *t, node_t *z) {  
  while(z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;        //case1 z의 삼촌 y가 적색인 경우
        y->color = RBTREE_BLACK;                //case1
        z->parent->parent->color = RBTREE_RED;  //case1
        z = z->parent->parent;                  //case1
      }else{
        if(z == z->parent->right){
          z = z->parent;                        //case2 z의 삼촌 y가 흑색이며 z가 오른쪽 자식인 경우
          left_Rotate(t, z);                    //case2
        }
        z->parent->color = RBTREE_BLACK;        //case3 z의 삼촌 y가 흑색이며 z가 왼쪽 자식인 경우
        z->parent->parent->color = RBTREE_RED;  //case3
        right_Rotate(t, z->parent->parent);     //case3   
      }
    }else{                                      //case4~6 (left <-> right)
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->left){
          z = z->parent;
          right_Rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_Rotate(t, z->parent->parent);      
      }
    }
  }
  t->root->color = RBTREE_BLACK;                //루트노드는 검정색이어야 한다
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t*)calloc(1,sizeof(node_t));
  z->key = key;
  while(x != t->nil) {                        //x가 nil이 될 때 까지 반복, nil은 key가 들어갈 노드z의 위치를 뜻한다
    y = x;                                    //x의 부모 포인터 y를 기록하며 내려간다
    if(z->key < x->key) {
      x = x->left;
    }else {
      x = x->right;
    }
  }
  z->parent = y;                               //z가 들어갈 nil의 위치를 찾았을때는 수정해야 하는 노드를 한단계 지났으므로 부모 포인터 y가 필요
  if(y == t->nil) {
    t->root = z;                               //트리 t는 빈 트리
  }else if(z->key < y->key) {
    y->left = z;
  }else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;                       //추가되는 노드는 적색이다
  rb_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;

  while(current != t->nil){                     //BST의 find와 같은 원리로 작동
    if(current->key > key){
      current = current->left;
    }else if(current->key < key){
      current = current->right;
    }else if(current->key == key){
      return current;
    }
  }
  return NULL;
}

node_t *_rbtree_min(const rbtree *t, node_t *x){
  node_t *temp = x;
  while(temp->left != t->nil) {                 //트리의 최소 노드는 왼쪽 끝
    temp = temp->left;
  }
  return temp;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return _rbtree_min(t, t->root);
}

node_t *_rbtree_max(const rbtree *t, node_t *x){
  node_t *temp = x;
  while(temp->right != t->nil) {                  //트리의 최대 노드는 오른쪽 끝
    temp = temp->right;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return _rbtree_max(t, t->root);
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil) {                        //두 노드의 위치를 바꾸는 작업
    t->root = v;
  }else if(u == u->parent->left) {
    u->parent->left = v;
  }else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rb_erase_fixup(rbtree *t, node_t *x){
  node_t *w;
  while(x != t->root && x->color == RBTREE_BLACK) {
    if(x == x->parent->left) {
      w = x->parent->right;                         //w는 x의 삼촌노드
      if(w->color == RBTREE_RED) {                  //case 1 w가 red인 경우
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_Rotate(t, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                      //case 2 w가 black, w의 두 자식 모두 black인 경우
        x = x->parent;
      }
      else {
        if(w->right->color == RBTREE_BLACK){        //case 3 w가 black, w의 왼쪽 자식은 red, w의 오른쪽 자식은 black인 경우
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_Rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;                //case 4 w는 black, w의 오른쪽 자식은 red인 경우
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_Rotate(t, x->parent);
        x = t->root;       
      }
    }else {                                         //case 5~8 (left <-> right)
      w = x->parent->left;
      if(w->color == RBTREE_RED) {                  
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_Rotate(t, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                      
        x = x->parent;
      }
      else {
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_Rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_Rotate(t, x->parent);
        x = t->root;  
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_original_color;

  y = z;
  y_original_color = y->color;               //y의 색깔이 변경 될 수 있으므로 변경 전 y의 색을 저장한다

  if(z->left == t->nil){                     //삭제하는 노드의 자식이 1개 이하인 경우
    x = z->right;
    rb_transplant(t,z,z->right);
  }else if(z->right == t->nil){
    x = z->left;
    rb_transplant(t,z,z->left);
  }else{                                     //삭제하는 노드의 자식이 2개인 경우
    y = _rbtree_min(t, z->right);            //succesor를 찾는다(오른쪽 서브트리에서 가장 작은 노드)
    y_original_color = y->color;
    x = y->right;                            //x가 y의 원래 위치로 이동하기 때문에 x.p는 항상 y의 부모의 원래 위치를 가리키게 된다
    if(y->parent == z){                      //하지만 y의 원래 부모가 z면 x.p가 y의 부모를 가리키면 안된다, 부모 z가 삭제되는 노드이기 때문 
      x->parent = y;                         //x.p를 y로 설정함으로써 x.p가 y의 부모의 원래 위치를 가리키게 된다
    }else{
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t,z,y);                    //삭제하는 z의 위치에 y를 대체한다
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if(y_original_color == RBTREE_BLACK){      //삭제되는 노드가 적색이면 문제가 없지만, 흑색이면 문제가 발생하여 fixup이 필요하다
    rb_erase_fixup(t,x);
  }
  free(z);
  return 0;
}

void _rbtree_array(const rbtree *t, key_t *arr, const size_t n, node_t *x, int *count) {
  if(x == t->nil) {
    return;
  }
  _rbtree_array(t, arr, n, x->left, count);         //inorder 방식으로 array로 변환
  if(*count < n){
    arr[*count] = x->key;
    *count = *count + 1 ;
  }
  _rbtree_array(t, arr, n, x->right, count);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int count = 0;
  node_t *temp = t->root;
  _rbtree_array(t, arr, n, temp, &count);
  return 0;
}
