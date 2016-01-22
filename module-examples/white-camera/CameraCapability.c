/*
 * Copyright (c) 2015 Google, Inc.
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

int getCapabilities(uint32_t *size, uint8_t *cap)
{
	struct camera_metadata_package  *pMetaData;
	int index = 0, buffercount = 0;

	//init
	pMetaData = zalloc(sizeof(struct camera_metadata_package));
	if(NULL == pMetaData) 
	    return ERROR;

	pMetaData->entries = zalloc(MAX_METADATA_NUMBER * sizeof(struct camera_metadata_entry));
	if(NULL == pMetaData->entries) {
        free(pMetaData);
        return ERROR;
    }  	
	
	pMetaData->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE); //need to modify it 
	if(NULL == pMetaData->data ) {
        free(pMetaData);
        free(pMetaData->entries);
        return ERROR;
    }
	pMetaData->header.version = ARA_METADATA_VERSION;
		
	//CONTROL_AE_AVAILABLE_ANTIBANDING_MODES
    static const uint8_t availableAntibandingModes[] = {
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_OFF,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_50HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_60HZ,
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO
    };
    updateMetadata(pMetaData,TYPE_BYTE, CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,
	    sizeof(availableAntibandingModes), availableAntibandingModes);

    //CONTROL_AE_AVAILABLE_MODES
    static const uint8_t availableAeModes[] = {
        ANDROID_CONTROL_AE_MODE_OFF,
        ANDROID_CONTROL_AE_MODE_ON,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_ALWAYS_FLASH,
        ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE
    };
    updateMetadata(pMetaData,TYPE_BYTE, CONTROL_AE_AVAILABLE_MODES,
	    sizeof(availableAeModes), availableAeModes);

    //CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES
    static const int32_t availableTargetFpsRanges[] = {
        5, 30, 15, 30
    };
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES, sizeof(availableTargetFpsRanges), (uint8_t *)availableTargetFpsRanges);

    //CONTROL_AE_COMPENSATION_STEP
	static const int32_t exposureCompensationRange[] = {-9, 9};
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_COMPENSATION_RANGE, sizeof(exposureCompensationRange), exposureCompensationRange);

    //CONTROL_AE_COMPENSATION_STEP
    /*static const camera_metadata_rational exposureCompensationStep = {1, 3};
    updateMetadata(pMetaData,TYPE_RATIONAL,
		CONTROL_AE_COMPENSATION_STEP, sizeof(exposureCompensationRange), exposureCompensationRange);*/

    //CONTROL_AF_AVAILABLE_MODES
    static const uint8_t availableAfModesBack[] = {
        ANDROID_CONTROL_AF_MODE_OFF,
        ANDROID_CONTROL_AF_MODE_AUTO,
        ANDROID_CONTROL_AF_MODE_MACRO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_VIDEO,
        ANDROID_CONTROL_AF_MODE_CONTINUOUS_PICTURE
    };
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AF_AVAILABLE_MODES, sizeof(availableAfModesBack), availableAfModesBack);

    //CONTROL_AVAILABLE_SCENE_MODES
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
    updateMetadata(pMetaData,TYPE_BYTE,
        CONTROL_AVAILABLE_SCENE_MODES, sizeof(availableSceneModes), availableSceneModes);

    //CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES
    static const uint8_t availableVstabModes[] = {
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF,
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_ON	
    };
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES, sizeof(availableVstabModes), availableVstabModes);

	//CONTROL_AWB_AVAILABLE_MODES
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
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AWB_AVAILABLE_MODES, sizeof(availableAwbModes), availableAwbModes);

    //CONTROL_MAX_REGIONS
    static const int32_t max3aRegions[] = {/*AE*/ 0,/*AWB*/ 0,/*AF*/ 0};
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_MAX_REGIONS, sizeof(max3aRegions), max3aRegions);

    //CONTROL_SCENE_MODES_OVERRIDES
    //Need to implement

    //FLASH_INFO_AVAILABLE
    static const uint8_t flashAvailable = ANDROID_FLASH_INFO_AVAILABLE_FALSE;
    updateMetadata(pMetaData,TYPE_BYTE,
		FLASH_INFO_AVAILABLE, sizeof(flashAvailable), &flashAvailable);

	// android.jpeg
	//JPEG_AVAILABLE_THUMBNAIL_SIZES
    static const int32_t jpegThumbnailSizes[] = {
        0, 0,
        160, 120,
        320, 240
     };
	updateMetadata(pMetaData,TYPE_INT32,
		JPEG_AVAILABLE_THUMBNAIL_SIZES, sizeof(jpegThumbnailSizes), jpegThumbnailSizes);

	//LENS_INFO_AVAILABLE_FOCAL_LENGTHS
	static const float focalLength = 3.30f; // mm
	updateMetadata(pMetaData,TYPE_FLOAT,
		LENS_INFO_AVAILABLE_FOCAL_LENGTHS, sizeof(focalLength), &focalLength);    

    //REQUEST_MAX_NUM_OUTPUT_STREAMS
    static const int32_t max_output_streams[] = {
        MAX_STALLING_STREAMS,
        MAX_PROCESSED_STREAMS,
        MAX_RAW_STREAMS
    };
	updateMetadata(pMetaData,TYPE_INT32,
		REQUEST_MAX_NUM_OUTPUT_STREAMS, sizeof(max_output_streams), max_output_streams);

    //REQUEST_AVAILABLE_REQUEST_KEYS = 0x68,
	//REQUEST_AVALIABLE_RESULT_KEYS,
	//REQUEST_AVALIABLE_CHARACTERISTICS,	
	
	//SCALER_AVAILABLE_FORMATS
	static const int32_t scalar_formats[] = {
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW16,
        ANDROID_SCALER_AVAILABLE_FORMATS_RAW_OPAQUE,
        ANDROID_SCALER_AVAILABLE_FORMATS_YV12,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCrCb_420_SP,
        ANDROID_SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED,
        ANDROID_SCALER_AVAILABLE_FORMATS_YCbCr_420_888,
        ANDROID_SCALER_AVAILABLE_FORMATS_BLOB
	};
    updateMetadata(pMetaData,TYPE_INT32,
		SCALER_AVAILABLE_FORMATS, sizeof(scalar_formats), scalar_formats);
		
    //SCALER_AVAILABLE_MAX_DIGITAL_ZOOM
	static const float maxZoom = 10;
    updateMetadata(pMetaData,TYPE_FLOAT,
		SCALER_AVAILABLE_MAX_DIGITAL_ZOOM, sizeof(maxZoom), &maxZoom);
			
    //SENSOR_ORIENTATION
	static const int32_t orientation = 0; // unrotated (0 degrees)
    updateMetadata(pMetaData,TYPE_INT32,
		SENSOR_ORIENTATION, sizeof(orientation), &orientation);

    //SENSOR_INFO_SENSITIVITY_RANGE
	static const int32_t SensitivityRange[2] = {100, 1600};
    updateMetadata(pMetaData,TYPE_INT32,
		SENSOR_INFO_SENSITIVITY_RANGE, sizeof(SensitivityRange), SensitivityRange);
	
	//SENSOR_INFO_PHYSICAL_SIZE
    static const float sensorPhysicalSize[2] = {3.20f, 2.40f}; // mm
    updateMetadata(pMetaData,TYPE_FLOAT,
		SENSOR_INFO_PHYSICAL_SIZE, sizeof(sensorPhysicalSize), sensorPhysicalSize);
	
	//SENSOR_INFO_PIXEL_ARRAY_SIZE
	static const int32_t Resolution[]  =  {640, 480};
     updateMetadata(pMetaData,TYPE_INT32,
		SENSOR_INFO_PIXEL_ARRAY_SIZE, sizeof(Resolution), Resolution);	

    //STATISTICS_INFO_MAX_FACE_COUNT
    static const int32_t maxFaceCount = 8;
    updateMetadata(pMetaData,TYPE_INT32,
		STATISTICS_INFO_MAX_FACE_COUNT, sizeof(maxFaceCount), &maxFaceCount);

    //Do the Data Copy
    memcpy(cap, &pMetaData->header, sizeof(struct camera_metadata_header));
	buffercount = sizeof(struct camera_metadata_header);
	pMetaData->header.entry_start = buffercount;
	cap += buffercount;
        
	for (index = 0; index < pMetaData->header.entry_count; index++) {
		memcpy(cap, &pMetaData->entries[index], sizeof(struct camera_metadata_entry));
		buffercount += sizeof(struct camera_metadata_entry);
	    cap += sizeof(struct camera_metadata_entry);
	}
    
	pMetaData->header.metadata_data_start = buffercount;
	memcpy(cap, &pMetaData->data[0], pMetaData->header.size);

	buffercount += pMetaData->header.size; //total size of metadata package size
	pMetaData->header.size = buffercount;
    *size = buffercount;
    pMetaData->header.metadata_data_count = pMetaData->header.entry_count;
    /*
    printf("=========================== \n");    
    printf("pMetaData->header.version = %d\n", pMetaData->header.version);
    printf("pMetaData->header.size = %d\n", pMetaData->header.size);
    printf("pMetaData->header.entry_count = %d\n", pMetaData->header.entry_count);
    printf("pMetaData->header.entry_start = %d\n", pMetaData->header.entry_start);    
    printf("pMetaData->header.metadata_data_count = %d\n", pMetaData->header.metadata_data_count);
    printf("pMetaData->header.metadata_data_start = %d\n", pMetaData->header.metadata_data_start); 
    */
	//Copy header, entries and metadata to one buffer
	free(pMetaData->data);
	free(pMetaData->entries);
	free(pMetaData);
    return OK;
}

