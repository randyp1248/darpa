#!/usr/bin/perl

####################################################################################
####################################################################################
#####                          Walsh Generation {                              #####
####################################################################################
####################################################################################

$spreadingBits = shift;
$spreadingSymbols = 1<<($spreadingBits-2);  # -1 for not bit, -1 for qpsk
$spreadingRows = 1<<($spreadingBits-1); # -1 for not bit
$minBytes = &lcm($spreadingBits);

if ($spreadingBits < 1)
{
   &usage;
   exit;
}

# Generate walshMatrix that eventually will be 2^spreadingBits x 2^spreadingBits.
# For example, if spreadingBits is 6, then this will be a 64x64 matrix.

# Start with a 2^0 x 2^0 = 1x1 matrix containing only one row of one column: 0
$walshMatrix = "0";
for($i=1; $i<= $spreadingBits-1; ++$i)
{
   @rows = split(';', $walshMatrix);
   $topHalf = "";
   $bottomHalf = "";
   foreach $row (@rows)
   {
      # Determine what the inversion of that row would be.
      $invertRow = $row;
      $invertRow =~ s/0/2/g;
      $invertRow =~ s/1/0/g;
      $invertRow =~ s/2/1/g;

      # Create the top half of new matrix
      $topHalf .= $row . "," . $row . ";";

      # Create bottom half of new matrix
      $bottomHalf .= $row . "," . $invertRow . ";";

      # Concatenate the top and bottom and overwrite the matrix
      $walshMatrix = $topHalf . $bottomHalf;

      # Remove the last simicolon
      chop $walshMatrix;
   }

   #print "Walsh matrix for $i bits:\n";
   #$out = $walshMatrix;
   #$out =~ s/,/ /g;
   #$out =~ s/;/\n/g;
   #print "$out\n";
}

for($row=0; $row<$spreadingRows; ++$row)
{
   $walshRow =    "   {";
   $negWalshRow = "   {";
   for($column=0; $column<$spreadingRows; $column+=2)
   {
      $i=&index($row, $column);
      $q=&index($row, $column+1);
      if ($i > 0) { $i = "+0.7071067811865475"; } else { $i = "-0.7071067811865475"; }
      if ($q > 0) { $q = "+0.7071067811865475"; } else { $q = "-0.7071067811865475"; }
      if ($i > 0) { $negi = "-0.7071067811865475"; } else { $negi = "+0.7071067811865475"; }
      if ($q > 0) { $negq = "-0.7071067811865475"; } else { $negq = "+0.7071067811865475"; }
      $walshRow .= "gr_complex($i,$q)";
      $negWalshRow .= "gr_complex($negi,$negq)";
      if ($column < $spreadingRows-2)
      {
         $walshRow .=",";
	 $negWalshRow .=",";
      }
   }
   $walshTable .= $walshRow . "}";
   $negWalshTable .= $negWalshRow . "}";
   if ($row < $spreadingRows-1)
   {
      $walshTable .= ",\n";
      $negWalshTable .= ",\n";
   }
}


# Loop over the input bits, extracting spreadingBits for the output each time.
$spreaderCode = "";
$despreaderCode = "";
$inputBitPos = 0;
$spreaderOutputBufferIndex = 0;
while ($inputBitPos < 8*$minBytes)
{
   # Extract spreadingBits from input as an index for the output.
   $inputBytePos = int($inputBitPos/8);
   $inputBitPosWithinByte = $inputBitPos % 8;

   $spreaderCode .= <<END;
   index = (((
         (((unsigned)in[$inputBytePos])<<24) | 
         (((unsigned)in[$inputBytePos+1])<<16) | 
         (((unsigned)in[$inputBytePos+2])<<8) | 
         (((unsigned)in[$inputBytePos+3]))
      ) >> (32-$inputBitPosWithinByte-$spreadingBits)) & ((1<<$spreadingBits)-1));
   sign = index & 0x01;
   index >>= 1;
//printf("=======================================\\n");
//printf("Spreading index: %d  sign: %d\\n", index, sign);
   memcpy(
      out+$spreaderOutputBufferIndex*$spreadingSymbols, 
      (sign ? (&negWalshTable[index][0]) : (&walshTable[index][0])) , 
      $spreadingSymbols*sizeof(gr_complex));
END

   $despreaderCode .= <<END;
   maxCorrelationValue = 0.0;
END

   # TODO: This can be made more efficient by using the structure of the walsh code
   # walshIndex iterates over the hypotheses for walsh codes that might have been sent.
   # chipIndex iterates over the walsh symbols actually received.
   for($walshIndex=0; $walshIndex<$spreadingRows; ++$walshIndex)
   {
      $despreaderCode .= <<END;
   currentCorrelationValue =
END
      for($chipIndex=0; $chipIndex<$spreadingSymbols; ++$chipIndex)
      {
         if    ( (&index($walshIndex, $chipIndex*2  ) == +1) &&
                 (&index($walshIndex, $chipIndex*2+1) == +1))
         {
            $despreaderCode .= <<END;
      + in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].real() + in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].imag()
END
         }
         elsif ( (&index($walshIndex, $chipIndex*2  ) == +1) &&
                 (&index($walshIndex, $chipIndex*2+1) == -1))
         {
            $despreaderCode .= <<END;
      + in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].real() - in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].imag()
