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
#include <string>
#include <utility>

Volume::Volume(const ColorScheme& colorScheme, std::string cardName)
    : Field(colorScheme)
    , cardName(std::move(cardName))
{
}

void Volume::update()
{
    static const std::string fgBadColor { "^fg(" + getColor("fg-bad") + ")" };
    static const std::string fgGoodColor { "^fg(" + getColor("fg-good") + ")" };

    bool enabled;
    int level;
    std::tie(enabled, level) = getVolumeLevel();

    std::ostringstream result;

    result << "A: "
           << (enabled ? fgGoodColor : fgBadColor)
           << ((level < 0) ? "XX" : std::to_string(level)) << "%";

    Field::setText(result.str());
}

std::pair<bool, int> Volume::getVolumeLevel() const
{
    // TODO: don't repeat the whole procedure every time.  Even if we can't
    // keep handles around for a while, at least cache `min` and `max` values.

    static snd_mixer_t *raw_handle;
    if (raw_handle == nullptr) {
        if (snd_mixer_open(&raw_handle, 0) < 0) {
            return {false, -1};
        }

        if (snd_mixer_attach(raw_handle, cardName.c_str()) < 0) {
            return {false, -1};
        }
        if (snd_mixer_selem_register(raw_handle, nullptr, nullptr) < 0) {
            return {false, -1};
        }
    }

    if (snd_mixer_load(raw_handle) < 0) {
        return {false, -1};
    }
    snd_mixer_selem_id_t *sid;
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "Master");

    snd_mixer_elem_t *elem = snd_mixer_find_selem(raw_handle, sid);
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

    snd_mixer_free(raw_handle);

    return {state, (100*(vol - min))/(max - min)};
}
