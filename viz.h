
struct audio_data {
  double *cava_in;
  
  int input_buffer_size;
  int cava_buffer_size;
  
  int format;
  unsigned int rate;
  unsigned int channels;
  int threadparams; // shared variable used to prevent main thread from cava_init
                    // threads have finalized parameters (0=allow cava_init, 1=dis
  char *source;     // alsa device, fifo path or pulse source
  int im;           // input mode alsa, fifo, pulse, portaudio, shmem or sndio
  int terminate;    // shared variable used to terminate audio thread
  char error_message[1024];
  int samples_counter;
  int IEEE_FLOAT;   // format for 32bit (0=int, 1=float)
  int autoconnect;  // auto connect to audio source (0=off, 1=once at startup, 2=r
  int active;       // actively monitor sources when the graph is idle
  int remix;        // remix the incoming stream to this many channels
  int virtual_node; // set node.virtual to avoid recording notifications
  pthread_mutex_t lock;
};
