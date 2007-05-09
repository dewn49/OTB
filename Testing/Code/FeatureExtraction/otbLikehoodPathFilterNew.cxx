/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "otbLikehoodPathFilter.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbImage.h"

int otbLikehoodPathFilterNew(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension =2;
      typedef double PixelType;
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::PolyLineParametricPathWithValue<double,Dimension> PathType;
      typedef otb::LikehoodPathFilter<PathType,ImageType> LinkPathFilterType;
      
      // Instantiating object
      LinkPathFilterType::Pointer object = LinkPathFilterType::New();
    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
