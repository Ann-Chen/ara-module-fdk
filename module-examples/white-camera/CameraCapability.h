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

#ifndef FDK_CAMERACAPABILITY_C
#define FDK_CAMERACAPABILITY_C

/* Versioning information */
#define ARA_METADATA_VERSION   1
#define MAX_METADATA_NUMBER    30
#define MAX_METADATA_SIZE      20
#define MAX_RAW_STREAMS        1
#define MAX_STALLING_STREAMS   1
#define MAX_PROCESSED_STREAMS  3
#define ALIGNMENT(size, align) ((((int)size+(align-1))/align)*align)

enum {
    /* Unsigned 8-bit integer (uint8_t) */
    TYPE_BYTE = 0,
    /* Signed 32-bit integer (int32_t) */
    TYPE_INT32 = 1,
    /* 32-bit float (float) */
    TYPE_FLOAT = 2,
    /* Signed 64-bit integer (int64_t) */
    TYPE_INT64 = 3,
    /* 64-bit float (double) */
    TYPE_DOUBLE = 4,
    /* A 64-bit fraction (camera_metadata_rational_t) */
    TYPE_RATIONAL = 5,
    /* Number of type fields */
    NUM_TYPES
};

struct camera_metadata_rational {
    int32_t numerator;
    int32_t denominator;
};

typedef struct camera_metadata_rational camera_metadata_rational_t;

/* Memory format (entries and data separate) */
struct camera_metadata_header {
    uint16_t    version;
    uint16_t    size;
    uint16_t    entry_count;
    uint16_t    entry_start;
    uint16_t    metadata_data_count;
    uint16_t    metadata_data_start;
};

struct camera_metadata_entry {
    uint16_t    entry_tag;
    uint16_t    data_type;
    uint16_t    data_count;
    uint16_t    data_offset;
};

struct camera_metadata_package {
    struct camera_metadata_header   header;
    struct camera_metadata_entry    *entries;
    uint8_t                         *data;
};

typedef enum {
    CONTROL_AE_ANTIBANDING_MODE = 0x05,
    CONTROL_AE_EXPOSURE_COMPENSATION,
    CONTROL_AE_LOCK,
    CONTROL_AE_MODE,
    CONTROL_AE_REGIONS,
    CONTROL_AE_TARGET_FPS_RANGE,
    CONTROL_AE_PRECAPTURE_TRIGGER,
    CONTROL_AF_MODE = 0x0C,
    CONTROL_AF_REGIONS,
    CONTROL_AWB_LOCK = 0x0F,
    CONTROL_AWB_MODE,
    CONTROL_AWB_REGIONS,
    CONTROL_CAPTURE_INTENT,
    CONTROL_EFFECT_MODE,
    CONTROL_MODE,
    CONTROL_SCENE_MODE,
    CONTROL_VIDEO_STABILIZATION_MODE,
    CONTROL_AE_AVAILABLE_ANTIBANDING_MODES = 0x17,
    CONTROL_AE_AVAILABLE_MODES,
    CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
    CONTROL_AE_COMPENSATION_RANGE,
    CONTROL_AE_COMPENSATION_STEP,
    CONTROL_AF_AVAILABLE_MODES,
    CONTROL_AVAILABLE_SCENE_MODES = 0x1E,
    CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES,
    CONTROL_AWB_AVAILABLE_MODES,
    CONTROL_MAX_REGIONS,
    CONTROL_SCENE_MODES_OVERRIDES,
    FLASH_MODE = 0x2F,
    FLASH_INFO_AVAILABLE = 0x33,
    JPEG_ORIENTATION = 0x3A,
    JPEG_QUALITY,
    JPEG_THUMBNAIL_SIZE = 0x3D,
    JPEG_AVAILABLE_THUMBNAIL_SIZES = 0x3E,
    LENS_INFO_AVAILABLE_FOCAL_LENGTHS = 0x43,
    /* LENS_FOCAL_LENGTH = 0x43, Need to confirm */
    LENS_FOCUS_DISTANCE = 0x44,
    LENS_FOCUS_RANGE = 0x49,
    REQUEST_MAX_NUM_OUTPUT_STREAMS = 0x61,
    REQUEST_AVAILABLE_REQUEST_KEYS = 0x68,
    REQUEST_AVALIABLE_RESULT_KEYS,
    REQUEST_AVALIABLE_CHARACTERISTICS,
    SCALER_CROP_REGION = 0x6B,
    SCALER_AVAILABLE_FORMATS = 0x6C,
    SCALER_AVAILABLE_MAX_DIGITAL_ZOOM = 0x6F,
    SENSOR_ORIENTATION = 0x87,
    SENSOR_TIMESTAMP = 0x89,
    SENSOR_INFO_SENSITIVITY_RANGE = 0x95,
    SENSOR_INFO_PHYSICAL_SIZE = 0x99,
    SENSOR_INFO_PIXEL_ARRAY_SIZE = 0x9A,
    STATISTICS_FACE_DETECT_MODE =0x9F,
    STATISTICS_INFO_MAX_FACE_COUNT = 0xB2,
    STATISTICS_FACE_IDS = 0xA3,
    STATISTICS_FACE_LANDMARKS,
    STATISTICS_FACE_RECTANGLES,
    STATISTICS_FACE_SCORES,
    ARA_CM_SUPPORT_JPEG = 0x1000,
    ARA_CM_SUPPORT_SCALER,
    ARA_METADATA_FORMAT_CONFORMANT,
    ARA_METADATA_TRANSPORT,
    ARA_VENDOR_TAGS = 0x8000,
} Camera_Metadata_type_t;

