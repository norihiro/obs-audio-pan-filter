/*
OBS Audio Pan Filter
Copyright (C) 2020 Norihiro Kamae <norihiro@nagater.net>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#define _USE_MATH_DEFINES
#include <obs-module.h>
#include <media-io/audio-math.h>
#include <math.h>
#include "plugin-macros.generated.h"

struct stereo_pan_data
{
	obs_source_t *context;
	size_t channels;
	float gain[2][2];
};

static const char *sp_name(void *type_data)
{
	UNUSED_PARAMETER(type_data);
	return obs_module_text("Stereo Pan");
}

static void sp_destroy(void *data)
{
	struct stereo_pan_data *sp = data;
	bfree(sp);
}

static void sp_update(void *data, obs_data_t *s)
{
	struct stereo_pan_data *sp = data;

	sp->channels = audio_output_get_channels(obs_get_audio());

	double gainL = db_to_mul(obs_data_get_double(s, "gainL")) * M_SQRT1_2;
	double gainR = db_to_mul(obs_data_get_double(s, "gainR")) * M_SQRT1_2;
	double panL = obs_data_get_double(s, "panL") * (M_PI_4/100.0);
	double panR = obs_data_get_double(s, "panR") * (M_PI_4/100.0);

	// left inputs
	sp->gain[0][0] = gainL * (cos(panL) - sin(panL));
	sp->gain[1][0] = gainL * (cos(panL) + sin(panL));

	// right inputs
	sp->gain[0][1] = gainR * (cos(panR) - sin(panR));
	sp->gain[1][1] = gainR * (cos(panR) + sin(panR));
}

static void *sp_create(obs_data_t *settings, obs_source_t *filter)
{
	struct stereo_pan_data *sp = bzalloc(sizeof(*sp));
	sp->context = filter;
	sp_update(sp, settings);
	return sp;
}

static struct obs_audio_data *sp_filter_audio(void *data, struct obs_audio_data *audio)
{
	struct stereo_pan_data *sp = data;
	const size_t channels = sp->channels;
	float **adata = (float **)audio->data;
	const float gL2L = sp->gain[0][0];
	const float gL2R = sp->gain[1][0];
	const float gR2L = sp->gain[0][1];
	const float gR2R = sp->gain[1][1];

	for (size_t i = 0; i < audio->frames; i++) {
		const register float iL = channels>0 ? adata[0][i] : 0.0f;
		const register float iR = channels>1 ? adata[1][i] : 0.0f;
		if (channels > 0) adata[0][i] = gL2L * iL + gR2L * iR;
		if (channels > 1) adata[1][i] = gL2R * iL + gR2R * iR;
	}

	return audio;
}

static void sp_defaults(obs_data_t *s)
{
	obs_data_set_default_double(s, "gainL", 0.0);
	obs_data_set_default_double(s, "gainR", 0.0);
	obs_data_set_default_double(s, "panL", -100.0);
	obs_data_set_default_double(s, "panR", +100.0);
}

static obs_properties_t *sp_properties(void *data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t *prop = obs_properties_create();
	obs_property_t *p;

	p = obs_properties_add_float_slider(prop, "gainL", obs_module_text("Gain (Left)"), -80.0, +30.0, 1.0);
	obs_property_float_set_suffix(p, " dB");

	p = obs_properties_add_float_slider(prop, "panL", obs_module_text("Pan (Left)"), -100.0, +100.0, 1.0);

	p = obs_properties_add_float_slider(prop, "gainR", obs_module_text("Gain (Right)"), -80.0, +30.0, 1.0);
	obs_property_float_set_suffix(p, " dB");

	p = obs_properties_add_float_slider(prop, "panR", obs_module_text("Pan (Right)"), -100.0, +100.0, 1.0);

	return prop;
}

struct obs_source_info stereo_pan_filter = {
	.id = "stereo_pan_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_AUDIO,
	.get_name = sp_name,
	.create = sp_create,
	.destroy = sp_destroy,
	.update = sp_update,
	.filter_audio = sp_filter_audio,
	.get_defaults = sp_defaults,
	.get_properties = sp_properties,
};
