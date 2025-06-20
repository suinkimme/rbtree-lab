#include "rbtree.h"

#include <stdlib.h>

/* level: 1 */
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  /*
  node_t 구조체 하나만큼의 메모리 크기를 확보하고 그 메모리의 모든 바이트를 0으로 초기화
  */
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  nil_node->color = RBTREE_BLACK; // nil 노드 색상 설정
  nil_node->left = NULL;
  nil_node->right = NULL;
  nil_node->parent = NULL;

  /*
  p->nil이란?
  - 트리 전체에서 사용할 공용 검정색 리프 노드를 설정해주는 것이다.
  - 트리의 모든 자식/리프 노드가 p->nil을 바라보도록 만들기 위한 준비

  if (node->left == p->nil) { ... }
  while (x != p->nill) { ... }

  앞으로 이런 코드가 자주 등장하게 된다. 이게 가능한 이유는 모든 노드의 자식이 NULL이 아니라
  공통된 setinel 노드, 즉 p->nil을 가리키도록 했기 때문이다.

  트리 구조를 일관되고 안전하게 유지하려면 꼭 필요함
  */
  p->nil = nil_node;

  /*
  아직 트리에 아무 노드도 삽입되지 않은 상태이기 때문에,
  루트 노드가 존재하지 않는다는 걸 나타내기 위해 root를 nil_node로 설정한다.
  */
  p->root = NULL;

  return p;
}

/* level: 2 */
/* ----------------------------------------------------------------- */

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) {
    return NULL;
  }

  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  if (t->root == t->nil) {
    new_node->key = key;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->parent = t->nil; // rbtree->nil은 sentinel 노드로, NULL을 대체해서 쓰는 값이다.
    new_node->color = RBTREE_BLACK;
    t->root = new_node;
    return new_node;
  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

/* level: 4 */
/* ----------------------------------------------------------------- */

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

/* level: 3 */
/* ----------------------------------------------------------------- */

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

/* level: 5 */
/* ----------------------------------------------------------------- */

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
