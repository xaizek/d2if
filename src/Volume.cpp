// d2if
// Copyright (C) 2012 xaizek.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include "Volume.hpp"

#include <alsa/asoundlib.h>

#include <memory>
#include <sstream>

void Volume::update()
{
    const std::string BAR = "#A6F09D";

    const std::pair<bool, int> state = getVolumeLevel();

    std::stringstream result;

    result << "^fg(white)A: "
           << "^fg(" << (state.first ? BAR : "black") << ")"
           << ((state.second < 0) ? "XX" : std::to_string(state.second)) << "%";

    Field::setText(result.str());
}

std::pair<bool, int> Volume::getVolumeLevel() const
{
    static const auto mixerDeleter = [] (snd_mixer_t *mixer) {
        snd_mixer_close(mixer);
    };

    snd_mixer_t *raw_handle;
    if (snd_mixer_open(&raw_handle, 0) < 0) {
        return {false, -1};
    }
    std::unique_ptr<snd_mixer_t, decltype(mixerDeleter)> handle {
        raw_handle, mixerDeleter
    };
    raw_handle = nullptr;

    if (snd_mixer_attach(handle.get(), "default") < 0) {
        return {false, -1};
    }
    if (snd_mixer_selem_register(handle.get(), nullptr, nullptr) < 0) {
        return {false, -1};
    }
    if (snd_mixer_load(handle.get()) < 0) {
        return {false, -1};
    }

    snd_mixer_selem_id_t *sid;
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "Master");

    snd_mixer_elem_t *elem = snd_mixer_find_selem(handle.get(), sid);
    if (elem == nullptr) {
        return {false, -1};
    }

    int state;
    if (snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT,
                                            &state) < 0) {
        return {false, -1};
    }

    long vol;
    if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT,
                                            &vol) < 0) {
        return {false, -1};
    }

    long min, max;
    if (snd_mixer_selem_get_playback_volume_range(elem, &min, &max) < 0) {
        return {false, -1};
    }

    return {state, (100*(vol - min))/(max - min)};
}

// vim: set filetype=cpp.cpp11 :
