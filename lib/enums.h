#ifndef _ENUMS_H
#define _ENUMS_H

typedef enum {
	CLIENT = -1,
	SERVER = 1
} Role;

typedef enum {
	WRN_PM, SVR_CL,
	SCK_FAIL, SSO_FAIL, BND_FAIL,
	LSN_FAIL, ACP_FAIL, INT_FAIL, 
	CNT_FAIL, ERR_LEN
} ErrorType;

#endif
