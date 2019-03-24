#include "cube.h"

#include "neloidgame.h"
#include <nel/misc/matrix.h>
#include <nel/3d/transform_shape.h>

#include "CTextureLookup.h"
#include "CMove.h"
#include "CState.h"

#define ROTATE_TIME 0.5f
#define SPAWN_TIME 1.0f

Cube::Cube() {
	m_isAnimating = false;

	// Assign level

	// Assign sounds.

	// Load 3D model.
	m_cubeInstance = NeloidGame::getInstance().scene().createInstance("cuboid.shape");
	if(m_cubeInstance.getObjectPtr() == NULL) {
		nlwarning("Failed to creation uinstance!!");
		return;
	}
	m_cubeInstance.setTransformMode(NL3D::UTransformable::DirectMatrix);
	setVisible(false);
	m_mat = m_cubeInstance.getMaterial(0).getObjectPtr();
	m_cubeInstance.setTransparency(true);

	// Create temp parent for rotation
	m_dummyCube = NeloidGame::getInstance().scene().createInstance("cuboidGroup.shape");
	m_dummyCube.setTransformMode(NL3D::UTransformable::RotQuat);
	m_dummyCube.setPos(0.0f, 0.0f, 0.0f);
	m_dummyCube.setTransparency(true);
	m_dummyCube.hide();

	// Texture lookup used for animation
	m_interp = new CTextureLookup("interp01.tga");

	// Cube stats, good place?
}

Cube::~Cube() {
	NeloidGame::getInstance().scene().deleteInstance(m_cubeInstance);
	NeloidGame::getInstance().scene().deleteInstance(m_dummyCube);
	clearParticles();
	delete m_interp;
}

void Cube::applyMove(const CMove& move) {
	rotateCube(NLMISC::CVector((float)move.x, (float)move.y, 0.f));
}

void Cube::rotateCube(NLMISC::CVector dir) {
	m_dummyCube.setPos(m_cubeInstance.getMatrix().getPos());
	m_dummyCube.setRotQuat(NLMISC::CQuat::Identity);
	NLMISC::CMatrix rotTransform;
	rotTransform.setRot(NLMISC::CVector(0.f, 0.f, (float)NLMISC::Pi/2.f), NLMISC::CMatrix::XYZ);
	NLMISC::CVector axis = rotTransform.mulVector(dir);
	dir.z = -1.f;
	NLMISC::CAABBox bbox;
	m_cubeInstance.getShapeAABBox(bbox);
	bbox = NLMISC::CAABBox::transformAABBox(m_cubeInstance.getMatrix(), bbox);
	NLMISC::CVector half = bbox.getHalfSize();
	NLMISC::CVector pivot(dir.x*half.x, dir.y*half.y, dir.z*half.z);
//	nlinfo("half %f %f %f", half.x, half.y, half.z);
//	nlinfo("dir %f %f %f", dir.x, dir.y, dir.z);
//	nlinfo("pivot %f %f %f", pivot.x, pivot.y, pivot.z);
//	nlinfo("axis %f %f %f", axis.x, axis.y, axis.z);
//	nlinfo("requesting cube rotation.");
	m_cubeInstance.parent(m_dummyCube);
	NLMISC::CMatrix realMatrix = m_cubeInstance.getMatrix();
	realMatrix = m_dummyCube.getMatrix().inverted() * realMatrix;
	m_cubeInstance.setMatrix(realMatrix);

	m_endQuat;
	m_dummyCube.setPivot(pivot);
	m_endQuat.setAngleAxis(axis, (float)NLMISC::Pi/2.f);
//	m_dummyCube.setRotQuat(m_dummyCube.getRotQuat()*m_endQuat);
	m_startQuat = m_dummyCube.getRotQuat();
	startAnimation(Rotate);
}

void Cube::update(double dt) {
	if(m_particles.size() > 0) {
		if(!m_particles.begin()->hasParticles()) {
			NeloidGame::getInstance().scene().deleteInstance(*m_particles.begin());
			m_particles.erase(m_particles.begin());
		}
	}

	if(!isAnimating())
		return;

	switch(m_animation) {
		case Rotate:
			doRotateAnimation();
			break;
		case Spawn:
			doSpawnAnimation();
			break;
		default:
			stopAnimation();
			break;
	}

	m_animTime += dt;
}

