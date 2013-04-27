#!/usr/bin/perl


#initialize the taps
$numStages= 0;
for($tap=0; $tap<100; ++$tap)
{
   $taps[$tap] = 0;
}

#read the taps from the command line
while ($tap = shift)
{
   $taps[$tap] = 1;
   if ($tap > $numStages)
   {
      $numStages = $tap;
   }
}
$codeLength = (1<<$numStages)-1;

if ($numStages == 0)
{
   &usage;
   exit(-1);
}

#initialize the stage delays to all ones
for($stageNum=1; $stageNum<=$numStages; ++$stageNum)
{
   $stage[$stageNum] = 1;
}

$tapString = "";
for($tap=$numStages; $tap>=1; --$tap)
{
   if ($taps[$tap] == 1) {$tapString .= "$tap ";};
}

$pnSequence = "";
$correlateRealCode = "";
$correlateImagCode = "";
#iternate over the pn sequence
for($iter=0; $iter<$codeLength; ++$iter)
{
   if ($stage[$numStages])
   {
      $output = "+1";
      $newline = <<END;
      _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
      _accImag[index] += imag;
END
      $correlate = $newline . $correlate;
   }
   else
   {
      $output = "-1";
      $newline = <<END;
      _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
      _accImag[index] -= imag;
END
      $correlate = $newline . $correlate;
   }
   $newshift = 0;
   for($stageNum=$numStages; $stageNum>1; --$stageNum)
   {
      $newshift ^= ($taps[$stageNum] * $stage[$stageNum]);
      $stage[$stageNum] = $stage[$stageNum-1];
   }
   $stage[1] = $newshift;
   if ($iter%16 == 0) {$pnSequence .= "   ";};
   $pnSequence .= $output;
   if ($iter < $codeLength-1) {$pnSequence .= ", ";};
   if ($iter%16 == 15) {$pnSequence .= "\n";};
}


####################################################################################
####################################################################################
#####                           Header Generation                              #####
####################################################################################
####################################################################################

open(HEADER, '>correlator_cc.h');
open(CODE, '>correlator_cc.cc');

print HEADER <<END;
/* -*- c++ -*- */
/*
 * Convolve the input against a PN sequence to find the frame boundary.
 */
#ifndef CORRELATOR_H
#define CORRELATOR_H

#include <gr_block.h>

#define CODE_LENGTH ($codeLength)
#define ACCUMULATOR_LENGTH (2048)
#define ACCUMULATOR_LENGTH_MASK (0x7ff)

class correlator_cc;

typedef boost::shared_ptr<correlator_cc> correlator_cc_sptr;

/*!
 * \\brief Return a shared_ptr to a new instance of correlator_cc.
 */
correlator_cc_sptr make_correlator_cc();

/*!
 * \\brief correlate a stream of complex against a preamble, and pass on the data.
 */
class correlator_cc : public gr_block
{
private:
   // The friend declaration allows howto_make_square_ff to
   // access the private constructor.
   friend correlator_cc_sptr make_correlator_cc();

   /*!
    * \\brief correlate a stream of complex
    */
   correlator_cc();  // private constructor

   void detect_peak(long real, long imag);

public:
   ~correlator_cc(); // public destructor

   // Where all the action really happens
   int general_work (int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);

private:

   const int _sequence[CODE_LENGTH];

   long _accReal[ACCUMULATOR_LENGTH];
   long _accImag[ACCUMULATOR_LENGTH];
   int _accIndex;  // Indexes the accumulator that is receiving its last contribution
   unsigned long _sampleNum;
};

#endif /* CORRELATOR_H */
END

####################################################################################
####################################################################################
#####                            Code Generation                               #####
####################################################################################
####################################################################################

print CODE <<END;


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <correlator_cc.h>
#include <gr_io_signature.h>
#include <complex>
#include <math.h>

//Taps: [ $tapString]
const int correlator_cc::_sequence[$codeLength] = {
$pnSequence
};
const int correlator_cc::CODE_LENGTH = $codeLength;

/*
 * Create a new instance of correlator_cc and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
make_correlator_cc_sptr
make_correlator_cc()
{
   return gnuradio::get_initial_sptr(new correlator_cc());
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr_block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
static const int MIN_OUT = 1  // minimum number of output streams
static const int MAX_OUT = 1; // maximum number of output streams

/*
 * The private constructor
 */
