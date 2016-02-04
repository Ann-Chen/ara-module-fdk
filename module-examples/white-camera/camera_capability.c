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
#include "camera_capability.h"

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
        CONTROL_AE_ANTIBANDING_MODE_OFF,
        CONTROL_AE_ANTIBANDING_MODE_50HZ,
        CONTROL_AE_ANTIBANDING_MODE_60HZ,
        CONTROL_AE_ANTIBANDING_MODE_AUTO
    };
    const uint8_t availableAeModes[] = {
        CONTROL_AE_MODE_OFF,
        CONTROL_AE_MODE_ON,
        CONTROL_AE_MODE_ON_AUTO_FLASH,
        CONTROL_AE_MODE_ON_ALWAYS_FLASH,
        CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE
    };
    const int32_t availableTargetFpsRanges[] = {5, 30, 15, 30};
    const int32_t exposureCompensationRange[] = {-9, 9};
    const camera_metadata_rational_t exposureCompensationStep = {1, 3};
    const uint8_t availableAfModesBack[] = {
        CONTROL_AF_MODE_OFF,
        CONTROL_AF_MODE_AUTO,
        CONTROL_AF_MODE_MACRO,
        CONTROL_AF_MODE_CONTINUOUS_VIDEO,
        CONTROL_AF_MODE_CONTINUOUS_PICTURE
    };
    const uint8_t availableSceneModes[] = {
        CONTROL_SCENE_MODE_DISABLED,
        CONTROL_SCENE_MODE_FACE_PRIORITY,
        CONTROL_SCENE_MODE_ACTION,
        CONTROL_SCENE_MODE_PORTRAIT,
        CONTROL_SCENE_MODE_LANDSCAPE,
        CONTROL_SCENE_MODE_NIGHT,
        CONTROL_SCENE_MODE_NIGHT_PORTRAIT,
        CONTROL_SCENE_MODE_THEATRE,
        CONTROL_SCENE_MODE_BEACH,
        CONTROL_SCENE_MODE_SNOW,
        CONTROL_SCENE_MODE_SUNSET,
        CONTROL_SCENE_MODE_STEADYPHOTO,
        CONTROL_SCENE_MODE_FIREWORKS,
        CONTROL_SCENE_MODE_SPORTS,
        CONTROL_SCENE_MODE_PARTY,
        CONTROL_SCENE_MODE_CANDLELIGHT,
        CONTROL_SCENE_MODE_BARCODE,
        CONTROL_SCENE_MODE_HIGH_SPEED_VIDEO,
        CONTROL_SCENE_MODE_HDR
    };
    const uint8_t availableVstabModes[] = {
        CONTROL_VIDEO_STABILIZATION_MODE_OFF,
        CONTROL_VIDEO_STABILIZATION_MODE_ON
    };
    const uint8_t availableAwbModes[] = {
        CONTROL_AWB_MODE_OFF,
        CONTROL_AWB_MODE_AUTO,
        CONTROL_AWB_MODE_INCANDESCENT,
        CONTROL_AWB_MODE_FLUORESCENT,
        CONTROL_AWB_MODE_WARM_FLUORESCENT,
        CONTROL_AWB_MODE_DAYLIGHT,
        CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
        CONTROL_AWB_MODE_TWILIGHT,
        CONTROL_AWB_MODE_SHADE,
    };
    const int32_t max3aRegions[] = {0, 0, 0};
    const uint8_t flashAvailable = FLASH_INFO_AVAILABLE_FALSE;
    const int32_t jpegThumbnailSizes[] = {0, 0, 160, 120, 320, 240};
    const float focalLength = 2.50f;
    const int32_t max_output_streams[] = {
        MAX_STALLING_STREAMS,
        MAX_PROCESSED_STREAMS,
        MAX_RAW_STREAMS
    };
    const int32_t scalar_formats[] = {
        SCALER_AVAILABLE_FORMATS_RAW16,
        SCALER_AVAILABLE_FORMATS_RAW_OPAQUE,
        SCALER_AVAILABLE_FORMATS_YV12,
        SCALER_AVAILABLE_FORMATS_YCrCb_420_SP,
        SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED,
        SCALER_AVAILABLE_FORMATS_YCbCr_420_888,
        SCALER_AVAILABLE_FORMATS_BLOB
    };
    const float maxZoom = 10.0f;
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
        goto err_free_metadata_ptr;
    }

    /* need to modify it */
    metadata_ptr->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (!metadata_ptr->data ) {
        goto err_free_metadata;
    }
    metadata_ptr->header.version = ARA_METADATA_VERSION;

    /* CONTROL_AE_AVAILABLE_ANTIBANDING_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,
                          sizeof(availableAntibandingModes),
                          availableAntibandingModes);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AE_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AE_AVAILABLE_MODES,
                          sizeof(availableAeModes), availableAeModes);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
                          sizeof(availableTargetFpsRanges),
                          (uint8_t *)availableTargetFpsRanges);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AE_COMPENSATION_STEP */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          CONTROL_AE_COMPENSATION_RANGE,
                          sizeof(exposureCompensationRange),
                          (uint8_t *)exposureCompensationRange);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AE_COMPENSATION_STEP */
    ret = update_metadata(metadata_ptr, TYPE_RATIONAL,
                          CONTROL_AE_COMPENSATION_STEP,
                          sizeof(exposureCompensationStep),
                          (uint8_t *)&exposureCompensationStep);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AF_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AF_AVAILABLE_MODES,
                          sizeof(availableAfModesBack), availableAfModesBack);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AVAILABLE_SCENE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AVAILABLE_SCENE_MODES,
                          sizeof(availableSceneModes), availableSceneModes);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE,
                          CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES,
                          sizeof(availableVstabModes), availableVstabModes);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_AWB_AVAILABLE_MODES */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_AWB_AVAILABLE_MODES,
                          sizeof(availableAwbModes), availableAwbModes);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_MAX_REGIONS */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, CONTROL_MAX_REGIONS,
                          sizeof(max3aRegions), (uint8_t *)max3aRegions);
    if (ret) {
        goto err_free_all;
    }

    /* CONTROL_SCENE_MODES_OVERRIDES */

    /* FLASH_INFO_AVAILABLE */
    ret = update_metadata(metadata_ptr, TYPE_BYTE, FLASH_INFO_AVAILABLE,
                          sizeof(flashAvailable), &flashAvailable);
    if (ret) {
        goto err_free_all;
    }

    /* JPEG_AVAILABLE_THUMBNAIL_SIZES */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          JPEG_AVAILABLE_THUMBNAIL_SIZES,
                          sizeof(jpegThumbnailSizes),
                          (uint8_t *)jpegThumbnailSizes);
    if (ret) {
        goto err_free_all;
    }

    /* LENS_INFO_AVAILABLE_FOCAL_LENGTHS */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT,
                          LENS_INFO_AVAILABLE_FOCAL_LENGTHS,
                          sizeof(focalLength), (uint8_t *)&focalLength);
    if (ret) {
        goto err_free_all;
    }

    /* REQUEST_MAX_NUM_OUTPUT_STREAMS */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          REQUEST_MAX_NUM_OUTPUT_STREAMS,
                          sizeof(max_output_streams),
                          (uint8_t *)max_output_streams);
    if (ret) {
        goto err_free_all;
    }

    /* REQUEST_AVAILABLE_REQUEST_KEYS = 0x68 */
    /* REQUEST_AVALIABLE_RESULT_KEYS */
    /* REQUEST_AVALIABLE_CHARACTERISTICS */

    /* SCALER_AVAILABLE_FORMATS */
    ret = update_metadata(metadata_ptr, TYPE_INT32, SCALER_AVAILABLE_FORMATS,
                          sizeof(scalar_formats), (uint8_t *)scalar_formats);
    if (ret) {
        goto err_free_all;
    }

    /* SCALER_AVAILABLE_MAX_DIGITAL_ZOOM */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT,
                          SCALER_AVAILABLE_MAX_DIGITAL_ZOOM,
                          sizeof(maxZoom), (uint8_t *)&maxZoom);
    if (ret) {
        goto err_free_all;
    }

    /* SENSOR_ORIENTATION */
    ret = update_metadata(metadata_ptr, TYPE_INT32, SENSOR_ORIENTATION,
                          sizeof(orientation), (uint8_t *)&orientation);
    if (ret) {
        goto err_free_all;
    }

    /* SENSOR_INFO_SENSITIVITY_RANGE */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          SENSOR_INFO_SENSITIVITY_RANGE,
                          sizeof(SensitivityRange),
                          (uint8_t *)SensitivityRange);
    if (ret) {
        goto err_free_all;
    }

    /* SENSOR_INFO_PHYSICAL_SIZE */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT, SENSOR_INFO_PHYSICAL_SIZE,
                          sizeof(sensorPhysicalSize),
                          (uint8_t *)sensorPhysicalSize);
    if (ret) {
        goto err_free_all;
    }

    /* SENSOR_INFO_PIXEL_ARRAY_SIZE */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          SENSOR_INFO_PIXEL_ARRAY_SIZE,
                          sizeof(Resolution), (uint8_t *)Resolution);
    if (ret) {
        goto err_free_all;
    }

    /* STATISTICS_INFO_MAX_FACE_COUNT */
    ret = update_metadata(metadata_ptr, TYPE_INT32,
                          STATISTICS_INFO_MAX_FACE_COUNT,
                          sizeof(maxFaceCount), (uint8_t *)&maxFaceCount);
    if (ret) {
        goto err_free_all;
    }

    /* Do the Data Copy */
    memcpy(cap, &metadata_ptr->header, sizeof(metadata_ptr->header));
    buffercount = sizeof(metadata_ptr->header);
    metadata_ptr->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < metadata_ptr->header.entry_count; index++) {
        memcpy(cap, &metadata_ptr->entries[index],
               sizeof(metadata_ptr->entries[0]));
        buffercount += sizeof(metadata_ptr->entries[0]);
        cap += sizeof(metadata_ptr->entries[0]);
    }

    metadata_ptr->header.metadata_data_start = buffercount;
    memcpy(cap, &metadata_ptr->data[0], metadata_ptr->header.size);

    /* total size of metadata package size */
    buffercount += metadata_ptr->header.size;
    metadata_ptr->header.size = buffercount;
    *size = buffercount;
    metadata_ptr->header.metadata_data_count =
        metadata_ptr->header.entry_count;

    free(metadata_ptr->data);
    free(metadata_ptr->entries);
    free(metadata_ptr);
    return 0;

