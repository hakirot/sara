#include <stdlib.h>
#include <string.h>
#include "viz.h"

int funky (void){
  struct audio_data audio;
  audio.cava_in = (double *)malloc(audio.cava_buffer_size * sizeof(double));
  audio.source = malloc(1 + strlen(p.audio_source));
  memset(audio.cava_in, 0, sizeof(double) * audio.cava_buffer_size);
}
