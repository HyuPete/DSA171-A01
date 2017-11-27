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

//using  L1Event = L1List<ninjaEvent_t>;
void printEvent(L1Item<ninjaEvent_t> *a) { if (a->pNext) cout << a->data.code << ' '; else cout << a->data.code << '\n'; }

// struct for the coordinates
struct PointOnEarth
{
	double longt{ 0.0 }, lat{ 0.0 };
	PointOnEarth() {}
	PointOnEarth(double a, double b) :longt{ a }, lat{ b } {}
};
// structure for global data
//struct TimeAnalytics
//
class PointerToGData
{
public:
	// attributes
	L1List<ninjaEvent_t> pLE;                                 // copy list of events list for reprint
	L1List<L1List<NinjaInfo_t>> pLL;                          // list of lists
	double *totalDistance{ nullptr };                         // total distance of traveling array
	time_t *totalUnmovingTime{ nullptr };                     // array of total unmoved time of each ninja
	size_t *numOfTimesUnmoving{ nullptr };					  // array of the number of times each ninja unmove 
	time_t *firstTimeMoving{ nullptr };                       // array of each ninja's first time moving
	time_t *lastTimeUnmoving{ nullptr };                      // array of each ninja's last time unmoving
	time_t *totalMovingTime{ nullptr };                       // array of each ninja's total moving time
	// methods
	PointerToGData() {}
	~PointerToGData();
	const char* findMaxID();
	const char* findAndKill(char *killerID);
	void statisticizeData(); // Ninjas'data statistic
	const char* findFirstTimeMovingOf(char *ninjaID);
	const char* findLastTimeUnmovingOf(char *ninjaID);
	int findNumberOfUnmovingTimesOf(char *ninjaID);
	string findSumOfTravelingDistanceOf(char *ninjaID);
	const char* findMaxTrvlDist();
	const char* findMaxTrvlTime();
	const char* findMaxUnmovingTime();
	void findAndPrintListOfNinjaTrapped(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D);
	void findAndPrintListOfNinjasLost();
	// friend function
	friend bool checkIfTowGivenLineSegmentsIntersect(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D);
	friend bool checkIfAGivenLineSegmentIntersectAGivenSquare(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D, PointOnEarth &E, PointOnEarth &F);
	friend bool checkIfAPointIsInsideOrOnTheEdgeOfAGivenSquare(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D, PointOnEarth &E);
private:
};

