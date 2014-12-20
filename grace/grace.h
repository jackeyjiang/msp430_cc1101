/* --COPYRIGHT--,ESD
 *  Copyright (c) 2008 Texas Instruments. All rights reserved. 
 *  This program and the accompanying materials are made available under the 
 *  terms of the Eclipse Public License v1.0 and Eclipse Distribution License
 *  v. 1.0 which accompanies this distribution. The Eclipse Public License is
 *  available at http://www.eclipse.org/legal/epl-v10.html and the Eclipse
 *  Distribution License is available at 
 *  http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 * --/COPYRIGHT--*/

/*
 *  ======== Grace ========
 *  Register-level / API-based Chip Configuration Tool
 */

#ifndef ti_mcu_msp430_Grace__include
#define ti_mcu_msp430_Grace__include

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== Grace_init ========
 *  Initialize all peripherals configured by Grace
 *  
 *  This function is main entry point into the Grace
 *  generated initialization code and it initializes only
 *  the set of peripherals specified by the application's
 *  configuration script.
 *
 *  This function should be called just once.  Applications
 *  should not access any peripheral initialized by
 *  Grace_init() before it's called.
 *
 *  The signature of this function is always the same, but
 *  the generated function body varies depending on the set
 *  of peripherals to be initialized.
 */
void Grace_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_mcu_msp430_Grace__include */
