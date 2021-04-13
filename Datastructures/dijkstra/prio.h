/**
* klassenaam: A priority queue class 
* Dennis Buurman (s2027100)
* prio.h
* datum laatste wijziging: 5-12-2019
*
* The root element will be at Arr[0].
* Below table shows indexes of other nodes for the ith node, i.e., Arr[i]:
*
* Arr[(i-1)/2]	Returns the parent node
* Arr[(2*i)+1]	Returns the left child node
* Arr[(2*i)+2]	Returns the right child node
**/
#ifndef prio_h
#define prio_h

#include <iostream>

int const LIST_SIZE = 1024;

template<typename T>
struct Element
{
	T item;
	int prio;
};

template <typename T>
class Prio
{
public:
	Prio();
	~Prio();

	/**
	* @function: print
	* @abstract: prints the queue elements
	* @param: -
	* @return: -
	**/
	void print() const;

	/**
	* @function: isEmpty
	* @abstract: checks if there are elements in the queue and returns the result
	* @param: -
	* @return: true if empty, false if not empty
	**/
	bool isEmpty() const;

	/**
	* @function: insert
	* @abstract: inserts an item with priority in the queue
	* @param: T item -> item to be added
	* int prio -> priority of the item
	* @return: -
	**/
	void insert(T const item, int const prio);

	/**
	* @function: getMinimum
	* @abstract: gives back the item with the lowest priority
	* @param: -
	* @return: T (Element.item)
	**/
	T getMinimum();

	/**
	* @function: deleteMinimum
	* @abstract: removes the item with the lowest priority
	* @param: -
	* @return: -
	**/
	void deleteMinimum();

	/**
	* @function: bubbleUp
	* @abstract: restores heap by bubbling up an element (swap with parent till restored)
	* @param: int i -> index of the element
	* @return: -
	**/
	void bubbleUp(int i);

	/**
	* @function: trickleDown
	* @abstract: restores heap by trickling down an element (swap with a child till restored)
	* @param: int i -> index of the element
	* @return: -
	**/
	void trickleDown(int const i);

	/**
	* @function: decreaseKey
	* @abstract: decreases the priority of an item
	* @param: T item -> item
	* int nprio -> new priority of the item
	* @return: -
	**/
	void decreaseKey(T const item, int const nprio);

	/**
	* @function: parent
	* @abstract: returns the parent index of the given index
	* @param: int i -> element index 
	* @return: 
	**/
	int parent(int const i) const;

	/**
	* @function: left
	* @abstract: returns the left child index of the given index
	* @param: int i -> element index 
	* @return: 
	**/
	int left(int const i) const;

	/**
	* @function: right
	* @abstract: returns the right child index of the given index
	* @param: int i -> element index 
	* @return: 
	**/
	int right(int const i) const;

private:
	int nodes;
	Element<T> array[LIST_SIZE];
};

template <typename T>
Prio<T>::Prio()
{
	nodes = 0;
}

template <typename T>
Prio<T>::~Prio()
{

}

template <typename T>
void Prio<T>::print() const
{
	for(int i = 0; i < nodes; i++)
		std::cout << i << ") " << array[i].item  << ", " << array[i].prio << std::endl;
}

template <typename T>
int Prio<T>::parent(int const i) const
{
	return ((i-1)/2);
}

template <typename T>
int Prio<T>::left(int const i) const
{
	return ((2*i)+1);	
}

template <typename T>
int Prio<T>::right(int const i) const
{
	return ((2*i)+2);
}


template <typename T>
void Prio<T>::bubbleUp(int i)
{
	int tp;
	T ti;

	while(array[parent(i)].prio > array[i].prio && i != 0)
	{
		ti = array[parent(i)].item;
		tp = array[parent(i)].prio;
		array[parent(i)].item = array[i].item;
		array[parent(i)].prio = array[i].prio;
		array[i].item = ti;
		array[i].prio = tp;
		i = parent(i);
	}
}

template <typename T>
void Prio<T>::trickleDown(int const i)
{
	int min = i;
	int tp;
	T ti;

	if(left(i) < nodes && array[left(i)].prio < array[i].prio)
		min = left(i);
	if(right(i) < nodes && array[right(i)].prio < array[i].prio)
		min = right(i);
	if(min != i)
	{
		ti = array[min].item;
		tp = array[min].prio;
		array[min].item = array[i].item;
		array[min].prio = array[i].prio;
		array[i].item = ti;
		array[i].prio = tp;

		trickleDown(min);
	}
}

template <typename T>
void Prio<T>::decreaseKey(T const item, int const nprio)
{
	int i = 0;
	bool found = false;

	if(isEmpty())
		return;
	while(i < nodes && !found)
	{
		if(array[i].item == item)
			found = true;
		else
			i++;
	}
	if(!found)
	{
		std::cout << "Item not in queue" << std::endl;
		return;
	}

	array[i].prio = nprio;
	bubbleUp(i);
}

template<typename T>
bool Prio<T>::isEmpty() const
{
	if(nodes)
		return false;
	return true;
}

template <typename T>
void Prio<T>::insert(T const item, int const prio)
{
	if(prio < 0)
	{
		std::cout << "Negative priority not allowed" << std::endl;
		return;
	}
	if(nodes < LIST_SIZE)
	{
		array[nodes].item = item;
		array[nodes].prio = prio;
		nodes++;
		if(prio < array[(nodes-1)/2].prio)
			bubbleUp(nodes-1);
	}
	else
		std::cout << "Overflow" << std::endl;
}

template <typename T>
T Prio<T>::getMinimum()
{
	return array[0].item;
}

template <typename T>
void Prio<T>::deleteMinimum()
{
	if(isEmpty())
		return;
	if(nodes == 1)
	{
		nodes--;
		return;
	}

	array[0] = array[nodes-1];
	nodes--;
	trickleDown(0);
}

#endif