/**
 * Enumeration definitions for the various entries that need them
 */

/* ANDROID_COLOR_CORRECTION_MODE */
typedef enum camera_metadata_enum_android_color_correction_mode {
    ANDROID_COLOR_CORRECTION_MODE_TRANSFORM_MATRIX,
    ANDROID_COLOR_CORRECTION_MODE_FAST,
    ANDROID_COLOR_CORRECTION_MODE_HIGH_QUALITY,
} camera_metadata_enum_android_color_correction_mode_t;

/* ANDROID_COLOR_CORRECTION_ABERRATION_MODE */
typedef enum camera_metadata_enum_android_color_correction_aberration_mode {
    ANDROID_COLOR_CORRECTION_ABERRATION_MODE_OFF,
    ANDROID_COLOR_CORRECTION_ABERRATION_MODE_FAST,
    ANDROID_COLOR_CORRECTION_ABERRATION_MODE_HIGH_QUALITY,
} camera_metadata_enum_android_color_correction_aberration_mode_t;

/* ANDROID_CONTROL_AE_ANTIBANDING_MODE */
typedef enum camera_metadata_enum_android_control_ae_antibanding_mode {
    ANDROID_CONTROL_AE_ANTIBANDING_MODE_OFF,
    ANDROID_CONTROL_AE_ANTIBANDING_MODE_50HZ,
    ANDROID_CONTROL_AE_ANTIBANDING_MODE_60HZ,
    ANDROID_CONTROL_AE_ANTIBANDING_MODE_AUTO,
} camera_metadata_enum_android_control_ae_antibanding_mode_t;

/* ANDROID_CONTROL_AE_LOCK */
typedef enum camera_metadata_enum_android_control_ae_lock {
    ANDROID_CONTROL_AE_LOCK_OFF,
    ANDROID_CONTROL_AE_LOCK_ON,
} camera_metadata_enum_android_control_ae_lock_t;

/* ANDROID_CONTROL_AE_MODE */
typedef enum camera_metadata_enum_android_control_ae_mode {
    ANDROID_CONTROL_AE_MODE_OFF,
    ANDROID_CONTROL_AE_MODE_ON,
    ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH,
    ANDROID_CONTROL_AE_MODE_ON_ALWAYS_FLASH,
    ANDROID_CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE,
} camera_metadata_enum_android_control_ae_mode_t;

/* ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER */
typedef enum camera_metadata_enum_android_control_ae_precapture_trigger {
    ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_IDLE,
    ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_START,
    ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_CANCEL,
} camera_metadata_enum_android_control_ae_precapture_trigger_t;

