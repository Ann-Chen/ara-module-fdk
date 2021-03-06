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
#define ARA_METADATA_VERSION        1
#define MAX_METADATA_NUMBER         30
#define MAX_METADATA_SIZE           20
#define MAX_RAW_STREAMS             1
#define MAX_STALLING_STREAMS        1
#define MAX_PROCESSED_STREAMS       3
#define ALIGNMENT(size, align) \
        ((((int)size + (align - 1)) / align) * align)
#define SIZE_CAPABILITIES_VALUE             436
#define SIZE_CAPTURE_RESULTS_METADATA_VALUE 44

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

/* COLOR_CORRECTION_MODE */
typedef enum camera_metadata_enum_color_correction_mode {
    COLOR_CORRECTION_MODE_TRANSFORM_MATRIX,
    COLOR_CORRECTION_MODE_FAST,
    COLOR_CORRECTION_MODE_HIGH_QUALITY,
} camera_metadata_enum_color_correction_mode_t;

/* COLOR_CORRECTION_ABERRATION_MODE */
typedef enum camera_metadata_enum_color_correction_aberration_mode {
    COLOR_CORRECTION_ABERRATION_MODE_OFF,
    COLOR_CORRECTION_ABERRATION_MODE_FAST,
    COLOR_CORRECTION_ABERRATION_MODE_HIGH_QUALITY,
} camera_metadata_enum_color_correction_aberration_mode_t;

/* CONTROL_AE_ANTIBANDING_MODE */
typedef enum camera_metadata_enum_control_ae_antibanding_mode {
    CONTROL_AE_ANTIBANDING_MODE_OFF,
    CONTROL_AE_ANTIBANDING_MODE_50HZ,
    CONTROL_AE_ANTIBANDING_MODE_60HZ,
    CONTROL_AE_ANTIBANDING_MODE_AUTO,
} camera_metadata_enum_control_ae_antibanding_mode_t;

/* CONTROL_AE_LOCK */
typedef enum camera_metadata_enum_control_ae_lock {
    CONTROL_AE_LOCK_OFF,
    CONTROL_AE_LOCK_ON,
} camera_metadata_enum_control_ae_lock_t;

/* CONTROL_AE_MODE */
typedef enum camera_metadata_enum_control_ae_mode {
    CONTROL_AE_MODE_OFF,
    CONTROL_AE_MODE_ON,
    CONTROL_AE_MODE_ON_AUTO_FLASH,
    CONTROL_AE_MODE_ON_ALWAYS_FLASH,
    CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE,
} camera_metadata_enum_control_ae_mode_t;

/* CONTROL_AE_PRECAPTURE_TRIGGER */
typedef enum camera_metadata_enum_control_ae_precapture_trigger {
    CONTROL_AE_PRECAPTURE_TRIGGER_IDLE,
    CONTROL_AE_PRECAPTURE_TRIGGER_START,
    CONTROL_AE_PRECAPTURE_TRIGGER_CANCEL,
} camera_metadata_enum_control_ae_precapture_trigger_t;

/* CONTROL_AF_MODE */
typedef enum camera_metadata_enum_control_af_mode {
    CONTROL_AF_MODE_OFF,
    CONTROL_AF_MODE_AUTO,
    CONTROL_AF_MODE_MACRO,
    CONTROL_AF_MODE_CONTINUOUS_VIDEO,
    CONTROL_AF_MODE_CONTINUOUS_PICTURE,
    CONTROL_AF_MODE_EDOF,
} camera_metadata_enum_control_af_mode_t;

/* CONTROL_AF_TRIGGER */
typedef enum camera_metadata_enum_control_af_trigger {
    CONTROL_AF_TRIGGER_IDLE,
    CONTROL_AF_TRIGGER_START,
    CONTROL_AF_TRIGGER_CANCEL,
} camera_metadata_enum_control_af_trigger_t;

/* CONTROL_AWB_LOCK */
typedef enum camera_metadata_enum_control_awb_lock {
    CONTROL_AWB_LOCK_OFF,
    CONTROL_AWB_LOCK_ON,
} camera_metadata_enum_control_awb_lock_t;

