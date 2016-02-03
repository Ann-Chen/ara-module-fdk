/*
 * Copyright (c) 2016 Google, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nuttx/kmalloc.h>
#include "CameraCapability.h"

/**
 * @brief Camera sensor Capabilities
 * @param buffer size
 * @param buffer address
 * @return OK for success or ERROR on any faillure
 */
int getCapabilities(uint32_t *size, uint8_t *cap)
{
    struct camera_metadata_package  *pMetaData;
    int index = 0, buffercount = 0;
    static const uint8_t availableAntibandingModes[] = {
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_OFF,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_50HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_60HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO
    };
    static const uint8_t availableAeModes[] = {
        ANDROID_CONTROL_AE_MODE_OFF,
        ANDROID_CONTROL_AE_MODE_ON,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_ALWAYS_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE
    };
    static const int32_t availableTargetFpsRanges[] = {5, 30, 15, 30};
    static const int32_t exposureCompensationRange[] = {-9, 9};
    static const camera_metadata_rational_t exposureCompensationStep = {1, 3};
    static const uint8_t availableAfModesBack[] = {
        ANDROID_CONTROL_AF_MODE_OFF,
        ANDROID_CONTROL_AF_MODE_AUTO,
        ANDROID_CONTROL_AF_MODE_MACRO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_VIDEO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_PICTURE
    };
    static const uint8_t availableSceneModes[] = {
        ANDROID_CONTROL_SCENE_MODE_DISABLED,
        ANDROID_CONTROL_SCENE_MODE_FACE_PRIORITY,
        ANDROID_CONTROL_SCENE_MODE_ACTION,
        ANDROID_CONTROL_SCENE_MODE_PORTRAIT,
        ANDROID_CONTROL_SCENE_MODE_LANDSCAPE,
        ANDROID_CONTROL_SCENE_MODE_NIGHT,
        ANDROID_CONTROL_SCENE_MODE_NIGHT_PORTRAIT,
        ANDROID_CONTROL_SCENE_MODE_THEATRE,
        ANDROID_CONTROL_SCENE_MODE_BEACH,
        ANDROID_CONTROL_SCENE_MODE_SNOW,
        ANDROID_CONTROL_SCENE_MODE_SUNSET,
        ANDROID_CONTROL_SCENE_MODE_STEADYPHOTO,
        ANDROID_CONTROL_SCENE_MODE_FIREWORKS,
        ANDROID_CONTROL_SCENE_MODE_SPORTS,
        ANDROID_CONTROL_SCENE_MODE_PARTY,
        ANDROID_CONTROL_SCENE_MODE_CANDLELIGHT,
        ANDROID_CONTROL_SCENE_MODE_BARCODE,
        ANDROID_CONTROL_SCENE_MODE_HIGH_SPEED_VIDEO,
        ANDROID_CONTROL_SCENE_MODE_HDR
    };
    static const uint8_t availableVstabModes[] = {
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF,
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_ON
    };
    static const uint8_t availableAwbModes[] = {
        ANDROID_CONTROL_AWB_MODE_OFF,
        ANDROID_CONTROL_AWB_MODE_AUTO,
        ANDROID_CONTROL_AWB_MODE_INCANDESCENT,
        ANDROID_CONTROL_AWB_MODE_FLUORESCENT,
        ANDROID_CONTROL_AWB_MODE_WARM_FLUORESCENT,
        ANDROID_CONTROL_AWB_MODE_DAYLIGHT,
        ANDROID_CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
        ANDROID_CONTROL_AWB_MODE_TWILIGHT,
        ANDROID_CONTROL_AWB_MODE_SHADE,
    };
    static const int32_t max3aRegions[] = {0, 0, 0};
    static const uint8_t flashAvailable = ANDROID_FLASH_INFO_AVAILABLE_FALSE;
    static const int32_t jpegThumbnailSizes[] = {0, 0, 160, 120, 320, 240};
    static const float focalLength = 3.30f;
    static const int32_t max_output_streams[] = {
        MAX_STALLING_STREAMS,
        MAX_PROCESSED_STREAMS,
        MAX_RAW_STREAMS
    };
    static const int32_t scalar_formats[] = {
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW16,
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW_OPAQUE,
        ANDROID_SCALER_AVAILABLE_FORMATS_YV12,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCrCb_420_SP,
        ANDROID_SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCbCr_420_888,
        ANDROID_SCALER_AVAILABLE_FORMATS_BLOB
    };
    static const float maxZoom = 10;
    static const int32_t orientation = 0;
    static const int32_t SensitivityRange[2] = {100, 1600};
    static const float sensorPhysicalSize[2] = {3.20f, 2.40f};
    static const int32_t Resolution[]  =  {640, 480};
    static const int32_t maxFaceCount = 8;

    /* buffer initial */
    pMetaData = zalloc(sizeof(struct camera_metadata_package));
    if (NULL == pMetaData)
        return ERROR;

    pMetaData->entries = zalloc(MAX_METADATA_NUMBER *
                                sizeof(struct camera_metadata_entry));
    if (NULL == pMetaData->entries) {
        free(pMetaData);
        return ERROR;
    }

    /* need to modify it */
    pMetaData->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (NULL == pMetaData->data ) {
        free(pMetaData);
        free(pMetaData->entries);
        return ERROR;
    }
    pMetaData->header.version = ARA_METADATA_VERSION;

    /* CONTROL_AE_AVAILABLE_ANTIBANDING_MODES */
    updateMetadata(pMetaData, TYPE_BYTE,
                   CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,
                   sizeof(availableAntibandingModes),
                   availableAntibandingModes);

    /* CONTROL_AE_AVAILABLE_MODES */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AE_AVAILABLE_MODES,
                   sizeof(availableAeModes), availableAeModes);

    /* CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES */
    updateMetadata(pMetaData, TYPE_INT32,
                   CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
                   sizeof(availableTargetFpsRanges),
                   (uint8_t *)availableTargetFpsRanges);

    /* CONTROL_AE_COMPENSATION_STEP */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AE_COMPENSATION_RANGE,
                   sizeof(exposureCompensationRange),
                   (uint8_t *)exposureCompensationRange);

    /* CONTROL_AE_COMPENSATION_STEP */
    updateMetadata(pMetaData, TYPE_RATIONAL, CONTROL_AE_COMPENSATION_STEP,
                   sizeof(exposureCompensationStep),
                   (uint8_t *)&exposureCompensationStep);

    /* CONTROL_AF_AVAILABLE_MODES */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AF_AVAILABLE_MODES,
                   sizeof(availableAfModesBack), availableAfModesBack);

    /* CONTROL_AVAILABLE_SCENE_MODES */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AVAILABLE_SCENE_MODES,
                   sizeof(availableSceneModes), availableSceneModes);

    /* CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES */
    updateMetadata(pMetaData, TYPE_BYTE,
                   CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES,
                   sizeof(availableVstabModes), availableVstabModes);

    /* CONTROL_AWB_AVAILABLE_MODES */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AWB_AVAILABLE_MODES,
                   sizeof(availableAwbModes), availableAwbModes);

    /* CONTROL_MAX_REGIONS */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_MAX_REGIONS,
                   sizeof(max3aRegions), (uint8_t *)max3aRegions);

    /* CONTROL_SCENE_MODES_OVERRIDES */

    /* FLASH_INFO_AVAILABLE */
    updateMetadata(pMetaData, TYPE_BYTE, FLASH_INFO_AVAILABLE,
                   sizeof(flashAvailable), &flashAvailable);

    /* android.jpeg - JPEG_AVAILABLE_THUMBNAIL_SIZES */
    updateMetadata(pMetaData, TYPE_INT32, JPEG_AVAILABLE_THUMBNAIL_SIZES,
                   sizeof(jpegThumbnailSizes), (uint8_t *)jpegThumbnailSizes);

    /* LENS_INFO_AVAILABLE_FOCAL_LENGTHS */
    updateMetadata(pMetaData, TYPE_FLOAT, LENS_INFO_AVAILABLE_FOCAL_LENGTHS,
                   sizeof(focalLength), (uint8_t *)&focalLength);

    /* REQUEST_MAX_NUM_OUTPUT_STREAMS */
    updateMetadata(pMetaData, TYPE_INT32, REQUEST_MAX_NUM_OUTPUT_STREAMS,
                   sizeof(max_output_streams), (uint8_t *)max_output_streams);

    /* REQUEST_AVAILABLE_REQUEST_KEYS = 0x68 */
    /* REQUEST_AVALIABLE_RESULT_KEYS */
    /* REQUEST_AVALIABLE_CHARACTERISTICS */

    /* SCALER_AVAILABLE_FORMATS */
    updateMetadata(pMetaData, TYPE_INT32, SCALER_AVAILABLE_FORMATS,
                   sizeof(scalar_formats), (uint8_t *)scalar_formats);

    /* SCALER_AVAILABLE_MAX_DIGITAL_ZOOM */
    updateMetadata(pMetaData, TYPE_FLOAT, SCALER_AVAILABLE_MAX_DIGITAL_ZOOM,
                   sizeof(maxZoom), (uint8_t *)&maxZoom);

    /* SENSOR_ORIENTATION */
    updateMetadata(pMetaData, TYPE_INT32, SENSOR_ORIENTATION,
                   sizeof(orientation), (uint8_t *)&orientation);

    /* SENSOR_INFO_SENSITIVITY_RANGE */
    updateMetadata(pMetaData, TYPE_INT32, SENSOR_INFO_SENSITIVITY_RANGE,
                   sizeof(SensitivityRange), (uint8_t *)SensitivityRange);

    /* SENSOR_INFO_PHYSICAL_SIZE */
    updateMetadata(pMetaData, TYPE_FLOAT, SENSOR_INFO_PHYSICAL_SIZE,
                   sizeof(sensorPhysicalSize), (uint8_t *)sensorPhysicalSize);

    /* SENSOR_INFO_PIXEL_ARRAY_SIZE */
    updateMetadata(pMetaData, TYPE_INT32, SENSOR_INFO_PIXEL_ARRAY_SIZE,
                   sizeof(Resolution), (uint8_t *)Resolution);

    /* STATISTICS_INFO_MAX_FACE_COUNT */
    updateMetadata(pMetaData, TYPE_INT32, STATISTICS_INFO_MAX_FACE_COUNT,
                   sizeof(maxFaceCount), (uint8_t *)&maxFaceCount);

    /* Do the Data Copy */
    memcpy(cap, &pMetaData->header, sizeof(struct camera_metadata_header));
    buffercount = sizeof(struct camera_metadata_header);
    pMetaData->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < pMetaData->header.entry_count; index++) {
        memcpy(cap, &pMetaData->entries[index],
               sizeof(struct camera_metadata_entry));
        buffercount += sizeof(struct camera_metadata_entry);
        cap += sizeof(struct camera_metadata_entry);
    }

    pMetaData->header.metadata_data_start = buffercount;
    memcpy(cap, &pMetaData->data[0], pMetaData->header.size);

    /* total size of metadata package size */
    buffercount += pMetaData->header.size;
    pMetaData->header.size = buffercount;
    *size = buffercount;
    pMetaData->header.metadata_data_count = pMetaData->header.entry_count;

    free(pMetaData->data);
    free(pMetaData->entries);
    free(pMetaData);
    return OK;
}