END
         }
         elsif ( (&index($walshIndex, $chipIndex*2  ) == -1) &&
                 (&index($walshIndex, $chipIndex*2+1) == +1))
         {
            $despreaderCode .= <<END;
      - in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].real() + in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].imag()
END
         }
         elsif ( (&index($walshIndex, $chipIndex*2  ) == -1) &&
                 (&index($walshIndex, $chipIndex*2+1) == -1))
         {
            $despreaderCode .= <<END;
      - in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].real() - in[$chipIndex+$spreaderOutputBufferIndex*$spreadingSymbols].imag()
END
         }
         else
         {
            print "ERROR in walsh table.  Not one of the four possibilities.\n";
            exit 1;
         }
      }
      chop $despreaderCode;
      $despreaderCode .= ";\n";
      $despreaderCode .= <<END;
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = $walshIndex;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 0 : 1);
   }
//printf("corr=%f\\n", currentCorrelationValue);
END
   }

   $despreaderCode .= <<END;
//printf("max corr=%f\\n", maxCorrelationValue);
//printf("=======================================\\n");
//printf("Despreading index: %d  sign: %d\\n", maxCorrelationIndex, maxCorrelationSign);
   temp = (((unsigned)out[$inputBytePos])<<24) | 
          (((unsigned)out[$inputBytePos+1])<<16) | 
          (((unsigned)out[$inputBytePos+2])<<8) | 
          (((unsigned)out[$inputBytePos+3]));
   temp &= ~(((1<<$spreadingBits)-1) << (32-$inputBitPosWithinByte-$spreadingBits));
   temp |= ((maxCorrelationIndex<<1)|maxCorrelationSign) << (32-$inputBitPosWithinByte-$spreadingBits);
   out[$inputBytePos]   = (temp>>24) & 0xff;
   out[$inputBytePos+1] = (temp>>16) & 0xff;
   out[$inputBytePos+2] = (temp>>8 ) & 0xff;
   out[$inputBytePos+3] = (temp    ) & 0xff;


END


   $inputBitPos += $spreadingBits;
   ++$spreaderOutputBufferIndex;
}

sub index
{
   local $rowIndex = shift;
   local $columnIndex = shift;

   local @rows = split(';', $walshMatrix);
   local $row = $rows[$rowIndex];
   local @columns = split(',', $row);
   return (($columns[$columnIndex] == 0) ? "-1" : "+1");
}

# compute lcm(x, 8bits), in bytes.  For example, lcm(3 bits) = 3 bytes
# lcm(2 bits) = 1 byte
# In other words, lcm(x) finds the number of bytes that has an integer
# number of x bits in it.
sub lcm
{
   $bits = shift;

   for($bytes=1; $bytes<$bits*8; ++$bytes)
   {
      if ((($bytes*8) % $bits) == 0)
      {
         return $bytes;
      }
   }
   return $bits;
}

open(SPREAD_HEADER, '>spreader_bc_impl.h');
open(SPREAD_CODE, '>spreader_bc_impl.cc');
open(DESPREAD_HEADER, '>despreader_cb_impl.h');
open(DESPREAD_CODE, '>despreader_cb_impl.cc');
open(SPREAD_TESTCODE, '>../python/qa_spreader_bc.py');
open(DESPREAD_TESTCODE, '>../python/qa_despreader_cb.py');

