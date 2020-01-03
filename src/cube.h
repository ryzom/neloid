#ifndef NL_CUBE_H
#define NL_CUBE_H

#include <string>
#include <vector>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>

#include <nel/misc/vector.h>

#include <nel/3d/u_instance.h>
#include <nel/3d/u_particle_system_instance.h>
#include <nel/3d/material.h>

class CMove;
class CState;
class CTextureLookup;

class Cube {
public:
	typedef std::vector<NL3D::UParticleSystemInstance> TParticleList;

	enum TAnimation {
		Rotate,
		Spawn,
		Fall,
		Win
	};

private:
	// Animation stuff
	double			m_animTime;
	NLMISC::CQuat	m_startQuat;
	NLMISC::CQuat	m_endQuat;
	CTextureLookup* m_interp;
	TAnimation		m_animation;
	bool			m_isAnimating;

	TParticleList	m_particles;

	NL3D::UInstance m_dummyCube;
	NL3D::CMaterial* m_mat;

	void rotateCube(NLMISC::CVector dir);

	void setAnimating(bool animating) { m_isAnimating = animating; }
	void animationDone();
	void doRotateAnimation();
	void doSpawnAnimation();
	void spawnParticlesAtBase();
	bool addParticle(const std::string & fileName, const NLMISC::CVector & position);

	void startAnimation(TAnimation animation);

public:
	NL3D::UInstance m_cubeInstance;

	Cube();
	~Cube();

	void applyMove(const CMove& move);
	void setToState(const CState& state);

//	TODO
//	void win(); // trigger the win animation after rotate
//	void fall(a tile or something...we need to know the edge of the level); // triggers the fall animation after rotate

	bool isAnimating() { return m_isAnimating; }
	void stopAnimation();

	void update(double deltaTime);

	void setVisible(bool visible);
	bool isVisible();

	void clearParticles();
	void toggleDebugVisibility();
	NLMISC::CVector getDummyPos();
};

#endif // NL_CUBE_H