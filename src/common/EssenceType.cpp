/*
 * Copyright (C) 2011, British Broadcasting Corporation
 * All Rights Reserved.
 *
 * Author: Philip de Nier
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the British Broadcasting Corporation nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bmx/EssenceType.h>
#include <bmx/Utils.h>
#include <bmx/Logging.h>
#include <bmx/BMXException.h>

using namespace bmx;


typedef struct
{
    EssenceType essence_type;
    EssenceType generic_essence_type;
    const char *str;
} EssenceTypeInfo;

static const EssenceTypeInfo ESSENCE_TYPE_INFO[] =
{
    {UNKNOWN_ESSENCE_TYPE,      UNKNOWN_ESSENCE_TYPE,   "unknown essence type"},
    {PICTURE_ESSENCE,           PICTURE_ESSENCE,        "picture essence"},
    {SOUND_ESSENCE,             SOUND_ESSENCE,          "sound essence"},
    {DATA_ESSENCE,              DATA_ESSENCE,           "data essence"},
    {D10_30,                    PICTURE_ESSENCE,        "D10 30Mbps"},
    {D10_40,                    PICTURE_ESSENCE,        "D10 40Mbps"},
    {D10_50,                    PICTURE_ESSENCE,        "D10 50Mbps"},
    {IEC_DV25,                  PICTURE_ESSENCE,        "IEC DV25"},
    {DVBASED_DV25,              PICTURE_ESSENCE,        "DV-Based DV25"},
    {DV50,                      PICTURE_ESSENCE,        "DV50"},
    {DV100_1080I,               PICTURE_ESSENCE,        "DV100 1080i"},
    {DV100_720P,                PICTURE_ESSENCE,        "DV100 720p"},
    {AVCI100_1080I,             PICTURE_ESSENCE,        "AVCI 100Mbps 1080i"},
    {AVCI100_1080P,             PICTURE_ESSENCE,        "AVCI 100Mbps 1080p"},
    {AVCI100_720P,              PICTURE_ESSENCE,        "AVCI 100Mbps 720p"},
    {AVCI50_1080I,              PICTURE_ESSENCE,        "AVCI 50Mbps 1080i"},
    {AVCI50_1080P,              PICTURE_ESSENCE,        "AVCI 50Mbps 1080p"},
    {AVCI50_720P,               PICTURE_ESSENCE,        "AVCI 50Mbps 720p"},
    {AVC_HIGH_10_INTRA_UNCS,    PICTURE_ESSENCE,        "AVC High 10 Intra Unconstrained"},
    {AVC_HIGH_422_INTRA_UNCS,   PICTURE_ESSENCE,        "AVC High 4:2:2 Intra Unconstrained"},
    {UNC_SD,                    PICTURE_ESSENCE,        "uncompressed SD"},
    {UNC_HD_1080I,              PICTURE_ESSENCE,        "uncompressed HD 1080i"},
    {UNC_HD_1080P,              PICTURE_ESSENCE,        "uncompressed HD 1080p"},
    {UNC_HD_720P,               PICTURE_ESSENCE,        "uncompressed HD 720p"},
    {AVID_10BIT_UNC_SD,         PICTURE_ESSENCE,        "Avid 10-bit uncompressed SD"},
    {AVID_10BIT_UNC_HD_1080I,   PICTURE_ESSENCE,        "Avid 10-bit uncompressed HD 1080i"},
    {AVID_10BIT_UNC_HD_1080P,   PICTURE_ESSENCE,        "Avid 10-bit uncompressed HD 1080p"},
    {AVID_10BIT_UNC_HD_720P,    PICTURE_ESSENCE,        "Avid 10-bit uncompressed HD 720p"},
    {AVID_ALPHA_SD,             PICTURE_ESSENCE,        "Avid uncompressed Alpha SD"},
    {AVID_ALPHA_HD_1080I,       PICTURE_ESSENCE,        "Avid uncompressed Alpha HD 1080i"},
    {AVID_ALPHA_HD_1080P,       PICTURE_ESSENCE,        "Avid uncompressed Alpha HD 1080p"},
    {AVID_ALPHA_HD_720P,        PICTURE_ESSENCE,        "Avid uncompressed Alpha HD 720p"},
    {MPEG2LG_422P_HL_1080I,     PICTURE_ESSENCE,        "MPEG-2 Long GOP 422P@HL 1080i"},
    {MPEG2LG_422P_HL_1080P,     PICTURE_ESSENCE,        "MPEG-2 Long GOP 422P@HL 1080p"},
    {MPEG2LG_422P_HL_720P,      PICTURE_ESSENCE,        "MPEG-2 Long GOP 422P@HL 720p"},
    {MPEG2LG_MP_HL_1920_1080I,  PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@HL 1920x1080i"},
    {MPEG2LG_MP_HL_1920_1080P,  PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@HL 1920x1080p"},
    {MPEG2LG_MP_HL_1440_1080I,  PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@HL 1440x1080i"},
    {MPEG2LG_MP_HL_1440_1080P,  PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@HL 1440x1080p"},
    {MPEG2LG_MP_HL_720P,        PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@HL 720p"},
    {MPEG2LG_MP_H14_1080I,      PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@H14 1080i"},
    {MPEG2LG_MP_H14_1080P,      PICTURE_ESSENCE,        "MPEG-2 Long GOP MP@H14 1080p"},
    {VC3_1080P_1235,            PICTURE_ESSENCE,        "VC3 1080p 1235"},
    {VC3_1080P_1237,            PICTURE_ESSENCE,        "VC3 1080p 1237"},
    {VC3_1080P_1238,            PICTURE_ESSENCE,        "VC3 1080p 1238"},
    {VC3_1080I_1241,            PICTURE_ESSENCE,        "VC3 1080i 1241"},
    {VC3_1080I_1242,            PICTURE_ESSENCE,        "VC3 1080i 1242"},
    {VC3_1080I_1243,            PICTURE_ESSENCE,        "VC3 1080i 1243"},
    {VC3_720P_1250,             PICTURE_ESSENCE,        "VC3 720p 1250"},
    {VC3_720P_1251,             PICTURE_ESSENCE,        "VC3 720p 1251"},
    {VC3_720P_1252,             PICTURE_ESSENCE,        "VC3 720p 1252"},
    {VC3_1080P_1253,            PICTURE_ESSENCE,        "VC3 1080p 1253"},
    {MJPEG_2_1,                 PICTURE_ESSENCE,        "MJPEG 2:1"},
    {MJPEG_3_1,                 PICTURE_ESSENCE,        "MJPEG 3:1"},
    {MJPEG_10_1,                PICTURE_ESSENCE,        "MJPEG 10:1"},
    {MJPEG_20_1,                PICTURE_ESSENCE,        "MJPEG 20:1"},
    {MJPEG_4_1M,                PICTURE_ESSENCE,        "MJPEG 4:1m"},
    {MJPEG_10_1M,               PICTURE_ESSENCE,        "MJPEG 10:1m"},
    {MJPEG_15_1S,               PICTURE_ESSENCE,        "MJPEG 15:1s"},
    {WAVE_PCM,                  SOUND_ESSENCE,          "WAVE PCM"},
    {D10_AES3_PCM,              SOUND_ESSENCE,          "D10 AES3 PCM"},
    {ANC_DATA,                  DATA_ESSENCE,           "ANC data"},
    {VBI_DATA,                  DATA_ESSENCE,           "VBI data"},
};



const char* bmx::essence_type_to_string(EssenceType essence_type)
{
    BMX_ASSERT((size_t)essence_type < BMX_ARRAY_SIZE(ESSENCE_TYPE_INFO));
    BMX_ASSERT(ESSENCE_TYPE_INFO[essence_type].essence_type == essence_type);

    return ESSENCE_TYPE_INFO[essence_type].str;
}

EssenceType bmx::get_generic_essence_type(EssenceType essence_type)
{
    BMX_ASSERT((size_t)essence_type < BMX_ARRAY_SIZE(ESSENCE_TYPE_INFO));
    BMX_ASSERT(ESSENCE_TYPE_INFO[essence_type].essence_type == essence_type);

    return ESSENCE_TYPE_INFO[essence_type].generic_essence_type;
}

