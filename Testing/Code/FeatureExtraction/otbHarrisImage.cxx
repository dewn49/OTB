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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbHarrisImageFilter.h"

int otbHarrisImage( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
	double SigmaD((double)::atof(argv[3]));
	double SigmaI((double)::atof(argv[4]));
	double Alpha((double)::atof(argv[5]));
	        
        typedef unsigned char                                   InputPixelType;
        const   unsigned int        	                        Dimension = 2;
        typedef unsigned char   	                        OutputPixelType;
	
        typedef itk::Image< InputPixelType,  Dimension >	              InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >	              OutputImageType;
        typedef otb::ImageFileReader< InputImageType  >                       ReaderType;  
	typedef otb::HarrisImageFilter<InputImageType,OutputImageType>        FunctionType;
        typedef otb::ImageFileWriter< OutputImageType >                       WriterType;
  

        ReaderType::Pointer reader   = ReaderType::New();
        WriterType::Pointer writer   = WriterType::New();
	FunctionType::Pointer harris = FunctionType::New();
	
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
 	
	harris->SetInput( reader->GetOutput() );
        harris->SetSigmaD( SigmaD );
        harris->SetSigmaI( SigmaI );
	harris->SetAlpha( Alpha );

        writer->SetInput( harris->GetOutput() );
        writer->Update(); 
		
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