/* CONTROL_AWB_MODE */
typedef enum camera_metadata_enum_control_awb_mode {
    CONTROL_AWB_MODE_OFF,
    CONTROL_AWB_MODE_AUTO,
    CONTROL_AWB_MODE_INCANDESCENT,
    CONTROL_AWB_MODE_FLUORESCENT,
    CONTROL_AWB_MODE_WARM_FLUORESCENT,
    CONTROL_AWB_MODE_DAYLIGHT,
    CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
    CONTROL_AWB_MODE_TWILIGHT,
    CONTROL_AWB_MODE_SHADE,
} camera_metadata_enum_control_awb_mode_t;

/* CONTROL_CAPTURE_INTENT */
typedef enum camera_metadata_enum_control_capture_intent {
    CONTROL_CAPTURE_INTENT_CUSTOM,
    CONTROL_CAPTURE_INTENT_PREVIEW,
    CONTROL_CAPTURE_INTENT_STILL_CAPTURE,
    CONTROL_CAPTURE_INTENT_VIDEO_RECORD,
    CONTROL_CAPTURE_INTENT_VIDEO_SNAPSHOT,
    CONTROL_CAPTURE_INTENT_ZERO_SHUTTER_LAG,
    CONTROL_CAPTURE_INTENT_MANUAL,
} camera_metadata_enum_control_capture_intent_t;

/* CONTROL_EFFECT_MODE */
typedef enum camera_metadata_enum_control_effect_mode {
    CONTROL_EFFECT_MODE_OFF,
    CONTROL_EFFECT_MODE_MONO,
    CONTROL_EFFECT_MODE_NEGATIVE,
    CONTROL_EFFECT_MODE_SOLARIZE,
    CONTROL_EFFECT_MODE_SEPIA,
    CONTROL_EFFECT_MODE_POSTERIZE,
    CONTROL_EFFECT_MODE_WHITEBOARD,
    CONTROL_EFFECT_MODE_BLACKBOARD,
    CONTROL_EFFECT_MODE_AQUA,
} camera_metadata_enum_control_effect_mode_t;

/* CONTROL_MODE */
typedef enum camera_metadata_enum_control_mode {
    CONTROL_MODE_OFF,
    CONTROL_MODE_AUTO,
    CONTROL_MODE_USE_SCENE_MODE,
    CONTROL_MODE_OFF_KEEP_STATE,
} camera_metadata_enum_control_mode_t;

/* CONTROL_SCENE_MODE */
typedef enum camera_metadata_enum_control_scene_mode {
    CONTROL_SCENE_MODE_DISABLED                         = 0,
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
    CONTROL_SCENE_MODE_HDR,
    CONTROL_SCENE_MODE_FACE_PRIORITY_LOW_LIGHT,
} camera_metadata_enum_control_scene_mode_t;

/* CONTROL_VIDEO_STABILIZATION_MODE */
typedef enum camera_metadata_enum_control_video_stabilization_mode {
    CONTROL_VIDEO_STABILIZATION_MODE_OFF,
    CONTROL_VIDEO_STABILIZATION_MODE_ON,
} camera_metadata_enum_control_video_stabilization_mode_t;

/* CONTROL_AE_STATE */
typedef enum camera_metadata_enum_control_ae_state {
    CONTROL_AE_STATE_INACTIVE,
    CONTROL_AE_STATE_SEARCHING,
    CONTROL_AE_STATE_CONVERGED,
    CONTROL_AE_STATE_LOCKED,
    CONTROL_AE_STATE_FLASH_REQUIRED,
    CONTROL_AE_STATE_PRECAPTURE,
} camera_metadata_enum_control_ae_state_t;

/* CONTROL_AF_STATE */
typedef enum camera_metadata_enum_control_af_state {
    CONTROL_AF_STATE_INACTIVE,
    CONTROL_AF_STATE_PASSIVE_SCAN,
    CONTROL_AF_STATE_PASSIVE_FOCUSED,
    CONTROL_AF_STATE_ACTIVE_SCAN,
    CONTROL_AF_STATE_FOCUSED_LOCKED,
    CONTROL_AF_STATE_NOT_FOCUSED_LOCKED,
    CONTROL_AF_STATE_PASSIVE_UNFOCUSED,
} camera_metadata_enum_control_af_state_t;

/* CONTROL_AWB_STATE */
typedef enum camera_metadata_enum_control_awb_state {
    CONTROL_AWB_STATE_INACTIVE,
    CONTROL_AWB_STATE_SEARCHING,
    CONTROL_AWB_STATE_CONVERGED,
    CONTROL_AWB_STATE_LOCKED,
} camera_metadata_enum_control_awb_state_t;