/* ANDROID_CONTROL_AF_MODE */
typedef enum camera_metadata_enum_android_control_af_mode {
    ANDROID_CONTROL_AF_MODE_OFF,
    ANDROID_CONTROL_AF_MODE_AUTO,
    ANDROID_CONTROL_AF_MODE_MACRO,
    ANDROID_CONTROL_AF_MODE_CONTINUOUS_VIDEO,
    ANDROID_CONTROL_AF_MODE_CONTINUOUS_PICTURE,
    ANDROID_CONTROL_AF_MODE_EDOF,
} camera_metadata_enum_android_control_af_mode_t;

/* ANDROID_CONTROL_AF_TRIGGER */
typedef enum camera_metadata_enum_android_control_af_trigger {
    ANDROID_CONTROL_AF_TRIGGER_IDLE,
    ANDROID_CONTROL_AF_TRIGGER_START,
    ANDROID_CONTROL_AF_TRIGGER_CANCEL,
} camera_metadata_enum_android_control_af_trigger_t;

/* ANDROID_CONTROL_AWB_LOCK */
typedef enum camera_metadata_enum_android_control_awb_lock {
    ANDROID_CONTROL_AWB_LOCK_OFF,
    ANDROID_CONTROL_AWB_LOCK_ON,
} camera_metadata_enum_android_control_awb_lock_t;

/* ANDROID_CONTROL_AWB_MODE */
typedef enum camera_metadata_enum_android_control_awb_mode {
    ANDROID_CONTROL_AWB_MODE_OFF,
    ANDROID_CONTROL_AWB_MODE_AUTO,
    ANDROID_CONTROL_AWB_MODE_INCANDESCENT,
    ANDROID_CONTROL_AWB_MODE_FLUORESCENT,
    ANDROID_CONTROL_AWB_MODE_WARM_FLUORESCENT,
    ANDROID_CONTROL_AWB_MODE_DAYLIGHT,
    ANDROID_CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
    ANDROID_CONTROL_AWB_MODE_TWILIGHT,
    ANDROID_CONTROL_AWB_MODE_SHADE,
} camera_metadata_enum_android_control_awb_mode_t;

/* ANDROID_CONTROL_CAPTURE_INTENT */
typedef enum camera_metadata_enum_android_control_capture_intent {
    ANDROID_CONTROL_CAPTURE_INTENT_CUSTOM,
    ANDROID_CONTROL_CAPTURE_INTENT_PREVIEW,
    ANDROID_CONTROL_CAPTURE_INTENT_STILL_CAPTURE,
    ANDROID_CONTROL_CAPTURE_INTENT_VIDEO_RECORD,
    ANDROID_CONTROL_CAPTURE_INTENT_VIDEO_SNAPSHOT,
    ANDROID_CONTROL_CAPTURE_INTENT_ZERO_SHUTTER_LAG,
    ANDROID_CONTROL_CAPTURE_INTENT_MANUAL,
} camera_metadata_enum_android_control_capture_intent_t;

/* ANDROID_CONTROL_EFFECT_MODE */
typedef enum camera_metadata_enum_android_control_effect_mode {
    ANDROID_CONTROL_EFFECT_MODE_OFF,
    ANDROID_CONTROL_EFFECT_MODE_MONO,
    ANDROID_CONTROL_EFFECT_MODE_NEGATIVE,
    ANDROID_CONTROL_EFFECT_MODE_SOLARIZE,
    ANDROID_CONTROL_EFFECT_MODE_SEPIA,
    ANDROID_CONTROL_EFFECT_MODE_POSTERIZE,
    ANDROID_CONTROL_EFFECT_MODE_WHITEBOARD,
    ANDROID_CONTROL_EFFECT_MODE_BLACKBOARD,
    ANDROID_CONTROL_EFFECT_MODE_AQUA,
} camera_metadata_enum_android_control_effect_mode_t;

/* ANDROID_CONTROL_MODE */
typedef enum camera_metadata_enum_android_control_mode {
    ANDROID_CONTROL_MODE_OFF,
    ANDROID_CONTROL_MODE_AUTO,
    ANDROID_CONTROL_MODE_USE_SCENE_MODE,
    ANDROID_CONTROL_MODE_OFF_KEEP_STATE,
} camera_metadata_enum_android_control_mode_t;

