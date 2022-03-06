#ifndef FRAME_h
#define FRAME_h

#include"http2.h"

#include <stdlib.h>
// #include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stdarg.h>
#include <vector>
#include<string>
#include<memory>



struct STREAM_PRIORITY
{
    STREAM_PRIORITY();
    int32_t streamDependancy;
    int32_t weigth;
    uint8_t exclusive;
};

class FrameHeader
{
public:
    FrameHeader(size_t length, int32_t streamID, uint8_t m_type, int flags, int reserved=0);

    // void init();
    
    void SetLength(size_t length) { m_length = length; }
    size_t GetLength() { return m_length; }

    void SetStreamID(int32_t streamID) { m_streamID = streamID; }
    int32_t GetStreamID() { return m_streamID; }


protected:
    size_t m_length;
    int32_t m_streamID;
    uint8_t m_type;
    uint8_t m_flags;
    uint8_t m_reserved;
};


class Frame
{
public:
    Frame(size_t length, int32_t streamID, uint8_t m_type, int flags, int reserved=0);
    virtual  ~Frame();

    virtual void init() = 0;

    virtual void pack() = 0;

    virtual void unpack() = 0;


protected:
    FrameHeader* m_header;

};


class SettingsFrame : public Frame
{
    enum SettingsID
    {
        SETTINGS_HEADER_TABLE_SIZE = 0x01,

        SETTINGS_ENABLE_PUSH = 0x02,

        SETTINGS_MAX_CONCURRENT_STREAMS = 0x03,

        SETTINGS_INITIAL_WINDOW_SIZE = 0x04,

        SETTINGS_MAX_FRAME_SIZE = 0x05,

        SETTINGS_MAX_HEADER_LIST_SIZE = 0x06,

        SETTINGS_ACCEPT_CACHE_DIGEST = 0x07,

        SETTINGS_ENABLE_CONNECT_PROTOCOL = 0x08
    };
    struct SettingsEntry
    {
        SettingsID id;

        int32_t value;
    };

public:
    // SettingsFrame();
    SettingsFrame(size_t ivCount, std::vector<SettingsEntry> settingsEntrys, size_t length, int32_t streamID, 
                                    FRAME_TYPE type = SETTINGS, uint8_t flags = 0, int reserved = 0);
    void init( std::vector<SettingsEntry> settingsEntrys);

protected:
    
    using ivEntry = std::pair<std::string, std::string>;

    size_t m_ivCount; //number of  id-val pair

    std::vector<SettingsEntry> m_settingsEntrys;
};


class HeadersFrame : public Frame
{
public:
    HeadersFrame(size_t ivCount, std::vector<ivEntry> entrys, STREAM_PRIORITY priority, size_t length, int32_t streamID, 
                                    FRAME_TYPE type = HEADERS, uint8_t flags = 0, int reserved = 0);
    ~HeadersFrame();
    void init();

protected:
    using ivEntry = std::pair<std::string, std::string>;
    STREAM_PRIORITY m_priority;

    size_t m_niv;
    
    std::vector<ivEntry > m_entrys;
    
    size_t m_padLen;

    char* m_padding; 
};

class PriorityFrame : public Frame
{
public:
    PriorityFrame(STREAM_PRIORITY priority, size_t length, int32_t streamID, 
                                    FRAME_TYPE type = PRIORITY, uint8_t flags = 0, int reserved = 0);
    void init();

protected:
    STREAM_PRIORITY m_priority;
};


class RstStreamFrame : public Frame
{
public:
    RstStreamFrame(uint32_t m_errorCode, int32_t streamID, 
                                    FRAME_TYPE type = RST_STREAM, uint8_t flags = 0, int reserved = 0);
    ~RstStreamFrame();
    void init();

protected:
    uint32_t m_errorCode;
};


class PushPromiseFrame : public Frame
{
public:
    PushPromiseFrame(uint32_t promisedStreamID,size_t niv, std::vector<ivEntry> m_entrys, int32_t streamID, 
                                    FRAME_TYPE type = PUSH_PROMISE, uint8_t flags = 0, int reserved = 0);
    ~PushPromiseFrame();
    void init();

protected:
    using ivEntry = std::pair<std::string, std::string>;
    uint32_t m_promisedStreamID;
    std::vector<ivEntry> m_entrys;
    size_t m_niv;
    uint8_t m_reserved;
    uint8_t m_padLen;
    char* m_padding;
};


class WindowsUpdateFrame : public Frame
{
public:
    WindowsUpdateFrame(uint32_t winSizeincrement,  int32_t streamID, 
                                    FRAME_TYPE type = WINDOW_UPDATE, uint8_t flags = 0, int reserved = 0);
    void init();

protected:
    uint32_t m_winSizeincrement;
};


class GoAwayFrame : public Frame
{
public:
    GoAwayFrame(int32_t lastStreamID, int32_t errorcode, uint8_t* additionalData, size_t additionalDataLen, 
                                    int32_t streamID,  FRAME_TYPE type = GOAWAY, uint8_t flags = 0, int reserved = 0);
    ~GoAwayFrame();
    void init();

protected:

    int32_t m_lastStreamID;
    int32_t m_errorcode;
    uint8_t* m_additionalData;
    size_t m_additionalDataLen;
    uint8_t m_reserved;
};


class PingFrame : public Frame
{
public:
    PingFrame(uint8_t* opaqueData, int32_t streamID,  FRAME_TYPE type = GOAWAY, uint8_t flags = 0, 
                            int reserved = 0);
    ~PingFrame();
    void init();

protected:
    uint8_t* m_opaqueData;
    size_t m_opaqueDataLen;
};



class DataFrame : public Frame
{
public:
    DataFrame(int32_t streamID,  FRAME_TYPE type = DATA, uint8_t flags = 0, 
                            int reserved = 0);
    void init();

protected:

    size_t padLen;

    char* padding;

    // char* data;
};



class ContinuationFrame : public Frame
{
public:
    ContinuationFrame();
    void init();

protected:
    char* m_headerBlockFragment;

    size_t blockLen;
};



class ExtensionFrame : public Frame
{
    public:
    void init();

protected:
    char* payload;

    size_t payloadLen;
};

















#endif