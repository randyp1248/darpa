#ifndef TRITONS_INCL
#define TRITONS_INCL

// TODO: this will need to be set by command line parameter
#define CENTER_FREQUENCY 1500000000

#define OVERSAMPLE_FACTOR 2

// N bits are transmitted that encode the K information bits
// If concatenated codes are used, then multiply both code Ns together here.
// TODO: set the code rate N and K once the coding scheme is design
#define CODE_RATE_N 1

// K information bits
// If concatenated codes are used, then multiply both code Ks together here.
#define CODE_RATE_K 1

// Number log2 of the alphabet size, or the code bit to symbol ratio
#define MODULATION_ORDER_M 2

// Number of constellation points
#define MODULATION_ALPHABET_SIZE (1<<MODULATION_ORDER_M)

// Sample rate is the rate at which IQ samples sent to the radio over TCP are
// sent over the air.
// DARPA rules require the sample rate to be less than or equal to 5Msps
#define SAMPLE_RATE (SYMBOL_RATE * OVERSAMPLE_FACTOR)

// Symbol rate is the rate at which the signal changes, which determines the bandwidth
#define SYMBOL_RATE (CODE_BIT_RATE / MODULATION_ORDER_M)

// Code bit rate is the bit rate out of the encoder
#define CODE_BIT_RATE (INFORMATION_BIT_RATE * CODE_RATE_N / CODE_RATE_K)

// The information bit rate is the rate at which bits are fed into the encoder.
// It is not exactly the true information bit rate because some of those bits
// will be CRC and any other overhead bits.  Also, some time will be spent
// DTX or sending preamble.
// TODO: Tune this number so that the SAMPLE_RATE is close to 5Msps.  Low number for testing purposes.
#define INFORMATION_BIT_RATE 1000

// TODO: Compute FFT size
#define FFT_SIZE

// Length of preamble PN Sequence in symbols.
// TODO: Tune this to be robust against interference.  Initial number for testing purposes.
#define PREAMBLE_SYMBOL_LENGTH 1023

// Length of preamble PN Sequence in samples.
#define PREAMBLE_SAMPLE_LENGTH (PREAMBLE_SYMBOL_LENGTH * OVERSAMPLE_FACTOR)

// Frequency error is stated within 2.5 ppm for the USRP without GPS frequency reference.
// Error between two radios can be as high as twice this, or 5ppm.
#define FREQUENCY_ERROR_PPM 5

// Length of data capsule in symbols.  This is limited by the sampling clock skew as well as LO frequency skew.
// Length of capsule is limited by the phase error due to frequency error.
// The numerator is symbols/s, and the denominator is error cycles/s, which comes to symbols/error cycle.  All of this is
// multiplied by the fractional allowed cycle (20 degrees).
//#define CAPSULE_SYMBOL_LENGTH (SYMBOL_RATE / (CENTER_FREQUENCY * FREQUENCY_ERROR_PPM/1000000) * 20/360)
//#define CAPSULE_SYMBOL_LENGTH 4096
#define CAPSULE_SYMBOL_LENGTH 16320

// Length of data capsule in samples.
#define CAPSULE_SAMPLE_LENGTH (CAPSULE_SYMBOL_LENGTH * OVERSAMPLE_FACTOR)

// Length of Ack or Nak PN Sequence in symbols.
#define ARQ_SYMBOL_LENGTH (PREAMBLE_SYMBOL_LENGTH)

// Length of Ack or Nak PN Sequence in samples.
#define ARQ_SAMPLE_LENGTH (ARQ_SYMBOL_LENGTH * OVERSAMPLE_FACTOR)

#endif //TRITONS_INCL
