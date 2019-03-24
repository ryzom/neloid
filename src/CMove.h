#ifndef NL_CMOVE_H
#define NL_CMOVE_H

/// Represents a basic relative move.
class CMove {
public:
	int x, y;

	static const CMove xpos;
	static const CMove xneg;
	static const CMove ypos;
	static const CMove yneg;

	CMove(int n_x, int n_y) : x(n_x), y(n_y) {};
	~CMove() {};

	bool isReverse (const CMove& move) const {
		return x == -move.x && y == -move.y;
	};
};

#endif // NL_CMOVE_H
