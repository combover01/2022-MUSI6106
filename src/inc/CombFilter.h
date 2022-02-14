//Inspired by CAudioFileIf/CAudioFile, 
//create the files CombFilter.h/.cpp in the 
//appropriate folder locations and populate them 
//with your classes/functions etc. 
//The CombFilterIf.h header contains a 
//declaration of the CCombFilterBase class. 
//This class must be defined in CombFilter.h. 
//Note: when adding files to the directory, 
//CMake must be run again in order to view the files in the built project.

#include "CombFilterIf.h"
#include "RingBuffer.h"

//define CCombFilterBase
class CCombFilterBase : public CCombFilterIf {
public:

    CCombFilterBase();
    CCombFilterBase(int iMaxDelaySamps, int iNumChannels);
    virtual ~CCombFilterBase();

	//needs:
	// a delay line length 
	// a ring buffer? and all things associated with it
	// a current output

	//chose which type it is with an if statement


    /*! sets a comb filter parameter
    \param eParam what parameter (see ::FilterParam_t)
    \param fParamValue value of the parameter
    \return Error_t
    */
    Error_t setParam(FilterParam_t eParam, float fParamValue);

    /*! return the value of the specified parameter
    \param eParam
    \return float
    */
    float getParam(FilterParam_t eParam);

    /*! processes one block of audio
    \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
    \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
    \param iNumberOfFrames buffer length (per channel)
    \return Error_t
    */
    virtual Error_t process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames);

protected:
    CRingBuffer <float> **m_ppRingBuffer;
    float m_iMaxDelayLengthSamps;
    int m_iCurDelayLengthSamps;
    int m_iNumChannels;
    float m_fGain;

private:
    CCombFilterBase(const CCombFilterBase& that);
};

class CFIR : public CCombFilterBase {
public:
    CFIR(int iMaxDelaySamps, int iNumChannels);
    virtual ~CFIR();
    Error_t process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames);
};

class CIIR : public CCombFilterBase {
public:
    CIIR(int iMaxDelaySamps, int iNumChannels);
    virtual ~CIIR();
    Error_t process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames);
};