/**
 * @brief Camera sensor capture
 * @param buffer size
 * @param buffer address
 * @return OK for success or ERROR on any faillure
 */
int getDefaultRequestSettings(int32_t *size, uint8_t *cap)
{
    struct camera_metadata_package  *pMetaData;
    int index = 0, buffercount = 0;
    static const uint8_t aeAntibandingMode =
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO;
    static const int32_t aeExpCompensation = 0;
    static const uint8_t aeLock = ANDROID_CONTROL_AE_LOCK_OFF;
    static const uint8_t aeMode = ANDROID_CONTROL_AE_MODE_ON;
    static const int32_t controlRegions[5] = {0, 0, 640, 480, 1000};
    static const int32_t aeTargetFpsRange[2] = {10, 30};
    uint8_t afMode = ANDROID_CONTROL_AF_MODE_AUTO;
    static const uint8_t awbLock = ANDROID_CONTROL_AWB_LOCK_OFF;
    static const uint8_t awbMode = ANDROID_CONTROL_AWB_MODE_AUTO;
    uint8_t controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_PREVIEW;
    static const uint8_t effectMode = ANDROID_CONTROL_EFFECT_MODE_OFF;
    static const uint8_t controlMode = ANDROID_CONTROL_MODE_AUTO;
    static const uint8_t sceneMode = ANDROID_CONTROL_SCENE_MODE_FACE_PRIORITY;
    static const uint8_t vstabMode =
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF;
    static const uint8_t flashMode = ANDROID_FLASH_MODE_OFF;
    static const int32_t jpegOrientation = 0;
    static const uint8_t jpegQuality = 80;
    static const int32_t thumbnailSize[2] = {640, 480};
    static const float focusDistance = 0;
    static const uint8_t faceDetectMode =
        ANDROID_STATISTICS_FACE_DETECT_MODE_OFF;

    /* buffer initial */
    pMetaData = zalloc(sizeof(struct camera_metadata_package));
    if (NULL == pMetaData)
        return ERROR;

    pMetaData->entries = zalloc(MAX_METADATA_NUMBER *
                                sizeof(struct camera_metadata_entry));
    if (NULL == pMetaData->entries) {
        free(pMetaData);
        return ERROR;
    }

    pMetaData->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (NULL == pMetaData->data ) {
        free(pMetaData);
        free(pMetaData->entries);
        return ERROR;
    }
    pMetaData->header.version = ARA_METADATA_VERSION;

    /* android.control - CONTROL_AE_ANTIBANDING_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AE_ANTIBANDING_MODE,
                   sizeof(aeAntibandingMode), &aeAntibandingMode);

    /* CONTROL_AE_EXPOSURE_COMPENSATION */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AE_EXPOSURE_COMPENSATION,
                   sizeof(aeExpCompensation), (uint8_t *)&aeExpCompensation);

    /* CONTROL_AE_LOCK */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AE_LOCK, sizeof(aeLock),
                   &aeLock);

    /* CONTROL_AE_MODE */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AE_MODE, sizeof(aeMode),
                   &aeMode);

    /* CONTROL_AE_REGIONS */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AE_REGIONS,
                   sizeof(controlRegions), (uint8_t *)controlRegions);

    /* CONTROL_AE_TARGET_FPS_RANGE */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AE_TARGET_FPS_RANGE,
                   sizeof(aeTargetFpsRange), (uint8_t *)aeTargetFpsRange);

    /* CONTROL_AE_PRECAPTURE_TRIGGER */

    /* CONTROL_AF_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AF_MODE, sizeof(afMode),
                   &afMode);

    /* CONTROL_AF_REGIONS */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AF_REGIONS,
                   sizeof(controlRegions), (uint8_t *)controlRegions);

    /* CONTROL_AWB_LOCK */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AWB_LOCK, sizeof(awbLock),
                   &awbLock);

    /* CONTROL_AWB_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_AWB_MODE, sizeof(awbMode),
                   &awbMode);

    /* CONTROL_AWB_REGIONS */
    updateMetadata(pMetaData, TYPE_INT32, CONTROL_AWB_REGIONS,
                   sizeof(controlRegions), (uint8_t *)controlRegions);

    /* CONTROL_CAPTURE_INTENT */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_CAPTURE_INTENT,
                   sizeof(controlIntent), &controlIntent);

    /* CONTROL_EFFECT_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_EFFECT_MODE,
                   sizeof(effectMode), &effectMode);

    /* CONTROL_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_MODE, sizeof(controlMode),
                   &controlMode);

    /* CONTROL_SCENE_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_SCENE_MODE, sizeof(sceneMode),
                   &sceneMode);

    /* CONTROL_VIDEO_STABILIZATION_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, CONTROL_VIDEO_STABILIZATION_MODE,
                   sizeof(vstabMode), &vstabMode);

    /* FLASH_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, FLASH_MODE, sizeof(flashMode),
                   &flashMode);

    /* JPEG_ORIENTATION */
    updateMetadata(pMetaData, TYPE_INT32, JPEG_ORIENTATION,
                   sizeof(jpegOrientation), (uint8_t *)&jpegOrientation);

    /* JPEG_QUALITY */
    updateMetadata(pMetaData, TYPE_BYTE, JPEG_QUALITY, sizeof(jpegQuality),
                   &jpegQuality);

    /* JPEG_THUMBNAIL_SIZE */
    updateMetadata(pMetaData, TYPE_INT32, JPEG_THUMBNAIL_SIZE,
                   sizeof(thumbnailSize), (uint8_t *)thumbnailSize);

    /* LENS_FOCUS_DISTANCE */
    updateMetadata(pMetaData,TYPE_FLOAT, LENS_FOCUS_DISTANCE,
                   sizeof(focusDistance), (uint8_t *)&focusDistance);

    /* SCALER_CROP_REGION */

    /* STATISTICS_FACE_DETECT_MODE */
    updateMetadata(pMetaData, TYPE_BYTE, STATISTICS_FACE_DETECT_MODE,
                   sizeof(faceDetectMode), &faceDetectMode);

    /* Do the Data Copy */
    memcpy(cap, &pMetaData->header, sizeof(struct camera_metadata_header));
    buffercount = sizeof(struct camera_metadata_header);
    pMetaData->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < pMetaData->header.entry_count; index++) {
        memcpy(cap, &pMetaData->entries[index],
               sizeof(struct camera_metadata_entry));
        buffercount += sizeof(struct camera_metadata_entry);
        cap += sizeof(struct camera_metadata_entry);
    }

    pMetaData->header.metadata_data_start = buffercount;
    memcpy(cap, &pMetaData->data[0], pMetaData->header.size);

    /* total size of metadata package size */
    buffercount += pMetaData->header.size;
    pMetaData->header.size = buffercount;
    *size = buffercount;
    pMetaData->header.metadata_data_count = pMetaData->header.entry_count;

    /* Copy header, entries and metadata to one buffer */
    free(pMetaData->data);
    free(pMetaData->entries);
    free(pMetaData);
    return OK;
}

/**
 * @brief update metadata to buffer
 * @param Pointer to structure of metadata data
 * @param metadata type
 * @param metadata Key ID
 * @param metadata size
 * @param matadata value
 * @return  OK for success or ERROR on any faillure
 */
int updateMetadata(struct camera_metadata_package *pData, uint8_t type,
    Camera_Metadata_type_t KeyiD, int size, const uint8_t *values)
{
    int nEntry = 0;

    if (pData->header.entry_count >= MAX_METADATA_NUMBER) {
        return ERROR;
    }
    nEntry = pData->header.entry_count;
    pData->entries[nEntry].entry_tag = KeyiD;
    pData->entries[nEntry].data_type = type;
    pData->entries[nEntry].data_count = size;

    memcpy(&pData->data[pData->header.size], (void *)values, size);

    //address alignment
    pData->header.size += size;
    pData->header.size = ALIGNMENT(pData->header.size, 8);
    pData->entries[nEntry].data_offset = pData->header.size;

    pData->header.entry_count++;
    return OK;
}
