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

#pragma once

#include "HistogramDevice.h"

class HistogramController : public HistogramDevice {
public:
    HistogramController(ExynosDisplay* display) : HistogramDevice(display, 1, {}) {}
    virtual void initPlatformHistogramCapability() override;
// TODO: b/295990513 - Remove the if defined after kernel prebuilts are merged.
#if defined(EXYNOS_HISTOGRAM_CHANNEL_REQUEST)
    virtual int createHistogramDrmConfigLocked(const ChannelInfo& channel,
                                               std::shared_ptr<void>& configPtr,
                                               size_t& length) const override
            REQUIRES(channel.channelInfoMutex);
    virtual int parseDrmEvent(void* event, uint8_t& channelId, char16_t*& buffer) const override;
#endif
};
