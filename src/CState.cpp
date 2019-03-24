#include "CState.h"
#include "CMove.h"
#include "level_manager.h"
#include "tile.h"

CState::CState(const Tile& tile)
	: x1(tile.PosX), y1(tile.PosY), x2(tile.PosX), y2(tile.PosY), NeedMoveCheck(false)
{}

void CState::getOrient(int &x, int &y) const {
	x = x2 - x1;
	y = y2 - y1;
}

int CState::getOrientNormSq() const {
	int x, y;
	getOrient(x, y);
	return x*x + y*y;
}

bool CState::isUpright() const {
	return getOrientNormSq() == 0;
}

CState& CState::operator*=(const CMove& move) {
	// calculate orientation
	int xOrient, yOrient;

	getOrient(xOrient, yOrient);

	bool zeroOrient = xOrient == 0 && yOrient == 0;

//	nlinfo("orient %i %i", xOrient, yOrient);

	// calculate orientation*move
	int orientMove = xOrient*move.x + yOrient*move.y;
//	nlinfo("orientMove %i", orientMove);

	// figure out new orientation
	int nxOrient, nyOrient;
	if(orientMove == 0) {
		if(zeroOrient) {
			nxOrient = move.x;
			nyOrient = move.y;
		} else {
			nxOrient = xOrient;
			nyOrient = yOrient;
		}
	} else {
		nxOrient = nyOrient = 0;
	}

//	nlinfo("new orient %i %i", nxOrient, nyOrient);

	// translate
	if(orientMove == 0) {
		x1 += move.x;
		y1 += move.y;
	} else {
		x1 += std::max<int>(orientMove, 0)*move.x + move.x;
		y1 += std::max<int>(orientMove, 0)*move.y + move.y;
	}
//	nlinfo("new pos %i %i", x1, y1);

	// reorient
	x2 = x1 + nxOrient;
	y2 = y1 + nyOrient;

	// Signal the system to check our last move.
	NeedMoveCheck=true;

	return *this;
}

CState CState::operator*(const CMove& move) {
	CState ret(*this);
	ret *= move;
	return ret;
}
