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
 * @param size buffer size
 * @param capabilities buffer address
 * @return zero for success or non-zero on any faillure
 */
int get_capabilities(uint32_t *size, uint8_t *capabilities)
{
    struct camera_metadata_package  *metadata_ptr;
    uint8_t *cap;
    int index = 0, buffercount = 0, ret;

    const uint8_t availableAntibandingModes[] = {
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_OFF,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_50HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_60HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO
    };
    const uint8_t availableAeModes[] = {
        ANDROID_CONTROL_AE_MODE_OFF,
        ANDROID_CONTROL_AE_MODE_ON,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_ALWAYS_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE
    };
    const int32_t availableTargetFpsRanges[] = {5, 30, 15, 30};
    const int32_t exposureCompensationRange[] = {-9, 9};
    const camera_metadata_rational_t exposureCompensationStep = {1, 3};
    const uint8_t availableAfModesBack[] = {
        ANDROID_CONTROL_AF_MODE_OFF,
        ANDROID_CONTROL_AF_MODE_AUTO,
        ANDROID_CONTROL_AF_MODE_MACRO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_VIDEO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_PICTURE
    };
    const uint8_t availableSceneModes[] = {
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
    const uint8_t availableVstabModes[] = {
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF,
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_ON
    };
    const uint8_t availableAwbModes[] = {
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
    const int32_t max3aRegions[] = {0, 0, 0};
    const uint8_t flashAvailable = ANDROID_FLASH_INFO_AVAILABLE_FALSE;
    const int32_t jpegThumbnailSizes[] = {0, 0, 160, 120, 320, 240};
    const float focalLength = 3.30f;
    const int32_t max_output_streams[] = {
        MAX_STALLING_STREAMS,
        MAX_PROCESSED_STREAMS,
        MAX_RAW_STREAMS
    };
    const int32_t scalar_formats[] = {
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW16,
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW_OPAQUE,
        ANDROID_SCALER_AVAILABLE_FORMATS_YV12,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCrCb_420_SP,
        ANDROID_SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCbCr_420_888,
        ANDROID_SCALER_AVAILABLE_FORMATS_BLOB
    };
    const float maxZoom = 10;
    const int32_t orientation = 0;
    const int32_t SensitivityRange[2] = {100, 1600};
    const float sensorPhysicalSize[2] = {3.20f, 2.40f};
    const int32_t Resolution[] = {640, 480};
    const int32_t maxFaceCount = 8;

    /* buffer initial */
    cap = capabilities;
    metadata_ptr = zalloc(sizeof(struct camera_metadata_package));
    if (!metadata_ptr)
        return -ENOMEM;

    metadata_ptr->entries = zalloc(MAX_METADATA_NUMBER *
                                   sizeof(struct camera_metadata_entry));
    if (!metadata_ptr->entries) {
        free(metadata_ptr);
        return -ENOMEM;
    }

    /* need to modify it */
    metadata_ptr->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (!metadata_ptr->data ) {
        free(metadata_ptr);
        free(metadata_ptr->entries);
        return -ENOMEM;
    }
    metadata_ptr->header.version = ARA_METADATA_VERSION;

    /* CONTROL_AE_AVAILABLE_ANTIBANDING_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,
                          sizeof(availableAntibandingModes),
                          availableAntibandingModes);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AE_AVAILABLE_MODES,
                          sizeof(availableAeModes), availableAeModes);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
                          sizeof(availableTargetFpsRanges),
                          (uint8_t *)availableTargetFpsRanges);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_COMPENSATION_STEP */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_COMPENSATION_RANGE,
                          sizeof(exposureCompensationRange),
                          (uint8_t *)exposureCompensationRange);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_COMPENSATION_STEP */
    ret = update_metadata(metadata_ptr, TYPE_RATIONAL,
                          CONTROL_AE_COMPENSATION_STEP,
                          sizeof(exposureCompensationStep),
                          (uint8_t *)&exposureCompensationStep);
    if (ret) {
        return ret;
    }

    /* CONTROL_AF_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AF_AVAILABLE_MODES,
                          sizeof(availableAfModesBack), availableAfModesBack);
    if (ret) {
        return ret;
    }

    /* CONTROL_AVAILABLE_SCENE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AVAILABLE_SCENE_MODES,
                          sizeof(availableSceneModes), availableSceneModes);
    if (ret) {
        return ret;
    }

    /* CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES,
                          sizeof(availableVstabModes), availableVstabModes);
    if (ret) {
        return ret;
    }

    /* CONTROL_AWB_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AWB_AVAILABLE_MODES,
                          sizeof(availableAwbModes), availableAwbModes);
    if (ret) {
        return ret;
    }

    /* CONTROL_MAX_REGIONS */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_MAX_REGIONS,
                          sizeof(max3aRegions), (uint8_t *)max3aRegions);
    if (ret) {
        return ret;
    }

    /* CONTROL_SCENE_MODES_OVERRIDES */

    /* FLASH_INFO_AVAILABLE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, FLASH_INFO_AVAILABLE,
                          sizeof(flashAvailable), &flashAvailable);
    if (ret) {
        return ret;
    }

    /* android.jpeg - JPEG_AVAILABLE_THUMBNAIL_SIZES */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          JPEG_AVAILABLE_THUMBNAIL_SIZES,
                          sizeof(jpegThumbnailSizes),
                          (uint8_t *)jpegThumbnailSizes);
    if (ret) {
        return ret;
    }

    /* LENS_INFO_AVAILABLE_FOCAL_LENGTHS */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT,
                          LENS_INFO_AVAILABLE_FOCAL_LENGTHS,
                          sizeof(focalLength), (uint8_t *)&focalLength);
    if (ret) {
        return ret;
    }

    /* REQUEST_MAX_NUM_OUTPUT_STREAMS */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          REQUEST_MAX_NUM_OUTPUT_STREAMS,
                          sizeof(max_output_streams),
                          (uint8_t *)max_output_streams);
    if (ret) {
        return ret;
    }

    /* REQUEST_AVAILABLE_REQUEST_KEYS = 0x68 */
    /* REQUEST_AVALIABLE_RESULT_KEYS */
    /* REQUEST_AVALIABLE_CHARACTERISTICS */

    /* SCALER_AVAILABLE_FORMATS */
    ret = update_metadata(metadata_ptr, TYPE_INT32, SCALER_AVAILABLE_FORMATS,
                          sizeof(scalar_formats), (uint8_t *)scalar_formats);
    if (ret) {
        return ret;
    }

    /* SCALER_AVAILABLE_MAX_DIGITAL_ZOOM */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT,
                          SCALER_AVAILABLE_MAX_DIGITAL_ZOOM,
                          sizeof(maxZoom), (uint8_t *)&maxZoom);
    if (ret) {
        return ret;
    }

    /* SENSOR_ORIENTATION */
    ret = update_metadata(metadata_ptr, TYPE_INT32, SENSOR_ORIENTATION,
                          sizeof(orientation), (uint8_t *)&orientation);
    if (ret) {
        return ret;
    }

    /* SENSOR_INFO_SENSITIVITY_RANGE */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          SENSOR_INFO_SENSITIVITY_RANGE,
                          sizeof(SensitivityRange),
                          (uint8_t *)SensitivityRange);
    if (ret) {
        return ret;
    }

    /* SENSOR_INFO_PHYSICAL_SIZE */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT, SENSOR_INFO_PHYSICAL_SIZE,
                          sizeof(sensorPhysicalSize),
                          (uint8_t *)sensorPhysicalSize);
    if (ret) {
        return ret;
    }

    /* SENSOR_INFO_PIXEL_ARRAY_SIZE */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          SENSOR_INFO_PIXEL_ARRAY_SIZE,
                          sizeof(Resolution), (uint8_t *)Resolution);
    if (ret) {
        return ret;
    }

    /* STATISTICS_INFO_MAX_FACE_COUNT */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          STATISTICS_INFO_MAX_FACE_COUNT,
                          sizeof(maxFaceCount), (uint8_t *)&maxFaceCount);
    if (ret) {
        return ret;
    }

    /* Do the Data Copy */
    memcpy(cap, &metadata_ptr->header, sizeof(struct camera_metadata_header));
    buffercount = sizeof(struct camera_metadata_header);
    metadata_ptr->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < metadata_ptr->header.entry_count; index++) {
        memcpy(cap, &metadata_ptr->entries[index],
               sizeof(struct camera_metadata_entry));
        buffercount += sizeof(struct camera_metadata_entry);
        cap += sizeof(struct camera_metadata_entry);
    }

    metadata_ptr->header.metadata_data_start = buffercount;
    memcpy(cap, &metadata_ptr->data[0], metadata_ptr->header.size);

    /* total size of metadata package size */
    buffercount += metadata_ptr->header.size;
    metadata_ptr->header.size = buffercount;
    *size = buffercount;
    metadata_ptr->header.metadata_data_count = metadata_ptr->header.entry_count;

    free(metadata_ptr->data);
    free(metadata_ptr->entries);
    free(metadata_ptr);
    return 0;
}

