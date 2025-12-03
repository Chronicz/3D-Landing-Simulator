
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Simple Octree Implementation 11/10/2020
// 
//  Copyright (c) by Kevin M. Smith
//  Copying or use without permission is prohibited by law. 
//


#include "Octree.h"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <cfloat>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <cfloat>
 


//draw a box from a "Box" class  
//
void Octree::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box Octree::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	cout << "vertices: " << n << endl;
//	cout << "min: " << min << "max: " << max << endl;
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained 
//                      inside the Box.  Return count of points found;
//
int Octree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
	Box & box, vector<int> & pointsRtn)
{
	int count = 0;
	for (int i = 0; i < points.size(); i++) {
		ofVec3f v = mesh.getVertex(points[i]);
		if (box.inside(Vector3(v.x, v.y, v.z))) {
			count++;
			pointsRtn.push_back(points[i]);
		}
	}
	return count;
}

// getMeshFacesInBox:  return an array of indices to Faces in mesh that are contained 
//                      inside the Box.  Return count of faces found;
//
int Octree::getMeshFacesInBox(const ofMesh & mesh, const vector<int>& faces,
	Box & box, vector<int> & facesRtn)
{
	int count = 0;
	for (int i = 0; i < faces.size(); i++) {
		ofMeshFace face = mesh.getFace(faces[i]);
		ofVec3f v[3];
		v[0] = face.getVertex(0);
		v[1] = face.getVertex(1);
		v[2] = face.getVertex(2);
		Vector3 p[3];
		p[0] = Vector3(v[0].x, v[0].y, v[0].z);
		p[1] = Vector3(v[1].x, v[1].y, v[1].z);
		p[2] = Vector3(v[2].x, v[2].y, v[2].z);
		if (box.inside(p,3)) {
			count++;
			facesRtn.push_back(faces[i]);
		}
	}
	return count;
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void Octree::subDivideBox8(const Box &box, vector<Box> & boxList) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	float xdist = (max.x() - min.x()) / 2;
	float ydist = (max.y() - min.y()) / 2;
	float zdist = (max.z() - min.z()) / 2;
	Vector3 h = Vector3(0, ydist, 0);

	//  generate ground floor
	//
	Box b[8];
	b[0] = Box(min, center);
	b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
	b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
	b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));

	boxList.clear();
	for (int i = 0; i < 4; i++)
		boxList.push_back(b[i]);

	// generate second story
	//
	for (int i = 4; i < 8; i++) {
		b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
		boxList.push_back(b[i]);
	}
}

void Octree::create(const ofMesh & geo, int numLevels) {
	// initialize octree structure
	//
	mesh = geo;
	int level = 0;
	root.box = meshBounds(mesh);
	if (!bUseFaces) {
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			root.points.push_back(i);
		}
	}
	else {
		// need to load face vertices here
		//
		for (int i = 0; i < mesh.getNumIndices()/3; i++) {
			root.points.push_back(i);
		}
	}

	// recursively buid octree
	//
	level++;
    subdivide(mesh, root, numLevels, level);
}


//
// subdivide:  recursive function to perform octree subdivision on a mesh
//
//  subdivide(node) algorithm:
//     1) subdivide box in node into 8 equal side boxes - see helper function subDivideBox8().
//     2) For each child box
//            sort point data into each box  (see helper function getMeshFacesInBox())
//        if a child box contains at list 1 point
//            add child to tree
//            if child is not a leaf node (contains more than 1 point)
//               recursively call subdivide(child)
//         
//      
             
void Octree::subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level) {
	//stop condition
	if (level >= numLevels) return;

	// subdvide algorithm implemented here
	//divide this node's box into 8 smaller ones
	vector<Box> childBoxes;
	subDivideBox8(node.box, childBoxes); //use helper function given

	//for each child box, get points that are inside it
	for (int i = 0; i < childBoxes.size(); i++) {
		vector<int> pointsInChild;
		int count = getMeshPointsInBox(mesh, node.points, childBoxes[i], pointsInChild); //use helper function given
		// remember from class that we didn't need to use getmeshfacesinbox method for this particular project

		//if at least one point is in the child box, create a new child node
		if (count > 0) {
			TreeNode childNode;
			childNode.box = childBoxes[i];
			childNode.points = pointsInChild;

			//if child node has more than one point, subdivide it further then add child to parent node
			if (count > 1) {
				subdivide(mesh, childNode, numLevels, level + 1);
			}
			node.children.push_back(childNode);

		}
	}
}

// Implement functions below for Homework project
//

bool Octree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {
	bool intersects = false;

	if (node.box.intersect(ray, 0, 10000)) { //check if ray intersects node box
		//if leaf node, return this node
		if (node.children.empty()) {
			nodeRtn = node;
			return true;
		}
		else { //else recursively check node's children
			for (int i = 0; i < node.children.size(); i++) {
				if (intersect(ray, node.children[i], nodeRtn)) {
					intersects = true;
					break; //stop at first intersection
				}
			}
		}
	}


	return intersects;
}

