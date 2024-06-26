#include "ofApp.h"
#include "utils.h"
#include "sound_generation.h"

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
  keyboard_ctrl_modifier = false;
  use_pass_filter = false;

	omega0 = 0.1;
  quality = 0.8;

  current_filter = 0;
  listen_pass = false;

  use_LPF=false;
  use_HPF=false;

	x1_pass_filter = 0;
	x2_pass_filter = 0;

	y1_pass_filter = 0;
	y2_pass_filter = 0;

  lAudio.assign(bufferSize, 0.0);
  rAudio.assign(bufferSize, 0.0);
  //carre.assign(bufferSize, 0.0);

  soundStream.printDeviceList();

  //settings audio initiaux La4
  octave = 4;
  //frequence_pitch = 440;
  //pitch = 57;

 
 
  ofSoundStreamSettings settings;

  //++++
  f=440;
  op=0;
  brillance=10;
 
  
  //int scintillance=20;

 // float f=440.0;

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

  // Draw WIKI

  ofTranslate(32, 0, 0);
  ofSetColor(225);

  ofDrawBitmapString("WIKI", 986, 400);
  ofSetLineWidth(1);
  ofDrawRectangle(972, 375, 600, 370);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofDrawBitmapString("press '*' to UNPAUSE the audio\npress '/' to PAUSE the audio\npress 'p' to enable/disable pass filter\npress 'r' to reset the pass filter ", 
      986, 418);

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



// Draw fourier LEFT channel:
ofPushStyle();
ofPushMatrix();
ofTranslate(32, 375, 0);

ofSetColor(225);

std::vector<float> left_transform; 
if (!use_pass_filter){
  ofDrawBitmapString("Fourier transform (Left channel)", 4, 18);

  left_transform = get_fourier_transform_from_signal(lAudio, sampleRate);
}else{
  ofDrawBitmapString("Pass-filter", 4, 18);
  //std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
  //                                    float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter);

  
  left_transform = soustractive_synthese(lAudio, 2, lAudio.size(), 
                        y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, use_LPF, use_HPF);
  
}


ofSetLineWidth(1);
ofDrawRectangle(470, 0, 430, 200);


ofSetColor(245, 58, 135);
ofSetLineWidth(3);

ofBeginShape();
//float max_val = *std::max_element(left_transform.begin(), left_transform.end()); 
for (unsigned int i = 0; i < lAudio.size(); i++) {
  float x = ofMap(i, 0, left_transform.size(), 0, 430, true);
  float y = ofMap(left_transform[i], -1, 1, 0, 200, true);
  //ofVertex(x, 200 - left_transform[i]);
  ofVertex(x, 200 - y);
}
ofEndShape(false);

ofPopMatrix();
ofPopStyle();

// // Draw fourier RIGHT channel:
// ofPushStyle();
// ofPushMatrix();
// ofTranslate(32, 375, 0);

// ofSetColor(225);
// std::vector<float> right_transform;
// if (!use_pass_filter){
//   ofDrawBitmapString("Fourier transform (Right Channel)", 474, 18);

