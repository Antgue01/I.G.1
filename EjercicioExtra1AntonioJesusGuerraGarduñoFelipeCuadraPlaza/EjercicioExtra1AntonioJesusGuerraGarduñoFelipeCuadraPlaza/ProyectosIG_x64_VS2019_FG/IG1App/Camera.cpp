#include "Camera.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>

using namespace glm;

//-------------------------------------------------------------------------

Camera::Camera(Viewport* vp) : mViewPort(vp), mViewMat(1.0), mProjMat(1.0),
xRight(vp->width() / 2.0), xLeft(-xRight),
yTop(vp->height() / 2.0), yBot(-yTop)
{
	setPM();
}
//-------------------------------------------------------------------------

void Camera::uploadVM() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mViewMat)); // transfers view matrix to the GPU 
}
//-------------------------------------------------------------------------

void Camera::setVM()
{

	mViewMat = lookAt(mEye, mLook, mUp);  // glm::lookAt defines the view matrix 
	SetAxes();
}
//-------------------------------------------------------------------------

void Camera::set2D()
{
	mEye = dvec3(0, 0, 500);
	mLook = dvec3(0, 0, 0);
	mUp = dvec3(0, 1, 0);
	mAng = 270;
	setVM();
}
//-------------------------------------------------------------------------

void Camera::set3D()
{
	mLook = dvec3(0, 10, 0);
	mAng = -45;
	mEye = dvec3(mLook.x + cos(radians(mAng)) * mRadio, 500, mLook.z - sin(radians(mAng)) * mRadio);
	mUp = dvec3(0, 1, 0);
	setVM();
}
//-------------------------------------------------------------------------

void Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}
//-------------------------------------------------------------------------

void Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}
//-------------------------------------------------------------------------

void Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
}
//-------------------------------------------------------------------------

void Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0) mScaleFact = 0.01;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mNearVal = 1;
		mProjMat = ortho(xLeft * mScaleFact, xRight * mScaleFact, yBot * mScaleFact, yTop * mScaleFact, mNearVal, mFarVal);
	}// glm::ortho defines the orthogonal projection matrix
	else {		//if perspective projection
		mNearVal = 2 * yTop;
		mProjMat = frustum(xLeft * mScaleFact, xRight * mScaleFact, yBot * mScaleFact, yTop * mScaleFact, mNearVal, mFarVal);

	}//glm::frustum defines the perspective projection matrix

}
//-------------------------------------------------------------------------

void Camera::uploadPM() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(mProjMat)); // transfers projection matrix to the GPU
	glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------------------------------
void Camera::moveLR(GLdouble cs) {
	mEye += (dvec3)(row(mViewMat, 0)) * cs;
	mLook += (dvec3)(row(mViewMat, 0)) * cs;
	setVM();
}
void Camera::moveUD(GLdouble cs) {
	mEye += (dvec3)(row(mViewMat, 1)) * cs;
	mLook += (dvec3)(row(mViewMat, 1)) * cs;
	setVM();
}
void Camera::moveFB(GLdouble cs) {
	dvec3 aux = mLook - mEye;
	mEye += aux * cs;
	mLook += aux * cs;

	setVM();
}
void Camera::SetAxes() {
	mRight = row(mViewMat, 0);
	mUp = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);

}
void Camera::orbit(GLdouble incAng, GLdouble incY) {

	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}
void Camera::changePrj() {

	bOrto = !bOrto;
	setPM();
}
void Camera::setCenital() {
	mEye = { 0,1000,0 };
	mLook = { 0,0,0 };
	mUp = { -1,0,0 };
	setVM();
}

