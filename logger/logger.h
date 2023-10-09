#ifndef LOGGER_H
#define LOGGER_H

#include <toolbox/stream/file_stream.h>

#include "../bme280/bme280.h"


Stream* logger_stream_alloc(Storage* storage);
void logger_stream_free(Stream* stream);

void logger_stream_append(Stream* file_stream, Bme280Data* bme_data);



#endif