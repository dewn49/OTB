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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {MeanImageFilterOutput.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  The \doxygen{itk}{MeanImageFilter} is commonly used for noise
//  reduction. The filter computes the value of each output pixel by
//  finding the statistical mean of the neighborhood of the
//  corresponding input pixel.  The following figure illustrates the
//  local effect of the MeanImageFilter. The statistical mean of the
//  neighborhood on the left is passed as the output value associated
//  with the pixel at the center of the neighborhood.
//
//  \begin{center}
//  \begin{picture}(200, 46)
//  \put(   5.0,  0.0 ){\framebox(30.0, 15.0){25}}
//  \put(  35.0,  0.0 ){\framebox(30.0, 15.0){30}}
//  \put(  65.0,  0.0 ){\framebox(30.0, 15.0){32}}
//  \put(   5.0, 15.0 ){\framebox(30.0, 15.0){27}}
//  \put(  35.0, 15.0 ){\framebox(30.0, 15.0){25}}
//  \put(  65.0, 15.0 ){\framebox(30.0, 15.0){29}}
//  \put(   5.0, 30.0 ){\framebox(30.0, 15.0){28}}
//  \put(  35.0, 30.0 ){\framebox(30.0, 15.0){26}}
//  \put(  65.0, 30.0 ){\framebox(30.0, 15.0){50}}
//  \put( 100.0, 22.0 ){\vector(1, 0){20.0}}
//  \put( 125.0, 15.0 ){\framebox(34.0, 15.0){30.22}}
//  \put( 160.0, 22.0 ){\vector(1, 0){20.0}}
//  \put( 185.0, 15.0 ){\framebox(30.0, 15.0){30}}
//  \end{picture}
//  \end{center}
//
//  Note that this algorithm is sensitive to the presence of outliers in the
//  neighborhood.  This filter will work on images of any dimension thanks to
//  the internal use of \doxygen{itk}{SmartNeighborhoodIterator} and
//  \doxygen{itk}{NeighborhoodOperator}.  The size of the neighborhood over which
//  the mean is computed can be set by the user.
//
//  \index{itk::MeanImageFilter}
//
//  Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginLatex
//
//  The header file corresponding to this filter should be included first.
//
//  \index{itk::MeanImageFilter!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkMeanImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile   outputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Then the pixel types for input and output image must be defined and, with
  //  them, the image types can be instantiated.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef   unsigned char InputPixelType;
  typedef   unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  //  Software Guide : BeginLatex
  //
  //  Using the image types it is now possible to instantiate the filter type
  //  and create the filter object.
  //
  //  \index{itk::MeanImageFilter!instantiation}
  //  \index{itk::MeanImageFilter!New()}
  //  \index{itk::MeanImageFilter!Pointer}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::MeanImageFilter<
      InputImageType, OutputImageType>  FilterType;

  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The size of the neighborhood is defined along every dimension by
  //  passing a \code{SizeType} object with the corresponding values. The
  //  value on each dimension is used as the semi-size of a rectangular
  //  box. For example, in $2D$ a size of \(1, 2\) will result in a $3 \times
  //  5$ neighborhood.
  //
  //  \index{itk::MeanImageFilter!Radius}
  //  \index{itk::MeanImageFilter!Neighborhood}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  InputImageType::SizeType indexRadius;

  indexRadius[0] = 1; // radius along x
  indexRadius[1] = 1; // radius along y

  filter->SetRadius(indexRadius);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The input to the filter can be taken from any other filter, for example
  //  a reader. The output can be passed down the pipeline to other filters,
  //  for example, a writer. An update call on any downstream filter will
  //  trigger the execution of the mean filter.
  //
  //  \index{itk::MeanImageFilter!SetInput()}
  //  \index{itk::MeanImageFilter!GetOutput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MeanImageFilterOutput.eps}
  // \itkcaption[Effect of the MedianImageFilter]{Effect of the MeanImageFilter.}
  // \label{fig:MeanImageFilterOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:MeanImageFilterOutput} illustrates the effect of this
  //  filter using neighborhood radii of
  //  \(1, 1\) which corresponds to a $ 3 \times 3 $ classical neighborhood.
  //  It can be seen from this picture that edges are rapidly degraded by the
  //  diffusion of intensity values among neighbors.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
