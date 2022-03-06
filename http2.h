#ifndef HTTP2_H
#define HTTP2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
// #include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stdarg.h>
#include <vector>
#include<string>
#include<memory>

#define H2_PROTO_VERSION_ID "h2"

#define H2_PROTO_VERSION_ID_LEN 2

#define H2_PROTO_ALPN "\x2h2" 

#define H2_PROTO_ALPN_LEN (sizeof(H2_PROTO_ALPN) - 1)

#define H2_CLEARTEXT_PROTO_VERSION_ID "h2c"

#define H2_CLEARTEXT_PROTO_VERSION_ID_LEN 3

#define H2_VERSION_AGE 1

struct H2_INFO
{
    int age;
    int version_num;
    const char* version_str;
    const char* proto_str;
}

#define HTTP2_DEFAULT_WEIGHT 16

#define H2_MAX_WEIGHT 256
#define H2_MIN_WEIGHT 1
#define H2_MAX_WINDOW_SIZE ((int32_t)((1U << 31) - 1))
#define H2_INITIAL_WINDOW_SIZE ((1 << 16) - 1)
#define H2_INITIAL_CONNECTION_WINDOW_SIZE ((1 << 16) - 1)
#define H2_DEFAULT_HEADER_TABLE_SIZE (1 << 12)


#define HTTP2_CLIENT_MAGIC "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"

#define HTTP2_CLIENT_MAGIC_LEN 24

enum H2_ERROR{
  
  H2_ERR_INVALID_ARGUMENT = -501,
  
  H2_ERR_BUFFER_ERROR = -502,
  
  H2_ERR_UNSUPPORTED_VERSION = -503,
  
  H2_ERR_WOULDBLOCK = -504,
  
  H2_ERR_PROTO = -505,
  
  H2_ERR_INVALID_FRAME = -506,
  
  H2_ERR_EOF = -507,
  
  H2_ERR_DEFERRED = -508,
  
  H2_ERR_STREAM_ID_NOT_AVAILABLE = -509,
  
  H2_ERR_STREAM_CLOSED = -510,
  
  H2_ERR_STREAM_CLOSING = -511,
  
  H2_ERR_STREAM_SHUT_WR = -512,
 
  H2_ERR_INVALID_STREAM_ID = -513,
  
  H2_ERR_INVALID_STREAM_STATE = -514,
  
  H2_ERR_DEFERRED_DATA_EXIST = -515,

  H2_ERR_START_STREAM_NOT_ALLOWED = -516,
 
  H2_ERR_GOAWAY_ALREADY_SENT = -517,

  H2_ERR_INVALID_HEADER_BLOCK = -518,

  H2_ERR_INVALID_STATE = -519,

  H2_ERR_TEMPORAL_CALLBACK_FAILURE = -521,

  H2_ERR_FRAME_SIZE_ERROR = -522,

  H2_ERR_HEADER_COMP = -523,
  
  H2_ERR_FLOW_CONTROL = -524,

  H2_ERR_INSUFF_BUFSIZE = -525,

  H2_ERR_PAUSE = -526,

  H2_ERR_TOO_MANY_INFLIGHT_SETTINGS = -527,

  H2_ERR_PUSH_DISABLED = -528,

  H2_ERR_DATA_EXIST = -529,

  H2_ERR_SESSION_CLOSING = -530,

  H2_ERR_HTTP_HEADER = -531,

  H2_ERR_HTTP_MESSAGING = -532,

  H2_ERR_REFUSED_STREAM = -533,

  H2_ERR_INTERNAL = -534,

  H2_ERR_CANCEL = -535,
 
  H2_ERR_FATAL = -900,

  H2_ERR_NOMEM = -901,

  H2_ERR_CALLBACK_FAILURE = -902,

  H2_ERR_BAD_CLIENT_MAGIC = -903,
 
  H2_ERR_FLOODED = -904
};


enum FRAME_TYPE{ 
    DATA = 0,
  
    HEADERS = 0x01,
  
    PRIORITY = 0x02,
  
    RST_STREAM = 0x03,

    SETTINGS = 0x04,
  
    PUSH_PROMISE = 0x05,
 
    PING = 0x06,
  
    GOAWAY = 0x07,
  
    WINDOW_UPDATE = 0x08,
  
    CONTINUATION = 0x09,
  
    ALTSVC = 0x0a
};

