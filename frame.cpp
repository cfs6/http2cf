#include"frame.h"

#include"http2.h"

#define SETTINGS_ENTRY_LENGTH 6
#define DEFAULT_WEIGHT 16
#define PRIORITY_LENGTH 5
#define RST_LENGTH 4
#define WINUPDATE_LENGTH 4
#define GOAWAY_LENGTH 8
#define PING_LENGTH 8


////////////////////////////////////////////////
//////StreamPriority
STREAM_PRIORITY::STREAM_PRIORITY()
    : streamDependancy(0)
    , weigth(DEFAULT_WEIGHT)
    , exclusive(0)
{

}

////////////////////////////////////////////////
//////FrameHeader

FrameHeader::FrameHeader(size_t length, int32_t streamID, uint8_t type, int flags, int reserved)
    : m_length(length)
    , m_streamID(streamID)
    , m_type(type)
    , m_flags(flags)
    ,m_reserved (reserved)
{

}


////////////////////////////////////////////////
//////Frame
Frame::Frame(size_t length, int32_t streamID, uint8_t type, int flags, int reserved)
{
    m_header = new FrameHeader(length, streamID, type, flags, reserved);

}

Frame::~Frame()
{
    if(m_header)
    {
        delete m_header;
        m_header = nullptr;
    }
}





////////////////////////////////////////////////
//////Settings

// SettingsFrame::SettingsFrame()
//     :m_ivCount(0)
// {

// }

SettingsFrame::SettingsFrame(size_t ivCount, std::vector<SettingsEntry> settingsEntrys, size_t length,
                                                                int32_t streamID, FRAME_TYPE type, uint8_t flags,int reserved)
    : m_ivCount(ivCount)
    , Frame(ivCount * SETTINGS_ENTRY_LENGTH, streamID, type, flags, reserved)
{
    init(settingsEntrys);

}

void SettingsFrame::init(std::vector<SettingsEntry> settingsEntrys)
{
    m_settingsEntrys = settingsEntrys;
}





////////////////////////////////////////////////
//////Headers
HeadersFrame::HeadersFrame(size_t ivCount, std::vector<ivEntry> entrys, STREAM_PRIORITY priority, size_t length, int32_t streamID, 
                                    FRAME_TYPE type, uint8_t flags, int reserved)
    : Frame(ivCount * SETTINGS_ENTRY_LENGTH, streamID, type, flags, reserved)
    , m_niv(ivCount)
    , m_priority(priority)
    , m_padLen(0)
    , m_padding(nullptr)
{
    init();
}

HeadersFrame::~HeadersFrame()
{
    if(m_padLen)
    {
        free(m_padding);
        m_padding  = delete;
        m_padLen = 0;
    }
}



////////////////////////////////////////////////
//////PriorityFrame
PriorityFrame::PriorityFrame(STREAM_PRIORITY priority, size_t length, int32_t streamID, 
                                    FRAME_TYPE type, uint8_t flags, int reserved)
    : Frame(PRIORITY_LENGTH, streamID, type, flags, reserved)
{

}

PriorityFrame::~PriorityFrame()
{

}




////////////////////////////////////////////////
//////RstStreamFrame
RstStreamFrame::RstStreamFrame(uint32_t errorCode, int32_t streamID, 
                                    FRAME_TYPE type = RST_STREAM, uint8_t flags = 0, int reserved = 0)
    : Frame(RST_LENGTH, streamID, type, flags, reserved)
    , m_errorCode(m_errorCode)
{

}

RstStreamFrame::~RstStreamFrame()
{

}





////////////////////////////////////////////////
//////PushPromiseFrame

PushPromiseFrame::PushPromiseFrame(uint32_t promisedStreamID, size_t niv, std::vector<ivEntry> m_entrys, int32_t streamID, 
                                    FRAME_TYPE type, uint8_t flags, int reserved)
    : Frame(0, streamID, type, flags, reserved)//LENGTH?
    , m_promisedStreamID(promisedStreamID)
    , m_niv(niv)
    , m_padLen(0)
    , m_padding(nullptr)
{
    m_entrys = m_entrys;
}

PushPromiseFrame::~PushPromiseFrame()
{
    if(m_padLen)
    {
        free(m_padding);
        m_padding = nullptr;
        m_padLen = 0;
    }
}


////////////////////////////////////////////////
//////WindowsUpdateFrame
WindowsUpdateFrame::WindowsUpdateFrame(uint32_t winSizeincrement,  int32_t streamID, 
                                    FRAME_TYPE type, uint8_t flags, int reserved)
    : Frame(WINUPDATE_LENGTH, streamID, flags, reserved)
    , m_winSizeincrement(winSizeincrement)
{

}








////////////////////////////////////////////////
//////GoAwayFrame
GoAwayFrame::GoAwayFrame(int32_t lastStreamID, int32_t errorcode, uint8_t* additionalData, size_t additionalDataLen, 
                                    int32_t streamID,  FRAME_TYPE type = GOAWAY, uint8_t flags = 0, int reserved = 0)
    : Frame(GOAWAY_LENGTH, streamID, type, flags, reserved)
    , m_lastStreamID(lastStreamID)
    , m_errorcode(errorcode)
    , m_additionalDataLen(additionalDataLen)
    , m_reserved(0)
{
    m_additionalData = (uint8_t*)malloc(m_additionalDataLen);

}

GoAwayFrame::~GoAwayFrame()
{
    if(m_additionalDataLen)
    {
        free(m_additionalData);
        m_additionalData = nullptr;
        m_additionalDataLen = 0;
    }
}



////////////////////////////////////////////////
//////PingFrame
PingFrame::PingFrame(uint8_t* opaqueData, int32_t streamID,  FRAME_TYPE type, uint8_t flags, 
                            int reserved)
    : Frame(PING_LENGTH, streamID, type, flags, reserved)
{
    m_opaqueData = (uint8_t*)malloc(PING_LENGTH * sizeof(uint8_t));
    m_opaqueData = opaqueData;
}

PingFrame::~PingFrame()
{
    if(m_opaqueData)
    {
        free(m_opaqueData);
        m_opaqueData = nullptr;
    }
}

////////////////////////////////////////////////
//////DataFrame

DataFrame::DataFrame(int32_t streamID,  FRAME_TYPE type, uint8_t flags, 
                                                int reserved)
    : Frame(0, streamID, type, flags, reserved)
    , padLen(0)
    , padding(nullptr)
{

}

DataFrame::~DataFrame()
{

}



////////////////////////////////////////////////
//////ContinuationFrame

ContinuationFrame::ContinuationFrame()
{

}

ContinuationFrame::~ContinuationFrame()
{

}