/* ANDROID_CONTROL_SCENE_MODE */
typedef enum camera_metadata_enum_android_control_scene_mode {
    ANDROID_CONTROL_SCENE_MODE_DISABLED                         = 0,
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
    ANDROID_CONTROL_SCENE_MODE_HDR,
    ANDROID_CONTROL_SCENE_MODE_FACE_PRIORITY_LOW_LIGHT,
} camera_metadata_enum_android_control_scene_mode_t;

/* ANDROID_CONTROL_VIDEO_STABILIZATION_MODE */
typedef enum camera_metadata_enum_android_control_video_stabilization_mode {
    ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_OFF,
    ANDROID_CONTROL_VIDEO_STABILIZATION_MODE_ON,
} camera_metadata_enum_android_control_video_stabilization_mode_t;

/* ANDROID_CONTROL_AE_STATE */
typedef enum camera_metadata_enum_android_control_ae_state {
    ANDROID_CONTROL_AE_STATE_INACTIVE,
    ANDROID_CONTROL_AE_STATE_SEARCHING,
    ANDROID_CONTROL_AE_STATE_CONVERGED,
    ANDROID_CONTROL_AE_STATE_LOCKED,
    ANDROID_CONTROL_AE_STATE_FLASH_REQUIRED,
    ANDROID_CONTROL_AE_STATE_PRECAPTURE,
} camera_metadata_enum_android_control_ae_state_t;

/* ANDROID_CONTROL_AF_STATE */
typedef enum camera_metadata_enum_android_control_af_state {
    ANDROID_CONTROL_AF_STATE_INACTIVE,
    ANDROID_CONTROL_AF_STATE_PASSIVE_SCAN,
    ANDROID_CONTROL_AF_STATE_PASSIVE_FOCUSED,
    ANDROID_CONTROL_AF_STATE_ACTIVE_SCAN,
    ANDROID_CONTROL_AF_STATE_FOCUSED_LOCKED,
    ANDROID_CONTROL_AF_STATE_NOT_FOCUSED_LOCKED,
    ANDROID_CONTROL_AF_STATE_PASSIVE_UNFOCUSED,
} camera_metadata_enum_android_control_af_state_t;

/* ANDROID_CONTROL_AWB_STATE */
typedef enum camera_metadata_enum_android_control_awb_state {
    ANDROID_CONTROL_AWB_STATE_INACTIVE,
    ANDROID_CONTROL_AWB_STATE_SEARCHING,
    ANDROID_CONTROL_AWB_STATE_CONVERGED,
    ANDROID_CONTROL_AWB_STATE_LOCKED,
} camera_metadata_enum_android_control_awb_state_t;

/* ANDROID_CONTROL_AE_LOCK_AVAILABLE */
typedef enum camera_metadata_enum_android_control_ae_lock_available {
    ANDROID_CONTROL_AE_LOCK_AVAILABLE_FALSE,
    ANDROID_CONTROL_AE_LOCK_AVAILABLE_TRUE,
} camera_metadata_enum_android_control_ae_lock_available_t;

/* ANDROID_CONTROL_AWB_LOCK_AVAILABLE */
typedef enum camera_metadata_enum_android_control_awb_lock_available {
    ANDROID_CONTROL_AWB_LOCK_AVAILABLE_FALSE,
    ANDROID_CONTROL_AWB_LOCK_AVAILABLE_TRUE,
} camera_metadata_enum_android_control_awb_lock_available_t;

/* ANDROID_DEMOSAIC_MODE */
typedef enum camera_metadata_enum_android_demosaic_mode {
    ANDROID_DEMOSAIC_MODE_FAST,
    ANDROID_DEMOSAIC_MODE_HIGH_QUALITY,
} camera_metadata_enum_android_demosaic_mode_t;

/* ANDROID_EDGE_MODE */
typedef enum camera_metadata_enum_android_edge_mode {
    ANDROID_EDGE_MODE_OFF,
    ANDROID_EDGE_MODE_FAST,
    ANDROID_EDGE_MODE_HIGH_QUALITY,
    ANDROID_EDGE_MODE_ZERO_SHUTTER_LAG,
} camera_metadata_enum_android_edge_mode_t;

