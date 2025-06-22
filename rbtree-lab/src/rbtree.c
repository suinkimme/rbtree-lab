#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

/* level: 1 */
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  /*
  node_t 구조체 하나만큼의 메모리 크기를 확보하고 그 메모리의 모든 바이트를 0으로 초기화
  */
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  nil_node->color = RBTREE_BLACK; // nil 노드 색상 설정
  nil_node->left = nil_node;
  nil_node->right = nil_node;
  nil_node->parent = nil_node;

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
  루트 노드가 존재하지 않는다는 걸 나타내기 위해 root를 nil node로 설정한다.
  */
  p->root = nil_node;

  return p;
}

/* level: 2 */
/* ----------------------------------------------------------------- */

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rotate_right(rbtree *t, node_t *p) {
  node_t *l = p->left;
  p->left = l->right;
  if (l->right != t->nil) {
    l->right->parent = p;
  }
  l->parent = p->parent;

  if (p->parent == t->nil) {
    t->root = l;
  } else if (p == p->parent->right) {
    p->parent->right = l;
  } else {
    p->parent->left = l;
  }

  l->right = p;
  p->parent = l;
}

void rotate_left(rbtree *t, node_t *p) {
  node_t *l = p->right;
  p->right = l->left;
  // 추가된 노드의 왼쪽 자식이 회전 후에 p의 오른쪽 자식이 되는 구조 (낙동강 오리알)
  if (l->left != t->nil) {
    l->left->parent = p;
  }
  // 추가된 노드의 부모를 원래 부모의 부모(할아버지)로 바꾼다.
  // 중간에 올라오는 노드가 기존의 위치를 대체해야 하므로, 할아버지에서 이 노드를 가리키도록 연결함
  l->parent = p->parent;

  // 루트 부모는 항상 t->nil
  // 루트가 바뀌는 경우는 회전이 루트에서 발생했을 때 인데
  // p가 루트였다면 아래 조건이 참이되고 그 경우 새로 추가된 노드를 루트로 지정함
  if (p->parent == t->nil) {
    t->root = l;
  // 회전의 기준이 된 노드 p가 그 부모의 왼쪽 자식이라면, 회전 후 l을 그 부모의 왼쪽 자식으로 지정한다. 반대의 경우는 오른쪽으로 설정하면 된다.
  } else if (p == p->parent->left) {
    p->parent->left = l;
  } else {
    p->parent->right = l;
  }

  l->left = p;
  p->parent = l;
}

void rbtree_insert_fixup(rbtree *t, node_t *p) {
  while (p->parent->color == RBTREE_RED) {
    // 왼쪽 서브트리
    if (p->parent == p->parent->parent->left) {
      node_t *uncle = p->parent->parent->right;

      if (uncle->color == RBTREE_RED) {
        p->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      } else {
        // 꺽인 상태인지 확인
        // 할아버지와 부모가 왼쪽으로 일직선이니까 자신이 부모의 오른쪽에 위치해 있으면 꺽인거라 할 수 있음
        if (p == p->parent->right) {
          p = p->parent;
          rotate_left(t, p);
        }

        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        rotate_right(t, p->parent->parent);
      }

    // 오른쪽 서브트리
    } else {
      node_t *uncle = p->parent->parent->left;

      if (uncle->color == RBTREE_RED) {
        p->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      } else {
        if (p == p->parent->left) {
          p = p->parent;
          rotate_right(t, p);
        }

        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        rotate_left(t, p->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) {
    return NULL;
  }

  if (rbtree_find(t, key) != NULL) {
    return NULL;
  }

  // 새로 추가할 노드의 메모리 공간을 확보함
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;

  // 루트가 없을 경우에 생성한 노드를 루트로 지정함
  // 루트로 사용하기 위해
  if (t->root == t->nil) {
    new_node->parent = t->nil;
    new_node->color = RBTREE_BLACK;
    t->root = new_node;
    return new_node;
  }

  node_t *current = t->root;
  node_t *parent = t->nil;

  // 이진 탐색 그자체 어떤 부모 밑에 추가 되어야할지 정하는거임
  while (current != t->nil) {
    parent = current;

    if (key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  new_node->parent = parent;
  new_node->color = RBTREE_RED;

  if (key < parent->key) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }

  rbtree_insert_fixup(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t == NULL) {
    return NULL;
  }

  node_t *current = t->root;

  while(current != t->nil) {
    if (key < current->key) {
      current = current->left;
    } else if (key > current->key) {
      current = current->right;
    } else {
      return current;
    }
  }

  return NULL;
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

void rbtree_erase_fixup(rbtree *t, node_t *p) {
  while (p != t->root && p->color == RBTREE_BLACK) {
    if (p == p->parent->left) {
      node_t *brother = p->parent->right;

      if (brother->color == RBTREE_RED) {
        brother->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rotate_left(t, p->parent);
        brother = p->parent->right;
      }

      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK) {
        brother->color = RBTREE_RED;
        p = p->parent;
      } else {
        if (brother->color == RBTREE_BLACK && brother->left->color == RBTREE_RED) {
          brother->color = RBTREE_RED;
          brother->left->color = RBTREE_BLACK;
          rotate_right(t, brother);
          brother = p->parent->right;
        }

        brother->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        rotate_left(t, p->parent);
        p = t->root;
      }
    } else {
      node_t *brother = p->parent->left;

      if (brother->color == RBTREE_RED) {
        brother->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rotate_right(t, p->parent);
        brother = p->parent->left;
      }

      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK) {
        brother->color = RBTREE_RED;
        p = p->parent;
      } else {
        if (brother->color == RBTREE_BLACK && brother->right->color == RBTREE_RED) {
          brother->color = RBTREE_RED;
          brother->right->color = RBTREE_BLACK;
          rotate_left(t, brother);
          brother = p->parent->left;
        }

        brother->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        rotate_right(t, p->parent);
        p = t->root;
      }
    }
  }

  p->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if (t == NULL || p == NULL || p == t->nil) {
    return 0;
  }

  if (p->left == t->nil && p->right == t->nil) {
    color_t deleted_color = p->color;
    node_t *fix_node = t->nil;

    if (p == p->parent->left) {
      p->parent->left = t->nil;
    } else if (p == p->parent->right) {
      p->parent->right = t->nil;
    } else {
      t->root = t->nil;
    }

    free(p);

    if (deleted_color == RBTREE_BLACK) {
      rbtree_erase_fixup(t, fix_node);
    }

    return 1;
  }

  if (p->left == t->nil || p->right == t->nil) {
    node_t *child = (p->left != t->nil) ? p->left : p->right;
    color_t deleted_color = p->color;
    node_t *fix_node = child;

    if (p == t->root) {
      t->root = child;
    } else if (p == p->parent->left) {
      p->parent->left = child;
    } else {
      p->parent->right = child;
    }

    child->parent = p->parent;

    free(p);

    if (deleted_color == RBTREE_BLACK) {
      rbtree_erase_fixup(t, fix_node);
    }

    return 1;
  }

  node_t *successor = p->right;
  while (successor->left != t->nil) {
    successor = successor->left;
  }

  p->key = successor->key;
  return rbtree_erase(t, successor);
}

/* level: 5 */
/* ----------------------------------------------------------------- */

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
