// Copyright (C) 2025 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#ifndef WHISPER_UTILS_H
#define WHISPER_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "openvino/genai/c/whisper_pipeline.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_PATH_LENGTH 1024

// Error handling macro
#define CHECK_STATUS(return_status)                                                                       \
    if (return_status != OK) {                                                                            \
        const char* error_msg = "Unknown error";                                                          \
        switch (return_status) {                                                                          \
        case INVALID_C_PARAM:                                                                             \
            error_msg = "Invalid parameter";                                                              \
            break;                                                                                        \
        case NOT_FOUND:                                                                                   \
            error_msg = "Not found";                                                                      \
            break;                                                                                        \
        case OUT_OF_BOUNDS:                                                                               \
            error_msg = "Out of bounds";                                                                  \
            break;                                                                                        \
        case UNEXPECTED:                                                                                  \
            error_msg = "Unexpected error";                                                               \
            break;                                                                                        \
        case NOT_IMPLEMENTED:                                                                             \
            error_msg = "Not implemented";                                                                \
            break;                                                                                        \
        case UNKNOW_EXCEPTION:                                                                            \
            error_msg = "Unknown exception";                                                              \
            break;                                                                                        \
        }                                                                                                 \
        fprintf(stderr, "[ERROR] %s (status code: %d) at line %d\n", error_msg, return_status, __LINE__); \
        exit_code = EXIT_FAILURE;                                                                         \
        goto err;                                                                                         \
    }

// Default values
#define DEFAULT_DEVICE      "CPU"
#define DEFAULT_LANGUAGE    ""
#define DEFAULT_TASK        "transcribe"
#define DEFAULT_SAMPLE_RATE 16000.0f
#define DEFAULT_DURATION    2.0f

// Options structure
typedef struct {
    const char* model_path;
    const char* audio_path;
    const char* device;
    const char* language;
    const char* task;
    const char* initial_prompt;
    bool return_timestamps;
    bool use_synthetic_audio;
    float sample_rate;
    float duration;
} Options;

// WAV file header structure
typedef struct {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} WAVHeader;

// Function declarations
void print_usage(const char* program_name);
int parse_arguments(int argc, char* argv[], Options* options);
int load_wav_file(const char* filename, float** audio_data, size_t* audio_length, float* sample_rate);
void generate_synthetic_audio(float* audio, size_t length, float frequency, float sample_rate);
float* resample_audio(const float* input,
                      size_t input_length,
                      float input_rate,
                      float target_rate,
                      size_t* output_length);

#endif  // WHISPER_UTILS_H
