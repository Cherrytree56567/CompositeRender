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

#include "jpeg_image.h"
#include "../parameter_validation.h"
#include "../penguinv_exception.h"

// Include stb_image for loading and stb_image_write for saving
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace
{
    int jpegQuality = 100;
}

namespace Jpeg_Operation
{
    penguinV::Image Load( const std::string & path )
    {
        penguinV::Image image;

        Load( path, image );
        return image;
    }

    void Save( const std::string & path, const penguinV::Image & image )
    {
        Save( path, image, 0, 0, image.width(), image.height() );
    }

    void SetImageQuality( int quality )
    {
        if ( quality < 1 || quality > 100 )
            throw penguinVException( "JPEG quality value must be between 1 and 100" );

        jpegQuality = quality;
    }

    int GetImageQuality()
    {
        return jpegQuality;
    }
}

#ifndef PENGUINV_ENABLED_JPEG_SUPPORT

namespace Jpeg_Operation
{
    void Load( const std::string &, penguinV::Image & )
    {
        throw penguinVException( "JPEG is not supported" );
    }

    void Save( const std::string &, const penguinV::Image &, uint32_t, uint32_t, uint32_t, uint32_t )
    {
        throw penguinVException( "JPEG is not supported" );
    }
}

#else

namespace Jpeg_Operation
{
    void Load( const std::string & path, penguinV::Image & image )
    {
        if ( path.empty() )
            throw penguinVException( "Incorrect parameters for JPEG image loading" );

        int width, height, channels;
        // Use stb_image to load the image
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
            throw penguinVException( "Cannot open JPEG image with stb_image" );

        // Set the color count based on channels
        image.setColorCount(channels);
        image.resize(width, height);

        // Copy the loaded data into the image object
        std::memcpy(image.data(), data, width * height * channels);

        stbi_image_free(data); // Free the raw image data
    }

    void Save( const std::string & path, const penguinV::Image & image, uint32_t startX, uint32_t startY, uint32_t width, uint32_t height )
    {
        Image_Function::ValidateImageParameters( image, startX, startY, width, height );

        // Use stb_image_write to save the image
        if (!stbi_write_jpg(path.c_str(), width, height, image.colorCount(), image.data() + image.rowSize() * startY, jpegQuality))
        {
            throw penguinVException( "Failed to save image using stb_image" );
        }
    }
}
#endif
