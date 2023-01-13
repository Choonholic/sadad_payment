#include "loopbuf.h"

#ifdef __cplusplus 
extern "C"{ 
#endif 

#define     MAX_SDLC_FRAME_LEN  1024+128 

#define BB_MODEM_ERROR      BB_ERROR 
#define BB_MODEM_SUCCESS    BB_SUCCESS 
 
#define BB_MODEM_MODE_ASYS  0 
#define BB_MODEM_MODE_SDLC  1 
#define BB_MODEM_MODE_PPP   2 

#define BB_ONLINE_AT        1 
#define BB_ONLINE_DIALING   2 
#define BB_ONLINE_CONNECTED 3 
#define BB_ONLINE_DATA      4 
#define BB_ONLINE_OFFLINE   5 

#define BB_HDLC_TRANSMIT_NODATA			    0
#define BB_HDLC_TRANSMIT_DATA			    1
#define BB_HDLC_TRANSMIT_DATA_NOACK			2

#define UNSIGNED unsigned int

#define BB_DIALERR_NONE         0 
#define BB_DIALERR_NOCARRIER    1 
#define BB_DIALERR_NOANSWER     2 
#define BB_DIALERR_NODIALTONE   3 
#define BB_DIALERR_BUSY         4 
#define BB_DIALERR_TIMEOUT      5 
#define BB_DIALERR_ELSE         6

#define BB_MODEM_CX         1 
#define BB_MODEM_SI         2 
#define BB_MODEM_UNKNOWN    0 

// HDLC 
#define		BB_im_ERR_PACKET_DATA	(uchar)(0x0C) 
#define		BB_im_ERR_PACKET_FORMAT	(uchar)(0x0D) 
#define		BB_im_ERR_PACKET_NONE	(uchar)(0x0E) 
#define		BB_im_ERR_PACKET_REJ	(uchar)(0x0F) 
#define		BB_im_ERR_TXIF_FAIL		(uchar)(0x10) 
#define		BB_im_ERR_POWERUP		(uchar)(0x11) 
#define		BB_im_ERR_POWERDOWN		(uchar)(0x12) 
 
#define		BB_modem_ERRHANDLE		BB_ERRHANDLE		//0x8C 
#define		BB_modem_ERR_PARA		BB_ERRPARAM			//0x8B 
#define		BB_im_ERR_NO_DATAMODE	(uchar)(0x52) 
#define		BB_im_ERR_UN_OBTAINABLE_NUMBER	(uchar)(0x54) 
#define		BB_im_ERR_NO_CMDMODE	(uchar)(0x53) 
#define		BB_modem_DATAMODE		(uchar)(0x51) 

#define		BB_modem_SUCCESS		EM_SUCCESS 
#define		BB_modem_ERROR			EM_ERROR 
#define		BB_modem_FAIL			(uchar)'F' 
#define		BB_modem_NODIALTONE		(uchar)'6' 
#define		BB_modem_PHONEINUSE		(uchar)'P' 
#define		BB_modem_NOCARRIER		(uchar)'3' 
#define		BB_modem_LINEBUSY		(uchar)'7' 
#define		BB_modem_HANGUP			(uchar)'O' 
#define		BB_modem_INDIAL			(uchar)'D' 
#define		BB_modem_RECVNOACK		(uchar)'G' 
#define		BB_modem_ATCOMRIGHT		(uchar)'0' 
#define		BB_modem_ATCOMERROR		(uchar)'E' 
#define		BB_modem_INPROCESS		(uchar)'C' 
#define		BB_modem_TIMEOUT		(uchar)'V' 
#define     BB_modem_NOPATCH        (uchar)'Z'

#define     BB_modem_CNT            (uchar)(0xC0)
#define 	BB_modem_CNT300V21		(uchar)(0xA0) 
#define 	BB_modem_HDLC300V21		(uchar)(0xA1) 
 
#define		BB_modem_CNT1200V22		(uchar)(0xA2) 
#define 	BB_modem_HDLC1200V22	(uchar)(0xA3) 
#define 	BB_modem_CNT1200V22V42BIS	(uchar)(0xA4) 
 
#define 	BB_modem_CNT2400V22B		(uchar)(0xA5) 
#define		BB_modem_HDLC2400V22B	    (uchar)(0xA6) 
#define 	BB_modem_CNT2400V22BV42BIS  (uchar)(0xA7) 
#define		BB_modem_CNT2400V34		(uchar)(0xA8) 
 
#define		BB_modem_CNT4800V34		(uchar)(0xA9) 
 
#define		BB_modem_CNT9600V32		(uchar)(0xAA) 
#define 	BB_modem_CNT9600V32V42BIS	(uchar)(0xAB) 
#define		BB_modem_CNT9600V34		(uchar)(0xAC) 
 
 
#define 	BB_modem_CNT14400V32B		(uchar)(0xAD) 
#define 	BB_modem_CNT14400V32BV42BIS (uchar)(0xAE) 
#define		BB_modem_CNT14400V34		(uchar)(0xAF) 
 
#define		BB_modem_CNT19200V34		(uchar)(0xB1) 
#define 	BB_modem_CNT19200V34V42BIS	(uchar)(0xB2) 
 
#define 	BB_modem_CNT24000V34		(uchar)(0xB3) 
#define 	BB_modem_CNT24000V34V42BIS	(uchar)(0xB4) 
 
#define 	BB_modem_CNT26400V34		(uchar)(0xB5) 
#define 	BB_modem_CNT26400V34V42BIS	(uchar)(0xB6) 
 
#define		BB_modem_CNT28800V34		(uchar)(0xB7) 
#define 	BB_modem_CNT28800V34V42BIS	(uchar)(0xB8) 
 
#define		BB_modem_CNT33600V34		(uchar)(0xB9) 
#define		BB_modem_CNT28800V8		(uchar)(0xBA) 
#define		BB_modem_CNT33600V34V42BIS	(uchar)(0xBB)

#define DBG_MSG 1
#define DBG_LOW_LEVEL_MSG 1

typedef struct 
{ 
    unsigned int rece_data_flag; 
    unsigned int send_data_flag; 
    unsigned int txrate; 
    unsigned int rxrate; 
}_v80_s;
struct hdlc_frame_info 
{ 
    unsigned int  type;     /* HDLC Frame Type*/ 
    unsigned char addr;     /* Address */ 
    unsigned char nr;       /*N(R), 다음번 전송때 수신 받을 프래임 넘버*/ 
    unsigned char ns;       /*N(S), 이번에 전송 받은 프레임 넘버*/ 
    unsigned char pf;       /*Poll(command)/Final(response)*/ 
    unsigned char control;  /*컨트롤 데이터*/ 
};
 
typedef struct 
{ 
    uchar addr; 
    uchar can_send; 
    uchar send_flag; 
    uchar frmr_type; 
    unsigned int status; 
    unsigned int vr; 
    unsigned int vs; 
    unsigned int re_cnt;
    unsigned int re_count;
    struct hdlc_frame_info frmr_data; 
    unsigned char resend_buf[MAX_SDLC_FRAME_LEN];
    unsigned int  resend_len;
}_hdlc_s; 
 
typedef struct 
{ 
    char   			v80_sendbuf[MAX_SDLC_FRAME_LEN*2]; 
    unsigned int    v80_sendbuf_len; 
    _loopbuf  		*hdlc_recvbuf; 
    _loopbuf  		*v80_recvbuf;     
    _loopbuf  		*serial_recvbuf;
	_v80_s    		v80_s; 
    _hdlc_s   		hdlc_s;
	HANDLE          hSerialTask;
	unsigned int 	mode;
	unsigned int 	modem;
	UNSIGNED    	dial_start;
	UNSIGNED        ring_start; 
    UNSIGNED        dialoff_start; 
	unsigned int 	online;
	char    		dialing_buf[256]; 
    unsigned int    dialing_buf_len; 
    unsigned int    dial_err;
	unsigned int    max_time_toconnect; 
    unsigned int    max_time_todata; 
	UNSIGNED    	dial_connect;
    HANDLE    		drv_api_SEM;
	int             bQuitThread;
}BB_modem_STS; 

/* ISOModem_HDLC.c */ 
void BB_SetHdlc_sendflag(uchar status); 
void BB_Init_hdlc(void); 
void BB_hdlc_protocol_process(void); 
void BB_hdlc_send_iframe(unsigned char *buf , unsigned int len); 
 
/* ISOModem_v80.c */ 
void BB_hdlc_receive_v80tohdlc(unsigned char temp); 
char BB_hdlc_transmit_hdlctov80(unsigned char *buf , unsigned int len); 
void BB_Init_v80(void);

void BB_SetModemOnline(unsigned int online);
void BB_ModemOffline(void);
void BB_hdlc_recv_snrm(struct hdlc_frame_info *frame)  ;

char BB_hdlc_insert_rece(unsigned char i);   
char BB_hdlc_insert_send(unsigned char *buf , unsigned char i);   
char BB_hdlc_send_v80(unsigned char *buf);

void BB_SendAT(char *atcmd);

/* PSTN_AT.c */ 
int BB_WriteUart(char *buf,unsigned int len); 
void BB_ClearUartRecvBuf(void); 
int BB_ReadUart_OK(unsigned int time); 
int BB_SendAT_OK(char *atcmd,unsigned int time); 
int BB_SendATE0_OK(void); 
void BB_SendAT(char *atcmd); 
unsigned int BB_CalcCrLf(char *buf); 
uchar BB_si_AT_R6C(int	*piEcho); 
uchar BB_cx_AT_TRV(float	*pfEcho); 
uchar BB_cx_AT_IDC(float	*pfEcho); 
uchar BB_At_TillStr(char *atcmd,char *resp,unsigned int time); 
uchar BB_SendAT_OK_Recv(char *atcmd,char *resp,unsigned int max,unsigned int time);

 #ifdef __cplusplus 
} 
#endif 