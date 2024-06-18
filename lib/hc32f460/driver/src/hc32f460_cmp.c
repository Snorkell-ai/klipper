/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file hc32f460_cmp.c
 **
 ** A detailed description is available at
 ** @link CmpGroup CMP @endlink
 **
 **   - 2018-10-22  CDT  First version for Device Driver Library of CMP.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f460_cmp.h"
#include "hc32f460_utility.h"

/**
 *******************************************************************************
 ** \addtogroup CmpGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for CMP Instances. */
#define IS_VALID_CMP(__CMPx__)                                                 \
(   (M4_CMP1 == (__CMPx__))                     ||                             \
    (M4_CMP2 == (__CMPx__))                     ||                             \
    (M4_CMP3 == (__CMPx__)))

/*!< Parameter valid check for CMP function */
#define IS_VALID_CMP_FUNCTION(x)                                               \
(   (CmpOutput      == (x))                     ||                             \
    (CmpOutpuInv    == (x))                     ||                             \
    (CmpVcoutOutput == (x)))

/*! Parameter validity check for edge sel. */
#define IS_VALID_EDGESEL(x)                                                    \
(   (CmpNoneEdge     == (x))                    ||                             \
    (CmpBothEdge     == (x))                    ||                             \
    (CmpRisingEdge   == (x))                    ||                             \
    (CmpFaillingEdge == (x)))

/*!< Parameter CMP FLT validity check for clock division. */
#define IS_VALID_FLTCLK_DIVISION(x)                                            \
(   (CmpNoneFlt       == (x))                   ||                             \
    (CmpFltPclk3Div1  == (x))                   ||                             \
    (CmpFltPclk3Div2  == (x))                   ||                             \
    (CmpFltPclk3Div4  == (x))                   ||                             \
    (CmpFltPclk3Div8  == (x))                   ||                             \
    (CmpFltPclk3Div16 == (x))                   ||                             \
    (CmpFltPclk3Div32 == (x))                   ||                             \
    (CmpFltPclk3Div64 == (x)))

/*!< Parameter validity check for INP4 SEL. */
#define IS_VALID_INP4SEL(x)                                                    \
(   (CmpInp4None      == (x))                   ||                             \
    (CmpInp4PGAO      == (x))                   ||                             \
    (CmpInp4PGAO_BP   == (x))                   ||                             \
    (CmpInp4CMP1_INP4 == (x)))

/*!< Parameter validity check for INP INPUT SEL. */
#define IS_VALID_INPSEL(x)                                                     \
(   (CmpInpNone        == (x))                  ||                             \
    (CmpInp1           == (x))                  ||                             \
    (CmpInp2           == (x))                  ||                             \
    (CmpInp3           == (x))                  ||                             \
    (CmpInp4           == (x))                  ||                             \
    (CmpInp1_Inp2      == (x))                  ||                             \
    (CmpInp1_Inp3      == (x))                  ||                             \
    (CmpInp2_Inp3      == (x))                  ||                             \
    (CmpInp1_Inp4      == (x))                  ||                             \
    (CmpInp2_Inp4      == (x))                  ||                             \
    (CmpInp3_Inp4      == (x))                  ||                             \
    (CmpInp1_Inp2_Inp3 == (x))                  ||                             \
    (CmpInp1_Inp2_Inp4 == (x))                  ||                             \
    (CmpInp1_Inp3_Inp4 == (x))                  ||                             \
    (CmpInp2_Inp3_Inp4 == (x))                  ||                             \
    (CmpInp1_Inp2_Inp3_Inp4 == (x)))

/*!< Parameter validity check for INM INPUT SEL. */
#define IS_VALID_INMSEL(x)                                                     \
(   (CmpInm1    == (x))                         ||                             \
    (CmpInm2    == (x))                         ||                             \
    (CmpInm3    == (x))                         ||                             \
    (CmpInm4    == (x))                         ||                             \
    (CmpInmNone == (x)))

/*!< Parameter validity check for CMP_CR channel. */
#define IS_VALID_CMP_CR_CH(x)                                                  \
(   (CmpDac1  == (x))                           ||                             \
    (CmpDac2  == (x)))

/*!< Parameter validity check for ADC internal reference voltage path. */
#define IS_VALID_ADC_REF_VOLT_PATH(x)                                          \
(   (CmpAdcRefVoltPathDac1 == (x))              ||                             \
    (CmpAdcRefVoltPathDac2 == (x))              ||                             \
    (CmpAdcRefVoltPathVref == (x)))