####################################################################################
####################################################################################
#####                    } Spreader Header Generation {                        #####
####################################################################################
####################################################################################
print SPREAD_HEADER <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 * 
 * <<<====================================================================>>>
 * <<<====================================================================>>>
 * <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
 * <<<====================================================================>>>
 * <<<====================================================================>>>
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

#ifndef INCLUDED_SPREADER_SPREADER_BC_IMPL_H
#define INCLUDED_SPREADER_SPREADER_BC_IMPL_H

#include <spreader/spreader_bc.h>

namespace gr {
namespace spreader {

class spreader_bc_impl : public spreader_bc
{
private:
   static gr_complex walshTable[$spreadingRows][$spreadingSymbols];
   static gr_complex negWalshTable[$spreadingRows][$spreadingSymbols];

public:
   spreader_bc_impl();
   ~spreader_bc_impl();

   void forecast (int noutput_items, gr_vector_int &ninput_items_required);

   int general_work(int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items);
};

} // namespace spreader
} // namespace gr

#endif /* INCLUDED_SPREADER_SPREADER_BC_IMPL_H */

END

####################################################################################
####################################################################################
#####                     } Spreader Code Generation {                         #####
####################################################################################
####################################################################################
print SPREAD_CODE <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Spread input bits with an indexed walsh code and map to constellation.
 *
 * <<<====================================================================>>>
 * <<<====================================================================>>>
 * <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
 * <<<====================================================================>>>
 * <<<====================================================================>>>
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
#include "spreader_bc_impl.h"
#include <stdio.h>

namespace gr {
namespace spreader {

static const int MIN_IN = 1;
static const int MAX_IN = 1;
static const int MIN_OUT = 1;
static const int MAX_OUT = 1;

gr_complex spreader_bc_impl::walshTable[$spreadingRows][$spreadingSymbols] = 
{
$walshTable
};

gr_complex spreader_bc_impl::negWalshTable[$spreadingRows][$spreadingSymbols] = 
{
$negWalshTable
};

spreader_bc::sptr
spreader_bc::make()
{
  return gnuradio::get_initial_sptr (new spreader_bc_impl());
}

spreader_bc_impl::spreader_bc_impl()
   : gr_block("spreader_bc",
                  gr_make_io_signature(MIN_IN, MAX_IN, sizeof(unsigned char)),
                  gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof(gr_complex)))
{
   set_min_noutput_items($minBytes * 8 / $spreadingBits * $spreadingSymbols);
   set_max_noutput_items($minBytes * 8 / $spreadingBits * $spreadingSymbols);
}

spreader_bc_impl::~spreader_bc_impl()
{
}

void
spreader_bc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = $minBytes;
}

int
spreader_bc_impl::general_work (int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
   const unsigned char* in = (unsigned char*) input_items[0];
   gr_complex* out = (gr_complex*) output_items[0];

   unsigned index;
   unsigned sign;

   if ((noutput_items < ($minBytes * 8 / $spreadingBits * $spreadingSymbols)) ||
       (ninput_items[0] < $minBytes))
   {
      return 0;
   }

$spreaderCode

   consume_each ($minBytes);

   return ($minBytes * 8 / $spreadingBits * $spreadingSymbols);
}

} /* namespace spreader */
} /* namespace gr */

END

####################################################################################
####################################################################################
#####                   } Despreader Header Generation {                       #####
####################################################################################
####################################################################################
print DESPREAD_HEADER <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * <<<====================================================================>>>
 * <<<====================================================================>>>
 * <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
 * <<<====================================================================>>>
 * <<<====================================================================>>>
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

#ifndef INCLUDED_SPREADER_DESPREADER_CB_IMPL_H
#define INCLUDED_SPREADER_DESPREADER_CB_IMPL_H

#include <spreader/despreader_cb.h>

namespace gr {
namespace spreader {

class despreader_cb_impl : public despreader_cb
{
private:

public:
   despreader_cb_impl();
   ~despreader_cb_impl();

   void forecast (int noutput_items, gr_vector_int &ninput_items_required);

   // Where all the action really happens
   int general_work(int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items);
};

} // namespace spreader
} // namespace gr

#endif /* INCLUDED_SPREADER_DESPREADER_CB_IMPL_H */

END

