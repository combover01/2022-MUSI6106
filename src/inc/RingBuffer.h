#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstring>

/*! \brief implement a circular buffer of type T
*/
template <class T> 
class CRingBuffer
{
public:
    explicit CRingBuffer(int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples)
    {
        assert(iBufferLengthInSamples > 0);

        // allocate and init
        m_iWriteIdx(0);
        m_iReadIdx(0);
        //create an array
        blockArr(0);

    }

    virtual ~CRingBuffer()
    {
        // free memory
        delete[] blockArr;
    }

    /*! add a new value of type T to write index and increment write index
    \param tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        blockArr[m_iWriteIdx] = tNewValue;
        m_iWriteIdx = CRingBuffer::incrementIdx(m_iWriteIdx);
    }

    /*! add a new value of type T to write index
    \param tNewValue the new value
    \return void
    */
    void put(T tNewValue)
    {
        blockArr[m_iWriteIdx] = tNewValue;
    }
    
    /*! return the value at the current read index and increment the read pointer
    \return float the value from the read index
    */
    T getPostInc()
    {
        T curVal = blockArr[m_iReadIdx];
        m_iReadIdx = CRingBuffer::incrementIdx(m_iReadIdx);
        return static_cast<T>(curVal);
    }

    /*! return the value at the current read index
    \return float the value from the read index
    */
    T get() const
    {
        return static_cast<T>(blockArr[m_iReadIdx]);
    }
    
    /*! set buffer content and indices to 0
    \return void
    */
    void reset()
    {

    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx() const
    {
        return m_iWriteIdx;
    }

    /*! move the write index to a new position
    \param iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx(int iNewWriteIdx)
    {
        m_iWriteIdx = iNewWriteIdx;
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx() const
    {
        return m_iReadIdx;
    }

    /*! move the read index to a new position
    \param iNewReadIdx: new position
    \return void
    */
    void setReadIdx(int iNewReadIdx)
    {
        m_iReadIdx = iNewReadIdx;
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer() const
    {
        
        return -1;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength() const
    {
        return -1;
    }
private:
    CRingBuffer();
    CRingBuffer(const CRingBuffer& that);

    int m_iBuffLength;              //!< length of the internal buffer
    int m_iWriteIdx;
    int m_iReadIdx;
    blockArr = new T[m_iBuffLength];
    int incrementIdx(int val) {
        int outIdx;
        if (val + 1 < iBufferLengthInSamples) {
            outIdx = val + 1;
        }
        else {
            outIdx = 0;
        }
    }

};
#endif // __RingBuffer_hdr__