PointerToGData::~PointerToGData()
{
	delete[] totalDistance;
	delete[] totalUnmovingTime;
	delete[] numOfTimesUnmoving; 
	delete[] firstTimeMoving;
	delete[] lastTimeUnmoving;
	delete[] totalMovingTime;
}
// return "-1"/ID
const char* PointerToGData::findMaxID()
{
	try
	{
		L1Item<L1List<NinjaInfo>> *p{ pLL.getHead() };
		if (!p) { return "-1"; }
		char *maxID{ p->data[0].id };
		p = p->pNext;
		while (p)
		{
			char *ctmp{ p->data[0].id };
			if (strcmp(maxID, ctmp) < 0) maxID = ctmp;
			p = p->pNext;
		}
		return maxID;
	}
	catch (DSAException &e)
	{
		cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
		return "-1";
	}
}
//############################
// return "-1"/ID
template<>
const char* L1List<L1List<NinjaInfo>>::traverseAndRemove(char *killerID, int &targetIndex)
{
	if (!killerID) { targetIndex = -1; return "-1"; }
	if (!_pHead) { targetIndex = -1;  return "-1"; }
	// find out whether the killer exist
	L1Item<L1List<NinjaInfo>> *p{ _pHead }, *pPreTarget{ nullptr }, *pPre{ nullptr };
	bool isTargetExist{ 0 };
	char *target{ new char[ID_MAX_LENGTH] }; //{""}: doesn't work in C++11
	strcpy( target , "" );
	size_t index{ 0 };
	while (true)
	{
		char *possibleTarget{ p->data[0].id };
		int cmp{ strcmp(possibleTarget, killerID) };
		if (cmp < 0 && strcmp(possibleTarget, target) > 0)
		{
			strcpy(target, possibleTarget);
			pPreTarget = pPre; // save for killing
			if (!isTargetExist) isTargetExist = 1;
			targetIndex = index;
		}
		//else if (cmp == 0) isKillerExist = 1;
		if (!p->pNext)
		{
			if (!isTargetExist) { delete target; targetIndex = -1; return "-1"; }
			// killing
			if (!pPreTarget) removeHead();  // target is head
			else if (!pPreTarget->pNext->pNext) // target is tail
			{
				delete pTail; // the process already include cleaning
				pPreTarget->pNext = nullptr;
				pTail = pPreTarget;
			}
			else
			{
				p = pPreTarget->pNext; // p <-- target
				pPreTarget->pNext = p->pNext;
				delete p; // the process already include cleaning
			}
			_size--;
			return target;
		}
		pPre = p;
		p = p->pNext;
		index++;
	}
}
// return true/false, refernece parameter is referrenced to a copy of found ID which should be deallocated
const char* PointerToGData::findAndKill(char *killerID)
{
	int targetIndex;
	const char *ch{ pLL.traverseAndRemove(killerID, targetIndex) };
	// update statistical data
	if (targetIndex != -1) // if someone was killed --> update data
	{
		size_t s{ pLL.getSize() }; // 1 unit less than the original
		double *tD   = new double[s]();
		time_t *tUT  = new time_t[s]();
		size_t *nOTU = new size_t[s]();
		time_t *fTM  = new time_t[s]();
		time_t *lTU  = new time_t[s]();
		time_t *tMT  = new time_t[s]();
		for (size_t i = 0, j = 0; i < s; i++, j++) // copy stage
		{
			if (j == targetIndex) j++; // skip if index == targetIndex
			tD[i]   = totalDistance[j];
			tUT[i]  = totalUnmovingTime[j];
			nOTU[i] = numOfTimesUnmoving[j];
			fTM[i]  = firstTimeMoving[j];
			lTU[i]  = lastTimeUnmoving[j];
			tMT[i]  = totalMovingTime[j];
		}
		// clear old data
		delete[] totalDistance;
		delete[] totalUnmovingTime;
		delete[] numOfTimesUnmoving;
		delete[] firstTimeMoving;
		delete[] lastTimeUnmoving;
		delete[] totalMovingTime;
		// update
		totalDistance      = tD;
		totalUnmovingTime  = tUT;
		numOfTimesUnmoving = nOTU;
		firstTimeMoving    = fTM;
		lastTimeUnmoving   = lTU;
		totalMovingTime    = tMT;
	}
	return ch;
}
//############################
const double epsilon{ 1e-12 }, conventionalUnmvDist{5e-3};
void PointerToGData::statisticizeData()
{
	if (pLL.isEmpty()) return;
	auto s{ pLL.getSize() };
	totalDistance      = new double[s]();
	totalUnmovingTime  = new time_t[s]();
	numOfTimesUnmoving = new size_t[s]();
	firstTimeMoving    = new time_t[s]();
	lastTimeUnmoving   = new time_t[s]();
	totalMovingTime    = new time_t[s]();
	auto p{ pLL.getHead() };
	// traverse and statisticize
	for (size_t i = 0; i < s; i++)
	{
		auto pBegin{ p->data.getHead() }; // save for finding first time moving
		auto plist{ pBegin }; // for traversing
		L1Item<NinjaInfo> *pCurrTUnmv{ pBegin }; // the beginning of current unmoving period
		//lastTimeUnmoving[i] = pBegin->data.timestamp; // temporary first time unmoving
		bool isMoving{ 1 };
		while (plist->pNext)
		{
			// check for first time moving
			if (firstTimeMoving[i] == 0)
			{
				if (distanceEarth(pBegin->data.latitude, pBegin->data.longitude, plist->pNext->data.latitude, plist->pNext->data.longitude) - conventionalUnmvDist >= epsilon)
				{
					firstTimeMoving[i] = plist->pNext->data.timestamp;
					//lastTimeUnmoving[i] = firstTimeMoving[i]; // temporary current time unmoving
					//pCurrTUnmv = plist;
				}
			}
			// sum of distance
			totalDistance[i] += distanceEarth(plist->data.latitude, plist->data.longitude, plist->pNext->data.latitude, plist->pNext->data.longitude);
			// sum of unmoving time and last time unmoving
			if (distanceEarth(plist->pNext->data.latitude, plist->pNext->data.longitude, pCurrTUnmv->data.latitude, pCurrTUnmv->data.longitude) - conventionalUnmvDist < epsilon)
			{
				totalUnmovingTime[i] += plist->pNext->data.timestamp - plist->data.timestamp;
				if (isMoving)
				{
					isMoving = 0;
					lastTimeUnmoving[i] = pCurrTUnmv->data.timestamp; // temporary
					numOfTimesUnmoving[i]++;
				}
			}
			else
			{
				totalMovingTime[i] += plist->pNext->data.timestamp - plist->data.timestamp;
				pCurrTUnmv = plist->pNext;
				if (!isMoving) { isMoving = 1; }
			}
			plist = plist->pNext;
		}
		p = p->pNext;
	}
	return;
}
//############################
template<>
bool L1List<L1List<NinjaInfo>>::find(char *ninjaID, int &idx)
{
	auto p{ _pHead };
	int i{ 0 };
	while (p)
	{
		if (ninjaID == p->data[0]) { idx = i; return 1; }
		p = p->pNext;
		i++;
	}
	idx = -1;
	return 0;
}
// return "-1"/string time (should be deallocated)
const char* PointerToGData::findFirstTimeMovingOf(char *ninjaID)
{
	int idx;
	if (pLL.find(ninjaID, idx))
	{
		if (firstTimeMoving[idx] == 0) return "-1";
		char *des = new char[27];
		strPrintTime(des, firstTimeMoving[idx]);
		return des;
	}
	return "-1";
}
const char* PointerToGData::findLastTimeUnmovingOf(char *ninjaID)
{
	int idx;
	if (pLL.find(ninjaID, idx))
	{
		if (lastTimeUnmoving[idx] == 0) return "-1";
		char *des = new char[27];
		strPrintTime(des, lastTimeUnmoving[idx]);
		return des;
	}
	return "-1";
}
int PointerToGData::findNumberOfUnmovingTimesOf(char *ninjaID)
{
	int idx;
	if (pLL.find(ninjaID, idx))
	{
		return numOfTimesUnmoving[idx];
	}
	return -1;
}
string PointerToGData::findSumOfTravelingDistanceOf(char *ninjaID)
{
	int idx;
	if (pLL.find(ninjaID, idx))
	{
		string str;
		stringstream buf;
		buf << fixed << setprecision(12) << totalDistance[idx];
		buf >> str;
		return str;
	}
	return "-1";
}
//###########################
const char* PointerToGData::findMaxTrvlDist()
{
	L1Item<L1List<NinjaInfo>> *p{ pLL.getHead() };
	if (!p) return "-1";
	double maxDist{ totalDistance[0] };
	const char *maxTrvlNinjaID{ p->data[0].id }; // set max to head
	p = p->pNext;
	size_t i{ 1 };
	while (p)
	{
		if (totalDistance[i] > maxDist)
		{
			maxDist = totalDistance[i];
			maxTrvlNinjaID = p->data[0].id;
		}
		p = p->pNext;
		i++;
	}
	return maxTrvlNinjaID;
}
const char* PointerToGData::findMaxTrvlTime()
{
	L1Item<L1List<NinjaInfo>> *p{ pLL.getHead() };
	if (!p) return "-1";
	time_t maxTrvlTime{ totalMovingTime[0] };
	const char *mostMovingNinjaID{ p->data[0].id }; // set max to head
	p = p->pNext;
	size_t i{ 1 };
	while (p)
	{
		if (totalMovingTime[i] > maxTrvlTime)
		{
			maxTrvlTime = totalMovingTime[i];
			mostMovingNinjaID = p->data[0].id;
		}
		p = p->pNext;
		i++;
	}
	return mostMovingNinjaID;
}
const char* PointerToGData::findMaxUnmovingTime()
{
	L1Item<L1List<NinjaInfo>> *p{ pLL.getHead() };
	if (!p) return "-1";
	time_t maxUnmovingTime{ totalUnmovingTime[0] };
	const char *mostUnmovingNinjaID{ p->data[0].id };
	p = p->pNext;
	size_t i{ 1 };
	while (p)
	{
		if (totalUnmovingTime[i] > maxUnmovingTime)
		{
			maxUnmovingTime = totalUnmovingTime[i];
			mostUnmovingNinjaID = p->data[0].id;
		}
		p = p->pNext;
		i++;
	}
	return mostUnmovingNinjaID;
}
//##########################

