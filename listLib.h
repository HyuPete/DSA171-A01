/*
 * =========================================================================================
 * Name        : listLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};


template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
	L1Item(char *a){} // using for type char
};

template <class T>
class L1List {
	L1Item<T>   *_pHead;// The head pointer of linked list
	L1Item<T> *pTail;
	size_t      _size;// number of elements in this list
public:
	L1List() : _pHead(NULL), pTail(nullptr), _size(0) {}
	L1List(T &a) :_pHead{ new L1Item<T>(a) }, _size{ 1 } { pTail = _pHead; }
	L1List(T *a) {}
	L1List(char a){}
	// copy constructor
	L1List(L1List<T> &L): L1List()
	{
		L1Item<T> *p{ L.getHead() };
		while (p)
		{
			push_back(p->data);
			p = p->pNext;
		}
	}
	~L1List()
	{
		clean();
	}

	void    clean()
	{
		while (removeHead() == 0) { continue; }
		return;
	}
	bool    isEmpty() {
		return _pHead == NULL;
	}
	size_t  getSize() {
		return _size;
	}
	L1Item<T>* getHead() { return _pHead; }
	L1Item<T>* getTail() { return pTail; }
	// decrease size, be carefull to use this one
	void decreaseSizeBy1() { _size--; }

	// access list member, i = 0..(_size - 1)
	T&      at(int i)
	{
		if (!_pHead) throw DSAException(1, "Empty list!");
		if (i<0 || i> _size - 1) throw DSAException(2, "Invalid index!");
		if (i == _size - 1)
		{
			if (pTail) return pTail->data;
			else throw DSAException(3, "Out of bound!");
		}
		L1Item<T> *p{ _pHead };
		while (true)
		{
			if (i == 0) return p->data;
			p = p->pNext; i--;
			if (!p) throw DSAException(3, "Out of bound!");
		}
	}
	T&      operator[](int i)
	{
		if (!_pHead) throw DSAException(1, "Empty list!");
		if (i < 0 || i > _size - 1) throw DSAException(2, "Invalid index!");
		if (i == _size - 1)
		{
			if (pTail) return pTail->data;
			else throw DSAException(3, "Out of bound!");
		}
		L1Item<T> *p{ _pHead };
		while (true)
		{
			if (i == 0) return p->data;
			p = p->pNext; i--;
			if (!p) throw DSAException(3, "Out of bound!");
		}
	}

	// find the element having the target data, return true/false
	// idx will be assigned to the index of the found element for future access
	// if not found idx = -1
	bool    find(T &a, int &idx)
	{
		try
		{
			if (!_pHead) throw DSAException(3, "Empty list!");
			L1Item<T> *p{ _pHead };
			idx = 0;
			while (p)
			{
				if (p->data == a) return 1;
				p = p->pNext; idx++;
			}
			idx = -1;
			return 0;
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			idx = -1;
			return 0;
		}
	}
	// return true/false, if not found idx = -1
	bool find(char *ch, int &idx) {}
	// insert a item after the the item with index i
	// return 0 if success
	int     insert(int i, T &a)
	{
		try
		{
			if (!_pHead) throw DSAException(1, "Empty list!");
			if (i < 0 || i >_size - 1) throw DSAException(2, "Invalid index!");
			L1Item<T> *p{ _pHead };
			while (true)
			{
				if (i == 0)
				{
					L1Item<T> *pNew{ new L1Item<T>(a) };
					pNew->pNext = p->pNext;
					p->pNext = pNew;
					_size++;
					return 0;
				}
				p = p->pNext; i--;
				if (!p) throw DSAException(3, "Out of bound!");
			}
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			return -1;
		}
	}
	// remove node past this one
	// return 0 if success
	int     remove(int i)
	{
		try
		{
			if (!_pHead) throw DSAException(1, "Empty list!");
			if (i < 0 || i >_size - 1) throw DSAException(2, "Invalid index!");
			if (i == _size - 1) throw DSAException(4, "There is no item after the last item in the list to be removed!");
			L1Item<T> *p{ _pHead };
			while (true)
			{
				if (!p->pNext) throw DSAException(3, "Out of bound!");
				if (i == 0)
				{
					L1Item<T> *pDel{ p->pNext };
					p->pNext = pDel->pNext;
					delete pDel;
					_size--;
					return 0;
				}
				p = p->pNext; i--;
			}
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			return -1;
		}
	}
	// remove node past this one
	int     push_back(T &a); // implement below
	int push_back(char *a) {} // using for type char[]
	int     insertHead(T &a); // implement below
	//void insertHead(char){}
	int insertHead(char *a) {} // using for type char[]
	/// Remove the first item of the list
	/// Return 0 if success
	int     removeHead()
	{
		if (_pHead) {
			if (pTail == _pHead) pTail = nullptr;
			L1Item<T> *p = _pHead;
			_pHead = p->pNext;
			delete p;
			_size--;
			return 0;
		}
		return -1;
	}
    int     removeLast(); // implement below
	
	// call the hiden function reverse implemented by recursion
	void reverse()
	{
		reverse(_pHead); return;
	}

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
	void traverse(int(*op)(T&))
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	void traverse(void(*op)(L1Item<T>*))
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p);
			p = p->pNext;
		}
	}
	// copy the entire list to another list
	void traverse(L1List<T> &pL)
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			pL.push_back(p->data);
			p = p->pNext;
		}
	}
	// special traverse for 
	void traverse(T &a) {}
	void traverse(T *a) {}
	//
    void    traverse(void (*op)(T&, void*), void *pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
	// traverse until found item.data < a <= item.next.data, then insert a in between
	void traverseAndInsert(T &a) {}
	// special traverse funtction for list ninjaInfo
	void traverse(L1List<L1List<T>> &lL){}
	// if not found idx = -1
	const char* traverseAndRemove(char *killerID, int &targetIndex) {}
	// overload operator
	
protected:
	// reverse the list by recursion
	// parameter 'int i' to mark whether or not pH are pointing to the original head item of the list (true when i = 0)
	void    reverse(L1Item<T> *pH, short i = 0)
		{
			if (!pH)
			{
				cerr << "\nEmpty list cannot be reversed!\n";
				return;
			}
			if (!pH->pNext)
			{
				_pHead = pH;
				return;
			}
			reverse(pH->pNext, 1);
			pH->pNext->pNext = pH;
			if (i == 0)
			{
				pH->pNext = nullptr; 
				pTail = pH;
			}
			return;
		}
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
		pTail = _pHead;
    }
    else {
        /*L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);*/
		pTail->pNext = new L1Item<T>(a);
		pTail = pTail->pNext;
    }
    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
	if (!pTail) pTail = _pHead;
    _size++;
    return 0;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
			pTail = prev;
        }
        else {
            delete _pHead;
			_pHead = pTail = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}





#endif //A01_LISTLIB_H