//   right_transform = get_fourier_transform_from_signal(rAudio, sampleRate);
// }else{
//   ofDrawBitmapString("Pass-filter",474, 18);
//   //std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
//   //                                    float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter);

  
//   right_transform = soustractive_synthese(rAudio, 2, rAudio.size(), 
//                         y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, use_LPF, use_HPF);
                        
  
// }
  // draw the RIGHT channel:
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(32, 150, 0);

  ofSetColor(225);
  ofDrawBitmapString("Filtered sound", 474, 18);

  ofSetLineWidth(1);
  ofDrawRectangle(470, 0, 430, 200);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  std::vector<float> right_transform;
  right_transform = soustractive_synthese(rAudio, 2, rAudio.size(),y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, use_LPF, use_HPF);

  for (unsigned int i = 0; i < right_transform.size(); i++) {
    float x = ofMap(i, 0, right_transform.size(), 470, 900, true);
    ofVertex(x, 100 - right_transform[i] * 180.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

// Draw fourier RIGHT channel:
ofPushStyle();
ofPushMatrix();
ofTranslate(32, 375, 0);

ofSetColor(225);


ofDrawBitmapString("Fourier transform (filtered sound)", 474, 18);
std::vector<float> right_transform_fourier;
right_transform_fourier = get_fourier_transform_from_signal(right_transform, sampleRate);
  

right_transform = soustractive_synthese(rAudio, 2, rAudio.size(), 
                        y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, use_LPF, use_HPF);

//print_array_float(rAudio);
ofSetLineWidth(1);
ofDrawRectangle(0, 0, 430, 200);

ofSetColor(245, 58, 135);
ofSetLineWidth(3);

ofBeginShape();
//max_val = *std::max_element(right_transform.begin(), right_transform.end()); 
for (unsigned int i = 0; i < right_transform_fourier.size(); i++) {
  float x = ofMap(i, 0, right_transform_fourier.size(), 470, 900, true);
  float y = ofMap(right_transform_fourier[i], -1, 1, 0, 200, true);
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
ofDrawRectangle(940, 150, 600, 200);

ofSetColor(245, 58, 135);
ofSetLineWidth(3);

string reportString = "volume: (" + ofToString(volume, 2) +
                      ") modify with -/+ keys\npan: (" + ofToString(pan, 2) +
                      ") modify with mouse x\nsynthesis: ";
if (!bNoise) {
  reportString += "sine wave (" + ofToString(frequence_pitch, 2) +
                  "hz) modify with mouse y\n";
} else {
  reportString += "noise";
}

reportString += "omega_0: (" + ofToString(omega0, 2) + 
                  ") modify with 1/ctrl+1 keys\nquality: (" + ofToString(quality, 2) +
                  "modify with 4/ctrl+4 keys\nFilter configuration: LPF:" + 
                  ofToString(use_LPF) + " HPF: " + ofToString(use_HPF) + " modify with 3\n"+
                  "Octave: " + ofToString(octave) + " modify with 2/ctrl+2" ;

ofDrawBitmapString(reportString, 954, 200);

// PIANO


int numKeys = 7; 
float keyWidth = 900 / numKeys;

// WHITE keys
ofSetColor(225);
for(int i = 0; i < numKeys; i++) {
    ofDrawRectangle(i * keyWidth, ofGetHeight() - 400, keyWidth, 100);
}

// BLACK keys
ofSetColor(0);
for(int i = 0; i < numKeys; i++) {
    if(i != 2 && i != 6) { 
        float x = i * keyWidth + keyWidth * 0.65;
        ofDrawRectangle(x, ofGetHeight() - 400, keyWidth * 0.7, 60);
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
    ofDrawBitmapString(label, i * keyWidth + keyWidth * 0.5, ofGetHeight() - 380);
    ofDrawBitmapString(keyboard_label, i * keyWidth + keyWidth * 0.5, ofGetHeight() - 310);

}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  //std::cout << key << std::endl;

  if (key == '-' || key == '_') {
    volume -= 0.05;
    volume = MAX(volume, 0);
  } else if (key == '+' || key == '=') {
    volume += 0.05;
    volume = MIN(volume, 1);
  }

  if (key == '*') {
    soundStream.start();
  }
  if (key == 3682){
    keyboard_ctrl_modifier = true;
  }
  if (key == '/') {
    soundStream.stop();
  }

// modification de brillance
if (key == 'm'){
    brillance+=1;
  }
  if (key == 'n'){
    if (brillance > 0){
      brillance-=1;
    }
  }
// choix de type de signal
    if (key == '0'){
    op+=1;
    op=op%3;
    if (op==0){
    name_wave.assign(" sinusoidal ");
    }
    else if (op==1){
    name_wave.assign(" square ");
    }
    else if (op==2){
    name_wave.assign(" sawtooth ");      
    }
    
  }

  if (key == '1'){
    if (keyboard_ctrl_modifier){
      omega0-=0.05;
    }else{
      omega0+=0.05;
    }
    omega0 = MIN(PI, MAX(0, omega0));
    reset_pass_filter_coeff(x1_pass_filter, x2_pass_filter, y1_pass_filter, y2_pass_filter);
  }
    if (key == '2'){
      //std::cout << "Changing octave" << std::endl;
      if (keyboard_ctrl_modifier){
        octave-=1;
      }else{
        octave+=1;
      }
      octave = MIN(20, MAX(0, octave));

    reset_pass_filter_coeff(x1_pass_filter, x2_pass_filter, y1_pass_filter, y2_pass_filter);
  }
  if (key == '4'){
    if (keyboard_ctrl_modifier){
      quality-=0.05;
    }else{
      quality+=0.05;
    }
    quality = MIN(1, MAX(0, quality));
    reset_pass_filter_coeff(x1_pass_filter, x2_pass_filter, y1_pass_filter, y2_pass_filter);
  }
  if (key == '5'){
    listen_pass = true;
  }

  if (key == '3'){
    current_filter+=1;
    int id = current_filter%4;
    if (id==0){
      use_LPF = false; use_HPF=false;
    }else if (id == 1){
      use_LPF = true; use_HPF=false;
    }else if(id == 2){
      use_LPF = false; use_HPF=true;
    }else{
      use_LPF = true; use_HPF=true;
    }
  }
  if (key == 'p'){
    use_pass_filter = !(use_pass_filter);
  }
  if (key == 'r'){
    omega0 = 0.1;
    quality = 0.8;

  reset_pass_filter_coeff(x1_pass_filter, x2_pass_filter, y1_pass_filter, y2_pass_filter);
  }
  // PIANO keys and corresponding notes
  if (key == 'q'){note = 0; }
  if (key == 'z'){note = 1;}
  if (key == 's'){note = 2;}
  if (key == 'e'){note= 3;}
  if (key == 'd') {note = 4;} 
  if (key == 'f') {note = 5;}
  if (key == 't') {note = 6;}
  if (key == 'g') {note = 7;}
  if (key == 'y') {note = 8;}
  if (key == 'h') {note = 9;}
  if (key == 'u') {note = 10;}
  if (key == 'j') {note = 11;}
// std::cout << note << std::endl;
// std::cout << key << std::endl;

  frequence_pitch = keytofrequency(octave, note, pitch, A4frequency, A4pitch);
  phaseAdderTarget = (frequence_pitch / (float)sampleRate) * TWO_PI;
  //std::cout << frequence_pitch << std::endl;

}



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
  if (key == 3682){
    keyboard_ctrl_modifier = false;
  }

  if (key == '5'){
    listen_pass = false;
  }

}

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
/*void ofApp::audioOut(ofSoundBuffer &buffer) {
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
    cout << "La valeur de la fréquence est : " <<frequence_pitch << endl;
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
*/
void ofApp::audioOut(ofSoundBuffer &buffer) {
  // pan = 0.5f;
  float leftScale = 1 - pan;
  float rightScale = pan;
  //+++
  float somme;
  float sample;

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
      /*rAudio[i] = buffer[i * buffer.getNumChannels() + 1] =
          ofRandom(0, 1) * volume * rightScale;*/
    }
  } else {
  
    phaseAdder = 0.75f * phaseAdder + 0.25f * phaseAdderTarget;
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
      phase += phaseAdder;

      if (op==0){sample = sin(phase); }
      //carré
      else if (op==1){   
        somme=0.0;
        for (size_t k = 0; k < brillance; k++) {                     
        somme=somme+4/PI*(sin((2*k+1)*phase))/(2*k+1);      
        }   
        sample = somme;
      }
      else {   //if (op==2)
        somme=0.0;
        for (size_t k = 1; k < brillance; k++) {
          float sign = k % 2 == 0 ? 1.f : -1.f;                     
          somme = somme + sign*(sin(k*phase)/k);   
        }   
        sample = (2/PI)*somme; 
      }
      if (!listen_pass){
        lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * volume * leftScale;
        rAudio[i] = buffer[i * buffer.getNumChannels()+1] =sample * volume * leftScale;
      }else{
        std::vector<float> new_audio;
        lAudio[i] = sample * volume * leftScale;
        y1_pass_filter= 0.0, y2_pass_filter= 0.0, x1_pass_filter= 0.0, x2_pass_filter = 0.0;
        new_audio = soustractive_synthese(lAudio, 2, lAudio.size(), 
                      y1_pass_filter, y2_pass_filter, x1_pass_filter, x2_pass_filter, quality, omega0, true, use_LPF, use_HPF);
        buffer[i * buffer.getNumChannels()] = new_audio[i];
        buffer[i * buffer.getNumChannels()+1] = new_audio[i];
      }
      /*rAudio[i] = buffer[i * buffer.getNumChannels() + 1] =
          sample * volume * rightScale;*/
    }
  /*}*/
}
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}


//--------------------------------------------------------------
int ofApp::keytopitch(int octave, int note) {

  return octave * 12 + note;

}

// Function that convert pitch into frequency. Pitch is equal to noctave * 12 + note

float ofApp::pitchToFrequency(int pitch, float A4frequency, int A4pitch){
	return A4frequency * pow(2, ((pitch - A4pitch) / 12.f));
}

//
float ofApp::keytofrequency(int octave, int note,int pitch, float A4frequency, int A4pitch){
	pitch = keytopitch(octave, note);
  frequence_pitch = pitchToFrequency(pitch, A4frequency, A4pitch);
  return frequence_pitch;
}