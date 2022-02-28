
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    CFft::complex_t* pOutputSpectrum;
    CFft::complex_t* pBufferSpectrum;

    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;
    int                     iBlockLength;
    int                     iHopLength;

    //static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float** ppfAudioData = 0;

    CAudioFileIf* phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    CFft* cfft = 0;

    // generate the output, of type complex t


    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments

    // should be AudioFilePath BlockLength Hoplength
    if (argc != 4)
    {
        cout << "Expected usage: \<AudioFilePath\> \<BlockLength\> \<HopLength\>";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        iBlockLength = (int)argv[2];
        iHopLength = (int)argv[3];
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
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }

    ///////////////////////////////////////////
    // start an instance of cfft
    CFft::createInstance(cfft);
    CFft::initInstance()
   


    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[iBlockLength];

    if (ppfAudioData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    if (ppfAudioData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }

    time = clock();




    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long iNumFrames = iBlockLength;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);

        cout << "\r" << "reading and writing";

        // write
        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                hOutputFile << ppfAudioData[c][i] << "\t";
            }
            hOutputFile << endl;
        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    // all done
    return 0;

}
///////////////// 
// take fft for a block given any input sample
// inputs: starting sample, pointer to the array, and block length
// output: 1d array (buffer) of the fft of that block
float* blockFFT(int iStartingSamp, float* pInputArr, int iBlockLength, CFft::complex_t pBufferSpectrum) {
    CFft::doFft(pBufferSpectrum, pInputArr);



}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout << endl;

    return;
}

