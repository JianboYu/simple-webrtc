# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
AUDIO_CODEC_TOP_PATH := $(call my-dir)

include $(call all-makefiles-under, \
					$(AUDIO_CODEC_TOP_PATH)/main/acm2/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/main/tests/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/neteq/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/cng/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/g722/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/g711/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/ilbc/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/isac/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/opus/Android.mk \
					$(AUDIO_CODEC_TOP_PATH)/codecs/pcm16b/Android.mk \
)
