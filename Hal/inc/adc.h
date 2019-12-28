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
/*! \brief Photo State
 *
 */
typedef void (*Photo_Init)(HardLink_t *hardlink);
typedef void (*Photo_Open)(void);
typedef void (*Photo_Close)(void);
typedef enPhotoState (*Photo_GetState)(void);
typedef void (*Photo_Process)(enPhotoState state);
/*****************************************************************************/
typedef struct
{
  HardLink_t hardLink;  /*硬件连接*/
  Photo_Open open;      /*开*/
  Photo_Close close;    /*关*/
  Photo_GetState GetState;
  Photo_Process process;
  enPhotoState state;
}PhotoControl_t;
/*****************************************************************************/
extern PhotoControl_t PhotoDev;
/*****************************************************************************/
void ADC_Init(PhotoControl_t *dev);

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

/*! \brief Photo State Process
 *
 */
void PhotoState_Process(enPhotoState state);
#endif