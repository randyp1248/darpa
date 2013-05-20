/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Convolve the input against a PN sequence to find the frame boundary.
 * 
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <complex>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "correlator_cc_impl.h"
#include "TRITONS.h"

#if (255 != PREAMBLE_SYMBOL_LENGTH)
//#error Mismatch preamble length (255 != PREAMBLE_SYMBOL_LENGTH)
#endif

namespace gr {
namespace correlator_cc {

static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
static const int MIN_OUT = 1; // minimum number of output streams
static const int MAX_OUT = 1; // maximum number of output streams

correlator_cc::sptr
correlator_cc::make()
{
   return gnuradio::get_initial_sptr (new correlator_cc_impl());
}

/*
 * The private constructor
 */
correlator_cc_impl::correlator_cc_impl()
   : gr_block(
        "correlator_cc",
        gr_make_io_signature(MIN_IN, MAX_IN, sizeof(gr_complex)),
        gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof(gr_complex)))
	
{
   for (int i=0; i<ACCUMULATOR_LENGTH; ++i)
   {
      _accRealOdd[i] = 0;
      _accImagOdd[i] = 0;

      _accRealEven[i] = 0;
      _accImagEven[i] = 0;
   }
   _accIndexOdd = 0;
   _accIndexEven = 0;

   _sampleNum = 0;
   _capsuleLen = 0;

   _oddSample = 0;
   _oddData = 0;

   _correlationMagnitude = 0.0;

   _primed = 0;
   _movingSum = 0.0;
   _movingSumIndex = 0;
   for (int i=0; i<2*CODE_LENGTH; ++i)
      _movingSumAddends[i] = 0.0;
}

/*
 * Our virtual destructor.
 */
correlator_cc_impl::~correlator_cc_impl()
{
}

void
correlator_cc_impl::detect_peak(sampleType real, sampleType imag)
{
   static sampleType lastRealOdd = 0;
   static sampleType lastImagOdd = 0;
   static sampleType lastRealEven = 0;
   static sampleType lastImagEven = 0;

   sampleType& lastReal = (_oddSample ? lastRealOdd : lastRealEven);
   sampleType& lastImag = (_oddSample ? lastImagOdd : lastImagEven);

   int& accIndex = (_oddSample ? _accIndexOdd : _accIndexEven);

   sampleType* accRealArr = (_oddSample ? _accRealOdd : _accRealEven);
   sampleType* accImagArr = (_oddSample ? _accImagOdd : _accImagEven);

   sampleType* realPtr = accRealArr + accIndex;
   sampleType* imagPtr = accImagArr + accIndex;

   sampleType a = lastReal;
   sampleType b = lastImag;
   sampleType e = real;
   sampleType f = imag;
   lastReal = real;
   lastImag = imag;

   real = (a*e + f*b) / (a*a + b*b);
   imag = (a*f - e*b) / (a*a + b*b);

   // Zero out the accumulator that is getting its first sample
   accRealArr[accIndex+CODE_LENGTH] = 0;
   accImagArr[accIndex+CODE_LENGTH] = 0;

   *(realPtr + 0) += real;
   *(realPtr + 1) += imag;
   *(realPtr + 2) -= imag;
   *(realPtr + 3) -= imag;
   *(realPtr + 4) += imag;
   *(realPtr + 5) += real;
   *(realPtr + 6) += imag;
   *(realPtr + 7) -= imag;
   *(realPtr + 8) -= real;
   *(realPtr + 9) -= real;
   *(realPtr + 10) += real;
   *(realPtr + 11) -= real;
   *(realPtr + 12) += real;
   *(realPtr + 13) += real;
   *(realPtr + 14) -= imag;
   *(realPtr + 15) += imag;
   *(realPtr + 16) += imag;
   *(realPtr + 17) -= real;
   *(realPtr + 18) -= real;
   *(realPtr + 19) -= real;
   *(realPtr + 20) -= imag;
   *(realPtr + 21) += imag;
   *(realPtr + 22) -= real;
   *(realPtr + 23) -= imag;
   *(realPtr + 24) += real;
   *(realPtr + 25) += real;
   *(realPtr + 26) -= real;
   *(realPtr + 27) += real;
   *(realPtr + 28) += real;
   *(realPtr + 29) -= imag;
   *(realPtr + 30) += real;
   *(realPtr + 31) -= real;
   *(realPtr + 32) += imag;
   *(realPtr + 33) -= real;
   *(realPtr + 34) += real;
   *(realPtr + 35) -= imag;
   *(realPtr + 36) -= real;
   *(realPtr + 37) -= imag;
   *(realPtr + 38) += imag;
   *(realPtr + 39) -= imag;
   *(realPtr + 40) -= real;
   *(realPtr + 41) += real;
   *(realPtr + 42) -= real;
   *(realPtr + 43) += real;
   *(realPtr + 44) += imag;
   *(realPtr + 45) += imag;
   *(realPtr + 46) -= real;
   *(realPtr + 47) -= imag;
   *(realPtr + 48) -= imag;
   *(realPtr + 49) -= real;
   *(realPtr + 50) += imag;
   *(realPtr + 51) += imag;
   *(realPtr + 52) -= imag;
   *(realPtr + 53) += real;
   *(realPtr + 54) += real;
   *(realPtr + 55) += imag;
   *(realPtr + 56) -= real;
   *(realPtr + 57) -= real;
   *(realPtr + 58) += real;
   *(realPtr + 59) -= imag;
   *(realPtr + 60) += real;
   *(realPtr + 61) -= imag;
   *(realPtr + 62) -= imag;
   *(realPtr + 63) -= real;
   *(realPtr + 64) += imag;
   *(realPtr + 65) += imag;
   *(realPtr + 66) -= real;
   *(realPtr + 67) += real;
   *(realPtr + 68) += imag;
   *(realPtr + 69) -= real;
   *(realPtr + 70) += imag;
   *(realPtr + 71) += imag;
   *(realPtr + 72) += real;
   *(realPtr + 73) -= real;
   *(realPtr + 74) -= imag;
   *(realPtr + 75) -= imag;
   *(realPtr + 76) += imag;
   *(realPtr + 77) -= real;
   *(realPtr + 78) -= imag;
   *(realPtr + 79) += imag;
   *(realPtr + 80) -= real;
   *(realPtr + 81) += imag;
   *(realPtr + 82) -= real;
   *(realPtr + 83) += imag;
   *(realPtr + 84) -= imag;
   *(realPtr + 85) += imag;
   *(realPtr + 86) += real;
   *(realPtr + 87) += imag;
   *(realPtr + 88) -= real;
   *(realPtr + 89) += imag;
   *(realPtr + 90) += real;
   *(realPtr + 91) -= real;
   *(realPtr + 92) -= imag;
   *(realPtr + 93) += imag;
   *(realPtr + 94) += real;
   *(realPtr + 95) -= imag;
   *(realPtr + 96) += imag;
   *(realPtr + 97) -= imag;
   *(realPtr + 98) -= imag;
   *(realPtr + 99) += real;
   *(realPtr + 100) -= real;
   *(realPtr + 101) += imag;
   *(realPtr + 102) += imag;
   *(realPtr + 103) += imag;
   *(realPtr + 104) += imag;
   *(realPtr + 105) += imag;
   *(realPtr + 106) += real;
   *(realPtr + 107) += imag;
   *(realPtr + 108) += real;
   *(realPtr + 109) -= imag;
   *(realPtr + 110) -= imag;
   *(realPtr + 111) += real;
   *(realPtr + 112) += real;
   *(realPtr + 113) -= real;
   *(realPtr + 114) += imag;
   *(realPtr + 115) += real;
   *(realPtr + 116) += imag;
   *(realPtr + 117) += real;
   *(realPtr + 118) -= real;
   *(realPtr + 119) -= real;
   *(realPtr + 120) += real;
   *(realPtr + 121) -= imag;
   *(realPtr + 122) -= imag;
   *(realPtr + 123) += imag;
   *(realPtr + 124) -= imag;
   *(realPtr + 125) -= real;
   *(realPtr + 126) -= imag;
   *(realPtr + 127) += real;
   *(realPtr + 128) += real;
   *(realPtr + 129) += imag;
   *(realPtr + 130) -= imag;
   *(realPtr + 131) += imag;
   *(realPtr + 132) += imag;
   *(realPtr + 133) += real;
   *(realPtr + 134) -= real;
   *(realPtr + 135) -= real;
   *(realPtr + 136) += imag;
   *(realPtr + 137) -= real;
   *(realPtr + 138) += real;
   *(realPtr + 139) += real;
   *(realPtr + 140) += imag;
   *(realPtr + 141) += imag;
   *(realPtr + 142) -= imag;
   *(realPtr + 143) += real;
   *(realPtr + 144) += imag;
   *(realPtr + 145) -= real;
   *(realPtr + 146) += real;
   *(realPtr + 147) += real;
   *(realPtr + 148) += imag;
   *(realPtr + 149) += real;
   *(realPtr + 150) -= real;
   *(realPtr + 151) -= imag;
   *(realPtr + 152) += imag;
   *(realPtr + 153) += real;
   *(realPtr + 154) -= real;
   *(realPtr + 155) -= imag;
   *(realPtr + 156) -= real;
   *(realPtr + 157) -= real;
   *(realPtr + 158) -= imag;
   *(realPtr + 159) -= imag;
   *(realPtr + 160) -= imag;
   *(realPtr + 161) += real;
   *(realPtr + 162) -= real;
   *(realPtr + 163) += imag;
   *(realPtr + 164) -= imag;
   *(realPtr + 165) -= real;
   *(realPtr + 166) -= imag;
   *(realPtr + 167) -= real;
   *(realPtr + 168) -= imag;
   *(realPtr + 169) += real;
   *(realPtr + 170) += imag;
   *(realPtr + 171) -= imag;
   *(realPtr + 172) -= real;
   *(realPtr + 173) += imag;
   *(realPtr + 174) -= real;
   *(realPtr + 175) += real;
   *(realPtr + 176) += imag;
   *(realPtr + 177) += real;
   *(realPtr + 178) -= imag;
   *(realPtr + 179) -= real;
   *(realPtr + 180) -= imag;
   *(realPtr + 181) -= imag;
   *(realPtr + 182) -= imag;
   *(realPtr + 183) -= imag;
   *(realPtr + 184) += imag;
   *(realPtr + 185) += imag;
   *(realPtr + 186) -= real;
   *(realPtr + 187) += imag;
   *(realPtr + 188) += imag;
   *(realPtr + 189) += imag;
   *(realPtr + 190) -= real;
   *(realPtr + 191) += imag;
   *(realPtr + 192) += imag;
   *(realPtr + 193) += imag;
   *(realPtr + 194) -= imag;
   *(realPtr + 195) -= imag;
   *(realPtr + 196) -= real;
   *(realPtr + 197) += real;
   *(realPtr + 198) -= real;
   *(realPtr + 199) += real;
   *(realPtr + 200) += real;
   *(realPtr + 201) += imag;
   *(realPtr + 202) -= imag;
   *(realPtr + 203) += real;
   *(realPtr + 204) -= real;
   *(realPtr + 205) += imag;
   *(realPtr + 206) -= imag;
   *(realPtr + 207) -= real;
   *(realPtr + 208) -= real;
   *(realPtr + 209) -= real;
   *(realPtr + 210) += real;
   *(realPtr + 211) -= imag;
   *(realPtr + 212) -= real;
   *(realPtr + 213) += real;
   *(realPtr + 214) += real;
   *(realPtr + 215) -= real;
   *(realPtr + 216) -= imag;
   *(realPtr + 217) += real;
   *(realPtr + 218) -= imag;
   *(realPtr + 219) += real;
   *(realPtr + 220) -= imag;
   *(realPtr + 221) -= real;
   *(realPtr + 222) -= imag;
   *(realPtr + 223) += real;
   *(realPtr + 224) += real;
   *(realPtr + 225) += real;
   *(realPtr + 226) -= imag;
   *(realPtr + 227) += imag;
   *(realPtr + 228) += real;
   *(realPtr + 229) -= real;
   *(realPtr + 230) -= real;
   *(realPtr + 231) += imag;
   *(realPtr + 232) += imag;
   *(realPtr + 233) += imag;
   *(realPtr + 234) += imag;
   *(realPtr + 235) -= imag;
   *(realPtr + 236) -= imag;
   *(realPtr + 237) -= imag;
   *(realPtr + 238) += imag;
   *(realPtr + 239) += real;
   *(realPtr + 240) -= real;
   *(realPtr + 241) += imag;
   *(realPtr + 242) -= real;
   *(realPtr + 243) += imag;
   *(realPtr + 244) += real;
   *(realPtr + 245) += real;
   *(realPtr + 246) += imag;
   *(realPtr + 247) += real;
   *(realPtr + 248) += real;
   *(realPtr + 249) += real;
   *(realPtr + 250) += real;
   *(realPtr + 251) += real;
   *(realPtr + 252) += real;
   *(realPtr + 253) += real;

   *(imagPtr + 0) += imag;
   *(imagPtr + 1) -= real;
   *(imagPtr + 2) += real;
   *(imagPtr + 3) += real;
   *(imagPtr + 4) -= real;
   *(imagPtr + 5) += imag;
   *(imagPtr + 6) -= real;
   *(imagPtr + 7) += real;
   *(imagPtr + 8) -= imag;
   *(imagPtr + 9) -= imag;
   *(imagPtr + 10) += imag;
   *(imagPtr + 11) -= imag;
   *(imagPtr + 12) += imag;
   *(imagPtr + 13) += imag;
   *(imagPtr + 14) += real;
   *(imagPtr + 15) -= real;
   *(imagPtr + 16) -= real;
   *(imagPtr + 17) -= imag;
   *(imagPtr + 18) -= imag;
   *(imagPtr + 19) -= imag;
   *(imagPtr + 20) += real;
   *(imagPtr + 21) -= real;
   *(imagPtr + 22) -= imag;
   *(imagPtr + 23) += real;
   *(imagPtr + 24) += imag;
   *(imagPtr + 25) += imag;
   *(imagPtr + 26) -= imag;
   *(imagPtr + 27) += imag;
   *(imagPtr + 28) += imag;
   *(imagPtr + 29) += real;
   *(imagPtr + 30) += imag;
   *(imagPtr + 31) -= imag;
   *(imagPtr + 32) -= real;
   *(imagPtr + 33) -= imag;
   *(imagPtr + 34) += imag;
   *(imagPtr + 35) += real;
   *(imagPtr + 36) -= imag;
   *(imagPtr + 37) += real;
   *(imagPtr + 38) -= real;
   *(imagPtr + 39) += real;
   *(imagPtr + 40) -= imag;
   *(imagPtr + 41) += imag;
   *(imagPtr + 42) -= imag;
   *(imagPtr + 43) += imag;
   *(imagPtr + 44) -= real;
   *(imagPtr + 45) -= real;
   *(imagPtr + 46) -= imag;
   *(imagPtr + 47) += real;
   *(imagPtr + 48) += real;
   *(imagPtr + 49) -= imag;
   *(imagPtr + 50) -= real;
   *(imagPtr + 51) -= real;
   *(imagPtr + 52) += real;
   *(imagPtr + 53) += imag;
   *(imagPtr + 54) += imag;
   *(imagPtr + 55) -= real;
   *(imagPtr + 56) -= imag;
   *(imagPtr + 57) -= imag;
   *(imagPtr + 58) += imag;
   *(imagPtr + 59) += real;
   *(imagPtr + 60) += imag;
   *(imagPtr + 61) += real;
   *(imagPtr + 62) += real;
   *(imagPtr + 63) -= imag;
   *(imagPtr + 64) -= real;
   *(imagPtr + 65) -= real;
   *(imagPtr + 66) -= imag;
   *(imagPtr + 67) += imag;
   *(imagPtr + 68) -= real;
   *(imagPtr + 69) -= imag;
   *(imagPtr + 70) -= real;
   *(imagPtr + 71) -= real;
   *(imagPtr + 72) += imag;
   *(imagPtr + 73) -= imag;
   *(imagPtr + 74) += real;
   *(imagPtr + 75) += real;
   *(imagPtr + 76) -= real;
   *(imagPtr + 77) -= imag;
   *(imagPtr + 78) += real;
   *(imagPtr + 79) -= real;
   *(imagPtr + 80) -= imag;
   *(imagPtr + 81) -= real;
   *(imagPtr + 82) -= imag;
   *(imagPtr + 83) -= real;
   *(imagPtr + 84) += real;
   *(imagPtr + 85) -= real;
   *(imagPtr + 86) += imag;
   *(imagPtr + 87) -= real;
   *(imagPtr + 88) -= imag;
   *(imagPtr + 89) -= real;
   *(imagPtr + 90) += imag;
   *(imagPtr + 91) -= imag;
   *(imagPtr + 92) += real;
   *(imagPtr + 93) -= real;
   *(imagPtr + 94) += imag;
   *(imagPtr + 95) += real;
   *(imagPtr + 96) -= real;
   *(imagPtr + 97) += real;
   *(imagPtr + 98) += real;
   *(imagPtr + 99) += imag;
   *(imagPtr + 100) -= imag;
   *(imagPtr + 101) -= real;
   *(imagPtr + 102) -= real;
   *(imagPtr + 103) -= real;
   *(imagPtr + 104) -= real;
   *(imagPtr + 105) -= real;
   *(imagPtr + 106) += imag;
   *(imagPtr + 107) -= real;
   *(imagPtr + 108) += imag;
   *(imagPtr + 109) += real;
   *(imagPtr + 110) += real;
   *(imagPtr + 111) += imag;
   *(imagPtr + 112) += imag;
   *(imagPtr + 113) -= imag;
   *(imagPtr + 114) -= real;
   *(imagPtr + 115) += imag;
   *(imagPtr + 116) -= real;
   *(imagPtr + 117) += imag;
   *(imagPtr + 118) -= imag;
   *(imagPtr + 119) -= imag;
   *(imagPtr + 120) += imag;
   *(imagPtr + 121) += real;
   *(imagPtr + 122) += real;
   *(imagPtr + 123) -= real;
   *(imagPtr + 124) += real;
   *(imagPtr + 125) -= imag;
   *(imagPtr + 126) += real;
   *(imagPtr + 127) += imag;
   *(imagPtr + 128) += imag;
   *(imagPtr + 129) -= real;
   *(imagPtr + 130) += real;
   *(imagPtr + 131) -= real;
   *(imagPtr + 132) -= real;
   *(imagPtr + 133) += imag;
   *(imagPtr + 134) -= imag;
   *(imagPtr + 135) -= imag;
   *(imagPtr + 136) -= real;
   *(imagPtr + 137) -= imag;
   *(imagPtr + 138) += imag;
   *(imagPtr + 139) += imag;
   *(imagPtr + 140) -= real;
   *(imagPtr + 141) -= real;
   *(imagPtr + 142) += real;
   *(imagPtr + 143) += imag;
   *(imagPtr + 144) -= real;
   *(imagPtr + 145) -= imag;
   *(imagPtr + 146) += imag;
   *(imagPtr + 147) += imag;
   *(imagPtr + 148) -= real;
   *(imagPtr + 149) += imag;
   *(imagPtr + 150) -= imag;
   *(imagPtr + 151) += real;
   *(imagPtr + 152) -= real;
   *(imagPtr + 153) += imag;
   *(imagPtr + 154) -= imag;
   *(imagPtr + 155) += real;
   *(imagPtr + 156) -= imag;
   *(imagPtr + 157) -= imag;
   *(imagPtr + 158) += real;
   *(imagPtr + 159) += real;
   *(imagPtr + 160) += real;
   *(imagPtr + 161) += imag;
   *(imagPtr + 162) -= imag;
   *(imagPtr + 163) -= real;
   *(imagPtr + 164) += real;
   *(imagPtr + 165) -= imag;
   *(imagPtr + 166) += real;
   *(imagPtr + 167) -= imag;
   *(imagPtr + 168) += real;
   *(imagPtr + 169) += imag;
   *(imagPtr + 170) -= real;
   *(imagPtr + 171) += real;
   *(imagPtr + 172) -= imag;
   *(imagPtr + 173) -= real;
   *(imagPtr + 174) -= imag;
   *(imagPtr + 175) += imag;
   *(imagPtr + 176) -= real;
   *(imagPtr + 177) += imag;
   *(imagPtr + 178) += real;
   *(imagPtr + 179) -= imag;
   *(imagPtr + 180) += real;
   *(imagPtr + 181) += real;
   *(imagPtr + 182) += real;
   *(imagPtr + 183) += real;
   *(imagPtr + 184) -= real;
   *(imagPtr + 185) -= real;
   *(imagPtr + 186) -= imag;
   *(imagPtr + 187) -= real;
   *(imagPtr + 188) -= real;
   *(imagPtr + 189) -= real;
   *(imagPtr + 190) -= imag;
   *(imagPtr + 191) -= real;
   *(imagPtr + 192) -= real;
   *(imagPtr + 193) -= real;
   *(imagPtr + 194) += real;
   *(imagPtr + 195) += real;
   *(imagPtr + 196) -= imag;
   *(imagPtr + 197) += imag;
   *(imagPtr + 198) -= imag;
   *(imagPtr + 199) += imag;
   *(imagPtr + 200) += imag;
   *(imagPtr + 201) -= real;
   *(imagPtr + 202) += real;
   *(imagPtr + 203) += imag;
   *(imagPtr + 204) -= imag;
   *(imagPtr + 205) -= real;
   *(imagPtr + 206) += real;
   *(imagPtr + 207) -= imag;
   *(imagPtr + 208) -= imag;
   *(imagPtr + 209) -= imag;
   *(imagPtr + 210) += imag;
   *(imagPtr + 211) += real;
   *(imagPtr + 212) -= imag;
   *(imagPtr + 213) += imag;
   *(imagPtr + 214) += imag;
   *(imagPtr + 215) -= imag;
   *(imagPtr + 216) += real;
   *(imagPtr + 217) += imag;
   *(imagPtr + 218) += real;
   *(imagPtr + 219) += imag;
   *(imagPtr + 220) += real;
   *(imagPtr + 221) -= imag;
   *(imagPtr + 222) += real;
   *(imagPtr + 223) += imag;
   *(imagPtr + 224) += imag;
   *(imagPtr + 225) += imag;
   *(imagPtr + 226) += real;
   *(imagPtr + 227) -= real;
   *(imagPtr + 228) += imag;
   *(imagPtr + 229) -= imag;
   *(imagPtr + 230) -= imag;
   *(imagPtr + 231) -= real;
   *(imagPtr + 232) -= real;
   *(imagPtr + 233) -= real;
   *(imagPtr + 234) -= real;
   *(imagPtr + 235) += real;
   *(imagPtr + 236) += real;
   *(imagPtr + 237) += real;
   *(imagPtr + 238) -= real;
   *(imagPtr + 239) += imag;
   *(imagPtr + 240) -= imag;
   *(imagPtr + 241) -= real;
   *(imagPtr + 242) -= imag;
   *(imagPtr + 243) -= real;
   *(imagPtr + 244) += imag;
   *(imagPtr + 245) += imag;
   *(imagPtr + 246) -= real;
   *(imagPtr + 247) += imag;
   *(imagPtr + 248) += imag;
   *(imagPtr + 249) += imag;
   *(imagPtr + 250) += imag;
   *(imagPtr + 251) += imag;
   *(imagPtr + 252) += imag;
   *(imagPtr + 253) += imag;


   ++_sampleNum;

   // Threshold the correlation
   double accReal = (double)accRealArr[accIndex];
   double accImag = (double)accImagArr[accIndex];
   double mag = sqrt(accReal*accReal + accImag*accImag);
   if (_correlationMagnitude > 0.0)
   {
      // Previous sample was peak.  Check if this one is larger.
      _oddData = _oddSample ^ ((mag > _correlationMagnitude) ? 0 : 1);
      // Start passing capsule data out of this block.
      _capsuleLen = CAPSULE_SYMBOL_LENGTH;
      // Reset the correlation magnitude to start looking for next peak.
      _correlationMagnitude = 0.0;
   }
   else if (_primed && (mag > 4*_movingSum/CODE_LENGTH)) // 8 times the average
   {
      printf("Peak on sample %ld\n", _sampleNum);
      _correlationMagnitude = mag;
   }

   // Update the moving sum of magnitudes for threshold comparisons
   _movingSum -= _movingSumAddends[_movingSumIndex];
   _movingSumAddends[_movingSumIndex] = mag;
   _movingSum += mag;
   _movingSumIndex = (_movingSumIndex+1)%(2*CODE_LENGTH);
   if (!_movingSumIndex)
   {
      _primed = 1;
   }

   ++accIndex;

   // Prevent need for modulo arithematic for circular buffer.  Instead copy back to start of array.
   if (accIndex + CODE_LENGTH == ACCUMULATOR_LENGTH)
   {
      for(int i=0; i<CODE_LENGTH; ++i)
      {
         accRealArr[i] = accRealArr[accIndex+i];
         accImagArr[i] = accImagArr[accIndex+i];
      }
      accIndex = 0;
   }
}


void
correlator_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = noutput_items;
}

int
correlator_cc_impl::general_work (
   int noutput_items,
   gr_vector_int &ninput_items,
   gr_vector_const_void_star &input_items,
   gr_vector_void_star &output_items
)
{
   const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
   gr_complex *out = reinterpret_cast<gr_complex*>(output_items[0]);
   int samplesRemaining = ninput_items[0];
   int samplesOutput = 0;
   int samplesRead = 0;

   while (samplesRemaining)
   {
      while (samplesRemaining && _capsuleLen)
      {
         // Peak has been detected, output this sample (only correct clock)

	 if (_oddSample == _oddData)
	 {
	    out[samplesOutput++] = in[samplesRead];
	    --_capsuleLen;
	 }
	 ++samplesRead;
	 --samplesRemaining;
	 _oddSample ^= 1;
      }

      while (samplesRemaining && !_capsuleLen)
      {
	 detect_peak(in[samplesRead].real(), in[samplesRead].imag());
	 ++samplesRead;
	 --samplesRemaining;
	 _oddSample ^= 1;
      }
   }

   // Tell runtime system how many input items we consumed on each input stream.
   consume_each (samplesRead);

   // Tell runtime system how many output items we produced.
   return samplesOutput;
}

} /* namespace correlator_cc */
} /* namespace gr */