int getDefaultRequestSettings(int32_t *size, uint8_t *cap)
{
	struct camera_metadata_package  *pMetaData;
	int index = 0, buffercount = 0;

	//init
	pMetaData = zalloc(sizeof(struct camera_metadata_package));
	if(NULL == pMetaData) 
	    return ERROR;

	pMetaData->entries = zalloc(MAX_METADATA_NUMBER * sizeof(struct camera_metadata_entry));
	if(NULL == pMetaData->entries) {
        free(pMetaData);
        return ERROR;
    }  	
	
	pMetaData->data = zalloc(MAX_METADATA_NUMBER * MAX_METADATA_SIZE); //need to modify it 
	if(NULL == pMetaData->data ) {
        free(pMetaData);
        free(pMetaData->entries);
        return ERROR;
    }
	pMetaData->header.version = ARA_METADATA_VERSION;

    //android.control
    //CONTROL_AE_ANTIBANDING_MODE
    static const uint8_t aeAntibandingMode =
        ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AE_ANTIBANDING_MODE, sizeof(aeAntibandingMode), &aeAntibandingMode);

    //CONTROL_AE_EXPOSURE_COMPENSATION
    static const int32_t aeExpCompensation = 0;
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_EXPOSURE_COMPENSATION, sizeof(aeExpCompensation), &aeExpCompensation);

    //CONTROL_AE_LOCK
	static const uint8_t aeLock = ANDROID_CONTROL_AE_LOCK_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AE_LOCK, sizeof(aeLock), &aeLock);

    //CONTROL_AE_MODE
    static const uint8_t aeMode = ANDROID_CONTROL_AE_MODE_ON;
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_MODE, sizeof(aeMode), &aeMode);

    //CONTROL_AE_REGIONS
	//const int32_t Resolution[2]  = {640, 480};
    static const int32_t controlRegions[5] = {
        0, 0, 640, 480, 1000
    };
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_REGIONS, sizeof(controlRegions), controlRegions);

    //CONTROL_AE_TARGET_FPS_RANGE
    static const int32_t aeTargetFpsRange[2] = {
        10, 30
    };
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AE_TARGET_FPS_RANGE, sizeof(aeTargetFpsRange), aeTargetFpsRange);

	//CONTROL_AE_PRECAPTURE_TRIGGER,

    //CONTROL_AF_MODE
    uint8_t afMode = ANDROID_CONTROL_AF_MODE_AUTO;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AF_MODE, sizeof(afMode), &afMode);

    //CONTROL_AF_REGIONS
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AF_REGIONS, sizeof(controlRegions), controlRegions);

    //CONTROL_AWB_LOCK
    static const uint8_t awbLock = ANDROID_CONTROL_AWB_LOCK_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AWB_LOCK, sizeof(awbLock), &awbLock);

    //CONTROL_AWB_MODE
    static const uint8_t awbMode =
        ANDROID_CONTROL_AWB_MODE_AUTO;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_AWB_MODE, sizeof(awbMode), &awbMode);

    //CONTROL_AWB_REGIONS
    updateMetadata(pMetaData,TYPE_INT32,
		CONTROL_AWB_REGIONS, sizeof(controlRegions), controlRegions);

    //CONTROL_CAPTURE_INTENT
    uint8_t controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_PREVIEW;
    /*switch (type) {
      case CAMERA3_TEMPLATE_PREVIEW:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_PREVIEW;
        break;
      case CAMERA3_TEMPLATE_STILL_CAPTURE:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_STILL_CAPTURE;
        break;
      case CAMERA3_TEMPLATE_VIDEO_RECORD:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_VIDEO_RECORD;
        break;
      case CAMERA3_TEMPLATE_VIDEO_SNAPSHOT:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_VIDEO_SNAPSHOT;
        break;
      case CAMERA3_TEMPLATE_ZERO_SHUTTER_LAG:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_ZERO_SHUTTER_LAG;
        break;
      default:
        controlIntent = ANDROID_CONTROL_CAPTURE_INTENT_CUSTOM;
        break;
    }*/
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_CAPTURE_INTENT, sizeof(controlIntent), &controlIntent);

    //CONTROL_EFFECT_MODE
    static const uint8_t effectMode = ANDROID_CONTROL_EFFECT_MODE_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_EFFECT_MODE, sizeof(effectMode), &effectMode);

    //CONTROL_MODE
    static const uint8_t controlMode = ANDROID_CONTROL_MODE_AUTO;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_MODE, sizeof(controlMode), &controlMode);

    //CONTROL_SCENE_MODE
    static const uint8_t sceneMode = ANDROID_CONTROL_SCENE_MODE_FACE_PRIORITY;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_SCENE_MODE, sizeof(sceneMode), &sceneMode);

    //CONTROL_VIDEO_STABILIZATION_MODE
    static const uint8_t vstabMode =
        ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		CONTROL_VIDEO_STABILIZATION_MODE, sizeof(vstabMode), &vstabMode);

    //android.flash
    //FLASH_MODE
    static const uint8_t flashMode = ANDROID_FLASH_MODE_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		FLASH_MODE, sizeof(flashMode), &flashMode);

    //android.jpeg
    //JPEG_ORIENTATION
    static const int32_t jpegOrientation = 0;
    updateMetadata(pMetaData,TYPE_INT32,
		JPEG_ORIENTATION, sizeof(jpegOrientation), &jpegOrientation);

    //JPEG_QUALITY
    static const uint8_t jpegQuality = 80;
    updateMetadata(pMetaData,TYPE_BYTE,
		JPEG_QUALITY, sizeof(jpegQuality), &jpegQuality);

    //JPEG_THUMBNAIL_SIZE
    static const int32_t thumbnailSize[2] = {
        640, 480
    };
    updateMetadata(pMetaData,TYPE_INT32,
		JPEG_THUMBNAIL_SIZE, sizeof(thumbnailSize), thumbnailSize);

    // android.lens 
    //LENS_FOCUS_DISTANCE  
    static const float focusDistance = 0;
    updateMetadata(pMetaData,TYPE_FLOAT,
		LENS_FOCUS_DISTANCE, sizeof(focusDistance), &focusDistance);

    // android.scaler
    //SCALER_CROP_REGION
    //static const int32_t cropRegion[3] = {
    //    0, 0, (int32_t)Sensor::kResolution[0]
    //};
    //updateMetadata(pMetaData,TYPE_INT32,
	//	SCALER_CROP_REGION, sizeof(cropRegion), cropRegion);

	// android.stats
    //STATISTICS_FACE_DETECT_MODE
    static const uint8_t faceDetectMode =
        ANDROID_STATISTICS_FACE_DETECT_MODE_OFF;
    updateMetadata(pMetaData,TYPE_BYTE,
		STATISTICS_FACE_DETECT_MODE, sizeof(faceDetectMode), &faceDetectMode);

    //Do the Data Copy
    memcpy(cap, &pMetaData->header, sizeof(struct camera_metadata_header));
	buffercount = sizeof(struct camera_metadata_header);
	pMetaData->header.entry_start = buffercount;
	cap += buffercount;
    printf("cap =0x %x\n", cap);
    printf("header buffercount =%d\n", buffercount);

	for (index = 0; index < pMetaData->header.entry_count; index++) {
		memcpy(cap, &pMetaData->entries[index], sizeof(struct camera_metadata_entry));
		buffercount += sizeof(struct camera_metadata_entry);
	    cap += sizeof(struct camera_metadata_entry);
	}

	pMetaData->header.metadata_data_start = buffercount;
	memcpy(cap, &pMetaData->data[0], pMetaData->header.size);

	buffercount += pMetaData->header.size; //total size of metadata package size
	pMetaData->header.size = buffercount;
    *size = buffercount;
    pMetaData->header.metadata_data_count = pMetaData->header.entry_count;