correlator_cc::correlator_cc()
   : gr_block (
        "correlator_cc",
        gr_make_io_signature (MIN_IN, MAX_IN, sizeof (float)),
        gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (float))
     )
{
   int i;
   for (i=0; i<ACCUMULATOR_LENGTH; ++i)
   {
      _accReal[i] = 0;
      _accImag[i] = 0;
      _accIndex = 0;
   }
   _sampleNum = 0;
}

/*
 * Virtual destructor.
 */
correlator_cc::~correlator_cc()
{
}

int
correlator_cc::general_work (
   int noutput_items,
   gr_vector_int &ninput_items,
   gr_vector_const_void_star &input_items,
   gr_vector_void_star &output_items)
{
   const std::complex<int> *in = (const complex *) input_items[0];
   complex *out = (std::complex<int> *) output_items[0];

   for(i=0; i<noutput_items; ++i)
   {
      int index = (_accIndex + ACCUMULATOR_LENGTH - CODE_LENGTH - 1) 
                  & ACCUMULATOR_LENGTH_MASK;
      long real = in[i].real();
      long imag = in[i].imag();
{
   int i;
   for (i=0; i<ACCUMULATOR_LENGTH; ++i)
   {
      _accReal[i] = 0;
      _accImag[i] = 0;
      _accIndex = 0;
   }
}

/*
 * Virtual destructor.
 */
correlator_cc::~correlator_cc()
{
}

void
correlator_cc::detect_peak(long real, long imag)
{
   int index = _accIndex;
   double mag = sqrt(real*real + imag*imag);
   double scale = 65536 / mag;  // Normalize magnitude to 2^16

   // Normalization prevents weighting based on power.  While this may normally be desirable,
   // here it is not because it may weight the transmitter feedback incorrectly.
   // The result is more of a phase correlator.

   double mag = sqrt(real*real + imag*imag);
   double scale = 65536 / mag;  // Normalize magnitude to 2^16
   real = (long)(real*scale);
   imag = (long)(imag*scale);

$correlate

   ++_sampleNum;

   // Threshold the correlation
   double accReal = (double)_accReal[_accIndex];
   double accImag = (double)_accImag[_accIndex];
   if (sqrt(accReal*accReal + accImag*accImag) > 65536.0/2*CODE_LENGTH)
   {
      printf("Peak on sample %ld\\n", _sampleNum);
   }

   ++_accIndex;

}

int
correlator_cc::general_work (
   int noutput_items,
   gr_vector_int &ninput_items,
   gr_vector_const_void_star &input_items,
   gr_vector_void_star &output_items)
{
   const std::complex<int> *in = (const complex *) input_items[0];
   complex *out = (std::complex<int> *) output_items[0];

   for(i=0; i<noutput_items; ++i)
   {
      detect_peak(in[i].real(), in[i].imag());
   }

   // Tell runtime system how many input items we consumed on each input stream.
   consume_each (noutput_items);

   // Tell runtime system how many output items we produced.
   return noutput_items;
}

END

close(CODE);
close(HEADER);


sub usage
{
   print <<END;
Usage: ./pngen.pl <tap>...
The number of taps must be non-zero.  
Number of stages is derived from the largest tap.

Maximal length taps are:
=========================
[ 2 1 ]
[ 3 2 ] [ 3 1 ]
[ 4 3 ] [ 4 1 ]
[ 5 3 ] [ 5 2 ]
[ 6 5 ] [ 6 1 ]
[ 7 6 ] [ 7 3 ] [ 7 1 ]
[ 8 6 5 4 ] [ 8 6 5 3 ]
[ 9 5 ] [ 9 6 4 3 ]
[ 10 7 ] [ 10 3 ]
[ 11 9 ] [ 11 8 5 2 ]
[ 12 6 4 1 ]
[ 13 4 3 1 ]
[ 14 5 3 1 ]
[ 15 14 ] [ 15 4 ]
[ 16 15 13 4 ]
[ 17 14 ] [ 17 3 ]
[ 18 11 ] [ 18 7 ]
[ 19 6 2 1 ]
[ 20 17 ] [ 20 3 ]
[ 21 19 ] [ 21 2 ]
[ 22 21 ] [ 22 1 ]
[ 23 18 ] [ 23 5 ]
[ 24 23 22 17 ]
[ 25 22 ] [ 25 3 ]
=========================
END
}
