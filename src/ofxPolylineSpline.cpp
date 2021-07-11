//
//  ofxPolylineSpline.cpp
//
//  Created by Ben Snell
//
//

#include "ofxPolylineSpline.hpp"

// ----------------------------------------------------------------------------
void ofxPolylineSpline::splineTo(glm::vec3 vertex, int resolution) {

	// Add this control point
	controlPoints.push_back(vertex);
	// Use this resolution for all new points
	splineResolution = resolution;

	buildSpline();
}

// ----------------------------------------------------------------------------
void ofxPolylineSpline::buildSpline() {

	// If we have enough control points, then rebuild the curve
	if (!isSplineValid()) return;
	
	// Clear all points in the underlying polyline
	bool _isClosed = isClosed();
	ofPolyline::clear();
	ofPolyline::setClosed(_isClosed);

	// There's a bug on OF ofPolyline.inl line 301 where the sub-points are 1- instead of
	// 0-indexed. This means, we have to add an extra point for non-closed shapes in
	// order to get them to start at the starting point and be indexed as 0 at this point.
	if (!isClosed()) lineTo(controlPoints.front());

	// Add an invisible handle for the first point. This handle depends on
	// whether or not the curve is closed.
	auto handleFirst = isClosed() ? controlPoints.back() : controlPoints.front(); // todo: use projection if open
	curveTo(handleFirst, splineResolution);

	// Then, add all control points
	for (auto& point : controlPoints) curveTo(point, splineResolution);

	// If the polyline is closed, then add an additional handle for the 
	// first control point
	if (isClosed()) curveTo(controlPoints.front(), splineResolution);

	// Finally, add an invisible last handle. This handle is similar to handle00.
	auto handleLast = isClosed() ? controlPoints[1] : controlPoints.back();
	curveTo(handleLast, splineResolution);
}

// ----------------------------------------------------------------------------
void ofxPolylineSpline::clear() {

	controlPoints.clear();
	ofPolyline::clear();
	buildSpline();
}

// ----------------------------------------------------------------------------
void ofxPolylineSpline::close() {

	ofPolyline::close();
	buildSpline();
}

// ----------------------------------------------------------------------------
void ofxPolylineSpline::setClosed(bool tf) {

	ofPolyline::setClosed(tf);
	buildSpline();
}

// ----------------------------------------------------------------------------
void ofxPolylineSpline::flagHasChanged() {
	buildSpline(); // this will also flag the polyline as changed
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getClosestPoint(const glm::vec3& target, 
	unsigned int* nearestIndex) {

	unsigned int _nearestIndex;
	glm::vec3 closestPoint = ofPolyline::getClosestPoint(target, &_nearestIndex);
	*nearestIndex = size_t(round(float(_nearestIndex) / float(size()))) % size();
	return closestPoint;
}

// ----------------------------------------------------------------------------
size_t ofxPolylineSpline::size() {

	// Don't return the actual number of points in the underlying ofPolyline,
	// since these are used only to approximate a curve. Instead, return the 
	// foundational controlPoints of the ideal curve.
	return controlPoints.size();
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getClosestPoint(const glm::vec3& target, float* findex) {

	float nearestIndex;
	glm::vec3 closestPoint = getLerpIndexAtClosestPoint(*this, target, &nearestIndex);

	// The returned nearestIndex exists in the range [0, size()*splineResolution).
	// We want it in the range of the controlPoints from [0, size()).
	// In order to achieve this, we have to map between two sets of indices. This
	// assumes that spline resolution sub-points exist at an equal interval between any given 
	// set of two adjacent control points. Thus, the interpolated index indicates:
	// (1) The control points above and below, via floor(findex) and ceil(findex)%size(), and
	// (2) The sub-points above and below, via floor(ffindex) and ceil(ffindex), where
	// ffindex = fmod(findex,1)*splineResolution, and 
	// (3) The percent this point is between the neighboring sub-points, via fmod(ffindex,1).

	*findex = nearestIndex / float(splineResolution);

	return closestPoint;
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getLerpIndexAtClosestPoint(const ofPolyline& polyline, 
	const glm::vec3& target, float* nearestIndex) {
	if (polyline.size() < 2) {
		if (nearestIndex != nullptr) {
			nearestIndex = 0;
		}
		return target;
	}

	float distance = 0;
	glm::vec3 nearestPoint;
	unsigned int nearest = 0;
	float normalizedPosition = 0;
	unsigned int lastPosition = polyline.size() - 1;
	if (polyline.isClosed()) {
		lastPosition++;
	}
	for (int i = 0; i < (int)lastPosition; i++) {
		bool repeatNext = i == (int)(polyline.size() - 1);

		const auto& cur = polyline[i];
		const auto& next = repeatNext ? polyline[0] : polyline[i + 1];

		float curNormalizedPosition = 0;
		auto curNearestPoint = getClosestPointUtil(cur, next, target, &curNormalizedPosition);
		float curDistance = glm::distance(toGlm(curNearestPoint), toGlm(target));
		if (i == 0 || curDistance < distance) {
			distance = curDistance;
			nearest = i;
			nearestPoint = curNearestPoint;
			normalizedPosition = curNormalizedPosition;
		}
	}

	if (nearestIndex != nullptr) {
		*nearestIndex = nearest + normalizedPosition;
	}

	return nearestPoint;
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getPointAtIndexInterpolated(float findex) {

	// If the polyline is closed, offset the index by one to account for
	// ofPolyline's implementation of 1-indexed subvertices.
	return ofPolyline::getPointAtIndexInterpolated(
		findex * float(splineResolution) + (isClosed() ? -1 : 0));
}

// ----------------------------------------------------------------------------
float ofxPolylineSpline::getLengthAtIndex(int index) {

	return ofPolyline::getLengthAtIndex(
		index * splineResolution);
}

// ----------------------------------------------------------------------------
float ofxPolylineSpline::getLengthAtIndexInterpolated(float findex) {

	// First, wrap the index
	findex = fmod(findex + ceil(abs(findex / float(size()))) * float(size()), float(size()));
	
	// Closed polylines don't calculate length correctly between the last and first subpoint,
	// so adjust for this beyond the boundary line.
	float indexAtBoundary = float(size()) - 1.0 / float(splineResolution);
	float length = ofPolyline::getLengthAtIndexInterpolated(
		findex * float(splineResolution));
	if (isClosed() && findex > indexAtBoundary) {
		float lengthAtBoundary = getLengthAtIndexInterpolated(indexAtBoundary);
		length = ofMap(length, lengthAtBoundary, 0, lengthAtBoundary, getPerimeter(), true);
	}
	return length;
}

// ----------------------------------------------------------------------------
float ofxPolylineSpline::getIndexAtPercent(float f) {

	return ofPolyline::getIndexAtPercent(f) / float(splineResolution);
}

// ----------------------------------------------------------------------------
float ofxPolylineSpline::getIndexAtLength(float f) {

	return ofPolyline::getIndexAtLength(f) / float(splineResolution);
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getPointAtLength(float f) {

	return getPointAtIndexInterpolated(getIndexAtLength(f));
}

// ----------------------------------------------------------------------------
glm::vec3 ofxPolylineSpline::getPointAtPercent(float f) {

	return getPointAtIndexInterpolated(getIndexAtPercent(f));
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
