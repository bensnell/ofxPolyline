#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	int resolution = 20;
	line.add({ 100, 100, 0 }, resolution);
	line.add({ 500, 100, 0 }, resolution);
	line.add({ 500, 500, 0 }, resolution);
	line.add({ 100, 500, 0 }, resolution);
	line.close();
}

//--------------------------------------------------------------
void ofApp::update(){

	// Get the closest point, interpolated index & length at this point, 
	// at the location of the mouse.

	glm::vec3 cursor(ofGetMouseX(), ofGetMouseY(), 0);
	float interpIndex;
	closestPt = line.getClosestPoint(cursor, &interpIndex);
	float length = line.getLengthAtIndexInterpolated(interpIndex);
	float percent = length / line.getPerimeter();

	ptInfo.str("");
	ptInfo << setprecision(3);
	ptInfo << "Closest Point: " << closestPt << "\n";
	ptInfo << "Interp Index:  " << interpIndex << "\n";
	ptInfo << "Length:        " << length << "\n";
	ptInfo << "Percent:       " << percent;

	// additional debug info:
	//ptInfo << "\n---\n";
	//unsigned int nearestIndex;
	//glm::vec3 cp2 = line.getClosestPoint(cursor, &nearestIndex);
	//ptInfo << "Closest Pt2:   " << cp2 << "\n";
	//ptInfo << "Index (Int):   " << nearestIndex << "\n";
	//ptInfo << "Closest Pt3:   " << line.getPointAtIndexInterpolated(interpIndex) << "\n";
	//ptInfo << "Closest Pt4:   " << line.getPointAtLength(length) << "\n";
	//ptInfo << "Closest Pt5:   " << line.getPointAtPercent(percent) << "\n";
	//ptInfo << "Length (Int):  " << line.getLengthAtIndex(nearestIndex) << "\n";
	//ptInfo << "Interp Index2: " << line.getIndexAtLength(length) << "\n";
	//ptInfo << "Interp Index3: " << line.getIndexAtPercent(percent);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);

	ofSetColor(255, 0, 0);
	line.draw();

	ofDrawBitmapStringHighlight(ptInfo.str(), { closestPt.x, closestPt.y + 13 }, ofColor(0, 128));

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'm') line.cycleMode();
	if (key == 'c') line.cycleClosed();

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
