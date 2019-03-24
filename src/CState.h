#ifndef NL_CSTATE_H
#define NL_CSTATE_H

class CMove;
class Tile;

class CState {
public:
	int x1, y1;
	int x2, y2;
	bool NeedMoveCheck;

	CState()
		: x1(0), y1(0), x2(0), y2(0), NeedMoveCheck(false)
	{};
	CState(int n_x1, int n_y1, int n_x2, int n_y2)
		: x1(n_x1), y1(n_y1), x2(n_x2), y2(n_y2), NeedMoveCheck(false)
	{};
	CState(const CState& state)
		: x1(state.x1), y1(state.y1), x2(state.x2), y2(state.y2), NeedMoveCheck(state.NeedMoveCheck)
	{};
	CState(const Tile& tile);
	~CState() {};

	int getOrientNormSq() const;
	void getOrient(int &x, int &y) const;

	bool isUpright() const;

	CState& operator*=(const CMove& move);
	CState operator*(const CMove& move);

	
};

#endif // NL_CSTATE_H
