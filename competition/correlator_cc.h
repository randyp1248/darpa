/* -*- c++ -*- */
/*
 * Convolve the input against a PN sequence to find the frame boundary.
 */
#ifndef CORRELATOR_H
#define CORRELATOR_H

#include <gr_block.h>

#define CODE_LENGTH (1023)
#define ACCUMULATOR_LENGTH (2048)
#define ACCUMULATOR_LENGTH_MASK (0x7ff)

class correlator_cc;

typedef boost::shared_ptr<correlator_cc> correlator_cc_sptr;

/*!
 * \brief Return a shared_ptr to a new instance of correlator_cc.
 */
correlator_cc_sptr make_correlator_cc();

/*!
 * \brief correlate a stream of complex against a preamble, and pass on the data.
 */
class correlator_cc : public gr_block
{
private:
   // The friend declaration allows howto_make_square_ff to
   // access the private constructor.
   friend correlator_cc_sptr make_correlator_cc();

   /*!
    * \brief correlate a stream of complex
    */
   correlator_cc();  // private constructor

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