/* CONTROL_AE_LOCK_AVAILABLE */
typedef enum camera_metadata_enum_control_ae_lock_available {
    CONTROL_AE_LOCK_AVAILABLE_FALSE,
    CONTROL_AE_LOCK_AVAILABLE_TRUE,
} camera_metadata_enum_control_ae_lock_available_t;

/* CONTROL_AWB_LOCK_AVAILABLE */
typedef enum camera_metadata_enum_control_awb_lock_available {
    CONTROL_AWB_LOCK_AVAILABLE_FALSE,
    CONTROL_AWB_LOCK_AVAILABLE_TRUE,
} camera_metadata_enum_control_awb_lock_available_t;

/* DEMOSAIC_MODE */
typedef enum camera_metadata_enum_demosaic_mode {
    DEMOSAIC_MODE_FAST,
    DEMOSAIC_MODE_HIGH_QUALITY,
} camera_metadata_enum_demosaic_mode_t;

/* EDGE_MODE */
typedef enum camera_metadata_enum_edge_mode {
    EDGE_MODE_OFF,
    EDGE_MODE_FAST,
    EDGE_MODE_HIGH_QUALITY,
    EDGE_MODE_ZERO_SHUTTER_LAG,
} camera_metadata_enum_edge_mode_t;

/* FLASH_MODE */
typedef enum camera_metadata_enum_flash_mode {
    FLASH_MODE_OFF,
    FLASH_MODE_SINGLE,
    FLASH_MODE_TORCH,
} camera_metadata_enum_flash_mode_t;

/* FLASH_STATE */
typedef enum camera_metadata_enum_flash_state {
    FLASH_STATE_UNAVAILABLE,
    FLASH_STATE_CHARGING,
    FLASH_STATE_READY,
    FLASH_STATE_FIRED,
    FLASH_STATE_PARTIAL,
} camera_metadata_enum_flash_state_t;

/* FLASH_INFO_AVAILABLE */
typedef enum camera_metadata_enum_flash_info_available {
    FLASH_INFO_AVAILABLE_FALSE,
    FLASH_INFO_AVAILABLE_TRUE,
} camera_metadata_enum_flash_info_available_t;

/* HOT_PIXEL_MODE */
typedef enum camera_metadata_enum_hot_pixel_mode {
    HOT_PIXEL_MODE_OFF,
    HOT_PIXEL_MODE_FAST,
    HOT_PIXEL_MODE_HIGH_QUALITY,
} camera_metadata_enum_hot_pixel_mode_t;

/* LENS_OPTICAL_STABILIZATION_MODE */
typedef enum camera_metadata_enum_lens_optical_stabilization_mode {
    LENS_OPTICAL_STABILIZATION_MODE_OFF,
    LENS_OPTICAL_STABILIZATION_MODE_ON,
} camera_metadata_enum_lens_optical_stabilization_mode_t;

/* LENS_FACING */
typedef enum camera_metadata_enum_lens_facing {
    LENS_FACING_FRONT,
    LENS_FACING_BACK,
    LENS_FACING_EXTERNAL,
} camera_metadata_enum_lens_facing_t;

/* LENS_STATE */
typedef enum camera_metadata_enum_lens_state {
    LENS_STATE_STATIONARY,
    LENS_STATE_MOVING,
} camera_metadata_enum_lens_state_t;

/* LENS_INFO_FOCUS_DISTANCE_CALIBRATION */
typedef enum
  camera_metadata_enum_lens_info_focus_distance_calibration {
    LENS_INFO_FOCUS_DISTANCE_CALIBRATION_UNCALIBRATED,
    LENS_INFO_FOCUS_DISTANCE_CALIBRATION_APPROXIMATE,
    LENS_INFO_FOCUS_DISTANCE_CALIBRATION_CALIBRATED,
} camera_metadata_enum_lens_info_focus_distance_calibration_t;

/* NOISE_REDUCTION_MODE */
typedef enum camera_metadata_enum_noise_reduction_mode {
    NOISE_REDUCTION_MODE_OFF,
    NOISE_REDUCTION_MODE_FAST,
    NOISE_REDUCTION_MODE_HIGH_QUALITY,
    NOISE_REDUCTION_MODE_MINIMAL,
    NOISE_REDUCTION_MODE_ZERO_SHUTTER_LAG,
} camera_metadata_enum_noise_reduction_mode_t;

