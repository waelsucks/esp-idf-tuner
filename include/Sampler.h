#if !defined(SAMPLER_H_)
#define SAMPLER_H_

#define ADC_OFFSET  1530
#define SAMPLE_SIZE 64

void init_adc();

void start_sampling(int frequency);

void sample();

void stop_sampling();

void reset_sampling();

void flush_buffers();

int get_adc_reading();

float process_frequency();


#endif // SAMPLER_H_