/* ANDROID_FLASH_MODE */
typedef enum camera_metadata_enum_android_flash_mode {
    ANDROID_FLASH_MODE_OFF,
    ANDROID_FLASH_MODE_SINGLE,
    ANDROID_FLASH_MODE_TORCH,
} camera_metadata_enum_android_flash_mode_t;

/* ANDROID_FLASH_STATE */
typedef enum camera_metadata_enum_android_flash_state {
    ANDROID_FLASH_STATE_UNAVAILABLE,
    ANDROID_FLASH_STATE_CHARGING,
    ANDROID_FLASH_STATE_READY,
    ANDROID_FLASH_STATE_FIRED,
    ANDROID_FLASH_STATE_PARTIAL,
} camera_metadata_enum_android_flash_state_t;

/* ANDROID_FLASH_INFO_AVAILABLE */
typedef enum camera_metadata_enum_android_flash_info_available {
    ANDROID_FLASH_INFO_AVAILABLE_FALSE,
    ANDROID_FLASH_INFO_AVAILABLE_TRUE,
} camera_metadata_enum_android_flash_info_available_t;

/* ANDROID_HOT_PIXEL_MODE */
typedef enum camera_metadata_enum_android_hot_pixel_mode {
    ANDROID_HOT_PIXEL_MODE_OFF,
    ANDROID_HOT_PIXEL_MODE_FAST,
    ANDROID_HOT_PIXEL_MODE_HIGH_QUALITY,
} camera_metadata_enum_android_hot_pixel_mode_t;

/* ANDROID_LENS_OPTICAL_STABILIZATION_MODE */
typedef enum camera_metadata_enum_android_lens_optical_stabilization_mode {
    ANDROID_LENS_OPTICAL_STABILIZATION_MODE_OFF,
    ANDROID_LENS_OPTICAL_STABILIZATION_MODE_ON,
} camera_metadata_enum_android_lens_optical_stabilization_mode_t;

/* ANDROID_LENS_FACING */
typedef enum camera_metadata_enum_android_lens_facing {
    ANDROID_LENS_FACING_FRONT,
    ANDROID_LENS_FACING_BACK,
    ANDROID_LENS_FACING_EXTERNAL,
} camera_metadata_enum_android_lens_facing_t;

/* ANDROID_LENS_STATE */
typedef enum camera_metadata_enum_android_lens_state {
    ANDROID_LENS_STATE_STATIONARY,
    ANDROID_LENS_STATE_MOVING,
} camera_metadata_enum_android_lens_state_t;

/* ANDROID_LENS_INFO_FOCUS_DISTANCE_CALIBRATION */
typedef enum
  camera_metadata_enum_android_lens_info_focus_distance_calibration {
    ANDROID_LENS_INFO_FOCUS_DISTANCE_CALIBRATION_UNCALIBRATED,
    ANDROID_LENS_INFO_FOCUS_DISTANCE_CALIBRATION_APPROXIMATE,
    ANDROID_LENS_INFO_FOCUS_DISTANCE_CALIBRATION_CALIBRATED,
} camera_metadata_enum_android_lens_info_focus_distance_calibration_t;

/* ANDROID_NOISE_REDUCTION_MODE */
typedef enum camera_metadata_enum_android_noise_reduction_mode {
    ANDROID_NOISE_REDUCTION_MODE_OFF,
    ANDROID_NOISE_REDUCTION_MODE_FAST,
    ANDROID_NOISE_REDUCTION_MODE_HIGH_QUALITY,
    ANDROID_NOISE_REDUCTION_MODE_MINIMAL,
    ANDROID_NOISE_REDUCTION_MODE_ZERO_SHUTTER_LAG,
} camera_metadata_enum_android_noise_reduction_mode_t;

/* ANDROID_QUIRKS_PARTIAL_RESULT */
typedef enum camera_metadata_enum_android_quirks_partial_result {
    ANDROID_QUIRKS_PARTIAL_RESULT_FINAL,
    ANDROID_QUIRKS_PARTIAL_RESULT_PARTIAL,
} camera_metadata_enum_android_quirks_partial_result_t;

/* ANDROID_REQUEST_METADATA_MODE */
typedef enum camera_metadata_enum_android_request_metadata_mode {
    ANDROID_REQUEST_METADATA_MODE_NONE,
    ANDROID_REQUEST_METADATA_MODE_FULL,
} camera_metadata_enum_android_request_metadata_mode_t;