/*!< RVADC Write Protection Key. */
#define RVADC_WRITE_PROT_KEY                    (0x5500u)

/*!< Timer4x ECER register address. */
#define CMP_CR_DADRx(__DACx__)                                                 \
( (CmpDac1 == (__DACx__)) ? &M4_CMP_CR->DADR1 : &M4_CMP_CR->DADR2)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Initializes the specified CMP.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] pstcInitCfg             Pointer to CMP configure structure
 ** \arg This parameter detail refer @ref stc_cmp_init_t
 **
 ** \retval Ok                          CMP is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - CMPx is invalid
 **                                     - pstcInitCfg == NULL
 **
 ******************************************************************************/
/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_Init(M4_CMP_TypeDef *CMPx, const stc_cmp_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx && pstcInitCfg pointer */
    if ((IS_VALID_CMP(CMPx)) && (NULL != pstcInitCfg))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_EDGESEL(pstcInitCfg->enEdgeSel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpIntEN));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpInvEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpOutputEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpVcoutOutputEn));
        DDL_ASSERT(IS_VALID_FLTCLK_DIVISION(pstcInitCfg->enFltClkDiv));

        /* De-Initialize CMP */
        CMPx->CTRL = (uint16_t)0x0000u;
        CMPx->VLTSEL = (uint16_t)0x0000u;
        CMPx->CVSSTB = (uint16_t)0x0005u;
        CMPx->CVSPRD = (uint16_t)0x000Fu;

        CMPx->CTRL_f.IEN = (uint16_t)pstcInitCfg->enCmpIntEN;
        CMPx->CTRL_f.INV = (uint16_t)pstcInitCfg->enCmpInvEn;
        CMPx->CTRL_f.EDGSL = (uint16_t)pstcInitCfg->enEdgeSel;
        CMPx->CTRL_f.FLTSL = (uint16_t)pstcInitCfg->enFltClkDiv;
        CMPx->CTRL_f.CMPOE = (uint16_t)pstcInitCfg->enCmpOutputEn;
        CMPx->CTRL_f.OUTEN = (uint16_t)pstcInitCfg->enCmpVcoutOutputEn;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_DeInit(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL = (uint16_t)0x0000u;
        CMPx->VLTSEL = (uint16_t)0x0000u;
        CMPx->CVSSTB = (uint16_t)0x0005u;
        CMPx->CVSPRD = (uint16_t)0x000Fu;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_Cmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        CMPx->CTRL_f.CMPON = (uint16_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_IrqCmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.IEN = (uint16_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetScanTime(M4_CMP_TypeDef *CMPx,
                                uint8_t u8ScanStable,
                                uint8_t u8ScanPeriod)
{
    uint16_t u16Flts;
    uint16_t u16FltslDiv;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if ((!IS_VALID_CMP(CMPx)) || (u8ScanStable & 0xF0u))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        u16Flts = CMPx->CTRL_f.FLTSL;
        u16FltslDiv = ((uint16_t)1u << (u16Flts - 1u));

        if ((0u != u16Flts) &&
             (u8ScanPeriod <= (u8ScanStable + u16FltslDiv * 4u + 5u)))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            CMPx->CVSSTB_f.STB = u8ScanStable;
            CMPx->CVSPRD_f.PRD = u8ScanPeriod;
        }
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_FuncCmd(M4_CMP_TypeDef *CMPx,
                    en_cmp_func_t enFunc,
                    en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        DDL_ASSERT(IS_VALID_CMP_FUNCTION(enFunc));

        if (Enable == enCmd)
        {
            CMPx->CTRL |= (uint16_t)enFunc;
        }
        else
        {
            CMPx->CTRL &= (uint16_t)(~((uint16_t)enFunc));
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_StartScan(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.CVSEN = (uint16_t)1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_StopScan(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.CVSEN = (uint16_t)0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetFilterClkDiv(M4_CMP_TypeDef *CMPx,
                                        en_cmp_fltclk_div_t enFltClkDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_FLTCLK_DIVISION(enFltClkDiv));
        CMPx->CTRL_f.FLTSL = (uint16_t)enFltClkDiv;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_fltclk_div_t CMP_GetFilterClkDiv(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_fltclk_div_t)CMPx->CTRL_f.FLTSL;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetEdgeSel(M4_CMP_TypeDef *CMPx,
                                en_cmp_edge_sel_t enEdgeSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_EDGESEL(enEdgeSel));
        CMPx->CTRL_f.EDGSL = (uint16_t)enEdgeSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_edge_sel_t CMP_GetEdgeSel(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_edge_sel_t)CMPx->CTRL_f.EDGSL;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_InputSel(M4_CMP_TypeDef *CMPx,
                            const stc_cmp_input_sel_t *pstcInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx && pstcInputSel pointer */
    if ((IS_VALID_CMP(CMPx)) && (NULL != pstcInputSel))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INMSEL(pstcInputSel->enInmSel));
        DDL_ASSERT(IS_VALID_INPSEL(pstcInputSel->enInpSel));
        DDL_ASSERT(IS_VALID_INP4SEL(pstcInputSel->enInp4Sel));

        if ((CmpInp4PGAO == pstcInputSel->enInp4Sel) ||
            (CmpInp4PGAO_BP == pstcInputSel->enInp4Sel))
        {
            if (M4_CMP3 != CMPx)
            {
                enRet = Ok;
            }
        }
        else if (CmpInp4CMP1_INP4 == pstcInputSel->enInp4Sel)
        {
            if (M4_CMP1 == CMPx)
            {
                enRet = Ok;
            }
        }
        else
        {
            enRet = Ok;
        }

        if (enRet == Ok)
        {
            CMPx->VLTSEL_f.CVSL = (uint16_t)pstcInputSel->enInpSel;
            CMPx->VLTSEL_f.RVSL = (uint16_t)pstcInputSel->enInmSel;
            CMPx->VLTSEL_f.C4SL = (uint16_t)pstcInputSel->enInp4Sel;
        }
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetInp(M4_CMP_TypeDef *CMPx, en_cmp_inp_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INPSEL(enInputSel));
        CMPx->VLTSEL_f.CVSL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_inp_sel_t CMP_GetInp(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp_sel_t)CMPx->VLTSEL_f.CVSL;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetInm(M4_CMP_TypeDef *CMPx, en_cmp_inm_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INMSEL(enInputSel));
        CMPx->VLTSEL_f.RVSL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_inm_sel_t CMP_GetInm(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inm_sel_t)CMPx->VLTSEL_f.RVSL;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_SetInp4(M4_CMP_TypeDef *CMPx,en_cmp_inp4_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    DDL_ASSERT(M4_CMP3 != CMPx);
    DDL_ASSERT(IS_VALID_INP4SEL(enInputSel));

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->VLTSEL_f.C4SL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_inp4_sel_t CMP_GetInp4(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp4_sel_t)CMPx->VLTSEL_f.C4SL;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_output_state_t CMP_GetOutputState(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_output_state_t)(CMPx->OUTMON_f.OMON);
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_cmp_inp_state_t CMP_GetInpState(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp_state_t)(CMPx->OUTMON_f.CVST);
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_DAC_Init(en_cmp_dac_ch_t enCh,
                            const stc_cmp_dac_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_CMP_CR_CH(enCh)) && (pstcInitCfg != NULL))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpDacEN));

        M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh));    /* Disable DAC */

        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = pstcInitCfg->u8DacData; /* Set DAC data */

        if (Enable == pstcInitCfg->enCmpDacEN)
        {
            M4_CMP_CR->DACR |= (uint16_t)(1ul << enCh); /* Enable DAC */
        }
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_DAC_DeInit(en_cmp_dac_ch_t enCh)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh));
        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = 0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_DAC_Cmd(en_cmp_dac_ch_t enCh, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        if(Enable == enCmd)
        {
            M4_CMP_CR->DACR |= (uint16_t)(1ul << enCh);
        }
        else
        {
            M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh));
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_DAC_SetData(en_cmp_dac_ch_t enCh, uint8_t u8DacData)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = u8DacData;
        enRet = Ok;
    }

    return enRet;
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
uint8_t CMP_DAC_GetData(en_cmp_dac_ch_t enCh)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP_CR_CH(enCh));

    return *(__IO uint8_t *)CMP_CR_DADRx(enCh);
}

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
en_result_t CMP_ADC_SetRefVoltPath(en_cmp_adc_int_ref_volt_path_t enRefVoltPath)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_ADC_REF_VOLT_PATH(enRefVoltPath))
    {
        M4_CMP_CR->RVADC = RVADC_WRITE_PROT_KEY;  /* Release write protection */
        M4_CMP_CR->RVADC =  enRefVoltPath;        /* Set reference voltage path */
        enRet = Ok;
    }

    return enRet;
}

//@} // CmpGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
