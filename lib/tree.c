#include "tree.h"
#include "stdlib.h"
#include "stdio.h"
#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

struct tree_node {
  int value;
  struct tree_node *left;
  struct tree_node *right;
};

// Création d'un arbre
void tree_create(struct tree *self)
{
  self->root = NULL;
}

// Suppression recursive d'un arbre
void tree_destroy_rec(struct tree_node *node)
{
  if(node != NULL)
  {
    tree_destroy_rec(node->left);
    tree_destroy_rec(node->right);
    free(node);
  }
}

// initialisation de la suppression de l'arbre
void tree_destroy(struct tree *self) 
{
  if(!tree_is_empty(self))
  {
    tree_destroy_rec(self->root);
  }
}

// renvoie true si l'abre contient la valeur value
bool tree_contains_rec(const struct tree_node *node, int value)
{
  if(node != NULL)
  {
    if(node->value == value)
    {
      return true;
    }
    return tree_contains_rec(node->left, value) || tree_contains_rec(node->right, value);
  }
  return false;
}

// Initialise la fonction recursive ci-dessus
bool tree_contains(const struct tree *self, int value) 
{
  if(!tree_is_empty(self))
  {
    return tree_contains_rec(self->root, value);
  }
  return false;
}

// revoie un nouvel arbre avec le nouvel élément inséré.
struct tree_node *tree_insert_rec(struct tree_node *self, int data) 
{
  if (self == NULL) 
  {
    struct tree_node *tree = malloc(sizeof(struct tree_node));
    tree->left = tree->right = NULL; 
    tree->value = data;
    return tree;
  }
  if (data < self->value) 
  {
    self->left = tree_insert_rec(self->left, data);
    return self;
  }
  if (data > self->value) 
  {
    self->right = tree_insert_rec(self->right, data);
    return self;
  }
  return self;
}

// Initialise la fonction recursive ci-dessus
void tree_insert(struct tree *self, int value) 
{
  self->root = tree_insert_rec(self->root, value);
}

// Supprime le minimum d'un arbre
struct tree_node *tree_delete_minimum(struct tree_node *self, struct tree_node **min) 
{
  if (self->left == NULL) 
  {
    struct tree_node *right = self->right;
    self->right = NULL;
    *min = self;
    return right;
  }
  self->left = tree_delete_minimum(self->left, min);
  return self;
}


struct tree_node *tree_delete(struct tree_node *self)
{
  struct tree_node *left = self->left;
  struct tree_node *right = self->right;
  free(self); 
  self = NULL;
  if (left == NULL && right == NULL) 
  {
    return NULL;
  }
  if (left == NULL) 
  {
    return right;
  }
  if (right == NULL) 
  {
    return left;
  }
  right = tree_delete_minimum(right, &self);
  self->left = left; 
  self->right = right;
  return self;
}

struct tree_node *tree_remove_rec(struct tree_node *self, int data) 
{
  if (self == NULL) 
  {
    return NULL;
  }
  if (data < self->value) 
  {
    self->left = tree_remove_rec(self->left, data);
    return self;
  }
  if (data > self->value) 
  {
    self->right = tree_remove_rec(self->right, data);
    return self;
  }
  return tree_delete(self);
}

void tree_remove(struct tree *self, int value)
{
  self->root = tree_remove_rec(self->root, value);
}

// Renvoie true si l'arbre est vide
bool tree_is_empty(const struct tree *self) 
{
  if(self->root == NULL)
  {
    return true;
  }
  return false;
}

// Renvoie la taille d'un arbre (le nombre de noeuds)
size_t tree_size_rec(const struct tree_node *node)
{
  if(node != NULL)
  {
    return 1 + tree_size_rec(node->left) + tree_size_rec(node->right);
  }
  return 0;
}

// Initialise la fonction recursive ci-dessus
size_t tree_size(const struct tree *self) 
{
  if(!tree_is_empty(self))
  {
    return tree_size_rec(self->root);
  }
  return 0;
}

//Renvoie la hauteur d'un arbre en comparant la taille du fils droit et du fils gauche
size_t tree_height_rec(const struct tree_node *node)
{
  if(node != NULL)
  {
    return max(1 + tree_size_rec(node->left), 1 + tree_size_rec(node->right));
  }
  return 0;
}

// Initialise la fonction recursive ci-dessus
size_t tree_height(const struct tree *self)
{
  if(!tree_is_empty(self))
  {
    return tree_height_rec(self->root);
  }
  return 0;
}

void tree_walk_pre_order_rec(const struct tree_node *self, tree_func_t func, void *user_data)  
{
  if(self == NULL)
  {
    return;
  }
  func(self->value, user_data);
  tree_walk_pre_order_rec(self->left, func, user_data);
  tree_walk_pre_order_rec(self->right, func, user_data);
}

// Parcoure prefixe d'un arbre
void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data)  
{
  tree_walk_pre_order_rec(self->root, func, user_data);
}

void tree_walk_in_order_rec(const struct tree_node *self, tree_func_t func, void *user_data)  
{
  if(self == NULL)
  {
    return;
  }
  tree_walk_in_order_rec(self->left, func, user_data);
  func(self->value, user_data);
  tree_walk_in_order_rec(self->right, func, user_data);
}

// Parcoure infixe d'un arbre
void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) 
{
  tree_walk_in_order_rec(self->root, func, user_data);
}

void tree_walk_post_order_rec(const struct tree_node *self, tree_func_t func, void *user_data)  
{
  if(self == NULL)
  {
    return;
  }
  tree_walk_post_order_rec(self->left, func, user_data);
  tree_walk_post_order_rec(self->right, func, user_data);
  func(self->value, user_data);
}

// Parcoure postfixe d'un arbre
void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) 
{
  tree_walk_post_order_rec(self->root, func, user_data);
}

// Affiche les élément un à un en effectuant un parcoure infixe.
void tree_dump_rec(const struct tree_node *self)
{
  if(self == NULL)
  {
    return;
  }
  tree_dump_rec(self->left);
  printf("%i\n", self->value);
  tree_dump_rec(self->right);
}

// Initialise la fonction recursive ci-dessus
void tree_dump(const struct tree *self)
{
  if(self != NULL)
  {
    tree_dump_rec(self->root);
  }
}