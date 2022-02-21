
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
Error_t processFilterExec(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float fDelayInSec, float fGain, int blockSize);
void runTests();



/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath;

    CCombFilterIf* phCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType;

    // filter params:
    float fDelayInSec = 0;
    float fGain = 1;
    float fMaxDelayInSec = 1; // USER OF THE INTERFACE CAN DEFINE THIS, NOT USER OF EXECUTABLE

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float** ppfOutputAudioData = 0;


    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;
    //CAudioFileIf::FileSpec_t stFileSpec;


    showClInfo();

    //runTests();


    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    //input path, filer type. delay time, gain
    
    if (argc != 5 && argc != 2)
    {
        cout << "Usage: \<input path\>, \<filter type ('FIR' or 'IIR')\>, \<delay time\>, \<gain\>";
        return -1;
    }
    else if (argc == 2) {
        //run tests
        cout << "Running tests" << "\n";
        runTests();
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + "_combFiltered.wav";



        fDelayInSec = atof(argv[3]);
        fGain = atof(argv[4]);

        if (strcmp(argv[2], "FIR")) {
            filterType = CCombFilterIf::CombFilterType_t::kCombFIR;
            processFilterExec(sInputFilePath, sOutputFilePath, filterType, fDelayInSec, fGain, kBlockSize);

        }
        else if (strcmp(argv[2], "IIR")) {
            filterType = CCombFilterIf::CombFilterType_t::kCombIIR;
            processFilterExec(sInputFilePath, sOutputFilePath, filterType, fDelayInSec, fGain, kBlockSize);

        }
        else {
            cout << "Incorrect filter type, possible types are 'FIR' or 'IIR'" << "\n";
        }
    }


    // all done
    return 0;
}

Error_t processFilterExec(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float fDelayInSec, float fGain, int blockSize) {
    //The command line application instantiates your class with the correct parameters, 
    //read the parameters from the command line(file name, effect - parameters), 
    //reads and processes audio data in blocksand writes the result to an output wav file.

    //instantiate the class with the correct parameters:

    cout << "processing the filter stuff" << endl;


    float** ppfInputAudioData = 0;
    float** ppfOutputAudioData = 0;


    CAudioFileIf* phInputAudioFile = 0;
    CAudioFileIf* phOutputAudioFile = 0;
    CAudioFileIf::FileSpec_t stFileSpec;
    clock_t time = 0;


  


    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phInputAudioFile);

    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen())
    {
        cout << "Input wav file open error!" << endl;
        CAudioFileIf::destroy(phInputAudioFile);
        return Error_t::kFileOpenError;
    }
    phInputAudioFile->getFileSpec(stFileSpec);
  
    // open output wave file
    CAudioFileIf::create(phOutputAudioFile);
    phInputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phOutputAudioFile->isOpen())
    {
        cout << "Output wav file open error!" << endl;
        CAudioFileIf::destroy(phOutputAudioFile);
        return Error_t::kFileOpenError;
    }

    float sampleRate = stFileSpec.fSampleRateInHz;
    int numChannels = stFileSpec.iNumChannels;

  // create the filter
    cout << "making fiulter" << endl;
    CCombFilterIf* pCombFilter = 0;
    cout << "creating " << endl;
    CCombFilterIf::create(pCombFilter);
    cout << "created" << endl;
    pCombFilter->init(filterType, fDelayInSec, sampleRate, numChannels);
    cout << "inited " << endl;
    pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamDelay, fDelayInSec * sampleRate);
    pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamGain, fGain);


    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfInputAudioData = new float* [stFileSpec.iNumChannels];
    ppfOutputAudioData = new float* [stFileSpec.iNumChannels];

    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfInputAudioData[i] = new float[blockSize];
        ppfOutputAudioData[i] = new float[blockSize];
    }


    ////////////////////////////////////////////////////////////////////////////////
    //// get input audio data and write it to the output text file (one column per channel)
    while (!phInputAudioFile->isEof())
    {
        // set block length variable
        long long iNumFrames = blockSize;

        // read data (iNumOfFrames might be updated!)
        phInputAudioFile->readData(ppfInputAudioData, iNumFrames);

        // filter the audio
        pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, iNumFrames);

        // write to output file path
        phOutputAudioFile->writeData(ppfOutputAudioData, iNumFrames);
    }

    cout << "\nreading/filtering/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;




    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phInputAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);
    CCombFilterIf::destroy(pCombFilter);

    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        delete[] ppfInputAudioData[i];
        delete[] ppfOutputAudioData[i];
    }

    delete[] ppfInputAudioData;
    delete[] ppfOutputAudioData;
    ppfInputAudioData = 0;
    ppfOutputAudioData = 0;



    system("pause");


    return Error_t::kNoError;
}


void runTests() {
    cout << "running all the tests!" << endl;

    cout << "Test 1:" << endl;
    std::string sInputFilePath1 = "../sinesweep.wav";
    std::string sOutputFilePath1 = "../test1_output.wav";
    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::CombFilterType_t::kCombFIR;
    float fDelayInSec = 1.0f;
    float fGain = 1.0f;
    int iBlockSize = 1024;

    cout << sInputFilePath1 << endl;
    processFilterExec(sInputFilePath1, sOutputFilePath1, filterType, fDelayInSec, fGain, iBlockSize);

    cout << "Test 2:" << endl;

    cout << "Test 3:" << endl;

    cout << "Test 4:" << endl;

    cout << "Test 5:" << endl;

    system("pause");

}



void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
}
