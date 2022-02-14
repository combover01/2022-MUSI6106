#include "CombFilter.h"
#include "CombFilterIf.h"
#include "RingBuffer.h"


#include <iostream>
using namespace std;

//Inspired by CAudioFileIf/CAudioFile, 
//create the files CombFilter.h/.cpp in the 
//appropriate folder locations and populate them 
//with your classes/functions etc. 
//The CombFilterIf.h header contains a 
//declaration of the CCombFilterBase class. 
//This class must be defined in CombFilter.h. 
//Note: when adding files to the directory, 
//CMake must be run again in order to view the files in the built project.


//implement the filter here

CCombFilterBase::CCombFilterBase() :
	//set default values here, like the init function

	//comb filter type ?

	//fmaxdelaylengthins
	m_iMaxDelayLengthSamps(1),
	m_iCurDelayLengthSamps(0),
	m_ppRingBuffer(0),
	m_fGain(1.0),
	m_iNumChannels(1)


	//fsamplerateinhz

	//inumchannels

{}

CCombFilterBase::CCombFilterBase(int iMaxDelaySamps, int iNumChannels) {
	// initializing the ringbuffer in each channel
	m_ppRingBuffer = new CRingBuffer<float>*[iNumChannels];
	for (int i = 0; i < iNumChannels; i++) {
		m_ppRingBuffer[i] = new CRingBuffer<float>(m_iMaxDelayLengthSamps);
	}		
		
	m_iMaxDelayLengthSamps = iMaxDelaySamps;
	m_iNumChannels = iNumChannels;


}

//the destroy function - destroy the ring buffer arrays
CCombFilterBase::~CCombFilterBase() {
	for (int i = 0; i < m_iNumChannels; i++) {
		delete m_ppRingBuffer[i];
	}
	delete[] m_ppRingBuffer;
	m_ppRingBuffer = 0;
}

Error_t CCombFilterBase::setParam(FilterParam_t eParam, float fParamValue) {
	//set the parameters
	if (eParam == kParamDelay) {
		//sorry we are not going to save the original asked for delay length in seconds
		CCombFilterBase::m_iCurDelayLengthSamps = (int)round(fParamValue);
		for (int i = 0; i < m_iNumChannels; i++) {
			// sets write index in ring buffer ahead by x samps
			m_ppRingBuffer[i] -> setWriteIdx((int)round(fParamValue));
			// do we need to set a read index too? it should stay the same
		}

	}
	else if (eParam == kParamGain) {
		CCombFilterBase::m_fGain = fParamValue;
	}
	else {
		return Error_t::kFunctionInvalidArgsError;
	}


	return Error_t::kNoError;
}

float CCombFilterBase::getParam(FilterParam_t eParam) {
	// get the parameters 
	if (eParam == kParamDelay) {
		//since we didnt save the delay length in seconds we need to convert back from sample rate
		return m_iCurDelayLengthSamps/CCombFilterIf::m_fSampleRate;
	}
	else if (eParam == kParamGain) {
		return m_fGain;
	}
	else {
		// if given the wrong inputs, return 0
		return 0;
	}
}


Error_t CFIR::process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames) {
	
	cout << "Running comb filter FIR" << "\n";
	// output = input[n] + gain*input[n-delay]
	for (int i = 0; i < m_iNumChannels; i++) {
		//curOut = read + gain*write
		ppfOutputBuffer**


	}



	return Error_t::kNoError;
}
Error_t CIIR::process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames) {
	cout << "Running comb filter IIR" << "\n";
	// output = input[n] + gain*output[n-delay]
	//curOut = read + gain*
	
	
	
	return Error_t::kNoError;

}

CFIR::CFIR(int iMaxDelaySamps, int iNumChannels) : CCombFilterBase(iMaxDelaySamps, iNumChannels) {}

CIIR::CIIR(int iMaxDelaySamps, int iNumChannels) : CCombFilterBase(iMaxDelaySamps, iNumChannels) {}

CFIR::~CFIR() {} 

CIIR::~CIIR() {}
