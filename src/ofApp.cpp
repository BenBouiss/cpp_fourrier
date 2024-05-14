#include "ofApp.h"
#include "utils.h"
#include "sound_generation.h"
//--------------------------------------------------------------
void ofApp::setup() {

  ofBackground(34, 34, 34);

  int bufferSize = 512;
  sampleRate = 44100;
  phase = 0;
  phaseAdder = 0.0f;
  phaseAdderTarget = 0.0f;
  volume = 0.1f;
  bNoise = false;

  use_pass_filter = false;

	omega0 = 0.1;
  quality = 0.8;

	x1_pass_filter = 0;
	x2_pass_filter = 0;

	y1_pass_filter = 0;
	y2_pass_filter = 0;

  lAudio.assign(bufferSize, 0.0);
  rAudio.assign(bufferSize, 0.0);

  soundStream.printDeviceList();
 
 
  ofSoundStreamSettings settings;

  // if you want to set the device id to be different than the default:
  //
  //	auto devices = soundStream.getDeviceList();
  //	settings.setOutDevice(devices[3]);

  // you can also get devices for an specific api:
  //
  //	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
  //	settings.setOutDevice(devices[0]);

  // or get the default device for an specific api:
  //
  // settings.api = ofSoundDevice::Api::PULSE;

  // or by name:
  //
  //	auto devices = soundStream.getMatchingDevices("default");
  //	if(!devices.empty()){
  //		settings.setOutDevice(devices[0]);
  //	}

#ifdef TARGET_LINUX
  // Latest linux versions default to the HDMI output
  // this usually fixes that. Also check the list of available
  // devices if sound doesn't work
  auto devices = soundStream.getMatchingDevices("default");
  if (!devices.empty()) {
    settings.setOutDevice(devices[0]);
  }
#endif

  settings.setOutListener(this);
  settings.sampleRate = sampleRate;
  settings.numOutputChannels = 2;
  settings.numInputChannels = 0;
  settings.bufferSize = bufferSize;
  soundStream.setup(settings);
  // stop added no sound at beginning
  soundStream.stop();

  // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you
  // need to synchronize buffersizes. use ofFmodSetBuffersize(bufferSize) to set
  // the buffersize in fmodx prior to loading a file.
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------

void ofApp::draw() {

  ofSetColor(225);
  ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
  ofDrawBitmapString(
      "press 's' to unpause the audio\npress 'e' to pause the audio\npress 'p' to enable/disable pass filter\npress 'r' to reset the pass filter ", 31, 92);

  ofNoFill();

  // draw the LEFT channel:
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(32, 150, 0);

  ofSetColor(225);
  ofDrawBitmapString("Left Channel", 4, 18);

  ofSetLineWidth(1);
  ofDrawRectangle(0, 0, 430, 200);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  for (unsigned int i = 0; i < lAudio.size(); i++) {
    float x = ofMap(i, 0, lAudio.size(), 0, 430, true);
    ofVertex(x, 100 - lAudio[i] * 180.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

  // draw the RIGHT channel:
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(32, 150, 0);

  ofSetColor(225);
  ofSetLineWidth(1);
  
  ofDrawRectangle(470, 0, 430, 200);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  for (unsigned int i = 0; i < rAudio.size(); i++) {
    float x = ofMap(i, 0, rAudio.size(), 470, 900, true);
    ofVertex(x, 100 - rAudio[i] * 180.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

// Draw FOURRIER LEFT channel:
ofPushStyle();
ofPushMatrix();
ofTranslate(32, 375, 0);

ofSetColor(225);

std::vector<float> left_transform; 
if (!use_pass_filter){
  ofDrawBitmapString("Fourier transform (Left channel)", 4, 18);

  left_transform = get_fourrier_transform_from_signal(lAudio, sampleRate);
}else{
  ofDrawBitmapString("pass filter", 4, 18);
  //std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
  //                                    float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter);

  
  left_transform = soustractive_synthese(lAudio, 2, lAudio.size(), 
                        y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, true, true);
  
}


ofSetLineWidth(1);
ofDrawRectangle(0, 0, 430, 200);


ofSetColor(245, 58, 135);
ofSetLineWidth(3);

ofBeginShape();

for (unsigned int i = 0; i < lAudio.size(); i++) {
  float x = ofMap(i, 0, left_transform.size(), 470, 900, true);
  float y = ofMap(left_transform[i], 0, left_transform.size(), 0, 200, true);
  //ofVertex(x, 200 - left_transform[i]);
  ofVertex(x, 200 - y);
}
ofEndShape(false);

ofPopMatrix();
ofPopStyle();

// Draw FOURRIER RIGHT channel:
ofPushStyle();
ofPushMatrix();
ofTranslate(32, 375, 0);

ofSetColor(225);
std::vector<float> right_transform;
if (!use_pass_filter){
  ofDrawBitmapString("Fourier transform (Right Channel)", 474, 18);

  right_transform = get_fourrier_transform_from_signal(rAudio, sampleRate);
}else{
  ofDrawBitmapString("pass filter",474, 18);
  //std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
  //                                    float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter);

  
  right_transform = soustractive_synthese(rAudio, 2, rAudio.size(), 
                        y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, true, true);
  
}
//std::vector<float> right_transform = get_fourrier_transform_from_signal(rAudio, sampleRate);

ofSetLineWidth(1);
ofDrawRectangle(470, 0, 430, 200);

ofSetColor(245, 58, 135);
ofSetLineWidth(3);

ofBeginShape();
float max_val = *std::max_element(right_transform.begin(), right_transform.end()); 
for (unsigned int i = 0; i < rAudio.size(); i++) {
  float x = ofMap(i, 0, right_transform.size(), 0, 430, true);
  float y = ofMap(right_transform[i], 0, max_val, 0, 200, true);
  ofVertex(x, 200 - y);
}
ofEndShape(false);

ofPopMatrix();
ofPopStyle();

// Draw report string

ofTranslate(32, 0, 0);

ofSetColor(225);
ofDrawBitmapString("Report", 954, 168);

ofSetLineWidth(1);
ofDrawRectangle(940, 150, 600, 425);

ofSetColor(245, 58, 135);
ofSetLineWidth(3);

string reportString = "volume: (" + ofToString(volume, 2) +
                      ") modify with -/+ keys\npan: (" + ofToString(pan, 2) +
                      ") modify with mouse x\nsynthesis: ";
if (!bNoise) {
  reportString += "sine wave (" + ofToString(targetFrequency, 2) +
                  "hz) modify with mouse y\n";
} else {
  reportString += "noise";
}
  if (use_pass_filter){
    reportString += "omega_0: (" + ofToString(omega0, 2) + 
                    ") modify with 1/2 keys\nquality: (" + ofToString(quality, 2) +
                    "modify with 4/5 keys";
  }
ofDrawBitmapString(reportString, 954, 200);

// PIANO


int numKeys = 7; 
float keyWidth = 900 / numKeys;

// WHITE keys
ofSetColor(225);
for(int i = 0; i < numKeys; i++) {
    ofDrawRectangle(i * keyWidth, ofGetHeight() - 300, keyWidth, 100);
}

// BLACK keys
ofSetColor(0);
for(int i = 0; i < numKeys; i++) {
    if(i != 2 && i != 6) { 
        float x = i * keyWidth + keyWidth * 0.65;
        ofDrawRectangle(x, ofGetHeight() - 300, keyWidth * 0.7, 60);
    }
}

// Labels  WHITE keys
ofSetColor(225);
for(int i = 0; i < numKeys; i++) {
    string label;
    string keyboard_label;
    switch(i) {
        case 0: label = "C"; keyboard_label = "q"; break;
        case 1: label = "D"; keyboard_label = "s"; break;
        case 2: label = "E"; keyboard_label = "d"; break;
        case 3: label = "F"; keyboard_label = "f"; break;
        case 4: label = "G"; keyboard_label = "g"; break;
        case 5: label = "A"; keyboard_label = "h"; break;
        case 6: label = "B"; keyboard_label = "j"; break;
    }
    ofDrawBitmapString(label, i * keyWidth + keyWidth * 0.5, ofGetHeight() - 280);
    ofDrawBitmapString(keyboard_label, i * keyWidth + keyWidth * 0.5, ofGetHeight() - 210);

}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == '-' || key == '_') {
    volume -= 0.05;
    volume = MAX(volume, 0);
  } else if (key == '+' || key == '=') {
    volume += 0.05;
    volume = MIN(volume, 1);
  }

  if (key == 's') {
    soundStream.start();
  }

  if (key == 'e') {
    soundStream.stop();
  }
  if (key == '1'){
    omega0-=0.05;
  }
  if (key == '2'){
    omega0+=0.05;
  }
    if (key == '4'){
    quality-=0.05;
  }
  if (key == '5'){
    quality+=0.05;
    quality = MAX(0.1, quality);
  }
  if (key == 'p'){
    use_pass_filter = !(use_pass_filter);
  }
  if (key == 'r'){
    omega0 = 0.1;
    quality = 0.8;

    x1_pass_filter = 0;
    x2_pass_filter = 0;

    y1_pass_filter = 0;
    y2_pass_filter = 0;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
  int width = ofGetWidth();
  pan = (float)x / (float)width;
  float height = (float)ofGetHeight();
  float heightPct = ((height - y) / height);
  targetFrequency = 2000.0f * heightPct;
  phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
  int width = ofGetWidth();
  pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) { bNoise = true; }

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) { bNoise = false; }

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer) {
  // pan = 0.5f;
  float leftScale = 1 - pan;
  float rightScale = pan;

  // sin (n) seems to have trouble when n is very large, so we
  // keep phase in the range of 0-TWO_PI like this:
  while (phase > TWO_PI) {
    phase -= TWO_PI;
  }

  if (bNoise == true) {
    // ---------------------- noise --------------
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
      lAudio[i] = buffer[i * buffer.getNumChannels()] =
          ofRandom(0, 1) * volume * leftScale;
      rAudio[i] = buffer[i * buffer.getNumChannels() + 1] =
          ofRandom(0, 1) * volume * rightScale;
    }
  } else {
    phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
      phase += phaseAdder;
      float sample = sin(phase);
      lAudio[i] = buffer[i * buffer.getNumChannels()] =
          sample * volume * leftScale;
      rAudio[i] = buffer[i * buffer.getNumChannels() + 1] =
          sample * volume * rightScale;
    }
  }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
