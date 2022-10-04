#if !defined(SAMPLER_H_)
#define SAMPLER_H_

void init_adc();

void start_sampling(int frequency);

void sample();

void stop_sampling();

#endif // SAMPLER_H_