/* ANDROID_REQUEST_TYPE */
typedef enum camera_metadata_enum_android_request_type {
    ANDROID_REQUEST_TYPE_CAPTURE,
    ANDROID_REQUEST_TYPE_REPROCESS,
} camera_metadata_enum_android_request_type_t;

/* ANDROID_REQUEST_AVAILABLE_CAPABILITIES */
typedef enum camera_metadata_enum_android_request_available_capabilities {
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_BACKWARD_COMPATIBLE,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_MANUAL_SENSOR,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_MANUAL_POST_PROCESSING,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_RAW,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_PRIVATE_REPROCESSING,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_READ_SENSOR_SETTINGS,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_BURST_CAPTURE,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_YUV_REPROCESSING,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_DEPTH_OUTPUT,
    ANDROID_REQUEST_AVAILABLE_CAPABILITIES_CONSTRAINED_HIGH_SPEED_VIDEO,
} camera_metadata_enum_android_request_available_capabilities_t;

/* ANDROID_SCALER_AVAILABLE_FORMATS */
typedef enum camera_metadata_enum_android_scaler_available_formats {
    ANDROID_SCALER_AVAILABLE_FORMATS_RAW16                      = 0x20,
    ANDROID_SCALER_AVAILABLE_FORMATS_RAW_OPAQUE                 = 0x24,
    ANDROID_SCALER_AVAILABLE_FORMATS_YV12                       = 0x32315659,
    ANDROID_SCALER_AVAILABLE_FORMATS_YCrCb_420_SP               = 0x11,
    ANDROID_SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED     = 0x22,
    ANDROID_SCALER_AVAILABLE_FORMATS_YCbCr_420_888              = 0x23,
    ANDROID_SCALER_AVAILABLE_FORMATS_BLOB                       = 0x21,
} camera_metadata_enum_android_scaler_available_formats_t;

/* ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS */
typedef enum
  camera_metadata_enum_android_scaler_available_stream_configurations {
    ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT,
    ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_INPUT,
} camera_metadata_enum_android_scaler_available_stream_configurations_t;

/* ANDROID_SCALER_CROPPING_TYPE */
typedef enum camera_metadata_enum_android_scaler_cropping_type {
    ANDROID_SCALER_CROPPING_TYPE_CENTER_ONLY,
    ANDROID_SCALER_CROPPING_TYPE_FREEFORM,
} camera_metadata_enum_android_scaler_cropping_type_t;

/* ANDROID_SENSOR_REFERENCE_ILLUMINANT1 */
typedef enum camera_metadata_enum_android_sensor_reference_illuminant1 {
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT               = 1,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_FLUORESCENT            = 2,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_TUNGSTEN               = 3,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_FLASH                  = 4,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_FINE_WEATHER           = 9,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_CLOUDY_WEATHER         = 10,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_SHADE                  = 11,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT_FLUORESCENT   = 12,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_DAY_WHITE_FLUORESCENT  = 13,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_COOL_WHITE_FLUORESCENT = 14,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_WHITE_FLUORESCENT      = 15,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_A             = 17,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_B             = 18,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_C             = 19,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_D55                    = 20,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_D65                    = 21,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_D75                    = 22,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_D50                    = 23,
    ANDROID_SENSOR_REFERENCE_ILLUMINANT1_ISO_STUDIO_TUNGSTEN    = 24,
} camera_metadata_enum_android_sensor_reference_illuminant1_t;

/* ANDROID_SENSOR_TEST_PATTERN_MODE */
typedef enum camera_metadata_enum_android_sensor_test_pattern_mode {
    ANDROID_SENSOR_TEST_PATTERN_MODE_OFF,
    ANDROID_SENSOR_TEST_PATTERN_MODE_SOLID_COLOR,
    ANDROID_SENSOR_TEST_PATTERN_MODE_COLOR_BARS,
    ANDROID_SENSOR_TEST_PATTERN_MODE_COLOR_BARS_FADE_TO_GRAY,
    ANDROID_SENSOR_TEST_PATTERN_MODE_PN9,
    ANDROID_SENSOR_TEST_PATTERN_MODE_CUSTOM1                    = 256,
} camera_metadata_enum_android_sensor_test_pattern_mode_t;

/* ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT */
typedef enum camera_metadata_enum_android_sensor_info_color_filter_arrangement
{
    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGGB,
    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GRBG,
    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GBRG,
    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_BGGR,
    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGB,
} camera_metadata_enum_android_sensor_info_color_filter_arrangement_t;

/* ANDROID_SENSOR_INFO_TIMESTAMP_SOURCE */
typedef enum camera_metadata_enum_android_sensor_info_timestamp_source {
    ANDROID_SENSOR_INFO_TIMESTAMP_SOURCE_UNKNOWN,
    ANDROID_SENSOR_INFO_TIMESTAMP_SOURCE_REALTIME,
} camera_metadata_enum_android_sensor_info_timestamp_source_t;

/* ANDROID_SENSOR_INFO_LENS_SHADING_APPLIED */
typedef enum camera_metadata_enum_android_sensor_info_lens_shading_applied {
    ANDROID_SENSOR_INFO_LENS_SHADING_APPLIED_FALSE,
    ANDROID_SENSOR_INFO_LENS_SHADING_APPLIED_TRUE,
} camera_metadata_enum_android_sensor_info_lens_shading_applied_t;

/* ANDROID_SHADING_MODE */
typedef enum camera_metadata_enum_android_shading_mode {
    ANDROID_SHADING_MODE_OFF,
    ANDROID_SHADING_MODE_FAST,
    ANDROID_SHADING_MODE_HIGH_QUALITY,
} camera_metadata_enum_android_shading_mode_t;

/* ANDROID_STATISTICS_FACE_DETECT_MODE */
typedef enum camera_metadata_enum_android_statistics_face_detect_mode {
    ANDROID_STATISTICS_FACE_DETECT_MODE_OFF,
    ANDROID_STATISTICS_FACE_DETECT_MODE_SIMPLE,
    ANDROID_STATISTICS_FACE_DETECT_MODE_FULL,
} camera_metadata_enum_android_statistics_face_detect_mode_t;

/* ANDROID_STATISTICS_HISTOGRAM_MODE */
typedef enum camera_metadata_enum_android_statistics_histogram_mode {
    ANDROID_STATISTICS_HISTOGRAM_MODE_OFF,
    ANDROID_STATISTICS_HISTOGRAM_MODE_ON,
} camera_metadata_enum_android_statistics_histogram_mode_t;

/* ANDROID_STATISTICS_SHARPNESS_MAP_MODE */
typedef enum camera_metadata_enum_android_statistics_sharpness_map_mode {
    ANDROID_STATISTICS_SHARPNESS_MAP_MODE_OFF,
    ANDROID_STATISTICS_SHARPNESS_MAP_MODE_ON,
} camera_metadata_enum_android_statistics_sharpness_map_mode_t;

/* ANDROID_STATISTICS_HOT_PIXEL_MAP_MODE */
typedef enum camera_metadata_enum_android_statistics_hot_pixel_map_mode {
    ANDROID_STATISTICS_HOT_PIXEL_MAP_MODE_OFF,
    ANDROID_STATISTICS_HOT_PIXEL_MAP_MODE_ON,
} camera_metadata_enum_android_statistics_hot_pixel_map_mode_t;

/* ANDROID_STATISTICS_SCENE_FLICKER */
typedef enum camera_metadata_enum_android_statistics_scene_flicker {
    ANDROID_STATISTICS_SCENE_FLICKER_NONE,
    ANDROID_STATISTICS_SCENE_FLICKER_50HZ,
    ANDROID_STATISTICS_SCENE_FLICKER_60HZ,
} camera_metadata_enum_android_statistics_scene_flicker_t;

/* ANDROID_STATISTICS_LENS_SHADING_MAP_MODE */
typedef enum camera_metadata_enum_android_statistics_lens_shading_map_mode {
    ANDROID_STATISTICS_LENS_SHADING_MAP_MODE_OFF,
    ANDROID_STATISTICS_LENS_SHADING_MAP_MODE_ON,
} camera_metadata_enum_android_statistics_lens_shading_map_mode_t;

