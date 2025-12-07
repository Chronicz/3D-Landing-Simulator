
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
#include <limits>
#include <float.h>
 


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
// FIXED: Properly initialize min/max to handle negative coordinates
//
Box Octree::meshBounds(const ofMesh & mesh) {
	int n = (int)mesh.getNumVertices();
	
	if (n == 0) {
		cout << "WARNING: Empty mesh in meshBounds()" << endl;
		return Box(Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
	
	// FIX #2: Correct bounding box initialization
	// Initialize min to FLT_MAX and max to -FLT_MAX to properly handle negative coordinates
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
	
	for (int i = 0; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);
		
		// Use min/max functions to properly clamp bounds
		minX = (v.x < minX) ? v.x : minX;
		maxX = (v.x > maxX) ? v.x : maxX;
		
		minY = (v.y < minY) ? v.y : minY;
		maxY = (v.y > maxY) ? v.y : maxY;
		
		minZ = (v.z < minZ) ? v.z : minZ;
		maxZ = (v.z > maxZ) ? v.z : maxZ;
	}
	
	// Validate min <= max for all axes
	if (minX > maxX || minY > maxY || minZ > maxZ) {
		cout << "ERROR: Invalid bounding box - min > max detected!" << endl;
		cout << "  min: (" << minX << ", " << minY << ", " << minZ << ")" << endl;
		cout << "  max: (" << maxX << ", " << maxY << ", " << maxZ << ")" << endl;
		// Swap if needed
		if (minX > maxX) { float temp = minX; minX = maxX; maxX = temp; }
		if (minY > maxY) { float temp = minY; minY = maxY; maxY = temp; }
		if (minZ > maxZ) { float temp = minZ; minZ = maxZ; maxZ = temp; }
	}
	
	// Debug output
	cout << "Mesh bounds calculation:" << endl;
	cout << "  Vertices: " << n << endl;
	cout << "  Min: (" << minX << ", " << minY << ", " << minZ << ")" << endl;
	cout << "  Max: (" << maxX << ", " << maxY << ", " << maxZ << ")" << endl;
	cout << "  Size: (" << (maxX - minX) << ", " << (maxY - minY) << ", " << (maxZ - minZ) << ")" << endl;
	
	return Box(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
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

	//  generate ground floor - standard octree subdivision order
	//  Bottom layer (y: min to center), then top layer (y: center to max)
	//  Each layer: left-front, right-front, right-back, left-back
	//
	Box b[8];
	
	// Bottom layer
	b[0] = Box(min, center);  // 0: bottom-left-front
	b[1] = Box(
		Vector3(center.x(), min.y(), min.z()),
		Vector3(max.x(), center.y(), center.z())
	);  // 1: bottom-right-front
	b[2] = Box(
		Vector3(center.x(), min.y(), center.z()),
		Vector3(max.x(), center.y(), max.z())
	);  // 2: bottom-right-back
	b[3] = Box(
		Vector3(min.x(), min.y(), center.z()),
		Vector3(center.x(), center.y(), max.z())
	);  // 3: bottom-left-back

	boxList.clear();
	for (int i = 0; i < 4; i++)
		boxList.push_back(b[i]);

	// generate second story (top layer)
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
	if (level >= numLevels) return;
	
	// Stop if this node has 1 or fewer points (leaf node)
	if (node.points.size() <= 1) return;

	// 1) Subdivide box in node into 8 equal side boxes
	vector<Box> childBoxes;
	subDivideBox8(node.box, childBoxes);

	// 2) For each child box, sort point data into each box
	for (int i = 0; i < childBoxes.size(); i++) {
		vector<int> pointsInBox;
		getMeshPointsInBox(mesh, node.points, childBoxes[i], pointsInBox);
		
		// If a child box contains at least 1 point, add child to tree
		if (pointsInBox.size() > 0) {
			TreeNode child;
			child.box = childBoxes[i];
			child.points = pointsInBox;
			
			// If child is not a leaf node (contains more than 1 point), recursively call subdivide
			if (pointsInBox.size() > 1 && level + 1 < numLevels) {
				subdivide(mesh, child, numLevels, level + 1);
			}
			
			node.children.push_back(child);
		}
	}
}

// Implement functions below for Homework project
//

bool Octree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {
	// First check if the ray intersects the current node's box
	if (!node.box.intersect(ray, 0, 10000)) {
		return false;
	}
	
	// If this is a leaf node (no children or only 1 point), return this node
	if (node.children.empty() || node.points.size() <= 1) {
		nodeRtn = node;
		return true;
	}
	
	// Otherwise, recursively check children
	// We want to find the closest intersection, so check all children
	TreeNode closestNode;
	bool found = false;
	float closestDist = std::numeric_limits<float>::max();
	
	for (int i = 0; i < node.children.size(); i++) {
		TreeNode childResult;
		if (intersect(ray, node.children[i], childResult)) {
			// Calculate distance to the first point in the child node
			if (!childResult.points.empty()) {
				ofVec3f point = mesh.getVertex(childResult.points[0]);
				Vector3 pointVec(point.x, point.y, point.z);
				Vector3 toPoint = pointVec - ray.origin;
				float dist = toPoint.length();
				
				if (!found || dist < closestDist) {
					closestNode = childResult;
					closestDist = dist;
					found = true;
				}
			}
		}
	}
	
	if (found) {
		nodeRtn = closestNode;
		return true;
	}
	
	// If no children intersected but this box did, return this node
	nodeRtn = node;
	return true;
}

bool Octree::intersect(const Box &box, TreeNode & node, vector<Box> & boxListRtn) {
	bool intersects = false;
	return intersects;
}

void Octree::draw(TreeNode & node, int numLevels, int level, const vector<ofColor> *colors) {
	// Only draw if we're at or below the requested number of levels
	if (level >= numLevels) return;
	
	// Set color based on level if colors are provided
	if (colors && level < colors->size()) {
		ofSetColor((*colors)[level]);
	} else {
		// Default to white if no colors provided or level exceeds color array
		ofSetColor(ofColor::white);
	}
	
	// Draw the current node's box
	drawBox(node.box);
	
	// Recursively draw all children
	for (int i = 0; i < node.children.size(); i++) {
		draw(node.children[i], numLevels, level + 1, colors);
	}
}

// Optional
//
void Octree::drawLeafNodes(TreeNode & node) {


}




