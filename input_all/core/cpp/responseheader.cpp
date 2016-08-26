#include "responseheader.h"

const uint8_t READ_MASK= 0x80, SILENT_MASK = 0x40, NO_DATA_ID = 0xff;
#define COMPARE(param) (param > other.param ? 1 : (param == other.param ? 0 : -1))

ResponseHeader::ResponseHeader(uint8_t** state_stream) {
    module_id = **state_stream;
    register_id = *(++(*state_stream));
    data_id = *(++(*state_stream));
    (*state_stream)++;
}

ResponseHeader::ResponseHeader(uint8_t module_id, uint8_t register_id) : 
        ResponseHeader(module_id, register_id, NO_DATA_ID) { }

ResponseHeader::ResponseHeader(uint8_t module_id, uint8_t register_id, uint8_t data_id) : 
        module_id(module_id), register_id(register_id), data_id(data_id) { }

ResponseHeader::ResponseHeader(const ResponseHeader &original) :
        module_id(original.module_id), register_id(original.register_id), data_id(original.data_id) { }

void ResponseHeader::disable_silent() {
    register_id &= ~SILENT_MASK;
}

void ResponseHeader::enable_silent() {
    register_id |= SILENT_MASK;
}

bool ResponseHeader::is_readable() const {
    return (READ_MASK & register_id) == READ_MASK;
}

bool ResponseHeader::has_data_id() const {
    return data_id == NO_DATA_ID;
}

bool ResponseHeader::operator < (const ResponseHeader& other) const {
    return COMPARE(module_id) * 4 + COMPARE(register_id) * 2 + COMPARE(data_id) < 0;
}

bool ResponseHeader::operator ==(const ResponseHeader& other) const {
    return module_id == other.module_id && register_id == other.register_id && data_id == other.data_id;
}

void ResponseHeader::serialize(std::vector<uint8_t>& state) const {
    state.push_back(module_id);
    state.push_back(register_id);
    state.push_back(data_id);
}

namespace std {

size_t hash<ResponseHeader>::operator()(const ResponseHeader& key) const {
    ///< Generated by IntelliJ
    size_t result = (size_t) key.module_id;
    result = 31 * result + (size_t) key.register_id;
    result = 31 * result + (size_t) key.data_id;
    return result;
}

}