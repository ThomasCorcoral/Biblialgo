#include "list.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

struct list_node {
  int value;
  struct list_node *next;
  // struct list_node *prev;
};

// Création d'une liste
void list_create(struct list *self)
{
	self->first = NULL;
}

// Suppression d'une liste
void list_destroy(struct list *self) 
{
	if(!list_is_empty(self))
	{
		struct list_node *curr = self->first;
		struct list_node *next;
		while(curr != NULL)
		{
			next = curr->next;
			free(curr);
			curr = next;
		}
	}
}

// Renvoie true si les deux listes sont égales
bool list_equals(const struct list *self, const int *data, size_t size) 
{
	if(!list_is_empty(self))
	{
		size_t size_liste = list_size(self);
		if(size_liste != size)
		{
			return false;
		}
		else
		{
			struct list_node *curr = self->first;
			int i = 0;
			while(i != size)
			{
				if(curr->value == data[i])
				{
					i++;
					curr = curr->next;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
	}
	else
	{
		if(size == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// Ajoute un élément en fin de liste. Il faut donc créer ce nouvel élément, parcourir la liste jusqu'à la fin et "relier" le nouvel élément
void list_add_back(struct list *self, int value)
{
	struct list_node *add_back = calloc(1,sizeof(struct list_node));
	add_back->value = value;
	if(list_is_empty(self))
	{
		self->first = add_back;
	}
	else
	{
		struct list_node *curr = self->first;
		struct list_node *prev;
		while(curr != NULL)
		{
			prev = curr;
			curr = curr->next;
		}
		prev->next = add_back;
	}
}

// Ajoute un élément au début d'une liste, il faut modifier le pointeur first du self et relier le nouvel élément à l'ancien premier élément
void list_add_front(struct list *self, int value) 
{
	struct list_node *add_front = calloc(1,sizeof(struct list_node));
	add_front->next = self->first;
	add_front->value = value;
	self->first = add_front;   
}

// Insertion d'un élément à l'emplacement index, il suffit d'avancer jusqu'à ce que l'on tombe sur le bon indice
void list_insert(struct list *self, int value, size_t index)
{
	struct list_node *new = calloc(1,sizeof(struct list_node));
	new->value = value;
	struct list_node *curr = self->first;
	if(index == 0)
	{
		self->first = new;
		new->next = curr;
	}
	else
	{
		int i = 1;
		while(i!=index)
		{
			curr = curr->next;
			i++;
		}
		struct list_node *next_new = curr->next;
		curr->next = new;
		new->next = next_new;
	}
}

// Supprime un élément d'indice index dans une liste. 
void list_remove(struct list *self, size_t index) 
{
	if(index == 0)
	{
		struct list_node *to_del = self->first;
		struct list_node *to_keep = self->first->next;
		free(to_del);
		self->first = to_keep;
	}
	else
	{
		int i = 1;
		struct list_node *curr = self->first;
		while(i!=index)
		{
			curr = curr->next;
			i++;
		}
		struct list_node *next_del = curr->next->next;
		struct list_node *to_del = curr->next;
		free(to_del);
		curr->next = next_del;
	}
}

// Récupère un élément à l'indice index
int *list_get(const struct list *self, size_t index)
{
	struct list_node *curr = self->first;
	for(int i = 1; i <= index; i++)
	{
		curr = curr->next;
	}
	return &curr->value;
}

// renvoie true si la liste est vide
bool list_is_empty(const struct list *self)
{
	if(self->first == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// calcul la taille de la liste
size_t list_size(const struct list *self) 
{
	size_t size = 0;
	if(self->first == NULL)
	{
		return size;
	}
	else
	{
		struct list_node *curr = self->first;
		while(curr != NULL)
		{
			curr = curr->next;
			size++;
		}
		return size;
	}
}

// Recherche un élément dans une liste et renvoie son indice
size_t list_search(const struct list *self, int value) 
{
	size_t size = 0;
	size_t size_liste = list_size(self);
	struct list_node *curr = self->first;
	while(curr->value != value)
	{
		if(size == size_liste-1)
		{
			return size+1;
		}
		curr = curr->next;
		size++;
	}
	return size;
}

// importe un tableau dans une liste
void list_import(struct list *self, const int *other, size_t size)
{
	if(!list_is_empty(self))
	{
		list_destroy(self);
		list_create(self);
	}
	if(size > 0)
	{
		for(size_t i=0; i < size; ++i )
		{
			list_insert(self,other[i], i);
		}
	}
}

// Inverse de la fonction ci-dessus. Copie une liste dans un tableau
void list_export(struct list_node *self, int *other)
{
	if(self != NULL)
	{
		int i = 0;
		struct list_node *curr = self;
		while(curr != NULL)
		{
			other[i] = curr->value;
			i++;
			curr = curr->next;
		}
	}
}

// Affiche un tableau
void list_dump(const struct list *self) 
{
	if(!list_is_empty(self))
	{
		struct list_node *curr = self->first;
		while(curr != NULL)
		{
			printf("%i\n", curr->value);
			curr = curr->next;
		}
	}
}

// Renvoie true si la liste est triée
bool list_is_sorted(const struct list *self) 
{
	if(self->first != NULL)
	{
		struct list_node *curr = self->first;
		while(curr->next != NULL)
		{
			if(curr->value > curr->next->value)
			{
				return false;
			}
			curr = curr->next;
		}
	}
	return true;
}

// permet de trier le tableau
void array_merge2(int *self, size_t i, size_t m, size_t j,int *tmp) 
{
	size_t a = i;
	size_t b = m;
	for (size_t k = i; k < j; ++k) 
	{
		if (a < m && (b == j || self[a] < self[b])) 
		{
			tmp[k] = self[a];
			a++;
		} 
		else 
		{
			tmp[k] = self[b];
			b++;
		}
	}
}

// effectue le tri fusion sur le tableau
void array_merge_sort_partial2(int *tab, size_t i, size_t j, int *tmp)
{
	if(j - i < 2)
	{
		return;
	}
	size_t mid = (i + j) / 2;
	array_merge_sort_partial2(tab, i, mid, tmp);
	array_merge_sort_partial2(tab, mid, j, tmp);
	array_merge2(tab, i, mid, j, tmp);
	memcpy(tab + i, tmp + i, (j - i) * sizeof(int));

}


// La technique utilisé ici consiste à transférer la liste dans un tableau et à effectuer le tri sur ce dernier puis de retransférer son contenu trier dans la liste.
void list_merge_sort(struct list *self)
{
	if(list_is_sorted(self))
	{
		return;
	}
	else
	{
		struct list_node *el = self->first;
		size_t size = list_size(self);
		int *tab = calloc(size, sizeof(int));
		list_export(el, tab);
		int *tmp = calloc(size, sizeof(int));
		array_merge_sort_partial2(tab, 0, size, tmp);
		free(tmp);
		list_import(self, tab, size);
		free(tab);
	}
}
