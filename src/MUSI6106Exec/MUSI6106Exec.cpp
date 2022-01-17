
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float** ppfAudioBuffer = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments

    if (argc == 3) {
        sInputFilePath = argv[argc - 2];
        sOutputFilePath = argv[argc - 1];
    }
    else if (argc == 2) {
        sInputFilePath = argv[argc - 1];
        sOutputFilePath = "outputText.txt";
    }
    else {
        // default behavior: take last argument as the output file name
        printf("Usage: \nMUSI6106Exec.exe <\"Audio File Path\"> [\"Output File Path\"]");
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    //new //array, allocation is done in a loop. know your type
   
    stFileSpec.eFormat = CAudioFileIf::kFileFormatWav;
    stFileSpec.eBitStreamType = CAudioFileIf::kFileBitStreamFloat32;
    stFileSpec.iNumChannels = 2;
    stFileSpec.fSampleRateInHz = 44100;

    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead, &stFileSpec);
    bool isOpenq = phAudioFile->isOpen();
    bool isInit = phAudioFile->isInitialized();
 
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    std::ofstream outputText{ sOutputFilePath};

 
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    // we need to find size of the audio file 
    long long iLengthInFrames;
    phAudioFile->getLength(*&iLengthInFrames);
    const long long frameCount = iLengthInFrames;
    const int channelCount = stFileSpec.iNumChannels;

    ppfAudioData = new float* [channelCount]; //columns
    for (int i = 0; i < channelCount; ++i) {
        ppfAudioData[i] = new float[kBlockSize]; //rows. only 1024 samples every time
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)

    long long readAmount = kBlockSize; //should read 1024 samples every time
    while (readAmount == kBlockSize) {
        phAudioFile->readData(ppfAudioData, readAmount);
        for (int k = 0; k < readAmount; ++k) {
            for (int j = 0; j < channelCount; ++j) {
                std::string curData = std::to_string(ppfAudioData[j][k]) + "\t";
                outputText << curData;
            }
            outputText << "\n";
        }

    }





    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    phAudioFile->closeFile();
    CAudioFileIf::destroy(phAudioFile);
    // all done
    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