/**
 * @brief Camera sensor capture settings
 * @param size buffer size
 * @param capabilities buffer address
 * @return zero for success or non-zero on any faillure
 */
int get_capture_request_settings(uint8_t *capabilities)
{
    struct camera_metadata_package  *metadata_ptr;
    uint8_t *cap, *size;
    int index = 0, buffercount = 0, ret;

    const uint8_t aeAntibandingMode =
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO;
    const int32_t aeExpCompensation = 0;
    const uint8_t aeLock = ANDROID_CONTROL_AE_LOCK_OFF;
    const uint8_t aeMode = ANDROID_CONTROL_AE_MODE_ON;
    const int32_t controlRegions[5] = {0, 0, 640, 480, 1000};
    const int32_t aeTargetFpsRange[2] = {10, 30};
    uint8_t afMode = ANDROID_CONTROL_AF_MODE_AUTO;
    const uint8_t awbLock = ANDROID_CONTROL_AWB_LOCK_OFF;
    const uint8_t awbMode = ANDROID_CONTROL_AWB_MODE_AUTO;
    uint8_t controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_PREVIEW;
    const uint8_t effectMode = ANDROID_CONTROL_EFFECT_MODE_OFF;
    const uint8_t controlMode = ANDROID_CONTROL_MODE_AUTO;
    const uint8_t sceneMode = ANDROID_CONTROL_SCENE_MODE_FACE_PRIORITY;
    const uint8_t vstabMode =
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF;
    const uint8_t flashMode = ANDROID_FLASH_MODE_OFF;
    const int32_t jpegOrientation = 0;
    const uint8_t jpegQuality = 80;
    const int32_t thumbnailSize[2] = {640, 480};
    const float focusDistance = 0;
    const uint8_t faceDetectMode =
        ANDROID_STATISTICS_FACE_DETECT_MODE_OFF;

    /* buffer initial */
    cap = capabilities;
    metadata_ptr = zalloc(sizeof(struct camera_metadata_package));
    if (!metadata_ptr)
        return -ENOMEM;

    metadata_ptr->entries = zalloc(MAX_METADATA_NUMBER *
                                   sizeof(struct camera_metadata_entry));
    if (!metadata_ptr->entries) {
        free(metadata_ptr);
        return -ENOMEM;
    }

    metadata_ptr->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (!metadata_ptr->data ) {
        free(metadata_ptr);
        free(metadata_ptr->entries);
        return -ENOMEM;
    }
    metadata_ptr->header.version = ARA_METADATA_VERSION;

    /* CONTROL_AE_ANTIBANDING_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AE_ANTIBANDING_MODE,
                          sizeof(aeAntibandingMode), &aeAntibandingMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_EXPOSURE_COMPENSATION */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_EXPOSURE_COMPENSATION,
                          sizeof(aeExpCompensation),
                          (uint8_t *)&aeExpCompensation);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_LOCK */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AE_LOCK,
                          sizeof(aeLock), &aeLock);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_MODE */
    ret = update_metadata(metadata_ptr, TYPE_INT32, CONTROL_AE_MODE,
                          sizeof(aeMode), &aeMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_REGIONS */
    ret = update_metadata(metadata_ptr, TYPE_INT32, CONTROL_AE_REGIONS,
                          sizeof(controlRegions), (uint8_t *)controlRegions);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_TARGET_FPS_RANGE */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_TARGET_FPS_RANGE,
                          sizeof(aeTargetFpsRange),
                          (uint8_t *)aeTargetFpsRange);
    if (ret) {
        return ret;
    }

    /* CONTROL_AE_PRECAPTURE_TRIGGER */

    /* CONTROL_AF_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AF_MODE,
                          sizeof(afMode), &afMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_AF_REGIONS */
    ret = update_metadata(metadata_ptr, TYPE_INT32, CONTROL_AF_REGIONS,
                          sizeof(controlRegions), (uint8_t *)controlRegions);
    if (ret) {
        return ret;
    }

    /* CONTROL_AWB_LOCK */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AWB_LOCK,
                          sizeof(awbLock), &awbLock);
    if (ret) {
        return ret;
    }

    /* CONTROL_AWB_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AWB_MODE,
                          sizeof(awbMode), &awbMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_AWB_REGIONS */
    ret = update_metadata(metadata_ptr, TYPE_INT32, CONTROL_AWB_REGIONS,
                          sizeof(controlRegions), (uint8_t *)controlRegions);
    if (ret) {
        return ret;
    }

    /* CONTROL_CAPTURE_INTENT */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_CAPTURE_INTENT,
                          sizeof(controlIntent), &controlIntent);
    if (ret) {
        return ret;
    }

    /* CONTROL_EFFECT_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_EFFECT_MODE,
                          sizeof(effectMode), &effectMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_MODE,
                          sizeof(controlMode), &controlMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_SCENE_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_SCENE_MODE,
                          sizeof(sceneMode), &sceneMode);
    if (ret) {
        return ret;
    }

    /* CONTROL_VIDEO_STABILIZATION_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_VIDEO_STABILIZATION_MODE, sizeof(vstabMode),
                          &vstabMode);
    if (ret) {
        return ret;
    }

    /* FLASH_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, FLASH_MODE,
                          sizeof(flashMode), &flashMode);
    if (ret) {
        return ret;
    }

    /* JPEG_ORIENTATION */
    ret = update_metadata(metadata_ptr, TYPE_INT32, JPEG_ORIENTATION,
                          sizeof(jpegOrientation),
                          (uint8_t *)&jpegOrientation);
    if (ret) {
        return ret;
    }

    /* JPEG_QUALITY */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, JPEG_QUALITY,
                          sizeof(jpegQuality), &jpegQuality);
    if (ret) {
        return ret;
    }

    /* JPEG_THUMBNAIL_SIZE */
    ret = update_metadata(metadata_ptr, TYPE_INT32, JPEG_THUMBNAIL_SIZE,
                          sizeof(thumbnailSize), (uint8_t *)thumbnailSize);
    if (ret) {
        return ret;
    }

    /* LENS_FOCUS_DISTANCE */
    ret = update_metadata(metadata_ptr,TYPE_FLOAT, LENS_FOCUS_DISTANCE,
                          sizeof(focusDistance), (uint8_t *)&focusDistance);
    if (ret) {
        return ret;
    }

    /* SCALER_CROP_REGION */

    /* STATISTICS_FACE_DETECT_MODE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, STATISTICS_FACE_DETECT_MODE,
                          sizeof(faceDetectMode), &faceDetectMode);
    if (ret) {
        return ret;
    }

    /* Do the Data Copy */
    memcpy(cap, &metadata_ptr->header, sizeof(struct camera_metadata_header));
    buffercount = sizeof(struct camera_metadata_header);
    metadata_ptr->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < metadata_ptr->header.entry_count; index++) {
        memcpy(cap, &metadata_ptr->entries[index],
               sizeof(struct camera_metadata_entry));
        buffercount += sizeof(struct camera_metadata_entry);
        cap += sizeof(struct camera_metadata_entry);
    }

    metadata_ptr->header.metadata_data_start = buffercount;
    memcpy(cap, &metadata_ptr->data[0], metadata_ptr->header.size);

    /* total size of metadata package size */
    buffercount += metadata_ptr->header.size;
    metadata_ptr->header.size = buffercount;
    *size = buffercount;
    metadata_ptr->header.metadata_data_count = metadata_ptr->header.entry_count;

    free(metadata_ptr->data);
    free(metadata_ptr->entries);
    free(metadata_ptr);
    return 0;
}

/**
 * @brief update metadata to buffer
 * @param data Pointer to structure of metadata data
 * @param type metadata type
 * @param keyid metadata Key ID
 * @param size metadata size
 * @param values matadata value
 * @return zero for success or non-zero on any faillure
 */
int update_metadata(struct camera_metadata_package *data, uint8_t type,
    Camera_Metadata_type_t keyid, int size, const uint8_t *values)
{
    int nEntry = 0;

    if (data->header.entry_count >= MAX_METADATA_NUMBER) {
        return -ENOMEM;
    }
    nEntry = data->header.entry_count;
    data->entries[nEntry].entry_tag = keyid;
    data->entries[nEntry].data_type = type;
    data->entries[nEntry].data_count = size;

    memcpy(&data->data[data->header.size], (void *)values, size);

    //address alignment
    data->header.size += size;
    data->header.size = ALIGNMENT(data->header.size, 8);
    data->entries[nEntry].data_offset = data->header.size;

    data->header.entry_count++;
    return 0;
}
