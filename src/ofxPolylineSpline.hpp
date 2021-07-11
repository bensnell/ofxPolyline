//
//  ofxPolylineSpline.hpp
//
//  Created by Ben Snell
//
//

#pragma once
#include "ofMain.h"

// This class behaves similarly to ofPolylines, allowing for interpolation, etc.
// Not all ofPolyline functions have been adapted for use with this function.
class ofxPolylineSpline : protected ofPolyline {
public:
	
	// Add a vertex to the spline
	// There must be at least 2 to be valid
	void splineTo(glm::vec3 vertex, int resolution);
	bool isSplineValid() { return controlPoints.size() >= 2; }

	size_t size();
	void close();
	void setClosed(bool tf);
	void clear();
	void flagHasChanged();

	glm::vec3 getClosestPoint(const glm::vec3& target, unsigned int* nearestIndex);
	glm::vec3 getClosestPoint(const glm::vec3& target, float* findex);

	glm::vec3 getPointAtIndexInterpolated(float findex);

	float getLengthAtIndex(int index);
	float getLengthAtIndexInterpolated(float findex);

	float getIndexAtLength(float f);
	float getIndexAtPercent(float f);

	vector<glm::vec3> getVertices() { return controlPoints; }
	vector<glm::vec3> getSubVertices() { return ofPolyline::getVertices(); }

	glm::vec3 getPointAtLength(float f);
	glm::vec3 getPointAtPercent(float f);

	// Passthrough functions
	float getPerimeter() { return ofPolyline::getPerimeter(); }
	void draw() { return ofPolyline::draw(); }
	bool isClosed() { return ofPolyline::isClosed(); }

	// Many functions not listed here are incompatible with this object
	// or have not yet been implemented.

private:

	// These are the foundational points on which the curve is built
	vector<glm::vec3> controlPoints;

	// Resolution of the spline.
	// This is the number of points needed to represent every control point.
	int splineResolution = 20;

	// Build the spline, using the control points
	void buildSpline();

	// Taken from here: https://forum.openframeworks.cc/t/ofpolyline-interpolated-index-of-closest-point/24462
	glm::vec3 ofxPolylineSpline::getLerpIndexAtClosestPoint(const ofPolyline& polyline, 
		const glm::vec3& target, float* nearestIndex);
};