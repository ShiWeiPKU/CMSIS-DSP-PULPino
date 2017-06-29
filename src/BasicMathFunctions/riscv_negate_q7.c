/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015
* $Revision: 	V.1.4.5
*    
* Project: 	    CMSIS DSP Library    
* Title:		arm_negate_q7.c    
*    
* Description:	Negates Q7 vectors.    
*    
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*  
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the 
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE. 

 Modifications 2017  Mostafa Saleh       (Ported to RISC-V PULPino)  
* -------------------------------------------------------------------- */

#include "riscv_math.h"

/**    
 * @ingroup groupMath    
 */

/**    
 * @addtogroup negate    
 * @{    
 */

/**    
 * @brief  Negates the elements of a Q7 vector.    
 * @param[in]  *pSrc points to the input vector    
 * @param[out]  *pDst points to the output vector    
 * @param[in]  blockSize number of samples in the vector    
 * @return none.    
 *    
 * <b>Scaling and Overflow Behavior:</b>    
 * \par    
 * The function uses saturating arithmetic.    
 * The Q7 value -1 (0x80) will be saturated to the maximum allowable positive value 0x7F.    
 */

void riscv_negate_q7(
  q7_t * pSrc,
  q7_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* loop counter */
  q7_t in;
#if defined (USE_DSP_RISCV)
  q31_t input;                                   /* Input values1-4 */
  q31_t zero = 0x00000000;
  charV *VectInA;
  charV VectInC; 

  /*loop Unrolling */
  blkCnt = blockSize >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0u)
  {
    /*read 4 elements*/
    VectInA = (charV*)pSrc;
    /*find the negative*/
    VectInC = neg4(*VectInA);
    /*check for saturation the save to destination buffer*/ 
    *pDst++ = ( VectInC[0] == -128)?0x7f:VectInC[0];
    *pDst++ = ( VectInC[1] == -128)?0x7f:VectInC[1];
    *pDst++ = ( VectInC[2] == -128)?0x7f:VectInC[2];
    *pDst++ = ( VectInC[3] == -128)?0x7f:VectInC[3];
    /*increment source buffer*/
    pSrc+=4;
    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4u;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
#endif
  while(blkCnt > 0u)
  {
    /* C = -A */
    /* Negate and then store the results in the destination buffer. */ 
      in = *pSrc++;
    *pDst++ = (in == (q7_t) 0x80) ? 0x7f : -in;

    /* Decrement the loop counter */
    blkCnt--;
  }
}

/**    
 * @} end of negate group    
 */