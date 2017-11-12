/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */
#include "eventLib.h"
#include "dbLib.h"

 // removeHead version for List of list
template<>
int L1List<L1List<NinjaInfo>>::removeHead()
{
	if (_pHead)
	{
		if (pTail == _pHead) pTail = nullptr;
		_pHead->data.clean();
		L1Item<L1List<NinjaInfo_t>> *p = _pHead;
		_pHead = p->pNext;
		delete p;
		_size--;
		return 0;
	}
	return -1;
}
 // Traverse in a list of a specific ninja's data and insert new item in ascending order of time
template<>
void L1List<NinjaInfo>::traverseAndInsert(NinjaInfo &a)
{
	if (!_pHead)
	{
		push_back(a);
		return;
	}
	if (a.timestamp <= _pHead->data.timestamp)
	{
		insertHead(a);
		return;
	}
	if (!_pHead->pNext)
	{
		push_back(a);
		return;
	}
	L1Item<NinjaInfo> *pCurr{ _pHead->pNext };
	L1Item<NinjaInfo> *pPre{ _pHead };
	while (1)
	{
		if (a.timestamp <= pCurr->data.timestamp)
		{
			pPre->pNext = new L1Item<NinjaInfo>(a);
			pPre->pNext->pNext = pCurr;
			return;
		}
		if (a.timestamp > pCurr->data.timestamp && !pCurr->pNext)
		{
			push_back(a);
			return;
		}
		pPre = pCurr;
		pCurr = pCurr->pNext;
	}
}
// Traverse the list of list and insert data to the suitable list in ascending order
void traverse(L1List<L1List<NinjaInfo>> &L, NinjaInfo &a)
{
	if (!L.getHead())
	{
		L1List<NinjaInfo> l(a);
		L.insertHead(l);
		return;
	}
	L1Item<L1List<NinjaInfo_t>> *p{ L.getHead() };
	try
	{
		while (1)
		{
			if (a == p->data[0])
			{
				p->data.traverseAndInsert(a);
				return;
			}
			p = p->pNext;
			if (!p)
			{
				L1List<NinjaInfo> l(a);
				L.push_back(l);
				return;
			}
		}
		return;
	}
	catch (DSAException &e)
	{
		cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
		return;
	}
}
// traverse and insert item to new list, then remove that item from the list
void traverse(L1List<NinjaInfo> &lN, L1List<L1List<NinjaInfo>> &lL)
{
	try
	{
		L1Item<NinjaInfo> *p{ lN.getHead() };
		while (p)
		{
			traverse(lL, p->data);
			p = p->pNext;
		}
		return;
	}
	catch (DSAException &e)
	{
		cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
		return;
	}
}

using  L1Event = L1List<ninjaEvent_t>;
void printEvent(L1Item<ninjaEvent_t> *a) { if (a->pNext) cout << a->data.code << ' '; else cout << a->data.code << '\n'; }

// structure for global data
struct PointerToGData
{
	L1List<ninjaEvent_t> pLE;
	L1List<L1List<NinjaInfo_t>> pLL;
};
// function handle envent
bool processEvent(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList, void *pGData) {
    // TODO: Your code comes here
	try
	{
		if (event.code[0] >= '0' && event.code[0] <= '9')
		{
			switch (event.code[0])
			{
			case'0': // '0'
				if (event.code[1] == 0)
				{
					cout << event.code << ": ";
					static_cast<PointerToGData*>(pGData)->pLE.traverse(printEvent);
					return 1;
				}
				else
				{
					return 0;
				}
			case'1': // '1' or "1x..."
				if (event.code[1] == 0) // '1'
				{
					cout << event.code << ": ";
					cout << nList[0].id << '\n';
					return 1;
				}
				else if (event.code[1] >= '0' && event.code[1] <= '4') // "1x..."
				{
					switch (event.code[0])
					{
					case'0': // "10"
						if (event.code[2] == 0)
						{

						}
						else
						{
							static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
							return 0;
						}
					case'1': // "11XYZT"
						if (event.code[6] == 0)
						{

						}
						else
						{
							static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
							return 0;
						}
					case'2': // "12"
						if (event.code[2] == 0)
						{

						}
						else
						{
							static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
							return 0;
						}
					case'3': // "13ABCDmnpqEFGHuvrs
						if (event.code[18] == 0)
						{

						}
						else
						{
							static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
							return 0;
						}
					case'4':
						if (event.code[2] == 0)
						{

						}
						else
						{
							static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
							return 0;
						}
					default:
						break;
					}
				}
				else
				{
					return 0;
				}
			case'2': // '2'
				if (event.code[1] == 0)
				{
					cout << event.code << ": " << nList.getTail()->data.id << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			case'3': // '3'
				if (event.code[1] == 0)
				{
					cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->pLL.getSize() << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			case'4': // '4'
				if (event.code[1] == 0)
				{
					

					return 1;
				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			case'5': // "5ABCD"
				if (event.code[5] == 0)
				{

				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			case'6': // "6ABCD"
				if (event.code[5] == 0)
				{

				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			case'7': // "7ABCD"
				if (event.code[5] == 0)
				{

				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			case'8': // "8ABCD"
				if (event.code[5] == 0)
				{

				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			case'9': // '9'
				if (event.code[1] == 0)
				{
					//Todo:

					return 1;
				}
				else
				{
					static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
					return 0;
				}
			default:
				static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
				return 0;
			}
		}
		else
		{
			static_cast<L1List<ninjaEvent_t>*>(pGData)->push_back(event);
			return 0;
		}
	}
	catch (DSAException &e)
	{
		cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
		return 0;
	}
    /// NOTE: The output of the event will be printed on one line
    /// end by the endline character.
}

//
void printListNinja(L1List<NinjaInfo> &l)
{
	l.traverse(printNinjaInfo);
	return;
}