/* QUIRKS_PARTIAL_RESULT */
typedef enum camera_metadata_enum_quirks_partial_result {
    QUIRKS_PARTIAL_RESULT_FINAL,
    QUIRKS_PARTIAL_RESULT_PARTIAL,
} camera_metadata_enum_quirks_partial_result_t;

/* REQUEST_METADATA_MODE */
typedef enum camera_metadata_enum_request_metadata_mode {
    REQUEST_METADATA_MODE_NONE,
    REQUEST_METADATA_MODE_FULL,
} camera_metadata_enum_request_metadata_mode_t;

/* REQUEST_TYPE */
typedef enum camera_metadata_enum_request_type {
    REQUEST_TYPE_CAPTURE,
    REQUEST_TYPE_REPROCESS,
} camera_metadata_enum_request_type_t;

/* REQUEST_AVAILABLE_CAPABILITIES */
typedef enum camera_metadata_enum_request_available_capabilities {
    REQUEST_AVAILABLE_CAPABILITIES_BACKWARD_COMPATIBLE,
    REQUEST_AVAILABLE_CAPABILITIES_MANUAL_SENSOR,
    REQUEST_AVAILABLE_CAPABILITIES_MANUAL_POST_PROCESSING,
    REQUEST_AVAILABLE_CAPABILITIES_RAW,
    REQUEST_AVAILABLE_CAPABILITIES_PRIVATE_REPROCESSING,
    REQUEST_AVAILABLE_CAPABILITIES_READ_SENSOR_SETTINGS,
    REQUEST_AVAILABLE_CAPABILITIES_BURST_CAPTURE,
    REQUEST_AVAILABLE_CAPABILITIES_YUV_REPROCESSING,
    REQUEST_AVAILABLE_CAPABILITIES_DEPTH_OUTPUT,
    REQUEST_AVAILABLE_CAPABILITIES_CONSTRAINED_HIGH_SPEED_VIDEO,
} camera_metadata_enum_request_available_capabilities_t;

/* SCALER_AVAILABLE_FORMATS */
typedef enum camera_metadata_enum_scaler_available_formats {
    SCALER_AVAILABLE_FORMATS_RAW16                      = 0x20,
    SCALER_AVAILABLE_FORMATS_RAW_OPAQUE                 = 0x24,
    SCALER_AVAILABLE_FORMATS_YV12                       = 0x32315659,
    SCALER_AVAILABLE_FORMATS_YCrCb_420_SP               = 0x11,
    SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED     = 0x22,
    SCALER_AVAILABLE_FORMATS_YCbCr_420_888              = 0x23,
    SCALER_AVAILABLE_FORMATS_BLOB                       = 0x21,
} camera_metadata_enum_scaler_available_formats_t;

/* SCALER_AVAILABLE_STREAM_CONFIGURATIONS */
typedef enum
  camera_metadata_enum_scaler_available_stream_configurations {
    SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT,
    SCALER_AVAILABLE_STREAM_CONFIGURATIONS_INPUT,
} camera_metadata_enum_scaler_available_stream_configurations_t;

/* SCALER_CROPPING_TYPE */
typedef enum camera_metadata_enum_scaler_cropping_type {
    SCALER_CROPPING_TYPE_CENTER_ONLY,
    SCALER_CROPPING_TYPE_FREEFORM,
} camera_metadata_enum_scaler_cropping_type_t;

/* SENSOR_REFERENCE_ILLUMINANT1 */
typedef enum camera_metadata_enum_sensor_reference_illuminant1 {
    SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT               = 1,
    SENSOR_REFERENCE_ILLUMINANT1_FLUORESCENT            = 2,
    SENSOR_REFERENCE_ILLUMINANT1_TUNGSTEN               = 3,
    SENSOR_REFERENCE_ILLUMINANT1_FLASH                  = 4,
    SENSOR_REFERENCE_ILLUMINANT1_FINE_WEATHER           = 9,
    SENSOR_REFERENCE_ILLUMINANT1_CLOUDY_WEATHER         = 10,
    SENSOR_REFERENCE_ILLUMINANT1_SHADE                  = 11,
    SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT_FLUORESCENT   = 12,
    SENSOR_REFERENCE_ILLUMINANT1_DAY_WHITE_FLUORESCENT  = 13,
    SENSOR_REFERENCE_ILLUMINANT1_COOL_WHITE_FLUORESCENT = 14,
    SENSOR_REFERENCE_ILLUMINANT1_WHITE_FLUORESCENT      = 15,
    SENSOR_REFERENCE_ILLUMINANT1_STANDARD_A             = 17,
    SENSOR_REFERENCE_ILLUMINANT1_STANDARD_B             = 18,
    SENSOR_REFERENCE_ILLUMINANT1_STANDARD_C             = 19,
    SENSOR_REFERENCE_ILLUMINANT1_D55                    = 20,
    SENSOR_REFERENCE_ILLUMINANT1_D65                    = 21,
    SENSOR_REFERENCE_ILLUMINANT1_D75                    = 22,
    SENSOR_REFERENCE_ILLUMINANT1_D50                    = 23,
    SENSOR_REFERENCE_ILLUMINANT1_ISO_STUDIO_TUNGSTEN    = 24,
} camera_metadata_enum_sensor_reference_illuminant1_t;

