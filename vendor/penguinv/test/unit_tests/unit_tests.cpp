/***************************************************************************
 *   penguinV: https://github.com/ihhub/penguinV                           *
 *   Copyright (C) 2017 - 2022                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "unit_test_blob_detection.h"
#include "unit_test_edge_detection.h"
#include "unit_test_fft.h"
#include "unit_test_file.h"
#include "unit_test_framework.h"
#include "unit_test_helper.h"
#include "unit_test_image_buffer.h"
#include "unit_test_image_function.h"
#include "unit_test_math.h"

int main( int argc, char * argv[] )
{
    Unit_Test::setRunCount( argc, argv, 100 );

    cpu_Memory::MemoryAllocator::instance().reserve( 32 * 1024 * 1024 ); // reserve preallocated memory

    UnitTestFramework framework;
    addTests_Blob_Detection( framework );
    addTests_Edge_Detection( framework );
    addTests_File( framework );
    addTests_FFT( framework );
    addTests_Image_Buffer( framework );
    addTests_Image_Function( framework );
    addTests_Math( framework );

    return framework.run();
}
