
// Kevin M.Smith - CS 134 SJSU

#include "Util.h"




//---------------------------------------------------------------
// test if a ray intersects a plane.  If there is an intersection, 
// return true and put point of intersection in "point"
//
bool rayIntersectPlane(const ofVec3f &rayPoint, const ofVec3f &raydir, const ofVec3f &planePoint,
	const ofVec3f &planeNorm, ofVec3f &point)
{
	// if d1 is 0, then the ray is on the plane or there is no intersection
	//
	const float eps = .000000001;
	float d1 = (planePoint - rayPoint).dot(planeNorm);
	if (abs(d1) < eps) return false;

	//  if d2 is 0, then the ray is parallel to the plane
	//
	float d2 = raydir.dot(planeNorm);
	if (abs(d2) < eps) return false;

	//  compute the intersection point and return it in "point"
	//
	point = (d1 / d2) * raydir + rayPoint;
	return true;
}

// Compute the reflection of a vector incident on a surface at the normal.
// 
//
ofVec3f reflectVector(const ofVec3f &v, const ofVec3f &n) {
	return (v - 2 * v.dot(n) * n);
}

//  added by ronald - ray-box intersection test
bool rayIntersectBox(const ofVec3f & rayStart,
	const ofVec3f & rayEnd,
	const ofVec3f & boxMin,
	const ofVec3f & boxMax,
	float * hitDist) {
	ofVec3f dir = rayEnd - rayStart;

	// Avoid division by zero
	ofVec3f invDir(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);

	float t1 = (boxMin.x - rayStart.x) * invDir.x;
	float t2 = (boxMax.x - rayStart.x) * invDir.x;
	float t3 = (boxMin.y - rayStart.y) * invDir.y;
	float t4 = (boxMax.y - rayStart.y) * invDir.y;
	float t5 = (boxMin.z - rayStart.z) * invDir.z;
	float t6 = (boxMax.z - rayStart.z) * invDir.z;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// No intersection
	if (tmax < 0) return false;
	if (tmin > tmax) return false;

	if (hitDist) *hitDist = tmin;

	return true;
}