/* SENSOR_TEST_PATTERN_MODE */
typedef enum camera_metadata_enum_sensor_test_pattern_mode {
    SENSOR_TEST_PATTERN_MODE_OFF,
    SENSOR_TEST_PATTERN_MODE_SOLID_COLOR,
    SENSOR_TEST_PATTERN_MODE_COLOR_BARS,
    SENSOR_TEST_PATTERN_MODE_COLOR_BARS_FADE_TO_GRAY,
    SENSOR_TEST_PATTERN_MODE_PN9,
    SENSOR_TEST_PATTERN_MODE_CUSTOM1                    = 256,
} camera_metadata_enum_sensor_test_pattern_mode_t;

/* SENSOR_INFO_COLOR_FILTER_ARRANGEMENT */
typedef enum camera_metadata_enum_sensor_info_color_filter_arrangement {
    SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGGB,
    SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GRBG,
    SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GBRG,
    SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_BGGR,
    SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGB,
} camera_metadata_enum_sensor_info_color_filter_arrangement_t;

/* SENSOR_INFO_TIMESTAMP_SOURCE */
typedef enum camera_metadata_enum_sensor_info_timestamp_source {
    SENSOR_INFO_TIMESTAMP_SOURCE_UNKNOWN,
    SENSOR_INFO_TIMESTAMP_SOURCE_REALTIME,
} camera_metadata_enum_sensor_info_timestamp_source_t;

/* SENSOR_INFO_LENS_SHADING_APPLIED */
typedef enum camera_metadata_enum_sensor_info_lens_shading_applied {
    SENSOR_INFO_LENS_SHADING_APPLIED_FALSE,
    SENSOR_INFO_LENS_SHADING_APPLIED_TRUE,
} camera_metadata_enum_sensor_info_lens_shading_applied_t;

/* SHADING_MODE */
typedef enum camera_metadata_enum_shading_mode {
    SHADING_MODE_OFF,
    SHADING_MODE_FAST,
    SHADING_MODE_HIGH_QUALITY,
} camera_metadata_enum_shading_mode_t;

/* STATISTICS_FACE_DETECT_MODE */
typedef enum camera_metadata_enum_statistics_face_detect_mode {
    STATISTICS_FACE_DETECT_MODE_OFF,
    STATISTICS_FACE_DETECT_MODE_SIMPLE,
    STATISTICS_FACE_DETECT_MODE_FULL,
} camera_metadata_enum_statistics_face_detect_mode_t;

/* STATISTICS_HISTOGRAM_MODE */
typedef enum camera_metadata_enum_statistics_histogram_mode {
    STATISTICS_HISTOGRAM_MODE_OFF,
    STATISTICS_HISTOGRAM_MODE_ON,
} camera_metadata_enum_statistics_histogram_mode_t;

/* STATISTICS_SHARPNESS_MAP_MODE */
typedef enum camera_metadata_enum_statistics_sharpness_map_mode {
    STATISTICS_SHARPNESS_MAP_MODE_OFF,
    STATISTICS_SHARPNESS_MAP_MODE_ON,
} camera_metadata_enum_statistics_sharpness_map_mode_t;

/* STATISTICS_HOT_PIXEL_MAP_MODE */
typedef enum camera_metadata_enum_statistics_hot_pixel_map_mode {
    STATISTICS_HOT_PIXEL_MAP_MODE_OFF,
    STATISTICS_HOT_PIXEL_MAP_MODE_ON,
} camera_metadata_enum_statistics_hot_pixel_map_mode_t;