//// AB & CD
//bool checkIfTowGivenLineSegmentsIntersect(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D) 
//{
//	// using cross product method ( [AB, BC] )
//	double abc{ (B.longt - A.longt)*(C.lat - B.lat) - (C.longt - B.longt)*(B.lat - A.lat) };
//	double abd{ (B.longt - A.longt)*(D.lat - B.lat) - (D.longt - B.longt)*(B.lat - A.lat) };
//	if (abc < epsilon && abc >= 0) // if A, B, C are collinear => abc = 0
//	{
//		if ((C.lat - A.lat)*(C.lat - B.lat) < epsilon && (C.longt - A.longt)*(C.longt - B.longt) < epsilon) // if ((yC - yA)(yC - yB)) <= 0
//			return 1; // if AB // Ox
//		else if (abd < epsilon && abd >= 0) // if A, B, D are collinear
//		{
//			if ((D.lat - A.lat)*(D.lat - B.lat) < epsilon && (D.longt - A.longt)*(D.longt - B.longt) < epsilon) // if ((yD - yA)(yD - yB)) <= 0
//				return 1;
//			else if ((D.lat - A.lat)*(C.lat - A.lat) < 0 || (D.longt - A.longt)*(C.longt - A.longt) < 0)
//				return 1; // AB is a part of CD
//			else return 0; // A, B, C, D are all collinear but the two line segments do not intersect
//		}
//		else return 0;
//	}
//	if (abd < epsilon && abd >=0) // if A, B, D are collinear
//	{
//		if ((D.lat - A.lat)*(D.lat - B.lat) < epsilon && (D.longt - A.longt)*(D.longt - B.longt) < epsilon) // if ((yD - yA)(yD - yB)) <= 0
//			return 1;
//		else return 0;
//	}
//	double cda{ (D.longt - C.longt)*(A.lat - D.lat) - (A.longt - D.longt)*(D.lat - C.lat) };
//	double cdb{ (D.longt - C.longt)*(B.lat - D.lat) - (B.longt - D.longt)*(D.lat - C.lat) };
//	if (cda >= 0 && cda < epsilon) // C, D, A are collinear
//	{
//		if ((A.lat - C.lat)*(A.lat - D.lat) < epsilon && (A.longt - C.longt)*(A.longt - D.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if (cdb >= 0 && cdb < epsilon) // C, D, B are collinear
//	{
//		if ((B.lat - C.lat)*(B.lat - D.lat) < epsilon && (B.longt - C.longt)*(B.longt - D.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if (abc*abd < 0 && cda * cdb < 0) // (A, B, C) , (A, B, D) differ in orientation and so do (C, D, A) & (C, D, B).
//		return 1;
//	return 0;
//}
//// square ABCD, line segment EF
//bool checkIfAGivenLineSegmentIntersectAGivenSquare(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D, PointOnEarth &E, PointOnEarth &F) // square ABCD, line segment EF
//{
//	return checkIfTowGivenLineSegmentsIntersect(A, B, E, F) || checkIfTowGivenLineSegmentsIntersect(B, C, E, F) || checkIfTowGivenLineSegmentsIntersect(C, D, E, F) || checkIfTowGivenLineSegmentsIntersect(D, A, E, F);
//}
//bool checkIfAPointIsInsideOrOnTheEdgeOfAGivenSquare(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D, PointOnEarth &E)
//{
//	double abe ((B.longt - A.longt)*(E.lat - B.lat) - (E.longt - B.longt)*(B.lat - A.lat));
//	double bce ((C.longt - B.longt)*(E.lat - C.lat) - (E.longt - C.longt)*(C.lat - B.lat));
//	double cde ((D.longt - C.longt)*(E.lat - D.lat) - (E.longt - D.longt)*(D.lat - C.lat));
//	double dae ((A.longt - D.longt)*(E.lat - A.lat) - (E.longt - A.longt)*(A.lat - D.lat));
//	if (abe >= 0 && abe < epsilon)
//	{
//		if ((E.lat - A.lat)*(E.lat - B.lat) < epsilon && (E.longt - A.longt)*(E.longt - B.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if (bce >= 0 && bce < epsilon)
//	{
//		if ((E.lat - B.lat)*(E.lat - C.lat) < epsilon && (E.longt - B.longt)*(E.longt - C.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if (cde >= 0 && cde < epsilon)
//	{
//		if ((E.lat - C.lat)*(E.lat - D.lat) < epsilon && (E.longt - C.longt)*(E.longt - D.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if (dae >= 0 && dae < epsilon)
//	{
//		if ((E.lat - D.lat)*(E.lat - A.lat) < epsilon && (E.longt - D.longt)*(E.longt - A.longt) < epsilon)
//			return 1;
//		else return 0;
//	}
//	if ((abe >= epsilon && bce >= epsilon && cde >= epsilon && dae >= epsilon) || (abe < 0 && bce < 0 && cde < 0 && dae < 0)) // if (A,B,E), (B,C,E), (C,D,E), (D,A,E) all have the same orientation
//		return 1; 
//	return 0;
//}
template<>
L1Item<char[ID_MAX_LENGTH]>::L1Item(char *a) : pNext{ nullptr }
{
	strcpy(data, a);
}
template<>
int L1List<char[ID_MAX_LENGTH]>::insertHead(char *a)
{
	L1Item<char[ID_MAX_LENGTH]> *p = new L1Item<char[ID_MAX_LENGTH]>(a);
	p->pNext = _pHead;
	_pHead = p;
	if (!pTail) pTail = _pHead;
	_size++;
	return 0;
}
//void PointerToGData::findAndPrintListOfNinjaTrapped(PointOnEarth &A, PointOnEarth &B, PointOnEarth &C, PointOnEarth &D) // the trap is the square ABCD
//{
//	auto p{ pLL.getHead() };
//	if (!p) { cout << "-1\n"; return; }
//	PointOnEarth currP, nxtP;
//	L1List<char[ID_MAX_LENGTH]> ninjaTrappedList;
//	while (true)
//	{
//		auto pSubList{ p->data.getHead() };
//		if (pSubList)
//		{
//			if (pSubList->pNext) // multi point
//			{
//				while (pSubList->pNext)
//				{
//					currP.longt = pSubList->data.longitude;
//					currP.lat   = pSubList->data.latitude;
//					nxtP.longt  = pSubList->pNext->data.longitude;
//					nxtP.lat    = pSubList->pNext->data.latitude;
//					if (checkIfAGivenLineSegmentIntersectAGivenSquare(A, B, C, D, currP, nxtP))
//					{
//						char *ch = new char[ID_MAX_LENGTH];
//						strcpy(ch, pSubList->data.id);
//						ninjaTrappedList.insertHead(ch);
//						delete[] ch;
//						break;
//					}
//					pSubList = pSubList->pNext;
//				}
//			}
//			else // single point
//			{
//				// check if a point is inside or on the edge of a square
//				//double maxLongtOfSquare{ A.longt }; // find max longtitude
//				//if (maxLongtOfSquare < B.longt) maxLongtOfSquare = B.longt;
//				//if (maxLongtOfSquare < C.longt) maxLongtOfSquare = C.longt;
//				//if (maxLongtOfSquare < D.longt) maxLongtOfSquare = D.longt;
//				currP.longt = pSubList->data.longitude; // create a segment
//				currP.lat = pSubList->data.latitude; // with this point is a head
//				//nxtP.longt = maxLongtOfSquare; // and the other head is a point on the far right of
//				//nxtP.lat = currP.lat; // the square with the same ordinate 
//				//if (checkIfTowGivenLineSegmentsIntersect(A,B,currP,nxtP) ^ checkIfTowGivenLineSegmentsIntersect(B,C,currP,nxtP) ^ checkIfTowGivenLineSegmentsIntersect(C,D,currP,nxtP) ^ checkIfTowGivenLineSegmentsIntersect(D,A,currP, nxtP)) // if the number of intersect times is odd then the point is inside or on the edge of the square
//				if (checkIfAPointIsInsideOrOnTheEdgeOfAGivenSquare(A, B, C, D, currP))
//				{
//					char *ch = new char[ID_MAX_LENGTH];
//					strcpy(ch, pSubList->data.id);
//					ninjaTrappedList.insertHead(ch);
//					delete[] ch;
//				}
//			}
//		}
//		if (!p->pNext)
//		{
//			if (ninjaTrappedList.isEmpty()) cout << "-1\n";
//			else
//			{
//				do
//				{
//					if (ninjaTrappedList.getHead()->pNext) cout << ninjaTrappedList[0] << ' ';
//					else cout << ninjaTrappedList[0] << '\n';
//					ninjaTrappedList.removeHead();
//				} while (!ninjaTrappedList.isEmpty());
//			}
//			return;
//		}
//		else p = p->pNext;
//	} 
//}