void Cube::spawnParticlesAtBase() {
	NLMISC::CMatrix realMatrix = m_cubeInstance.getMatrix();
	float dot = abs(realMatrix.getK() * NLMISC::CVector(0.f, 0.f, 1.f));
	if(dot > 0.5f)
	{
		addParticle("testParticles01.ps", NLMISC::CVector(realMatrix.getPos().x, realMatrix.getPos().y, -1.f));
	}
	else
	{
		addParticle("testParticles01.ps", NLMISC::CVector(realMatrix.getPos().x, realMatrix.getPos().y, -1.f) + (realMatrix.getK()*0.5f));
		addParticle("testParticles01.ps", NLMISC::CVector(realMatrix.getPos().x, realMatrix.getPos().y, -1.f) - (realMatrix.getK()*0.5f));
	}
}

bool Cube::addParticle (const std::string & fileName, const NLMISC::CVector & position)
{
	NL3D::UParticleSystemInstance particle;
	particle.cast(NeloidGame::getInstance().scene().createInstance(fileName));
	if(particle.empty())
	{
		return false;
	}
	particle.setPos(position);
	m_particles.push_back(particle);
	return true;
}

void Cube::doRotateAnimation() {
	if(m_animTime > ROTATE_TIME) {
		animationDone();
		m_dummyCube.setRotQuat(m_startQuat*m_endQuat);
		// Disconnect from the dummy object.
		m_cubeInstance.parent(NULL);
		NLMISC::CMatrix realMatrix = m_cubeInstance.getMatrix();
		realMatrix = m_dummyCube.getMatrix() * realMatrix;
		m_cubeInstance.setMatrix(realMatrix);

		spawnParticlesAtBase();
		return;
	}

	NLMISC::CQuat animQuat = NLMISC::CQuat::slerp(m_startQuat, m_endQuat, m_interp->lookup(0, (float)m_animTime/ROTATE_TIME).R);
	m_dummyCube.setRotQuat(animQuat);
}

void Cube::doSpawnAnimation() {
	if(m_animTime > SPAWN_TIME) {
		m_mat->setOpacity(255);
		spawnParticlesAtBase();
		animationDone();
		return;
	}
	m_mat->setOpacity((uint8)(255*(m_animTime/SPAWN_TIME)));
}

void Cube::startAnimation(Cube::TAnimation animation) {
	setAnimating(true);
	m_animation = animation;
	m_animTime = 0.0;
}

void Cube::stopAnimation() {
	m_cubeInstance.parent(NULL); // just in case
	setAnimating(false);
}

void Cube::animationDone() {
	// just in case we need to do something different here
	stopAnimation();
}

void Cube::setToState(const CState& state) {
	stopAnimation();
	NLMISC::CMatrix mat;

	if(state.isUpright()) {
		mat.setPos(NLMISC::CVector((float)state.x1, (float)state.y1, 0.f));
	} else {
		mat.setPos(NLMISC::CVector((float)(state.x1 + state.x2)/2.f, (float)(state.y1 + state.y2)/2.f, -0.5f));
		int ox, oy;
		state.getOrient(ox, oy);
		NLMISC::CVector orient((float)ox, (float)oy, 0.f);
		NLMISC::CVector axis = orient^NLMISC::CVector::K;
		mat.setRot(NLMISC::CQuat(axis, (float)NLMISC::Pi/2.f));
	}

	m_cubeInstance.setMatrix(mat);
	m_mat->setOpacity(0);
	startAnimation(Spawn);
}

void Cube::setVisible(bool visible) {
	if(visible)
		m_cubeInstance.show();
	else
		m_cubeInstance.hide();
}

bool Cube::isVisible() {
	return m_cubeInstance.getVisibility() == NL3D::UTransform::Hide;
}

void Cube::clearParticles() {
	for(TParticleList::iterator pIter = m_particles.begin(); pIter != m_particles.end(); pIter++) {
		NeloidGame::getInstance().scene().deleteInstance(*pIter);
	}
	m_particles.clear();
}

void Cube::toggleDebugVisibility() {
	if(m_dummyCube.getVisibility() == NL3D::UTransform::Show) {
		m_dummyCube.hide();
	} else {
		m_dummyCube.show();
	}
}