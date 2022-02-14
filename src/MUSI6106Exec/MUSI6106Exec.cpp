
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
int processFilterExec(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float fDelayInSec, float fGain, int blockSize);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath;

    CCombFilterIf::CombFilterType_t filterType;
    CCombFilterIf* phCombFilter = 0;
    // filter params:
    float fDelayInSec = 0;
    float fGain = 1;
    float fMaxDelayInSec = 1; // USER OF THE INTERFACE CAN DEFINE THIS, NOT USER OF EXECUTABLE

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfAudioData = 0;
    float** ppfAudioOutputData = 0;


    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    


    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    //input path, filer type. delay time, gain
    if (argc != 5 && argc != 1)
    {
        cout << "Usage: \<input path\>, \<filter type ('FIR' or 'IIR')\>, \<delay time\>, \<gain\>";
        return -1;
    }
    else if (argc == 1) {
        //run tests
        cout << "Running tests" << "\n";
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + "_combFiltered.wav";

        if (strcmp(argv[2], "FIR")) {
            filterType = CCombFilterIf::CombFilterType_t::kCombFIR;
        }
        else if (strcmp(argv[2], "IIR")) {
            filterType = CCombFilterIf::CombFilterType_t::kCombIIR;
        }
        else {
            cout << "Incorrect filter type, possible types are 'FIR' or 'IIR'" << "\n";
        }

        fDelayInSec = atof(argv[3]);
        fGain = atof(argv[4]);
        
    }




    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);



    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

    //if (ppfAudioData == 0)
    //{
    //    CAudioFileIf::destroy(phAudioFile);
    //    hOutputFile.close();
    //    return -1;
    //}
    //if (ppfAudioData[0] == 0)
    //{
    //    CAudioFileIf::destroy(phAudioFile);
    //    hOutputFile.close();
    //    return -1;
    //}

    //time = clock();



    ////////////////////////////////////////////////////////////////////////////////
    //// get audio data and write it to the output text file (one column per channel)
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);

        // filter the audio
        phCombFilter->process(ppfAudioData, ppfAudioOutputData, iNumFrames);

        // write to output file path
        phOutputAudioFile->writeData(ppfAudioOutputData, iNumFrames);
    }

    cout << "\nreading/filtering/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;




    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);
    CCombFilterIf::destroy(phCombFilter);

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    // all done
    return processFilterExec(sInputFilePath,sOutputFilePath,filterType,fDelayInSec,fGain,kBlockSize);

}

int processFilterExec(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float fDelayInSec, float fGain, int blockSize) {
    //The command line application instantiates your class with the correct parameters, 
    //read the parameters from the command line(file name, effect - parameters), 
    //reads and processes audio data in blocksand writes the result to an output wav file.

    //instantiate the class with the correct parameters:








    return 0;
}





void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}