//##########################
template<>
int L1List<char[ID_MAX_LENGTH]>::push_back(char *a)
{
	if (_pHead == NULL) {
		_pHead = new L1Item<char[ID_MAX_LENGTH]>(a);
		pTail = _pHead;
	}
	else {
		pTail->pNext = new L1Item<char[ID_MAX_LENGTH]>(a);
		pTail = pTail->pNext;
	}
	_size++;
	return 0;
}
void PointerToGData::findAndPrintListOfNinjasLost()
{
	auto p{ pLL.getHead() };
	if (!p) { cout << "-1\n"; return; }
	L1List<PointOnEarth> traversingList;
	L1List<char[ID_MAX_LENGTH]> ninjaLostList;
	while (true)
	{
		// go through a data list of a specific ninja
		auto pSubList{ p->data.getHead() };
		if (pSubList)
		{
			PointOnEarth coordinate{ pSubList->data.longitude, pSubList->data.latitude };
			traversingList.insertHead(coordinate);
			pSubList = pSubList->pNext;
			while (pSubList)
			{
				// check whether the point is a moving point
				coordinate.lat = pSubList->data.latitude; coordinate.longt = pSubList->data.longitude;
				if (distanceEarth(coordinate.lat, coordinate.longt, traversingList.getTail()->data.lat, traversingList.getTail()->data.longt) - conventionalUnmvDist >= epsilon)
				{
					// scan traversingList for existence of a extremely close point
					auto pTravL{ traversingList.getHead() };
					auto pTrvLTail{ traversingList.getTail() };
					while (!pTravL->pNext)
					{
						// if found
						if (distanceEarth(pTrvLTail->data.lat, pTrvLTail->data.longt, pTravL->data.lat, pTravL->data.longt) - conventionalUnmvDist < epsilon)
						{
							char *ch = new char[ID_MAX_LENGTH];
							strcpy(ch, pSubList->data.id);
							ninjaLostList.push_back(ch);
							delete[] ch;
							break;
						}
						pTravL = pTravL->pNext;
					}
					traversingList.push_back(coordinate);
				}
				pSubList = pSubList->pNext;
			}
		}
		if (!p->pNext)
		{
			if (ninjaLostList.isEmpty()) cout << "-1\n";
			else
			{
				do
				{
					if (ninjaLostList.getHead()->pNext) cout << ninjaLostList[0] << ' ';
					else cout << ninjaLostList[0] << '\n';
					ninjaLostList.removeHead();
				} while (!ninjaLostList.isEmpty());
			}
			return;
		}
		p = p->pNext;
		traversingList.clean();
	}
}

