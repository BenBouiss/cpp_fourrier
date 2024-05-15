#pragma once

#include "ofMain.h"
#include "utils.h"
#include "sound_generation.h"

enum class Notes
{
	Do,
	Do_diese,
	Re,
	Re_diese,
	Mi,
	Fa,
	Fa_diese,
	Sol,
	Sol_diese,
	La,
	La_diese,
	Si	
};




class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void audioOut(ofSoundBuffer & buffer);
		//------ calculate the pitch
		int keytopitch(int  octave, int note);
		float pitchToFrequency(int pitch, float A4frequency, int A4pitch);
		float keytofrequency(int octave, int note,int pitch, float A4frequency, int A4pitch);

		ofSoundStream soundStream;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		vector <float> lAudio;
		vector <float> rAudio;
		
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;


		// key modifiers
		bool keyboard_ctrl_modifier;

		//-------------------- Substractive synth stuff
		bool use_pass_filter;
		int current_filter;
		bool use_LPF;
		bool use_HPF;


		float omega0;
		float quality;

		float x1_pass_filter;
		float x2_pass_filter;
		float y1_pass_filter;
		float y2_pass_filter;

		//---------- for calculate frequency from keyboard input and note
		int octave;
		int note;
		int frequence_pitch;
		int pitch;
		float A4frequency = 440.f;
		int A4pitch = 69;
		int op;
		float f;
		int brillance;
};
