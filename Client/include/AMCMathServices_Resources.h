#pragma once
#include <Windows.h>

#define	IDBM_AMCLOGO	01

#define	IDPB_OK	98
#define	IDPB_CANCEL	99
#define	IDPB_LOG	100

#define	IDRB_REGISTERED	101
#define	IDRB_RETIRING	102
#define	IDRB_REGISTERING	103

#define IDET_REGID	120
#define IDET_FNAME	121
#define IDET_MNAME	122
#define IDET_LNAME	123
#define IDET_ADDITION_X	124
#define IDET_ADDITION_Y	125
#define IDET_SUBTRACTION_X	126
#define IDET_SUBTRACTION_Y	127
#define IDET_MULTIPLICATION_X	128
#define IDET_MULTIPLICATION_Y	129
#define IDET_DIVISION_X	130
#define IDET_DIVISION_Y	131
#define IDET_SQUARE_X	132
#define IDET_CUBE_X	133
#define IDET_SQUAREROOT_X	134
#define IDET_CUBEROOT_X	135
#define IDET_SINE_X	136
#define	IDET_COSINE_X	137

#define IDCOMBO_ADDRESS	140

#define IDST_ADDRESS	160
#define IDST_BASIC1	161
#define IDST_BASIC2	162
#define IDST_POWER1	163
#define IDST_POWER2	164
#define IDST_TRIG	165
#define IDST_ERRORMESSAGE	166

#define IDCB_ADDITION	180
#define IDCB_SUBTRACTION	181
#define IDCB_MULTIPLICATION	182
#define IDCB_DIVISION	183
#define IDCB_SQUARE	184
#define IDCB_CUBE	185
#define IDCB_SQUAREROOT	186
#define IDCB_CUBEROOT	187
#define IDCB_SINE	188
#define	IDCB_COSINE	189

#define IDPB_RDLG_OK	200
#define IDST_RDLG_RESULT	201
#define IDMI_RDLG_ABOUT	202
#define IDMI_RDLG_QUIT	203

#define IDPB_ADLG_EXIT	221

#define IDI_NAMASAKARAM	241


/* Array of States and their Capital Cities for the Combo Box */
TCHAR* pszAddresses[36][2] = {
	{TEXT("ANDAMAN & NICOBAR"),	TEXT("PORT BLAIR")},
	{TEXT("CHANDIGADH"),		TEXT("CHANDIGADH")},
	{TEXT("DAMAN & DIU"),		TEXT("DAMAN")},
	{TEXT("DELHI"),				TEXT("NEW DELHI")},
	{TEXT("JAMMU & KASHMIR"),	TEXT("SHRINAGAR")},
	{TEXT("LADAKH"),			TEXT("LEH")},
	{TEXT("LAKSHADWEEP"),		TEXT("KAVARATTI")},
	{TEXT("PUDUCHERRY"),		TEXT("PUDUCHERRY")},
	{TEXT("ANDHRA PRADESH"),	TEXT("AMARAVATI")},
	{TEXT("ARUNACHAL PRADESH"),	TEXT("ITANAGAR")},
	{TEXT("ASSAM"),				TEXT("DISPUR")},
	{TEXT("BIHAR"),				TEXT("PATNA")},
	{TEXT("CHHATTISGADH"),		TEXT("RAIPUR")},
	{TEXT("GOA"),				TEXT("PANAJI")},
	{TEXT("GUJARAT"),			TEXT("GANDHINAGAR")},
	{TEXT("HARYANA"),			TEXT("CHANDIGADH")},
	{TEXT("HIMACHAL PRADESH"),	TEXT("SHIMLA")},
	{TEXT("JHARKHAND"),			TEXT("RANCHI")},
	{TEXT("KARNATAK"),			TEXT("BENGALURU")},
	{TEXT("KERALA"),			TEXT("THIRUVANANTHAPURAM")},
	{TEXT("MADHYA PRADESH"),	TEXT("BHOPAL")},
	{TEXT("MAHARASHTRA"),		TEXT("MUMBAI")},
	{TEXT("MANIPUR"),			TEXT("IMPHAL")},
	{TEXT("MEGHALAY"),			TEXT("SHILLONG")},
	{TEXT("MIZORAM"),			TEXT("AIZAWL")},
	{TEXT("NAGALAND"),			TEXT("KOHIMA")},
	{TEXT("ODISHA"),			TEXT("BHUVANESHWAR")},
	{TEXT("PUNJAB"),			TEXT("CHANDIGADH")},
	{TEXT("RAJASTHAN"),			TEXT("JAIPUR")},
	{TEXT("SIKKIM"),			TEXT("GANGTOK")},
	{TEXT("TAMIL NADU"),		TEXT("CHENNAI")},
	{TEXT("TELANGANA"),			TEXT("HYDERABAD")},
	{TEXT("TRIPURA"),			TEXT("AGARTALA")},
	{TEXT("UTTAR PRADESH"),		TEXT("LUCKNOW")},
	{TEXT("UTTARAKHAND"),		TEXT("DEHRADUN")},
	{TEXT("PASCHIM BANGAL"),	TEXT("KALKATTA")}
};