####################################################################################
####################################################################################
#####                    } Despreader Code Generation {                        #####
####################################################################################
####################################################################################
print DESPREAD_CODE <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Despread input constellation stream and correlate with each row of the
 * walsh table to determine the index bits to output.
 *
 * <<<====================================================================>>>
 * <<<====================================================================>>>
 * <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
 * <<<====================================================================>>>
 * <<<====================================================================>>>
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
#include "despreader_cb_impl.h"
#include <stdio.h>

namespace gr {
namespace spreader {

static const int MIN_IN = 1;
static const int MAX_IN = 1;
static const int MIN_OUT = 1;
static const int MAX_OUT = 1;

despreader_cb::sptr
despreader_cb::make()
{
   return gnuradio::get_initial_sptr (new despreader_cb_impl());
}

despreader_cb_impl::despreader_cb_impl()
   : gr_block("despreader_cb",
                  gr_make_io_signature(MIN_IN, MAX_IN, sizeof (gr_complex)),
                  gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof (unsigned char)))
{
   set_min_noutput_items($minBytes);
   set_max_noutput_items($minBytes);
}

despreader_cb_impl::~despreader_cb_impl()
{
}

void
despreader_cb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = ($minBytes * 8 / $spreadingBits * $spreadingSymbols);
}

int
despreader_cb_impl::general_work (int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
   const gr_complex* in = (gr_complex*) input_items[0];
   unsigned char* out = (unsigned char*) output_items[0];
   float currentCorrelationValue;
   float maxCorrelationValue;
   unsigned maxCorrelationIndex;
   unsigned maxCorrelationSign;
   unsigned temp;
   
$despreaderCode

   consume_each($minBytes * 8 / $spreadingBits * $spreadingSymbols);

   return ($minBytes);
}

} /* namespace spreader */
} /* namespace gr */

END

####################################################################################
####################################################################################
#####                     } Spreader Test Generation {                         #####
####################################################################################
####################################################################################
print SPREAD_TESTCODE <<END;
#!/usr/bin/env python
# 
# Copyright 2013 TRITONS
#
# <<<====================================================================>>>
# <<<====================================================================>>>
# <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
# <<<====================================================================>>>
# <<<====================================================================>>>
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
import spreader_swig as spreader

class qa_spreader_bc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_spreader_bc, "qa_spreader_bc.xml")
END

####################################################################################
####################################################################################
#####                    } Despreader Test Generation {                        #####
####################################################################################
####################################################################################
print DESPREAD_TESTCODE <<END;
#!/usr/bin/env python
# 
# Copyright 2013 TRITONS
#
# <<<====================================================================>>>
# <<<====================================================================>>>
# <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
# <<<====================================================================>>>
# <<<====================================================================>>>
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
import spreader_swig as spreader
import random

class qa_despreader_cb (gr_unittest.TestCase):

    randomBytes = []

    def setUp (self):
        self.tb = gr.top_block ()
        random.seed(None)
        self.randomBytes[:] = []
        for x in range(0, $minBytes*3):
            self.randomBytes.append(random.randint(0,255));

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src_data = self.randomBytes
        expected_data = self.randomBytes
        source = gr.vector_source_b(src_data)
        spread = spreader.spreader_bc()
        despread = spreader.despreader_cb()
        sink = gr.vector_sink_b()
        self.tb.connect(source, spread)
        self.tb.connect(spread, despread)
        self.tb.connect(despread, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\\n"
        #print expected_data
        #print "Results\\n"
        #print result_data
        self.assertEqual(tuple(expected_data), result_data)


if __name__ == '__main__':
    gr_unittest.run(qa_despreader_cb, "qa_despreader_cb.xml")
END

close(DESPREAD_TESTCODE);
close(SPREAD_TESTCODE);
close(DESPREAD_CODE);
close(DESPREAD_HEADER);
close(SPREAD_CODE);
close(SPREAD_HEADER);

####################################################################################
####################################################################################
#####                                } Usage                                   #####
####################################################################################
####################################################################################
sub usage
{
   print <<END;
Usage: ./walshgen.pl <spreadingBitWidth>
The spreadingBitWidth determines the number of data capsule bits that are
spread by indexing the walsh table and transmitting 2^spreadingBitWidth-1
symbols.  The minus one is due to mapping two walsh symbols into a single
QPSK symbol.
END
}