bool Octree::intersect(const Box &box, TreeNode & node, vector<Box> & boxListRtn) {
	bool intersects = false;

	if (!node.box.overlap(box)) {
		return intersects; //false, no intersection
	} else {
		//if leaf node, add this box to return list
		if (node.children.size() == 0) {
			boxListRtn.push_back(node.box);
			intersects = true;
			return intersects;
		} else { //else check children
			for (int i = 0; i < node.children.size(); i++) {
				if (intersect(box, node.children[i], boxListRtn)) {
					intersects = true;
				}
			}
		}

		return intersects;
	}
}

void Octree::draw(TreeNode & node, int numLevels, int level) {
	if (level > numLevels) return; //stop condition - only draws up to specified level based on the slider
	// draw this node's box
	drawBox(node.box);

	// recursively draw node's children
	for (int i = 0; i < node.children.size(); i++) {
		draw(node.children[i], numLevels, level + 1);
	}
}

// Optional
//
void Octree::drawLeafNodes(TreeNode & node) {


}

// Ray-triangle intersection using Möller-Trumbore algorithm
// Returns true if ray intersects a triangle in the mesh, and fills in intersectionPoint, normal, and distance
bool Octree::intersectRayMesh(const Ray &ray, const TreeNode &node, glm::vec3 &intersectionPoint, glm::vec3 &normal, float &distance) {
	bool foundIntersection = false;
	float closestDistance = FLT_MAX;
	glm::vec3 closestPoint;
	glm::vec3 closestNormal;

	// Check if ray intersects this node's bounding box first
	if (!node.box.intersect(ray, 0, 10000)) {
		return false;
	}

	// If this is a leaf node, check triangles
	if (node.children.empty()) {
		// Check all triangles in the mesh that might be in this node
		// We'll check all triangles and see if they intersect the ray and are in this node's box
		for (int j = 0; j < mesh.getNumIndices(); j += 3) {
			int idx0 = mesh.getIndex(j);
			int idx1 = mesh.getIndex(j + 1);
			int idx2 = mesh.getIndex(j + 2);

			// Get triangle vertices
			ofVec3f v0 = mesh.getVertex(idx0);
			ofVec3f v1 = mesh.getVertex(idx1);
			ofVec3f v2 = mesh.getVertex(idx2);

			// Check if triangle is in this node's box (at least one vertex)
			bool inBox = false;
			for (int k = 0; k < node.points.size(); k++) {
				if (node.points[k] == idx0 || node.points[k] == idx1 || node.points[k] == idx2) {
					inBox = true;
					break;
				}
			}
			if (!inBox) continue;

			// Convert to glm::vec3
			glm::vec3 p0(v0.x, v0.y, v0.z);
			glm::vec3 p1(v1.x, v1.y, v1.z);
			glm::vec3 p2(v2.x, v2.y, v2.z);

			// Möller-Trumbore ray-triangle intersection
			glm::vec3 edge1 = p1 - p0;
			glm::vec3 edge2 = p2 - p0;
			glm::vec3 rayDir = glm::vec3(ray.direction.x(), ray.direction.y(), ray.direction.z());
			glm::vec3 h = glm::cross(rayDir, edge2);
			float a = glm::dot(edge1, h);

			if (abs(a) < 0.0001f) continue; // Ray is parallel to triangle

			float f = 1.0f / a;
			glm::vec3 s = glm::vec3(ray.origin.x(), ray.origin.y(), ray.origin.z()) - p0;
			float u = f * glm::dot(s, h);

			if (u < 0.0f || u > 1.0f) continue;

			glm::vec3 q = glm::cross(s, edge1);
			float v = f * glm::dot(rayDir, q);

			if (v < 0.0f || u + v > 1.0f) continue;

			float t = f * glm::dot(edge2, q);

			if (t > 0.0001f && t < closestDistance) {
				closestDistance = t;
				closestPoint = glm::vec3(ray.origin.x(), ray.origin.y(), ray.origin.z()) + rayDir * t;
				closestNormal = glm::normalize(glm::cross(edge1, edge2));
				foundIntersection = true;
			}
		}
	} else {
		// Recursively check children
		for (int i = 0; i < node.children.size(); i++) {
			glm::vec3 childPoint;
			glm::vec3 childNormal;
			float childDistance;
			if (intersectRayMesh(ray, node.children[i], childPoint, childNormal, childDistance)) {
				if (childDistance < closestDistance) {
					closestDistance = childDistance;
					closestPoint = childPoint;
					closestNormal = childNormal;
					foundIntersection = true;
				}
			}
		}
	}

	if (foundIntersection) {
		intersectionPoint = closestPoint;
		normal = closestNormal;
		distance = closestDistance;
	}

	return foundIntersection;
}