/* ANDROID_TONEMAP_MODE */
typedef enum camera_metadata_enum_android_tonemap_mode {
    ANDROID_TONEMAP_MODE_CONTRAST_CURVE,
    ANDROID_TONEMAP_MODE_FAST,
    ANDROID_TONEMAP_MODE_HIGH_QUALITY,
    ANDROID_TONEMAP_MODE_GAMMA_VALUE,
    ANDROID_TONEMAP_MODE_PRESET_CURVE,
} camera_metadata_enum_android_tonemap_mode_t;

/* ANDROID_TONEMAP_PRESET_CURVE */
typedef enum camera_metadata_enum_android_tonemap_preset_curve {
    ANDROID_TONEMAP_PRESET_CURVE_SRGB,
    ANDROID_TONEMAP_PRESET_CURVE_REC709,
} camera_metadata_enum_android_tonemap_preset_curve_t;

/* ANDROID_LED_TRANSMIT */
typedef enum camera_metadata_enum_android_led_transmit {
    ANDROID_LED_TRANSMIT_OFF,
    ANDROID_LED_TRANSMIT_ON,
} camera_metadata_enum_android_led_transmit_t;

/* ANDROID_LED_AVAILABLE_LEDS */
typedef enum camera_metadata_enum_android_led_available_leds {
    ANDROID_LED_AVAILABLE_LEDS_TRANSMIT,
} camera_metadata_enum_android_led_available_leds_t;

/* ANDROID_INFO_SUPPORTED_HARDWARE_LEVEL */
typedef enum camera_metadata_enum_android_info_supported_hardware_level {
    ANDROID_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED,
    ANDROID_INFO_SUPPORTED_HARDWARE_LEVEL_FULL,
    ANDROID_INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY,
} camera_metadata_enum_android_info_supported_hardware_level_t;

/* ANDROID_BLACK_LEVEL_LOCK */
typedef enum camera_metadata_enum_android_black_level_lock {
    ANDROID_BLACK_LEVEL_LOCK_OFF,
    ANDROID_BLACK_LEVEL_LOCK_ON,
} camera_metadata_enum_android_black_level_lock_t;

/* ANDROID_SYNC_FRAME_NUMBER */
typedef enum camera_metadata_enum_android_sync_frame_number {
    ANDROID_SYNC_FRAME_NUMBER_CONVERGING                        = -1,
    ANDROID_SYNC_FRAME_NUMBER_UNKNOWN                           = -2,
} camera_metadata_enum_android_sync_frame_number_t;

/* ANDROID_SYNC_MAX_LATENCY */
typedef enum camera_metadata_enum_android_sync_max_latency {
    ANDROID_SYNC_MAX_LATENCY_PER_FRAME_CONTROL                  = 0,
    ANDROID_SYNC_MAX_LATENCY_UNKNOWN                            = -1,
} camera_metadata_enum_android_sync_max_latency_t;

/* ANDROID_DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS */
typedef enum
  camera_metadata_enum_android_depth_available_depth_stream_configurations {
    ANDROID_DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS_OUTPUT,
    ANDROID_DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS_INPUT,
} camera_metadata_enum_android_depth_available_depth_stream_configurations_t;

/* ANDROID_DEPTH_DEPTH_IS_EXCLUSIVE */
typedef enum camera_metadata_enum_android_depth_depth_is_exclusive {
    ANDROID_DEPTH_DEPTH_IS_EXCLUSIVE_FALSE,
    ANDROID_DEPTH_DEPTH_IS_EXCLUSIVE_TRUE,
} camera_metadata_enum_android_depth_depth_is_exclusive_t;

/**
 * @brief Camera sensor Capabilities
 * @param buffer size
 * @param buffer address
 * @return OK for success or ERROR on any faillure
 */
int getCapabilities(uint32_t *size, uint8_t *cap);

/**
 * @brief Camera sensor capture
 * @param buffer size
 * @param buffer address
 * @return OK for success or ERROR on any faillure
 */
int getDefaultRequestSettings(int32_t *size, uint8_t *cap);

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
    Camera_Metadata_type_t KeyiD, int size, const uint8_t *values);

#endif