/* STATISTICS_SCENE_FLICKER */
typedef enum camera_metadata_enum_statistics_scene_flicker {
    STATISTICS_SCENE_FLICKER_NONE,
    STATISTICS_SCENE_FLICKER_50HZ,
    STATISTICS_SCENE_FLICKER_60HZ,
} camera_metadata_enum_statistics_scene_flicker_t;

/* STATISTICS_LENS_SHADING_MAP_MODE */
typedef enum camera_metadata_enum_statistics_lens_shading_map_mode {
    STATISTICS_LENS_SHADING_MAP_MODE_OFF,
    STATISTICS_LENS_SHADING_MAP_MODE_ON,
} camera_metadata_enum_statistics_lens_shading_map_mode_t;

/* TONEMAP_MODE */
typedef enum camera_metadata_enum_tonemap_mode {
    TONEMAP_MODE_CONTRAST_CURVE,
    TONEMAP_MODE_FAST,
    TONEMAP_MODE_HIGH_QUALITY,
    TONEMAP_MODE_GAMMA_VALUE,
    TONEMAP_MODE_PRESET_CURVE,
} camera_metadata_enum_tonemap_mode_t;

/* TONEMAP_PRESET_CURVE */
typedef enum camera_metadata_enum_tonemap_preset_curve {
    TONEMAP_PRESET_CURVE_SRGB,
    TONEMAP_PRESET_CURVE_REC709,
} camera_metadata_enum_tonemap_preset_curve_t;

/* LED_TRANSMIT */
typedef enum camera_metadata_enum_led_transmit {
    LED_TRANSMIT_OFF,
    LED_TRANSMIT_ON,
} camera_metadata_enum_led_transmit_t;

/* LED_AVAILABLE_LEDS */
typedef enum camera_metadata_enum_led_available_leds {
    LED_AVAILABLE_LEDS_TRANSMIT,
} camera_metadata_enum_led_available_leds_t;

/* INFO_SUPPORTED_HARDWARE_LEVEL */
typedef enum camera_metadata_enum_info_supported_hardware_level {
    INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED,
    INFO_SUPPORTED_HARDWARE_LEVEL_FULL,
    INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY,
} camera_metadata_enum_info_supported_hardware_level_t;

/* BLACK_LEVEL_LOCK */
typedef enum camera_metadata_enum_black_level_lock {
    BLACK_LEVEL_LOCK_OFF,
    BLACK_LEVEL_LOCK_ON,
} camera_metadata_enum_black_level_lock_t;

/* SYNC_FRAME_NUMBER */
typedef enum camera_metadata_enum_sync_frame_number {
    SYNC_FRAME_NUMBER_CONVERGING                        = -1,
    SYNC_FRAME_NUMBER_UNKNOWN                           = -2,
} camera_metadata_enum_sync_frame_number_t;

/* SYNC_MAX_LATENCY */
typedef enum camera_metadata_enum_sync_max_latency {
    SYNC_MAX_LATENCY_PER_FRAME_CONTROL                  = 0,
    SYNC_MAX_LATENCY_UNKNOWN                            = -1,
} camera_metadata_enum_sync_max_latency_t;

/* DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS */
typedef enum
  camera_metadata_enum_depth_available_depth_stream_configurations {
    DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS_OUTPUT,
    DEPTH_AVAILABLE_DEPTH_STREAM_CONFIGURATIONS_INPUT,
} camera_metadata_enum_depth_available_depth_stream_configurations_t;

/* DEPTH_DEPTH_IS_EXCLUSIVE */
typedef enum camera_metadata_enum_depth_depth_is_exclusive {
    DEPTH_DEPTH_IS_EXCLUSIVE_FALSE,
    DEPTH_DEPTH_IS_EXCLUSIVE_TRUE,
} camera_metadata_enum_depth_depth_is_exclusive_t;

/**
 * @brief Camera sensor Capabilities
 * @param size buffer size
 * @param capabilities buffer address
 * @return zero for success or non-zero on any faillure
 */
int get_capabilities(uint32_t *size, uint8_t *capabilities);

/**
 * @brief Camera sensor capture result metadata
 * @param size buffer size
 * @param capabilities buffer address
 * @return zero for success or non-zero on any faillure
 */
int get_capture_results_metadata(uint32_t *size, uint8_t *capabilities);

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
    Camera_Metadata_type_t keyid, int size, const uint8_t *values);

#endif