err_free_all:
    free(metadata_ptr->data);
err_free_metadata:
    free(metadata_ptr->entries);
err_free_metadata_ptr:
    free(metadata_ptr);
    return -ENOMEM;
}

/**
 * @brief Camera sensor capture result metadata
 * @param size buffer size
 * @param capabilities buffer address
 * @return zero for success or non-zero on any faillure
 */
int get_capture_results_metadata(uint32_t *size, uint8_t *capabilities)
{
    struct camera_metadata_package  *metadata_ptr;
    uint8_t *cap;
    int index = 0, buffercount = 0, ret;

    const float focalLength = 2.50f;
    const float focusDistance = 0;

    /* buffer initial */
    cap = capabilities;
    metadata_ptr = zalloc(sizeof(struct camera_metadata_package));
    if (!metadata_ptr)
        return -ENOMEM;

    metadata_ptr->entries = zalloc(MAX_METADATA_NUMBER *
                                   sizeof(struct camera_metadata_entry));
    if (!metadata_ptr->entries) {
        free(metadata_ptr);
        goto err_free_metadata_ptr;
    }

    metadata_ptr->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE);
    if (!metadata_ptr->data ) {
        goto err_free_metadata;
    }
    metadata_ptr->header.version = ARA_METADATA_VERSION;

    /* LENS_FOCUS_RANGE */
    /* SENSOR_TIMESTAMP */
    /* STATISTICS_FACE_IDS */
    /* STATISTICS_FACE_LANDMARKS */
    /* STATISTICS_FACE_RECTANGLES */
    /* STATISTICS_FACE_SCORES */

    /* LENS_INFO_AVAILABLE_FOCAL_LENGTHS */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT,
                          LENS_INFO_AVAILABLE_FOCAL_LENGTHS,
                          sizeof(focalLength), (uint8_t *)&focalLength);
    if (ret) {
        goto err_free_all;
    }

    /* LENS_FOCUS_DISTANCE */
    ret = update_metadata(metadata_ptr, TYPE_FLOAT, LENS_FOCUS_DISTANCE,
                          sizeof(focusDistance), (uint8_t *)&focusDistance);
    if (ret) {
        goto err_free_all;
    }

    /* Do the Data Copy */
    memcpy(cap, &metadata_ptr->header, sizeof(metadata_ptr->header));
    buffercount = sizeof(metadata_ptr->header);
    metadata_ptr->header.entry_start = buffercount;
    cap += buffercount;

    for (index = 0; index < metadata_ptr->header.entry_count; index++) {
        memcpy(cap, &metadata_ptr->entries[index],
               sizeof(metadata_ptr->entries[0]));
        buffercount += sizeof(metadata_ptr->entries[0]);
        cap += sizeof(metadata_ptr->entries[0]);
    }

    metadata_ptr->header.metadata_data_start = buffercount;
    memcpy(cap, &metadata_ptr->data[0], metadata_ptr->header.size);

    /* total size of metadata package size */
    buffercount += metadata_ptr->header.size;
    metadata_ptr->header.size = buffercount;
    *size = buffercount;
    metadata_ptr->header.metadata_data_count =
        metadata_ptr->header.entry_count;

    free(metadata_ptr->data);
    free(metadata_ptr->entries);
    free(metadata_ptr);
    return 0;

err_free_all:
    free(metadata_ptr->data);
err_free_metadata:
    free(metadata_ptr->entries);
err_free_metadata_ptr:
    free(metadata_ptr);
    return -ENOMEM;
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
        printf("update_metadata no MEMORY\n");
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
