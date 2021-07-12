//
//  ofxPolyline.hpp
//
//  Created by Ben Snell
//
//

#pragma once
#include "ofMain.h"

enum OFXPOLYLINE_MODE {
	OFXPOLYLINE_INVALID = -1,
	OFXPOLYLINE_LINEAR,
	OFXPOLYLINE_SPLINE,
	OFXPOLYLINE_NUM
};

// This class builds upon the ofPolyline to add additional functionality to the curveTo
// function, allowing the polyline to switch between linear and curved modes.
// Not all ofPolyline functions have been adapted for use with this function.
class ofxPolyline : protected ofPolyline {
public:

	// Set the mode of this polyline.
	void setMode(OFXPOLYLINE_MODE _mode);
	void cycleMode();

	// Add a vertex to the polyline. Instead of using line/curve-to functions,
	// a universal addVertex function is adopted. Resolution is only used for
	// the spline mode.
	// There must be at least 2 to be valid.
	void add(glm::vec3 vertex, int resolution = 20);
	void insert(glm::vec3 vertex, int index, int resolution = 20);
	bool isValid() { return controlPoints.size() >= 2; }

	size_t size();
	void close();
	void setClosed(bool tf);
	void cycleClosed() { return setClosed(!isClosed()); }
	void clear();
	void flagHasChanged();

	glm::vec3 getClosestPoint(const glm::vec3& target, unsigned int* nearestIndex);
	glm::vec3 getClosestPoint(const glm::vec3& target, float* findex);

	glm::vec3 getPointAtIndexInterpolated(float findex);
	glm::vec3 getPointAtLength(float f);
	glm::vec3 getPointAtPercent(float f);

	float getLengthAtIndex(int index);
	float getLengthAtIndexInterpolated(float findex);

	float getIndexAtLength(float f);
	float getIndexAtPercent(float f);

	vector<glm::vec3> getVertices() { return controlPoints; }
	vector<glm::vec3> getSubVertices() { return ofPolyline::getVertices(); }

	// Passthrough functions
	float getPerimeter() { return ofPolyline::getPerimeter(); }
	void draw() { return ofPolyline::draw(); }
	bool isClosed() { return ofPolyline::isClosed(); }

	// Many functions not listed here are incompatible with this object
	// or have not yet been implemented.

private:

	// Mode of the polyline
	OFXPOLYLINE_MODE mode = OFXPOLYLINE_LINEAR;

	// These are the foundational points on which the curve is built
	vector<glm::vec3> controlPoints;

	// Resolution of the spline.
	// This is the number of points needed to represent every control point.
	int splineResolution = 20;

	// Build the spline, using the control points
	void build();
	void buildLinear();
	void buildSpline();

	// Taken from here: https://forum.openframeworks.cc/t/ofpolyline-interpolated-index-of-closest-point/24462
	glm::vec3 getLerpIndexAtClosestPoint(const ofPolyline& polyline,
		const glm::vec3& target, float* nearestIndex);
};