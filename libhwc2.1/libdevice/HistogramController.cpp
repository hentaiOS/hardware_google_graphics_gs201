/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HistogramController.h"

void HistogramController::initPlatformHistogramCapability() {
    mHistogramCapability.supportSamplePosList.push_back(HistogramSamplePos::PRE_POSTPROC);
}

// TODO: b/295990513 - Remove the if defined after kernel prebuilts are merged.
#if defined(EXYNOS_HISTOGRAM_CHANNEL_REQUEST)
int HistogramController::createHistogramDrmConfigLocked(const ChannelInfo& channel,
                                                        std::shared_ptr<void>& configPtr,
                                                        size_t& length) const {
    configPtr = std::make_shared<struct histogram_channel_config>();
    struct histogram_channel_config* channelConfig =
            (struct histogram_channel_config*)configPtr.get();

    if (channelConfig == nullptr) {
        ALOGE("%s: histogram failed to allocate histogram_channel_config", __func__);
        return NO_MEMORY;
    }

    channelConfig->roi.start_x = channel.workingConfig.roi.left;
    channelConfig->roi.start_y = channel.workingConfig.roi.top;
    channelConfig->roi.hsize = channel.workingConfig.roi.right - channel.workingConfig.roi.left;
    channelConfig->roi.vsize = channel.workingConfig.roi.bottom - channel.workingConfig.roi.top;
    channelConfig->weights.weight_r = channel.workingConfig.weights.weightR;
    channelConfig->weights.weight_g = channel.workingConfig.weights.weightG;
    channelConfig->weights.weight_b = channel.workingConfig.weights.weightB;
    channelConfig->pos = (channel.workingConfig.samplePos == HistogramSamplePos::POST_POSTPROC)
            ? POST_DQE
            : PRE_DQE;
    channelConfig->threshold = channel.threshold;

    length = sizeof(struct histogram_channel_config);

    return NO_ERROR;
}

int HistogramController::parseDrmEvent(void* event, uint8_t& channelId, char16_t*& buffer) const {
    struct exynos_drm_histogram_channel_event* histogram_channel_event =
            (struct exynos_drm_histogram_channel_event*)event;
    channelId = histogram_channel_event->hist_id;
    buffer = (char16_t*)&histogram_channel_event->bins;
    return NO_ERROR;
}
#endif