//template<>
//L1List<char>::L1List(char a) : _pHead{ new L1Item<char>(a) }, pTail{ _pHead }, _size{ 1 }{}
//template<>
//void L1List<char>::insertHead(char a)
//{
//	L1Item<char> *p = new L1Item<char>(a);
//	p->pNext = _pHead;
//	_pHead = p;
//	if (!pTail) pTail = _pHead;
//	_size++;
//	return;
//}



// function handle envent
bool processEvent(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList, void *pGData) 
{
    // TODO: Your code comes here
	try
	{
		char c{ event.code[0] };
		if ( c >= '0' && c <= '9')
		{
			switch (c)
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
				c = event.code[1];
				if (c == 0) // '1'
				{
					cout << event.code << ": " << nList[0].id << '\n';
					return 1;
				}
				else if (c >= '0' && c <= '4') // "1xABCD"
				{
					switch (c)
					{
					case'0': // "10"
						if (event.code[2] == 0)
						{
							cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findMaxTrvlTime() << '\n';
							return 1;
						}
						else
						{
							return 0;
						}
					case'1': // "11XYZT"
						if (event.code[6] == 0)
						{
							const char *ch{ static_cast<PointerToGData*>(pGData)->findAndKill(&(event.code[2])) }; // result return is in heap
							cout << event.code << ": " << ch << '\n';
							if (strcmp(ch,"-1") != 0) delete[] ch;
							return 1;
						}
						else
						{
							return 0;
						}
					case'2': // "12"
						if (event.code[2] == 0)
						{
							cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findMaxUnmovingTime() << '\n';
							return 1;
						}
						else
						{
							return 0;
						}
					case'3': // "13ABCDmnpqEFGHuvrs"
						if (event.code[18] == 0)
						{
							for (int i{ 2 }; i < 18; i++)
								if (!isdigit(event.code[i])) return 0;
							//PointOnEarth A{ 0.0, 0.0 }, B{ 0.0, 0.0 };
							//{
							//	double coordinate[4]{ 0.0, 0.0, 0.0, 0.0 }; //longtA{ 0.0 }, latA{ 0.0 }, longtB{ 0.0 }, latB{ 0.0 }
							//	int lat{ static_cast<int>(nList[0].latitude) };
							//	int longt{ static_cast<int>(nList[0].longitude) };
							//	double coefficient{ 0.1 };
							//	for (int i = 2, k = 0; i < 18; i++)
							//	{
							//		if (isdigit(event.code[i]))
							//		{
							//			coordinate[k] += static_cast<double>(event.code[i] - '0') * coefficient;
							//			if (i % 4 == 1)
							//			{
							//				if (k % 2 == 0)
							//				{
							//					if (longt < 0) coordinate[k] = longt - coordinate[k];
							//					else coordinate[k] += longt;
							//					k++;
							//				}
							//				else
							//				{
							//					if (lat < 0) coordinate[k] = lat - coordinate[k];
							//					else coordinate[k] += lat;
							//					k++;
							//				}
							//				coefficient = 0.1;
							//			}
							//			else coefficient *= 0.1;
							//		}
							//		else return 0;
							//	}
							//	A.longt = coordinate[0]; A.lat = coordinate[1]; B.longt = coordinate[2]; B.lat = coordinate[3];
							//}
							//PointOnEarth C, D;
							//C.longt = (A.longt + A.lat + B.longt - B.lat) * 0.5;
							//C.lat = (A.lat + B.longt + B.lat - A.longt) * 0.5;
							//D.longt = (A.longt + B.longt + B.lat - A.lat) * 0.5;
							//D.lat = (A.longt + A.lat + B.lat - B.longt) * 0.5;
							//cout << event.code << ": ";
							//static_cast<PointerToGData*>(pGData)->findAndPrintListOfNinjaTrapped(A, C, B, D);
							return 1;
						}
						else
						{
							return 0;
						}
					case'4': // "14"
						if (event.code[2] == 0)
						{
							cout << event.code << ": ";
							static_cast<PointerToGData*>(pGData)->findAndPrintListOfNinjasLost();
							return 1;
						}
						else
						{
							return 0;
						}
					default:
						return 0;
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
					cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findMaxID() << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			case'5': // "5ABCD"
				if (event.code[5] == 0)
				{
					const char *strTime{ static_cast<PointerToGData*>(pGData)->findFirstTimeMovingOf(&(event.code[1])) };
					cout << event.code << ": " << strTime << '\n';
					if (strcmp(strTime, "-1")) delete[] strTime;
					return 1;
				}
				else
				{
					return 0;
				}
			case'6': // "6ABCD"
				if (event.code[5] == 0)
				{
					const char *strTime{ static_cast<PointerToGData*>(pGData)->findLastTimeUnmovingOf(&(event.code[1])) };
					cout << event.code << ": " << strTime << '\n';
					if (strcmp(strTime, "-1")) delete[] strTime;
					return 1;
				}
				else
				{
					return 0;
				}
			case'7': // "7ABCD"
				if (event.code[5] == 0)
				{
					cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findNumberOfUnmovingTimesOf(&event.code[1]) << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			case'8': // "8ABCD"
				if (event.code[5] == 0)
				{
					cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findSumOfTravelingDistanceOf(&event.code[1]) << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			case'9': // '9'
				if (event.code[1] == 0)
				{
					cout << event.code << ": " << static_cast<PointerToGData*>(pGData)->findMaxTrvlDist() << '\n';
					return 1;
				}
				else
				{
					return 0;
				}
			default:
				return 0;
			}
		}
		else
		{
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