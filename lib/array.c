#include "array.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

// Initialise un tableau
void array_create(struct array *self)
{
	self->size = 0;
	self->capacity = 10;
	self->data = calloc(self->capacity, sizeof(int));
}

// Supprime un tableau
void array_destroy(struct array *self) 
{
	self->size = 0;
	self->capacity = 0;
	free(self->data);
}

// Renvoie true si les deux tableaux sont égaux 
bool array_equals(const struct array *self, const int *content, size_t size) 
{
	if(self->size == size)
	{
		for(int i = 0; i < size; i++)
		{
			if(!(self->data[i] == content[i]))		// Cas d'arrêt
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

// Fonction qui permet d'agrandir la capacité d'un tableau
void array_grow(struct array *self)
{
	self->capacity *= 2;	//O(1) amorti
	int *new_data = calloc(self->capacity, sizeof(int));	// nouveau tableau
	memcpy(new_data, self->data, self->size * sizeof(int));	// Copie
	free(self->data);	// Suppression de l'ancien
	self->data = new_data;	// Copie du nouveau dans la structure
}

// Ajoute l'élément e
void array_add(struct array *self, int e)
{
	if (self->size == self->capacity)	// Si plus de place dans le tableau
	{
		array_grow(self);
	}
	self->data[self->size] = e;
	self->size += 1;
}

// Insertion d'un élélent value à l'emplacement index
void array_insert(struct array *self, int value, size_t index) 
{
	if (self->size == self->capacity)
	{
		array_grow(self);
	}
	self->size += 1;
	int save1 = self->data[index];
	self->data[index] = value;
	int save2 = 0;
	for(int i = index+1; i < self->size; i++)
	{
		if((i-index)%2 != 0)
		{
			save2 = self->data[i];
			self->data[i] = save1;
		}
		else
		{
			save1 = self->data[i];
			self->data[i] = save2;
		}	
	}
}

// Supprime l'élément à l'emplacement index peu importe son contenu
void array_remove(struct array *self, size_t index) 
{
	self->size -= 1;
	for(int i = index; i < self->size; i++)
	{
		self->data[i] = self->data[i+1];
	}
}

// Récupère l'élément qui se situe à l'indice index
int *array_get(const struct array *self, size_t index) 
{
	if(index >= self->size)
	{
		return NULL;
	}
	return &self->data[index];
}

// Renvoie true si le tableau est vide
bool array_is_empty(const struct array *self) 
{
	if(self->size == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// renvoie la taille du tableau
size_t array_size(const struct array *self) 
{
	return self->size;
}

// renvoie l'indice de l'élement value. Si ce dernier n'est pas présent, revoie 0
size_t array_search(const struct array *self, int value) 
{
	if(!array_is_empty(self))
	{
		size_t res = 0;
		for(int i = 0; i < self->size; i++)	
		{
			if(self->data[i] == value)
			{
				return res;
			}
			res++;
		}
		return res;
	}
	return 0;
}

// Effectue une recherche dans un tableau en le divisant en deux à chaque fois. Le tableau doit être trié.
size_t array_binary_search(const struct array *self, size_t n, int e, size_t lo, size_t hi) 
{
	if (lo == hi) 
	{
		return n;
	}
	size_t mid = (lo + hi) / 2;
	// On coupe le tableau en deux et on recherche l'élément dans la moitié correspondante.
	if (e < self->data[mid]) 
	{
		return array_binary_search(self, n, e, lo, mid);
	}
	if (self->data[mid] < e) 
	{
		return array_binary_search(self, n, e, mid + 1, hi);
	}
	return mid;
}

// initialise la fonction ci-dessus
size_t array_search_sorted(const struct array *self, int value)
{
	return array_binary_search(self, self->size, value, 0, self->size);
}

// Copie le contenue d'un autre tableau dans celui de la structure
void array_import(struct array *self, const int *other, size_t size) 
{
	while(self->capacity < size)	// tant que la capacité ne suffit pas, on l'augmente. 
	{
		array_grow(self);
	}
	for(int i = 0; i < size; i++)
	{
		self->data[i] = other[i];
	}
	self->size = size;
}

// Affichage simple
void array_dump(const struct array *self)
{
	for(int i = 0; i < self->size ; ++i)
	{
		printf("Element %i : %i\n", i , self->data[i]);
	}
}

// Renvoie true si le tableau est trié (par ordre croissant)
bool array_is_sorted(const struct array *self)
{
	for(size_t i = 1; i < self->size ; ++i)
	{
		if(self->data[i-1] > self->data[i])
		{
			return false;
		}
	}
	return true;
}

// Echange les éléments aux indices i et j entre eux
void array_swap(struct array *self, size_t i, size_t j)
{
	int save = self->data[i];
	self->data[i] = self->data[j];
	self->data[j] = save;
}

// Tri par selection
void array_selection_sort(struct array *self)
{
	for (size_t i = 0; i < self->size ; ++i) 
	{
		size_t j = i;
		// On parcoure le tableau et on remet l'élément à sa place en fonction des autres éléments
		for (size_t k = j + 1; k < self->size ; ++k) 
		{
			if (self->data[k] < self->data[j]) 
			{
				j = k;
			}
		}
		array_swap(self, i, j);
	}
}

// Tri à bulle
void array_bubble_sort(struct array *self) 
{
	for (size_t i = 0; i < self->size - 1; ++i)	// Parcoure les éléments du tableau
	{
		for (size_t j = self->size - 1; j > i; --j) // Parcoure les éléments restants jusqu'à la fin du tableau
		{
			if (self->data[j] < self->data[j - 1]) 	// Si l'élément n'est pas à la bonne place comparé à l'élément précédent, on les inverses
			{
				array_swap(self, j, j - 1);
			}
		}
	}
}

// Tri par insertion
void array_insertion_sort(struct array *self)
{
	for (size_t i = 1; i < self->size; ++i)
	{
		int x = self->data[i];
		size_t j = i;
		while (j > 0 && self->data[j - 1] > x)	// Pour chaque élément on parcoure le tableau dans le sens inverse et on les inverses si jamais 
		{										// leur emplacement n'est pas correct
			self->data[j] = self->data[j - 1];
			j--;
		}
		self->data[j] = x;
	}
}

// Partitionne un tableau en prenant pour pivot la plus petite valeur renseignée
signed array_partition(struct array *self, signed i, signed j) 
{
	signed pivot_index = i;
	const int pivot = self->data[pivot_index];
	array_swap(self, pivot_index, j);
	signed l = i;
	for (signed k = i; k < j; ++k)
	{
		if (self->data[k] < pivot)
		{
			array_swap(self, k, l);
			l++;
		}
	}
	array_swap(self, l, j);
	return l;
}

// Applique le tri rapide autour d'un privot (p)
void array_quick_sort_partial(struct array *self, signed i, signed j)
{
	if (i < j)
	{
		signed p = array_partition(self, i, j);
		array_quick_sort_partial(self, i, p - 1);
		array_quick_sort_partial(self, p + 1, j);
	}
}

// Initialise le tri rapide
void array_quick_sort(struct array *self)
{
	array_quick_sort_partial(self, 0, self->size - 1);
}

// Tri fusion 
void array_merge(struct array *self, size_t i, size_t m, size_t j,int *tmp)
{
	size_t a = i;
	size_t b = m;
	for (size_t k = i; k < j; ++k) 
	{
		if (a < m && (b == j || self->data[a] < self->data[b])) 
		{
			tmp[k] = self->data[a];a++;
		} 
		else 
		{
			tmp[k] = self->data[b];
			b++;
		}
	}
}

// Applique le tri fusion
void array_merge_sort_partial(struct array *self,size_t i, size_t j, int *tmp) 
{
	if (j - i < 2) 
	{
		return;
	}
	size_t m = (i + j) / 2;
	array_merge_sort_partial(self, i, m, tmp);
	array_merge_sort_partial(self, m, j, tmp);
	array_merge(self, i, m, j, tmp);
	memcpy(self->data + i, tmp + i, (j - i) * sizeof(int));
}

// Tri d'un tas avec un tri fusion
void array_heap_sort(struct array *self)
{
	int *tmp = calloc(self->size, sizeof(int));
	array_merge_sort_partial(self, 0, self->size, tmp);
	free(tmp);
}

// Vérifie que le tableau est un tas
bool array_is_heap(const struct array *self)
{
  size_t i = 0;
  while (i < self->size) 
  {
	size_t lt = 2 * i + 1;
	size_t rt = 2 * i + 2;
	if(lt < self->size)
	{
		if (self->data[i] < self->data[lt])
		{
			return false;
		}
	}
	if(rt < self->size)
	{
		if (self->data[i] < self->data[rt])
		{
			return false;
		}
	}
	i++;
  }
  return true;
}

// Ajoute un élément à un tas
void array_heap_add(struct array *self, int value) 
{
	if(self->size == self->capacity)
    {
        array_grow(self);
    }
	size_t i = self->size;
	self->data[i] = value;
	while (i > 0) 
	{
		signed j = (i - 1) / 2;
		if (self->data[i] < self->data[j]) 
		{
			break;
		}
		array_swap(self, i, j);
		i = j;
	}
    self->size++;
}

// Renvoie la premier élément du tas
int array_heap_top(const struct array *self) 
{
  return self->data[0];
}

// Repositionne les éléments du tas à leur bonne place une fois le plus grand élément supprimé
void heap_fix(struct array *self, int size, int i)
{
  if (size == 1){}
  else
  {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && self->data[l] > self->data[largest])
	{
      largest = l;
	}
    if (r < size && self->data[r] > self->data[largest])
    {
	  largest = r;
	}
    if (largest != i)
    {
	  array_swap(self, i, largest);
      heap_fix(self, size, largest);
    }
  }
}

// Supprime un élément d'un tas
void heap_delete(struct array *self, int num)
{
  int i;
  for (i = 0; i < self->size; i++)
  {
    if (num == self->data[i])
      break;
  }
  array_swap(self, i, self->size - 1);
  self->size--;
  for (int i = self->size / 2 - 1; i >= 0; i--)
  {
    heap_fix(self, self->size, i);
  }
}

// Supprime le plus grand élément d'un tas
void array_heap_remove_top(struct array *self)
{
	if(self->size > 0)
	{
		heap_delete(self, array_heap_top(self));
	}
}