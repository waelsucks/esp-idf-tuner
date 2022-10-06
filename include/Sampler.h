#if !defined(SAMPLER_H_)
#define SAMPLER_H_

#define ADC_OFFSET  1350
#define SAMPLE_SIZE 64

void init_adc();

void start_sampling(int frequency);

void sample();

void stop_sampling();

void reset_sampling();

void flush_buffers();


// ,,,

float get_duration();
int get_x();

#endif // SAMPLER_H_