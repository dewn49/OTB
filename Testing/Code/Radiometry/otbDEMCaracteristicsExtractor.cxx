/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodOperatorImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007/08/10 14:34:02 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "otbDEMCaracteristicsExtractor.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"


int otbDEMCaracteristicsExtractor(int argc, char * argv[])
{
  try
    {
      const char * inputFileName  = argv[1];
      const char * outputSlotFileName = argv[6];
      const char * outputAspectFileName = argv[7];
      const char * outputIncidenceFileName = argv[8];
      const char * outputExitanceFileName = argv[9];

      
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::Image<PixelType, Dimension> InputImageType;

      typedef otb::DEMCaracteristicsExtractor< InputImageType, InputImageType > DEMCaracteristicsExtractorType;
      typedef otb::ImageFileReader<InputImageType>  ReaderType;
      typedef otb::ImageFileWriter<InputImageType> WriterType;

      ReaderType::Pointer input = ReaderType::New();
      WriterType::Pointer slotWriter = WriterType::New();
      WriterType::Pointer aspectWriter = WriterType::New();
      WriterType::Pointer incidenceWriter = WriterType::New();
      WriterType::Pointer exitanceWriter = WriterType::New();
      DEMCaracteristicsExtractorType::Pointer filter = DEMCaracteristicsExtractorType::New();

      input->SetFileName(inputFileName);
      slotWriter->SetFileName(outputSlotFileName);
      aspectWriter->SetFileName(outputAspectFileName);
      incidenceWriter->SetFileName(outputIncidenceFileName);
      exitanceWriter->SetFileName(outputExitanceFileName);
      filter->SetInput(input->GetOutput());
      filter->SetViewAzimut(static_cast<double>(atof(argv[2])));
      filter->SetViewAngle(static_cast<double>(atof(argv[3])));
      filter->SetSolarAzimut(static_cast<double>(atof(argv[4])));
      filter->SetSolarAngle(static_cast<double>(atof(argv[5])));

      slotWriter->SetInput(filter->GetSlopOutput());
      aspectWriter->SetInput(filter->GetAspectOutput());
      incidenceWriter->SetInput(filter->GetIncidenceOutput());
      exitanceWriter->SetInput(filter->GetExitanceOutput());

      slotWriter->Update();
      aspectWriter->Update();
      incidenceWriter->Update();
      exitanceWriter->Update();
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
