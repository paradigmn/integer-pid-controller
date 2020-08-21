#ifndef __PID_H
#define __PID_H
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------- I N C L U D E S -------------------------------*/
#include <stdint.h>

/*----------------------- P U B L I C  D E F I N E S -------------------------*/
/** @defgroup  PID_PublicDefines
 *  @{
 */

/* pid update rate in ms (should be accurate for stability) */
#define PID_TA														(UINT32_C(50))

/**
 * @}
 */
/*---------------------- P U B L I C  T Y P E D E F S ------------------------*/
/** @defgroup  PID_PublicTypedefs
 *  @{
 */

/** @}
 *
 */
/*----------- P U B L I C  F U N C T I O N  P R O T O T Y P E S --------------*/
/** @defgroup  PID_PublicFunctions
 *  @{
 */

extern void PID_Test(void);
extern void PID_UpdateValues(void);
extern int32_t PID_GetOutput(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /*__PID_H */
