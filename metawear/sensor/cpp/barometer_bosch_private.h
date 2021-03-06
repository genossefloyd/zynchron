#pragma once

#include <vector>

#include "core/metawearboard_fwd.h"

void init_barometer_module(MblMwMetaWearBoard *board);
void serialize_barometer_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_barometer_config(MblMwMetaWearBoard *board, uint8_t** state_stream);