/*    printf("=========================== \n");
    printf("pMetaData->header.version = %d\n", pMetaData->header.version);
    printf("pMetaData->header.size = %d\n", pMetaData->header.size);
    printf("pMetaData->header.entry_count = %d\n", pMetaData->header.entry_count);
    printf("pMetaData->header.entry_start = %d\n", pMetaData->header.entry_start);
    printf("pMetaData->header.metadata_data_count = %d\n", pMetaData->header.metadata_data_count);
    printf("pMetaData->header.metadata_data_start = %d\n", pMetaData->header.metadata_data_start);
*/
	//Copy header, entries and metadata to one buffer
	free(pMetaData->data);
	free(pMetaData->entries);
	free(pMetaData);
    return OK;
}


int updateMetadata(struct camera_metadata_package *pData, uint8_t type,
    Camera_Metadata_type_t KeyiD, int size, const uint8_t *values)
{
    int nEntry = 0,addr_offset=0;

    if(pData->header.entry_count >= MAX_METADATA_NUMBER) {
    	//printf("%s, Memory is not enough\n"__func__);
        return ERROR;
	}
	nEntry = pData->header.entry_count;
    pData->entries[nEntry].entry_tag = KeyiD;
	pData->entries[nEntry].data_type = type;
	pData->entries[nEntry].data_count = size;

    memcpy(&pData->data[pData->header.size], (void *)values, size);

    //address alignment
    if((size % ALGINMENT_BITS) != 0)
	    addr_offset +=  (ALGINMENT_BITS-(size % ALGINMENT_BITS));
    pData->header.size += size + addr_offset;
    pData->entries[nEntry].data_offset = pData->header.size;

    pData->header.entry_count++;
    return OK;
}
