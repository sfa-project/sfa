/*
 * 	Module Name : sfa module
 * 	Description :
 * 	Author 		: eric
 * 	Date		:2014
 *
 */


#ifndef SFA_H_ERIC
#define SFA_H_ERIC 1

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "list.h"
#include "ofpbuf.h"
#include "util.h"
#include "hmap.h"
#include "ofp-msgs.h"


#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_ENTRY 500

enum sfaerr{
	SFA_MSG_PROCESS_OK,
	SFA_MSG_PROCESS_ERROR,
};

/* SFA extension. */
enum sfatype{

	OFPTYPE_SFA_TABLE_CREATE = 90,	/* sfa_craete_table msg type*/
	OFPTYPE_SFA_ST_ENTRY_MOD = 91,	/* sfa_st_entry_mode msg type*/
	OFPTYPE_SFA_AT_ENTRY_MOD = 92,	/* sfa_at_entry_mode msg type*/

};



// table set is the table structure in ovs

//****** STATUS TARANZ TABLE SET *****************************/
enum PARAM_TYPE
{
	PARAM_NON = 0,
	SFAPARAM_IN_PORT     = 1 ,  /* Switch input port. */
	SFAPARAM_DL_VLAN     = 2,  /* VLAN id. */
	SFAPARAM_DL_VLAN_PCP = 3,  /* VLAN priority. */
	SFAPARAM_DL_TYPE     = 4,  /* Ethernet frame type. */
	SFAPARAM_NW_TOS      = 5,  /* IP ToS (DSCP field, 6 bits). */
	SFAPARAM_NW_PROTO    = 6,  /* IP protocol. */
	SFAPARAM_TP_SRC      = 7,  /* TCP/UDP/SCTP source port. */
	SFAPARAM_TP_DST      = 8,  /* TCP/UDP/SCTP destination port. */
	SFAPARAM_TP_FLAG	 = 9,  /* TCP FLAG  */
	SFAPARAM_DL_SRC      = 10, /* Ethernet source address. */
	SFAPARAM_DL_DST      = 11, /* Ethernet destination address. */
	SFAPARAM_NW_SRC      = 12, /* IP source address. */
	SFAPARAM_NW_DST      = 13, /* IP destination address. */
	SFAPARAM_METADATA    = 14, /* Upper level data	*/
	SFAPARAM_CONST		 = 15,
} ;
enum OPRATOR
{
	OPRATOR_NON = 0,
	OPRATOR_ISEQUAL,	/* = */
	OPRATOR_ADD,		/* + */
	OPRATOR_SUB, 		/* - */
	OPRATOR_BITAND,		/* & */
	OPRATOR_BITOR, 		/* | */
	OPRATOR_BITWISE,	/* ^ */
	OPRATOR_GREATER,    /* > */
	OPRATOR_LESS,		/* < */
	OPRATOR_EQUALGREATER,	/* >= */
	OPRATOR_EQUALLESS,		/* <= 8 */
};

struct STT_MATCH_ENTRY{
	struct list node;
	enum PARAM_TYPE param_left_type;
	uint64_t param_left;
	enum OPRATOR oprator;
	enum PARAM_TYPE param_right_type;
	uint64_t param_right;
	uint32_t last_status;
	uint32_t cur_status;
} ;
struct STATUS_TRANZ_TABLE
{
	struct list stt_entrys; //store STT_MATCH_ENTRYs
};
//******** STATUS TARANZ TABLE SET  END *********************



//********** ACTION TABLE SET ******************************/
//SFA_ACTION IS ENUM+PARAM
//AT_MATCH_ENTRY is like ST_MATCH_ENTRY
struct SFA_ACTION
{
	enum SFA_ACTION_TYPE
	{
		SAT_OUTPUT,
		SAT_DROP,
		SAT_TOOPENFLOW,
	} actype;
	uint32_t acparam;
};
struct AT_MATCH_ENTRY{
	struct hmap_node node;
	char* data;
	uint32_t last_status;
	struct SFA_ACTION act;
};

struct ACTION_TABLE
{
	uint64_t bitmap;
	struct hmap at_entrys;  // store AT_MATCH_ENTRYS

};
//********** ACTION TABLE SET END **************************/



//*********** STATUS TABLE SET**************************/
//st table entry use hmap_node to store hash info
// hmap is stored in st, one st has one hmap

struct ST_MATCH_ENTRY{
	struct hmap_node node;
	char* data;
	uint32_t last_status;
};

struct STATUS_TABLE{
	uint64_t match_bitmap;
	struct hmap st_entrys;	// store ST_MATCH_ENTRYs
};
//*********** STATUS TABLE SET END *********************/


//************ CONTROLLERAPP SET ***********************/
struct CONTROLLAPP
{
	struct list node;
	uint32_t appid;
	struct STATUS_TABLE* pst;
	struct STATUS_TRANZ_TABLE* pstt;
	struct ACTION_TABLE* pat;

};

//************* CONTROLLERAPP SET END ********************/


struct APPS
{
	//private vars
	bool islistinit;
	struct list appslist; //point to CONTROLLAPPs
}g_apps;