enum H2_FLAG{ 
    H2_FLAG_NONE = 0,

    H2_FLAG_END_STREAM = 0x01,

    H2_FLAG_END_HEADERS = 0x04,

    H2_FLAG_ACK = 0x01,

    H2_FLAG_PADDED = 0x08,

    H2_FLAG_PRIORITY = 0x20  
};

enum H2_SETTINGS_ID{
    SETTINGS_HEADER_TABLE_SIZE = 0x01,
  
    SETTINGS_ENABLE_PUSH = 0x02,
  
    SETTINGS_MAX_CONCURRENT_STREAMS = 0x03,
  
    SETTINGS_INITIAL_WINDOW_SIZE = 0x04,
  
    SETTINGS_MAX_FRAME_SIZE = 0x05,
  
    SETTINGS_MAX_HEADER_LIST_SIZE = 0x06,

    SETTINGS_ACCEPT_CACHE_DIGEST = 0x07,

    SETTINGS_ENABLE_CONNECT_PROTOCOL = 0x08
};


struct H2_FRAME_HEADER
{
    size_t length;

    int32_t stream_id;

    uint8_t type;

    uint8_t flags;

    uint8_t reserved;

};



enum H2_HEADERS_CATEGORY
{
    HC_REQUESET = 0,

    HC_RESPONSE = 1,

    HC_PUSH_RESPONSE = 2,

    HC_HEADERS = 3
};

struct STREAM_PRIORITY
{
    int32_t streamDependancy;

    int32_t weigth;
    
    uint8_t exclusive;
};

struct H2_SETTINGS_ENTRY
{
    H2_SETTINGS_ID settingsID;

    int32_t value;
};


struct H2_SETTINGS_FRAME
{
    H2_FRAME_HEADER header;

    size_t ivCount; //number of  id-val pair

    std::vector<H2_SETTINGS_ENTRY> settingsEntrys;
};

using ivEntry = std::pair<std::string, std::string>;

struct H2_HEADERS_FRAME
{
    H2_FRAME_HEADER header;

    STREAM_PRIORITY priority;

    size_t niv;
    
    std::vector<ivEntry > entrys;

    size_t padLen;

    char* padding; 
};

struct H2_PRIORITY_FRAME
{
    H2_FRAME_HEADER header;

    STREAM_PRIORITY priority;
};

struct H2_RST_STRAM_FRAME
{
    H2_FRAME_HEADER header;

    uint32_t errorCode;
};

struct H2_PUSH_PROMISE_FRAME//TODO
{
    H2_FRAME_HEADER header;

    uint32_t promisedStreamID;

    std::vector<ivEntry> entrys;

    size_t niv;

    uint8_t reserved;

    uint8_t padLen;

    char* padding;
};

struct H2_WINDOWS_UPDATE_FRAME
{
    H2_FRAME_HEADER header;

    int32_t windowSizeIncrement;

    uint8_t reserved;
};

struct H2_GO_AWAY_FRAME
{
    H2_FRAME_HEADER header;

    int32_t lastStreamID;

    int32_t errorcode;

    char* additionalData;

    size_t additionalDataLen;

    uint8_t reserved;
};

struct H2_PING_FRAME
{
    H2_FRAME_HEADER header;

    uint8_t opaqueData[8];
};

struct H2_DATA_FRAME //TODO
{
    H2_FRAME_HEADER header;

    size_t padLen;

    char* padding;

    char* data;
};

struct H2_CONTINUATION_FRAME
{
    H2_FRAME_HEADER header;

    char* HeaderBlockFragment;
};

struct H2_EXTENSION_FRAME//TODO
{
    H2_FRAME_HEADER header;

    uint8_t* payload;
};

union H2_Frame
{
    H2_FRAME_HEADER header;

    H2_DATA_FRAME dataFrame;

    H2_HEADERS_FRAME headerFrame;

    H2_RST_STRAM_FRAME rstFrame;

    H2_SETTINGS_FRAME settingsFrame;

    H2_PUSH_PROMISE_FRAME pushFrame;

    H2_PING_FRAME pingFrame;

    H2_PRIORITY_FRAME priorityFrame;

    H2_GO_AWAY_FRAME goawayFrame;

    H2_WINDOWS_UPDATE_FRAME winupdateFrame;

    H2_EXTENSION_FRAME extensionFrame;
};


































#endif //HTTP2_H




