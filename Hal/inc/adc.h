/**
******************************************************************************
* @file
* @author
* @version
* @date
* @brief
*
******************************************************************************
*/
#ifndef _ADC_H
#define _ADC_H

#include "define.h"
/******************************************************************************/
typedef void (*Photo_Init)(HardLink_t *hardlink);
typedef void (*Photo_Open)(void);
typedef void (*Photo_Close)(void);
/*****************************************************************************/
/*! \brief Photo State
 *
 */
typedef enum
{
	DAY    = (uint8_t)0x00,
	NIGHT  = (uint8_t)0x01
}enPhotoState;

typedef struct
{
  HardLink_t hardLink;  /*硬件连接*/
  Photo_Open open;      /*开*/
  Photo_Close close;    /*关*/
  enPhotoState state;
}PhotoControl_t;
/*****************************************************************************/
void PhotoADC_Init(HardLink_t *hardlink);

/*! \brief Photo ADC Open
 *
 */
void PhotoADC_Open(void);

/*! \brief Photo ADC Close
 *
 */
void PhotoADC_Close(void);


/*! \brief Get Photo State
 *
 */
enPhotoState GetPhotoState(void);

#endif