//************  MSG PROTOCOL *******************************


struct sfa_msg_init_st{
	uint32_t aid;
	uint64_t mmp;
	uint32_t counts;
	//uint32_t lenth;
	//byte[] data;

//	if counts is 0 means no st entry else the next ele is
//	#counts of struct data{
//		uint32_t lenth;
//		byte[] data}

}; //align to 8 BYTE

/*
struct sfa_msg_init_stt{
	enum PARAM_TYPE param_left_type;
	uint64_t param_left;
	enum PARAM_TYPE param_right_type;
	uint64_t param_right;
	enum OPRATOR oprator;
	uint32_t last_status;
	uint32_t cur_status;
};

struct sfa_msg_init_stt{
	uint32_t counts;

*/
struct sfa_msg_init_stt
{
	enum PARAM_TYPE param_left_type;
	uint64_t param_left;
	enum PARAM_TYPE param_right_type;
	uint64_t param_right;
	enum OPRATOR oprator;
	uint32_t last_status;
	uint32_t cur_status;
};


struct sfa_msg_init_at{
	uint64_t bitmap;
	uint32_t counts;
};
/*
	#counts of structure
	{
	struct SFA_ACTION act;
	uint32_t last_status;
	uint32_t data_len;
	byte[] data;

	}
}

*/


//  mode type
enum MOD_TYPE
{
	ENTRY_ADD,
	ENTRY_UPDATE, //for action table update means update the action or status, for status table ,update means update status
	ENTRY_DEL,
};


struct sfa_msg_mod{
	uint32_t appid;
	uint32_t count;
	/*
		#counts of structure if is action table mod
		{
		enum TABLE_MOD_TYPE mod type;
		struct SFA_ACTION act;
		uint32_t last_status;
		uint32_t data_len;
		byte[] data;
		}
	*/
	/*
		#counts of structure if is status table mod
		{
		enum TABLE_MOD_TYPE mod type;
		uint32_t last_status;
		uint32_t data_len;
		byte[] data;
		}
		*/


};




/*	 sfa init msg pattern
 *
 *
 *  |--------------------------32bit ------------------------------|
 * 	+--------------------------------------------------------------+
 *  + version(8)   +   type(8)=90 +           length(16)           +
 *  + -------------------------------------------------------------+
 *  +						xid(32)								   +
 *  +--------------------------------------------------------------+
 *  +						appid(32)
 *  +--------------------------------------------------------------+
 *  +				status table.bitmap(32/64)
 *  +--------------------------------------------------------------+
 *  +				status table.bitmap(64/64)
 *  +--------------------------------------------------------------+
 *  + 				status table data.counts(32)
 *  +--------------------------------------------------------------+
 *  + 				if counts != 0 , see struct data{}
 *  +--------------------------------------------------------------+
 *  +            status transition table counter(32)               +
 *  +--------------------------------------------------------------+
 *  + 			    event  left param type (32)
 *  +--------------------------------------------------------------+
 *  +				event left param value(32/64)
 *  +--------------------------------------------------------------+
 *  +				event left param value(64/64)
 *  +--------------------------------------------------------------+
 *  + 			    event right param type (32)
 *  +--------------------------------------------------------------+
 *  +				event right param value(32/64)
 *  +--------------------------------------------------------------+
 *  +				event right param value(64/64)
 *  +--------------------------------------------------------------+
 *  +               event param operator(32)
 *  +---------------------------------------------------------------
 *  +				status transition table.laststatus(32)
 *  +--------------------------------------------------------------+
 *  +               status transition table.nextstatus(32)
 *  +---------------------------------------------------------------
 *  +          .... #(counter-1) of stt entrys.......
 *  +---------------------------------------------------------------
 *  +             action table bitmap(32/64)
 *  +----------------------------------------------------------------
 *  + 			  	action table bitmap(64/64)
 *  +--------------------------------------------------------------+
 *  +				action table.count(32)
 *  +--------------------------------------------------------------+
 *  +			...#(count-1) of at entrys ....
 *  +--------------------------------------------------------------+
 *  + 			    action table.type(32)
 *  +--------------------------------------------------------------+
 *  +             	action table.param(32)
 *  +-----------------------------------------------------------------
 *  +             	action table.laststatus(32)
 *  +-----------------------------------------------------------------
 *  +             	action table.data_len(32)
 *  +-----------------------------------------------------------------
 *  +             	action table.data
 *  +-----------------------------------------------------------------
 *  +             .........................
 *  +-----------------------------------------------------------------

 */
//
//enum sfaerr sfa_msg_init(struct ofconn *ofconn, const struct ofp_header *sfah );
//enum sfaerr sfa_msg_st_mod(struct ofconn *ofconn, const struct ofp_header *sfah);
//enum sfaerr sfa_msg_at_mod(struct ofconn *ofconn, const struct ofp_header *sfah);
//int sfa_handle_pkt(struct ofconn *ofconn, const struct ofpbuf *msg);
//
//



#ifdef  __cplusplus
}
#endif



#endif
